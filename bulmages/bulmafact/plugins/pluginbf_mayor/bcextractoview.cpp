/**********************************[20~*****************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLayout>
#include <QtCore/QTextStream>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFileDialog>

#include "blmainwindow.h"
#include "bfbulmafact.h"
#include "bcextractoview.h"
#include "bfcompany.h"
#include "bcasientoview.h"
//#include "bccentrocosteseleccionarview.h"
//#include "bccanalseleccionarview.h"
#include "bldatesearch.h"
#include "bcextractosubform.h"
#include "blprogressbar.h"
#include "blfunctions.h"
#include "blfile.h"

/// Este archivo contiene la implementaci&oacute;n de la clase BcExtractoView que saca el
/// extracto por pantalla de una o varias cuentas determinadas. Esta clase es una de las
/// tres principales junto a \ref introapuntes1 y \ref diarioview1.
/**
\param emp
\param parent
**/
BcExtractoView::BcExtractoView ( BfCompany *company, QWidget *parent, int ) : BfForm ( company, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );

    setAttribute(Qt::WA_DeleteOnClose);
    setTitleName ( _ ( "Extracto de cuentas" ) );
    setDbTableName ( "extracto" );
    setDbFieldId ( "idcuenta" );
    addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbNoSave, _ ( "idcuenta" ) );
    addDbField ( "fechaInicial", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "fechaInicial" ) );
    addDbField ( "fechaFinal", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "fechaFinal" ) );
    
    setDbValue("idcuenta","0");
    
    mui_list->setMainCompany ( company );

    connect (mui_list, SIGNAL(openAsiento()), this, SLOT(openAsiento()) );

    /// Iniciamos los componentes
    mui_cuentaInicial->setMainCompany ( company );
    mui_cuentaInicial->setLabel ( _ ( "Cuenta inicial:" ) );
    mui_cuentaInicial->setTableName ( "cuenta" );
    mui_cuentaInicial->setFieldId("idcuenta");
    mui_cuentaInicial->m_valores["descripcion"] = "";
    mui_cuentaInicial->m_valores["codigo"] = "";

    mui_cuentaFinal->setMainCompany ( company );
    mui_cuentaFinal->setLabel ( _ ( "Cuenta Final:" ) );
    mui_cuentaFinal->setTableName ( "cuenta" );
    mui_cuentaFinal->setFieldId("idcuenta");
    mui_cuentaFinal->m_valores["descripcion"] = "";
    mui_cuentaFinal->m_valores["codigo"] = "";

    mui_codigoContrapartida->setMainCompany ( company );
    mui_codigoContrapartida->setLabel ( _ ( "Cuenta contrapartida:" ) );
    mui_codigoContrapartida->setTableName ( "cuenta" );
    mui_codigoContrapartida->setFieldId("idcuenta");
    mui_codigoContrapartida->m_valores["descripcion"] = "";
    mui_codigoContrapartida->m_valores["codigo"] = "";


    connect(mui_cuentaInicial, SIGNAL(valueChanged(QString)), this, SLOT(cuentaIChanged ( QString )));
    
    mui_cuentaInicial->hideLabel();
    mui_cuentaFinal->hideLabel();
    mui_codigoContrapartida->hideLabel();

    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    QString cadena;
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year() );
    mui_fechaInicial->setText ( cadena );
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year() );
    mui_fechaFinal->setText ( cadena );
    m_cursorcta = NULL;
    insertWindow ( windowTitle(), this, false );

    m_tratarpunteos = true;


    /// Conectamos los botones del menu con las acciones de esta ventana.
/*
    connect (((BfBulmaFact *)g_main)->actionSiguiente, SIGNAL(triggered()), this, SLOT(botonSiguiente()));
    connect (((BfBulmaFact *)g_main)->actionAnterior, SIGNAL(triggered()), this, SLOT(botonAnterior()));
    connect (((BfBulmaFact *)g_main)->actionInicio, SIGNAL(triggered()), this, SLOT(botonInicio()));
    connect (((BfBulmaFact *)g_main)->actionFin, SIGNAL(triggered()), this, SLOT(botonFin()));
*/
    /// Llamamos a los scripts
    blScript(this);

    
}


/// Destructor de la clase.
/**
**/
BcExtractoView::~BcExtractoView()
{
    BL_FUNC_DEBUG
    save();
    delete m_cursorcta;
    mainCompany() ->removeWindow ( this );
    
}



void BcExtractoView::openAsiento()
{
    BL_FUNC_DEBUG
    
    QString idasiento = mui_list->dbValue ( "idasiento" );

    int resur = g_plugins->run ( "SNewBcAsientoView", (BfCompany *) mainCompany() );
    
    if ( ! resur) {
        blMsgInfo(_("No se pudo crear instancia de asientos."));
        return;
    } // end if
    
    BcAsientoView *asiento = (BcAsientoView *) g_plugParams;

    asiento ->muestraAsiento ( idasiento );
    asiento ->show();
    asiento ->setFocus();
}


