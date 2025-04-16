package com.example.spinner;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.content.Intent;
import android.os.Bundle;


import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import org.json.JSONObject;


public class operate extends AppCompatActivity implements WebSocketMessageListener  {
    private TextView battery;
    private Switch OnOff;
    private ImageButton returnToChoice;
    private  TextView isTopSpinning;

    private ImageView backward,forward,left,right;
    static boolean on = false;
    private TextView direction;
    String ip = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_operate);

        forward = findViewById(R.id.forward);
        forward.setImageResource(R.drawable.up);
        backward = findViewById(R.id.backward);
        backward.setImageResource(R.drawable.down);
        left = findViewById(R.id.left);
        left.setImageResource(R.drawable.left);
        right = findViewById(R.id.right);
        right.setImageResource(R.drawable.right);

        OnOff = findViewById(R.id.onOff);
        direction = findViewById(R.id.textView5);
        battery = findViewById(R.id.textView18);

        returnToChoice = findViewById(R.id.return_choice);
        isTopSpinning = findViewById(R.id.is_top_spinning);


        // Change text and color based on the boolean value
        if (MainClass.top_is_spinning) {
            isTopSpinning.setText("Spinning...");  // Set text
            isTopSpinning.setTextColor(ContextCompat.getColor(this, R.color.green));  // Set text color
        } else {
            isTopSpinning.setText("Not Spinning");  // Set text
            isTopSpinning.setTextColor(ContextCompat.getColor(this, R.color.red));  // Set text color
        }

        returnToChoice.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(operate.this, ChoiceActivity.class);
                startActivity(intent);
            }
        });
        ip = MainClass.ip;
        // Connect to WebSocket
        WebSocketManager.getInstance().setMessageListener(this);

        // Handle On/Off switch
        OnOff.setOnCheckedChangeListener((buttonView, isChecked) -> {
            on = isChecked;  // Update the global 'on' boolean based on Switch state
            String mode = isChecked ? "On" : "Off";  // Set mode based on switch state
            WebSocketManager.sendMessage(3, mode);  // Send the mode update to the server
        });

        // Handle forward button click
        forward.setOnClickListener(v -> {
            if(on) {
                WebSocketManager.sendMessage(1,"forward");
            }else{
                Toast.makeText(getBaseContext(), "Mode is off, can't move", Toast.LENGTH_LONG).show();
            }
        });

        // Handle backward button click
        backward.setOnClickListener(v -> {
            if(on) {
                WebSocketManager.sendMessage(1,"backward");
            }else{
                Toast.makeText(getBaseContext(), "Mode is off, can't move", Toast.LENGTH_LONG).show();
            }
        });

        // Handle right button click
        right.setOnClickListener(v -> {
            if(on) {
                WebSocketManager.sendMessage(1,"right");
            }else{
                Toast.makeText(getBaseContext(), "Mode is off, can't move", Toast.LENGTH_LONG).show();
            }
        });

        // Handle left button click
        left.setOnClickListener(v -> {
            if(on) {
                WebSocketManager.sendMessage(1,"left");
            }else{
                Toast.makeText(getBaseContext(), "Mode is off, can't move", Toast.LENGTH_LONG).show();
            }
        });

        // Handle battery click
        battery.setOnClickListener(v -> {
            WebSocketManager.sendMessage(2,"Battery");
        });
    }
    @Override
    protected void onStart() {
        super.onStart();
        if (!WebSocketManager.getInstance().isConnected()) {
            WebSocketManager.getInstance().connectToWebSocket();
        }
        // Set this activity as the listener to handle WebSocket messages
        WebSocketManager.getInstance().setMessageListener(this);
        WebSocketManager.sendMessage(4, "operate"); //send message that the activity has switched

    }
    @Override
    protected void onStop() {
        super.onStop();
        on = false;
    }
    @Override

    public void onMessageReceived(String message) {
        try {
            // Parse the received message as JSON
            JSONObject jsonResponse = new JSONObject(message);
            String action = jsonResponse.getString("action");

            if (action.equals("failToConnect")) {// Handle connection failure
                runOnUiThread(() -> {
                    Toast.makeText(getBaseContext(), "WebSocket not connected or send failed", Toast.LENGTH_LONG).show();
                });
            } else if (action.equals("move")) {// Handle "move" action
                String direction_str = jsonResponse.getString("direction");

                // Update the UI with the received direction
                runOnUiThread(() -> {
                    direction.setText("Choosing direction: " + direction_str);
                });
            } else if (action.equals("battery")) {// Handle "battery" action
                String batteryStatus = jsonResponse.getString("status");

                // Update the UI with the battery status
                runOnUiThread(() -> {
                    battery.setText("Battery: " + batteryStatus + "%");
                });
            } else if (action.equals("mode")) {// Handle "mode" action
                String mode = jsonResponse.getString("mode");
                String direction_str = jsonResponse.getString("direction");
                //  Update the UI based on the received mode
                runOnUiThread(() -> {
                    if (mode.equals("On")) {
                        on = true;
                        direction.setText("Choosing direction: " + direction_str);
                    } else if (mode.equals("Off")) {
                        on = false;
                        direction.setText("Choosing direction: Mode is Off");
                    }
                });
            } else if(action.equals("statusUpdate")) {
                String status = jsonResponse.getString("status");
                runOnUiThread(() -> {
                    if(status.equals("stopSpinning"))
                    {
                        MainClass.top_is_spinning = false;
                        isTopSpinning.setText("Not Spinning");  // Set text
                        isTopSpinning.setTextColor(ContextCompat.getColor(operate.this, R.color.red));  // Set text color
                    }
                    if(status.equals("startSpinning")) {
                        MainClass.top_is_spinning = true;
                        isTopSpinning.setText("Spinning...");  // Set text
                        isTopSpinning.setTextColor(ContextCompat.getColor(operate.this, R.color.green));  // Set text color
                    }
                });
            }
        } catch (Exception e) {
            e.printStackTrace();
            runOnUiThread(() -> {
                Toast.makeText(getBaseContext(), "Error processing response", Toast.LENGTH_LONG).show();
            });
        }
    }
    public void onOpenWebSocket() {
        runOnUiThread(() -> Toast.makeText(getBaseContext(), "WebSocket connected", Toast.LENGTH_LONG).show());
    }
    public void onClosedWebSocket(String reason) {
        runOnUiThread(() -> Toast.makeText(getBaseContext(), "WebSocket closed: " + reason, Toast.LENGTH_LONG).show());
    }
    public void onFailureWebSocket(String message) {
        runOnUiThread(() -> Toast.makeText(getBaseContext(), "WebSocket error" + message, Toast.LENGTH_LONG).show());
    }

}
