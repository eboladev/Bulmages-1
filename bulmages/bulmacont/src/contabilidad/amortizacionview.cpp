//
//
// C++ Implementation: $MODULE$
//
// Description:
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2003
// Copyright: See COPYING file that comes with this distribution
//
//
#include "amortizacionview.h"
#include "empresa.h"
#include "funcaux.h"
#include "aplinteligentesview.h"
#include "busquedacuenta.h"
#include <qpushbutton.h>
#include <qradiobutton.h>
//Added by qt3to4:
#include <Q3PopupMenu>
#include "asiento1view.h"

#define COL_FECHA                  0
#define COL_CUOTA                  1
#define COL_IDASIENTO              2
#define COL_EJERCICIO              3
#define COL_IDLINAMORTIZACION      4
#define COL_ORDENASIENTO           5

amortizacionview::amortizacionview(empresa *emp, QWidget *parent, const char *name )
        : QWidget(parent,name, Qt::WDestructiveClose), dialogChanges(this) {
	_depura("amortizacionview::amortizacionview", 0);
    setupUi(this);
    m_companyact = emp;
    idamortizacion = "";
    ctaactivo->setempresa(emp);
    ctaamortizacion->setempresa(emp);

    /// Buscamos cual es el asiento inteligente que realiza la amortización.
    QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion=valor AND configuracion.nombre='Amortizacion'";
    m_companyact->begin();
    cursor2 *cur = m_companyact->cargacursor(query,"hola");
    m_companyact->commit();
    if (!cur->eof()) {
        idainteligente = cur->valor("idainteligente");
    }// end if
    delete cur;
    //      table1->hideColumn(COL_IDLINAMORTIZACION);
    idamortizacion = "";
    idctaactivo="";
    idctaamortizacion="";

    // Ocultamos los campos que no interesa ver.
    table1->hideColumn(COL_IDASIENTO);
    table1->hideColumn(COL_EJERCICIO);
    table1->hideColumn(COL_IDLINAMORTIZACION);

    m_companyact->meteWindow( caption(), this);
	_depura("END amortizacionview::amortizacionview", 0);

}


void amortizacionview::s_newAmortizacion() {}

void amortizacionview::s_deleteAmortizacion() {
	_depura("amortizacionview::s_deleteAmortizacion", 0);
    trataModificado();
    QString codigo = idamortizacion;
    if (codigo != "") {
        QString query = "DELETE FROM linamortizacion WHERE idamortizacion ="+codigo;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        query = "DELETE FROM amortizacion WHERE idamortizacion="+codigo;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        close(0);
    }// end if
	_depura("END amortizacionview::s_deleteAmortizacion", 0);
}