///
/**
\param columna
**/
void BcExtractoView::on_mui_list_cellDoubleClicked ( int, int columna )
{
    BL_FUNC_DEBUG

    openAsiento();
}


///
void BcExtractoView::on_mui_configurar_clicked()
{
    BL_FUNC_DEBUG
    mui_list->showConfig();
    
}


///
/**
**/
void BcExtractoView::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    accept();
    
}





/// Esta funci&oacute;n carga el cursor de cuentas que forman el todo por el todo.
/** Tambi&eacute;n ser&aacute; la encargada de recoger la informaci&oacute;n de
    filtraci&oacute;n para que todo sea correcto. */
/**
**/
void BcExtractoView::accept()
{
    BL_FUNC_DEBUG
    QString codinicial = mui_cuentaInicial->fieldValue("codigo");
    QString codfinal = mui_cuentaFinal->fieldValue("codigo");
    QString query;
    /// Si los datos de c&oacute;digo inicial y final est&aacute;n vacios los ponemos
    /// nosotros.
    if ( codinicial == "" ) {
        codinicial = "0";
    } // end if
    if ( codfinal == "" ) {
        codfinal = "9999999";
    } // end if
    
    if ( mui_incluirAsientosAbiertos->isChecked() ) {
	/// Incluye asientos abiertos.
	query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM apunte UNION SELECT idcuenta FROM borrador) AND codigo >= '" + codinicial + "' AND codigo <= '" + codfinal + "' ORDER BY codigo";
    } else {
	query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM apunte) AND codigo >= '" + codinicial + "' AND codigo <= '" + codfinal + "' ORDER BY codigo";
    } // end if

    if ( m_cursorcta != NULL ) {
        delete m_cursorcta;
    } // end if
    m_cursorcta = mainCompany() ->loadQuery ( query );
    presentar();
    
}



/// Esta es la funci&oacute;n que avanza un registro entre las cuentas.
/**
**/
void BcExtractoView::botonSiguiente()
{
    BL_FUNC_DEBUG
      if ( m_cursorcta != NULL ) {
          if ( !m_cursorcta->isLastRecord() ) {
              save();
              m_cursorcta->nextRecord();
              presentar();
          } // end if
      } // end if
}


/// Esta es la funci&oacute;n que retrocede un registro entre las cuentas.
/**
**/
void BcExtractoView::botonAnterior()
{
    BL_FUNC_DEBUG
      if ( m_cursorcta != NULL ) {
          if ( !m_cursorcta->isFirstRecord() ) {
              save();
              m_cursorcta->previousRecord();
              presentar();
          } // end if
      } // end if
}

/// Retrocede al principio de las cuentas.
/**
**/
void BcExtractoView::botonInicio()
{
    BL_FUNC_DEBUG

      if ( m_cursorcta != NULL ) {
          save();
          m_cursorcta->firstRecord();
          presentar();
      } // end if
}

/// Avanza al final de las cuentas.
/**
**/
void BcExtractoView::botonFin()
{
    BL_FUNC_DEBUG
      if ( m_cursorcta != NULL ) {
          save();
          m_cursorcta->lastRecord();
          presentar();
      } // end if
}


///
/**
**/
void BcExtractoView::botonGuardar()
{
    BL_FUNC_DEBUG
/*
    QString fn = QFileDialog::getSaveFileName ( this,
                 _ ( "Guardar libro diario" ),
                 g_confpr->value( CONF_DIR_USER ),
                 _ ( "Diarios (*.txt)" ) );

    if ( !fn.isEmpty() ) {
        BcLibroMayorImprimir libromayor ( mainCompany() );
        QString finicial = mui_fechaInicial->text().toLatin1().constData();
        QString ffinal = mui_fechaFinal->text().toLatin1().constData();
        libromayor.inicializa1 ( mui_cuentaInicial->text(), mui_cuentaFinal->text(), finicial, ffinal );
        libromayor.inicializa2 ( ( char * ) fn.toLatin1().constData() );
        libromayor.accept();
    } // end if
*/
    
}




/// Limpia los totales
/**
**/
void BcExtractoView::vaciar()
{
    BL_FUNC_DEBUG
    mui_inicialDebe->setText ( "0" );
    mui_inicialHaber->setText ( "0" );
    mui_inicialSaldo->setText ( "0" );
    mui_totalParcialDebe->setText ( "0" );
    mui_totalParcialHaber->setText ( "0" );
    mui_totalParcialSaldo->setText ( "0" );
    mui_totalDebe->setText ( "0" );
    mui_totalHaber->setText ( "0" );
    mui_totalSaldo->setText ( "0" );
}


