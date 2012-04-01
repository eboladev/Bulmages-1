package com.nanoges;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.EditText;
import android.widget.Toast;
import android.view.View;

public class ArticuloActivity extends Activity{
	public static ArticulosDataSource adatasource;
	public Articulo articulo;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.articulo);
        String value= "None";
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
        	value = extras.getString("idarticulo");
        }
        
		adatasource = new ArticulosDataSource(this);
		adatasource.open();
		
		if (!value.equals("None")) {
			articulo = adatasource.getArticulo(value);
	        
	        EditText codigocompletoarticulo = (EditText) findViewById(R.id.mui_codigocompletoarticulo);
	        codigocompletoarticulo.setText(articulo.getCodigoCompleto());
	        
	        EditText nomarticulo = (EditText) findViewById(R.id.mui_nomarticulo);
	        nomarticulo.setText(articulo.getNom());
	        
	        EditText pvparticulo = (EditText) findViewById(R.id.mui_pvparticulo);
	        pvparticulo.setText(articulo.getPrecioVenta());
	        
	        EditText ivaarticulo = (EditText) findViewById(R.id.mui_ivaarticulo);
	        ivaarticulo.setText(articulo.getIva());

		} else {
			articulo = adatasource.createArticulo("000000", "*Cambiar*", "1", "18");
		}
    }   
    
    public void onAceptar(View view) {
		Toast.makeText(getApplicationContext(),
				"Guardando " + articulo.toString()+ articulo.getId(), Toast.LENGTH_LONG)
				.show();
        EditText codigocompletoarticulo = (EditText) findViewById(R.id.mui_codigocompletoarticulo);
        EditText nomarticulo = (EditText) findViewById(R.id.mui_nomarticulo);
        EditText pvparticulo = (EditText) findViewById(R.id.mui_pvparticulo);
        EditText ivaarticulo = (EditText) findViewById(R.id.mui_ivaarticulo);
		articulo.setArticulo(codigocompletoarticulo.getText().toString(), nomarticulo.getText().toString(), pvparticulo.getText().toString(), ivaarticulo.getText().toString());
		adatasource.saveArticulo(articulo);
		finish();
    }
    
    public void onBorrar(View view) {
		Toast.makeText(getApplicationContext(),
				"Borrar " + articulo.toString()+ articulo.getId(), Toast.LENGTH_LONG)
				.show();
		adatasource.deleteArticulo(articulo);		
    }
}
