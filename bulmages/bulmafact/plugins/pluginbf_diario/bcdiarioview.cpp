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

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtGui/QColor>
#include <QtWidgets/QLayout>
#include <QtWidgets/QCheckBox>
#include <QtGui/QPixmap>
#include <QtCore/QTextStream>
#include <QtWidgets/QFileDialog>

#include "bfcompany.h"
#include "bcdiarioview.h"
#include "blfunctions.h"
#include "bcdiarioprintview.h"
#include "bcdiarioimprimir2txt.h"
#include "bcasientoview.h"
#include "blconfiguration.h"
#include "bldatesearch.h"


///
/**
\param emp
\param parent
**/
BcDiarioView::BcDiarioView ( BfCompany  *emp, QWidget *parent, int )
        : BfForm ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );

    setAttribute(Qt::WA_DeleteOnClose);
    setTitleName ( _ ( "Diario" ) );
    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDbTableName ( "apunte" );

    mui_list->setMainCompany ( emp );
    
    connect (mui_list, SIGNAL(openAsiento()), this, SLOT(openAsiento()) );
    
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
    insertWindow ( windowTitle(), this, false );
    /// Llamamos a los scripts
    blScript(this);

    
}


///
/**
**/
BcDiarioView::~BcDiarioView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


/**
**/
void BcDiarioView::on_mui_actualizar_released()
{
    BL_FUNC_DEBUG
    accept();
    
}


///
/**
\param checked
**/
void BcDiarioView::on_mui_configurar_toggled ( bool checked )
{
    BL_FUNC_DEBUG

    if ( checked ) {
        mui_list->showConfig();
	mui_list->showMenu();
	
    } else {
        mui_list->hideConfig();
    } // end if  
}


