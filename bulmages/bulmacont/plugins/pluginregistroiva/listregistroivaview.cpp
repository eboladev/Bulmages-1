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

#include "listregistroivaview.h"
#include "regivaprintview.h"
#include "registroivaview.h"
#include "bccompany.h"
#include "asiento1view.h"

#include <QMenu>


///
/**
\param emp
\param parent
**/
ListRegistroIvaView::ListRegistroIvaView ( BcCompany * emp, QString, QWidget *parent )
        : QWidget ( parent )
{
    _depura ( "ListRegistroIvaView::ListRegistroIvaView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_companyact = emp;
    finicial->setText ( normalizafecha ( "01/01" ).toString ( "dd/MM/yyyy" ) );
    ffinal->setText ( normalizafecha ( "31/12" ).toString ( "dd/MM/yyyy" ) );
    emp->meteWindow ( windowTitle(), this );

    /// DEFINICIONES PARA LA TABLA DE IVA Soportado.
    mui_tablasoportado->setMainCompany ( emp );
    mui_tablasoportado->setDBTableName ( "registroiva" );
    mui_tablasoportado->setDBCampoId ( "idregistroiva" );
    mui_tablasoportado->addSHeader ( "idregistroiva", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite, _( "Id" ) );
    mui_tablasoportado->addSHeader ( "contrapartida", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "Concepto contable" ) );
    mui_tablasoportado->addSHeader ( "baseimp", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBNoView, _( "idcuenta" ) );
    mui_tablasoportado->addSHeader ( "iva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "IVA" ) );
    mui_tablasoportado->addSHeader ( "ffactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Fecha de factura" ) );
    mui_tablasoportado->addSHeader ( "femisionregistroiva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "descripcioncuenta" ) );
    mui_tablasoportado->addSHeader ( "factura", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "descripcion" ) );
    mui_tablasoportado->addSHeader ( "serieregistroiva", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "Debe" ) );
    mui_tablasoportado->addSHeader ( "idborrador", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "Haber" ) );
    mui_tablasoportado->addSHeader ( "incregistro", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Contrapartida" ) );
    mui_tablasoportado->addSHeader ( "regularizacion", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Comentario" ) );
    mui_tablasoportado->addSHeader ( "plan349", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "ID Canal" ) );
    mui_tablasoportado->addSHeader ( "numorden", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Conciliacion" ) );
    mui_tablasoportado->addSHeader ( "cif", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "idc_coste" ) );
    mui_tablasoportado->addSHeader ( "idfpago", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "idapunte" ) );
    mui_tablasoportado->addSHeader ( "factemitida", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "idtipoiva" ) );
    mui_tablasoportado->addSHeader ( "rectificaaregistroiva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "orden" ) );
    mui_tablasoportado->addSHeader ( "idasiento", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "idasiento" ) );
    mui_tablasoportado->setinsercion ( FALSE );

    /// DEFINICIONES PARA LA TABLA DE IVA Repercutido.
    mui_tablarepercutido->setMainCompany ( emp );
    mui_tablarepercutido->setDBTableName ( "registroiva" );
    mui_tablarepercutido->setDBCampoId ( "idregistroiva" );
    mui_tablarepercutido->addSHeader ( "idregistroiva", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite, _( "Id" ) );
    mui_tablarepercutido->addSHeader ( "contrapartida", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "Concepto Contable" ) );
    mui_tablarepercutido->addSHeader ( "baseimp", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBNoView, _( "idcuenta" ) );
    mui_tablarepercutido->addSHeader ( "iva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "codigo" ) );
    mui_tablarepercutido->addSHeader ( "ffactura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "tipocuenta" ) );
    mui_tablarepercutido->addSHeader ( "femisionregistroiva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "descripcioncuenta" ) );
    mui_tablarepercutido->addSHeader ( "factura", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "descripcion" ) );
    mui_tablarepercutido->addSHeader ( "serieregistroiva", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "Debe" ) );
    mui_tablarepercutido->addSHeader ( "idborrador", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "Haber" ) );
    mui_tablarepercutido->addSHeader ( "incregistro", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Contrapartida" ) );
    mui_tablarepercutido->addSHeader ( "regularizacion", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Comentario" ) );
    mui_tablarepercutido->addSHeader ( "plan349", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "ID Canal" ) );
    mui_tablarepercutido->addSHeader ( "numorden", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Conciliacion" ) );
    mui_tablarepercutido->addSHeader ( "cif", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "idc_coste" ) );
    mui_tablarepercutido->addSHeader ( "idfpago", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "idapunte" ) );
    mui_tablarepercutido->addSHeader ( "factemitida", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "idtipoiva" ) );
    mui_tablarepercutido->addSHeader ( "rectificaaregistroiva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "orden" ) );
    mui_tablarepercutido->addSHeader ( "idasiento", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "idasiento" ) );
    mui_tablarepercutido->setinsercion ( FALSE );

    /// Definiciones para la tabla de repercutido
    mui_totalRepercutido->setMainCompany ( emp );
    mui_totalRepercutido->setDBTableName ( "" );
    mui_totalRepercutido->setDBCampoId ( "" );
    mui_totalRepercutido->addSHeader ( "nombretipoiva", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite, _( "nombretipoiva" ) );
    mui_totalRepercutido->addSHeader ( "tivaiva", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "tivaiva" ) );
    mui_totalRepercutido->addSHeader ( "tbaseiva", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "tbaseiva" ) );
    mui_totalRepercutido->setinsercion ( FALSE );

    /// Definiciones para la tabla de soportado
    mui_totalSoportado->setMainCompany ( emp );
    mui_totalSoportado->setDBTableName ( "" );
    mui_totalSoportado->setDBCampoId ( "" );
    mui_totalSoportado->addSHeader ( "nombretipoiva", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite, _( "nombretipoiva" ) );
    mui_totalSoportado->addSHeader ( "tivaiva", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "tivaiva" ) );
    mui_totalSoportado->addSHeader ( "tbaseiva", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "tbaseiva" ) );
    mui_totalSoportado->setinsercion ( FALSE );

    /// Fin de las definiciones para la tabla de IVA.
    _depura ( "END ListRegistroIvaView::ListRegistroIvaView", 0 );
}