void amortizacionview::s_saveAmortizacion() {
	_depura("amortizacionview::s_saveAmortizacion", 0);
    QString query;
    QString namortizacion = nomamortizacion->text();
    double valorcompradbl = valorcompra->text().toDouble();
    int numcuotasint = numcuotas->text().toInt();
    QString fechacomprastr = fechacompra->text();
    QString fecha1cuotastr = fecha1cuota->text();
    QString ctaactivostr = ctaactivo->codigocuenta();
    QString ctaamortizacionstr = ctaamortizacion->text();
    QString agrupacionstr = agrupacion->text();
    fprintf(stderr,"Vamos a hacer un accept\n");
    if (idamortizacion == "") {
        fprintf(stderr,"Se trata de una inserción");
        query.sprintf("INSERT INTO amortizacion (nomamortizacion, valorcompra, numcuotas, fechacompra,fecha1cuota, idcuentaactivo, idcuentaamortizacion, agrupacion) VALUES ('%s', %f, %d, '%s','%s', id_cuenta('%s'), id_cuenta('%s'),'%s')", namortizacion.ascii(), valorcompradbl, numcuotasint, fechacomprastr.ascii(), fecha1cuotastr.ascii(), ctaactivostr.ascii(), ctaamortizacionstr.ascii(), agrupacionstr.ascii());
        m_companyact->begin();
        if (m_companyact->ejecuta(query)) {
            // El mensaje de error lo deberia dar la funcion ejecuta, por tanto esto
            // Va a ser obsoleto.
            QMessageBox::warning(this, tr("Error..."), tr("Ocurrió un error con la Base de Datos"), tr("Aceptar"));
            return;
        }//  end if
        m_companyact->commit();
        query.sprintf("SELECT max(idamortizacion) AS idamortizacion FROM amortizacion");
        m_companyact->begin();
        cursor2 *cur = m_companyact->cargacursor(query,"unquery");
        m_companyact->commit();
        if (!cur->eof()) {
            idamortizacion = cur->valor("idamortizacion");
        }// end if
        //Iteramos para cada linea en el subformulario.
        for(int i=0; i<table1->numRows(); i++) {
            //Insertamos en la base de datos cada linea de amortizacion.
            query.sprintf("INSERT INTO linamortizacion (idamortizacion, fechaprevista, cantidad) VALUES (%s, '%s', %s)", idamortizacion.ascii(), table1->text(i,COL_FECHA).ascii(), table1->text(i, COL_CUOTA).ascii());
            m_companyact->begin();
            m_companyact->ejecuta(query);
            m_companyact->commit();
        }// end for
    } else {
        fprintf(stderr,"Se trata de una modificación\n");
        query.sprintf("UPDATE amortizacion SET nomamortizacion='%s', valorcompra=%f, numcuotas=%d, fechacompra='%s', idcuentaactivo=%s, idcuentaamortizacion=%s, fecha1cuota='%s', agrupacion='%s' WHERE idamortizacion=%s", namortizacion.ascii(), valorcompradbl, numcuotasint,fechacomprastr.ascii(), idctaactivo.ascii(), idctaamortizacion.ascii(),fecha1cuotastr.ascii(),agrupacionstr.ascii(), idamortizacion.ascii());
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        //Iteramos para cada linea en el subformulario.
        for(int i=0; i<table1->numRows(); i++) {
            if (table1->text(i,COL_IDLINAMORTIZACION) != "") {
                //Modificamos las entradas en la linea de amortización.
                query.sprintf("UPDATE linamortizacion SET fechaprevista='%s', cantidad=%s WHERE idlinamortizacion=%s", table1->text(i,COL_FECHA).ascii(), table1->text(i,COL_CUOTA).ascii(), table1->text(i,COL_IDLINAMORTIZACION).ascii());
                m_companyact->begin();
                m_companyact->ejecuta(query);
                m_companyact->commit();
            } else {
                // AQUI VAMOS A PONER LA INSERCION DE NUEVAS ENTRADAS DE AMORTIZACION
                query = "INSERT INTO linamortizacion (idamortizacion, fechaprevista, cantidad) VALUES ("+idamortizacion+",'"+table1->text(i,COL_FECHA)+"',"+table1->text(i,COL_CUOTA)+")";
                m_companyact->begin();
                m_companyact->ejecuta(query);
                m_companyact->commit();
            }// end if
        }// end for
    }// end if
    dialogChanges_cargaInicial();
	_depura("END amortizacionview::s_saveAmortizacion", 0);
}


void amortizacionview::accept() {
    //	s_saveAmortizacion();
    //	done(1);
}// end accept


bool amortizacionview::close(bool ok) {
	_depura("amortizacionview::close", 0);
    trataModificado();
	_depura("END amortizacionview::close", 0);
    return QWidget::close(ok);
}// end close

amortizacionview::~amortizacionview() {}


