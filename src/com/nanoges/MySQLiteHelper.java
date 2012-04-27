package com.nanoges;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class MySQLiteHelper extends SQLiteOpenHelper {

	public static final String TABLE_COMMENTS = "comments";
	public static final String COLUMN_ID = "_id";
	public static final String COLUMN_COMMENT = "comment";

	private static final String DATABASE_NAME = "commments.db";
	private static final int DATABASE_VERSION = 2;

	// Database creation sql statement
	private static final String DATABASE_CREATE = "create table "
			+ TABLE_COMMENTS + "( " + COLUMN_ID
			+ " integer primary key autoincrement, " + COLUMN_COMMENT
			+ " text not null);";


	public MySQLiteHelper(Context context) {
		super(context, DATABASE_NAME, null, DATABASE_VERSION);
	}

	@Override
	public void onCreate(SQLiteDatabase database) {
		Log.w(MySQLiteHelper.class.getName(),
				"Creating database version " + DATABASE_VERSION);
		database.execSQL(DATABASE_CREATE);
		database.execSQL(" CREATE TABLE articulo ("
				+ " idarticulo integer primary key autoincrement,"
				+ " codigocompletoarticulo text not null,"
				+ " nomarticulo text not null, " 
				+ " pvparticulo text not null, "
				+ " ivaarticulo text not null);");
		
		database.execSQL(" INSERT INTO articulo (codigocompletoarticulo, nomarticulo, pvparticulo, ivaarticulo )"
				+ " VALUES ('123456', 'Aceite Virgen Extra', '10.00', '18');");
		
		database.execSQL(" INSERT INTO articulo (codigocompletoarticulo, nomarticulo, pvparticulo, ivaarticulo )"
				+ " VALUES ('123457', 'Aceite Virgen', '12.00', '18');");
		
		database.execSQL(" CREATE TABLE cliente ("
				+ " idcliente integer primary key autoincrement,"
				+ " cifcliente text not null,"
				+ " nomcliente text not null," 
				+ " longitudcliente text,"
				+ " latitudcliente text," 
				+ " diasemvisitacliente text,"
				+ " visitaquincenalcliente text);");
		
		database.execSQL(" INSERT INTO cliente (cifcliente, nomcliente )"
				+ " VALUES ('4308444W', 'Tomeu Borras Riera');");
		
		database.execSQL(" INSERT INTO cliente (cifcliente, nomcliente )"
				+ " VALUES ('B57367889', 'Conetxia Soluciones Informáticas');");
		
		database.execSQL(" CREATE TABLE pedidocliente ("
				+ " idpedidocliente integer primary key autoincrement,"
				+ " idcliente integer not null,"
				+ " descpedidocliente text not null," 
				+ " fechapedidocliente date,"
				+ " comentpedidocliente text);");
		
		database.execSQL(" INSERT INTO pedidocliente (idcliente, descpedidocliente, fechapedidocliente, comentpedidocliente )"
				+ " VALUES (1, 'Pedido 1', date('now'), 'Primer pedido de prueba');");
		
		database.execSQL(" CREATE TABLE lpedidocliente ("
				+ " idlpedidocliente integer primary key autoincrement,"
				+ " idpedidocliente integer not null,"
				+ " idarticulo integer not null," 
				+ " cantlpedidocliente integer not null,"
				+ " desclpedidocliente text);");
		
		database.execSQL(" INSERT INTO lpedidocliente (idpedidocliente, idarticulo, cantlpedidocliente, desclpedidocliente )"
				+ " VALUES (1,1, 3, 'Material a pedir');");
		/// Aqui las tablas que va a utilizar DIALMA
		// Familias especiales, solo contiene codigo de familia
		database.execSQL(" CREATE TABLE EspFamSMI ("
				+ " ESP_CODFAM    text primary key);");
		/// Articulos
		database.execSQL(" CREATE TABLE ArtDatSMI ("
				+ " ART_CODIGO text primary key,"
				+ " ART_NOMBRE text,"
				+ " ART_FAMILI text,"
				+ " ART_DESUNI text,"
				+ " ART_LOTE1U integer,"
				+ " ART_LOTE2U integer,"
				+ " ART_TIPIVA integer,"
				+ " ART_PVENTA real,"
				+ " ART_TIPMAT text default '3',"
				+ " ART_ENVPRE real,"
				+ " ART_DESCL1 text,"
				+ " ART_TIPART text default 'N',"
				+ " ART_TASAPV real,"
				+ " ART_PROMOC text);");
		
		/// Insercion de articulos
		database.execSQL(" INSERT INTO ArtDatSMI ("
				+ " ART_CODIGO ,"
				+ " ART_NOMBRE ,"
				+ " ART_FAMILI ,"
				+ " ART_DESUNI ,"
				+ " ART_LOTE1U ,"
				+ " ART_LOTE2U ,"
				+ " ART_TIPIVA ,"
				+ " ART_PVENTA ,"
				+ " ART_TIPMAT ,"
				+ " ART_ENVPRE ,"
				+ " ART_DESCL1 ,"
				+ " ART_TIPART ,"
				+ " ART_TASAPV ,"
				+ " ART_PROMOC ) VALUES ("
				+ " 'ART_CODIGO' ,"
				+ " 'ART_NOMBRE',"
				+ " 'ART_FAMILI' ,"
				+ " 'ART_DESUNI' ,"
				+ " 'ART_LOTE1U' ,"
				+ " 'ART_LOTE2U' ,"
				+ " 'ART_TIPIVA' ,"
				+ " 'ART_PVENTA' ,"
				+ " 'ART_TIPMAT' ,"
				+ " 'ART_ENVPRE' ,"
				+ " 'ART_DESCL1' ,"
				+ " 'ART_TIPART' ,"
				+ " 'ART_TASAPV' ,"
				+ " 'ART_PROMOC' );");
		
		/// Consumos
		database.execSQL(" CREATE TABLE ChcDatSMI ("
				+ " CHC_CODCLI text,"
				+ " CHC_CODART text,"
				+ " CHC_UNIDAD real,"
				+ " CHC_FECULT numeric);");
		
		/// Clientes
		database.execSQL(" CREATE TABLE CliDatSMI ("
				+ " CLI_CODIGO text,"
				+ " CLI_RAZSOC text,"
				+ " CLI_NOMBRE text,"
				+ " CLI_DIRECC text,"
				+ " CLI_POBLAC text,"
				+ " CLI_PROVIN text,"
				+ " CLI_CIFDNI text,"
				+ " CLI_TELEFO text,"
				+ " CLI_FAXTEL text,"
				+ " CLI_TARIFA text,"
				+ " CLI_RECARG text,"
				+ " CLI_DTOCIA real,"
				+ " CLI_DTOPPA real,"
				+ " CLI_ZONA text,"
				+ " CLI_CONTAC text,"
				+ " CLI_CADENA text,"
				+ " CLI_RAPPEL real,"
				+ " CLI_TASAPV text,"
				+ " CLI_TIPVIS text,"
				+ " CLI_DIRBAN text,"
				+ " CLI_FDIREC text,"
				+ " CLI_FPOBLA text,"
				+ " CLI_FPROVI text,"
				+ " CLI_NBANCO text,"
				+ " CLI_OBSERV text,"
				+ " CLI_COMENT text,"
				+ " CLI_EMAIL text,"
				+ " CLI_HORARIO text,"
				+ " CLI_CIA text,"
				+ " CLI_CIERRA text,"
				+ " CLI_DTOS text,"
				+ " CLI_CONDPAG text,"
				+ " CLI_RELMEN text,"
				+ " CLI_RIESGO real,"
				+ " CLI_OBSCOB text);");
	
		/// Inserciones en la tabla de clientes.
		
		/// Clientes
		database.execSQL(" INSERT INTO CliDatSMI ("
				+ " CLI_CODIGO ,"
				+ " CLI_RAZSOC ,"
				+ " CLI_NOMBRE ,"
				+ " CLI_DIRECC ,"
				+ " CLI_POBLAC ,"
				+ " CLI_PROVIN ,"
				+ " CLI_CIFDNI ,"
				+ " CLI_TELEFO ,"
				+ " CLI_FAXTEL ,"
				+ " CLI_TARIFA ,"
				+ " CLI_RECARG ,"
				+ " CLI_DTOCIA ,"
				+ " CLI_DTOPPA ,"
				+ " CLI_ZONA ,"
				+ " CLI_CONTAC ,"
				+ " CLI_CADENA ,"
				+ " CLI_RAPPEL ,"
				+ " CLI_TASAPV ,"
				+ " CLI_TIPVIS ,"
				+ " CLI_DIRBAN ,"
				+ " CLI_FDIREC ,"
				+ " CLI_FPOBLA ,"
				+ " CLI_FPROVI ,"
				+ " CLI_NBANCO ,"
				+ " CLI_OBSERV ,"
				+ " CLI_COMENT ,"
				+ " CLI_EMAIL ,"
				+ " CLI_HORARIO ,"
				+ " CLI_CIA ,"
				+ " CLI_CIERRA ,"
				+ " CLI_DTOS ,"
				+ " CLI_CONDPAG ,"
				+ " CLI_RELMEN ,"
				+ " CLI_RIESGO ,"
				+ " CLI_OBSCOB )" + "VALUES ("
				+ " 'CLI_CODIGO' ,"
				+ " 'CLI_RAZSOC' ,"
				+ " 'CLI_NOMBRE' ,"
				+ " 'CLI_DIRECC' ,"
				+ " 'CLI_POBLAC' ,"
				+ " 'CLI_PROVIN' ,"
				+ " 'CLI_CIFDNI' ,"
				+ " 'CLI_TELEFO' ,"
				+ " 'CLI_FAXTEL' ,"
				+ " 'CLI_TARIFA' ,"
				+ " 'CLI_RECARG' ,"
				+ " 'CLI_DTOCIA' ,"
				+ " 'CLI_DTOPPA' ,"
				+ " 'CLI_ZONA' ,"
				+ " 'CLI_CONTAC' ,"
				+ " 'CLI_CADENA' ,"
				+ " 'CLI_RAPPEL' ,"
				+ " 'CLI_TASAPV' ,"
				+ " 'CLI_TIPVIS' ,"
				+ " 'CLI_DIRBAN' ,"
				+ " 'CLI_FDIREC' ,"
				+ " 'CLI_FPOBLA' ,"
				+ " 'CLI_FPROVI' ,"
				+ " 'CLI_NBANCO' ,"
				+ " 'CLI_OBSERV' ,"
				+ " 'CLI_COMENT' ,"
				+ " 'CLI_EMAIL' ,"
				+ " 'CLI_HORARIO' ,"
				+ " 'CLI_CIA' ,"
				+ " 'CLI_CIERRA' ,"
				+ " 'CLI_DTOS' ,"
				+ " 'CLI_CONDPAG' ,"
				+ " 'CLI_RELMEN' ,"
				+ " 'CLI_RIESGO' ,"
				+ " 'CLI_OBSCOB' );");
		
		/// Descuentos y precios especiales.
		database.execSQL(" CREATE TABLE DcaDatSMI ( "
				+ " DCA_CODCLI text,"
				+ " DCA_ARTFAM text default 'B',"
				+ " DCA_CODART text,"
				+ " DCA_TIPDTO text,"
				+ " DCA_DESCUE real,"
				+ " DCA_FECINI numeric,"
				+ " DCA_FECFIN numeric);");
		
		/// Tarifas
		database.execSQL(" CREATE TABLE TarDatSMI ("
				+ " TAR_CODART text,"
				+ " TAR_CODTAR text,"
				+ " TAR_PRECIO real);");
		
		/// Insercion de datos en Tarifas
		database.execSQL(" INSERT INTO TarDatSMI ("
				+ " TAR_CODART ,"
				+ " TAR_CODTAR ,"
				+ " TAR_PRECIO ) VALUES ("
				+ " 'TAR_CODART' ,"
				+ " 'TAR_CODTAR' ,"
				+ " 'TAR_PRECIO');");
		
		/// Recibos pendientes
		database.execSQL(" CREATE TABLE EacDatSMI ("
				+ " EAC_FDOCUM numeric,"
				+ " EAC_NUMDOC text,"
				+ " EAC_CODCLI text,"
				+ " EAC_IMPORT real,"
				+ " EAC_GASTOS real,"
				+ " EAC_FVENCI numeric);");
		
		/// Insercion de datos de ejemplo en Recibos pendientes
		database.execSQL(" INSERT INTO EacDatSMI ("
				+ " EAC_FDOCUM ,"
				+ " EAC_NUMDOC ,"
				+ " EAC_CODCLI ,"
				+ " EAC_IMPORT ,"
				+ " EAC_GASTOS ,"
				+ " EAC_FVENCI ) VALUES ("
				+ " 'EAC_FDOCUM' ,"
				+ " 'EAC_NUMDOC' ,"
				+ " 'EAC_CODCLI' ,"
				+ " 'EAC_IMPORT' ,"
				+ " 'EAC_GASTOS' ,"
				+ " 'EAC_FVENCI');");
		
		// Incidencias para la pda
		database.execSQL(" CREATE TABLE IncDatSMI ("
				+ " INC_CODIGO integer,"
				+ " INC_NOMBRE text);");
		
		// Insercion de Datos de ejemplo en Incidencias para la pda
		database.execSQL(" INSERT INTO IncDatSMI ("
				+ " INC_CODIGO ,"
				+ " INC_NOMBRE ) VALUES (" 
				+ " 'INC_CODIGO' ,"
				+ " 'INC_NOMBRE');");
		
		/// Observaciones para la pda
		database.execSQL(" CREATE TABLE ObsDatSMI ("
				+ " OBS_CODIGO  integer,"
				+ " OBS_NOMBRE text);");
		
		/// Vendedores
		database.execSQL(" CREATE TABLE VenDatSMI ("
				+ " VEN_CODIGO text,"
				+ " VEN_NOMBRE text);");
		
		/// Rutas
		database.execSQL(" CREATE TABLE VisDatSMI ("
				+ " VIS_CODVEN text,"
				+ " VIS_DIASEM text,"
				+ " VIS_NORDEN text,"
				+ " VIS_CODCLI text);");
				
		database.execSQL(" INSERT INTO VisDatSMI (VIS_CODVEN, VIS_DIASEM, VIS_NORDEN, VIS_CODCLI )"
				+ " VALUES ('1', 'X', '1', 'CLI_CODIGO');");
		database.execSQL(" INSERT INTO VisDatSMI (VIS_CODVEN, VIS_DIASEM, VIS_NORDEN, VIS_CODCLI )"
				+ " VALUES ('1', 'S', '2', 'CLI_CODIGO');");
		database.execSQL(" INSERT INTO VisDatSMI (VIS_CODVEN, VIS_DIASEM, VIS_NORDEN, VIS_CODCLI )"
				+ " VALUES ('1', 'L', '3', 'CLI_CODIGO');");
		
		
		/// Familias
		database.execSQL(" CREATE TABLE FamDatSMI ("
				+ " FAM_CODIGO text,"
				+ " FAM_NOMBRE text);");
		
/// DATOS DE ESCRITURA
		/// Incidencias
		database.execSQL(" CREATE TABLE IncDatPDASMI ("
				+ " IDP_PDA integer,"
				+ " IDP_CODIGO integer,"
				+ " IDP_CODCLI text,"
				+ " IDP_NOMBRE text,"
				+ " IDP_FECHA numeric,"
				+ " IDP_EXPORTADO text default '0');");
		
		/// Lineas de pedido
		database.execSQL(" CREATE TABLE LinPedidosPda (" 
				+ " LPC_PDA text,"
				+ " LPC_DIALMA text,"
				+ " LPC_PEDIDO text,"
				+ " LPC_NROLIN integer,"
				+ " LPC_CODART text,"
				+ " LPC_UNIDAD real,"
				+ " LPC_LOTES real,"
				+ " LPC_PROMO text);");
		
		database.execSQL(" INSERT INTO LinPedidosPda (LPC_PDA, LPC_DIALMA, LPC_PEDIDO, LPC_NROLIN, LPC_CODART, LPC_UNIDAD, LPC_LOTES, LPC_PROMO )"
				+ " VALUES ('1', 'CPC_DIALMA', 'CPC_PEDIDO', 1, 'ART_CODIGO', 1, 1, 'P');");
		
		/// Cabeceras de pedido
		database.execSQL(" CREATE TABLE CabPedidosPda ("
				+ " CPC_PDA text,"
				+ " CPC_DIALMA text,"
				+ " CPC_PEDIDO text,"
				+ " CPC_CODCLI text,"
				+ " CPC_FECHA numeric,"
				+ " CPC_OBSERV text,"
				+ " CPC_DIASER text,"
				+ " CPC_SERURG text default '1',"
				+ " CPC_SECCIO text,"
				+ " CPC_EXPORTADO text default '1');");
		
		database.execSQL(" INSERT INTO CabPedidosPda (CPC_PDA, CPC_DIALMA, CPC_PEDIDO, CPC_CODCLI, CPC_FECHA, CPC_OBSERV, CPC_DIASER, CPC_SERURG, CPC_SECCIO, CPC_EXPORTADO )"
				+ " VALUES ('1', 'CPC_DIALMA', 'CPC_PEDIDO', 'CLI_CODIGO', 1, 'CPC_OBSERV', 'CPC_DIASER', '1', 'CPC_SECCIO', '0');");
		
		
		/// Cobros
		database.execSQL(" CREATE TABLE logDatPDASMI ("
				+ " IOG_NUMERO text,"
				+ " IOG_NOTERM text,"
				+ " IOG_FECEXP numeric,"
				+ " IOG_ID integer,"
				+ " IOG_CODIGO text,"
				+ " IOG_FECHA numeric,"
				+ " IOG_IMPORTE real,"
				+ " IOG_TIPCOB text,"
				+ " IOG_CODCLI text,"
				+ " IOG_EXPORT text default '1');");	
		
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		Log.w(MySQLiteHelper.class.getName(),
				"Upgrading database from version " + oldVersion + " to "
						+ newVersion + ", which will destroy all old data");
		db.execSQL("DROP TABLE IF EXISTS " + TABLE_COMMENTS);
		db.execSQL("DROP TABLE IF EXISTS articulo");
		db.execSQL("DROP TABLE IF EXISTS cliente");
		db.execSQL("DROP TABLE IF EXISTS pedidocliente");
		db.execSQL("DROP TABLE IF EXISTS lpedidocliente");
		
		db.execSQL("DROP TABLE IF EXISTS EspFamSMI");
		db.execSQL("DROP TABLE IF EXISTS FamDatSMI");
		db.execSQL("DROP TABLE IF EXISTS VisDatSMI");
		db.execSQL("DROP TABLE IF EXISTS VenDatSMI");
		db.execSQL("DROP TABLE IF EXISTS ObsDatSMI");
		db.execSQL("DROP TABLE IF EXISTS IncDatSMI");
		db.execSQL("DROP TABLE IF EXISTS EacDatSMI");
		db.execSQL("DROP TABLE IF EXISTS TarDatSMI");
		db.execSQL("DROP TABLE IF EXISTS DcaDatSMI");
		db.execSQL("DROP TABLE IF EXISTS ChcDatSMI");
		db.execSQL("DROP TABLE IF EXISTS ArtDatSMI");
		db.execSQL("DROP TABLE IF EXISTS logDatPdaSMI");
		db.execSQL("DROP TABLE IF EXISTS CabPedidosPda");
		db.execSQL("DROP TABLE IF EXISTS LinPedidosPda");
		db.execSQL("DROP TABLE IF EXISTS IncDatPDASMI");
		db.execSQL("DROP TABLE IF EXISTS CliDatSMI");
		onCreate(db);
	}

}
