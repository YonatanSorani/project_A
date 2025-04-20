package com.example.spinner;

public interface WebSocketMessageListener {
    void onMessageReceived(String message);
    void onOpenWebSocket();
    void onClosedWebSocket(String reason);
    void onFailureWebSocket(String message);
}
