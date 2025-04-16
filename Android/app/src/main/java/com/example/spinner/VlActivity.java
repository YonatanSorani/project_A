package com.example.spinner;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

public class VlActivity extends AppCompatActivity implements WebSocketMessageListener{

    private  LineChart[] mcharts = new LineChart[4];
    private ImageButton returnToChoice;
    private TextView isTopSpinning;
    private float[] max = {1,1,1};
    private float[] min = {-1,-1,-1};

    private int[]  mcolors = new int[]{
            Color.rgb(137,230,81), Color.rgb(89,199,250), Color.rgb(250,104,119),Color.rgb(240,230,30)
    };


    private List<List<Entry>> entries = new ArrayList<>();

    private int windowSize = 110; // Number of points to show
    private float time_window = 10;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.vl);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);

        for (int i = 0; i < 3; i++) {
            entries.add(new ArrayList<>());
        }

        mcharts[0] = (LineChart) findViewById(R.id.vl_chart1);
        mcharts[1] = (LineChart) findViewById(R.id.vl_chart2);
        mcharts[2] = (LineChart) findViewById(R.id.vl_chart3);

        for (int i = 0; i < 3; i++) {
            updateGraph(0, 0, i);
        }
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
                Intent intent = new Intent(VlActivity.this, ChoiceActivity.class);
                startActivity(intent);
            }
        });
    }
    private void updateGraph(float newX, float newY,int graph_num) {
        // Add new entry
        entries.get(graph_num).add(new Entry(newX, newY));

        if( max[graph_num] < newY)
        {
            max[graph_num] = newY;
        }
        if( min[graph_num] > newY)
        {
            min[graph_num] = newY;
        }
        // If the number of data points exceeds the window size, remove the oldest one
        if (entries.get(graph_num).size() > windowSize) {
            entries.get(graph_num).remove(0); // Remove the first (oldest) entry
        }
        if (!entries.get(graph_num).isEmpty()) {
            // Create a new LineDataSet with updated entries.get(graph_num)
            LineDataSet dataSet = new LineDataSet(entries.get(graph_num), "Data Label");
            dataSet.setLineWidth(3f);
            dataSet.setCircleRadius(5f);
            dataSet.setCircleHoleRadius(2.5f);
            dataSet.setColor(Color.WHITE);
            dataSet.setCircleColor(Color.WHITE);
            dataSet.setHighLightColor(Color.WHITE);

            dataSet.setDrawValues(false);
            dataSet.setDrawCircles(false);
            dataSet.setMode(LineDataSet.Mode.CUBIC_BEZIER);


            // Create a new LineData and set it to the chart
            LineData lineData = new LineData(dataSet);
            updateChart(mcharts[graph_num],lineData,mcolors[graph_num],newX,max[graph_num],min[graph_num]);
        }
    }
    private void updateChart (LineChart chart, LineData data, int color ,float newX, float max,float min){
        ((LineDataSet) data.getDataSetByIndex(0)).setCircleHoleColor(color);

        chart.getDescription().setEnabled(false); // Enable chart description
        chart.setDrawGridBackground(false);
        chart.setTouchEnabled(true);
        chart.setDragEnabled(true);
        chart.setScaleEnabled(true);
        chart.setPinchZoom(false);

        chart.setBackgroundColor(color);

        chart.getXAxis().setEnabled(true); // Enable X-Axis
        chart.getXAxis().setPosition(XAxis.XAxisPosition.BOTTOM);
        chart.getXAxis().setAxisMinimum(Math.max(newX - time_window,0));
        chart.getXAxis().setAxisMaximum(Math.max(newX,time_window));
        chart.getXAxis().setTextColor(Color.BLACK);
        chart.getXAxis().setGridColor(Color.GRAY);
        chart.getXAxis().setAxisLineColor(Color.BLACK);

        chart.getAxisLeft().setEnabled(true); // Enable Left Y-Axis
        chart.getAxisLeft().setAxisMinimum((float) (min * 1.1));
        chart.getAxisLeft().setAxisMaximum((float) (max * 1.1));
        chart.getAxisLeft().setTextColor(Color.BLACK);
        chart.getAxisLeft().setGridColor(Color.GRAY);
        chart.getAxisLeft().setAxisLineColor(Color.BLACK);

        chart.getAxisRight().setEnabled(false); // Enable Right Y-Axis

        Legend l = chart.getLegend();
        l.setEnabled(false);
        chart.setData(data);

        // Refresh the chart
        chart.notifyDataSetChanged();
        chart.invalidate(); // Redraw the chart
    }
    @Override
    protected void onStart() {
        super.onStart();
        if (!WebSocketManager.getInstance().isConnected()) {
            WebSocketManager.getInstance().connectToWebSocket();
        }
        // Set this activity as the listener to handle WebSocket messages
        WebSocketManager.getInstance().setMessageListener(this);
        Toast.makeText(getBaseContext(), "mpu listener", Toast.LENGTH_LONG).show();

        WebSocketManager.sendMessage(4, "vl"); //send message that the activity has switched
    }

    @Override
    protected void onStop() {
        super.onStop();
        // Optional: Set the listener to null when the activity is paused
        Toast.makeText(getBaseContext(), "listener is null", Toast.LENGTH_LONG).show();
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
                        isTopSpinning.setTextColor(ContextCompat.getColor(VlActivity.this, R.color.red));  // Set text color
                    }
                    if(status.equals("startSpinning")) {
                        MainClass.top_is_spinning = true;
                        isTopSpinning.setText("Spinning...");  // Set text
                        isTopSpinning.setTextColor(ContextCompat.getColor(VlActivity.this, R.color.green));  // Set text color
                    }
                });
            }else if (action.equals("data")) {
                float time = (float) jsonResponse.getDouble("time");//time
                float vl1 = (float) jsonResponse.getDouble("vl1");
                float vl2 = (float) jsonResponse.getDouble("vl2");
                float vl3 = (float) jsonResponse.getDouble("vl3");

                // Update the UI with the received direction
                runOnUiThread(() -> {
                    updateGraph( time/1000, vl1, 0);
                    updateGraph(time/1000, vl2, 1);
                    updateGraph(time/1000, vl3, 2);
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
