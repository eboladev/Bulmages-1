/***************************************************************************
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

#include <QLineEdit>
#include <QWidget>
#include <QToolButton>
#include <QLayout>
#include <QTextStream>
#include <QCheckBox>
#include <QFileDialog>

#include "blmainwindow.h"
#include "bcbulmacont.h"
#include "bcextractoview.h"
#include "bccompany.h"
#include "bcasientoview.h"
#include "bccentrocosteseleccionarview.h"
#include "bccanalseleccionarview.h"
#include "bldatesearch.h"
#include "bcextractosubform.h"
#include "blprogressbar.h"
#include "blfunctions.h"

/// Este archivo contiene la implementaci&oacute;n de la clase BcExtractoView que saca el
/// extracto por pantalla de una o varias cuentas determinadas. Esta clase es una de las
/// tres principales junto a \ref introapuntes1 y \ref diarioview1.
/**
\param emp
\param parent
**/
BcExtractoView::BcExtractoView ( BcCompany *emp, QWidget *parent, int ) : BcForm ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );

    setTitleName ( _ ( "Extracto de Cuentas" ) );
    setDbTableName ( "apunte" );

    mui_list->setMainCompany ( emp );

    connect (mui_list, SIGNAL(openAsiento()), this, SLOT(openAsiento()) );

    /// Iniciamos los componentes
    m_codigoinicial->setMainCompany ( emp );
    m_codigoinicial->setLabel ( _ ( "Cuenta Inicial:" ) );
    m_codigoinicial->setTableName ( "cuenta" );
    m_codigoinicial->setFieldId("idcuenta");
    m_codigoinicial->m_valores["descripcion"] = "";
    m_codigoinicial->m_valores["codigo"] = "";

    m_codigofinal->setMainCompany ( emp );
    m_codigofinal->setLabel ( _ ( "Cuenta Final:" ) );
    m_codigofinal->setTableName ( "cuenta" );
    m_codigofinal->setFieldId("idcuenta");
    m_codigofinal->m_valores["descripcion"] = "";
    m_codigofinal->m_valores["codigo"] = "";

    mui_codigocontrapartida->setMainCompany ( emp );
    mui_codigocontrapartida->setLabel ( _ ( "Cuenta Contrapartida:" ) );
    mui_codigocontrapartida->setTableName ( "cuenta" );
    mui_codigocontrapartida->setFieldId("idcuenta");
    mui_codigocontrapartida->m_valores["descripcion"] = "";
    mui_codigocontrapartida->m_valores["codigo"] = "";


    m_codigoinicial->hideLabel();
    m_codigofinal->hideLabel();
    mui_codigocontrapartida->hideLabel();

    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    QString cadena;
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year() );
    m_fechainicial1->setText ( cadena );
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year() );
    m_fechafinal1->setText ( cadena );
    m_cursorcta = NULL;
    insertWindow ( windowTitle(), this );

    m_tratarpunteos = TRUE;


    /// Conectamos los botones del menu con las acciones de esta ventana.
    connect (((BcBulmaCont *)g_main)->actionSiguiente, SIGNAL(triggered()), this, SLOT(botonSiguiente()));
    connect (((BcBulmaCont *)g_main)->actionAnterior, SIGNAL(triggered()), this, SLOT(botonAnterior()));
    connect (((BcBulmaCont *)g_main)->actionInicio, SIGNAL(triggered()), this, SLOT(botonInicio()));
    connect (((BcBulmaCont *)g_main)->actionFin, SIGNAL(triggered()), this, SLOT(botonFin()));

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

    int resur = g_plugins->lanza ( "SNewBcAsientoView", (BcCompany *) mainCompany() );
    
    if ( ! resur) {
        blMsgInfo("No se pudo crear instancia de asientos");
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
    QString codinicial = m_codigoinicial->fieldValue("codigo");
    QString codfinal = m_codigofinal->fieldValue("codigo");
    QString query;
    /// Si los datos de c&oacute;digo inicial y final est&aacute;n vacios los ponemos
    /// nosotros.
    if ( codinicial == "" ) {
        codinicial = "0";
    } // end if
    if ( codfinal == "" ) {
        codfinal = "9999999";
    } // end if
    
    if ( mui_asAbiertos->isChecked() ) {
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
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
      if ( m_cursorcta != NULL ) {
          if ( !m_cursorcta->isLastRecord() ) {
              save();
              m_cursorcta->nextRecord();
              presentar();
          } // end if
      } // end if
    } // end if
    
}


