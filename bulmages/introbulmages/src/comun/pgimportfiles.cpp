#include <qdatetime.h>
#include "pgimportfiles.h"
#include <qtextstream.h>

#define EURO  166.386

#define LEN_CODIGO_CUENTA 12
#define LEN_TITULO  40
#define LEN_NIF  15
#define LEN_DOMICILIO  35
#define LEN_POBLACION  25
#define LEN_PROVINCIA  20
#define LEN_CODPOSTAL  5
#define LEN_DIVISA  1
#define LEN_CTA_CODDIVISA  5
#define LEN_CTA_DOCUMENTO  1
#define LEN_AJUSTAME  1
#define LEN_TIPOIVA  1


#define LEN_ASIEN  6
#define LEN_FECHA  8
#define LEN_SUBCTA  12
#define LEN_CONTRA  12
#define LEN_PTADEBE  16
#define LEN_CONCEPTO  25
#define LEN_PTAHABER  16
#define LEN_FACTURA  8
#define LEN_BASEIMPO  16
#define LEN_IVA  5
#define LEN_RECEQUIV  5
#define LEN_DOCUMENTO  10
#define LEN_DEPARTA  3
#define LEN_CLAVE  6
#define LEN_ESTADO  1
#define LEN_NCASADO  6
#define LEN_TCASADO  1
#define LEN_TRANS  6
#define LEN_CAMBIO  16
#define LEN_DEBEME  16
#define LEN_HABERME  16
#define LEN_AUXILIAR  1
#define LEN_SERIE  1
#define LEN_SUCURSAL  4
#define LEN_CODDIVISA  5
#define LEN_IMPAUXME  16
#define LEN_MONEDAUSO  1
#define LEN_EURODEBE  16
#define LEN_EUROHABER  16
#define LEN_BASEEURO  16
#define LEN_NOCONV  1
#define LEN_NUMEROINV  10


pgimportfiles::pgimportfiles(postgresiface2 *con) {
	conexionbase = con;
}// end pgimportfiles

