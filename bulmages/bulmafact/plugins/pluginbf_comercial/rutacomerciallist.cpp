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
#include "company.h"
#include "blfunctions.h"
#include "blconfiguration.h"


///
/**
\param parent
**/
RutaComercialList::RutaComercialList ( QWidget *parent )
        : BlFormList ( NULL, parent )
{
    _depura ( "RutaComercialList::RutaComercialList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_idpresupuesto = "";

		/// Establecemos los parametros de busqueda del Cliente
    m_idcliente->setLabel ( _( "Cliente:" ) );
	m_idcliente->setTableName( "cliente" );
	m_idcliente->m_valores["cifcliente"] = "";
	m_idcliente->m_valores["nomcliente"] = "";

    _depura ( "END RutaComercialList::RutaComercialList", 0 );
}


///
/**
\param comp
\param parent
**/
RutaComercialList::RutaComercialList ( Company *comp, QWidget *parent )
        : BlFormList ( comp, parent )
{
    _depura ( "RutaComercialList::RutaComercialList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    setSubForm ( mui_list );
    m_idcliente->setMainCompany ( empresaBase() );
    mui_list->setMainCompany ( comp );
    presenta();
    m_idpresupuesto = "";
    hideBusqueda();
    empresaBase() ->meteWindow ( windowTitle(), this );
    _depura ( "END RutaComercialList::RutaComercialList", 0 );

		/// Establecemos los parametros de busqueda del Cliente
	m_idcliente->setMainCompany(comp);
    m_idcliente->setLabel ( _( "Cliente:" ) );
	m_idcliente->setTableName( "cliente" );
	m_idcliente->m_valores["cifcliente"] = "";
	m_idcliente->m_valores["nomcliente"] = "";
}


///
/**
**/
RutaComercialList::~RutaComercialList()
{
    _depura ( "RutaComercialList::~RutaComercialList", 0 );
    _depura ( "END RutaComercialList::~RutaComercialList", 0 );
}


///
/**
\param comp
**/
void RutaComercialList::setMainCompany ( Company *comp )
{
    _depura ( "RutaComercialList::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    m_idcliente->setMainCompany ( comp );
    _depura ( "RutaComercialList::setMainCompany", 0 );
}


///
/**
**/
void RutaComercialList::presenta()
{
    _depura ( "RutaComercialList::presenta()\n", 0 );
    QString SQLQuery = "SELECT * FROM (SELECT * FROM rutacomercial NATURAL LEFT JOIN incidenciacomercial UNION SELECT * FROM rutacomercial NATURAL RIGHT JOIN incidenciacomercial WHERE incidenciacomercial.idrutacomercial IS NULL) AS t1 NATURAL LEFT JOIN trabajador LEFT JOIN (SELECT * FROM cliente NATURAL LEFT JOIN zonacomercial) AS t2 ON t1.idcliente = t2.idcliente WHERE 1 = 1 " + generaFiltro();
    mui_list->cargar ( SQLQuery );
    _depura ( "end RutaComercialList::presenta()\n", 0 );
}


///
/**
\return
**/
QString RutaComercialList::generaFiltro()
{
    _depura ( "RutaComercialList::generaFiltro", 0 );
    /// Tratamiento de los filtros.
    QString filtro = "";
    if ( m_idcliente->id() != "" ) {
        filtro += " AND t1.idcliente = " + m_idcliente->id();
    } // end if
    if ( m_fechain->text() != "" )
        filtro += " AND t1.fechaincidenciacomercial >= '" + m_fechain->text() + "' ";
    if ( m_fechafin->text() != "" )
        filtro += " AND t1.fechaincidenciacomercial <= '" + m_fechafin->text() + "' ";
    _depura ( "END RutaComercialList::generaFiltro", 0 );
    return ( filtro );
}


///
/**
\param row
\return
**/
void RutaComercialList::editar ( int row )
{
    _depura ( "RutaComercialList::editar", 0 );
    QString idrutacomercial = mui_list->DBvalue ( "idrutacomercial", row );
    QString idincidenciacomercial = mui_list->DBvalue ( "idincidenciacomercial", row );
    RutaComercialIncView *rut = new RutaComercialIncView ( ( Company * ) empresaBase(), NULL );
    if ( rut->cargar ( idrutacomercial, idincidenciacomercial ) )
        return;
    empresaBase() ->m_pWorkspace->addWindow ( rut );
    empresaBase() ->meteWindow ( _("Nueva incidencia comercial"), rut );
    rut->show();
    _depura ( "END RutaComercialList::editar", 0 );
}


///
/**
**/
void RutaComercialList::crear()
{
    _depura ( "RutaComercialList::crear", 0 );
    RutaComercialIncView *rut = new RutaComercialIncView ( ( Company * ) empresaBase(), NULL );
    empresaBase() ->m_pWorkspace->addWindow ( rut );
    empresaBase() ->meteWindow ( _("Nueva incidencia comercial"), rut );
    rut->show();
    _depura ( "END RutaComercialList::crear", 0 );
}


///
/**
**/
void RutaComercialList::imprimir()
{
    _depura ( "RutaComercialList::imprimir", 0 );
    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "rutascomerciales.rml";
    QString archivod = confpr->valor ( CONF_DIR_USER ) + "rutascomerciales.rml";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii() );
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
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
    invocaPDF ( "rutascomerciales" );
    _depura ( "END RutaComercialList::imprimir", 0 );
}


///
/**
\return
**/
void RutaComercialList::borrar()
{
    _depura ( "RutaComercialList::borrar", 0 );
    QString idrutacomercial = mui_list->DBvalue ( "idrutacomercial" );
    QString idincidenciacomercial = mui_list->DBvalue ( "idincidenciacomercial" );
    RutaComercialIncView *rut = new RutaComercialIncView ( ( Company * ) empresaBase(), NULL );
    if ( rut->cargar ( idrutacomercial, idincidenciacomercial ) )
        return;
    rut->on_mui_borrar_clicked();
    _depura ( "END RutaComercialList::borrar", 0 );
}


///
/**
\return
**/
QString RutaComercialList::idpresupuesto()
{
    _depura ( "RutaComercialList::idpresupuesto", 0 );
    _depura ( "END RutaComercialList::idpresupuesto", 0 );
    return m_idpresupuesto;
}


///
/**
\param val
**/
void RutaComercialList::setidcliente ( QString val )
{
    _depura ( "RutaComercialList::setidcliente", 0 );
    m_idcliente->setId ( val );
    _depura ( "END RutaComercialList::setidcliente", 0 );
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
    _depura ( "RutaComercialListSubForm::RutaComercialListSubForm", 0 );
    setDBTableName ( "rutacomercial" );
    setDBCampoId ( "idrutacomercial" );
    addSHeader ( "cifcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "CIF del cliente" ) );
    addSHeader ( "nomcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre del cliente" ) );
    addSHeader ( "idzonacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID zona comercial" ) );
    addSHeader ( "fecharutacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha ruta comercial" ) );
    addSHeader ( "fechaincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha incidencia comercial" ) );
    addSHeader ( "nomtrabajador", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre del trabajador" ) );
    addSHeader ( "apellidostrabajador", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Apellidos del trabajador" ) );
    addSHeader ( "idcliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID cliente" ) );
    addSHeader ( "idtrabajador", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID trabajador" ) );
    addSHeader ( "idrutacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID ruta comercial" ) );
    addSHeader ( "comentariosrutacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Comentarios ruta comercial" ) );
    addSHeader ( "idincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID incidencia comercial" ) );
    addSHeader ( "comentincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Comentario de la incidencia comercial" ) );
    addSHeader ( "estadoincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Estado de la incidencia comercial" ) );
    addSHeader ( "idpedidocliente", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID pedido cliente" ) );
    addSHeader ( "idcobro", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID cobro" ) );
    addSHeader ( "nomzonacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre zona comercial" ) );
    addSHeader ( "refrutacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Referencia ruta" ) );
    addSHeader ( "horarutacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Hora ruta" ) );
    addSHeader ( "refincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Referencia de la incidencia" ) );
    addSHeader ( "horaincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Hora de la incidencia" ) );
    setinsercion ( FALSE );
    _depura ( "END RutaComercialListSubForm::RutaComercialListSubForm", 0 );
};

