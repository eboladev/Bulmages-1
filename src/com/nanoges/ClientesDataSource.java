package com.nanoges;


import java.util.ArrayList;
import java.util.List;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

public class ClientesDataSource {

	// Database fields
	private SQLiteDatabase database;
	private MySQLiteHelper dbHelper;
	private String[] allColumns = { "idcliente","cifcliente", "nomcliente", "longitudcliente", "latitudcliente" };

	public ClientesDataSource(Context context) {
		dbHelper = new MySQLiteHelper(context);
	}

	public void open() throws SQLException {
		database = dbHelper.getWritableDatabase();
	}

	public void close() {
		dbHelper.close();
	}

	public Cliente createCliente(String cif, String nom) {
		ContentValues values = new ContentValues();
		values.put("cifcliente", cif);
		values.put("nomcliente", nom);
		long insertId = database.insert("cliente", null,
				values);
		// To show how to query
		Cursor cursor = database.query("cliente",
				allColumns, "idcliente"+ " = " + insertId, null,
				null, null, null);
		cursor.moveToFirst();
		return cursorToCliente(cursor);
	}

	public void saveCliente(Cliente cliente) {
		ContentValues cv = new ContentValues();
		cv.put ("nomcliente", cliente.getNom());
		cv.put ("cifcliente", cliente.getCif());
		if (cliente.getLongitud() != null && cliente.getLatitud() != null) {
			cv.put ("longitudcliente", cliente.getLongitud());
			cv.put ("latitudcliente", cliente.getLatitud());
		}
		if (cliente.getId() > 0) {
			// Se trata de un update
			database.update("cliente", cv, "idcliente = " + String.valueOf(cliente.getId()), null);
		} else {
			// Se trata de una insercion
			long insertId = database.insert("cliente", null,
					cv);
			cliente.setId(insertId);
		}
	}
	
	public void deleteCliente(Cliente cliente) {
		long id = cliente.getId();
		System.out.println("Cliente deleted with id: " + id);
		database.delete("cliente", "idcliente"
				+ " = " + id, null);
	}

	public List<Cliente> getAllClientes() {
		List<Cliente> clientes = new ArrayList<Cliente>();
		Cursor cursor = database.query("cliente",
				allColumns, null, null, null, null, null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			Cliente cliente = cursorToCliente(cursor);
			clientes.add(cliente);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return clientes;
	}

	public List<Cliente> getClientes(String filtro) {
		List<Cliente> clientes = new ArrayList<Cliente>();
		Cursor cursor = database.rawQuery("SELECT * FROM cliente WHERE nomcliente LIKE '%"+filtro+"%'", null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			Cliente cliente = cursorToCliente(cursor);
			clientes.add(cliente);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return clientes;
	}
	
	private Cliente cursorToCliente(Cursor cursor) {
		Cliente cliente = new Cliente();
		cliente.setId(cursor.getLong(0));
		cliente.setCliente(cursor.getString(1), cursor.getString(2), cursor.getString(3), cursor.getString(4));
		return cliente;
	}
	
	public Cliente getCliente(String id) {
		//Cursor cursor = database.query("articulo", allColumns, null, null, null, null, null);
		Cursor cursor = database.rawQuery("SELECT * FROM cliente WHERE idcliente="+id, null);
		cursor.moveToFirst();
		Cliente cliente = cursorToCliente(cursor);
		cursor.close();
		return cliente;
	}
}
