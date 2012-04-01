package com.nanoges;

import java.util.List;
import java.util.Random;


import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.widget.AdapterView;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

public class PedidosActivity extends Activity {
	
	public static int a  =0;
	public ArrayAdapter<Pedido> adapter;
	public Pedido pedido = null;
	public PedidosDataSource cdatasource;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.pedidos);
        
		cdatasource = new PedidosDataSource(this);
		cdatasource.open();
		
		ListView lv = (ListView) this.findViewById(R.id.mui_list);
		lv.setTextFilterEnabled(true);
		
		/// Cogemos el identificador pasado y cargamos la lista 
        String midcliente= "0";
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	midcliente = extras.getString("idcliente");
        }
		List<Pedido> values = cdatasource.getPedidos(midcliente);

		// = (ArrayAdapter<Cliente>) getListAdapter();
		// Use the SimpleCursorAdapter to show the
		// elements in a ListView
		adapter = new ArrayAdapter<Pedido>(this,	android.R.layout.simple_list_item_1, values);
		lv.setAdapter(adapter);
        



		
		  lv.setOnItemClickListener(new OnItemClickListener() {
			    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
	  	 		    Pedido pedido = (Pedido) parent.getItemAtPosition(position);
					Toast.makeText(getApplicationContext(),
							"Click ListItem Number " + position + pedido.toString()+ pedido.getId(), Toast.LENGTH_LONG)
							.show();
			       // Creamos la ficha de pedido.
	  		 	   Intent intent = new Intent(view.getContext(), SupPedidoActivity.class);
	  		 	   intent.putExtra("idpedidocliente", String.valueOf(pedido.getId()));
		 		   startActivityForResult(intent,0);
			    }
			  });

		
    }
    
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void refresh() {


	}
	
	
	// Will be called via the onClick attribute
	// of the buttons in main.xml
	public void onNuevo(View view) {
		/*
	 	   Intent intent = new Intent(view.getContext(), ClienteActivity.class);
 		   startActivityForResult(intent,0);
 		*/
	}
	
}