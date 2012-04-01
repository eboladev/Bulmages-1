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
import android.widget.ListView;
import android.widget.TextView;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

public class LPedidosActivity extends Activity {
	
	public static int a  =0;
	private LPedidosAdapter adapter;
	public LPedido pedido = null;
	public LPedidosDataSource cdatasource;
    public String midpedidocliente= "0";
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.lpedidos);
        
		cdatasource = new LPedidosDataSource(this);

		
		/// Cogemos el identificador pasado y cargamos la lista 

        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	midpedidocliente = extras.getString("idpedidocliente");
        }

        refresh();

		
    }
    
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void refresh() {
		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		lv.setTextFilterEnabled(true);
		cdatasource.open();
		List<LPedido> values = cdatasource.getLPedidos(midpedidocliente);
		cdatasource.close();
		// = (ArrayAdapter<Cliente>) getListAdapter();
		// Use the SimpleCursorAdapter to show the
		// elements in a ListView
		adapter = new LPedidosAdapter(this,	R.layout.lpedidorow, values);
		lv.setAdapter(adapter);
		
		  lv.setOnItemClickListener(new OnItemClickListener() {
			    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
	  	 		    LPedido lpedido = (LPedido) parent.getItemAtPosition(position);
					Toast.makeText(getApplicationContext(),
							"Linea de Pedido " + position + lpedido.toString()+ lpedido.getId(), Toast.LENGTH_LONG)
							.show();
			       // Creamos la ficha de pedido.
				
	  		 	   Intent intent = new Intent(view.getContext(), LPedidoActivity.class);
	  		 	   intent.putExtra("idlpedidocliente", String.valueOf(lpedido.getId()));
		 		   startActivityForResult(intent,0);
	 		   
			    }
			  });
	}
	
	
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onNuevo(View view) {
		Toast.makeText(getApplicationContext(),
				"Nueva Linea ", Toast.LENGTH_LONG)
				.show();
	 	   Intent intent = new Intent(view.getContext(), LPedidoActivity.class);
		   intent.putExtra("idpedidocliente", midpedidocliente);
 		   startActivityForResult(intent,0);
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
	
	private class LPedidosAdapter extends ArrayAdapter<LPedido> {

	    private List<LPedido> items;

	    public LPedidosAdapter(Context context, int textViewResourceId, List<LPedido> items) {
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
	            LPedido o = items.get(position);
	            if (o != null) {
	                    TextView tt = (TextView) v.findViewById(R.id.toptext);
	                    TextView bt = (TextView) v.findViewById(R.id.bottomtext);
	                    if (tt != null) {
	                          //tt.setText("Name: "+o.getOrderName());                            }
	                    	  tt.setText("Name: "+ o.getCantl());                            }
	                    if(bt != null){
	                          //bt.setText("Status: "+ o.getOrderStatus());
	                          bt.setText("Status: "+ o.getDescl());
	                    }
	            }
	            return v;
	    }
	}
	
    
    
}