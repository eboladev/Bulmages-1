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

#include "extractoview1.h"
#include "extractoprintview.h"
#include "libromayorprint.h"
#include "bccompany.h"
#include "asiento1view.h"
#include "diarioview.h"
#include "listcuentasview1.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "busquedafecha.h"
#include "busquedacuenta.h"
#include "extractosubform.h"
#include "blprogressbar.h"

/// Este archivo contiene la implementaci&oacute;n de la clase extractoview1 que saca el
/// extracto por pantalla de una o varias cuentas determinadas. Esta clase es una de las
/// tres principales junto a \ref introapuntes1 y \ref diarioview1.
/**
\param emp
\param parent
**/
extractoview1::extractoview1 ( BcCompany *emp, QWidget *parent, int ) : FichaBc ( emp, parent )
{
    _depura ( "extractoview1::extractoview1", 0 );
    setupUi ( this );

    setTitleName ( _( "Extracto de Cuentas" ) );
    setDbTableName ( "apunte" );

    mui_list->setMainCompany ( emp );

    /// Iniciamos los componentes
    m_codigoinicial->setMainCompany ( emp );
    m_codigofinal->setMainCompany ( emp );
    mui_codigocontrapartida->setMainCompany ( emp );
    m_codigoinicial->hideNombre();
    m_codigofinal->hideNombre();

    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    QString cadena;
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year() );
    m_fechainicial1->setText ( cadena );
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year() );
    m_fechafinal1->setText ( cadena );
    m_cursorcta = NULL;
    meteWindow ( windowTitle(), this );
    _depura ( "END extractoview1::extractoview1", 0 );
}


/// Destructor de la clase.
/**
**/
extractoview1::~extractoview1()
{
    _depura ( "extractoview1::~extractoview1", 0 );
   guardar();
    delete m_cursorcta;
    mainCompany() ->sacaWindow ( this );
    _depura ( "END extractoview1::~extractoview1", 0 );
}


///
/**
\param columna
**/
void extractoview1::on_mui_list_cellDoubleClicked ( int, int columna )
{
    _depura ( "asientosview::on_mui_list_cellDoubleClicked", 0 );
    QString textoHeader;
    textoHeader =  mui_list->cabecera() ->at ( columna ) ->nomcampo().toAscii();
    if ( textoHeader == "ordenasiento" ) {
        QString idasiento = mui_list->DBvalue ( "idasiento" );
        mainCompany() ->intapuntsempresa() ->muestraasiento ( idasiento );
        mainCompany() ->intapuntsempresa() ->show();
        mainCompany() ->intapuntsempresa() ->setFocus();
        mainCompany() ->muestraapuntes1();
    } // end if
    _depura ( "END asientosview::on_mui_list_cellDoubleClicked", 0 );
}


///
/**
**/
void extractoview1::on_mui_actualizar_clicked()
{
    _depura ( "extractoview1::on_mui_actualizar_clicked", 0 );
    accept();
    _depura ( "END extractoview1::on_mui_actualizar_clicked", 0 );
}


///
void extractoview1::on_mui_configurar_clicked()
{
    _depura ( "extractoview1::on_mui_configurar_clicked", 0 );
    mui_list->showConfig();
    _depura ( "END extractoview1::on_mui_configurar_clicked", 0 );
}


/// Esta funci&oacute;n carga el cursor de cuentas que forman el todo por el todo.
/** Tambi&eacute;n ser&aacute; la encargada de recoger la informaci&oacute;n de
    filtraci&oacute;n para que todo sea correcto. */
/**
**/
void extractoview1::accept()
{
    _depura ( "extractoview1::accept", 0 );
    QString codinicial = m_codigoinicial->codigocuenta();
    QString codfinal = m_codigofinal->codigocuenta();
    QString query;
    /// Si los datos de c&oacute;digo inicial y final est&aacute;n vacios los ponemos
    /// nosotros.
    if ( codinicial == "" ) {
        codinicial = "0";
    } // end if
    if ( codfinal == "" ) {
        codfinal = "9999999";
    } // end if
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM apunte) AND codigo >= '" + codinicial + "' AND codigo <= '" + codfinal + "' ORDER BY codigo";

    if ( m_cursorcta != NULL ) {
        delete m_cursorcta;
    } // end if
    m_cursorcta = mainCompany() ->loadQuery ( query );
    presentar();
    _depura ( "END extractoview1::accept", 0 );
}


