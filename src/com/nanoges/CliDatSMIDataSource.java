package com.nanoges;


import java.util.ArrayList;
import java.util.List;

//import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

public class CliDatSMIDataSource {

	// Database fields
	private SQLiteDatabase database;
	private MySQLiteHelper dbHelper;
	private String[] allColumns = { "CLI_CODIGO","CLI_RAZSOC", "CLI_NOMBRE", "CLI_DIRECC", "CLI_POBLAC" 
			, "CLI_PROVIN", "CLI_CIFDNI", "CLI_TELEFO", "CLI_FAXTEL", "CLI_TARIFA", "CLI_RECARG", "CLI_DTOCIA"
			, "CLI_DTOPPA", "CLI_ZONA", "CLI_CONTAC", "CLI_CADENA", "CLI_RAPPEL", "CLI_TASAPV", "CLI_TIPVIS"
			, "CLI_DIRBAN", "CLI_FDIREC", "CLI_FPOBLA", "CLI_FPROVI", "CLI_NBANCO", "CLI_OBSERV", "CLI_COMENT"
			, "CLI_EMAIL", "CLI_HORARIO", "CLI_CIA", "CLI_CIERRA", "CLI_DTOS", "CLI_CONDPAG", "CLI_RELMEN", "CLI_RIESGO"
			, "CLI_OBSCOB"};

	public CliDatSMIDataSource(Context context) {
		dbHelper = new MySQLiteHelper(context);
	}

	public void open() throws SQLException {
		database = dbHelper.getWritableDatabase();
	}

	public void close() {
		dbHelper.close();
	}

	public List<CliDatSMI> getAllCliDatSMIs() {
		List<CliDatSMI> clientes = new ArrayList<CliDatSMI>();
		Cursor cursor = database.query("CliDatSMI",
				allColumns, null, null, null, null, null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			CliDatSMI cliente = cursorToCliDatSMI(cursor);
			clientes.add(cliente);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return clientes;
	}

	public List<CliDatSMI> getCliDatSMIs(String filtro) {
		List<CliDatSMI> clientes = new ArrayList<CliDatSMI>();
		Cursor cursor = database.rawQuery("SELECT * FROM CliDatSMI WHERE nomcliente LIKE '%"+filtro+"%'", null);
		cursor.moveToFirst();
		while (!cursor.isAfterLast()) {
			CliDatSMI cliente = cursorToCliDatSMI(cursor);
			clientes.add(cliente);
			cursor.moveToNext();
		}
		// Make sure to close the cursor
		cursor.close();
		return clientes;
	}
	
	private CliDatSMI cursorToCliDatSMI(Cursor cursor) {
		CliDatSMI cliente = new CliDatSMI();
		cliente.setCliDatSMI(cursor.getString(0),cursor.getString(1), cursor.getString(2), cursor.getString(3), cursor.getString(4)
				,cursor.getString(5),cursor.getString(6), cursor.getString(7), cursor.getString(8), cursor.getString(9)
				,cursor.getString(10),cursor.getString(11), cursor.getString(12), cursor.getString(13), cursor.getString(14)
				,cursor.getString(15),cursor.getString(16), cursor.getString(17), cursor.getString(18), cursor.getString(19)
				,cursor.getString(20),cursor.getString(21), cursor.getString(22), cursor.getString(23), cursor.getString(24)
				,cursor.getString(25),cursor.getString(26), cursor.getString(27), cursor.getString(28), cursor.getString(29)
				,cursor.getString(30),cursor.getString(31), cursor.getString(32), cursor.getString(33), cursor.getString(34)
				,"0","0");
		return cliente;
	}
	
	public CliDatSMI getCliDatSMI(String CODIGO) {
		//Cursor cursor = database.query("articulo", allColumns, null, null, null, null, null);
		Cursor cursor = database.rawQuery("SELECT * FROM CliDatSMI WHERE CLI_CODIGO='"+CODIGO+"'", null);
		cursor.moveToFirst();
		CliDatSMI cliente = cursorToCliDatSMI(cursor);
		cursor.close();
		return cliente;
	}
}
