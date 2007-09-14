/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QTextStream>
#include <QMessageBox>
#include <QFile>

#include "asiento1.h"
#include "asiento1view.h"
#include "fixed.h"
#include "funcaux.h"
#include "plugins.h"
#include "empresa.h"
#include "arbol.h"
#include "fixed.h"


Asiento1::Asiento1(Empresa *comp, QWidget *parent) : FichaBc (comp, parent) {
    _depura("Asiento1::Asiento1", 0);
    setTitleName(tr("Asiento Contable"));
    setDBTableName("asiento");
    setDBCampoId("idasiento");
    addDBCampo("idasiento", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Asiento1", "Id asiento"));
    addDBCampo("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate("Asiento1", "Descripcion del asiento"));
    addDBCampo("fecha", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Asiento1", "Fecha del asiento"));
    addDBCampo("comentariosasiento", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Asiento1", "Comentarios del asiento"));
    addDBCampo("ordenasiento", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Asiento1", "Orden de asiento"));
    addDBCampo("clase", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Asiento1", "Tipo de asiento"));
    listalineas = NULL;
    _depura("END Asiento1::Asiento1", 0);
}


Asiento1::~Asiento1() {
    _depura("Asiento1::~Asiento1", 0);
    _depura("END Asiento1::~Asiento1", 0);
}


void Asiento1::pintaidasiento(QString) {
    _depura("funcion no implementada pintaidasiento");
}


void Asiento1::pintadescripcion(QString) {
    _depura("funcion no implementada pintadescripcion");
}


void Asiento1::pintafecha(QString) {
    _depura("funcion no implementada pintafecha");
}


void Asiento1::pintacomentariosasiento(QString) {
    _depura("funcion no implementada pintacomentariosasiento", 2);
}


void Asiento1::pintaordenasiento(QString) {
    _depura("funcion no implementada pintaordenasiento", 2);
}


void Asiento1::pintaclase(QString) {
    _depura("funcion no implementada pintaclase", 2);
}


void Asiento1::calculaypintatotales() {
    _depura("funcion no implementada calculaypintatotales", 2);
}


void Asiento1::trataestadoAsiento1() {
    _depura("funcion no implementada trataestadoAsiento1", 2);
}


void Asiento1::asiento_cierre() {
    _depura("Funcion no implementada", 2);
}


void Asiento1::asiento_apertura() {
    _depura("Funcion no implementada", 2);
}


void Asiento1::asiento_regularizacion(QString finicial, QString ffinal) {
    _depura("Asiento1::regularizacion", 0);
	// Primero, cogemos los saldos de las cuentas usando la clase Arbol
	empresaBase()->begin();
	QString query = "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo";
	cursor2 *ramas;
	ramas = empresaBase()->cargacursor ( query, "Ramas" );
	Arbol *arbol;
	arbol = new Arbol;
	while ( !ramas->eof() )
	{
		if ( atoi ( ramas->valor ( "nivel" ).toAscii().constData() ) == 2 )
		{ /// Cuenta ra&iacute;z.
			arbol->nuevarama ( ramas );
		} // end if
		ramas->siguienteregistro();
	} // end while
	arbol->inicializa ( ramas );
	delete ramas;
	query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes,sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = EXTRACT(year FROM timestamp '"+finicial+"') GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '"+finicial+"' AND fecha <= '"+ffinal+"' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '"+finicial+"' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta WHERE cuenta.codigo SIMILAR TO '6%|7%' ORDER BY codigo";
	cursor2 *hojas;
	hojas = empresaBase()->cargacursor ( query, "Regularizacion" );
	// Para cada cuenta con sus saldos calculados hay que actualizar hojas del &aacute;rbol.
	while ( !hojas->eof() )
	{
		arbol->actualizahojas(hojas);
		hojas->siguienteregistro();
	} // end while

	// Ahora, habrá que introducir apuntes en el nuevo asiento creado para tal fin
	// Para ello, abrimos un asiento con fecha actual y le ponemos sus apuntes con ffinal para obtener los resultados del periodo
	Asiento1View *introapunts2 = empresaBase()->intapuntsempresa2();
	empresaBase()->commit();
	introapunts2->iniciar_asiento_nuevo(); // abrimos el asiento
	QString idasiento = introapunts2->idasiento();
	Fixed debe129 = Fixed("0.00");	// para acumular el debe que tendra la cuenta de regularizacion
	Fixed haber129 = Fixed("0.00"); // para acumular el haber que tendra la cuenta de regularizacion
	Fixed debe, haber;
	listalineas->setinsercion(TRUE);
	listalineas->borrar(listalineas->currentRow());
	SDBRecord *apunte;
	SDBCampo *campo;
	arbol->inicia();
	while(arbol->deshoja(7,FALSE)){
		listalineas->nuevoRegistro();
		apunte = listalineas->lineaact();
		switch (arbol->hojaactual("codigo").left(1).toInt()) {
		case 6:
			haber = arbol->hojaactual("saldo"); // las cuentas del grupo 6 se regularizan por el haber
			debe129 = debe129 + haber; // acumulamos a la contrapartida de regularizacion
			for(int i=0; i < apunte->lista()->size(); ++i){
				campo = (SDBCampo *) apunte->lista()->at(i);
				if(campo->nomcampo() == "fecha")
					campo->set(ffinal);
				else if (campo->nomcampo() == "haber")
					campo->set(haber.toQString());
				else if (campo->nomcampo() == "debe")
					campo->set("0.00");
				else if (campo->nomcampo() == "conceptocontable")
					campo->set("Asiento de Regularizacion");
				else if (campo->nomcampo() == "idcuenta")
					campo->set(arbol->hojaactual("idcuenta"));
				else if (campo->nomcampo() == "idasiento")
					campo->set(idasiento);
			} // end while
			break;
		case 7:
			debe = arbol->hojaactual("saldo"); // las cuentas del grupo 7 se regularizan por el debe
			debe = debe * -1; // cambiamos el signo
			haber129 = haber129 + debe; // acumulamos a la contrapartida de regularizacion
			for(int i=0; i < apunte->lista()->size(); ++i){
				campo = (SDBCampo *) apunte->lista()->at(i);
				if(campo->nomcampo() == "fecha")
					campo->set(ffinal);
				else if (campo->nomcampo() == "debe")
					campo->set(debe.toQString());
				else if (campo->nomcampo() == "haber")
					campo->set("0.00");
				else if (campo->nomcampo() == "conceptocontable")
					campo->set("Asiento de Regularizacion");
				else if (campo->nomcampo() == "idcuenta")
					campo->set(arbol->hojaactual("idcuenta"));
				else if (campo->nomcampo() == "idasiento")
					campo->set(idasiento);
			} // end while
		}
	}
	// Buscamos la cuenta de regularizacion
	empresaBase()->begin();
	query = "select idcuenta, codigo from cuenta where padre in (select idcuenta from cuenta where codigo='129')";
	cursor2 *regularizacion;
	regularizacion = empresaBase()->cargacursor ( query, "Regularizacion" );
	QString idcuenta = regularizacion->valor("idcuenta");
	QString codigo = regularizacion->valor("codigo");
	empresaBase()->commit();

	// Ahora, introducimos los dos ultimos apuntes de regularizacion
	listalineas->nuevoRegistro();
	apunte = listalineas->lineaact();
	for(int i=0; i < apunte->lista()->size(); ++i){
		campo = (SDBCampo *) apunte->lista()->at(i);
		if(campo->nomcampo() == "fecha")
			campo->set(ffinal);
		else if (campo->nomcampo() == "debe")
			campo->set(debe129.toQString());
		else if (campo->nomcampo() == "haber")
			campo->set("0.00");
		else if (campo->nomcampo() == "conceptocontable")
			campo->set("Asiento de Regularizacion");
		else if (campo->nomcampo() == "idcuenta")
			campo->set(idcuenta);
		else if (campo->nomcampo() == "idasiento")
			campo->set(idasiento);
	} // end while
	listalineas->nuevoRegistro();
	apunte = listalineas->lineaact();
	for(int i=0; i < apunte->lista()->size(); ++i){
		campo = (SDBCampo *) apunte->lista()->at(i);
		if(campo->nomcampo() == "fecha")
			campo->set(ffinal);
		else if (campo->nomcampo() == "debe") 
			campo->set("0.00");
		else if (campo->nomcampo() == "haber") 
			campo->set(haber129.toQString());
		else if (campo->nomcampo() == "conceptocontable") 
			campo->set("Asiento de Regularizacion");
		else if (campo->nomcampo() == "idcuenta")
			campo->set(idcuenta);
		else if (campo->nomcampo() == "idasiento")
			campo->set(idasiento);
	} // end while
	//listalineas->setOrdenEnabled(TRUE);
 	listalineas->guardar(); // guardamos los apuntes
	listalineas->setinsercion(FALSE);
	introapunts2->on_mui_cerrarasiento_clicked(); // cerramos el asiento
	introapunts2->asientocerradop(); // y en la ventana de introduccioin de apuntes lo mostramos cerrado
    _depura("END Asiento1::regularizacion", 0);
}


/** Metodo que se encarga del borrado completo de un asiento.
    Devuelve     0: No se ha especificado asiento a borrar.
                 2: Se ha cancelado la accion.
                 3: Se ha borrado correctamente.
                -1: Ha habido algun error.
*/
int Asiento1::borrar(bool atendido) {
    _depura("Asiento1::borrar", 0);
    int error;
    if (DBvalue("idasiento") != "") {
	if (atendido) {
		switch (QMessageBox::warning(0,
			QApplication::translate("Asiento1", "Borrar asiento"),
			QApplication::translate("Asiento1", "Se va a borrar el asiento. Esta seguro?"),
			QMessageBox::Ok,
			QMessageBox::Cancel)) {
		case QMessageBox::Ok: /// Retry clicked or Enter pressed.
		empresaBase()->begin();
		listalineas->borrar();
		error = empresaBase()->ejecuta("DELETE FROM apunte WHERE idasiento = " + DBvalue("idasiento"));
		error += empresaBase()->ejecuta("DELETE FROM asiento WHERE idasiento = " + DBvalue("idasiento"));
		if (error) {
			empresaBase()->rollback();
			return -1;
		} // end if
		empresaBase()->commit();
		vaciar();
		return 3;
		case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
		return 2;
		} // end switch
	} else {
		empresaBase()->begin();
		listalineas->borrar();
		error = empresaBase()->ejecuta("DELETE FROM apunte WHERE idasiento = " + DBvalue("idasiento"));
		error += empresaBase()->ejecuta("DELETE FROM asiento WHERE idasiento = " + DBvalue("idasiento"));
		if (error) {
			empresaBase()->rollback();
			return -1;
		} // end if
		empresaBase()->commit();
		vaciar();
		return 3;
	} // end if
    } // end if
    _depura("END Asiento1::borrar", 0);
    return 0;
}


void Asiento1::vaciar() {
    _depura("Asiento1::vaciar", 0);
    DBclear();

    /// Vaciamos compeltamente el subformulario para que no quede ningun tipo de restos
    listalineas->setColumnCount(0);
    listalineas->setRowCount(0);
    _depura("END Asiento1::vaciar", 0);
}


/** Se encarga del pintado del asiento.
*/
void Asiento1::pintar() {
    _depura("Asiento1::pintar", 0, idasiento());
    pintaidasiento(idasiento());
    pintadescripcion(DBvalue("descripcion"));
    pintafecha(DBvalue("fecha"));
    pintacomentariosasiento(DBvalue("comentariosasiento"));
    pintaordenasiento(DBvalue("ordenasiento"));
    pintaclase(DBvalue("clase"));
    /// Pintamos los totales.
    calculaypintatotales();
    trataestadoAsiento1();
    _depura("END Asiento1::pintar", 0);
}


/// Esta funci&oacute; carga un asiento.
int Asiento1::cargar(QString idasiento) {
    _depura("Asiento1::cargar", 0, idasiento);
    QString query = "SELECT * FROM asiento WHERE idasiento = " + idasiento;
    cursor2 *cur = empresaBase()->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    trataestadoAsiento1();
    listalineas->cargar(idasiento);
    pintar();
    _depura("END Asiento1::cargar", 0, idasiento);
    return 0;
}


Fixed Asiento1::totaldebe(QString idasiento) {
    return listalineas->totaldebe(idasiento);
}


Fixed Asiento1::totalhaber(QString idbudget) {
    return listalineas->totalhaber(idbudget);
}


void Asiento1::abrir() {
    _depura("Asiento1::abreAsiento1", 0);
    if (estadoAsiento1() != ASCerrado)
        return;
    QString id = DBvalue("idasiento");
    if (id == "") {
        _depura("No hay asiento");
        return;
    }
    empresaBase()->abreasiento(id.toInt());
    trataestadoAsiento1();
}


void Asiento1::cerrar() {
    _depura("Asiento1::cierraAsiento1", 0);
    if (estadoAsiento1() != ASAbierto) {
        _depura("asiento no abierto", 0);
        return;
    }
    if (guardar())
        return;
    QString id = DBvalue("idasiento");
    if (id == "") {
        _depura("No hay asiento", 0);
        return;
    }

    cursor2 *cur = empresaBase()->cargacursor("SELECT cierraasiento(" + id + ")");
    delete cur;
    vaciar();
    cargar(id);
    _depura("END Asiento1::cierraasiento1", 0);
}


Asiento1::estadoasiento Asiento1::estadoAsiento1() {
    _depura("Asiento1::estadoasiento", 0);
    if (DBvalue("idasiento") == "")
        return ASVacio;

    QString SQLQuery1 = "SELECT count(idapunte) AS cuenta1 FROM apunte WHERE idasiento = " + DBvalue("idasiento");
    cursor2 *cur1 = empresaBase()->cargacursor(SQLQuery1);
    QString numap = cur1->valor("cuenta1");
    delete cur1;

    QString SQLQuery = "SELECT count(idborrador) AS cuenta FROM borrador WHERE idasiento = " + DBvalue("idasiento");
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
    QString numborr = cur->valor("cuenta");
    delete cur;

    _depura("END Asiento1::estadoasiento", 0, "borradores: " + numborr + " -- apuntes: " + numap);

    if (numborr == "0") {
        return ASVacio;
    } else if (numap != "0") {
        return ASCerrado;
    } else {
        return ASAbierto;
    } // end if
}


int Asiento1::guardar() {
    _depura("Asiento1::guardar", 0);
    QString id;
    empresaBase()->begin();
    try {
        DBsave(id);
        setidasiento(id);
        listalineas->guardar();
        empresaBase()->commit();
        /// Disparamos los plugins
        int res = g_plugins->lanza("Asiento1_guardaAsiento1_post", this);
        if (res != 0)
            return 0;
        _depura("END Asiento1::guardar",0);

        if (estadoAsiento1() == ASCerrado)
            empresaBase()->cierraasiento(id.toInt());
        g_main->statusBar()->showMessage(tr("El asiento se ha guardado correctamente."), 2000);
        return 0;
    } catch (...) {
        _depura("Error guardando, se cancela la operacion", 1);
        empresaBase()->rollback();
        return -1;
    } // end try
}


Empresa *Asiento1::companyact() {
    _depura("Asiento1::companyact", 0);
    _depura("END Asiento1::companyact", 0);
    return empresaBase();
}


void Asiento1::setidasiento(QString val) {
    _depura("Asiento1::setidasiento", 0);
    setDBvalue("idasiento", val);
    listalineas->setColumnValue("idasiento", val);
    _depura("END Asiento1::setidasiento", 0);
}


QString Asiento1::idasiento() {
    _depura("Asiento1::idasiento", 0);
    _depura("END Asiento1::idasiento", 0);
    return DBvalue("idasiento");
}


/// Establece cual es la lista subformulario del presupuesto.
void Asiento1::setListLinAsiento1(ListLinAsiento1View *a) {
    _depura("Asiento1::setListLinAsiento1", 0);
    listalineas = a;
    _depura("END Asiento1::setListLinAsiento1", 0);
}

