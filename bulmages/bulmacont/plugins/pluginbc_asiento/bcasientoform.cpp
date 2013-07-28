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

#include <QtCore/QTextStream>
#include <QtWidgets/QMessageBox>
#include <QtCore/QFile>

#include "bcasientoform.h"
#include "bcasientoview.h"
#include "blfixed.h"
#include "blfunctions.h"
#include "blplugins.h"
#include "bccompany.h"
#include "blmainwindow.h"

///
/**
\param company
\param parent
**/
BcAsientoForm::BcAsientoForm ( BcCompany *company, QWidget *parent ) : BcForm ( company, parent )
{
    BL_FUNC_DEBUG
    setTitleName ( _ ( "Asiento contable" ) );
    setDbTableName ( "asiento" );
    setDbFieldId ( "idasiento" );
    addDbField ( "idasiento", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id asiento" ) );
    addDbField ( "descripcion", BlDbField::DbVarChar, BlDbField::DbNoSave, _ ( "Descripcion del asiento" ) );
    addDbField ( "fecha", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha del asiento" ) );
    addDbField ( "comentariosasiento", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios del asiento" ) );
    addDbField ( "ordenasiento", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Orden de asiento" ) );
    addDbField ( "clase", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Tipo de asiento" ) );
    m_listaLineas = NULL;
    /// Llamamos a los scripts
    blScript(this);
}


///
/**
**/
BcAsientoForm::~BcAsientoForm()
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcAsientoForm::pintaIdAsiento ( QString )
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcAsientoForm::pintaDescripcion ( QString )
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcAsientoForm::pintaFecha ( QString )
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcAsientoForm::pintaComentariosAsiento ( QString )
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcAsientoForm::pintaOrdenAsiento ( QString )
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcAsientoForm::pintaClase ( QString )
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcAsientoForm::calculaPintaTotales()
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcAsientoForm::trataEstadoBcAsientoForm()
{
    BL_FUNC_DEBUG
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
int BcAsientoForm::remove ( bool atendido )
{
    BL_FUNC_DEBUG
    int error;
    if ( dbValue ( "idasiento" ) != "" ) {
        if ( atendido ) {
            switch ( QMessageBox::warning ( 0,
                                            _ ( "Borrar asiento" ),
                                            _ ( "Se va a borrar el asiento. ¿Esta seguro?" ),
                                            QMessageBox::Ok,
                                            QMessageBox::Cancel ) ) {
            case QMessageBox::Ok: /// Retry clicked or Enter pressed.
                mainCompany() ->begin();
                m_listaLineas->remove();
                error = mainCompany() ->runQuery ( "DELETE FROM apunte WHERE idasiento = " + dbValue ( "idasiento" ) );
                error += mainCompany() ->runQuery ( "DELETE FROM asiento WHERE idasiento = " + dbValue ( "idasiento" ) );
                if ( error ) {
                    mainCompany() ->rollback();
                    return -1;
                } // end if
                mainCompany() ->commit();
                vaciar();
                dialogChanges_readValues();
                return 3;
            case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
                return 2;
            } // end switch
        } else {
            mainCompany() ->begin();
            m_listaLineas->remove();
            error = mainCompany() ->runQuery ( "DELETE FROM apunte WHERE idasiento = " + dbValue ( "idasiento" ) );
            error += mainCompany() ->runQuery ( "DELETE FROM asiento WHERE idasiento = " + dbValue ( "idasiento" ) );
            if ( error ) {
                mainCompany() ->rollback();
                return -1;
            } // end if
            mainCompany() ->commit();
            vaciar();
            dialogChanges_readValues();
            return 3;
        } // end if
    } // end if
    
    return 0;
}


///
/**
**/
void BcAsientoForm::vaciar()
{
    BL_FUNC_DEBUG
    DBclear();

    /// Vaciamos compeltamente el subformulario para que no quede ningun tipo de restos
    m_listaLineas->setColumnCount ( 0 );
    m_listaLineas->setRowCount ( 0 );
}


/// Se encarga del pintado del asiento.
/**
**/
void BcAsientoForm::pintar()
{
    BL_FUNC_DEBUG
    pintaIdAsiento ( idAsiento() );
    pintaDescripcion ( dbValue ( "descripcion" ) );
    pintaFecha ( dbValue ( "fecha" ) );
    pintaComentariosAsiento ( dbValue ( "comentariosasiento" ) );
    pintaOrdenAsiento ( dbValue ( "ordenasiento" ) );
    pintaClase ( dbValue ( "clase" ) );
    /// Pintamos los totales.
    calculaPintaTotales();
    trataEstadoBcAsientoForm();
    
}


/// Esta funci&oacute; carga un asiento.
/**
\param idAsiento
\return
**/
int BcAsientoForm::load ( QString idAsiento )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString("idasiento = '%1'").arg(idAsiento) );

    if ( dialogChanges_isChanged() ) {
        switch ( QMessageBox::warning ( this, "BulmaCont",
                                        "Asiento cambiado.\n"
                                        "Desea guardar los cambios \n\n ",
                                        QMessageBox::Save | QMessageBox::Discard
                                        | QMessageBox::Cancel,
                                        QMessageBox::Save ) ) {
        case QMessageBox::Save: // The user clicked the Retry again button or pressed Enter
            // try again
            save();
            break;
        case QMessageBox::Discard: // The user clicked the Quit or pressed Escape
            // exit
            break;
        case QMessageBox::Cancel:
	default:
            return 0;
        } // end switch
    } // end if

    QString query = "SELECT * FROM asiento WHERE idasiento = " + idAsiento;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        DBload ( cur );
    } // end if
    delete cur;
    trataEstadoBcAsientoForm();
    m_listaLineas->load ( idAsiento );
    pintar();

    dialogChanges_readValues();
    
    return 0;
}


///
/**
\return
**/
BlFixed BcAsientoForm::totalDebe ( QString idAsiento )
{
    BL_FUNC_DEBUG
    return m_listaLineas->totalDebe ( idAsiento );
}


///
/**
\return
**/
BlFixed BcAsientoForm::totalHaber ( QString idbudget )
{
    BL_FUNC_DEBUG
    return m_listaLineas->totalHaber ( idbudget );
}


///
/**
\return
**/
void BcAsientoForm::abrir()
{
    BL_FUNC_DEBUG
    if ( estadoBcAsientoForm() != AsientoCerrado ) {
        
        return;
    } // end if
    QString id = dbValue ( "idasiento" );
    if ( id == "" ) {
        
        return;
    }
    BlDbRecordSet *cursoraux = mainCompany() ->loadQuery ( "SELECT abreasiento(" + id + ")" );
    delete cursoraux;
    trataEstadoBcAsientoForm();
    
}


///
/**
\return
**/
void BcAsientoForm::cerrar()
{
    BL_FUNC_DEBUG

    if ( estadoBcAsientoForm() != AsientoAbierto ) {
        return;
    } // end if

    if ( save() )
        return;
    QString id = dbValue ( "idasiento" );

    if ( id == "" ) {
	
        return;
    } // end if

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT cierraasiento(" + id + ")" );
    delete cur;
    vaciar();
    dialogChanges_readValues();
    load ( id );
}


///
/**
\return
**/
BcAsientoForm::EstadoAsiento BcAsientoForm::estadoBcAsientoForm()
{
    BL_FUNC_DEBUG
    if ( dbValue ( "idasiento" ) == "" ) {
        
        return AsientoVacio;
    } // end if

    QString SQLQuery1 = "SELECT count(idapunte) AS cuenta1 FROM apunte WHERE idasiento = " + dbValue ( "idasiento" );
    BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( SQLQuery1 );
    QString numap = cur1->value( "cuenta1" );
    delete cur1;

    QString SQLQuery = "SELECT count(idborrador) AS cuenta FROM borrador WHERE idasiento = " + dbValue ( "idasiento" );
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    QString numborr = cur->value( "cuenta" );
    delete cur;

    if ( numborr == "0" ) {
        return AsientoVacio;
    } else if ( numap != "0" ) {
        return AsientoCerrado;
    } else {
        return AsientoAbierto;
    } // end if
}


///
/**
\return
**/
int BcAsientoForm::save()
{
    BL_FUNC_DEBUG
    QString id;
    mainCompany() ->begin();
    try {
        dbSave ( id );
        setIdAsiento ( id );
        m_listaLineas->save();

        /// Disparamos los plugins
        int res = g_plugins->run ( "BcAsientoForm_guardaAsiento1_post", this );
        if ( res != 0 )
            return 0;

        mainCompany() ->commit();

        if ( estadoBcAsientoForm() == AsientoCerrado ) {
            BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT cierraasiento(" + id + ")" );
            delete cur;
        } // end if
        dialogChanges_readValues();
        load ( id );
        g_main->statusBar() ->showMessage ( _ ( "El asiento se ha guardado correctamente." ), 2000 );
        
        return 0;
    } catch ( ... ) {
        mainCompany() ->rollback();
	blMsgError(_("Error en el guardardo"));
        
        return -1;
    } // end try
}


///
/**
\return
**/
BcCompany *BcAsientoForm::company()
{
    BL_FUNC_DEBUG
    return mainCompany();
}


///
/**
\param val
**/
void BcAsientoForm::setIdAsiento ( QString val )
{
    BL_FUNC_DEBUG
    setDbValue ( "idasiento", val );
    m_listaLineas->setColumnValue ( "idasiento", val );
}


///
/**
\return
**/
QString BcAsientoForm::idAsiento()
{
    BL_FUNC_DEBUG
    return dbValue ( "idasiento" );
}


/// Establece cual es la lista subformulario del presupuesto.
/**
\param a
**/
void BcAsientoForm::setListLinAsiento1 ( BcAsientoSubForm *a )
{
    BL_FUNC_DEBUG
    m_listaLineas = a;
}

