package com.nanoges;


import java.util.List;
import java.util.Random;
import android.app.TabActivity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.TabHost;
import android.widget.Toast;
import android.widget.Spinner;

public class RutaActivity extends TabActivity {
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.ruta);
        String value= "None";
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	value = extras.getString("cli_codigo");
        }
        
		Toast.makeText(getApplicationContext(),
				"Has pulsado sobre " + value, Toast.LENGTH_LONG)
				.show();
		
        Resources res = getResources(); // Resource object to get Drawables
        TabHost tabHost = getTabHost();  // The activity TabHost
        TabHost.TabSpec spec;  // Resusable TabSpec for each tab
        Intent intent;  // Reusable Intent for each tab
        

        // Create an Intent to launch an Activity for the tab (to be reused)
        intent = new Intent().setClass(this, LinPedidosActivity.class);

        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("artists").setIndicator("Pedidos",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);


        intent = new Intent().setClass(this, AlbumsActivity.class);

        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("albums").setIndicator("Cobros",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);
        
        
        intent = new Intent().setClass(this, AlbumsActivity.class);
        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("albums").setIndicator("Comerci.",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);
        
        intent = new Intent().setClass(this, AlbumsActivity.class);
        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("albums").setIndicator("Fiscales",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);

        intent = new Intent().setClass(this, AlbumsActivity.class);
        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("albums").setIndicator("Varios",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);

        intent = new Intent().setClass(this, AlbumsActivity.class);
        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("albums").setIndicator("Incidencia",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);

        intent = new Intent().setClass(this, AlbumsActivity.class);
        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("albums").setIndicator("P. Pendientes",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);

        
        tabHost.setCurrentTab(0);
		
        
    }
    
    
    public void onAceptar(View view) {

        finish();
    }
    
    public void onBorrar(View view) {

		finish();
    }
    
    public void onCogerCoordenadas(View view) {

    }
    
}

