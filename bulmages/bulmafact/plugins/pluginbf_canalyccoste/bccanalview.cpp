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

#include  "bccanalview.h"
#include  "bfcompany.h"
#include  "bccanalseleccionarview.h"


///
/**
\param emp
\param parent
**/
BcCanalView::BcCanalView ( BfCompany  *emp, QWidget *parent )
        : BfForm ( emp, parent )
{
    BL_FUNC_DEBUG

    setTitleName ( _ ( "Canal" ) );
    /// EStablezco cual es la tabla en la que basarse para los permisos
    setDbTableName ( "canal" );

    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_idCanal->setMainCompany ( emp );
    mui_idCanal->setId ( "0" );

    m_nomCanal = new QLineEdit();
    m_nomCanal->setText ( "" );
    m_descCanal = new QTextEdit();
    m_descCanal->setPlainText ( "" );

    idcanal = 0;
    dialogChanges_setExcludedObject ( mui_idCanal );
    dialogChanges_readValues();
    on_mui_idCanal_valueChanged ( 0 );
    mainCompany() ->insertWindow ( windowTitle(), this );
    
}


///
/**
**/
BcCanalView::~BcCanalView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


///
/**
**/
void BcCanalView::pintar()
{
    BL_FUNC_DEBUG
    /// Si el combocoste no esta vacio se muestra el registro que contiene.
    if ( idcanal != 0 ) {
        /// Habilita los campos para ser editados.
        mui_nomCanal->setEnabled ( true );
        mui_descCanal->setEnabled ( true );
        mostrarPlantilla();
    } else {
        idcanal = mui_idCanal->id().toInt();
        mostrarPlantilla();
    } // end if

    /// Si se han cambiado los canales, se rehace el selector de canales.
//    BcCanalSeleccionarView *scanal = mainCompany() ->getselcanales();
//    scanal->cargaCanales();
    
}


/// Esta funci&oacute;n sirve para hacer el cambio sobre un centro de coste.
/**
\param numcombo
**/
void BcCanalView::on_mui_idCanal_valueChanged ( QString numcombo )
{
    BL_FUNC_DEBUG
    int idcanal1 = numcombo.toInt();
    static bool flipflop = false;

    if ( dialogChanges_isChanged() && flipflop ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar canal" ),
                                    _ ( "Desea guardar los cambios." ),
                                    _ ( "&Guardar" ), _ ( "&Cancelar" ), 0 , 0, 1 ) == 0 )
            on_mui_guardar_clicked();
    } // end if

    /// Comprueba que el ID del 'combobox' este en la primera posicion '--' y deshabilita los
    /// campos de texto.
    if ( idcanal1 == 0 ) {
        mui_nomCanal->setText ( "" );
        mui_nomCanal->setEnabled ( false );
        mui_descCanal->setPlainText ( "" );
        mui_descCanal->setEnabled ( false );
        flipflop = false;
        return;
    } else {
        /// Habilita los campos para ser editados.
        mui_nomCanal->setEnabled ( true );
        mui_descCanal->setEnabled ( true );
    } // end if

    flipflop = true;
    idcanal = idcanal1;
    mostrarPlantilla();
    
}


/// Esta funci&oacute;n muestra el canal en la ventana.
/**
**/
void BcCanalView::mostrarPlantilla()
{
    BL_FUNC_DEBUG
    QString query;
    QTextStream ( &query ) << "SELECT * from canal WHERE idcanal = '" << idcanal << "'";
    BlDbRecordSet *cursorcanal = mainCompany() ->loadQuery ( query );
    if ( !cursorcanal->eof() ) {
        mui_nomCanal->setText ( cursorcanal->value( "nombre" ) );
        mui_descCanal->setPlainText ( cursorcanal->value( "descripcion" ) );
    } // end if
    mui_idCanal->setId ( QString::number ( idcanal ) );
    dialogChanges_readValues();
    

}


///
/**
**/
void BcCanalView::on_mui_guardar_clicked()
{
    BL_FUNC_DEBUG
    QString nom = mui_nomCanal->text();
    QString desc = mui_descCanal->toPlainText();
    QString query;
    QTextStream ( &query ) << "UPDATE canal SET nombre = '"
    << mainCompany() ->sanearCadena ( nom ).toLatin1().constData()
    << "', descripcion = '"
    << mainCompany() ->sanearCadena ( desc ).toLatin1().constData()
    << "' WHERE idcanal = '" << idcanal << "'";
    mainCompany() ->runQuery ( query );
    dialogChanges_readValues();
    pintar();
    
}


///
/**
**/
void BcCanalView::on_mui_crear_clicked()
{
    BL_FUNC_DEBUG
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar canal" ),
                                    _ ( "Desea guardar los cambios." ),
                                    _ ( "&Guardar" ), _ ( "&Cancelar" ), 0 , 0, 1 ) == 0 )
            on_mui_guardar_clicked();
    } // end if
    QString query = "";
    QTextStream ( &query ) << "INSERT INTO canal (nombre, descripcion) VALUES ('" << _ ( "Nuevo canal" ) << "', '" << _ ( "Escriba su descripcion" ) << "')";
    mainCompany() ->begin();
    mainCompany() ->runQuery ( query );
    query = "";
    QTextStream ( &query ) << "SELECT MAX(idcanal) AS id FROM canal";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query, "queryy" );
    idcanal = atoi ( cur->value( "id" ).toLatin1() );
    delete cur;
    mainCompany() ->commit();
    pintar();
    
}


///
/**
**/
void BcCanalView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
    switch ( QMessageBox::warning ( this,
                                    _ ( "Borrar canal" ),
                                    _ ( "Se va a borrar este canal.\nEsto puede ocasionar perdida de datos." ),
                                    _ ( "&Borrar" ), _ ( "&Cancelar" ), 0 , 0, 1 ) ) {
    case 0: /// Retry clicked or Enter pressed.
        QString query;
        query.sprintf ( "DELETE FROM canal WHERE idcanal = %d", idcanal );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        idcanal = 0;
        pintar();
    } // end switch
    
}


///
/**
\param e
**/
void BcCanalView::closeEvent ( QCloseEvent *e )
{
    BL_FUNC_DEBUG
    if ( dialogChanges_isChanged() ) {
        int val = QMessageBox::warning ( this,
                                         _ ( "Guardar canal" ),
                                         _ ( "Desea guardar los cambios?" ),
                                         _ ( "&Si" ), _ ( "&No" ), _ ( "&Cancelar" ), 0, 2 );
        if ( val == 0 )
            on_mui_guardar_clicked();
        if ( val == 2 )
            e->ignore();
    } // end if
    
}

