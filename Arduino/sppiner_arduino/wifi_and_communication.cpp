#include "wifi_and_communication.h"



ACTIVITY current_activity = CONNECT;
//this variable saves the mode status

bool is_left = false;
bool is_right = false;
bool is_forward = false;
bool is_backward = false; 

char Mode[8] = "Off";
char direction[32] = "direction: not picked yet";

vector<AsyncWebSocketClient *> clients;
AsyncWebServer server(80);
AsyncWebSocket web_s("/ws");//for connection managment
//JsonDocument for websocket

char general_output[256];
StaticJsonDocument<512> doc;
StaticJsonDocument<512> responseDoc;

void battery(char* out) {
    unsigned long level = random(0, 100);
    sprintf(out, "%lu", level);
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
    Serial.println(clients.size());
    clients.push_back(client); // Add the client to the list
    
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
    client->close();  // Close the WebSocket connection
    clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end()); // Remove the client from the list
  }else if(type == WS_EVT_DATA) {
    String message = String((char*)data);
    Serial.println("Received WebSocket message");

    // Parse incoming JSON message
    DeserializationError error = deserializeJson(doc, message);

    if (error) {
      Serial.print("Failed to parse JSON: ");
      Serial.println(error.f_str());
      return;
    }
    const char* action = doc["action"];

    if (action == nullptr) {
      Serial.println("Action field is missing");
      responseDoc["action"] = "failToConnect";
    } else {
      if (strcmp(action, "move") == 0) {
        if (doc.containsKey("direction")) {
          // Safely copy the direction from the JSON object to the 'direction' char array
          strncpy(direction, doc["direction"] | "not picked yet", sizeof(direction) - 1);
          direction[sizeof(direction) - 1] = '\0';  // Ensure null-termination
        }       
        Serial.print("Move direction: ");
        Serial.println(direction);

        responseDoc["action"] = "move";
        responseDoc["direction"] = direction;

        // Compare direction using strcmp()
        if (strcmp(direction, "left") == 0) {
            is_left = true;
            motor_u2_left();
        } 
        else if (strcmp(direction, "forward") == 0) {
            is_forward = true;
            motor_u2_forward();
        } 
        else if (strcmp(direction, "backward") == 0) {
            is_backward = true;
            motor_u2_backward();
        } 
        else if (strcmp(direction, "right") == 0) {
            is_right = true;
            motor_u2_right();
        }

      } else if (strcmp(action, "battery") == 0) {
        Serial.println("Battery status requested");
        char battery_str[10];
        battery(battery_str);
        responseDoc["action"] = "battery";
        responseDoc["status"] = battery_str;

      } else if (strcmp(action, "mode") == 0) {
        const char* mode_input = doc["mode"];
        Serial.print("Mode: ");
        Serial.println(mode_input);

        // Default to "On" if mode_input is not provided
        if (mode_input != nullptr && strcmp(mode_input, "Off") == 0) {
            is_left = false;
            is_right = false;
            is_forward = false;
            is_backward = false;

            // Reset the direction to "not picked yet"
            strncpy(direction, "not picked yet", sizeof(direction) - 1);
            direction[sizeof(direction) - 1] = '\0'; // Ensure null termination
            motor_u2_stop();

            // Set Mode to "Off" with strncpy to avoid overflow
            strncpy(Mode, "Off", sizeof(Mode) - 1);
            Mode[sizeof(Mode) - 1] = '\0';  // Ensure null termination
        } else {
            // Default mode is "On"
            strncpy(Mode, "On", sizeof(Mode) - 1);
            Mode[sizeof(Mode) - 1] = '\0';  // Ensure null termination
        }

        // Send the updated mode and direction to the client
        responseDoc["action"] = "mode";
        responseDoc["mode"] = Mode;  // `Mode` is now a char array
        responseDoc["direction"] = direction;

      } else if (strcmp(action, "changeActivity") == 0) {
        Serial.println("changing activity");
        if (doc.containsKey("activity")) {
          // Create a char array to hold the activity value safely
          char activity[32];
          strncpy(activity, doc["activity"].as<const char*>(), sizeof(activity) - 1);
          activity[sizeof(activity) - 1] = '\0'; // Ensure null termination
          if (strcmp(activity, "luna") == 0) {
            current_activity = LUNA;
          } else if (strcmp(activity, "mpu") == 0) {
            current_activity = MPU;
          } else if (strcmp(activity, "vl") == 0) {
           current_activity = VL;
          } else if (strcmp(activity, "choice") == 0) {
            current_activity = CHOICE;
          } else if (strcmp(activity, "connect") == 0) {
            current_activity = CONNECT;
          } else if (strcmp(activity, "operate") == 0) {
            current_activity = OPERATE;
          } else if (strcmp(activity, "sync") == 0) {
            current_activity = SYNC;
          } else if (strcmp(activity, "moveTop") == 0) {
            current_activity = MOVE_TOP;
          }
        }
        responseDoc["action"] = "changeActivity"; 
      }else {
        Serial.println("Unknown action");
        responseDoc["action"] = "failToConnect";
      }
    }

    char response_str[200];
    serializeJson(responseDoc, response_str);
    sendDataToClient(clients[0],response_str);
    responseDoc.clear();
    Serial.println(response_str);
    if( is_right)
    {
      digitalWrite(gpio_led[0], LOW);
    }else{
      digitalWrite(gpio_led[0], HIGH);
    }
    if( is_left)
    {
      digitalWrite(gpio_led[1], LOW);
    }else{
      digitalWrite(gpio_led[1], HIGH);
    }
    if( is_forward)
    {
      digitalWrite(gpio_led[2], LOW);
    }else{
      digitalWrite(gpio_led[2], HIGH);
    }
    if( is_backward)
    {
      digitalWrite(gpio_led[3], LOW);
    }else{
      digitalWrite(gpio_led[3], HIGH);
    }
  }

}

 void sendDataToClient(AsyncWebSocketClient *client, const char *message) {
  if (client != nullptr) {
    client->text(message);
  }
}