///
/**
**/
void BcExtractoView::ajustes()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
int BcExtractoView::save()
{
    BL_FUNC_DEBUG
    if (m_tratarpunteos) {
	if (! mui_incluirAsientosAbiertos->isChecked() ) 
	    mui_list->save();
    } // end if
    return 0;
    

}



/// Esta funci&oacute;n se encarga de montar la consulta que va a hacer a la base de datos.
/**
\return
**/
void BcExtractoView::presentar()
{
    BL_FUNC_DEBUG

    /// Guardamos el punteo
    save();


    BlFixed debe ( "0.00" ), haber ( "0.00" ), saldo ( "0.00" );
    BlFixed debeinicial ( "0.00" ), haberinicial ( "0.00" ), saldoinicial ( "0.00" );
    BlFixed debeparcial ( "0.00" ), haberparcial ( "0.00" ), saldoparcial ( "0.00" );
    BlFixed debefinal ( "0.00" ), haberfinal ( "0.00" ), saldofinal ( "0.00" );
    QString idcuenta;
    QString finicial = mui_fechaInicial->text();
    QString ffinal = mui_fechaFinal->text();
    QString contra = mui_codigoContrapartida->fieldValue("codigo");
    QString cad;
    QString cadaux;
    BlDbRecordSet *cursorapt = NULL;
    BlDbRecordSet *cursoraux = NULL;
    QString saldosup = "";
    QString saldoinf = "";
    try {
        /// Preparamos el string para que aparezca una u otra cosa seg&uacute;n el punteo.
        QString tipopunteo;
        tipopunteo = "";
        if ( mui_apuntesTodos->isChecked() ) {
            tipopunteo = "";
        } else if ( mui_apuntesPunteados->isChecked() ) {
            tipopunteo = " AND punteo = true ";
        } else {
            tipopunteo = " AND punteo = false ";
        } // end if


        if ( m_cursorcta->eof() || m_cursorcta->bof() )
            return;

        idcuenta = m_cursorcta->value( "idcuenta" );
        /// Escribimos el nombre de la cuenta y el c&oacute;digo de la misma.
        mui_codigoCuenta->setText ( m_cursorcta->value( "codigo" ) );
        mui_nombreCuenta->setText ( m_cursorcta->value( "descripcion" ) );
	
	/// Para que salga en el indexador modificadoel titulo hacemos un setDescripcion y un insertWindow
	setDescripcion(m_cursorcta->value( "codigo" ) + " " + m_cursorcta->value( "descripcion" ) );
        insertWindow ( windowTitle(), this, true );
	
        /// Hacemos la consulta de los apuntes a listar en la base de datos.
        QString query = "";
        /// Al igual que en el caso anterior los centros de coste han cambiado y a&uacute;n
        /// no se pueden implementar.

        QString ccostes = "";
	g_plugins->run("PgetSelCostes", &ccostes);
	
        if ( ccostes != "" ) {
            ccostes.sprintf ( " AND idc_coste IN (%s) ", ccostes.toLatin1().constData() );
        } // end if
        

        
        
        QString tabla;
        QString cont;
        if ( mui_incluirAsientosAbiertos->isChecked() ) {
            tabla = "borrador";
            cont = " false AS punteo, *, descripcion AS descripcionapunte ";
        } else {
            tabla = "apunte";
            cont = " *, descripcion AS descripcionapunte ";
        } // end if

        if ( contra != "" ) {
            tipopunteo += " AND " + tabla + ".contrapartida = id_cuenta('" + contra + "') ";
        } // end if
        bool ok = false;
        mui_saldoSuperiorA->text().toFloat ( &ok );
        if ( ok ) {
            saldosup = " AND " + tabla + ".debe + " + tabla + ".haber >= " + mui_saldoSuperiorA->text();
        } // end if
        ok = false;
        mui_saldoInferiorA->text().toFloat ( &ok );
        if ( ok ) {
            saldoinf = " AND " + tabla + ".debe + " + tabla + ".haber <= " + mui_saldoInferiorA->text();
        } // end if
        QString clase = "";
        if (mui_ocultarAperturas->isChecked()) {
          clase = " AND clase < 2 ";
        } // end if
        
        
        /// El calculo de los canales
        QString ccanales = "";
	g_plugins->run("PgetSelCanales", &ccanales);
	bool sincanal = true;
	g_plugins->run("PgetSinCanal", &sincanal);
	
        if (sincanal) {
	  if ( ccanales != "" ) {
	      ccanales = " AND ("+tabla+".idcanal ISNULL OR "+tabla+".idcanal IN (" + ccanales + ")) ";
	  } else {
	      ccanales = " AND "+tabla+".idcanal ISNULL ";	    
	  } // end if
	} else {
	  if ( ccanales != "" ) {
	      ccanales = " AND ("+tabla+".idcanal <> NULL OR "+tabla+".idcanal IN (" + ccanales + ")) ";
	  } else {
	      ccanales = " AND "+tabla+".idcanal <> NULL ";	    
	  } // end if
	} // end if
	
	
        query = "SELECT * FROM ((SELECT " + cont + " FROM " + tabla + " WHERE  idcuenta = " + idcuenta + " AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' " + ccostes + " " + ccanales + " " + tipopunteo + saldosup + saldoinf + ") AS t2 ";
        query += " LEFT JOIN (SELECT idcuenta AS idc, descripcion, codigo, tipocuenta FROM cuenta) AS t9 ON t2.idcuenta = t9.idc) AS t1";
        query += " LEFT JOIN (SELECT *, descripcion AS descripcionasiento FROM asiento) AS t4 ON t4.idasiento = t1.idasiento ";
        query += " LEFT JOIN (SELECT idc_coste AS idccoste, nombre AS nombrec_coste FROM c_coste) AS t5 ON t5.idccoste = t1.idc_coste ";
        query += " LEFT JOIN (SELECT idcanal AS id_canal, nombre AS nombrecanal FROM canal) AS t6 ON t6.id_canal = t1.idcanal ";
        query += " LEFT JOIN (SELECT idcuenta AS idcontrapartida, codigo AS codcontrapartida FROM cuenta) as t8 ON t8.idcontrapartida = t1.contrapartida WHERE 1=1 " + clase;
        query += " ORDER BY t1.fecha, ordenasiento, t1.orden"+tabla;

        mui_list->load ( query );


        cursorapt = mainCompany() ->loadQuery ( query );
        cursorapt->firstRecord();
        if ( !cursorapt->eof() ) {
            /// Cargamos los saldos iniciales.
            query = "SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte LEFT JOIN asiento ON apunte.idasiento = asiento.idasiento WHERE idcuenta =" + idcuenta + " AND apunte.fecha < '" + finicial + "'" + clase;
            cursoraux = mainCompany() ->loadQuery ( query );
            if ( !cursoraux->eof() ) {
                debeinicial = BlFixed ( cursoraux->value( "tdebe" ) );
                haberinicial = BlFixed ( cursoraux->value( "thaber" ) );
                saldoinicial = debeinicial - haberinicial;
            } // end if
            delete cursoraux;
            cursoraux = NULL;

            /// Establecemos los saldos iniciales
            mui_inicialDebe->setText ( debeinicial.toQString() );
            mui_inicialHaber->setText ( haberinicial.toQString() );
            mui_inicialSaldo->setText ( saldoinicial.toQString() );
            saldo = saldoinicial;
            debefinal = debeinicial;
            haberfinal = haberinicial;

            /// Recorremos la lista agregando el campo de saldo.
            int i = 0;
            while ( !cursorapt->eof() ) {
                debe = BlFixed ( cursorapt->value( "debe" ) );
                haber = BlFixed ( cursorapt->value( "haber" ) );
                saldoparcial = saldoparcial + debe - haber;
                saldo = saldo + debe - haber;
                debefinal = debefinal + debe;
                debeparcial = debeparcial + debe;
                haberfinal = haberfinal + haber;
                haberparcial = haberparcial + haber;
                if ( mui_list->lineaat ( i ) )
                    mui_list->setDbValue ( "saldo", i++, saldo.toQString() );
                cursorapt->nextRecord();
            } // end while

            saldofinal = debefinal - haberfinal;
            mui_totalDebe->setText ( debefinal.toQString() );
            mui_totalHaber->setText ( haberfinal.toQString() );
            mui_totalSaldo->setText ( saldofinal.toQString() );
            mui_totalParcialDebe->setText ( debeparcial.toQString() );
            mui_totalParcialHaber->setText ( haberparcial.toQString() );
            mui_totalParcialSaldo->setText ( saldoparcial.toQString() );
        } // end if
        delete cursorapt;
        cursorapt = NULL;
        ajustes();
    } catch ( ... ) {
        blMsgInfo ( "Error en los calculos" );
        /// Liberamos memoria que pueda haber quedado reservada.
        if ( cursorapt ) delete cursorapt;
        if ( cursoraux ) delete cursoraux;
        return;
    } // end catch
    
}




