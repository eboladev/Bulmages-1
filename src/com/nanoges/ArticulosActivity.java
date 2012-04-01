package com.nanoges;

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
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;


public class ArticulosActivity extends Activity {
	
	public static int a  =0;
	private ArticulosAdapter adapter;
	public Articulo articulo = null;
	public static ArticulosDataSource adatasource;
	public static String buscador="FALSE";
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.articulos);
        
        /// Buscamos el parametro buscador por si es modo modeless
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	buscador = extras.getString("buscador");
        }
        
		adatasource = new ArticulosDataSource(this);
		adatasource.open();

		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		lv.setTextFilterEnabled(true);
		
		List<Articulo> values = adatasource.getAllArticulos();

		// Use the SimpleCursorAdapter to show the
		// elements in a ListView
		adapter = new ArticulosAdapter(this,	R.layout.articulorow, values);
		
		lv.setAdapter(adapter);
        
		  lv.setOnItemClickListener(new OnItemClickListener() {
		    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
  	 		    Articulo articulo = (Articulo) parent.getItemAtPosition(position);
				Toast.makeText(getApplicationContext(),
						"Click ListItem Number " + position + articulo.toString()+ articulo.getId(), Toast.LENGTH_LONG)
						.show();
		    	
				Toast.makeText(getApplicationContext(),
						"Articulos en modo buscador " + buscador, Toast.LENGTH_LONG)
						.show();
				
				if (buscador.equals("TRUE")) {
					Intent resultIntent = getIntent();
					resultIntent.putExtra("idarticulo", String.valueOf(articulo.getId()));
					setResult(RESULT_OK, resultIntent);
					finish();
				} else {
			       // Creamos la ficha de articulo.
	  		 	   Intent intent = new Intent(view.getContext(), ArticuloActivity.class);
	  		 	   intent.putExtra("idarticulo", String.valueOf(articulo.getId()));
		 		   startActivityForResult(intent,0);
				}
		    }
		  });
    }

	
	public void onBuscar(View view) {
        EditText filtro = (EditText) findViewById(R.id.mui_filtro);
        String cad = filtro.getText().toString();
        
		List<Articulo> values = adatasource.getArticulos(cad);
		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		// = (ArrayAdapter<Cliente>) getListAdapter();
		// Use the SimpleCursorAdapter to show the
		// elements in a ListView
		adapter = new ArticulosAdapter(this,	R.layout.articulorow, values);
		lv.setAdapter(adapter);
	}
    
	private class ArticulosAdapter extends ArrayAdapter<Articulo> {

	    private List<Articulo> items;

	    public ArticulosAdapter(Context context, int textViewResourceId, List<Articulo> items) {
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
	            Articulo o = items.get(position);
	            if (o != null) {
	                    TextView tt = (TextView) v.findViewById(R.id.toptext);
	                    TextView bt = (TextView) v.findViewById(R.id.bottomtext);
	                    if (tt != null) {
	                          //tt.setText("Name: "+o.getOrderName());                            }
	                    	  tt.setText("Name: "+ o.getCodigoCompleto());                            }

	                    if(bt != null){
	                          //bt.setText("Status: "+ o.getOrderStatus());
	                          bt.setText("Status: "+ o.getNom());
	                    }
	            }
	            return v;
	    }
	}
	
    
    
}


