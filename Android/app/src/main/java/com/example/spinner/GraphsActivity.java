package com.example.spinner;

import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.WindowManager;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.util.ArrayList;
import java.util.List;

public class GraphsActivity extends AppCompatActivity {

    private  LineChart[] mcharts = new LineChart[4];

    private int[]  mcolors = new int[]{
            Color.rgb(137,230,81), Color.rgb(240,230,30), Color.rgb(89,199,250), Color.rgb(250,104,119)
    };

    private List<Entry> entries = new ArrayList<>();
    private int windowSize = 50; // Number of points to show
    private float currentX = 0; // Starting X value
    private Handler handler = new Handler(Looper.getMainLooper()); // Handler to run on main thread
    private Runnable updateGraphRunnable;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.graphs);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);

        mcharts[0] = (LineChart) findViewById(R.id.chart1);
        mcharts[1] = (LineChart) findViewById(R.id.chart2);
        mcharts[2] = (LineChart) findViewById(R.id.chart3);
        mcharts[3] = (LineChart) findViewById(R.id.chart4);

        for( int i = 1 ; i < mcharts.length; i++)
        {
            LineData data = getData(36,100 ); // random data
            setUpChart(mcharts[i], data, mcolors[i]);
        }

        updateGraphRunnable = new Runnable() {
            @Override
            public void run() {
                // Simulate receiving new data (you can replace this with actual data)
                float newY = (float) Math.random() * 100; // Simulate random Y value
                updateGraph(currentX, newY);

                // Increment X for the next data point
                currentX++;

                // Repeat the update every 1000 milliseconds (1 second)
                handler.postDelayed(this, 1000);  // Adjust time for your needs
            }
        };
        handler.post(updateGraphRunnable);

    }
    private void updateGraph(float newX, float newY) {
        // Add new entry
        entries.add(new Entry(newX, newY));

        // If the number of data points exceeds the window size, remove the oldest one
        if (entries.size() > windowSize) {
            entries.remove(0); // Remove the first (oldest) entry
        }
        if (!entries.isEmpty()) {
            // Create a new LineDataSet with updated entries
            LineDataSet dataSet = new LineDataSet(entries, "Data Label");
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
            updateChart(mcharts[0],lineData,mcolors[0],newX);
        }

    }

    private LineData getData(int count, int range) {
        ArrayList<Entry> yvals = new ArrayList<>();
        for ( int i =0; i<count ;i++)
        {
            float val = (float) (Math.random()*range) + 3;
            yvals.add(new Entry(i,val));
        }
        LineDataSet set1 = new LineDataSet(yvals," data set");

        set1.setLineWidth(3f);
        set1.setCircleRadius(5f);
        set1.setCircleHoleRadius(2.5f);
        set1.setColor(Color.WHITE);
        set1.setCircleColor(Color.WHITE);
        set1.setHighLightColor(Color.WHITE);

        //set1.setDrawValues(false);
        set1.setDrawCircles(false);
        set1.setMode(LineDataSet.Mode.CUBIC_BEZIER);

        LineData data = new LineData(set1);
        return data;
    }

    private void setUpChart (LineChart chart, LineData data, int color)
    {
        ((LineDataSet) data.getDataSetByIndex(0)).setCircleHoleColor(color);

        chart.getDescription().setEnabled(true); // Enable chart description
        chart.getDescription().setText("Chart Title");
        chart.getDescription().setTextColor(Color.BLACK);
        chart.getDescription().setTextSize(12f);
        chart.getDescription().setPosition(600f, 25f); // Set the title position (x, y)
        chart.setDrawGridBackground(false);

        chart.setTouchEnabled(true);
        chart.setDragEnabled(true);
        chart.setScaleEnabled(true);
        chart.setPinchZoom(false);

        chart.setBackgroundColor(color);

        chart.getXAxis().setEnabled(true); // Enable X-Axis
        chart.getXAxis().setPosition(XAxis.XAxisPosition.BOTTOM);
        chart.getXAxis().setAxisMinimum(0);
        chart.getXAxis().setAxisMaximum(40);
        chart.getXAxis().setTextColor(Color.RED);
        chart.getXAxis().setGridColor(Color.GRAY);
        chart.getXAxis().setAxisLineColor(Color.BLACK);

        chart.getAxisLeft().setEnabled(true); // Enable Left Y-Axis
        chart.getAxisLeft().setAxisMinimum(0);
        chart.getAxisLeft().setAxisMaximum(100);
        chart.getAxisLeft().setTextColor(Color.GREEN);
        chart.getAxisLeft().setGridColor(Color.GRAY);
        chart.getAxisLeft().setAxisLineColor(Color.BLACK);

        chart.getAxisRight().setEnabled(false); // Enable Right Y-Axis

        Legend l = chart.getLegend();
        l.setEnabled(false);
        chart.setData(data);

    }
    private void updateChart (LineChart chart, LineData data, int color ,float newX)
    {
        ((LineDataSet) data.getDataSetByIndex(0)).setCircleHoleColor(color);

        chart.getDescription().setEnabled(true); // Enable chart description
        chart.getDescription().setText("Chart Title");
        chart.getDescription().setTextColor(Color.BLACK);
        chart.getDescription().setTextSize(12f);
        chart.getDescription().setPosition(600f, 25f); // Set the title position (x, y)
        chart.setDrawGridBackground(false);

        chart.setTouchEnabled(true);
        chart.setDragEnabled(true);
        chart.setScaleEnabled(true);
        chart.setPinchZoom(false);

        chart.setBackgroundColor(color);

        chart.getXAxis().setEnabled(true); // Enable X-Axis
        chart.getXAxis().setPosition(XAxis.XAxisPosition.BOTTOM);
        chart.getXAxis().setAxisMinimum(Math.max(newX - windowSize + 1,0));
        chart.getXAxis().setAxisMaximum(Math.max(newX,20));
        chart.getXAxis().setTextColor(Color.RED);
        chart.getXAxis().setGridColor(Color.GRAY);
        chart.getXAxis().setAxisLineColor(Color.BLACK);

        chart.getAxisLeft().setEnabled(true); // Enable Left Y-Axis
        chart.getAxisLeft().setAxisMinimum(0);
        chart.getAxisLeft().setAxisMaximum(100);
        chart.getAxisLeft().setTextColor(Color.GREEN);
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
    protected void onDestroy() {
        super.onDestroy();
        // Make sure to remove any pending updates when the activity is destroyed
        handler.removeCallbacks(updateGraphRunnable);
    }
}
