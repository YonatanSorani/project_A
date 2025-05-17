package com.example.spinner;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;
import androidx.core.content.ContextCompat;

import org.json.JSONException;
import org.json.JSONObject;

public class ChoiceActivity extends AppCompatActivity implements  WebSocketMessageListener{
    private CardView moveTop, moveHammer, lunaGraphs, mpuGraphs, vlGraphs ;
    private ImageButton returnToConnect;
    private TextView isTopSpinning ,spinCountText;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.choice);
        moveTop = findViewById(R.id.move_top);
        moveHammer = findViewById(R.id.move_hammer);
        lunaGraphs = findViewById(R.id.luna_graphs);
        mpuGraphs = findViewById(R.id.mpu_graphs);
        vlGraphs = findViewById(R.id.vl_graphs);
        returnToConnect = findViewById(R.id.return_choice);
        isTopSpinning = findViewById(R.id.is_top_spinning);
        spinCountText = findViewById(R.id.spin_count_text);

        // Change text and color based on the boolean value
        if (MainClass.top_is_spinning) {
            isTopSpinning.setText("Spinning...");  // Set text
            isTopSpinning.setTextColor(ContextCompat.getColor(this, R.color.green));  // Set text color
        } else {
            isTopSpinning.setText("Not Spinning");  // Set text
            isTopSpinning.setTextColor(ContextCompat.getColor(this, R.color.red));  // Set text color
        }
        moveTop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ChoiceActivity.this, MoveTopActivity.class);
                startActivity(intent);
            }
        });

        moveHammer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ChoiceActivity.this, operate.class);
                startActivity(intent);
            }
        });

        lunaGraphs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ChoiceActivity.this, LunaActivity.class);
                startActivity(intent);
            }
        });

        mpuGraphs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ChoiceActivity.this, MpuActivity.class);
                startActivity(intent);
            }
        });

        vlGraphs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ChoiceActivity.this, VlActivity.class);
                startActivity(intent);
            }
        });

        returnToConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ChoiceActivity.this, connect.class);
                startActivity(intent);
            }
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
        WebSocketManager.sendMessage(4, "choice"); //send message that the activity has switched
        spinCountText.setText(String.format("%d", MainClass.spinCount));
    }
    @Override
    public void onMessageReceived(String message) {
        try {
            JSONObject jsonResponse = new JSONObject(message);
            String action = jsonResponse.getString("action");
            if (action.equals("failToConnect")) {// Handle connection failure
                runOnUiThread(() -> {
                    Toast.makeText(getBaseContext(), "WebSocket not connected or send failed", Toast.LENGTH_LONG).show();
                });
            } else if(action.equals("statusUpdate")) {
            String status = jsonResponse.getString("status");
            runOnUiThread(() -> {
                if(status.equals("stopSpinning"))
                {
                    MainClass.top_is_spinning = false;
                    isTopSpinning.setText("Not Spinning");  // Set text
                    isTopSpinning.setTextColor(ContextCompat.getColor(ChoiceActivity.this, R.color.red));  // Set text color
                }
                if(status.equals("startSpinning")) {
                    MainClass.top_is_spinning = true;
                    isTopSpinning.setText("Spinning...");  // Set text
                    isTopSpinning.setTextColor(ContextCompat.getColor(ChoiceActivity.this, R.color.green));  // Set text color
                }
                if(status.equals("spinCountUpdate")) {
                    try {
                        MainClass.spinCount =  (int) jsonResponse.getInt("spinCount");//time
                        spinCountText.setText(String.format("%d", MainClass.spinCount));
                    } catch (JSONException e) {
                        throw new RuntimeException(e);
                    }
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

    @Override
    public void onOpenWebSocket() {

    }

    @Override
    public void onClosedWebSocket(String reason) {

    }

    @Override
    public void onFailureWebSocket(String message) {

    }
}