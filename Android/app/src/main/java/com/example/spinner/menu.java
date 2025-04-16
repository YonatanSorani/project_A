package com.example.spinner;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;

public class menu extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);
    }
    String ip=getIntent().getStringExtra("ip").toString();

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item){
        int id = item.getItemId();
        if(id == R.id.back){
            Intent connect_act=new Intent(menu.this,connect.class);
            startActivity(connect_act);
            return true;
        }
        else if(id == R.id.operate) {
            Intent operate_act=new Intent(menu.this,operate.class);
            startActivity(operate_act);
            return true;
        }
        else if(id == R.id.report){
            Intent report_act=new Intent(menu.this,GraphsActivity.class);
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