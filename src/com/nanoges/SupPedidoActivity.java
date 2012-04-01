package com.nanoges;


import android.app.TabActivity;
import android.os.Bundle;
import android.content.res.Resources;
import android.content.Context;
import android.widget.TabHost;
import android.widget.Toast;
import android.content.Intent;
import android.location.LocationManager;
import android.location.LocationListener;
import android.location.Location;

public class SupPedidoActivity extends TabActivity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.suppedido);


        // setContentView(R.layout.menuprincipal);
     
        Resources res = getResources(); // Resource object to get Drawables
        TabHost tabHost = getTabHost();  // The activity TabHost
        TabHost.TabSpec spec;  // Resusable TabSpec for each tab
        Intent intent;  // Reusable Intent for each tab
        
        /// Creamos la pantalla de menu principal.
        //intent = new Intent().setClass(this, ClienteActivity.class);
        
        String value= "None";
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	value = extras.getString("idpedidocliente");
        }
        
        // Create an Intent to launch an Activity for the tab (to be reused)
        intent = new Intent().setClass(this, PedidoActivity.class);
	 	intent.putExtra("idpedidocliente", value);
        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("artists").setIndicator("Pedido",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);


        intent = new Intent().setClass(this, LPedidosActivity.class);
	 	intent.putExtra("idpedidocliente", value);
        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("albums").setIndicator("Detalle",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);
       
        tabHost.setCurrentTab(0);
        
    }
    
}