int pgimportfiles::contaplus2Bulmages(QFile &subcuentas, QFile &asientos) {
    QString idasiento;
    QString lopd_str, cuenta_str;
    QString debe, haber;
    QString query;

    // Subcuentas
    QTextStream stream( &subcuentas );
    while( !subcuentas.atEnd() ) {
			QString line = stream.readLine();
			if( line.length()<2 )
	  		break;
			int pos = 0;
			QString cod = line.mid(pos,LEN_CODIGO_CUENTA).stripWhiteSpace();
			pos += LEN_CODIGO_CUENTA;
			QString titulo = line.mid(pos,LEN_TITULO).stripWhiteSpace();
			pos += LEN_TITULO;
			QString nif = line.mid(pos,LEN_NIF).stripWhiteSpace();
			pos += LEN_NIF;
			QString domicilio = line.mid(pos,LEN_DOMICILIO).stripWhiteSpace();
			pos += LEN_DOMICILIO;
			QString poblacion = line.mid(pos,LEN_POBLACION).stripWhiteSpace();
			pos += LEN_POBLACION;
			QString provincia = line.mid(pos,LEN_PROVINCIA).stripWhiteSpace();
			pos += LEN_PROVINCIA;
			QString codpostal = line.mid(pos,LEN_CODPOSTAL).stripWhiteSpace();
			pos += LEN_CODPOSTAL;
			QString divisa = line.mid(pos,LEN_DIVISA).stripWhiteSpace();
			pos += LEN_DIVISA;
			QString cta_coddivisa = line.mid(pos,LEN_CTA_CODDIVISA).stripWhiteSpace();
			pos += LEN_CTA_CODDIVISA;
			QString cta_documento = line.mid(pos,LEN_CTA_DOCUMENTO).stripWhiteSpace();
			pos += LEN_CTA_DOCUMENTO;
			QString ajustame = line.mid(pos,LEN_AJUSTAME).stripWhiteSpace();
			pos += LEN_AJUSTAME;
			QString tipoiva = line.mid(pos,LEN_TIPOIVA).stripWhiteSpace();
			pos += LEN_TIPOIVA;
			/// Antes de hacer una inserción comprobamos que la cuenta no exista ya en el sistema.
			QString query = "SELECT * FROM cuenta WHERE codigo = '"+cod+"'";
			conexionbase->begin();
			cursor2 *cursaux=conexionbase->cargacursor(query,"hol");
			conexionbase->commit();
			if (cursaux->eof()) {
				if( !cod.isEmpty() ) {
					QString padre = searchParent(cod);
					QString idgrupo = cod.left(1);
					query = "INSERT INTO cuenta (imputacion, activo, tipocuenta, codigo, descripcion, cifent_cuenta, padre, idgrupo, nombreent_cuenta, dirent_cuenta, telent_cuenta, coment_cuenta, bancoent_cuenta, emailent_cuenta, webent_cuenta) VALUES  (TRUE, TRUE, 1,'"+cod+"', '"+titulo+"', '"+nif+"', "+padre+", "+idgrupo+", 'importada de ContaPlus','"+domicilio + poblacion+ provincia+codpostal+"','','','','','')";
					conexionbase->begin();
					conexionbase->ejecuta(query);
					conexionbase->commit();
				}// end if
			}// end if
			delete cursaux;
	  }

    QTextStream stream2( &asientos );
    QString lastasiento="0";
    int napunte=0;
    while( !asientos.atEnd() ) {
			QString line = stream2.readLine();
			if( line.length()<2 )
	  		  break;
			int pos=0;
			QString asiento = line.mid(pos,LEN_ASIEN).stripWhiteSpace();
			pos += LEN_ASIEN;
			QString fecha = line.mid(pos,LEN_FECHA).stripWhiteSpace();
			fecha = fecha.mid(0,4) + "-" + fecha.mid(4,2) + "-" + fecha.mid(6,2);
			pos += LEN_FECHA;
			QString subcta = line.mid(pos,LEN_SUBCTA).stripWhiteSpace();
			pos += LEN_SUBCTA;
			QString contra = line.mid(pos,LEN_CONTRA).stripWhiteSpace();
			pos += LEN_CONTRA;
			QString ptadebe = line.mid(pos,LEN_PTADEBE).stripWhiteSpace();
			pos += LEN_PTADEBE;
			QString concepto = line.mid(pos,LEN_CONCEPTO).stripWhiteSpace();
			pos += LEN_CONCEPTO;
			QString ptahaber = line.mid(pos,LEN_PTAHABER).stripWhiteSpace();
			pos += LEN_PTAHABER;
			QString factura = line.mid(pos,LEN_FACTURA).stripWhiteSpace();
			pos += LEN_FACTURA;
			QString baseimpo = line.mid(pos,LEN_BASEIMPO).stripWhiteSpace();
			pos += LEN_BASEIMPO;
			QString iva = line.mid(pos,LEN_IVA).stripWhiteSpace();
			pos += LEN_IVA;
			QString recequiv = line.mid(pos,LEN_RECEQUIV).stripWhiteSpace();
			pos += LEN_RECEQUIV;
			QString documento = line.mid(pos,LEN_DOCUMENTO).stripWhiteSpace();
			pos += LEN_DOCUMENTO;
			QString departa = line.mid(pos,LEN_DEPARTA).stripWhiteSpace();
			pos += LEN_DEPARTA;
			QString clave = line.mid(pos,LEN_CLAVE).stripWhiteSpace();
			pos += LEN_CLAVE;
			QString estado = line.mid(pos,LEN_ESTADO).stripWhiteSpace();
			pos += LEN_ESTADO;
			QString ncasado = line.mid(pos,LEN_NCASADO).stripWhiteSpace();
			pos += LEN_NCASADO;
			QString tcasado = line.mid(pos,LEN_TCASADO).stripWhiteSpace();
			pos += LEN_TCASADO;
			QString trans = line.mid(pos,LEN_TRANS).stripWhiteSpace();
			pos += LEN_TRANS;
			QString cambio = line.mid(pos,LEN_CAMBIO).stripWhiteSpace();
			pos += LEN_CAMBIO;
			QString debeme = line.mid(pos,LEN_DEBEME).stripWhiteSpace();
			pos += LEN_DEBEME;
			QString haberme = line.mid(pos,LEN_HABERME).stripWhiteSpace();
			pos += LEN_HABERME;
			QString auxiliar = line.mid(pos,LEN_AUXILIAR).stripWhiteSpace();
			pos += LEN_AUXILIAR;
			QString serie = line.mid(pos,LEN_SERIE).stripWhiteSpace();
			pos += LEN_SERIE;
			QString sucursal = line.mid(pos,LEN_SUCURSAL).stripWhiteSpace();
			pos += LEN_SUCURSAL;
			QString coddivisa = line.mid(pos,LEN_CODDIVISA).stripWhiteSpace();
			pos += LEN_CODDIVISA;
			QString impauxme = line.mid(pos,LEN_IMPAUXME).stripWhiteSpace();
			pos += LEN_IMPAUXME;
			QString monedauso = line.mid(pos,LEN_MONEDAUSO).stripWhiteSpace();
			pos += LEN_MONEDAUSO;
			QString eurodebe = line.mid(pos,LEN_EURODEBE).stripWhiteSpace();
			pos += LEN_EURODEBE;
			QString eurohaber = line.mid(pos,LEN_EUROHABER).stripWhiteSpace();
			pos += LEN_EUROHABER;
			QString baseeuro = line.mid(pos,LEN_BASEEURO).stripWhiteSpace();
			pos += LEN_BASEEURO;
			QString noconv = line.mid(pos,LEN_NOCONV).stripWhiteSpace();
			pos += LEN_NOCONV;
			QString numeroinv = line.mid(pos,LEN_NUMEROINV).stripWhiteSpace();
			pos += LEN_NUMEROINV;
	
			if( asiento != lastasiento ) {
				if (lastasiento != 0) {
					query = "SELECT cierraasiento("+idasiento+")";
				//	conexionbase->begin();
				//	conexionbase->ejecuta(query); 
				//	conexionbase->commit();
				}// end if
				fprintf(stderr,"Inserción de Asiento");
				query="INSERT INTO asiento (fecha, comentariosasiento, clase) VALUES ('"+fecha+"','Importado de Contaplus', 1 )";
				conexionbase->begin();
				conexionbase->ejecuta(query);
				query = "SELECT max(idasiento) as idasiento FROM asiento";
				cursor2 *cur=conexionbase->cargacursor(query,"lolailo");
				conexionbase->commit();
				idasiento = cur->valor("idasiento");
				delete cur;
				napunte = 0;
				lastasiento = asiento;
			}
			napunte++;
			if( monedauso == "1" ) { // Ptas
				debe = QString::number((ptahaber.toDouble()) / EURO);
				haber = QString::number((ptadebe.toDouble()) / EURO);
			} else {
				debe = QString::number(eurodebe.toDouble());
				haber = QString::number(eurohaber.toDouble());
			}// end if	
			query = "SELECT * FROM cuenta WHERE codigo='"+subcta+"'";
			conexionbase->begin();
			cursor2 *cur=conexionbase->cargacursor(query,"elquery");
			conexionbase->commit();
			if (!cur->eof()) {
				fprintf(stderr,"Inserción de Borrador\n");
				query="INSERT INTO borrador (idasiento,idcuenta,fecha, conceptocontable, debe, haber) VALUES ("+idasiento+",id_cuenta('"+subcta+"'), '"+fecha+"','"+concepto+"',"+debe+","+haber+" )";
				conexionbase->begin();
				conexionbase->ejecuta(query);
				conexionbase->commit();
			}// end if
    }
   return 1;
}

/** **********************************************
  * This function search in the database the account parent of the account selected
  * if there are not parent returns NULL
  */
QString pgimportfiles::searchParent(QString cod) {
	int lencod = cod.length();
	QString padre="NULL"; //almacena el padre de la cuenta.
	QString query;
	int i = 2;
	int fin=0;
	while (!fin) {
		query = "SELECT * FROM cuenta WHERE codigo = '"+cod.left(i)+"'";
		conexionbase->begin();
		cursor2 *cur = conexionbase->cargacursor(query,"unquery");
		conexionbase->commit();
		if (!cur->eof()) {
			padre = cur->valor("idcuenta");
		} else {
			fin=1;
		}// end if
		delete cur;
		i++;
	}// end while
	return padre;
}// end searchParent


