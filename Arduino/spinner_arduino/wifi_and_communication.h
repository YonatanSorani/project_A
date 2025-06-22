#ifndef WIFI_AND_COMMUNICATION_H
#define WIFI_AND_COMMUNICATION_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <vector>
#include "set_up.h"
#include "hammers.h"

#define SSID "spinner"
#define WIFI_PASSWORD ""

using namespace std;
enum ACTIVITY {LUNA = 1, MPU = 2, VL = 3, CHOICE = 4, CONNECT = 5, OPERATE = 6, SYNC = 7  , MOVE_TOP = 8};

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void updateApp(Data dataAll);
void initWiFi();
void sendStopSpinningMassege();
void sendStartSpinningMassege();


#endif