/// Esta es la funci&oacute;n que avanza un registro entre las cuentas.
/**
**/
void extractoview1::boton_siguiente()
{
    _depura ( "extractoview1::boton_siguiente", 0 );
    if ( m_cursorcta != NULL ) {
        if ( !m_cursorcta->esultimoregistro() ) {
            guardar();
            m_cursorcta->siguienteregistro();
            presentar();
        } // end if
    } // end if
    _depura ( "END extractoview1::boton_siguiente", 0 );
}


/// Esta es la funci&oacute;n que retrocede un registro entre las cuentas.
/**
**/
void extractoview1::boton_anterior()
{
    _depura ( "extractoview1::boton_anterior", 0 );
    if ( m_cursorcta != NULL ) {
        if ( !m_cursorcta->esprimerregistro() ) {
            guardar();
            m_cursorcta->registroanterior();
            presentar();
        } // end if
    } // end if
    _depura ( "END extractoview1::boton_anterior", 0 );
}

/// Retrocede al principio de las cuentas.
/**
**/
void extractoview1::boton_inicio()
{
    _depura ( "extractoview1::boton_inicio", 0 );
    if ( m_cursorcta != NULL ) {
        guardar();
        m_cursorcta->primerregistro();
        presentar();
    } // end if
    _depura ( "END extractoview1::boton_inicio", 0 );
}

/// Avanza al final de las cuentas.
/**
**/
void extractoview1::boton_fin()
{
    _depura ( "extractoview1::boton_fin", 0 );
    if ( m_cursorcta != NULL ) {
        guardar();
        m_cursorcta->ultimoregistro();
        presentar();
    } // end if
    _depura ( "END extractoview1::boton_fin", 0 );
}


/// Imprime el extracto
/**
**/
void extractoview1::boton_imprimir()
{
    _depura ( "extractoview1::boton_imprimir", 0 );
    ExtractoPrintView *print = new ExtractoPrintView ( mainCompany(), 0 );
    print->exec();
    _depura ( "END extractoview1::boton_imprimir", 0 );
}


///
/**
**/
void extractoview1::boton_guardar()
{
    _depura ( "extractoview1::boton_guardar", 0 );
    QString fn = QFileDialog::getSaveFileName ( this,
                 _( "Guardar libro diario" ),
                 confpr->valor ( CONF_DIR_USER ),
                 _( "Diarios (*.txt)" ) );

    if ( !fn.isEmpty() ) {
        libromayorprint libromayor ( mainCompany() );
        QString finicial = m_fechainicial1->text().toAscii().constData();
        QString ffinal = m_fechafinal1->text().toAscii().constData();
        libromayor.inicializa1 ( m_codigoinicial->text(), m_codigofinal->text(), finicial, ffinal );
        libromayor.inicializa2 ( ( char * ) fn.toAscii().constData() );
        libromayor.accept();
    } // end if
    _depura ( "END extractoview1::boton_guardar", 0 );
}


/// Limpia los totales
/**
**/
void extractoview1::vaciar()
{
    _depura ( "extractoview1::vaciar", 0 );
    inicialdebe->setText ( "0" );
    inicialhaber->setText ( "0" );
    inicialsaldo->setText ( "0" );
    totaldebe->setText ( "0" );
    totalhaber->setText ( "0" );
    totalsaldo->setText ( "0" );
    _depura ( "END extractoview1::vaciar", 0 );
}


///
/**
**/
void extractoview1::ajustes()
{
    _depura ( "extractoview1::ajustes", 0 );
    _depura ( "END extractoview1::ajustes", 0 );
}


///
/**
**/
int extractoview1::guardar()
{
    _depura ( "extractoview1::on_mui_guardar_clicked", 0 );
    if ( mui_asAbiertos->isChecked() ) return 0;
    mui_list->guardar();
    return 0;
    _depura ( "END extractoview1::on_mui_guardar_clicked", 0 );

}