///
/**
**/
ListRegistroIvaView::~ListRegistroIvaView()
{
    _depura ( "ListRegistroIvaView::~ListRegistroIvaView", 0 );
    m_companyact->sacaWindow ( this );
    _depura ( "END ListRegistroIvaView::~ListRegistroIvaView", 0 );
}


/// Al hacer doble click sobre la tabla de ivas se accede al asiento
/// que tiene dicha entrada.
/**
**/
void ListRegistroIvaView::on_mui_tablasoportado_cellDoubleClicked ( int, int )
{
    _depura ( "ListRegistroIvaView::on_mui_tablasoportado_itemDoubleClicked", 0 );
    int idasiento;
    idasiento = mui_tablasoportado->DBvalue ( "idasiento" ).toInt();
    m_companyact->intapuntsempresa() ->muestraasiento ( idasiento );
    m_companyact->intapuntsempresa() ->show();
    m_companyact->intapuntsempresa() ->setFocus();
    _depura ( "END ListRegistroIvaView::on_mui_tablasoportado_itemDoubleClicked", 0 );
}


/// Al hacer doble click sobre la tabla de ivas se accede al asiento
/// que tiene dicha entrada.
/**
**/
void ListRegistroIvaView::on_mui_tablarepercutido_cellDoubleClicked ( int, int )
{
    _depura ( "ListRegistroIvaView::on_mui_tablarepercutido_itemDoubleClicked", 0 );
    int idasiento;
    idasiento = mui_tablarepercutido->DBvalue ( "idasiento" ).toInt();
    m_companyact->intapuntsempresa() ->muestraasiento ( idasiento );
    m_companyact->intapuntsempresa() ->show();
    m_companyact->intapuntsempresa() ->setFocus();
    _depura ( "END ListRegistroIvaView::on_mui_tablarepercutido_itemDoubleClicked", 0 );
}


///
/**
**/
void ListRegistroIvaView::boton_print()
{
    _depura ( "ListRegistroIvaView::boton_print", 0 );
    regivaprintview *print = new regivaprintview ( m_companyact, 0 );
    print->inicializa1 ( finicial->text(), ffinal->text() );
    print->exec();
    delete print;
    _depura ( "END ListRegistroIvaView::boton_print", 0 );
}


///
/**
**/
void ListRegistroIvaView::on_mui_actualizar_clicked()
{
    _depura ( "ListRegistroIvaView::on_mui_actualizar_clicked", 0 );
    inicializa();
    _depura ( "END ListRegistroIvaView::on_mui_actualizar_clicked", 0 );
}


