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
#include "funcaux.h"
#include "configuracion.h"


///
/**
\param parent
**/
RutaComercialList::RutaComercialList ( QWidget *parent )
        : Listado ( NULL, parent )
{
    _depura ( "RutaComercialList::RutaComercialList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_idpresupuesto = "";
    _depura ( "END RutaComercialList::RutaComercialList", 0 );
}


///
/**
\param comp
\param parent
**/
RutaComercialList::RutaComercialList ( Company *comp, QWidget *parent )
        : Listado ( comp, parent )
{
    _depura ( "RutaComercialList::RutaComercialList", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    setSubForm ( mui_list );
    m_cliente->setEmpresaBase ( empresaBase() );
    mui_list->setEmpresaBase ( comp );
    presenta();
    m_idpresupuesto = "";
    hideBusqueda();
    empresaBase() ->meteWindow ( windowTitle(), this );
    _depura ( "END RutaComercialList::RutaComercialList", 0 );
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
void RutaComercialList::setEmpresaBase ( Company *comp )
{
    _depura ( "RutaComercialList::setEmpresaBase", 0 );
    PEmpresaBase::setEmpresaBase ( comp );
    m_cliente->setEmpresaBase ( comp );
    _depura ( "RutaComercialList::setEmpresaBase", 0 );
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
    if ( m_cliente->idcliente() != "" ) {
        filtro += " AND t1.idcliente = " + m_cliente->idcliente();
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
    m_cliente->setidcliente ( val );
    _depura ( "END RutaComercialList::setidcliente", 0 );
}




/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
///
/**
\param parent
**/
RutaComercialListSubForm::RutaComercialListSubForm ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "RutaComercialListSubForm::RutaComercialListSubForm", 0 );
    setDBTableName ( "rutacomercial" );
    setDBCampoId ( "idrutacomercial" );
    addSHeader ( "cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "CIF del cliente" ) );
    addSHeader ( "nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Nombre del cliente" ) );
    addSHeader ( "idzonacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, _( "ID zona comercial" ) );
    addSHeader ( "fecharutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Fecha ruta comercial" ) );
    addSHeader ( "fechaincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Fecha incidencia comercial" ) );
    addSHeader ( "nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Nombre del trabajador" ) );
    addSHeader ( "apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Apellidos del trabajador" ) );
    addSHeader ( "idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, _( "ID cliente" ) );
    addSHeader ( "idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, _( "ID trabajador" ) );
    addSHeader ( "idrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, _( "ID ruta comercial" ) );
    addSHeader ( "comentariosrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Comentarios ruta comercial" ) );
    addSHeader ( "idincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, _( "ID incidencia comercial" ) );
    addSHeader ( "comentincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Comentario de la incidencia comercial" ) );
    addSHeader ( "estadoincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Estado de la incidencia comercial" ) );
    addSHeader ( "idpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, _( "ID pedido cliente" ) );
    addSHeader ( "idcobro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, _( "ID cobro" ) );
    addSHeader ( "nomzonacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Nombre zona comercial" ) );
    addSHeader ( "refrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Referencia ruta" ) );
    addSHeader ( "horarutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Hora ruta" ) );
    addSHeader ( "refincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Referencia de la incidencia" ) );
    addSHeader ( "horaincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, _( "Hora de la incidencia" ) );
    setinsercion ( FALSE );
    _depura ( "END RutaComercialListSubForm::RutaComercialListSubForm", 0 );
};

