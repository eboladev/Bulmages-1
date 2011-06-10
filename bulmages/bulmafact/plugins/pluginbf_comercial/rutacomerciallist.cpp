/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>

#include "rutacomerciallist.h"
#include "rutacomercialincview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blconfiguration.h"

#include "bfbulmafact.h"
#include "blmainwindow.h"

///
/**
\param parent
**/
RutaComercialList::RutaComercialList ( QWidget *parent )
        : BlFormList ( NULL, parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_idpresupuesto = "";

    /// Establecemos los parametros de busqueda del Cliente
    m_idcliente->setLabel ( _ ( "Cliente:" ) );
    m_idcliente->setTableName ( "cliente" );
    m_idcliente->m_valores["cifcliente"] = "";
    m_idcliente->m_valores["nomcliente"] = "";
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param comp
\param parent
**/
RutaComercialList::RutaComercialList ( BfCompany *comp, QWidget *parent )
        : BlFormList ( comp, parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    setSubForm ( mui_list );
    m_idcliente->setMainCompany ( mainCompany() );
    mui_list->setMainCompany ( comp );
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presenta();
    m_idpresupuesto = "";
    hideBusqueda();
    mainCompany() ->insertWindow ( windowTitle(), this );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );

    /// Establecemos los parametros de busqueda del Cliente
    m_idcliente->setMainCompany ( comp );
    m_idcliente->setLabel ( _ ( "Cliente:" ) );
    m_idcliente->setTableName ( "cliente" );
    m_idcliente->m_valores["cifcliente"] = "";
    m_idcliente->m_valores["nomcliente"] = "";
    /// Llamamos a los scripts
    blScript(this);
}


///
/**
**/
RutaComercialList::~RutaComercialList()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param comp
**/
void RutaComercialList::setMainCompany ( BfCompany *comp )
{
    blDebug ( Q_FUNC_INFO, 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
//    setMainCompany(comp);
    m_idcliente->setMainCompany ( comp );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void RutaComercialList::presenta()
{
    blDebug ( Q_FUNC_INFO, 0 );
    QString SQLQuery = "SELECT * FROM (SELECT * FROM rutacomercial NATURAL LEFT JOIN incidenciacomercial UNION SELECT * FROM rutacomercial NATURAL RIGHT JOIN incidenciacomercial WHERE incidenciacomercial.idrutacomercial IS NULL) AS t1 NATURAL LEFT JOIN trabajador LEFT JOIN (SELECT * FROM cliente NATURAL LEFT JOIN zonacomercial) AS t2 ON t1.idcliente = t2.idcliente WHERE 1 = 1 " + generaFiltro();
    mui_list->load ( SQLQuery );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
QString RutaComercialList::generaFiltro()
{
    blDebug ( Q_FUNC_INFO, 0 );
    /// Tratamiento de los filtros.
    QString filtro = "";
    if ( m_idcliente->id() != "" ) {
        filtro += " AND t1.idcliente = " + m_idcliente->id();
    } // end if
    if ( m_fechain->text() != "" )
        filtro += " AND t1.fechaincidenciacomercial >= '" + m_fechain->text() + "' ";
    if ( m_fechafin->text() != "" )
        filtro += " AND t1.fechaincidenciacomercial <= '" + m_fechafin->text() + "' ";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return ( filtro );
}


///
/**
\param row
\return
**/
void RutaComercialList::editar ( int row )
{
    blDebug ( Q_FUNC_INFO, 0 );
    QString idrutacomercial = mui_list->dbValue ( "idrutacomercial", row );
    QString idincidenciacomercial = mui_list->dbValue ( "idincidenciacomercial", row );

    /**
    BfCompany *comp = ((BfBulmaFact *) g_main)->company();
    comp->loadQuery("SELECT 1 + 2");
    
    RutaComercialView *rut = new RutaComercialView (  this);
    rut->setMainCompany((BfCompany *) mainCompany() );
    rut->load(idrutacomercial);
    rut->show();
    **/

    RutaComercialIncView *rut = new RutaComercialIncView ( ( BfCompany * ) mainCompany(), NULL );
    if ( rut->load ( idrutacomercial, idincidenciacomercial ) )
        return;
    mainCompany() ->m_pWorkspace->addSubWindow ( rut );
    mainCompany() ->insertWindow ( _ ( "Nueva incidencia comercial" ), rut );
    rut->show();

    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void RutaComercialList::crear()
{
    blDebug ( Q_FUNC_INFO, 0 );
    RutaComercialIncView *rut = new RutaComercialIncView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( rut );
    mainCompany() ->insertWindow ( _ ( "Nueva incidencia comercial" ), rut );
    rut->show();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void RutaComercialList::imprimir()
{
    blDebug ( Q_FUNC_INFO, 0 );
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "rutascomerciales.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "rutascomerciales.rml";
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef Q_OS_WIN32

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii() );
    /// Copiamos el logo.
#ifdef Q_OS_WIN32

    archivologo = "copy " + archivologo + " " + g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii() );

    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    /// Crea el pdf y lo muestra.
    blCreateAndLoadPDF ( "rutascomerciales" );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
void RutaComercialList::remove()
{
    blDebug ( Q_FUNC_INFO, 0 );
    QString idrutacomercial = mui_list->dbValue ( "idrutacomercial" );
    QString idincidenciacomercial = mui_list->dbValue ( "idincidenciacomercial" );
    RutaComercialIncView *rut = new RutaComercialIncView ( ( BfCompany * ) mainCompany(), NULL );
    if ( rut->load ( idrutacomercial, idincidenciacomercial ) )
        return;
    rut->on_mui_borrar_clicked();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
QString RutaComercialList::idpresupuesto()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return m_idpresupuesto;
}


///
/**
\param val
**/
void RutaComercialList::setidcliente ( QString val )
{
    blDebug ( Q_FUNC_INFO, 0 );
    m_idcliente->setId ( val );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}




/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
///
/**
\param parent
**/
RutaComercialListSubForm::RutaComercialListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setDbTableName ( "rutacomercial" );
    setDbFieldId ( "idrutacomercial" );
    addSubFormHeader ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "CIF del cliente" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre del cliente" ) );
    addSubFormHeader ( "idzonacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID zona comercial" ) );
    addSubFormHeader ( "fecharutacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha ruta comercial" ) );
    addSubFormHeader ( "fechaincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha incidencia comercial" ) );
    addSubFormHeader ( "nomtrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre del trabajador" ) );
    addSubFormHeader ( "apellidostrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Apellidos del trabajador" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID cliente" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID trabajador" ) );
    addSubFormHeader ( "idrutacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID ruta comercial" ) );
    addSubFormHeader ( "comentariosrutacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentarios ruta comercial" ) );
    addSubFormHeader ( "idincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID incidencia comercial" ) );
    addSubFormHeader ( "comentincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentario de la incidencia comercial" ) );
    addSubFormHeader ( "estadoincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Estado de la incidencia comercial" ) );
    addSubFormHeader ( "idpedidocliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID pedido cliente" ) );
    addSubFormHeader ( "idcobro", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID cobro" ) );
    addSubFormHeader ( "nomzonacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre zona comercial" ) );
    addSubFormHeader ( "refrutacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia ruta" ) );
    addSubFormHeader ( "horarutacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Hora ruta" ) );
    addSubFormHeader ( "refincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia de la incidencia" ) );
    addSubFormHeader ( "horaincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Hora de la incidencia" ) );
    setInsert ( FALSE );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
};