/// Esta funci&oacute;n se encarga de montar la consulta que va a hacer a la base de datos.
/**
\return
**/
void extractoview1::presentar()
{
    _depura ( "extractoview1::presentar", 0 );
    BlFixed debe ( "0.00" ), haber ( "0.00" ), saldo ( "0.00" );
    BlFixed debeinicial ( "0.00" ), haberinicial ( "0.00" ), saldoinicial ( "0.00" );
    BlFixed debefinal ( "0.00" ), haberfinal ( "0.00" ), saldofinal ( "0.00" );
    QString idcuenta;
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString contra = mui_codigocontrapartida->text();
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

        idcuenta = m_cursorcta->valor ( "idcuenta" );
        /// Escribimos el nombre de la cuenta y el c&oacute;digo de la misma.
        codigocuenta->setText ( m_cursorcta->valor ( "codigo" ) );
        nombrecuenta->setText ( m_cursorcta->valor ( "descripcion" ) );
        /// Hacemos la consulta de los apuntes a listar en la base de datos.
        QString query = "";
        /// Al igual que en el caso anterior los centros de coste han cambiado y a&uacute;n
        /// no se pueden implementar.
        selectcanalview *scanal = mainCompany() ->getselcanales();
        SelectCCosteView *scoste = mainCompany() ->getselccostes();
        QString ccostes = scoste->cadcoste();
        if ( ccostes != "" ) {
            ccostes.sprintf ( " AND idc_coste IN (%s) ", ccostes.toAscii().constData() );
        } // end if
        QString ccanales = scanal->cadcanal();
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
        float ssup = mui_saldosup->text().toFloat ( &ok );
        if ( ok ) {
            saldosup = " AND " + tabla + ".debe + " + tabla + ".haber >= " + mui_saldosup->text();
        } // end if
        ok = FALSE;
        float sinf = mui_saldoinf->text().toFloat ( &ok );
        if ( ok ) {
            saldoinf = " AND " + tabla + ".debe + " + tabla + ".haber <= " + mui_saldoinf->text();
        } // end if
        query = "SELECT * FROM ((SELECT " + cont + " FROM " + tabla + " WHERE  idcuenta = " + idcuenta + " AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' " + ccostes + " " + ccanales + " " + tipopunteo + saldosup + saldoinf + ") AS t2 ";
        query += " LEFT JOIN (SELECT idcuenta AS idc, descripcion, codigo, tipocuenta FROM cuenta) AS t9 ON t2.idcuenta = t9.idc) AS t1";
        query += " LEFT JOIN (SELECT *, descripcion AS descripcionasiento FROM asiento) AS t4 ON t4.idasiento = t1.idasiento ";
        query += " LEFT JOIN (SELECT idc_coste AS idccoste, nombre AS nombrec_coste FROM c_coste) AS t5 ON t5.idccoste = t1.idc_coste ";
        query += " LEFT JOIN (SELECT idcanal AS id_canal, nombre AS nombrecanal FROM canal) AS t6 ON t6.id_canal = t1.idcanal ";
        query += " LEFT JOIN (SELECT idcuenta AS idcontrapartida, codigo AS codcontrapartida FROM cuenta) as t8 ON t8.idcontrapartida = t1.contrapartida";
        query += " ORDER BY t1.fecha, ordenasiento, t1.orden";

        mui_list->cargar ( query );


        cursorapt = mainCompany() ->loadQuery ( query );
        cursorapt->primerregistro();
        if ( !cursorapt->eof() ) {
            /// Cargamos los saldos iniciales.
            query = "SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE idcuenta =" + idcuenta + " AND fecha < '" + finicial + "'";
            cursoraux = mainCompany() ->loadQuery ( query );
            if ( !cursoraux->eof() ) {
                debeinicial = BlFixed ( cursoraux->valor ( "tdebe" ) );
                haberinicial = BlFixed ( cursoraux->valor ( "thaber" ) );
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
                debe = BlFixed ( cursorapt->valor ( "debe" ) );
                haber = BlFixed ( cursorapt->valor ( "haber" ) );
                saldo = saldo + debe - haber;
                debefinal = debefinal + debe;
                haberfinal = haberfinal + haber;
                if ( mui_list->lineaat ( i ) )
                    mui_list->setDbValue ( "saldo", i++, saldo.toQString() );
                cursorapt->siguienteregistro();
            } // end while

            saldofinal = debefinal - haberfinal;
            totaldebe->setText ( debefinal.toQString() );
            totalhaber->setText ( haberfinal.toQString() );
            totalsaldo->setText ( saldofinal.toQString() );
        } // end if
        delete cursorapt;
        cursorapt = NULL;
        ajustes();
    } catch ( ... ) {
        mensajeInfo ( "Error en los calculos" );
        /// Liberamos memoria que pueda haber quedado reservada.
        if ( cursorapt ) delete cursorapt;
        if ( cursoraux ) delete cursoraux;
        return;
    } // end catch
    _depura ( "END extractoview1::presentar", 0 );
}


