package com.example.spinner;

import static java.lang.Double.parseDouble;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.VideoView;

import com.androidnetworking.AndroidNetworking;
import com.androidnetworking.error.ANError;
import com.androidnetworking.interfaces.StringRequestListener;

public class report2 extends AppCompatActivity {

    TextView go,battery;
    EditText frq,dur;

    //boolean test = false;

    boolean isNum(String str){
        boolean num=true;
        try {
            parseDouble(str);
        }catch(NumberFormatException e) {
            num =false;
        }
        return num;
    }

    @Override

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_report2);
        String ip=MainClass.ip;
        AndroidNetworking.initialize(getApplicationContext());

        battery = findViewById(R.id.textView19);
        go = findViewById(R.id.go);
        frq = findViewById(R.id.frq);
        dur = findViewById(R.id.dur);


        battery.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AndroidNetworking.post("http://" + ip + "/battery")
                        .addQueryParameter("battery", "")
                        .build().getAsString(new StringRequestListener() {
                            @Override
                            public void onResponse(String s) {
                                battery.setText(s + "%");
                            }

                            @Override
                            public void onError(ANError anError) {
                                return;
                            }
                        });
            }
        });
    }

    public boolean onOptionsItemSelected(@NonNull MenuItem item){
        int id = item.getItemId();
        if(id == R.id.back){
            Intent connect_act=new Intent(report2.this,connect.class);
            startActivity(connect_act);
            return true;
        }
        else if(id == R.id.operate) {
            Intent operate_act=new Intent(report2.this,operate.class);
            startActivity(operate_act);
            return true;
        }
        else if(id == R.id.report){
            Intent report_act=new Intent(report2.this,report2.class);
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