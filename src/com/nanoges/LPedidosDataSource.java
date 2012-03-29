package com.nanoges;

import java.util.ArrayList;
import java.util.List;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

public class LPedidosDataSource {

	// Database fields
	private SQLiteDatabase database;
	private MySQLiteHelper dbHelper;
	private String[] allColumns = { "idlpedidocliente", "idpedidocliente", "idarticulo","cantlpedidocliente", "desclpedidocliente" };

	public LPedidosDataSource(Context context) {
		dbHelper = new MySQLiteHelper(context);
	}

	public void open() throws SQLException {
		database = dbHelper.getWritableDatabase();
	}

	public void close() {
		dbHelper.close();
	}

	public LPedido createLPedido(String midpedidocliente, String midarticulo, String mcantl, String mdescl) {
		ContentValues values = new ContentValues();
		values.put("idpedidocliente", midpedidocliente);
		values.put("idarticulo", midarticulo);
		values.put ("cantlpedidocliente", mcantl);
		values.put ("desclpedidocliente", mdescl);
		long insertId = database.insert("lpedidocliente", null,
				values);
		// To show how to query
		Cursor cursor = database.query("lpedidocliente",
				allColumns, "idlpedidocliente"+ " = " + insertId, null,
				null, null, null);
		cursor.moveToFirst();
		return cursorToLPedido(cursor);
	}

	public void saveLPedido(LPedido lpedido) {
		ContentValues cv = new ContentValues();
		cv.put ("idpedidocliente", lpedido.getIdPedidoCliente());
		cv.put ("idarticulo", lpedido.getIdArticulo());
		cv.put ("cantlpedidocliente", lpedido.getCantl());
		cv.put ("desclpedidocliente", lpedido.getDescl());
		database.update("lpedidocliente", cv, "idlpedidocliente = " + String.valueOf(lpedido.getId()), null);
		
	}
	
	public void deleteLPedido(LPedido lpedido) {
		long id = lpedido.getId();
		System.out.println("Pedido deleted with id: " + id);
		database.delete("lpedidocliente", "idlpedidocliente"
				+ " = " + id, null);
	}

	public List<LPedido> getAllLPedidos() {
		List<LPedido> lpedidos = new ArrayList<LPedido>();
		Cursor cursor = database.query("lpedidocliente",
				allColumns, null, null, null, null, null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			LPedido lpedido = cursorToLPedido(cursor);
			lpedidos.add(lpedido);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return lpedidos;
	}

	public List<LPedido> getLPedidos(String midpedidocliente) {
		List<LPedido> lpedidos = new ArrayList<LPedido>();
//		Cursor cursor = database.rawQuery("SELECT * FROM pedidocliente WHERE descpedidocliente LIKE '%"+filtro+"%'", null);
		Cursor cursor = database.rawQuery("SELECT * FROM lpedidocliente WHERE idpedidocliente =  "+midpedidocliente+"", null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			LPedido lpedido = cursorToLPedido(cursor);
			lpedidos.add(lpedido);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return lpedidos;
	}
	
	private LPedido cursorToLPedido(Cursor cursor) {
		LPedido lpedido = new LPedido();
		lpedido.setId(cursor.getLong(0));
		lpedido.setLPedido(cursor.getString(1), cursor.getString(2), cursor.getString(3), cursor.getString(4));
		return lpedido;
	}
	
	public LPedido getLPedido(String id) {
		//Cursor cursor = database.query("articulo", allColumns, null, null, null, null, null);
		Cursor cursor = database.rawQuery("SELECT * FROM lpedidocliente WHERE idlpedidocliente="+id, null);
		cursor.moveToFirst();
		LPedido lpedido = cursorToLPedido(cursor);
		cursor.close();
		return lpedido;
	}
}