///
/**
\param codinicial
\param codfinal
\param fecha1
\param fecha2
**/
void BcExtractoView::inicializa1 ( QString codinicial, QString codfinal, QString fecha1, QString fecha2, int )
{
    BL_FUNC_DEBUG
    mui_cuentaInicial->setText ( codinicial );
    mui_cuentaFinal->setText ( codfinal );
    mui_fechaInicial->setText ( blNormalizeDate ( fecha1 ).toString ( "dd/MM/yyyy" ) );
    mui_fechaFinal->setText ( blNormalizeDate ( fecha2 ).toString ( "dd/MM/yyyy" ) );
    
}




/// Realiza la casacion de los apuntes.
/**
**/
void BcExtractoView::on_mui_casacion_clicked()
{
    BL_FUNC_DEBUG
    try {
	if (m_cursorcta == 0) {
	    blMsgInfo(_("Cargue primero una cuenta."));
	    return;
	} // end if
	
	if (m_cursorcta->value( "idcuenta" ).isEmpty()) {
	    blMsgWarning(_("La cuenta no tiene apuntes."));
	    return;
	} // end if
	
        QString query = "SELECT * FROM apunte WHERE punteo = false AND haber <> 0 AND idcuenta = " + m_cursorcta->value( "idcuenta" ) + " ORDER BY fecha";
        BlDbRecordSet *curshaber = mainCompany() ->loadQuery ( query );

        BlProgressBar barra;
        barra.setRange ( 0, curshaber->numregistros() );
        barra.show();
        barra.setText ( _ ( "Cargando extracto de cuentas" ) );

        while ( !curshaber->eof() ) {
            query =  "SELECT * FROM apunte WHERE punteo = false AND debe = " + curshaber->value( "haber" ) + " AND idcuenta = " + m_cursorcta->value( "idcuenta" ) + " ORDER BY fecha";
            BlDbRecordSet *cursdebe = mainCompany() ->loadQuery ( query.toLatin1(), "cursdebe" );

            if ( !cursdebe->eof() ) {
                query = "UPDATE apunte set punteo = true WHERE idapunte = " + curshaber->value( "idapunte" );
                mainCompany() ->begin();
                mainCompany() ->runQuery ( query );
                query = "UPDATE apunte SET punteo = true WHERE idapunte = " + cursdebe->value( "idapunte" );
                mainCompany() ->runQuery ( query );
                mainCompany() ->commit();
            } // end if

            delete cursdebe;
            curshaber->nextRecord();
            barra.setValue ( barra.value() + 1 );
        } // end while

        delete curshaber;
	m_tratarpunteos = false;
        presentar();
	m_tratarpunteos = true;
    } catch ( ... ) {
        blMsgError ( _("Se produjo un error en la casacion") );
    } // end try
    
}