///
/**
**/
void ListRegistroIvaView::inicializa()
{
    _depura ( "ListRegistroIvaView::inicializa", 0 );

    QString query;
    QString sbaseimp, siva;
    QString cbaseimp, civa, ctotal;
    BlDbRecordSet *cur;

    QString SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva, sum(ivaiva) AS tivaiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='" + finicial->text() + "' AND ffactura <='" + ffinal->text() + "' AND factemitida) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta";

    mui_totalRepercutido->cargar ( SQLQuery );


    SQLQuery = "SELECT * FROM cuenta, tipoiva  LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva, SUM(ivaiva) AS tivaiva FROM iva WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='" + finicial->text() + "' AND ffactura <='" + ffinal->text() + "' AND NOT factemitida) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta";
    mui_totalSoportado->cargar ( SQLQuery );

    SQLQuery = "SELECT SUM(baseimp) AS tbaseimp, sum(iva) AS tbaseiva FROM registroiva WHERE factemitida AND ffactura >='" + finicial->text() + "' AND ffactura <='" + ffinal->text() + "'";
    cur = m_companyact->cargacursor ( SQLQuery );
    m_baseimps->setText ( cur->valor ( "tbaseimp" ) );
    m_ivas->setText ( cur->valor ( "tbaseiva" ) );
    delete cur;

    SQLQuery = "SELECT SUM(baseimp) AS tbaseimp, sum(iva) AS tbaseiva FROM registroiva WHERE NOT factemitida AND ffactura >='" + finicial->text() + "' AND ffactura <='" + ffinal->text() + "'";
    cur = m_companyact->cargacursor ( SQLQuery );
    m_baseimpr->setText ( cur->valor ( "tbaseimp" ) );
    m_ivar->setText ( cur->valor ( "tbaseiva" ) );
    delete cur;

    query.sprintf ( "SELECT *, (registroiva.baseimp + registroiva.iva) AS totalfactura FROM registroiva LEFT JOIN (SELECT  * FROM cuenta, borrador, asiento  WHERE cuenta.idcuenta = borrador.idcuenta AND asiento.idasiento = borrador.idasiento ) AS t1 ON t1.idborrador = registroiva.idborrador WHERE factemitida AND ffactura >= '%s' AND ffactura <= '%s' ", finicial->text().toAscii().constData(), ffinal->text().toAscii().constData() );
    mui_tablasoportado->cargar ( query );

    /// Hacemos el c&aacute;culo de los que no pertenecen a IVA soportado porque
    /// as&iacute; entran todos.
    query.sprintf ( "SELECT *, (registroiva.baseimp + registroiva.iva) AS totalfactura FROM registroiva LEFT JOIN (SELECT * FROM cuenta, borrador, asiento  WHERE cuenta.idcuenta = borrador.idcuenta AND asiento.idasiento = borrador.idasiento) AS t1 ON t1.idborrador = registroiva.idborrador WHERE NOT factemitida AND ffactura >= '%s' AND ffactura <= '%s'", finicial->text().toAscii().constData(), ffinal->text().toAscii().constData() );
    mui_tablarepercutido->cargar ( query );
    _depura ( "END ListRegistroIvaView::inicializa", 0 );
}


///
/**
\param menu
**/
void ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu ( QMenu *menu )
{
    _depura ( "ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu", 0 );
    m_verreg = menu->addAction ( "Editar registro" );
    m_verasiento = menu->addAction ( "Asiento contable" );
    menu->addSeparator();
    _depura ( "END ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu", 0 );
}


///
/**
\param menu
**/
void ListRegistroIvaView::on_mui_tablasoportado_pintaMenu ( QMenu *menu )
{
    _depura ( "ListRegistroIvaView::on_mui_tablasoportado_pintaMenu", 0 );
    m_verreg = menu->addAction ( "Editar registro" );
    m_verasiento = menu->addAction ( "Asiento contable" );
    menu->addSeparator();
    _depura ( "END ListRegistroIvaView::on_mui_tablasoportado_pintaMenu", 0 );
}


///
/**
\param ac
**/
void ListRegistroIvaView::on_mui_tablasoportado_trataMenu ( QAction *ac )
{
    _depura ( "ListRegistroIvaView::on_mui_tablasoportado_trataMenu", 0 );
    if ( m_verreg == ac ) {
        QString idborrador = mui_tablasoportado->DBvalue ( "idborrador" );
        RegistroIvaView *nuevae = new RegistroIvaView ( m_companyact, 0 );
        nuevae->inicializa1 ( idborrador.toInt() );
        m_companyact->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    } // end if
    if ( m_verasiento == ac ) {
        int idasiento;
        idasiento = mui_tablasoportado->DBvalue ( "idasiento" ).toInt();
        m_companyact->intapuntsempresa() ->muestraasiento ( idasiento );
        m_companyact->intapuntsempresa() ->show();
        m_companyact->intapuntsempresa() ->setFocus();
    } // end if
    _depura ( "END ListRegistroIvaView::on_mui_tablasoportado_trataMenu", 0 );
}


///
/**
\param ac
**/
void ListRegistroIvaView::on_mui_tablarepercutido_trataMenu ( QAction *ac )
{
    _depura ( "ListRegistroIvaView::on_mui_tablarepercutido_trataMenu", 0 );
    if ( m_verreg == ac ) {
        QString idborrador = mui_tablarepercutido->DBvalue ( "idborrador" );
        RegistroIvaView *nuevae = new RegistroIvaView ( m_companyact, 0 );
        nuevae->inicializa1 ( idborrador.toInt() );
        m_companyact->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    } // end if
    if ( m_verasiento == ac ) {
        int idasiento;
        idasiento = mui_tablarepercutido->DBvalue ( "idasiento" ).toInt();
        m_companyact->intapuntsempresa() ->muestraasiento ( idasiento );
        m_companyact->intapuntsempresa() ->show();
        m_companyact->intapuntsempresa() ->setFocus();
    } // end if
    _depura ( "END ListRegistroIvaView::on_mui_tablarepercutido_trataMenu", 0 );
}

