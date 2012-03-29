package com.nanoges;

import java.util.ArrayList;
import java.util.List;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

public class ArticulosDataSource {

	// Database fields
	private SQLiteDatabase database;
	private MySQLiteHelper dbHelper;
	private String[] allColumns = { "idarticulo","codigocompletoarticulo", "nomarticulo", "pvparticulo", "ivaarticulo" };

	public ArticulosDataSource(Context context) {
		dbHelper = new MySQLiteHelper(context);
	}

	public void open() throws SQLException {
		database = dbHelper.getWritableDatabase();
	}

	public void close() {
		dbHelper.close();
	}

	public Articulo createArticulo(String cod, String desc, String mpvp, String miva) {
		ContentValues values = new ContentValues();
		values.put("codigocompletoarticulo", cod);
		values.put("nomarticulo", desc);
		values.put("pvparticulo", mpvp);
		values.put("ivaarticulo", miva);
		long insertId = database.insert("articulo", null,
				values);
		// To show how to query
		Cursor cursor = database.query("articulo",
				allColumns, "idarticulo"+ " = " + insertId, null,
				null, null, null);
		cursor.moveToFirst();
		return cursorToArticulo(cursor);
	}

	public void saveArticulo(Articulo articulo) {
		ContentValues cv = new ContentValues();
		cv.put ("codigocompletoarticulo", articulo.getCodigoCompleto());
		cv.put ("nomarticulo", articulo.getNom());
		cv.put ("pvparticulo", articulo.getPrecioVenta());
		cv.put ("ivaarticulo", articulo.getIva());
		if (articulo.getId() > 0) {
			// Se trata de un update
			database.update("articulo", cv, "idarticulo = " + String.valueOf(articulo.getId()), null);
		} else {
			// Se trata de una insercion
			long insertId = database.insert("articulo", null, cv);
			articulo.setId(insertId);
		}
	}
	
	
	public void deleteArticulo(Articulo articulo) {
		long id = articulo.getId();
		System.out.println("Articulo deleted with id: " + id);
		database.delete("articulos", "idarticulo"
				+ " = " + id, null);
	}

	public List<Articulo> getAllArticulos() {
		List<Articulo> articulos = new ArrayList<Articulo>();
		Cursor cursor = database.query("articulo",
				allColumns, null, null, null, null, null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			Articulo articulo = cursorToArticulo(cursor);
			articulos.add(articulo);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return articulos;
	}

	public List<Articulo> getArticulos(String filtro) {
		List<Articulo> articulos = new ArrayList<Articulo>();
		Cursor cursor = database.rawQuery("SELECT * FROM articulo WHERE nomarticulo LIKE '%"+filtro+"%'", null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			Articulo articulo = cursorToArticulo(cursor);
			articulos.add(articulo);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return articulos;
	}
	
	private Articulo cursorToArticulo(Cursor cursor) {
		Articulo articulo = new Articulo();
		articulo.setId(cursor.getLong(0));
		articulo.setArticulo(cursor.getString(1), cursor.getString(2), cursor.getString(3), cursor.getString(4));
		return articulo;
	}
	
	public Articulo getArticulo(String id) {
		//Cursor cursor = database.query("articulo", allColumns, null, null, null, null, null);
		Cursor cursor = database.rawQuery("SELECT * FROM articulo WHERE idarticulo="+id, null);
		cursor.moveToFirst();
		Articulo articulo = cursorToArticulo(cursor);
		cursor.close();
		return articulo;
	}
}