///
/**
\param codinicial
\param codfinal
\param fecha1
\param fecha2
**/
void extractoview1::inicializa1 ( QString codinicial, QString codfinal, QString fecha1, QString fecha2, int )
{
    _depura ( "extractoview1::inicializa1", 0 );
    m_codigoinicial->setText ( codinicial );
    m_codigofinal->setText ( codfinal );
    m_fechainicial1->setText ( normalizafecha ( fecha1 ).toString ( "dd/MM/yyyy" ) );
    m_fechafinal1->setText ( normalizafecha ( fecha2 ).toString ( "dd/MM/yyyy" ) );
    _depura ( "END extractoview1::inicializa1", 0 );
}


/// Realiza la casacion de los apuntes.
/**
**/
void extractoview1::on_mui_casacion_clicked()
{
    _depura ( "extractoview1::on_mui_casacion_clicked", 0 );
    try {
        QString query = "SELECT * FROM apunte WHERE punteo = FALSE AND haber <> 0 AND idcuenta = " + m_cursorcta->valor ( "idcuenta" ) + " ORDER BY fecha";
        BlDbRecordSet *curshaber = mainCompany() ->loadQuery ( query );
        BlProgressBar barra;
        barra.setRange ( 0, curshaber->numregistros() );
        barra.show();
        barra.setText ( _( "Cargando Extracto de Cuentas" ) );
        while ( !curshaber->eof() ) {
            query =  "SELECT * FROM apunte WHERE punteo = FALSE AND debe = " + curshaber->valor ( "haber" ) + " AND idcuenta = " + m_cursorcta->valor ( "idcuenta" ) + " ORDER BY fecha";
            BlDbRecordSet *cursdebe = mainCompany() ->loadQuery ( query.toAscii(), "cursdebe" );
            if ( !cursdebe->eof() ) {
                query = "UPDATE apunte set punteo = TRUE WHERE idapunte = " + curshaber->valor ( "idapunte" );
                mainCompany() ->begin();
                mainCompany() ->ejecuta ( query );
                query = "UPDATE apunte SET punteo = TRUE WHERE idapunte = " + cursdebe->valor ( "idapunte" );
                mainCompany() ->ejecuta ( query );
                mainCompany() ->commit();
            } // end if
            delete cursdebe;
            curshaber->siguienteregistro();
            barra.setValue ( barra.value() + 1 );
        } // end while
        delete curshaber;
        presentar();
    } catch ( ... ) {
        mensajeError ( "Se produjo un error en la casacion" );
    } // end try
    _depura ( "END extractoview1::on_mui_casacion_clicked", 0 );
}


/// Guarda el punteo en disco para poder recuperarlo despues
/**
**/
void extractoview1::on_mui_guardarpunteo_clicked()
{
    _depura ( "extractoview1::on_mui_guardarpunteo_clicked", 0 );
    QString fn = QFileDialog::getSaveFileName ( this,
                 _( "Guardar punteo" ),
                 confpr->valor ( CONF_DIR_USER ),
                 _( "Punteos (*.pto)" ) );

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
                fprintf ( mifile, "%s\n", cursp->valor ( "idapunte" ).toAscii().constData() );
                cursp->siguienteregistro();
            } // end while
            delete cursp;
            fclose ( mifile );
        } // end if
    } // end if
    _depura ( "END extractoview1::on_mui_guardarpunteo_clicked", 0 );
}


/// Esta funci&oacute;n borra todo el punteo de un extracto y lo pone a cero.
/** Esta funci&oacute;n se activa justo cuando se pulsa sobre el bot&oacute;n de
    resetear el punteo.
    Por supuesto cuando se pulsa dicho bot&oacute;n se borra el punteo. */
