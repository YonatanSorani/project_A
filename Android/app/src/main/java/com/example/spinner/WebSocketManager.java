package com.example.spinner;

import android.widget.Toast;

import org.json.JSONObject;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.WebSocket;
import okhttp3.WebSocketListener;
import okhttp3.Response;


public class WebSocketManager {
    private static WebSocketManager instance;
    private static WebSocket webSocket;
    private static OkHttpClient client;
    private static WebSocketMessageListener listener;

    // Private constructor to prevent instantiation
    private WebSocketManager() {
        client = new OkHttpClient();
    }

    // Singleton getInstance method
    public static WebSocketManager getInstance() {
        if (instance == null) {
            synchronized (WebSocketManager.class) {// only one thread can create the WebSocketManager
                if (instance == null) {
                    instance = new WebSocketManager();
                }
            }
        }
        return instance;
    }

    // Set the listener that handles messages
    public void setMessageListener(WebSocketMessageListener listener) {
        WebSocketManager .listener = listener;
    }

    // Connect to WebSocket
    public void connectToWebSocket() {
        Request request = new Request.Builder()
                .url("ws://192.168.4.1/ws") // Use WebSocket URL
                .build();


        webSocket = client.newWebSocket(request, new WebSocketListener() {
            @Override
            public void onOpen(WebSocket webSocket, Response response) {
                super.onOpen(webSocket, response);
                if (listener != null) {
                    listener.onOpenWebSocket();
                }
            }

            @Override
            public void onMessage(WebSocket webSocket, String text) {
                super.onMessage(webSocket, text);
                if (listener != null) {
                    listener.onMessageReceived(text);
                }
            }

            @Override
            public void onClosed(WebSocket webSocket, int code, String reason) {
                super.onClosed(webSocket, code, reason);
                if (listener != null) {
                    listener.onClosedWebSocket(reason);
                }
            }

            @Override
            public void onFailure(WebSocket webSocket, Throwable t, Response response) {
                super.onFailure(webSocket, t, response);
                if (listener != null) {
                    listener.onFailureWebSocket(t.getMessage());
                }
            }
        });

    }

    public static void sendMessage(int action, String param ) {
        try {
            // Create a new JSON object
            JSONObject jsonMessage = new JSONObject();
            if (action == 1) { // move
                jsonMessage.put("action", "move");
                jsonMessage.put("direction", param);
            } else if(action == 2 ) {  // battery
                jsonMessage.put("action", "battery");
            } else if(action == 3 ) {  // mode on/off
                jsonMessage.put("action", "mode");
                jsonMessage.put("mode", param);
            }
             else if(action == 4 ) {  // mode on/off
                jsonMessage.put("action", "changeActivity");
                jsonMessage.put("activity", param);
            }

            // Send the JSON message over WebSocket
            if (webSocket != null && webSocket.send(jsonMessage.toString())) {
                // Don't update the UI yet, only send the message, update the UI on message received
            } else {
                jsonMessage.put("action", "failToConnect");
                listener.onMessageReceived(jsonMessage.toString());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public boolean isConnected() {
        return webSocket != null && client != null && webSocket.send("ping"); // or other connection check
    }
    // Close WebSocket
    public void closeWebSocket() {
        if (webSocket != null) {
            webSocket.close(1000, "Goodbye!");
        }
        if (client != null) {
            client.dispatcher().executorService().shutdown();
        }
    }
}