/// Esta es la funci&oacute;n que retrocede un registro entre las cuentas.
/**
**/
void BcExtractoView::botonAnterior()
{
    BL_FUNC_DEBUG
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
      if ( m_cursorcta != NULL ) {
          if ( !m_cursorcta->isFirstRecord() ) {
              save();
              m_cursorcta->previousRecord();
              presentar();
          } // end if
      } // end if
    } // end if
    
}

/// Retrocede al principio de las cuentas.
/**
**/
void BcExtractoView::botonInicio()
{
    BL_FUNC_DEBUG
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
      if ( m_cursorcta != NULL ) {
          save();
          m_cursorcta->firstRecord();
          presentar();
      } // end if
    } // end if
    
}

/// Avanza al final de las cuentas.
/**
**/
void BcExtractoView::botonFin()
{
    BL_FUNC_DEBUG
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
      if ( m_cursorcta != NULL ) {
          save();
          m_cursorcta->lastRecord();
          presentar();
      } // end if
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
        QString finicial = m_fechainicial1->text().toAscii().constData();
        QString ffinal = m_fechafinal1->text().toAscii().constData();
        libromayor.inicializa1 ( m_codigoinicial->text(), m_codigofinal->text(), finicial, ffinal );
        libromayor.inicializa2 ( ( char * ) fn.toAscii().constData() );
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
    inicialdebe->setText ( "0" );
    inicialhaber->setText ( "0" );
    inicialsaldo->setText ( "0" );
    totaldebeparcial->setText ( "0" );
    totalhaberparcial->setText ( "0" );
    totalsaldoparcial->setText ( "0" );
    totaldebe->setText ( "0" );
    totalhaber->setText ( "0" );
    totalsaldo->setText ( "0" );
    
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
	if (! mui_asAbiertos->isChecked() ) 
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
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString contra = mui_codigocontrapartida->fieldValue("codigo");
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
        if ( mui_punteotodos->isChecked() ) {
            tipopunteo = "";
        } else if ( mui_punteopunteado->isChecked() ) {
            tipopunteo = " AND punteo = TRUE ";
        } else {
            tipopunteo = " AND punteo = FALSE ";
        } // end if


        if ( m_cursorcta->eof() || m_cursorcta->bof() )
            return;

        idcuenta = m_cursorcta->value( "idcuenta" );
        /// Escribimos el nombre de la cuenta y el c&oacute;digo de la misma.
        codigocuenta->setText ( m_cursorcta->value( "codigo" ) );
        nombrecuenta->setText ( m_cursorcta->value( "descripcion" ) );
        /// Hacemos la consulta de los apuntes a listar en la base de datos.
        QString query = "";
        /// Al igual que en el caso anterior los centros de coste han cambiado y a&uacute;n
        /// no se pueden implementar.
        BcCanalSeleccionarView *scanal = mainCompany() ->getselcanales();
        BcCentroCosteSeleccionarView *scoste = mainCompany() ->getselccostes();
        QString ccostes = scoste->cadcoste();
        if ( ccostes != "" ) {
            ccostes.sprintf ( " AND idc_coste IN (%s) ", ccostes.toAscii().constData() );
        } // end if
        QString ccanales = scanal->cadCanal();
        if ( ccanales != "" ) {
            ccanales.sprintf ( " AND idcanal IN (%s) ", ccanales.toAscii().constData() );
        } // end if
        QString tabla;
        QString cont;
        if ( mui_asAbiertos->isChecked() ) {
            tabla = "borrador";
            cont = " FALSE AS punteo, *, descripcion AS descripcionapunte ";
        } else {
            tabla = "apunte";
            cont = " *, descripcion AS descripcionapunte ";
        } // end if

        if ( contra != "" ) {
            tipopunteo += " AND " + tabla + ".contrapartida = id_cuenta('" + contra + "') ";
        } // end if
        bool ok = FALSE;
        mui_saldosup->text().toFloat ( &ok );
        if ( ok ) {
            saldosup = " AND " + tabla + ".debe + " + tabla + ".haber >= " + mui_saldosup->text();
        } // end if
        ok = FALSE;
        mui_saldoinf->text().toFloat ( &ok );
        if ( ok ) {
            saldoinf = " AND " + tabla + ".debe + " + tabla + ".haber <= " + mui_saldoinf->text();
        } // end if
        QString clase = "";
        if (mui_apcierre->isChecked()) {
          clase = " AND clase < 2 ";
        } // end if
        
        query = "SELECT * FROM ((SELECT " + cont + " FROM " + tabla + " WHERE  idcuenta = " + idcuenta + " AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' " + ccostes + " " + ccanales + " " + tipopunteo + saldosup + saldoinf + ") AS t2 ";
        query += " LEFT JOIN (SELECT idcuenta AS idc, descripcion, codigo, tipocuenta FROM cuenta) AS t9 ON t2.idcuenta = t9.idc) AS t1";
        query += " LEFT JOIN (SELECT *, descripcion AS descripcionasiento FROM asiento) AS t4 ON t4.idasiento = t1.idasiento ";
        query += " LEFT JOIN (SELECT idc_coste AS idccoste, nombre AS nombrec_coste FROM c_coste) AS t5 ON t5.idccoste = t1.idc_coste ";
        query += " LEFT JOIN (SELECT idcanal AS id_canal, nombre AS nombrecanal FROM canal) AS t6 ON t6.id_canal = t1.idcanal ";
        query += " LEFT JOIN (SELECT idcuenta AS idcontrapartida, codigo AS codcontrapartida FROM cuenta) as t8 ON t8.idcontrapartida = t1.contrapartida WHERE 1=1 " + clase;
        query += " ORDER BY t1.fecha, ordenasiento, t1.orden";

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
            inicialdebe->setText ( debeinicial.toQString() );
            inicialhaber->setText ( haberinicial.toQString() );
            inicialsaldo->setText ( saldoinicial.toQString() );
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
            totaldebe->setText ( debefinal.toQString() );
            totalhaber->setText ( haberfinal.toQString() );
            totalsaldo->setText ( saldofinal.toQString() );
            totaldebeparcial->setText ( debeparcial.toQString() );
            totalhaberparcial->setText ( haberparcial.toQString() );
            totalsaldoparcial->setText ( saldoparcial.toQString() );
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
    m_codigoinicial->setText ( codinicial );
    m_codigofinal->setText ( codfinal );
    m_fechainicial1->setText ( blNormalizeDate ( fecha1 ).toString ( "dd/MM/yyyy" ) );
    m_fechafinal1->setText ( blNormalizeDate ( fecha2 ).toString ( "dd/MM/yyyy" ) );
    
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
	
        QString query = "SELECT * FROM apunte WHERE punteo = FALSE AND haber <> 0 AND idcuenta = " + m_cursorcta->value( "idcuenta" ) + " ORDER BY fecha";
        BlDbRecordSet *curshaber = mainCompany() ->loadQuery ( query );

        BlProgressBar barra;
        barra.setRange ( 0, curshaber->numregistros() );
        barra.show();
        barra.setText ( _ ( "Cargando Extracto de Cuentas" ) );

        while ( !curshaber->eof() ) {
            query =  "SELECT * FROM apunte WHERE punteo = FALSE AND debe = " + curshaber->value( "haber" ) + " AND idcuenta = " + m_cursorcta->value( "idcuenta" ) + " ORDER BY fecha";
            BlDbRecordSet *cursdebe = mainCompany() ->loadQuery ( query.toAscii(), "cursdebe" );

            if ( !cursdebe->eof() ) {
                query = "UPDATE apunte set punteo = TRUE WHERE idapunte = " + curshaber->value( "idapunte" );
                mainCompany() ->begin();
                mainCompany() ->runQuery ( query );
                query = "UPDATE apunte SET punteo = TRUE WHERE idapunte = " + cursdebe->value( "idapunte" );
                mainCompany() ->runQuery ( query );
                mainCompany() ->commit();
            } // end if

            delete cursdebe;
            curshaber->nextRecord();
            barra.setValue ( barra.value() + 1 );
        } // end while

        delete curshaber;
	m_tratarpunteos = FALSE;
        presentar();
	m_tratarpunteos = TRUE;
    } catch ( ... ) {
        blMsgError ( "Se produjo un error en la casacion" );
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
        mifile = fopen ( ( char * ) fn.toAscii().constData(), "wt" );
        if ( mifile != NULL ) {
            QString query;
            query = "SELECT * FROM apunte WHERE punteo = TRUE";
            BlDbRecordSet *cursp = mainCompany() ->loadQuery ( query );
            while ( !cursp->eof() ) {
                fprintf ( mifile, "%s\n", cursp->value( "idapunte" ).toAscii().constData() );
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
            mainCompany() ->runQuery ( "UPDATE apunte SET punteo = FALSE WHERE idcuenta =" + m_cursorcta->value( "idcuenta" ) );
            mainCompany() ->commit();

	    m_tratarpunteos = FALSE;
	    presentar();
	    m_tratarpunteos = TRUE;
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Se ha producido un error" ) );
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
            QFile file ( fn );
            if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
                return;
            } // end if
            QTextStream filestr ( &file );
            mainCompany()->begin();
            QString query = "UPDATE apunte SET punteo = FALSE";
            mainCompany() ->runQuery ( query );
            QString a = filestr.readLine();
            while ( !a.isNull() ) {
                QString query;
                query = "UPDATE apunte SET punteo = TRUE WHERE idapunte = " + a;
                mainCompany() ->runQuery ( query );
                a = filestr.readLine();
            } // end while
            mainCompany()->commit();
            file.close();
        } // end if
	m_tratarpunteos = FALSE;
        presentar();
	m_tratarpunteos = TRUE;
    } catch ( ... ) {
        blMsgInfo ( "Error en la carga del punteo" );
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
        QString finicial = m_fechainicial1->text();
        QString ffinal = m_fechafinal1->text();
        QString contra = mui_codigocontrapartida->fieldValue("codigo");

        /// Preparamos el string para que aparezca una u otra cosa seg&uacute;n el punteo.
        QString tipopunteo;
        tipopunteo = "";
        if ( mui_punteotodos->isChecked() ) {
            tipopunteo = "";
        } else if ( mui_punteopunteado->isChecked() ) {
            tipopunteo = " AND punteo = TRUE ";
        } else {
            tipopunteo = " AND punteo = FALSE ";
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
        BcCanalSeleccionarView *scanal = mainCompany() ->getselcanales();
        BcCentroCosteSeleccionarView *scoste = mainCompany() ->getselccostes();
        QString ccostes = scoste->cadcoste();
        if ( ccostes != "" ) {
            ccostes.sprintf ( " AND t5.idc_coste IN (%s) ", ccostes.toAscii().constData() );
        } // end if
        QString ccanales = scanal->cadCanal();
        if ( ccanales != "" ) {
            ccanales.sprintf ( " AND idcanal IN (%s) ", ccanales.toAscii().constData() );
        } // end if
        QString tabla;
        QString cont;
        if ( mui_asAbiertos->isChecked() ) {
            tabla = "borrador";
            cont = " FALSE AS punteo, * ";
        } else {
            tabla = "apunte";
            cont = " * ";
        } // end if

        query = "SELECT * FROM ((SELECT " + cont + ",idc_coste AS centrocoste FROM " + tabla + " WHERE  idcuenta = " + idcuenta + " AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' " + ccostes + " " + ccanales + " " + tipopunteo + ") AS t2 LEFT JOIN cuenta ON t2.idcuenta = cuenta.idcuenta) AS t1 LEFT JOIN asiento ON asiento.idasiento = t1.idasiento ";
        query += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste FROM c_coste) AS t5 ON t5.idc_coste = t1.centrocoste ";
        query += " LEFT JOIN (SELECT idcanal, nombre AS nombrecanal FROM canal) AS t6 ON t6.idcanal = t1.idcanal ";

        if ( !mui_asAbiertos->isChecked() ) {
            query += " LEFT JOIN (SELECT idapunte AS id, idcuenta AS idcuentacontra, codigo AS codcontrapartida, descripcion AS desccontrapartida FROM contrapart LEFT JOIN cuenta ON contrapart.contra = cuenta.idcuenta) AS t7 ON t7.id = t1.idapunte ";
        }

        query += " ORDER BY t1.fecha, ordenasiento, t1.orden";

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
        salida += "<td> Debe Inicial: " + debeinicial.toQString() + " </td>";
        salida += "<td> Haber Inicial: " + haberinicial.toQString() + " </td>";
        salida += "<td> Saldo Inicial: " + saldoinicial.toQString() + "</td>";
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
        if ( !mui_asAbiertos->isChecked() ) {
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
            if ( !mui_asAbiertos->isChecked() ) {
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
        blMsgError ( "Ocurrio un error inesperado" );
        return "";
    }
}





/// Slot que responde a la Impresion del extracto
/**
**/
void BcExtractoView::imprimir()
{
    BL_FUNC_DEBUG
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();

    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "extracto.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "extracto.rml";
    blCopyFile(archivo, archivod);

    /// Copiamos el logo.
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    blCopyFile(archivologo, logousuario);
    
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";

    QString codinicial = m_codigoinicial->fieldValue("codigo");
    QString codfinal = m_codigofinal->fieldValue("codigo");

    QString query;


    /// Preparamos la barra de progreso
    BlProgressBar *barra = new BlProgressBar;
    barra->setValue ( 0 );
    barra->show();
    barra->setText ( _ ( "Generando Extracto " )  );


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



