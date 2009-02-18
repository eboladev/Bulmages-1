/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
#include <QInputDialog>
#include <QDate>

#include "bbloqfecha.h"
#include "bccompany.h"


///
/**
\param parent
\return
**/
miQTreeWidgetItem::miQTreeWidgetItem ( QTreeWidget *parent )
        : QTreeWidgetItem ( parent )
{
    _depura ( "miQTreeWidgetItem::miQTreeWidgetItem", 0 );
    _depura ( "END miQTreeWidgetItem::miQTreeWidgetItem", 0 );
    return;
}


///
/**
\param parent
\return
**/
miQTreeWidgetItem::miQTreeWidgetItem ( QTreeWidgetItem *parent )
        : QTreeWidgetItem ( parent )
{
    _depura ( "miQTreeWidgetItem::miQTreeWidgetItem", 0 );
    _depura ( "END miQTreeWidgetItem::miQTreeWidgetItem", 0 );
    return;
}


///
/**
\return
**/
miQTreeWidgetItem::~miQTreeWidgetItem()
{
    return;
}


///
/**
\param emp
\param parent
**/
BbloqFecha::BbloqFecha ( BcCompany *emp, QWidget *parent )
        : FichaBc ( emp, parent )
{
    _depura ( "BbloqFecha::BbloqFecha", 0 );

    setTitleName ( _( "Bloquear Fechas" ) );
    /// Establecemos cual es la tabla en la que basarse para obtener permisos
    setDBTableName ( "asiento" );

    qsbloqueado = _( "Bloqueado" );
    qsabierto = _( "Abierto" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    QString query;
    inicializa();
    mainCompany() ->meteWindow ( windowTitle(), this );
    _depura ( "ENd BbloqFecha::BbloqFecha", 0 );
}


///
/**
**/
BbloqFecha::~BbloqFecha()
{
    _depura ( "BbloqFecha::~BbloqFecha", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "ENd BbloqFecha::~BbloqFecha", 0 );
}


///
/**
**/
void BbloqFecha::inicializa()
{
    _depura ( "BbloqFecha::inicializa", 0 );
    miQTreeWidgetItem *itemlevel0;
    miQTreeWidgetItem *itemlevel1 = 0;

    QString consultabd;
    mui_treeWidget->setColumnCount ( 2 );
    QStringList cabecera;
    cabecera << _( "Ejercicio" ) << _( "Estado" );
    mui_treeWidget->setColumnWidth ( 0, 200 );
    mui_treeWidget->setHeaderLabels ( cabecera );

    mui_treeWidget->clear();
    mui_treeWidget->setSortingEnabled ( FALSE );

    /// Consultamos a la base de datos.
    consultabd.sprintf ( "SELECT * FROM ejercicios WHERE periodo = 0 ORDER BY ejercicio DESC" );
    BlDbRecordSet *curPeri, *curEjer = mainCompany() ->cargacursor ( consultabd );

    while ( !curEjer->eof() ) {

        itemlevel0 = new miQTreeWidgetItem ( mui_treeWidget );
        itemlevel0->setText ( 0, curEjer->valor ( "ejercicio" ) ); /// Columna 0.

        if ( curEjer->valor ( "bloqueado" ) == "t" ) {
            itemlevel0->setText ( 1, qsbloqueado );
        } else {
            itemlevel0->setText ( 1, qsabierto );
        } // end if

        itemlevel0->ej = curEjer->valor ( "ejercicio" );
        itemlevel0->per = curEjer->valor ( "periodo" );


        consultabd.sprintf ( "SELECT * FROM ejercicios WHERE ejercicio = '%s' ORDER BY periodo DESC", curEjer->valor ( "ejercicio" ).toAscii().constData() );
        curPeri = mainCompany() ->cargacursor ( consultabd );
        while ( !curPeri->eof() ) {
            switch ( curPeri->valor ( "periodo" ).toInt() ) {
            case 12:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Diciembre" ) ); /// Columna 0.
                break;
            case 11:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Noviembre" ) ); /// Columna 0.
                break;
            case 10:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Octubre" ) ); /// Columna 0.
                break;
            case 9:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Septiembre" ) ); /// Columna 0.
                break;
            case 8:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Agosto" ) ); /// Columna 0.
                break;
            case 7:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Julio" ) ); /// Columna 0.
                break;
            case 6:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Junio" ) ); /// Columna 0.
                break;
            case 5:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Mayo" ) ); /// Columna 0.
                break;
            case 4:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Abril" ) ); /// Columna 0.
                break;
            case 3:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Marzo" ) ); /// Columna 0.
                break;
            case 2:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Febrero" ) ); /// Columna 0.
                break;
            case 1:
                itemlevel1 = new miQTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _( "Enero" ) ); /// Columna 0.
                break;
            } // end switch
            itemlevel1->ej = curEjer->valor ( "ejercicio" );
            itemlevel1->per = curPeri->valor ( "periodo" );
            curPeri->valor ( "bloqueado" ) == "t" ? itemlevel1->setText ( 1, qsbloqueado ) : itemlevel1->setText ( 1, qsabierto );
            curPeri->siguienteregistro();
        } // end while
        curEjer->siguienteregistro();
    } // end while
    _depura ( "ENd BbloqFecha::inicializa", 0 );
}