/// Guarda el punteo en disco para poder recuperarlo despues
/**
**/
void BcExtractoView::on_mui_guardarPunteo_clicked()
{
    BL_FUNC_DEBUG

    /// Guardamos el punteo por lo que pueda ser.
    save();

    QString fn = QFileDialog::getSaveFileName ( this,
                 _ ( "Guardar punteo" ),
                 g_confpr->value( CONF_DIR_USER ),
                 _ ( "Punteos (*.pto)" ) );

    if ( !fn.isEmpty() ) {
        // Si el archivo no tiene extension le ponemos extension .pto
        if  ( !fn.contains ( "." ) ) {
            fn = fn + ".pto";
        } // end if
        FILE * mifile;
        mifile = fopen ( ( char * ) fn.toLatin1().constData(), "wt" );
        if ( mifile != NULL ) {
            QString query;
            query = "SELECT * FROM apunte WHERE punteo = true";
            BlDbRecordSet *cursp = mainCompany() ->loadQuery ( query );
            while ( !cursp->eof() ) {
                cursp->nextRecord();
            } // end while
            delete cursp;
            fclose ( mifile );
        } // end if
    } // end if
    
}


/// Esta funci&oacute;n borra todo el punteo de un extracto y lo pone a cero.
/** Esta funci&oacute;n se activa justo cuando se pulsa sobre el bot&oacute;n de
    resetear el punteo.
    Por supuesto cuando se pulsa dicho bot&oacute;n se borra el punteo. */