/**
**/
void extractoview1::on_mui_borrapunteo_clicked()
{
    _depura ( "extractoview1::on_mui_borrapunteo_clicked", 0 );
    try {
        int valor = QMessageBox::warning ( 0,
                                           _( "Borrar punteo" ),
                                           _( "Se dispone a borrar el punteo. Este cambio \
                                                es irrecuperable si no ha guardado su punteo. \
                                                Desea continuar?" ),
                                           QMessageBox::Yes, QMessageBox::No );
        if ( valor == QMessageBox::Yes ) {
            mainCompany() ->begin();
            mainCompany() ->ejecuta ( "UPDATE apunte SET punteo = FALSE WHERE idcuenta =" + m_cursorcta->valor ( "idcuenta" ) );
            mainCompany() ->commit();
            presentar();
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _( "Se ha producido un error" ) );
    } // end try
    _depura ( "END extractoview1::on_mui_borrapunteo_clicked", 0 );
}


/// Carga los punteos desde un fichero de texto que se solicita mediante un QFileDialog.
/** El fichero de punteos requiere que no se hayan modificado los identificadores de
    borrador.
    Para ello es preciso que no se hayan abierto y cerrado los asientos correspondientes
    ya que en dicho caso la carga del punteo no funciona correctamente. */
void extractoview1::on_mui_cargarpunteos_clicked()
{
    _depura ( "extractoview1::on_mui_cargarpunteos_clicked", 0 );
    try {
        QString fn = QFileDialog::getOpenFileName ( this,
                     _( "Cargar punteo" ),
                     confpr->valor ( CONF_DIR_USER ),
                     _( "Punteo (*.pto);;Todos los archivos (*)" ) );

        if ( !fn.isEmpty() ) {
            QFile file ( fn );
            if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
                return;
            } // end if
            QTextStream filestr ( &file );
            mainCompany()->begin();
            QString query = "UPDATE apunte SET punteo = FALSE";
            mainCompany() ->ejecuta ( query );
            QString a = filestr.readLine();
            while ( !a.isNull() ) {
                QString query;
                query = "UPDATE apunte SET punteo = TRUE WHERE idapunte = " + a;
                mainCompany() ->ejecuta ( query );
                a = filestr.readLine();
            } // end while
            mainCompany()->commit();
            file.close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( "Error en la carga del punteo" );
        mainCompany()->rollback();
    } // end try
    _depura ( "END extractoview1::on_mui_cargarpunteos_clicked", 0 );
}


