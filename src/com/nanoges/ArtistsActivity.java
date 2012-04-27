package com.nanoges;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.View;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;

public class ArtistsActivity extends Activity {
	
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.menuprincipal);

    }
    
    

	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onClick(View view) {

		// ============ ALERT DIALOG
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setMessage("Seguro que deseas borrar ?")
		       .setCancelable(false)
		       .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
		           public void onClick(DialogInterface dialog, int id) {
		        	   //TestDatabaseActivity.this.finish();
		           }
		       })
		       .setNegativeButton("No", new DialogInterface.OnClickListener() {
		           public void onClick(DialogInterface dialog, int id) {
		                dialog.cancel();
		           }
		       });
		AlertDialog alert = builder.create();			
		alert.show();
		//============= FIN DEL ALERT DIALOG	
	}

	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onArticulos(View view) {
 	   Intent intent = new Intent(view.getContext(), ArtDatSMIListActivity.class);
	   startActivityForResult(intent,0);

	}
	
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onClientes(View view) {
 	   Intent intent = new Intent(view.getContext(), CliDatSMIListActivity.class);
	   startActivityForResult(intent,0);
	}
	
	
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onRutero(View view) {
 	   Intent intent = new Intent(view.getContext(), RuteroActivity.class);
	   startActivityForResult(intent,0);
	}
	
	
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onMapa(View view) {
	 	   Intent intent = new Intent(view.getContext(), MapaActivity.class);
		   startActivityForResult(intent,0);

	}
	
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onConfiguracion(View view) {
		SQLiteDatabase database;
		MySQLiteHelper dbHelper = new MySQLiteHelper(this);
		database = dbHelper.getWritableDatabase();
		dbHelper.onUpgrade(database,1, 2);

	}
	
}