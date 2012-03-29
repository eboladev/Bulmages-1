package com.nanoges;

import java.util.ArrayList;
import java.util.List;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

public class PedidosDataSource {

	// Database fields
	private SQLiteDatabase database;
	private MySQLiteHelper dbHelper;
	private String[] allColumns = { "idpedidocliente", "idcliente","descpedidocliente", "fechapedidocliente", "comentpedidocliente" };

	public PedidosDataSource(Context context) {
		dbHelper = new MySQLiteHelper(context);
	}

	public void open() throws SQLException {
		database = dbHelper.getWritableDatabase();
	}

	public void close() {
		dbHelper.close();
	}

	public Pedido createPedido(String midcliente, String mdesc, String mfecha, String mcoment) {
		ContentValues values = new ContentValues();
		values.put("idcliente", midcliente);
		values.put("descpedidocliente", mdesc);
		values.put ("fechapedidocliente", mfecha);
		values.put ("comentpedidocliente", mcoment);
		long insertId = database.insert("pedidocliente", null,
				values);
		// To show how to query
		Cursor cursor = database.query("pedidocliente",
				allColumns, "idpedidocliente"+ " = " + insertId, null,
				null, null, null);
		cursor.moveToFirst();
		return cursorToPedido(cursor);
	}

	public void savePedido(Pedido pedido) {
		ContentValues cv = new ContentValues();
		cv.put ("descpedidocliente", pedido.getDesc());
		cv.put ("fechapedidocliente", pedido.getFecha());
		cv.put ("comentpedidocliente", pedido.getComent());
		cv.put ("idcliente", pedido.getIdCliente());
		database.update("pedidocliente", cv, "idpedidocliente = " + String.valueOf(pedido.getId()), null);
		
	}
	
	public void deletePedido(Pedido pedido) {
		long id = pedido.getId();
		System.out.println("Pedido deleted with id: " + id);
		database.delete("pedidocliente", "idpedidocliente"
				+ " = " + id, null);
	}

	public List<Pedido> getAllPedidos() {
		List<Pedido> pedidos = new ArrayList<Pedido>();
		Cursor cursor = database.query("pedidocliente",
				allColumns, null, null, null, null, null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			Pedido pedido = cursorToPedido(cursor);
			pedidos.add(pedido);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return pedidos;
	}

	public List<Pedido> getPedidos(String midcliente) {
		List<Pedido> pedidos = new ArrayList<Pedido>();
//		Cursor cursor = database.rawQuery("SELECT * FROM pedidocliente WHERE descpedidocliente LIKE '%"+filtro+"%'", null);
		Cursor cursor = database.rawQuery("SELECT * FROM pedidocliente WHERE idcliente =  "+midcliente+"", null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			Pedido pedido = cursorToPedido(cursor);
			pedidos.add(pedido);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return pedidos;
	}
	
	private Pedido cursorToPedido(Cursor cursor) {
		Pedido pedido = new Pedido();
		pedido.setId(cursor.getLong(0));
		pedido.setPedido(cursor.getString(1), cursor.getString(2), cursor.getString(3), cursor.getString(4));
		return pedido;
	}
	
	public Pedido getPedido(String id) {
		//Cursor cursor = database.query("articulo", allColumns, null, null, null, null, null);
		Cursor cursor = database.rawQuery("SELECT * FROM pedidocliente WHERE idpedidocliente="+id, null);
		cursor.moveToFirst();
		Pedido pedido = cursorToPedido(cursor);
		cursor.close();
		return pedido;
	}
}
