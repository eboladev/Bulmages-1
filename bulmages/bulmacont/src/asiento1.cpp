/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QTextStream>
#include <QMessageBox>
#include <QFile>

#include "asiento1.h"
#include "asiento1view.h"
#include "blfixed.h"
#include "funcaux.h"
#include "plugins.h"
#include "empresa.h"
#include "arbol.h"


///
/**
\param comp
\param parent
**/
Asiento1::Asiento1 ( Empresa *comp, QWidget *parent ) : FichaBc ( comp, parent )
{
    _depura ( "Asiento1::Asiento1", 0 );
    setTitleName ( _( "Asiento Contable" ) );
    setDBTableName ( "asiento" );
    setDBCampoId ( "idasiento" );
    addDBCampo ( "idasiento", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Id asiento" ) );
    addDBCampo ( "descripcion", BlDbField::DBvarchar, BlDbField::DBNoSave, _( "Descripcion del asiento" ) );
    addDBCampo ( "fecha", BlDbField::DBdate, BlDbField::DBNothing, _( "Fecha del asiento" ) );
    addDBCampo ( "comentariosasiento", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Comentarios del asiento" ) );
    addDBCampo ( "ordenasiento", BlDbField::DBint, BlDbField::DBNothing, _( "Orden de asiento" ) );
    addDBCampo ( "clase", BlDbField::DBint, BlDbField::DBNothing, _( "Tipo de asiento" ) );
    listalineas = NULL;
    _depura ( "END Asiento1::Asiento1", 0 );
}


///
/**
**/
Asiento1::~Asiento1()
{
    _depura ( "Asiento1::~Asiento1", 0 );
    _depura ( "END Asiento1::~Asiento1", 0 );
}


///
/**
**/
void Asiento1::pintaidasiento ( QString )
{
    _depura ( "funcion no implementada pintaidasiento" );
}


///
/**
**/
void Asiento1::pintadescripcion ( QString )
{
    _depura ( "funcion no implementada pintadescripcion" );
}


///
/**
**/
void Asiento1::pintafecha ( QString )
{
    _depura ( "funcion no implementada pintafecha" );
}


///
/**
**/
void Asiento1::pintacomentariosasiento ( QString )
{
    _depura ( "funcion no implementada pintacomentariosasiento", 2 );
}


///
/**
**/
void Asiento1::pintaordenasiento ( QString )
{
    _depura ( "funcion no implementada pintaordenasiento", 2 );
}


///
/**
**/
void Asiento1::pintaclase ( QString )
{
    _depura ( "funcion no implementada pintaclase", 2 );
}


///
/**
**/
void Asiento1::calculaypintatotales()
{
    _depura ( "funcion no implementada calculaypintatotales", 2 );
}


///
/**
**/
void Asiento1::trataestadoAsiento1()
{
    _depura ( "funcion no implementada trataestadoAsiento1", 2 );
}





/** Metodo que se encarga del borrado completo de un asiento.
    Devuelve     0: No se ha especificado asiento a borrar.
                 2: Se ha cancelado la accion.
                 3: Se ha borrado correctamente.
                -1: Ha habido algun error.
*/
///
/**
\param atendido
\return
**/
int Asiento1::borrar ( bool atendido )
{
    _depura ( "Asiento1::borrar", 0 );
    int error;
    if ( DBvalue ( "idasiento" ) != "" ) {
        if ( atendido ) {
            switch ( QMessageBox::warning ( 0,
                                            _( "Borrar asiento" ),
                                            _( "Se va a borrar el asiento. Esta seguro?" ),
                                            QMessageBox::Ok,
                                            QMessageBox::Cancel ) ) {
            case QMessageBox::Ok: /// Retry clicked or Enter pressed.
                empresaBase() ->begin();
                listalineas->borrar();
                error = empresaBase() ->ejecuta ( "DELETE FROM apunte WHERE idasiento = " + DBvalue ( "idasiento" ) );
                error += empresaBase() ->ejecuta ( "DELETE FROM asiento WHERE idasiento = " + DBvalue ( "idasiento" ) );
                if ( error ) {
                    empresaBase() ->rollback();
                    return -1;
                } // end if
                empresaBase() ->commit();
                vaciar();
                dialogChanges_cargaInicial();
                return 3;
            case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
                return 2;
            } // end switch
        } else {
            empresaBase() ->begin();
            listalineas->borrar();
            error = empresaBase() ->ejecuta ( "DELETE FROM apunte WHERE idasiento = " + DBvalue ( "idasiento" ) );
            error += empresaBase() ->ejecuta ( "DELETE FROM asiento WHERE idasiento = " + DBvalue ( "idasiento" ) );
            if ( error ) {
                empresaBase() ->rollback();
                return -1;
            } // end if
            empresaBase() ->commit();
            vaciar();
            dialogChanges_cargaInicial();
            return 3;
        } // end if
    } // end if
    _depura ( "END Asiento1::borrar", 0 );
    return 0;
}


///
/**
**/
void Asiento1::vaciar()
{
    _depura ( "Asiento1::vaciar", 0 );
    DBclear();

    /// Vaciamos compeltamente el subformulario para que no quede ningun tipo de restos
    listalineas->setColumnCount ( 0 );
    listalineas->setRowCount ( 0 );
    _depura ( "END Asiento1::vaciar", 0 );
}


/// Se encarga del pintado del asiento.
/**
**/
void Asiento1::pintar()
{
    _depura ( "Asiento1::pintar", 0, idasiento() );
    pintaidasiento ( idasiento() );
    pintadescripcion ( DBvalue ( "descripcion" ) );
    pintafecha ( DBvalue ( "fecha" ) );
    pintacomentariosasiento ( DBvalue ( "comentariosasiento" ) );
    pintaordenasiento ( DBvalue ( "ordenasiento" ) );
    pintaclase ( DBvalue ( "clase" ) );
    /// Pintamos los totales.
    calculaypintatotales();
    trataestadoAsiento1();
    _depura ( "END Asiento1::pintar", 0 );
}


/// Esta funci&oacute; carga un asiento.
/**
\param idasiento
\return
**/
int Asiento1::cargar ( QString idasiento )
{
    _depura ( "Asiento1::cargar", 0, idasiento );

    if ( dialogChanges_hayCambios() ) {
        switch ( QMessageBox::warning ( this, "BulmaCont",
                                        "Asiento cambiado.\n"
                                        "Desea guardar los cambios \n\n ",
                                        QMessageBox::Save | QMessageBox::Discard
                                        | QMessageBox::Cancel,
                                        QMessageBox::Save ) ) {
        case QMessageBox::Save: // The user clicked the Retry again button or pressed Enter
            // try again
            guardar();
            break;
        case QMessageBox::Discard: // The user clicked the Quit or pressed Escape
            // exit
            break;
        case QMessageBox::Cancel:
            return 0;
        }
    } // end if

    QString query = "SELECT * FROM asiento WHERE idasiento = " + idasiento;
    cursor2 *cur = empresaBase() ->cargacursor ( query );
    if ( !cur->eof() ) {
        DBload ( cur );
    } // end if
    delete cur;
    trataestadoAsiento1();
    listalineas->cargar ( idasiento );
    pintar();

    dialogChanges_cargaInicial();
    _depura ( "END Asiento1::cargar", 0, idasiento );
    return 0;
}


///
/**
\return
**/
BlFixed Asiento1::totaldebe ( QString idasiento )
{
    return listalineas->totaldebe ( idasiento );
}


///
/**
\return
**/
BlFixed Asiento1::totalhaber ( QString idbudget )
{
    return listalineas->totalhaber ( idbudget );
}


///
/**
\return
**/
void Asiento1::abrir()
{
    _depura ( "Asiento1::abreAsiento1", 0 );
    if ( estadoAsiento1() != ASCerrado ) {
        _depura ( "END Asiento1::abreAsiento1", 0, "Asiento Abierto" );
        return;
    } // end if
    QString id = DBvalue ( "idasiento" );
    if ( id == "" ) {
        _depura ( "END Asiento1::abreAsiento1", 0, "Asiento Inexistente" );
        return;
    }
    empresaBase() ->abreasiento ( id.toInt() );
    trataestadoAsiento1();
    _depura ( "END Asiento1::abreAsiento1", 0 );
}


///
/**
\return
**/
void Asiento1::cerrar()
{
    _depura ( "Asiento1::cerrar", 0 );
    if ( estadoAsiento1() != ASAbierto ) {
        _depura ( "Asiento1::cerrar" , 0, "asiento no abierto" );
        return;
    } // end if
    if ( guardar() )
        return;
    QString id = DBvalue ( "idasiento" );
    if ( id == "" ) {
        _depura ( "Asiento1::cerrar" , 0, "No hay asiento" );
        return;
    } // end if

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT cierraasiento(" + id + ")" );
    delete cur;
    vaciar();
    dialogChanges_cargaInicial();
    cargar ( id );
    _depura ( "END Asiento1::cerrar", 0 );
}


///
/**
\return
**/
Asiento1::estadoasiento Asiento1::estadoAsiento1()
{
    _depura ( "Asiento1::estadoasiento", 0 );
    if ( DBvalue ( "idasiento" ) == "" ) {
        _depura ( "END Asiento1::estadoasiento", 0, "Asiento Vacio" );
        return ASVacio;
    } // end if

    QString SQLQuery1 = "SELECT count(idapunte) AS cuenta1 FROM apunte WHERE idasiento = " + DBvalue ( "idasiento" );
    cursor2 *cur1 = empresaBase() ->cargacursor ( SQLQuery1 );
    QString numap = cur1->valor ( "cuenta1" );
    delete cur1;

    QString SQLQuery = "SELECT count(idborrador) AS cuenta FROM borrador WHERE idasiento = " + DBvalue ( "idasiento" );
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    QString numborr = cur->valor ( "cuenta" );
    delete cur;


    if ( numborr == "0" ) {
        _depura ( "END Asiento1::estadoasiento", 0, "Asiento Vacio" );
        return ASVacio;
    } else if ( numap != "0" ) {
        _depura ( "END Asiento1::estadoasiento", 0, "Asiento Cerrado" );
        return ASCerrado;
    } else {
        _depura ( "END Asiento1::estadoasiento", 0, "Asiento Abierto" );
        return ASAbierto;
    } // end if
}


///
/**
\return
**/
int Asiento1::guardar()
{
    _depura ( "Asiento1::guardar", 0 );
    QString id;
    empresaBase() ->begin();
    try {
        DBsave ( id );
        setidasiento ( id );
        listalineas->guardar();

        /// Disparamos los plugins
        int res = g_plugins->lanza ( "Asiento1_guardaAsiento1_post", this );
        if ( res != 0 )
            return 0;

        empresaBase() ->commit();

        if ( estadoAsiento1() == ASCerrado )
            empresaBase() ->cierraasiento ( id.toInt() );
        dialogChanges_cargaInicial();
        cargar ( id );
        g_main->statusBar() ->showMessage ( _( "El asiento se ha guardado correctamente." ), 2000 );
        _depura ( "END Asiento1::guardar", 0 );
        return 0;
    } catch ( ... ) {
        empresaBase() ->rollback();
        _depura ( "END Asiento1::guardar", 0, "Error en el guardado" );
        return -1;
    } // end try
}


///
/**
\return
**/
Empresa *Asiento1::companyact()
{
    _depura ( "Asiento1::companyact", 0 );
    _depura ( "END Asiento1::companyact", 0 );
    return empresaBase();
}


///
/**
\param val
**/
void Asiento1::setidasiento ( QString val )
{
    _depura ( "Asiento1::setidasiento", 0 );
    setDBvalue ( "idasiento", val );
    listalineas->setColumnValue ( "idasiento", val );
    _depura ( "END Asiento1::setidasiento", 0 );
}


///
/**
\return
**/
QString Asiento1::idasiento()
{
    _depura ( "Asiento1::idasiento", 0 );
    _depura ( "END Asiento1::idasiento", 0 );
    return DBvalue ( "idasiento" );
}


/// Establece cual es la lista subformulario del presupuesto.
/**
\param a
**/
void Asiento1::setListLinAsiento1 ( ListLinAsiento1View *a )
{
    _depura ( "Asiento1::setListLinAsiento1", 0 );
    listalineas = a;
    _depura ( "END Asiento1::setListLinAsiento1", 0 );
}

