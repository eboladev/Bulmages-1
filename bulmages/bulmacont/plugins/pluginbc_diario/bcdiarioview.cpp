/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
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

#include <QWidget>
#include <QLineEdit>
#include <QColor>
#include <QLayout>
#include <QCheckBox>
#include <QPixmap>
#include <QTextStream>
#include <QFileDialog>

#include "bccompany.h"
#include "bcdiarioview.h"
#include "blfunctions.h"
#include "bcdiarioprintview.h"
#include "bcdiarioimprimir2txt.h"
#include "blconfiguration.h"
#include "bccentrocosteseleccionarview.h"
#include "bccanalseleccionarview.h"
#include "bldatesearch.h"


///
/**
\param emp
\param parent
**/
BcDiarioView::BcDiarioView ( BcCompany  *emp, QWidget *parent, int )
        : BcForm ( emp, parent )
{
    _depura ( "BcDiarioView::BcDiarioView", 0 );
    setupUi ( this );

    setTitleName ( _ ( "Diario" ) );
    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDbTableName ( "apunte" );

    mui_list->setMainCompany ( emp );
    /// Arreglamos la cuenta
    mui_contrapartida->setMainCompany ( emp );
    mui_contrapartida->setLabel ( _ ( "Contrapartida:" ) );
    mui_contrapartida->setTableName ( "cuenta" );
    mui_contrapartida->setFieldId("idcuenta");
    mui_contrapartida->m_valores["descripcion"] = "";
    mui_contrapartida->m_valores["codigo"] = "";
    mui_contrapartida->hideLabel();

    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    mui_fechainicial->setText ( "01/01/" + QString::number(QDate::currentDate().year()) );
    mui_fechafinal->setText ( "31/12/" + QString::number(QDate::currentDate().year()) );
    meteWindow ( windowTitle(), this );
    _depura ( "END BcDiarioView::BcDiarioView", 0 );
}


///
/**
**/
BcDiarioView::~BcDiarioView()
{
    _depura ( "BcDiarioView::~BcDiarioView", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "END BcDiarioView::~BcDiarioView", 0 );
}


/**
**/
void BcDiarioView::on_mui_actualizar_clicked()
{
    _depura ( "BcDiarioView::on_mui_actualizar_clicked", 0 );
    accept();
    _depura ( "END BcDiarioView::on_mui_actualizar_clicked", 0 );
}


///
/**
**/
void BcDiarioView::on_mui_configurar_clicked()
{
    _depura ( "BcDiarioView::on_mui_configurar_clicked", 0 );
    mui_list->showConfig();
    _depura ( "END BcDiarioView::on_mui_configurar_clicked", 0 );
}


