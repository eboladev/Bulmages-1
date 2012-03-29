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

public class LPedidoActivity extends Activity {
	public static LPedidosDataSource adatasource;
	public static LPedido lpedido;
	public static Articulo articulo;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        
        
        setContentView(R.layout.lpedido);
        
		adatasource = new LPedidosDataSource(this);
		adatasource.open();

        
        String value= "None";
        String idpedidocliente = "None";
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	value = extras.getString("idlpedidocliente");
        	idpedidocliente = extras.getString("idpedidocliente");
        }
        
        if (value != null) {
    		lpedido = adatasource.getLPedido(value);
        } else {
        	lpedido = adatasource.createLPedido(idpedidocliente, "1", "1", "Poner Cantidad");
        }
		
		
		ArticulosDataSource art = new ArticulosDataSource(this);
		art.open();
		articulo = art.getArticulo(lpedido.getIdArticulo());
		
        pintar();
    }

    
    public void pintar() {
        EditText marticulo = (EditText) findViewById(R.id.mui_articulo);
        EditText mui_desclpedidocliente = (EditText) findViewById(R.id.mui_desclpedidocliente);
        EditText mui_cantlpedidocliente = (EditText) findViewById(R.id.mui_cantlpedidocliente);
        marticulo.setText(articulo.getNom());
        mui_desclpedidocliente.setText(lpedido.getDescl());
        mui_cantlpedidocliente.setText(lpedido.getCantl());
    }
    
    public void onAceptar(View view) {
		Toast.makeText(getApplicationContext(),
				"Guardando " + lpedido.toString()+ lpedido.getId(), Toast.LENGTH_LONG)
				.show();

        EditText marticulo = (EditText) findViewById(R.id.mui_articulo);
        EditText mui_desclpedidocliente = (EditText) findViewById(R.id.mui_desclpedidocliente);
        EditText mui_cantlpedidocliente = (EditText) findViewById(R.id.mui_cantlpedidocliente);

        lpedido.setLPedido(lpedido.getIdPedidoCliente(), String.valueOf(articulo.getId()), mui_cantlpedidocliente.getText().toString(), mui_desclpedidocliente.getText().toString());
        adatasource.saveLPedido(lpedido);
        finish();
    }
    
    public void onBorrar(View view) {
		Toast.makeText(getApplicationContext(),
				"Borrar " + lpedido.toString()+ lpedido.getId(), Toast.LENGTH_LONG)
				.show();
		adatasource.deleteLPedido(lpedido);
		finish();
    }

    public void onBuscarArticulo(View view) {
		Toast.makeText(getApplicationContext(),
				"Buscar Articulo " + lpedido.toString()+ lpedido.getId(), Toast.LENGTH_LONG)
				.show();
	 	   Intent intent = new Intent(view.getContext(), ArticulosActivity.class);
		   intent.putExtra("buscador", "TRUE");
 		   startActivityForResult(intent,0);
    }
    
    // Recibimos el identificador de articulo a poner.
    @Override 
    public void onActivityResult(int requestCode, int resultCode, Intent data) {    
      super.onActivityResult(requestCode, resultCode, data); 
      Bundle extras = data.getExtras();
      switch(requestCode) { 
        case (0) : { 
          if (resultCode == Activity.RESULT_OK) { 
                 String m_idarticulo = extras.getString("idarticulo");
         		 Toast.makeText(getApplicationContext(),
        				"Seleccionado Articulo " + m_idarticulo, Toast.LENGTH_LONG)
        				.show();
          // TODO Switch tabs using the index.
         		 
         		ArticulosDataSource art = new ArticulosDataSource(this);
        		art.open();
        		articulo = art.getArticulo(m_idarticulo);
        		
                pintar();
         		 
          } 
          break; 
        } 
      } 
    }

    
}

