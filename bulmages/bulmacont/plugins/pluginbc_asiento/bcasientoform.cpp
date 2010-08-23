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

#include "bcasientoform.h"
#include "bcasientoview.h"
#include "blfixed.h"
#include "blfunctions.h"
#include "blplugins.h"
#include "bccompany.h"
// #include "bcplancontablearbol.h"
#include "blmainwindow.h"

///
/**
\param comp
\param parent
**/
BcAsientoForm::BcAsientoForm ( BcCompany *comp, QWidget *parent ) : BcForm ( comp, parent )
{
    blDebug ( "BcAsientoForm::BcAsientoForm", 0 );
    setTitleName ( _ ( "Asiento Contable" ) );
    setDbTableName ( "asiento" );
    setDbFieldId ( "idasiento" );
    addDbField ( "idasiento", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id asiento" ) );
    addDbField ( "descripcion", BlDbField::DbVarChar, BlDbField::DbNoSave, _ ( "Descripcion del asiento" ) );
    addDbField ( "fecha", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha del asiento" ) );
    addDbField ( "comentariosasiento", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios del asiento" ) );
    addDbField ( "ordenasiento", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Orden de asiento" ) );
    addDbField ( "clase", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Tipo de asiento" ) );
    listalineas = NULL;
    blDebug ( "END BcAsientoForm::BcAsientoForm", 0 );
}


///
/**
**/
BcAsientoForm::~BcAsientoForm()
{
    blDebug ( "BcAsientoForm::~BcAsientoForm", 0 );
    blDebug ( "END BcAsientoForm::~BcAsientoForm", 0 );
}


///
/**
**/
void BcAsientoForm::pintaidasiento ( QString )
{
    blDebug ( "funcion no implementada pintaidasiento" );
}


///
/**
**/
void BcAsientoForm::pintadescripcion ( QString )
{
    blDebug ( "funcion no implementada pintadescripcion" );
}


///
/**
**/
void BcAsientoForm::pintafecha ( QString )
{
    blDebug ( "funcion no implementada pintafecha" );
}


///
/**
**/
void BcAsientoForm::pintacomentariosasiento ( QString )
{
    blDebug ( "funcion no implementada pintacomentariosasiento", 2 );
}


///
/**
**/
void BcAsientoForm::pintaordenasiento ( QString )
{
    blDebug ( "funcion no implementada pintaordenasiento", 2 );
}


///
/**
**/
void BcAsientoForm::pintaclase ( QString )
{
    blDebug ( "funcion no implementada pintaclase", 2 );
}


///
/**
**/
void BcAsientoForm::calculaypintatotales()
{
    blDebug ( "funcion no implementada calculaypintatotales", 2 );
}


///
/**
**/
void BcAsientoForm::trataestadoBcAsientoForm()
{
    blDebug ( "funcion no implementada trataestadoBcAsientoForm", 2 );
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
int BcAsientoForm::borrar ( bool atendido )
{
    blDebug ( "BcAsientoForm::borrar", 0 );
    int error;
    if ( dbValue ( "idasiento" ) != "" ) {
        if ( atendido ) {
            switch ( QMessageBox::warning ( 0,
                                            _ ( "Borrar asiento" ),
                                            _ ( "Se va a borrar el asiento. Esta seguro?" ),
                                            QMessageBox::Ok,
                                            QMessageBox::Cancel ) ) {
            case QMessageBox::Ok: /// Retry clicked or Enter pressed.
                mainCompany() ->begin();
                listalineas->borrar();
                error = mainCompany() ->runQuery ( "DELETE FROM apunte WHERE idasiento = " + dbValue ( "idasiento" ) );
                error += mainCompany() ->runQuery ( "DELETE FROM asiento WHERE idasiento = " + dbValue ( "idasiento" ) );
                if ( error ) {
                    mainCompany() ->rollback();
                    return -1;
                } // end if
                mainCompany() ->commit();
                vaciar();
                dialogChanges_cargaInicial();
                return 3;
            case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
                return 2;
            } // end switch
        } else {
            mainCompany() ->begin();
            listalineas->borrar();
            error = mainCompany() ->runQuery ( "DELETE FROM apunte WHERE idasiento = " + dbValue ( "idasiento" ) );
            error += mainCompany() ->runQuery ( "DELETE FROM asiento WHERE idasiento = " + dbValue ( "idasiento" ) );
            if ( error ) {
                mainCompany() ->rollback();
                return -1;
            } // end if
            mainCompany() ->commit();
            vaciar();
            dialogChanges_cargaInicial();
            return 3;
        } // end if
    } // end if
    blDebug ( "END BcAsientoForm::borrar", 0 );
    return 0;
}


///
/**
**/
void BcAsientoForm::vaciar()
{
    blDebug ( "BcAsientoForm::vaciar", 0 );
    DBclear();

    /// Vaciamos compeltamente el subformulario para que no quede ningun tipo de restos
    listalineas->setColumnCount ( 0 );
    listalineas->setRowCount ( 0 );
    blDebug ( "END BcAsientoForm::vaciar", 0 );
}


/// Se encarga del pintado del asiento.
/**
**/
void BcAsientoForm::pintar()
{
    blDebug ( "BcAsientoForm::pintar", 0, idasiento() );
    pintaidasiento ( idasiento() );
    pintadescripcion ( dbValue ( "descripcion" ) );
    pintafecha ( dbValue ( "fecha" ) );
    pintacomentariosasiento ( dbValue ( "comentariosasiento" ) );
    pintaordenasiento ( dbValue ( "ordenasiento" ) );
    pintaclase ( dbValue ( "clase" ) );
    /// Pintamos los totales.
    calculaypintatotales();
    trataestadoBcAsientoForm();
    blDebug ( "END BcAsientoForm::pintar", 0 );
}


/// Esta funci&oacute; carga un asiento.
/**
\param idasiento
\return
**/
int BcAsientoForm::cargar ( QString idasiento )
{
    blDebug ( "BcAsientoForm::cargar", 0, idasiento );

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
	default:
            return 0;
        } // end switch
    } // end if

    QString query = "SELECT * FROM asiento WHERE idasiento = " + idasiento;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        DBload ( cur );
    } // end if
    delete cur;
    trataestadoBcAsientoForm();
    listalineas->cargar ( idasiento );
    pintar();

    dialogChanges_cargaInicial();
    blDebug ( "END BcAsientoForm::cargar", 0, idasiento );
    return 0;
}


///
/**
\return
**/
BlFixed BcAsientoForm::totaldebe ( QString idasiento )
{
    return listalineas->totaldebe ( idasiento );
}


///
/**
\return
**/
BlFixed BcAsientoForm::totalhaber ( QString idbudget )
{
    return listalineas->totalhaber ( idbudget );
}


///
/**
\return
**/
void BcAsientoForm::abrir()
{
    blDebug ( "BcAsientoForm::abreBcAsientoForm", 0 );
    if ( estadoBcAsientoForm() != ASCerrado ) {
        blDebug ( "END BcAsientoForm::abreBcAsientoForm", 0, "Asiento Abierto" );
        return;
    } // end if
    QString id = dbValue ( "idasiento" );
    if ( id == "" ) {
        blDebug ( "END BcAsientoForm::abreBcAsientoForm", 0, "Asiento Inexistente" );
        return;
    }
    BlDbRecordSet *cursoraux = mainCompany() ->loadQuery ( "SELECT abreasiento(" + id + ")" );
    delete cursoraux;
    trataestadoBcAsientoForm();
    blDebug ( "END BcAsientoForm::abreBcAsientoForm", 0 );
}


///
/**
\return
**/
void BcAsientoForm::cerrar()
{
    blDebug ( "BcAsientoForm::cerrar", 0 );
    if ( estadoBcAsientoForm() != ASAbierto ) {
        blDebug ( "BcAsientoForm::cerrar" , 0, "asiento no abierto" );
        return;
    } // end if
    if ( guardar() )
        return;
    QString id = dbValue ( "idasiento" );
    if ( id == "" ) {
        blDebug ( "BcAsientoForm::cerrar" , 0, "No hay asiento" );
        return;
    } // end if

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT cierraasiento(" + id + ")" );
    delete cur;
    vaciar();
    dialogChanges_cargaInicial();
    cargar ( id );
    blDebug ( "END BcAsientoForm::cerrar", 0 );
}


///
/**
\return
**/
BcAsientoForm::estadoasiento BcAsientoForm::estadoBcAsientoForm()
{
    blDebug ( "BcAsientoForm::estadoasiento", 0 );
    if ( dbValue ( "idasiento" ) == "" ) {
        blDebug ( "END BcAsientoForm::estadoasiento", 0, "Asiento Vacio" );
        return ASVacio;
    } // end if

    QString SQLQuery1 = "SELECT count(idapunte) AS cuenta1 FROM apunte WHERE idasiento = " + dbValue ( "idasiento" );
    BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( SQLQuery1 );
    QString numap = cur1->valor ( "cuenta1" );
    delete cur1;

    QString SQLQuery = "SELECT count(idborrador) AS cuenta FROM borrador WHERE idasiento = " + dbValue ( "idasiento" );
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    QString numborr = cur->valor ( "cuenta" );
    delete cur;


    if ( numborr == "0" ) {
        blDebug ( "END BcAsientoForm::estadoasiento", 0, "Asiento Vacio" );
        return ASVacio;
    } else if ( numap != "0" ) {
        blDebug ( "END BcAsientoForm::estadoasiento", 0, "Asiento Cerrado" );
        return ASCerrado;
    } else {
        blDebug ( "END BcAsientoForm::estadoasiento", 0, "Asiento Abierto" );
        return ASAbierto;
    } // end if
}


///
/**
\return
**/
int BcAsientoForm::guardar()
{
    blDebug ( "BcAsientoForm::guardar", 0 );
    QString id;
    mainCompany() ->begin();
    try {
        DBsave ( id );
        setidasiento ( id );
        listalineas->guardar();

        /// Disparamos los plugins
        int res = g_plugins->lanza ( "BcAsientoForm_guardaAsiento1_post", this );
        if ( res != 0 )
            return 0;

        mainCompany() ->commit();

        if ( estadoBcAsientoForm() == ASCerrado ) {
            BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT cierraasiento(" + id + ")" );
            delete cur;
        } // end if
        dialogChanges_cargaInicial();
        cargar ( id );
        g_main->statusBar() ->showMessage ( _ ( "El asiento se ha guardado correctamente." ), 2000 );
        blDebug ( "END BcAsientoForm::guardar", 0 );
        return 0;
    } catch ( ... ) {
        mainCompany() ->rollback();
        blDebug ( "END BcAsientoForm::guardar", 0, "Error en el guardado" );
        return -1;
    } // end try
}


///
/**
\return
**/
BcCompany *BcAsientoForm::companyact()
{
    blDebug ( "BcAsientoForm::companyact", 0 );
    blDebug ( "END BcAsientoForm::companyact", 0 );
    return mainCompany();
}


///
/**
\param val
**/
void BcAsientoForm::setidasiento ( QString val )
{
    blDebug ( "BcAsientoForm::setidasiento", 0 );
    setDbValue ( "idasiento", val );
    listalineas->setColumnValue ( "idasiento", val );
    blDebug ( "END BcAsientoForm::setidasiento", 0 );
}


///
/**
\return
**/
QString BcAsientoForm::idasiento()
{
    blDebug ( "BcAsientoForm::idasiento", 0 );
    blDebug ( "END BcAsientoForm::idasiento", 0 );
    return dbValue ( "idasiento" );
}


/// Establece cual es la lista subformulario del presupuesto.
/**
\param a
**/
void BcAsientoForm::setListLinAsiento1 ( BcAsientoSubForm *a )
{
    blDebug ( "BcAsientoForm::setListLinAsiento1", 0 );
    listalineas = a;
    blDebug ( "END BcAsientoForm::setListLinAsiento1", 0 );
}

