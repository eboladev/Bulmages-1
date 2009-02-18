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

#include "mpatrimonialesview.h"
#include "mpatrimonialview.h"
#include "bccompany.h"


///
/**
\param emp
\param parent
**/
mpatrimonialesview::mpatrimonialesview ( BcCompany *emp, QWidget *parent )
        : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    _depura ( "mpatrimonialesview::mpatrimonialesview", 0 );
    setupUi ( this );
    modo = 0;
    _depura ( "END mpatrimonialesview::mpatrimonialesview", 0 );
}


///
/**
**/
mpatrimonialesview::~mpatrimonialesview()
{
    _depura ( "mpatrimonialesview::~mpatrimonialesview", 0 );
    _depura ( "END mpatrimonialesview::~mpatrimonialesview", 0 );
}


///
/**
**/
void mpatrimonialesview::setmodoselector()
{
    _depura ( "mpatrimonialesview::setmodoselector", 0 );
    modo = 1;
    _depura ( "END mpatrimonialesview::setmodoselector", 0 );
}


///
/**
**/
void mpatrimonialesview::setmodoeditor()
{
    _depura ( "mpatrimonialesview::setmodoeditor", 0 );
    modo = 0;
    _depura ( "END mpatrimonialesview::setmodoeditor", 0 );
}


///
/**
\return
**/
QString mpatrimonialesview::getnommasa()
{
    _depura ( "mpatrimonialesview::getnommasa", 0 );
    _depura ( "END mpatrimonialesview::getnommasa", 0 );
    return nommasa;
}


///
/**
\return
**/
QString mpatrimonialesview::getidmasa()
{
    _depura ( "mpatrimonialesview::getidmasa", 0 );
    _depura ( "END mpatrimonialesview::getidmasa", 0 );
    return idmasa;
}


/// Esta funci&oacute;n se encarga de hacer las inicializaciones de todo el formulario.
/** Se llama as&iacute; y no desde el constructor porque as&iacute; la podemos llamar
    desde dentro de la misma clase, etc, etc, etc. */
/**
\return
**/
int mpatrimonialesview::inicializa()
{
    _depura ( "mpatrimonialesview::inicializa", 0 );
    BlDbRecordSet *cursoraux1;

    /// Vamos a cargar el n&uacute;mero de d&iacute;gitos de cuenta para poder hacer
    /// una introducci&oacute;n de numeros de cuenta m&aacute;s pr&aacute;ctica.
    mainCompany() ->begin();
    QString query = "SELECT valor FROM configuracion WHERE nombre = 'CodCuenta'";
    cursoraux1 = mainCompany() ->cargacursor ( query, "codcuenta" );
    mainCompany() ->commit();
    numdigitos = cursoraux1->valor ( "valor" ).length();
    delete cursoraux1;
    fprintf ( stderr, "las cuentas tienen %d digitos\n", numdigitos );
    inicializatabla();
    _depura ( "END mpatrimonialesview::inicializa", 0 );
    return ( 0 );
}


///
/**
**/
void mpatrimonialesview::inicializatabla()
{
    _depura ( "mpatrimonialesview::inicializatabla", 0 );
    QTableWidgetItem *item0, *item1;
    mui_tabla->setRowCount ( 0 );
    mui_tabla->setColumnCount ( 2 );
    QStringList etiquetas;
    etiquetas << _( "CODIGO" ) << _( "Masa patrimonial" );
    mui_tabla->setHorizontalHeaderLabels ( etiquetas );
    mui_tabla->hideColumn ( 2 );
    mui_tabla->hideColumn ( 0 );
    mui_tabla->setColumnWidth ( 1, 400 );

    QString query = "SELECT * FROM mpatrimonial WHERE idbalance ISNULL";
    mainCompany() ->begin();
    BlDbRecordSet *cursoraux1 = mainCompany() ->cargacursor ( query, "elquery" );
    mainCompany() ->commit();

    mui_tabla->setRowCount ( cursoraux1->numregistros() );
    int i = 0;
    while ( !cursoraux1->eof() ) {
        item0 = new QTableWidgetItem ( cursoraux1->valor ( "idmpatrimonial" ) );
        mui_tabla->setItem ( i, 0, item0 );
        item1 = new QTableWidgetItem ( cursoraux1->valor ( "descmpatrimonial" ) );
        mui_tabla->setItem ( i, 1, item1 );
        cursoraux1->siguienteregistro();
        i++;
    } // end while
    delete cursoraux1;
    _depura ( "END mpatrimonialesview::inicializatabla", 0 );
}


///
/**
\param row
\param colummn
\param button
\param mouse
**/
void mpatrimonialesview::dbtabla ( int row, int colummn, int button, const QPoint &mouse )
{
    _depura ( "mpatrimonialesview::dbtabla", 0 );
    fprintf ( stderr, "Se ha hecho doble click sobre la tabla\n" );
    /// Dependiendo del modo hacemos una cosa u otra.
    if ( modo == 0 ) {
        QString idmpatrimonial = mui_tabla->item ( row, 0 ) ->text();
        /// Creamos el objeto mpatrimonialview, y lo lanzamos.
        mpatrimonialview *masa = new mpatrimonialview ( ( BcCompany* ) mainCompany(), this );
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
    _depura ( "END mpatrimonialesview::dbtabla", 0 );
}


///
/**
**/
void mpatrimonialesview::on_mui_editar_clicked()
{
    _depura ( "mpatrimonialesview::on_mui_editar_clicked", 0 );
    int row = mui_tabla->currentRow();
    int col = mui_tabla->currentColumn();
    QPoint mouse;
    dbtabla ( row, col, 0, mouse );
    inicializatabla();
    _depura ( "END mpatrimonialesview::on_mui_editar_clicked", 0 );
}


///
/**
**/
void mpatrimonialesview::on_mui_borrar_clicked()
{
    _depura ( "mpatrimonialesview::on_mui_borrar_clicked", 0 );
    int row;
    row = mui_tabla->currentRow();
    idmasa = mui_tabla->item ( mui_tabla->currentRow(), 0 ) ->text();
    QString query;
    query.sprintf ( "DELETE FROM compmasap WHERE idmpatrimonial=%s", idmasa.toAscii().constData() );
    mainCompany() ->begin();
    mainCompany() ->ejecuta ( query );
    query.sprintf ( "DELETE FROM mpatrimonial WHERE idmpatrimonial=%s", idmasa.toAscii().constData() );
    mainCompany() ->ejecuta ( query );
    mainCompany() ->commit();
    inicializatabla();
    _depura ( "END mpatrimonialesview::on_mui_borrar_clicked", 0 );
}


///
/**
**/
void mpatrimonialesview::on_mui_nuevo_clicked()
{
    _depura ( "mpatrimonialesview::on_mui_nuevo_clicked", 0 );
    mpatrimonialview *masa = new mpatrimonialview ( ( BcCompany * ) mainCompany(), this );
    masa->exec();
    delete masa;
    /// Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado.
    inicializatabla();
    _depura ( "END mpatrimonialesview::on_mui_nuevo_clicked", 0 );
}


///
/**
**/
void mpatrimonialesview::on_mui_cancelar_clicked()
{
    _depura ( "mpatrimonialesview::on_mui_cancelar_clicked", 0 );
    close();
    _depura ( "END mpatrimonialesview::on_mui_cancelar_clicked", 0 );
}