/**
**/
void BcExtractoView::on_mui_borrarPunteo_clicked()
{
    BL_FUNC_DEBUG
    try {
        int valor = QMessageBox::warning ( 0,
                                           _ ( "Borrar punteo" ),
                                           _ ( "Se dispone a borrar el punteo. Este cambio \
                                                es irrecuperable si no ha guardado su punteo. \
                                                Desea continuar?" ),
                                           QMessageBox::Yes, QMessageBox::No );
        if ( valor == QMessageBox::Yes ) {

            mainCompany() ->begin();
            mainCompany() ->runQuery ( "UPDATE apunte SET punteo = false WHERE idcuenta =" + m_cursorcta->value( "idcuenta" ) );
            mainCompany() ->commit();

	    m_tratarpunteos = false;
	    presentar();
	    m_tratarpunteos = true;
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Se ha producido un error." ) );
    } // end try
    
}


/// Carga los punteos desde un fichero de texto que se solicita mediante un QFileDialog.
/** El fichero de punteos requiere que no se hayan modificado los identificadores de
    borrador.
    Para ello es preciso que no se hayan abierto y cerrado los asientos correspondientes
    ya que en dicho caso la carga del punteo no funciona correctamente. */
void BcExtractoView::on_mui_cargarPunteos_clicked()
{
    BL_FUNC_DEBUG
    try {
        QString fn = QFileDialog::getOpenFileName ( this,
                     _ ( "Cargar punteo" ),
                     g_confpr->value( CONF_DIR_USER ),
                     _ ( "Punteo (*.pto);;Todos los archivos (*)" ) );

        if ( !fn.isEmpty() ) {
            BlFile file ( fn );
            if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
                return;
            } // end if
            QTextStream filestr ( &file );
            mainCompany()->begin();
            QString query = "UPDATE apunte SET punteo = false";
            mainCompany() ->runQuery ( query );
            QString a = filestr.readLine();
            while ( !a.isNull() ) {
                QString query;
                query = "UPDATE apunte SET punteo = true WHERE idapunte = " + a;
                mainCompany() ->runQuery ( query );
                a = filestr.readLine();
            } // end while
            mainCompany()->commit();
            file.close();
        } // end if
	m_tratarpunteos = false;
        presentar();
	m_tratarpunteos = true;
    } catch ( ... ) {
        blMsgInfo ( _("Error en la carga del punteo.") );
        mainCompany()->rollback();
    } // end try
    
}