void updateGraph(Data dataAll,float mpuTemperature, float GyroZ, float currentEstimatedAngle) {
  switch ( current_activity){
    case LUNA:
      //Serial.println("luna");
      doc["action"] = "data";
      doc["time"] = dataAll.CurrentTime;
      doc["distance"] = dataAll.distanceLuna;
      doc["strength"] = dataAll.strengthLuna;
      doc["temperature"] = dataAll.temperatureLuna;
      serializeJson(doc, general_output);
      if (!clients.empty()) {
        //Serial.println("clients list not empty");
        //send data to the first client in the list
        sendDataToClient(clients[0], general_output);
      }
      doc.clear();
      break;
    case MPU:
      doc["action"] = "data";
      doc["time"] = dataAll.CurrentTime;
      doc["angle"] = currentEstimatedAngle;//dataAll.angle;
      doc["gyro"] = GyroZ;
      doc["count"] = mpuTemperature;
      serializeJson(doc, general_output);
      if (!clients.empty()) {
        //Serial.println("clients list not empty");
        //send data to the first client in the list
        sendDataToClient(clients[0], general_output);
      }
      doc.clear();
      break;
    case VL:
      doc["action"] = "data";
      doc["time"] = dataAll.CurrentTime;
      doc["vl1"] = dataAll.vldistance[0];
      doc["vl2"] = dataAll.vldistance[1];
      doc["vl3"] = dataAll.vldistance[2];
      serializeJson(doc, general_output);
      if (!clients.empty()) {
        //Serial.println("clients list not empty");
        //send data to the first client in the list
        sendDataToClient(clients[0], general_output);
      }
      doc.clear();
      Serial.println("vl");
      break;
    case CHOICE:
      doc["action"] = "statusUpdate";
      doc["status"] = "spinCount";
      doc["count"] = dataAll.rotationCount;
      serializeJson(doc, general_output);
      if (!clients.empty()) {
        //Serial.println("clients list not empty");
        //send data to the first client in the list
        sendDataToClient(clients[0], general_output);
      }
      doc.clear();
      //Serial.println("choice");
      break;
    case CONNECT:
    //Serial.println("connect");
      break;
    case OPERATE:
     //Serial.println("operate");
      break;
    case SYNC:
      //Serial.println("sync");
      break;
    case MOVE_TOP:
      //Serial.println("move top");
      break;
  }
}

// Initialize WiFi
void initWiFi() {
    WiFi.setSleep(false);
    WiFi.setAutoReconnect(true);
    WiFi.softAP(SSID, WIFI_PASSWORD);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    web_s.onEvent(onWebSocketEvent);
    // Serve the WebSocket at /ws
    server.addHandler(&web_s);
    // Start the server
    server.begin();
}

void sendStopSpinningMassege()
{
  doc["action"] = "statusUpdate";//a random special num
  doc["status"] = "stopSpinning";
  serializeJson(doc, general_output);
  if (!clients.empty()) {
    //Serial.println("clients list not empty");
    //send data to the first client in the list
    sendDataToClient(clients[0], general_output);
  }
  doc.clear();
}

void sendStartSpinningMassege()
{
  doc["action"] = "statusUpdate";
  doc["status"] = "startSpinning";//a random special num
  serializeJson(doc, general_output);
  if (!clients.empty()) {
    //Serial.println("clients list not empty");
    //send data to the first client in the list
    sendDataToClient(clients[0], general_output);
  }
  doc.clear();
}





