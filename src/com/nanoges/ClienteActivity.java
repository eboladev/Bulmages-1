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
import android.widget.Spinner;

public class ClienteActivity extends Activity {
	public static ClientesDataSource adatasource;
	public Cliente cliente;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.cliente);
        String value= "None";
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	value = extras.getString("idcliente");
        }
        
		adatasource = new ClientesDataSource(this);
		adatasource.open();
		
		if (value.equals("None")) {
			cliente = new Cliente();
		} else {
			cliente = adatasource.getCliente(value);
		}
        
        EditText cifcliente = (EditText) findViewById(R.id.mui_cifcliente);
        cifcliente.setText(cliente.getCif());
        
        EditText nomcliente = (EditText) findViewById(R.id.mui_nomcliente);
        nomcliente.setText(cliente.getNom());     
        
     // ====================000
	    Spinner spinner = (Spinner) findViewById(R.id.spinner1);
	    ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
	            this, R.array.planets_array, android.R.layout.simple_spinner_item);
	    adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
	    spinner.setAdapter(adapter);

// ======================
        
    }
    
    
    public void onAceptar(View view) {
		Toast.makeText(getApplicationContext(),
				"Guardando " + cliente.toString()+ cliente.getId(), Toast.LENGTH_LONG)
				.show();
		
        EditText cifcliente = (EditText) findViewById(R.id.mui_cifcliente);
        EditText nomcliente = (EditText) findViewById(R.id.mui_nomcliente);
        EditText longitud = (EditText) findViewById(R.id.mui_longitud);
        EditText latitud = (EditText) findViewById(R.id.mui_latitud);
        
        cliente.setCliente(cifcliente.getText().toString(), nomcliente.getText().toString(), longitud.getText().toString(), latitud.getText().toString());
        adatasource.saveCliente(cliente);
        finish();
    }
    
    public void onBorrar(View view) {
		Toast.makeText(getApplicationContext(),
				"Borrar " + cliente.toString()+ cliente.getId(), Toast.LENGTH_LONG)
				.show();
		adatasource.deleteCliente(cliente);
		((ClientesActivity)getParent()).refresh();
		finish();
    }
    
    public void onCogerCoordenadas(View view) {
    	EditText longitud = (EditText) findViewById(R.id.mui_longitud);
    	longitud.setText(String.valueOf(Global.lastlocation.getLongitude()));
    	
    	EditText latitud = (EditText) findViewById(R.id.mui_latitud);
    	latitud.setText(String.valueOf(Global.lastlocation.getLatitude()));
    }
    
}