///
/**
\param finicial
\param ffinal
**/
void BcDiarioView::inicializa1 ( QString finicial, QString ffinal, int )
{
    BL_FUNC_DEBUG
    QString s1, s2, s3;
    QDate fecha1aux;
    int dia, mes, ano;

    s1 = finicial;
    s2 = s1.mid ( 0, 2 );
    dia = atoi ( s2.toLatin1() );
    s2 = s1.mid ( 3, 2 );
    mes = atoi ( s2.toLatin1() );
    s2 = s1.mid ( 6, 4 );
    ano = atoi ( s2.toLatin1() );
    fecha1aux.setDate ( ano, mes, dia );
    QString cadena2;
    cadena2.sprintf ( "%2.2d/%2.2d/%4.4d", fecha1aux.day(), fecha1aux.month(), fecha1aux.year() );
    mui_fechainicial->setText ( cadena2 );

    s1 = ffinal;
    s2 = s1.mid ( 0, 2 );
    dia = atoi ( s2.toLatin1() );
    s2 = s1.mid ( 3, 2 );
    mes = atoi ( s2.toLatin1() );
    s2 = s1.mid ( 6, 4 );
    ano = atoi ( s2.toLatin1() );
    fecha1aux.setDate ( ano, mes, dia );
    cadena2.sprintf ( "%2.2d/%2.2d/%4.4d", fecha1aux.day(), fecha1aux.month(), fecha1aux.year() );
    mui_fechafinal->setText ( cadena2 );
    
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de imprimir.
/** Muestra el formulario de impresi&oacute;n de diario y lo ejecuta \ref BcDiarioPrintView */
/**
**/
void BcDiarioView::boton_imprimir()
{
    BL_FUNC_DEBUG
    BcDiarioPrintView *print = new BcDiarioPrintView ( mainCompany(), 0 );
    print->exec();
    
}


/// Se ha pulsado sobre el bot&oacute;n guardar del formulario.
/**
**/
void BcDiarioView::botonGuardar()
{
    BL_FUNC_DEBUG
    QString fn = QFileDialog::getSaveFileName ( this,
                 _ ( "Guardar Libro Diario" ),
                 g_confpr->value( CONF_DIR_USER ),
                 _ ( "Diarios (*.txt)" ) );

    if ( !fn.isEmpty() ) {
        /// Si se ha proporcionado un nombre de archivo v&aacute;lido
        /// invocamos la clase diarioprint y hacemos que guarde el archivo.
        BcDiarioImprimir2TXT diariop ( mainCompany() );
        QString finicial = mui_fechainicial->text();
        QString ffinal = mui_fechafinal->text();
        diariop.setMainCompany ( mainCompany() );
        diariop.inicializa1 ( ( char * ) finicial.toLatin1().constData(), ( char * ) ffinal.toLatin1().constData() );
        diariop.inicializa2 ( ( char * ) fn.toLatin1().constData() );
        diariop.accept();
    } // end if
    
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
**/
void BcDiarioView::accept()
{
    BL_FUNC_DEBUG
    presentar();
}


/// Pinta el listado en el subformulario
/**
**/
void BcDiarioView::presentar()
{
    BL_FUNC_DEBUG
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
        query += " LEFT JOIN (SELECT ordenasiento, fecha, idasiento FROM asiento) AS t5 ON t5.idasiento = " + tabla + ".idasiento";
        query += " LEFT JOIN (SELECT idcanal, nombre as nombrecanal FROM canal) AS t2 ON t2.idcanal = " + tabla + ".idcanal";
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


        setDescripcion(mui_fechainicial->text() + " -- " + mui_fechafinal->text());
        insertWindow ( windowTitle(), this, true );
	
        QString ccostes = "";
	g_plugins->run("PgetSelCostes", &ccostes);

	
        QString ccanales = "";
	g_plugins->run("PgetSelCanales", &ccanales);
	
	bool sincanal = true;
	g_plugins->run("PgetSinCanal", &sincanal);
	
	
        if ( ccostes != "" ) {
            ccostes = " " + tabla + ".idc_coste IN (" + ccostes + ") ";
            cad += cadwhere + cadand + ccostes;
            cadwhere = "";
            cadand = " AND ";
        } // end if

        
        /// El calculo de los canales
        if (sincanal) {
	  if ( ccanales != "" ) {
	      ccanales = " ("+tabla+".idcanal ISNULL OR "+tabla+".idcanal IN (" + ccanales + ")) ";
	  } else {
	      ccanales = " "+tabla+".idcanal ISNULL ";	    
	  } // end if
	} else {
	  if ( ccanales != "" ) {
	      ccanales = " ("+tabla+".idcanal <> NULL OR "+tabla+".idcanal IN (" + ccanales + ")) ";
	  } else {
	      ccanales = " "+tabla+".idcanal <> NULL ";	    
	  } // end if
	} // end if
        cad += cadwhere + cadand + ccanales;
        cadwhere = "";
        cadand = " AND ";
     

        bool ok = false;
        mui_saldosup->text().toFloat ( &ok );
        if ( ok ) {
            cad += cadand + tabla + ".debe + " + tabla + ".haber >= " + mui_saldosup->text();
            cadand = " AND ";
        } // end if
        ok = false;
        mui_saldoinf->text().toFloat ( &ok );
        if ( ok ) {
            cad += cadand + tabla + ".debe + " + tabla + ".haber <= " + mui_saldoinf->text();
            cadand = " AND ";
        } // end if

        if ( mui_contrapartida->fieldValue("codigo") != "" ) {
            cad += cadand + tabla + ".contrapartida = " + mui_contrapartida->id();
        } // end if

        totalcadena = query + cad + " ORDER BY ordenasiento,"+tabla+".orden"+tabla;
        mui_list->load ( totalcadena );

        cur = mainCompany() ->loadQuery ( "SELECT sum(debe) as totaldebe, sum(haber) as totalhaber from " + tabla + cad );
        if ( !cur->eof() ) {
            totaldebe->setText ( cur->value( "totaldebe" ) );
            totalhaber->setText ( cur->value( "totalhaber" ) );
        } // end if
        delete cur;
        
    } catch ( ... ) {
        blMsgInfo ( "Error en los calculos" );
        /// Liberamos memoria que pueda haber quedado reservada.
        if ( cur ) delete cur;
    }// end try
}

/// Boton de impresion del diario.
/**
**/
void BcDiarioView::on_mui_imprimir_released()
{
    BL_FUNC_DEBUG
    mui_list->printPDF ( "diario" );
    
}


///
/**
\param columna
**/
void BcDiarioView::on_mui_list_cellDoubleClicked ( int, int columna )
{
    BL_FUNC_DEBUG
    openAsiento();
}


void BcDiarioView::openAsiento()
{
    BL_FUNC_DEBUG
    
    QString idasiento = mui_list->dbValue ( "idasiento" );

    int resur = g_plugins->run ( "SNewBcAsientoView", (BfCompany *) mainCompany() );
    
    if ( ! resur) {
        blMsgInfo("No se pudo crear instancia de asientos");
        return;
    } // end if
    
    BcAsientoView *asiento = (BcAsientoView *) g_plugParams;

    asiento ->muestraAsiento ( idasiento );
    asiento ->show();
    asiento ->setFocus();
}

void BcDiarioView::on_mui_filtrar_toggled(bool check) {
  BL_FUNC_DEBUG
  frame->setVisible(check);
}