// Esta funcion inicializa la clase, y además hace la presentación de una determinada amortizacion.
void amortizacionview::inicializa(QString idamortiza) {
	_depura("amortizacionview::inicializa", 0);
    idamortizacion = idamortiza;
    fprintf(stderr,"Inicializamos el formulario %s\n", idamortizacion.ascii());
    QString query = "SELECT * FROM amortizacion LEFT JOIN (SELECT idcuenta AS idcta, codigo AS codctaactivo FROM cuenta) AS t1 ON t1.idcta=amortizacion.idcuentaactivo LEFT JOIN (SELECT idcuenta AS idcta1, codigo AS codctaamortizacion FROM cuenta) AS t2 ON t2.idcta1=amortizacion.idcuentaamortizacion WHERE idamortizacion="+idamortizacion;

    m_companyact->begin();
    cursor2 *curs=m_companyact->cargacursor(query,"unquery");
    m_companyact->commit();

    // Si existe el registro que se pasa como parámetro.
    if (!curs->eof()) {
        // Ponemos en pantalla todos los campos del registro de amortización que se ha cargado
        // de la base de datos.
        nomamortizacion->setText(curs->valor("nomamortizacion"));
        valorcompra->setText(curs->valor("valorcompra"));
        numcuotas->setText(curs->valor("numcuotas"));
        QString cadena;
        cadena.sprintf("%10.10s",curs->valor("fechacompra").ascii());
        fechacompra->setText(cadena);
        cadena.sprintf("%10.10s",curs->valor("fecha1cuota").ascii());
        fecha1cuota->setText(cadena);
        ctaactivo->setText(curs->valor("codctaactivo"));
        idctaactivo = curs->valor("idcta");
        ctaamortizacion->setText(curs->valor("codctaamortizacion"));
        idctaamortizacion= curs->valor("idcta1");
        agrupacion->setText(curs->valor("agrupacion"));
    }// end if
    delete curs;


    // Buscamos todos los plazos de amortizacion y los vamos poniendo.
    query = "SELECT *, fechaprevista<=now() AS ant FROM linamortizacion LEFT JOIN asiento ON linamortizacion.idasiento=asiento.idasiento WHERE idamortizacion ="+idamortizacion+" ORDER BY fechaprevista";
    m_companyact->begin();
    curs = m_companyact->cargacursor(query, "otroquery");
    m_companyact->commit();
    table1->setNumRows(curs->numregistros());
    for (int i=0; i<curs->numregistros(); i++) {
        if ( curs->valor("idasiento")== "" && curs->valor("ant")=="t") {
            //         table1->setText(i,COL_FECHA,curs->valor("fechaprevista"));
            table1->setItem(i,COL_FECHA, new QTableItem1(table1, Q3TableItem::OnTyping,"", 10));
            table1->setText(i,COL_FECHA,curs->valor("fechaprevista"));
            //         table1->setText(i,COL_CUOTA, curs->valor("cantidad"));
            table1->setItem(i,COL_CUOTA, new QTableItem1(table1, Q3TableItem::OnTyping,"", 10));
            table1->setText(i,COL_CUOTA,curs->valor("cantidad"));
        } else {
            table1->setText(i,COL_FECHA,curs->valor("fechaprevista"));
            table1->setText(i,COL_CUOTA, curs->valor("cantidad"));
        }// end if
        table1->setText(i,COL_IDLINAMORTIZACION, curs->valor("idlinamortizacion"));
        table1->setText(i,COL_IDASIENTO, curs->valor("idasiento"));
        table1->setText(i,COL_ORDENASIENTO, curs->valor("ordenasiento"));
        curs->siguienteregistro();
    }// end for
    delete curs;

    // Calculamos lo que ya llevamos amortizado y lo presentamos en la pantalla.
    query = "SELECT sum(cantidad) AS amortizado FROM linamortizacion WHERE idasiento IS NOT NULL AND idamortizacion="+idamortizacion;
    m_companyact->begin();
    curs = m_companyact->cargacursor(query,"amortizado");
    m_companyact->commit();
    if(!curs->eof()) {
        amortizado->setText(curs->valor("amortizado"));
    }// end if
    delete curs;


    // Calculamos lo que nos falta por amortizar y lo presentamos en la pantalla.
    query = "SELECT sum(cantidad) AS pdte FROM linamortizacion WHERE idasiento IS NULL AND idamortizacion="+idamortizacion;
    m_companyact->begin();
    curs = m_companyact->cargacursor(query,"pdte");
    m_companyact->commit();
    if(!curs->eof()) {
        pendiente->setText(curs->valor("pdte"));
    }// end if
    delete curs;

    // Desabilitamos el boton de calcular, porque la amortizacion ya está hecha.
    botoncalcular->setDisabled(TRUE);
    dialogChanges_cargaInicial();
    m_companyact->meteWindow( caption(), this);
	_depura("END amortizacionview::inicializa", 0);
}// end inicializa


