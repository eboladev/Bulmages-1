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

public class NanoGesActivity extends TabActivity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

		Global.datasource = new CommentsDataSource(this);
		Global.datasource.open();
        
        // setContentView(R.layout.menuprincipal);
     
        Resources res = getResources(); // Resource object to get Drawables
        TabHost tabHost = getTabHost();  // The activity TabHost
        TabHost.TabSpec spec;  // Resusable TabSpec for each tab
        Intent intent;  // Reusable Intent for each tab
        
        /// Creamos la pantalla de menu principal.
        intent = new Intent().setClass(this, MenuPrincipalActivity.class);
        
        // Create an Intent to launch an Activity for the tab (to be reused)
        intent = new Intent().setClass(this, ArtistsActivity.class);

        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("artists").setIndicator("Menu",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);


        intent = new Intent().setClass(this, AlbumsActivity.class);

        // Initialize a TabSpec for each tab and add it to the TabHost
        spec = tabHost.newTabSpec("albums").setIndicator("Sistema",
                          res.getDrawable(R.drawable.ic_tab_artists))
                      .setContent(intent);
        tabHost.addTab(spec);
        
        tabHost.setCurrentTab(0);
        
        habilitarGPS();
    }
    
    public void habilitarGPS() {
    	// Acquire a reference to the system Location Manager
    	LocationManager locationManager = (LocationManager) this.getSystemService(Context.LOCATION_SERVICE);

    	// Define a listener that responds to location updates
    	LocationListener locationListener = new LocationListener() {
    		
    	    public void onLocationChanged(Location location) {
    	      // Called when a new location is found by the network location provider.
    	      //makeUseOfNewLocation(location);
    			Toast.makeText(getApplicationContext(),
    					"Posicion: " + location.toString(), Toast.LENGTH_LONG)
    					.show();
    			Global.lastlocation = location;
    	    }

    	    public void onStatusChanged(String provider, int status, Bundle extras) {}

    	    public void onProviderEnabled(String provider) {}

    	    public void onProviderDisabled(String provider) {}
    	  };

    	// Register the listener with the Location Manager to receive location updates
    	locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
    }
    

}