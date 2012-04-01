package com.nanoges;

import java.util.ArrayList;
import java.util.List;

//import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

public class ArtDatSMIDataSource {

	// Database fields
	private SQLiteDatabase database;
	private MySQLiteHelper dbHelper;
	private String[] allColumns = { 			
			"ART_CODIGO",
			"ART_NOMBRE",
			"ART_FAMILI",
			"ART_DESUNI",
			"ART_LOTE1U",
			"ART_LOTE2U",
			"ART_TIPIVA",
			"ART_PVENTA",
			"ART_TIPMAT",
			"ART_ENVPRE",
			"ART_DESCL1",
			"ART_TIPART",
			"ART_TASAPV",
			"ART_PROMOC"
};

	public ArtDatSMIDataSource(Context context) {
		dbHelper = new MySQLiteHelper(context);
	}

	public void open() throws SQLException {
		database = dbHelper.getWritableDatabase();
	}

	public void close() {
		dbHelper.close();
	}

	public List<ArtDatSMI> getAllArtDatSMIs() {
		List<ArtDatSMI> clientes = new ArrayList<ArtDatSMI>();
		Cursor cursor = database.query("ArtDatSMI",
				allColumns, null, null, null, null, null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			ArtDatSMI cliente = cursorToArtDatSMI(cursor);
			clientes.add(cliente);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return clientes;
	}

	public List<ArtDatSMI> getArtDatSMIs(String filtro) {
		List<ArtDatSMI> clientes = new ArrayList<ArtDatSMI>();
		Cursor cursor = database.rawQuery("SELECT * FROM ArtDatSMI WHERE nomcliente LIKE '%"+filtro+"%'", null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			ArtDatSMI cliente = cursorToArtDatSMI(cursor);
			clientes.add(cliente);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return clientes;
	}
	
	private ArtDatSMI cursorToArtDatSMI(Cursor cursor) {
		ArtDatSMI articulo = new ArtDatSMI();
		articulo.setArtDatSMI(cursor.getString(0),cursor.getString(1), cursor.getString(2), cursor.getString(3), cursor.getString(4)
				,cursor.getString(5),cursor.getString(6), cursor.getString(7), cursor.getString(8), cursor.getString(9)
				,cursor.getString(10),cursor.getString(11), cursor.getString(12), cursor.getString(13));
		return articulo;
	}
	
	public ArtDatSMI getArtDatSMI(String CODIGO) {
		Cursor cursor = database.rawQuery("SELECT * FROM ArtDatSMI WHERE ART_CODIGO='"+CODIGO+"'", null);
		cursor.moveToFirst();
		ArtDatSMI articulo = cursorToArtDatSMI(cursor);
		cursor.close();
		return articulo;
	}
}