void amortizacionview::calculaamortizacion() {
	_depura("amortizacionview::calculaamortizacion", 0);
    // Para hacer el círculo de los plazos de cada amortizacion
    // Hay que obtener diversos datos.
    QDate f1cuota = normalizafecha(fecha1cuota->text());
    int ncuotas = numcuotas->text().toInt();
    QString periodicidadtxt = periodicidad->currentText();
    double valcuota;
    QString valcuotastr ;
    table1->setNumRows(ncuotas);


    // Si usamos el método lineal:
    if (metodolineal->isChecked()) {
        valcuota = valorcompra->text().toDouble() / ncuotas;
        valcuotastr.sprintf("%10.2f",valcuota);
        for (int i = 0; i<ncuotas; i++) {
            fprintf(stderr,"calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").ascii());
            table1->setText(i,0,f1cuota.toString("dd/MM/yyyy"));
            table1->setText(i,1,valcuotastr);
            // Dependiendo de la periodicidad actualizamos la fecha.
            if (periodicidadtxt == tr("Anual")) {
                f1cuota = f1cuota.addYears(1);
            } else if (periodicidadtxt == tr("Mensual")) {
                f1cuota = f1cuota.addMonths(1);
            } else if (periodicidadtxt == tr("Semestral")) {
                f1cuota = f1cuota.addMonths(6);
            } else if (periodicidadtxt == tr("Trimestral")) {
                f1cuota = f1cuota.addMonths(3);
            }// end if
        }// end for
    } else if (metodoincremental->isChecked()) {
        // El metodo de amortizacion es el incremental.
        double total=0;
        for (int j=1;j<=ncuotas;j++) {
            total = total+j;
        }// end for
        for (int i=1; i<=ncuotas; i++) {
            valcuota = valorcompra->text().toDouble()*i/total;
            valcuotastr.sprintf("%10.2f",valcuota);
            fprintf(stderr,"calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").ascii());
            table1->setText(i-1,0,f1cuota.toString("dd/MM/yyyy"));
            table1->setText(i-1,1,valcuotastr);
            // Dependiendo de la periodicidad actualizamos la fecha.
            if (periodicidadtxt == tr("Anual")) {
                f1cuota = f1cuota.addYears(1);
            } else if (periodicidadtxt == tr("Mensual")) {
                f1cuota = f1cuota.addMonths(1);
            } else if (periodicidadtxt == tr("Semestral")) {
                f1cuota = f1cuota.addMonths(6);
            } else if (periodicidadtxt == tr("Trimestral")) {
                f1cuota = f1cuota.addMonths(3);
            }// end if
        }// end for
    } else if (metododecremental->isChecked()) {
        // El metodo de amortizacion es el incremental.
        double total=0;
        for (int j=1;j<=ncuotas;j++) {
            total = total+j;
        }// end for
        for (int i=0; i<ncuotas; i++) {
            valcuota = valorcompra->text().toDouble()*(ncuotas-i)/total;
            valcuotastr.sprintf("%10.2f",valcuota);
            fprintf(stderr,"calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").ascii());
            table1->setText(i,0,f1cuota.toString("dd/MM/yyyy"));
            table1->setText(i,1,valcuotastr);
            // Dependiendo de la periodicidad actualizamos la fecha.
            if (periodicidadtxt == tr("Anual")) {
                f1cuota = f1cuota.addYears(1);
            } else if (periodicidadtxt == tr("Mensual")) {
                f1cuota = f1cuota.addMonths(1);
            } else if (periodicidadtxt == tr("Semestral")) {
                f1cuota = f1cuota.addMonths(6);
            } else if (periodicidadtxt == tr("Trimestral")) {
                f1cuota = f1cuota.addMonths(3);
            }// end if
        }// end for
    } else if (metodoporcentual->isChecked()) {
        // El metodo de amortizacion es el incremental.
        double porcent=(double) 1/ (double) ncuotas;
        fprintf(stderr,"El coeficiente es: %10.2f\n",porcent);
        double total=0;
        for (int i=0; i<ncuotas; i++) {
            if (i < ncuotas-1) {
                valcuota = (valorcompra->text().toDouble()-total)*porcent;
                total += valcuota;
                fprintf(stderr,"cuota: %10.2f -- total: %10.2f\n",valcuota, total);

            } else {
                valcuota = valorcompra->text().toDouble()-total;
            }// end if
            valcuotastr.sprintf("%10.2f",valcuota);
            fprintf(stderr,"calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").ascii());
            table1->setText(i,0,f1cuota.toString("dd/MM/yyyy"));
            table1->setText(i,1,valcuotastr);
            // Dependiendo de la periodicidad actualizamos la fecha.
            if (periodicidadtxt == tr("Anual")) {
                f1cuota = f1cuota.addYears(1);
            } else if (periodicidadtxt == tr("Mensual")) {
                f1cuota = f1cuota.addMonths(1);
            } else if (periodicidadtxt == tr("Semestral")) {
                f1cuota = f1cuota.addMonths(6);
            } else if (periodicidadtxt == tr("Trimestral")) {
                f1cuota = f1cuota.addMonths(3);
            }// end if
        }// end for
    }// end if
	_depura("amortizacionview::calculaamortizacion", 0);
}// end calculaamortizacion