///
/**
\param idcuenta
\return
**/
QString extractoview1::imprimeExtractoCuenta ( QString idcuenta )
{
    _depura ( "extractoview1::imprimeExtractoCuenta", 0, idcuenta );
    try {
        QString salida = "";
        BlFixed debeinicial ( "0" ), haberinicial ( "0" ), saldoinicial ( "0" );
        BlFixed debefinal ( "0" ), haberfinal ( "0" ), saldofinal ( "0" );
        QString finicial = m_fechainicial1->text();
        QString ffinal = m_fechafinal1->text();
        QString contra = mui_codigocontrapartida->text();

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
            _depura ( "END extractoview1::imprimeExtractoCuenta", 0, "No hay cuenta" );
            return "";
        } // end if

        /// Hacemos la consulta de los apuntes a listar en la base de datos.
        QString query = "";
        /// Al igual que en el caso anterior los centros de coste han cambiado y a&uacute;n
        /// no se pueden implementar.
        selectcanalview *scanal = mainCompany() ->getselcanales();
        SelectCCosteView *scoste = mainCompany() ->getselccostes();
        QString ccostes = scoste->cadcoste();
        if ( ccostes != "" ) {
            ccostes.sprintf ( " AND t5.idc_coste IN (%s) ", ccostes.toAscii().constData() );
        } // end if
        QString ccanales = scanal->cadcanal();
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

if (!mui_asAbiertos->isChecked() ) {
	query += " LEFT JOIN (SELECT idapunte AS id, idcuenta AS idcuentacontra, codigo AS codcontrapartida, descripcion AS desccontrapartida FROM contrapart LEFT JOIN cuenta ON contrapart.contra = cuenta.idcuenta) AS t7 ON t7.id = t1.idapunte ";
}

        query += " ORDER BY t1.fecha, ordenasiento, t1.orden";

        cursorapt = mainCompany() ->loadQuery ( query );
        if ( !cursorapt ) throw - 1;

        /// Cargamos los saldos iniciales.
        BlDbRecordSet *cursoraux;
        query = "SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM " +tabla+ " WHERE idcuenta =" + idcuenta + " AND fecha < '" + finicial + "'";
        cursoraux = mainCompany() ->loadQuery ( query );
        if ( !cursoraux ) {
            delete cursorapt;
            throw - 1;
        } // end if
        if ( !cursoraux->eof() ) {
            debeinicial = BlFixed ( cursoraux->valor ( "tdebe" ) );
            haberinicial = BlFixed ( cursoraux->valor ( "thaber" ) );
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
        salida += "<td> " + cursorcta->valor ( "codigo" ) + " -" +cursorcta->valor ( "descripcion" )+ " </td>";
        salida += "<td> Debe Inicial: " + debeinicial.toQString() + " </td>";
        salida += "<td> Haber Inicial: " + haberinicial.toQString() + " </td>";
        salida += "<td> Saldo Inicial: "+ saldoinicial.toQString() +"</td>";
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
if (!mui_asAbiertos->isChecked() ) {
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

            debefinal = debefinal + BlFixed ( cursorapt->valor ( "debe" ) );
            haberfinal = haberfinal + BlFixed ( cursorapt->valor ( "haber" ) );
	    saldofinal = debefinal - haberfinal;

		salida +=  "<tr>\n";
		salida +=  "<td>" + cursorapt->valor ( "fecha" ) + "</td>";
		salida +=  "<td>" + cursorapt->valor ( "ordenasiento" ) + "</td>";
		salida +=  "<td>" + cursorapt->valor ( "conceptocontable" ) + "-" + cursorapt->valor("descripcion") +"</td>";
		salida +=  "<td>" + cursorapt->valor ( "debe" ) + "</td>";
		salida +=  "<td>" + cursorapt->valor ( "haber" ) + "</td>";
		salida +=  "<td>" + saldofinal.toQString() + "</td>";
if (!mui_asAbiertos->isChecked() ) {
		salida +=  "<td>" + cursorapt->valor("codcontrapartida") + " - "+ cursorapt->valor("desccontrapartida") + "</td>";
} // end if
		salida +=  "</tr>\n";
//	    } // end if
//	    delete cur;
            cursorapt->siguienteregistro();
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
        _depura ( "END extractoview1::imprimeExtractoCuenta", 0 );
        return salida;
    } catch ( ... ) {
        mensajeError ( "Ocurrio un error inesperado" );
        return "";
    }
}



/// Slot que responde a la Impresion del extracto
/**
**/
void extractoview1::on_mui_imprimir_clicked()
{
    _depura ( "extractoview1::on_mui_imprimir_clicked", 0 );
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "extracto.rml";
    QString archivod = confpr->valor ( CONF_DIR_USER ) + "extracto.rml";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );
    /// Copiamos el logo
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii().constData() );
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";

    QString codinicial = m_codigoinicial->codigocuenta();
    QString codfinal = m_codigofinal->codigocuenta();

    QString query;


    /// Preparamos la barra de progreso
    BlProgressBar *barra = new BlProgressBar;
    barra->setValue ( 0 );
    barra->show();
    barra->setText ( _( "Generando Extracto " )  );


    /// Tabla temporal de contrapartidas.
    QString query1 = "CREATE TEMPORARY TABLE contrapart AS select idapunte, ccontrapartida(idapunte) AS contra FROM apunte";
    mainCompany()->ejecuta(query1);


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
    if ( !curcta ) return;
   	barra->setRange ( 0, curcta->numregistros() );
	int i = 0;
    while ( ! curcta->eof() ) {
        fitxersortidatxt += imprimeExtractoCuenta ( curcta->valor ( "idcuenta" ) );
        curcta->siguienteregistro();
		barra->setValue ( i++ );
    }// end while
    delete curcta;

    /// Tabla temporal de contrapartidas.
    query1 = "DROP TABLE contrapart";
    mainCompany()->ejecuta(query1);

	delete barra;

    buff.replace ( "[story]", fitxersortidatxt );
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    /// Crea el pdf y lo muestra.
    invocaPDF ( "extracto" );
    _depura ( "END extractoview1::on_mui_imprimir_clicked", 0 );
}
