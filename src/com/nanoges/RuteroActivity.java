package com.nanoges;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.widget.AdapterView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.Spinner;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.SimpleCursorAdapter; 
import android.database.Cursor;



public class RuteroActivity extends Activity {
	
	public static int a  =0;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.rutero);
        
        fillData();
		  
        // Now create a simple cursor adapter and set it to display
		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		  lv.setOnItemClickListener(new OnItemClickListener() {
			    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
	  	 		   // Cliente cliente = (Cliente) parent.getItemAtPosition(position);
	  	 		    Cursor cad = (Cursor) parent.getItemAtPosition(position);
	  	 		    
					Toast.makeText(getApplicationContext(),
							"Click ListItem Number " + position + cad.getString(0), Toast.LENGTH_LONG)
							.show();
			    	
			       // Creamos la ficha de cliente.
	
	  		 	   Intent intent = new Intent(view.getContext(), RutaActivity.class);
	  		 	   intent.putExtra("cli_codigo", String.valueOf(cad.getString(0)));
		 		   startActivityForResult(intent,0);
		
			   
			    }
			  });
        
    }

    private void fillData() {
        Cursor c = Global.database.rawQuery("SELECT VIS_CODCLI AS _id, VIS_CODVEN, VIS_NORDEN, VIS_CODCLI, VIS_DIASEM, CLI_NOMBRE FROM VisDatSMI LEFT JOIN (SELECT * FROM CliDatSMI) t1 ON VisDatSMI. VIS_CODCLI = t1.CLI_CODIGO", null);
        ArrayList<String> values = new ArrayList<String>();
        while (c.moveToNext()) {
            values.add(c.getString(c.getColumnIndex("VIS_NORDEN")));
        }
       // c.close();
        
        System.out.println ("Cursor finalizado");
        
        // Now create a simple cursor adapter and set it to display
		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		
        System.out.println ("Cojo el ListView");
        
        TextView tView = new TextView(this);
        tView.setText("This data is retrieved from the database and only 4 " + "of the results are displayed");
        
        lv.addHeaderView(tView);
        
		lv.setTextFilterEnabled(true);
//        ArrayAdapter<String> notes = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, values);
//        lv.setAdapter(notes);
        
        // the desired columns to be bound
        String[] columns = new String[] { "_id", "CLI_NOMBRE", "VIS_NORDEN", "VIS_CODCLI", "VIS_DIASEM" };
        // the XML defined views which the data will be bound to
        int[] to = new int[] { R.id.row, R.id.row1, R.id.row3, R.id.row3, R.id.row4 };
        // create the adapter using the cursor pointing to the desired data as well as the layout information
        SimpleCursorAdapter mAdapter = new SimpleCursorAdapter(this, R.layout.rowpruebas, c, columns, to);
        lv.setAdapter(mAdapter);
        
    }

    
   	
}
    
          