/** \brief SLOT que captura el menu contextual sobre la tabla de amortizacion.
  * Datos para el asiento de amortización.
  * El asiento inteligente debe llamarse como el parametro Amortizacion de confpr.
  * El asiento inteligente debe tener las variables que se llamen:
  * $cuenta$ Cuenta de Amortización.
  * $cuentabien$ Cuenta del Bien.
  * $cuota$ Cuota a pagar.
  */
void amortizacionview::contextMenuRequested(int row, int col, const QPoint &poin) {
	_depura("amortizacionview::contextMenuRequested", 0);
    Q3PopupMenu *popup;
    popup = new Q3PopupMenu;
    int opcion;
//    intapunts3view *intapunts = m_companyact->intapuntsempresa();
    /// Generamos el menú contextual.
    popup->insertItem(tr("Generar Asiento"),4);
    popup->insertSeparator();
    popup->insertItem(tr("Ver Asiento"),1);
    popup->insertSeparator();
    popup->insertItem(tr("Desvincular Asiento"),5);
    popup->insertItem(tr("Borrar Asiento"),6);
    popup->insertSeparator();
    popup->insertItem(tr("Insertar Cuota"),7);
    popup->insertItem(tr("Borrar Cuota"),8);

    if (table1->text(row, COL_IDASIENTO) == "") {
        popup->setItemEnabled(4,TRUE);
        popup->setItemEnabled(1,FALSE);
        popup->setItemEnabled(5,FALSE);
        popup->setItemEnabled(6,FALSE);
        popup->setItemEnabled(7,TRUE);
        popup->setItemEnabled(8,TRUE);
    } else {
        popup->setItemEnabled(4,FALSE);
        popup->setItemEnabled(1,TRUE);
        popup->setItemEnabled(5,TRUE);
        popup->setItemEnabled(6,TRUE);
        popup->setItemEnabled(7,TRUE);
        popup->setItemEnabled(8,FALSE);
    }// end if

    opcion = popup->exec(poin);
    delete popup;
    /// Inserción de una nueva cuota.
    if (opcion == 7) {
        table1->insertRows(row+1, 1);
    }// end if
    /// Eliminar una cuota.
    if (opcion == 8) {
        QString idlinamortizacion = table1->text(row, COL_IDLINAMORTIZACION);
        QString query="DELETE FROM linamortizacion WHERE idlinamortizacion="+idlinamortizacion;
        if(idlinamortizacion != "") {
            m_companyact->begin();
            m_companyact->ejecuta(query);
            m_companyact->commit();
        }// end if
        table1->removeRow(row);
    }// end if
    if (opcion == 1 || opcion == 6) {
        /// Si se va a mostrar el asiento, o se va a borrar
        QString idasiento = table1->text(row, COL_IDASIENTO);
        m_companyact->intapuntsempresa()->muestraasiento(idasiento.toInt());
    }// end if
    if (opcion == 5 || opcion ==6) {
        /// Si se va a desvincular el asiento o se va a borrar.
        QString idasiento = table1->text(row, COL_IDASIENTO);
        QString idlinamortizacion = table1->text(row, COL_IDLINAMORTIZACION);
        QString query = "UPDATE linamortizacion SET idasiento=NULL WHERE idlinamortizacion="+idlinamortizacion;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        table1->setText(row,COL_IDASIENTO,"");
        table1->setText(row,COL_ORDENASIENTO, "");
    }// end if
    if (opcion ==6) {
        /// Si se va a borrar el asiento
        m_companyact->intapuntsempresa()->borraAsiento1();
    }// end if
    if (opcion == 4) {
        /// Se va a generar el asiento
        QString fecha= table1->text(row,COL_FECHA);
        fprintf(stderr,"Fecha: %s\n", fecha.ascii());
        QString cant= table1->text(row, COL_CUOTA);
        fprintf(stderr,"Cuota: %s\n", cant.ascii());
        int numasiento = 0; //El asiento debe ser uno nuevo.
        aplinteligentesview *nueva=new aplinteligentesview(m_companyact,0,"");
        QString cuenta = ctaactivo->text();
        QString cuentaamort = ctaamortizacion->text();
        nueva->inicializa(numasiento);

        nueva->muestraplantilla("amortizacion.xml");


        nueva->setvalores("$cuenta$",cuentaamort);
        nueva->setvalores("$cuentabien$",cuenta);
        nueva->setvalores("$fechaasiento$",table1->text(row,COL_FECHA));
        nueva->setvalores("$cuota$",table1->text(row,COL_CUOTA));
        /// Ponemos la fecha del asiento para evitar escribir.
        nueva->setfechaasiento(table1->text(row,COL_FECHA));
        /// Ponemos los asientos plantilla en modo exclusivo, para poder recuperar el control en cuanto se haya hecho la inserción del asiento.
        nueva->setmodo(1);
        nueva->exec();
        int numasiento1=m_companyact->intapuntsempresa()->idasiento().toInt();
        QString ordenasiento;
        QString SQLQuery = "SELECT * FROM asiento where idasiento="+QString::number(numasiento1);
        m_companyact->begin();
        cursor2 *cur = m_companyact->cargacursor(SQLQuery,"hola");
        m_companyact->commit();
        if (!cur->eof()) {
            ordenasiento= cur->valor("ordenasiento");
        }// end if
        table1->setText(row,COL_IDASIENTO,QString::number(numasiento1));
        table1->setText(row,COL_ORDENASIENTO, ordenasiento);
        delete nueva;
        delete cur;

        /// Debemos guardar la modificacion en la linea de amortizacion.
        QString idlinamortizacion = table1->text(row,COL_IDLINAMORTIZACION);
        SQLQuery = "UPDATE linamortizacion set idasiento="+QString::number(numasiento1)+" WHERE idlinamortizacion="+idlinamortizacion;
        m_companyact->begin();
        m_companyact->ejecuta(SQLQuery);
        m_companyact->commit();
    }// end if
    col=0;
	_depura("END amortizacionview::contextMenuRequested", 0);
}


void amortizacionview::trataModificado() {
    _depura("amortizacionview::trataModificado", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, "Guardar Amortizacion",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            s_saveAmortizacion();
    }// end if
    _depura("END amortizacionview::trataModificado", 0);
}
