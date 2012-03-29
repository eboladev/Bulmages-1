package com.nanoges;

import java.util.List;
import java.util.Random;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Toast;

public class PedidoActivity extends Activity {
	public static PedidosDataSource adatasource;
	public Pedido pedido;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.pedido);
        String value= "None";
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	value = extras.getString("idpedidocliente");
        }
        
		adatasource = new PedidosDataSource(this);
		adatasource.open();
		pedido = adatasource.getPedido(value);
        
        EditText fecha = (EditText) findViewById(R.id.mui_fecha);
        fecha.setText(pedido.getFecha());
        
        EditText desc = (EditText) findViewById(R.id.mui_descripcion);
        desc.setText(pedido.getDesc());        
    }
    
    
    public void onAceptar(View view) {
		Toast.makeText(getApplicationContext(),
				"Guardando " + pedido.toString()+ pedido.getId(), Toast.LENGTH_LONG)
				.show();
		
        EditText fecha = (EditText) findViewById(R.id.mui_fecha);
        EditText desc = (EditText) findViewById(R.id.mui_descripcion);
        /*
        EditText longitud = (EditText) findViewById(R.id.mui_longitud);
        EditText latitud = (EditText) findViewById(R.id.mui_latitud);
        */
        
//        pedido.setPedido(cifpedido.getText().toString(), nompedido.getText().toString(), longitud.getText().toString(), latitud.getText().toString());
        adatasource.savePedido(pedido);
        finish();
    }
    
    public void onBorrar(View view) {
		Toast.makeText(getApplicationContext(),
				"Borrar " + pedido.toString()+ pedido.getId(), Toast.LENGTH_LONG)
				.show();
		adatasource.deletePedido(pedido);
		((PedidosActivity)getParent()).refresh();
		finish();
    }
    
}

