package com.example.spinner;

import android.widget.Toast;

public interface WebSocketMessageListener {
    void onMessageReceived(String message);
    void onOpenWebSocket();
    void onClosedWebSocket(String reason);
    void onFailureWebSocket(String message);
}