///
/**
\param item
\param columna
**/
void BbloqFecha::on_mui_treeWidget_itemDoubleClicked ( QTreeWidgetItem *item, int columna )
{
    _depura ( "BbloqFecha::on_mui_treeWidget_doubleClicked", 0 );
    int error;
    miQTreeWidgetItem *it = ( miQTreeWidgetItem * ) item;
    if ( columna == 1 ) {
        if ( item->text ( 1 ) == qsbloqueado ) {
            item->setText ( 1, qsabierto );
            QString consultabd = "UPDATE ejercicios SET bloqueado = FALSE WHERE ejercicio = '" + it->ej + "' AND periodo = '" + it->per + "'";
            error = mainCompany() ->ejecuta ( consultabd );
        } else {
            item->setText ( 1, qsbloqueado );
            QString consultabd = "UPDATE ejercicios SET bloqueado = TRUE WHERE ejercicio = '" + it->ej + "' AND periodo = '" + it->per + "'";
            error = mainCompany() ->ejecuta ( consultabd );
        } // end if
    } // end if
    _depura ( "END BbloqFecha::on_mui_treeWidget_doubleClicked", 0 );
}


///
/**
**/
void BbloqFecha::on_mui_crear_clicked()
{
    _depura ( "BbloqFecha::on_mui_crear_clicked", 0 );
    /// Tomamos el anyo pasado como referencia.
    int ejer = QDate::currentDate().year() - 1;

    /// Miramos si ya hay ejercicios introducidos y si es asi cogemos el siguiente como referencia.
    QString consultabd = "SELECT max(ejercicio) AS ej FROM ejercicios";
    BlDbRecordSet *cur = mainCompany()->cargacursor ( consultabd );
    if ( cur ) {
        if ( !cur->eof() ) {
            if ( cur->valor ( "ej" ).toInt() != 0 )
                ejer = cur->valor ( "ej" ).toInt();
        } // end if
        delete cur;
    } // end if
    ejer++;

    /// Presentamos el dialogo preguntado que ejercicio crear.
    bool ok = FALSE;
    ejer = QInputDialog::getInteger ( this, _( "Introduzca Ejercicio a Crear" ),
                                      _( "Ponga el anyo:" ), ejer, 0, 10000, 1, &ok );

    /// Comprobamos que el ejercicio introducido sea valido.
    if ( !ok ) return;
    if ( ejer < 1000 ) {
        mensajeInfo ( "Ejercicio invalido" );
        return;
    } // end if
    QString query = "SELECT * FROM ejercicios WHERE ejercicio = " + QString::number ( ejer );
    cur = mainCompany()->cargacursor ( query );
    if ( cur ) {
        if ( cur->numregistros() > 0 ) {
            delete cur;
            mensajeInfo ( _( "Ejercicios ya contemplado" ) );
            return;
        } // end if
    } // end if


    /// Creamos el ejercicio.
    for ( int x = 0; x <= 12; x++ ) {
        try {
            QString consultabd = "INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES('" + QString::number ( ejer ) + "', '" + QString::number ( x ) + "', 'f')";
            mainCompany()->begin();
            mainCompany() ->ejecuta ( consultabd );
            mainCompany()->commit();
        } catch ( ... ) {
            mensajeInfo ( "Error con la base de datos" );
            mainCompany()->rollback();
        } // end try
    } // end for

    /// Presentamos
    inicializa();

    _depura ( "BbloqFecha::on_mui_crear_clicked", 0 );
}

