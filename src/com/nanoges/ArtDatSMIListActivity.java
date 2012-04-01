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

public class ArtDatSMIListActivity extends Activity {
	
	public static int a  =0;
	private ArtDatSMIsAdapter adapter;
	public ArtDatSMI articulo = null;
	public ArtDatSMIDataSource cdatasource;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.articulos);
        
		cdatasource = new ArtDatSMIDataSource(this);
		cdatasource.open();
		
		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		lv.setTextFilterEnabled(true);
		

		List<ArtDatSMI> values = cdatasource.getAllArtDatSMIs();

		
	
		adapter = new ArtDatSMIsAdapter(this,	R.layout.clientrow, values);
		lv.setAdapter(adapter);
	
		  lv.setOnItemClickListener(new OnItemClickListener() {
			    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
	  	 		    ArtDatSMI articulo = (ArtDatSMI) parent.getItemAtPosition(position);
					Toast.makeText(getApplicationContext(),
							"Click ListItem Number " + position + articulo.toString()+ articulo.getART_CODIGO(), Toast.LENGTH_LONG)
							.show();
			    	
			       // Creamos la ficha de articulo.

//	  		 	   Intent intent = new Intent(view.getContext(), SupArtDatSMIActivity.class);
//	  		 	   intent.putExtra("CLI_CODIGO", articulo.getCLI_CODIGO());
//		 		   startActivityForResult(intent,0);

			    }
			  });	
    }
    
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void refresh() {
        EditText filtro = (EditText) findViewById(R.id.mui_filtro);
        String cad = filtro.getText().toString();
        
		List<ArtDatSMI> values = cdatasource.getArtDatSMIs(cad);
		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		// = (ArrayAdapter<ArtDatSMI>) getListAdapter();
		// Use the SimpleCursorAdapter to show the
		// elements in a ListView
		adapter = new ArtDatSMIsAdapter(this,	R.layout.clientrow, values);
		lv.setAdapter(adapter);
	}
	
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onClick(View view) {
		// adapter = (ArrayAdapter<ArtDatSMI>) getListAdapter();
		/*
		articulo = null;

			String[] comments = new String[] { "Cool", "Very nice", "Hate it" };
			int nextInt = new Random().nextInt(3);
			// Save the new comment to the database
			articulo = cdatasource.createArtDatSMI("123333",comments[nextInt]);
			adapter.add(articulo);
		
		adapter.notifyDataSetChanged();
		*/
	}
	
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onNuevo(View view) {
		/*
	 	   Intent intent = new Intent(view.getContext(), ArtDatSMIActivity.class);
 		   startActivityForResult(intent,0);
 		*/
	}
	
	public void onBuscar(View view) {
        EditText filtro = (EditText) findViewById(R.id.mui_filtro);
        String cad = filtro.getText().toString();
        
		List<ArtDatSMI> values = cdatasource.getArtDatSMIs(cad);
		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		// = (ArrayAdapter<ArtDatSMI>) getListAdapter();
		// Use the SimpleCursorAdapter to show the
		// elements in a ListView
		adapter = new ArtDatSMIsAdapter(this,	R.layout.clientrow, values);
		lv.setAdapter(adapter);
	}
	
    // Recibimos el identificador de articulo a poner.
    @Override 
    public void onActivityResult(int requestCode, int resultCode, Intent data) {    
    	/*
      super.onActivityResult(requestCode, resultCode, data); 
      Bundle extras = data.getExtras();
      switch(requestCode) { 
        case (0) : { 
          if (resultCode == Activity.RESULT_OK) { 

          } 
          break; 
        } 
      } 
      */
      refresh();
    }
	
	
	private class ArtDatSMIsAdapter extends ArrayAdapter<ArtDatSMI> {

	    private List<ArtDatSMI> items;

	    public ArtDatSMIsAdapter(Context context, int textViewResourceId, List<ArtDatSMI> items) {
	            super(context, textViewResourceId, items);
	            this.items = items;
	    }
	    @Override
	    public View getView(int position, View convertView, ViewGroup parent) {
	            View v = convertView;
	            if (v == null) {
	                LayoutInflater vi = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	                v = vi.inflate(R.layout.clientrow, null);
	            }
	            ArtDatSMI o = items.get(position);
	            if (o != null) {
	                    TextView tt = (TextView) v.findViewById(R.id.toptext);
	                    TextView bt = (TextView) v.findViewById(R.id.bottomtext);
	                    if (tt != null) {
	                          //tt.setText("Name: "+o.getOrderName());                            }
	                    	  tt.setText("Name: "+ o.getART_CODIGO());                            }
	                    if(bt != null){
	                          //bt.setText("Status: "+ o.getOrderStatus());
	                          bt.setText("Status: "+ o.getART_NOMBRE());
	                    }
	            }
	            return v;
	    }
	}
	
}