///
/**
\param idcuenta
\return
**/
QString BcExtractoView::imprimeExtractoCuenta ( QString idcuenta )
{
    BL_FUNC_DEBUG
    try {
        QString salida = "";
        BlFixed debeinicial ( "0" ), haberinicial ( "0" ), saldoinicial ( "0" );
        BlFixed debefinal ( "0" ), haberfinal ( "0" ), saldofinal ( "0" );
        QString finicial = mui_fechaInicial->text();
        QString ffinal = mui_fechaFinal->text();
        QString contra = mui_codigoContrapartida->fieldValue("codigo");

        /// Preparamos el string para que aparezca una u otra cosa seg&uacute;n el punteo.
        QString tipopunteo;
        tipopunteo = "";
        if ( mui_apuntesTodos->isChecked() ) {
            tipopunteo = "";
        } else if ( mui_apuntesPunteados->isChecked() ) {
            tipopunteo = " AND punteo = true ";
        } else {
            tipopunteo = " AND punteo = false ";
        } // end if

        if ( contra != "" ) {
            tipopunteo += " AND apunte.contrapartida = id_cuenta('" + contra + "') ";
        } // end if

        QString cad;
        QString cadaux;
        BlDbRecordSet *cursorapt;

        if ( idcuenta == "" ) {
	    
            return "";
        } // end if

        /// Hacemos la consulta de los apuntes a listar en la base de datos.
        QString query = "";
        /// Al igual que en el caso anterior los centros de coste han cambiado y a&uacute;n
        /// no se pueden implementar.
	QString ccostes = "";
	QString ccanales = "";
/*
        BcCanalSeleccionarView *scanal = mainCompany() ->getselcanales();
        BcCentroCosteSeleccionarView *scoste = mainCompany() ->getselccostes();
        QString ccostes = scoste->cadcoste();
        if ( ccostes != "" ) {
            ccostes.sprintf ( " AND t5.idc_coste IN (%s) ", ccostes.toLatin1().constData() );
        } // end if
        QString ccanales = scanal->cadCanal();
        if ( ccanales != "" ) {
            ccanales.sprintf ( " AND idcanal IN (%s) ", ccanales.toLatin1().constData() );
        } // end if
*/
        QString tabla;
        QString cont;
        if ( mui_incluirAsientosAbiertos->isChecked() ) {
            tabla = "borrador";
            cont = " false AS punteo, * ";
        } else {
            tabla = "apunte";
            cont = " * ";
        } // end if

        query = "SELECT * FROM ((SELECT " + cont + ",idc_coste AS centrocoste FROM " + tabla + " WHERE  idcuenta = " + idcuenta + " AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' " + ccostes + " " + ccanales + " " + tipopunteo + ") AS t2 LEFT JOIN cuenta ON t2.idcuenta = cuenta.idcuenta) AS t1 LEFT JOIN asiento ON asiento.idasiento = t1.idasiento ";
        query += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste FROM c_coste) AS t5 ON t5.idc_coste = t1.centrocoste ";
        query += " LEFT JOIN (SELECT idcanal, nombre AS nombrecanal FROM canal) AS t6 ON t6.idcanal = t1.idcanal ";

        if ( !mui_incluirAsientosAbiertos->isChecked() ) {
            query += " LEFT JOIN (SELECT idapunte AS id, idcuenta AS idcuentacontra, codigo AS codcontrapartida, descripcion AS desccontrapartida FROM contrapart LEFT JOIN cuenta ON contrapart.contra = cuenta.idcuenta) AS t7 ON t7.id = t1.idapunte ";
        }

        query += " ORDER BY t1.fecha, ordenasiento, t1.orden"+tabla;

        cursorapt = mainCompany() ->loadQuery ( query );
        if ( !cursorapt ) throw - 1;

        /// Cargamos los saldos iniciales.
        BlDbRecordSet *cursoraux;
        query = "SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM " + tabla + " WHERE idcuenta =" + idcuenta + " AND fecha < '" + finicial + "'";
        cursoraux = mainCompany() ->loadQuery ( query );
        if ( !cursoraux ) {
            delete cursorapt;
            throw - 1;
        } // end if
        if ( !cursoraux->eof() ) {
            debeinicial = BlFixed ( cursoraux->value( "tdebe" ) );
            haberinicial = BlFixed ( cursoraux->value( "thaber" ) );
            saldoinicial = debeinicial - haberinicial;
            debefinal = debeinicial;
            haberfinal = haberinicial;
            saldofinal = saldoinicial;
        } // end if
        delete cursoraux;

        /// Presentamos la informacion inicial y de la cuenta.
        BlDbRecordSet *cursorcta = mainCompany() ->loadQuery ( "SELECT * FROM cuenta WHERE idcuenta=" + idcuenta );
        if ( !cursorcta ) throw - 1;


        salida += "<blockTable style=\"tabla\">\n";
        salida += "<tr>";
        salida += "<td> " + cursorcta->value( "codigo" ) + " -" + cursorcta->value( "descripcion" ) + " </td>";
        salida += "<td> Debe inicial: " + debeinicial.toQString() + " </td>";
        salida += "<td> Haber inicial: " + haberinicial.toQString() + " </td>";
        salida += "<td> Saldo inicial: " + saldoinicial.toQString() + "</td>";
        salida += "</tr>";
        salida += "</blockTable>\n";

        delete cursorcta;
        /// Aqui va la impresion pero con cabeceras.

        salida += "<blockTable style=\"tablaextracto\" repeatRows=\"1\">\n";
        salida += "<tr>\n";
        salida += "<td> Fecha </td>";
        salida += "<td> Asiento </td>";
        salida += "<td> Concepto </td>";
        salida += "<td> Debe </td>";
        salida += "<td> Haber </td>";
        salida += "<td> Saldo </td>";
        if ( !mui_incluirAsientosAbiertos->isChecked() ) {
            salida += "<td> Contrapartida </td>";
        } // end if
        salida += "</tr>\n";


        salida +=  "<tr>\n";
        salida +=  "<td></td>";
        salida +=  "<td></td>";
        salida +=  "<td> Inicial:</td>";
        salida += "<td>" + debeinicial.toQString() + " </td>\n";
        salida += "<td>" + haberinicial.toQString() + " </td>\n";
        salida += "<td>" + saldoinicial.toQString() + "</td>\n";
        salida +=  "<td></td>";
        salida +=  "</tr>\n";

        while ( ! cursorapt->eof() ) {

            debefinal = debefinal + BlFixed ( cursorapt->value( "debe" ) );
            haberfinal = haberfinal + BlFixed ( cursorapt->value( "haber" ) );
            saldofinal = debefinal - haberfinal;

            salida +=  "<tr>\n";
            salida +=  "<td>" + cursorapt->value( "fecha" ) + "</td>";
            salida +=  "<td>" + cursorapt->value( "ordenasiento" ) + "</td>";
            salida +=  "<td>" + cursorapt->value( "conceptocontable" ) + "-" + cursorapt->value( "descripcion" ) + "</td>";
            salida +=  "<td>" + cursorapt->value( "debe" ) + "</td>";
            salida +=  "<td>" + cursorapt->value( "haber" ) + "</td>";
            salida +=  "<td>" + saldofinal.toQString() + "</td>";
            if ( !mui_incluirAsientosAbiertos->isChecked() ) {
                salida +=  "<td>" + cursorapt->value( "codcontrapartida" ) + " - " + cursorapt->value( "desccontrapartida" ) + "</td>";
            } // end if
            salida +=  "</tr>\n";
            cursorapt->nextRecord();
        } // end while



        salida +=  "<tr>\n";
        salida +=  "<td></td>";
        salida +=  "<td></td>";
        salida +=  "<td> Final:</td>";
        salida += "<td>" + debefinal.toQString() + " </td>\n";
        salida += "<td>" + haberfinal.toQString() + " </td>\n";
        salida += "<td>" + saldofinal.toQString() + "</td>\n";
        salida +=  "<td></td>";
        salida +=  "</tr>\n";

        salida += "</blockTable>\n";

        salida += "<spacer length=\"5mm\" width=\"1mm\"/>\n";

        delete cursorapt;
        
        return salida;
    } catch ( ... ) {
        blMsgError ( _("Ocurrio un error inesperado.") );
        return "";
    }
}


