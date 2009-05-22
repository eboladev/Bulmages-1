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

#include "bcmasapatrimoniallistview.h"
#include "bcmasapatrimonialview.h"
#include "bccompany.h"


///
/**
\param emp
\param parent
**/
BcMasaPatrimonialListView::BcMasaPatrimonialListView ( BcCompany *emp, QWidget *parent )
        : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    _depura ( "BcMasaPatrimonialListView::BcMasaPatrimonialListView", 0 );
    setupUi ( this );
    modo = 0;
    _depura ( "END BcMasaPatrimonialListView::BcMasaPatrimonialListView", 0 );
}


///
/**
**/
BcMasaPatrimonialListView::~BcMasaPatrimonialListView()
{
    _depura ( "BcMasaPatrimonialListView::~BcMasaPatrimonialListView", 0 );
    _depura ( "END BcMasaPatrimonialListView::~BcMasaPatrimonialListView", 0 );
}


///
/**
**/
void BcMasaPatrimonialListView::setmodoselector()
{
    _depura ( "BcMasaPatrimonialListView::setmodoselector", 0 );
    modo = 1;
    _depura ( "END BcMasaPatrimonialListView::setmodoselector", 0 );
}


///
/**
**/
void BcMasaPatrimonialListView::setmodoeditor()
{
    _depura ( "BcMasaPatrimonialListView::setmodoeditor", 0 );
    modo = 0;
    _depura ( "END BcMasaPatrimonialListView::setmodoeditor", 0 );
}


///
/**
\return
**/
QString BcMasaPatrimonialListView::getnommasa()
{
    _depura ( "BcMasaPatrimonialListView::getnommasa", 0 );
    _depura ( "END BcMasaPatrimonialListView::getnommasa", 0 );
    return nommasa;
}


///
/**
\return
**/
QString BcMasaPatrimonialListView::getidmasa()
{
    _depura ( "BcMasaPatrimonialListView::getidmasa", 0 );
    _depura ( "END BcMasaPatrimonialListView::getidmasa", 0 );
    return idmasa;
}


/// Esta funci&oacute;n se encarga de hacer las inicializaciones de todo el formulario.
/** Se llama as&iacute; y no desde el constructor porque as&iacute; la podemos llamar
    desde dentro de la misma clase, etc, etc, etc. */
/**
\return
**/
int BcMasaPatrimonialListView::inicializa()
{
    _depura ( "BcMasaPatrimonialListView::inicializa", 0 );
    BlDbRecordSet *cursoraux1;

    /// Vamos a cargar el n&uacute;mero de d&iacute;gitos de cuenta para poder hacer
    /// una introducci&oacute;n de numeros de cuenta m&aacute;s pr&aacute;ctica.
    mainCompany() ->begin();
    QString query = "SELECT valor FROM configuracion WHERE nombre = 'CodCuenta'";
    cursoraux1 = mainCompany() ->loadQuery ( query, "codcuenta" );
    mainCompany() ->commit();
    numdigitos = cursoraux1->valor ( "valor" ).length();
    delete cursoraux1;
    fprintf ( stderr, "las cuentas tienen %d digitos\n", numdigitos );
    inicializatabla();
    _depura ( "END BcMasaPatrimonialListView::inicializa", 0 );
    return ( 0 );
}


///
/**
**/
void BcMasaPatrimonialListView::inicializatabla()
{
    _depura ( "BcMasaPatrimonialListView::inicializatabla", 0 );
    QTableWidgetItem *item0, *item1;
    mui_tabla->setRowCount ( 0 );
    mui_tabla->setColumnCount ( 2 );
    QStringList etiquetas;
    etiquetas << _ ( "CODIGO" ) << _ ( "Masa patrimonial" );
    mui_tabla->setHorizontalHeaderLabels ( etiquetas );
    mui_tabla->hideColumn ( 2 );
    mui_tabla->hideColumn ( 0 );
    mui_tabla->setColumnWidth ( 1, 400 );

    QString query = "SELECT * FROM mpatrimonial WHERE idbalance ISNULL";
    mainCompany() ->begin();
    BlDbRecordSet *cursoraux1 = mainCompany() ->loadQuery ( query, "elquery" );
    mainCompany() ->commit();

    mui_tabla->setRowCount ( cursoraux1->numregistros() );
    int i = 0;
    while ( !cursoraux1->eof() ) {
        item0 = new QTableWidgetItem ( cursoraux1->valor ( "idmpatrimonial" ) );
        mui_tabla->setItem ( i, 0, item0 );
        item1 = new QTableWidgetItem ( cursoraux1->valor ( "descmpatrimonial" ) );
        mui_tabla->setItem ( i, 1, item1 );
        cursoraux1->nextRecord();
        i++;
    } // end while
    delete cursoraux1;
    _depura ( "END BcMasaPatrimonialListView::inicializatabla", 0 );
}