///
/**
\param finicial
\param ffinal
**/
void BcDiarioView::inicializa1 ( QString finicial, QString ffinal, int )
{
    _depura ( "BcDiarioView::inicializa1", 0 );
    QString s1, s2, s3;
    QDate fecha1aux;
    int dia, mes, ano;

    s1 = finicial;
    s2 = s1.mid ( 0, 2 );
    dia = atoi ( s2.toAscii() );
    s2 = s1.mid ( 3, 2 );
    mes = atoi ( s2.toAscii() );
    s2 = s1.mid ( 6, 4 );
    ano = atoi ( s2.toAscii() );
    fecha1aux.setYMD ( ano, mes, dia );
    QString cadena2;
    cadena2.sprintf ( "%2.2d/%2.2d/%4.4d", fecha1aux.day(), fecha1aux.month(), fecha1aux.year() );
    mui_fechainicial->setText ( cadena2 );

    s1 = ffinal;
    s2 = s1.mid ( 0, 2 );
    dia = atoi ( s2.toAscii() );
    s2 = s1.mid ( 3, 2 );
    mes = atoi ( s2.toAscii() );
    s2 = s1.mid ( 6, 4 );
    ano = atoi ( s2.toAscii() );
    fecha1aux.setYMD ( ano, mes, dia );
    cadena2.sprintf ( "%2.2d/%2.2d/%4.4d", fecha1aux.day(), fecha1aux.month(), fecha1aux.year() );
    mui_fechafinal->setText ( cadena2 );
    _depura ( "END BcDiarioView::inicializa1", 0 );
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de imprimir.
/** Muestra el formulario de impresi&oacute;n de diario y lo ejecuta \ref BcDiarioPrintView */
/**
**/
void BcDiarioView::boton_imprimir()
{
    _depura ( "BcDiarioView::boton_imprimir", 0 );
    BcDiarioPrintView *print = new BcDiarioPrintView ( mainCompany(), 0 );
    print->exec();
    _depura ( "END BcDiarioView::boton_imprimir", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n guardar del formulario.
/**
**/
void BcDiarioView::boton_guardar()
{
    _depura ( "BcDiarioView::boton_guardar", 0 );
    QString fn = QFileDialog::getSaveFileName ( this,
                 _ ( "Guardar Libro Diario" ),
                 g_confpr->valor ( CONF_DIR_USER ),
                 _ ( "Diarios (*.txt)" ) );

    if ( !fn.isEmpty() ) {
        /// Si se ha proporcionado un nombre de archivo v&aacute;lido
        /// invocamos la clase diarioprint y hacemos que guarde el archivo.
        BcDiarioImprimir2TXT diariop ( mainCompany() );
        QString finicial = mui_fechainicial->text();
        QString ffinal = mui_fechafinal->text();
        diariop.setMainCompany ( mainCompany() );
        diariop.inicializa1 ( ( char * ) finicial.toAscii().constData(), ( char * ) ffinal.toAscii().constData() );
        diariop.inicializa2 ( ( char * ) fn.toAscii().constData() );
        diariop.accept();
    } // end if
    _depura ( "END BcDiarioView::boton_guardar", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
**/
void BcDiarioView::accept()
{
    _depura ( "BcDiarioView::accept", 0 );
    presentar();
    _depura ( "END BcDiarioView::accept", 0 );
}


/// Pinta el listado en el subformulario
/**
**/
void BcDiarioView::presentar()
{
    _depura ( "BcDiarioView::presentar", 0 );
    BlDbRecordSet *cur = NULL;
    try {
        QString tabla = "apunte";
        QString tabla2 = "";
        mui_list->setDbTableName ( "apunte" );
        mui_list->setDbFieldId ( "idapunte" );
        if ( mui_asAbiertos->isChecked() ) {
            tabla = "borrador";
            tabla2 = "apunte,";
            mui_list->setDbTableName ( "borrador" );
            mui_list->setDbFieldId ( "idborrador" );
        } // end if

        QString query = "SELECT *, cuenta.descripcion AS descripcioncuenta FROM " + tabla2 + tabla + " LEFT JOIN cuenta ON cuenta.idcuenta = " + tabla + ".idcuenta ";
        query += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste FROM c_coste) AS t1 ON t1.idc_coste = " + tabla + ".idc_coste ";
//         query += " LEFT JOIN (SELECT (ordenasiento || ' - ' || fecha) AS ordenasientoconfecha, ordenasiento, idasiento, fecha FROM asiento) AS t5 ON t5.idasiento = " + tabla + ".idasiento";
        query += " LEFT JOIN (SELECT ordenasiento, fecha, idasiento FROM asiento) AS t5 ON t5.idasiento = " + tabla + ".idasiento";
        query += " LEFT JOIN (SELECT idcanal, nombre as nombrecanal FROM canal) AS t2 ON t2.idcanal = " + tabla + ".idcanal";
        if ( mui_asAbiertos->isChecked() ) {
            query += " LEFT JOIN (SELECT idregistroiva, factura, idborrador FROM registroiva) AS t3 ON t3.idborrador = " + tabla + ".idborrador ";
        } else {
            query += " LEFT JOIN (SELECT idborrador AS idborr, idapunte AS idap FROM borrador) AS t9 ON apunte.idapunte = t9.idap ";
            query += " LEFT JOIN (SELECT idregistroiva, factura, idborrador FROM registroiva) AS t3 ON t3.idborrador = t9.idborr";
        } // end if
        query += " LEFT JOIN (SELECT idcuenta AS idcontrapartida, codigo AS codcontrapartida FROM cuenta) as t8 ON t8.idcontrapartida = " + tabla + ".contrapartida";

        QString cad = "";
        QString cadwhere = " WHERE ";
        QString cadand = "";
        QString totalcadena = "";

        if ( mui_fechainicial->text() != "" ) {
            cad += cadwhere + cadand + tabla + ".fecha >= '" + mui_fechainicial->text() + "'";
            cadwhere = "";
            cadand = " AND ";
        } // end if

        if ( mui_fechafinal->text() != "" ) {
            cad += cadwhere + cadand + tabla + ".fecha <= '" + mui_fechafinal->text() + "'";
            cadwhere = "";
            cadand = " AND ";
        } // end if

        // Consideraciones para centros de coste y canales
        BcCanalSeleccionarView *scanal = mainCompany() ->getselcanales();
        BcCentroCosteSeleccionarView *scoste = mainCompany() ->getselccostes();
        QString ccostes = scoste->cadcoste();
        if ( ccostes != "" ) {
            ccostes = " " + tabla + ".idc_coste IN (" + ccostes + ") ";
            cad += cadwhere + cadand + ccostes;
            cadwhere = "";
            cadand = " AND ";
        } // end if

        QString ccanales = scanal->cadcanal();
        if ( ccanales != "" ) {
            ccanales = " " + tabla + ".idcanal IN (" + ccanales + ") ";
            cad += cadwhere + cadand + ccanales;
            cadwhere = "";
            cadand = " AND ";
        } // end if

        bool ok = FALSE;
        mui_saldosup->text().toFloat ( &ok );
        if ( ok ) {
            cad += cadand + tabla + ".debe + " + tabla + ".haber >= " + mui_saldosup->text();
            cadand = " AND ";
        } // end if
        ok = FALSE;
        mui_saldoinf->text().toFloat ( &ok );
        if ( ok ) {
            cad += cadand + tabla + ".debe + " + tabla + ".haber <= " + mui_saldoinf->text();
            cadand = " AND ";
        } // end if

        if ( mui_contrapartida->fieldValue("codigo") != "" ) {
            cad += cadand + tabla + ".contrapartida = " + mui_contrapartida->id();
        } // end if

        totalcadena = query + cad + " ORDER BY ordenasiento,apunte.haber,codigo ";
        mui_list->cargar ( totalcadena );

        cur = mainCompany() ->loadQuery ( "SELECT sum(debe) as totaldebe, sum(haber) as totalhaber from " + tabla + cad );
        if ( !cur->eof() ) {
            totaldebe->setText ( cur->valor ( "totaldebe" ) );
            totalhaber->setText ( cur->valor ( "totalhaber" ) );
        } // end if
        delete cur;
        _depura ( "END BcDiarioView::presentar", 0 );
    } catch ( ... ) {
        mensajeInfo ( "Error en los calculos" );
        /// Liberamos memoria que pueda haber quedado reservada.
        if ( cur ) delete cur;
    }// end try
}

/// Boton de impresion del diario.
/**
**/
void BcDiarioView::on_mui_imprimir_clicked()
{
    _depura ( "BcDiarioView::on_mui_imprimir_clicked", 0 );
    mui_list->imprimirPDF ( "diario" );
    _depura ( "END BcDiarioView::on_mui_imprimir_clicked", 0 );
}