void BcExtractoView::imprimir() {
       BL_FUNC_DEBUG
       recogeValores();
        /// El calculo de los canales
        QString ccanales = "";
	g_plugins->run("PgetSelCanales", &ccanales);
	if (ccanales != "") ccanales = ","+ccanales;
	bool sincanal = true;
	g_plugins->run("PgetSinCanal", &sincanal);
	setVar("canales", "(" + QString(sincanal?" idcanal IS NULL OR ":"")+" idcanal IN (0"+ccanales+") )");
	QString ccostes = "";
	g_plugins->run("PgetSelCostes", &ccostes);
        if ( ccostes != "" ) {
            ccostes.sprintf ( " AND idc_coste IN (%s) ", ccostes.toLatin1().constData() );
        } // end if
        setVar("ccostes", ccostes);
       BfForm::imprimir();
}


/// Slot que responde a la Impresion del extracto
/**
**/
/*
void BcExtractoView::imprimir()
{
    BL_FUNC_DEBUG
    QString finicial = mui_fechaInicial->text();
    QString ffinal = mui_fechaFinal->text();

    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "extracto.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "extracto.rml";
    blCopyFile(archivo, archivod);

    /// Copiamos el logo.
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    blCopyFile(archivologo, logousuario);
    
    BlFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";

    QString codinicial = mui_cuentaInicial->fieldValue("codigo");
    QString codfinal = mui_cuentaFinal->fieldValue("codigo");

    QString query;


    /// Preparamos la barra de progreso
    BlProgressBar *barra = new BlProgressBar;
    barra->setValue ( 0 );
    barra->show();
    barra->setText ( _ ( "Generando extracto " )  );


    /// Tabla temporal de contrapartidas.
    QString query1 = "CREATE TEMPORARY TABLE contrapart AS select idapunte, ccontrapartida(idapunte) AS contra FROM apunte LEFT JOIN cuenta ON cuenta.idcuenta = apunte.idcuenta WHERE cuenta.codigo >= '" + codinicial + "' AND cuenta.codigo <= '" + codfinal + "'";
    mainCompany()->runQuery ( query1 );


    /// Si los datos de c&oacute;digo inicial y final est&aacute;n vacios los ponemos
    /// nosotros.
    if ( codinicial == "" ) {
        codinicial = "0";
    } // end if
    if ( codfinal == "" ) {
        codfinal = "9999999";
    } // end if

    fitxersortidatxt += "<blockTable>\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "<td> Fecha Inicial: " + finicial + " </td>";
    fitxersortidatxt += "<td> Fecha Final: " + ffinal + " </td>";
    fitxersortidatxt += "</tr>\n";
    fitxersortidatxt += "</blockTable>\n";

    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM apunte) AND codigo >= '" + codinicial + "' AND codigo <= '" + codfinal + "' ORDER BY codigo";
    BlDbRecordSet *curcta = mainCompany() ->loadQuery ( query );
    if ( !curcta ) {
      delete barra;
      return;
    } // end if
    barra->setRange ( 0, curcta->numregistros() );
    int i = 0;
    while ( ! curcta->eof() ) {
        fitxersortidatxt += imprimeExtractoCuenta ( curcta->value( "idcuenta" ) );
        curcta->nextRecord();
        barra->setValue ( i++ );
    }// end while
    delete curcta;

    /// Tabla temporal de contrapartidas.
    query1 = "DROP TABLE contrapart";
    mainCompany()->runQuery ( query1 );

    delete barra;

    buff.replace ( "[story]", fitxersortidatxt );
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    /// Crea el pdf y lo muestra.
    blCreateAndLoadPDF ( "extracto" );
    
    return;
}
*/

void BcExtractoView::on_mui_inicio_released() {
      BL_FUNC_DEBUG
      botonInicio();
}

void BcExtractoView::on_mui_anterior_released() {
      BL_FUNC_DEBUG
      botonAnterior();
}

void BcExtractoView::on_mui_siguiente_released() {
      BL_FUNC_DEBUG
      botonSiguiente();
}

void BcExtractoView::on_mui_fin_released() {
      BL_FUNC_DEBUG
      botonFin();
}

void BcExtractoView::on_mui_filtrar_toggled(bool check) {
  BL_FUNC_DEBUG
  frame->setVisible(check);
}

void BcExtractoView::cuentaIChanged ( QString val) {
  BL_FUNC_DEBUG
  mui_cuentaFinal->setId(val);
}
