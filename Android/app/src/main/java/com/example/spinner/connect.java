package com.example.spinner;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class connect extends AppCompatActivity  implements WebSocketMessageListener{
    private Button connect_b;
    private TextView t;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connect);
        connect_b=findViewById(R.id.connect_button);
        t=findViewById(R.id.textView2);

        WebSocketManager.getInstance().setMessageListener(this);
        // Connect to WebSocket
        WebSocketManager.getInstance().connectToWebSocket();
        connect_b.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent_p=new Intent(connect.this, ChoiceActivity.class);
                //intent_p.putExtra("ip",MainClass.ip);
                startActivity(intent_p);
            }
        });

    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (WebSocketManager.getInstance().isConnected()) {
            WebSocketManager.getInstance().closeWebSocket();
        }
        super.onDestroy();
    }
    @Override
    protected void onStart() {
        super.onStart();
        // Set this activity as the listener to handle WebSocket messages
        WebSocketManager.getInstance().setMessageListener(this);
        if (!WebSocketManager.getInstance().isConnected()) {
            WebSocketManager.getInstance().connectToWebSocket();
        }
    }

    @Override
    public void onMessageReceived(String message) {
        //runOnUiThread(() -> Toast.makeText(getBaseContext(), "message" + message, Toast.LENGTH_LONG).show());
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