///
/**
\param row
\param colummn
\param button
\param mouse
**/
void BcMasaPatrimonialListView::dbtabla ( int row, int colummn, int button, const QPoint &mouse )
{
    _depura ( "BcMasaPatrimonialListView::dbtabla", 0 );
    fprintf ( stderr, "Se ha hecho doble click sobre la tabla\n" );
    /// Dependiendo del modo hacemos una cosa u otra.
    if ( modo == 0 ) {
        QString idmpatrimonial = mui_tabla->item ( row, 0 ) ->text();
        /// Creamos el objeto BcMasaPatrimonialView, y lo lanzamos.
        BcMasaPatrimonialView *masa = new BcMasaPatrimonialView ( ( BcCompany* ) mainCompany(), this );
        masa->inicializa1 ( idmpatrimonial );
        masa->exec();
        delete masa;
        /// Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado.
        inicializatabla();
    } else {
        idmasa = mui_tabla->item ( mui_tabla->currentRow(), 0 ) ->text();
        nommasa = mui_tabla->item ( mui_tabla->currentRow(), 1 ) ->text();
        close();
    } // end if

    /// Para quitar el warning.
    colummn = button = 0;
    mouse.isNull();
    _depura ( "END BcMasaPatrimonialListView::dbtabla", 0 );
}


///
/**
**/
void BcMasaPatrimonialListView::on_mui_editar_released()
{
    _depura ( "BcMasaPatrimonialListView::on_mui_editar_released", 0 );
    int row = mui_tabla->currentRow();
    int col = mui_tabla->currentColumn();
    QPoint mouse;
    dbtabla ( row, col, 0, mouse );
    inicializatabla();
    _depura ( "END BcMasaPatrimonialListView::on_mui_editar_released", 0 );
}


///
/**
**/
void BcMasaPatrimonialListView::on_mui_borrar_released()
{
    _depura ( "BcMasaPatrimonialListView::on_mui_borrar_released", 0 );
    int row;
    row = mui_tabla->currentRow();
    idmasa = mui_tabla->item ( mui_tabla->currentRow(), 0 ) ->text();
    QString query;
    query.sprintf ( "DELETE FROM compmasap WHERE idmpatrimonial=%s", idmasa.toAscii().constData() );
    mainCompany() ->begin();
    mainCompany() ->runQuery ( query );
    query.sprintf ( "DELETE FROM mpatrimonial WHERE idmpatrimonial=%s", idmasa.toAscii().constData() );
    mainCompany() ->runQuery ( query );
    mainCompany() ->commit();
    inicializatabla();
    _depura ( "END BcMasaPatrimonialListView::on_mui_borrar_released", 0 );
}


///
/**
**/
void BcMasaPatrimonialListView::on_mui_nuevo_released()
{
    _depura ( "BcMasaPatrimonialListView::on_mui_nuevo_released", 0 );
    BcMasaPatrimonialView *masa = new BcMasaPatrimonialView ( ( BcCompany * ) mainCompany(), this );
    masa->exec();
    delete masa;
    /// Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado.
    inicializatabla();
    _depura ( "END BcMasaPatrimonialListView::on_mui_nuevo_released", 0 );
}


///
/**
**/
void BcMasaPatrimonialListView::on_mui_cancelar_released()
{
    _depura ( "BcMasaPatrimonialListView::on_mui_cancelar_released", 0 );
    close();
    _depura ( "END BcMasaPatrimonialListView::on_mui_cancelar_released", 0 );
}

