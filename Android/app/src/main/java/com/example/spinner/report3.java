package com.example.spinner;

import static java.lang.Double.parseDouble;
import static java.lang.Thread.sleep;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.androidnetworking.AndroidNetworking;
import com.androidnetworking.error.ANError;
import com.androidnetworking.interfaces.StringRequestListener;
import com.androidplot.xy.XYPlot;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.BaseSeries;
import com.jjoe64.graphview.series.DataPointInterface;
import com.jjoe64.graphview.series.OnDataPointTapListener;
import com.jjoe64.graphview.series.Series;

import java.util.Timer;

public class
report3 extends AppCompatActivity {
    boolean on = false;
    TextView textBattery,point;
    boolean bat=false;
    GraphView angle_graph,distance_graph,z_axis_graph,radial_velocity_graph;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_report3);
        textBattery=findViewById(R.id.textView17);
        textBattery.setGravity(Gravity.RIGHT);
        point=findViewById(R.id.textView20);
        point.setBackgroundColor(Color.GREEN);
        point.setText("real time");


        textBattery.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(on){
                    bat=true;
                }else{
                    AndroidNetworking.post("http://" + MainClass.ip + "/battery")
                            .addQueryParameter("battery", "")
                            .build().getAsString(new StringRequestListener() {
                                @Override
                                public void onResponse(String s) {
                                    textBattery.setText(s + "%");
                                }
                                @Override
                                public void onError(ANError anError) {
                                    Toast.makeText(getApplicationContext(), "connection failed: " + anError, Toast.LENGTH_SHORT).show();
                                }
                            });
                }
            }
        });

    }

    public boolean onOptionsItemSelected(@NonNull MenuItem item){
        int id = item.getItemId();
        if(id == R.id.back){
            Intent connect_act=new Intent(report3.this,connect.class);
            startActivity(connect_act);
            return true;
        }
        else if(id == R.id.operate) {
            Intent operate_act=new Intent(report3.this,operate.class);
            startActivity(operate_act);
            return true;
        }
        else if(id == R.id.report){
            Intent report_act=new Intent(report3.this,report2.class);
            startActivity(report_act);
            return true;
        } else {
            return super.onOptionsItemSelected(item);

        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu){
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu,menu);
        return true;
    }
}