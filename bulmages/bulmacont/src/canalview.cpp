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

#include  "canalview.h"
#include  "bccompany.h"
#include  "selectcanalview.h"


///
/**
\param emp
\param parent
**/
canalview::canalview ( BcCompany  *emp, QWidget *parent )
        : FichaBc ( emp, parent )
{
    _depura ( "canalview::canalview", 0 );

    setTitleName ( _( "Canal" ) );
    /// EStablezco cual es la tabla en la que basarse para los permisos
    setDbTableName ( "canal" );

    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_idcanal->setMainCompany ( emp );
    mui_idcanal->setidcanal ( "0" );

    m_nomcanal = new QLineEdit();
    m_nomcanal->setText ( "" );
    m_desccanal = new QTextEdit();
    m_desccanal->setPlainText ( "" );

    idcanal = 0;
    dialogChanges_setQObjectExcluido ( mui_idcanal );
    dialogChanges_cargaInicial();
    on_mui_idcanal_valueChanged ( 0 );
    mainCompany() ->meteWindow ( windowTitle(), this );
    _depura ( "END canalview::canalview", 0 );
}


///
/**
**/
canalview::~canalview()
{
    _depura ( "canalview::~canalview", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "END canalview::~canalview", 0 );
}


///
/**
**/
void canalview::pintar()
{
    _depura ( "canalview::pintar", 0 );
    /// Si el combocoste no esta vacio se muestra el registro que contiene.
    if ( idcanal != 0 ) {
        /// Habilita los campos para ser editados.
        mui_nomcanal->setEnabled ( TRUE );
        mui_desccanal->setEnabled ( TRUE );
        mostrarplantilla();
    } else {
        idcanal = mui_idcanal->idcanal().toInt();
        mostrarplantilla();
    } // end if

    /// Si se han cambiado los canales, se rehace el selector de canales.
    selectcanalview *scanal = mainCompany() ->getselcanales();
    scanal->cargacanales();
    _depura ( "END canalview::pintar", 0 );
}


/// Esta funci&oacute;n sirve para hacer el cambio sobre un centro de coste.
/**
\param numcombo
**/
void canalview::on_mui_idcanal_valueChanged ( QString numcombo )
{
    _depura ( "canalview::on_mui_idcanal_valueChanged", 0 );
    int idcanal1 = numcombo.toInt();
    static bool flipflop = FALSE;

    if ( dialogChanges_hayCambios() && flipflop ) {
        if ( QMessageBox::warning ( this,
                                    _( "Guardar canal" ),
                                    _( "Desea guardar los cambios." ),
                                    _( "&Guardar" ), _( "&Cancelar" ), 0 , 0, 1 ) == 0 )
            on_mui_guardar_clicked();
    } // end if

    /// Comprueba que el ID del 'combobox' este en la primera posicion '--' y deshabilita los
    /// campos de texto.
    if ( idcanal1 == 0 ) {
        mui_nomcanal->setText ( "" );
        mui_nomcanal->setEnabled ( FALSE );
        mui_desccanal->setPlainText ( "" );
        mui_desccanal->setEnabled ( FALSE );
        flipflop = FALSE;
        return;
    } else {
        /// Habilita los campos para ser editados.
        mui_nomcanal->setEnabled ( TRUE );
        mui_desccanal->setEnabled ( TRUE );
    } // end if

    flipflop = TRUE;
    idcanal = idcanal1;
    mostrarplantilla();
    _depura ( "END canalview::on_mui_idcanal_valueChanged", 0 );
}


/// Esta funci&oacute;n muestra el canal en la ventana.
/**
**/
void canalview::mostrarplantilla()
{
    _depura ( "canalview::mostrarplantilla", 0 );
    QString query;
    QTextStream ( &query ) << "SELECT * from canal WHERE idcanal = '" << idcanal << "'";
    BlDbRecordSet *cursorcanal = mainCompany() ->cargacursor ( query );
    if ( !cursorcanal->eof() ) {
        mui_nomcanal->setText ( cursorcanal->valor ( "nombre" ) );
        mui_desccanal->setPlainText ( cursorcanal->valor ( "descripcion" ) );
    } // end if
    mui_idcanal->setidcanal ( QString::number ( idcanal ) );
    dialogChanges_cargaInicial();
    _depura ( "END canalview::mostrarplantilla", 0 );

}


///
/**
**/
void canalview::on_mui_guardar_clicked()
{
    _depura ( "canalview::on_mui_guardar_clicked", 0 );
    QString nom = mui_nomcanal->text();
    QString desc = mui_desccanal->toPlainText();
    QString query;
    QTextStream ( &query ) << "UPDATE canal SET nombre = '"
    << mainCompany() ->sanearCadena ( nom ).toAscii().constData()
    << "', descripcion = '"
    << mainCompany() ->sanearCadena ( desc ).toAscii().constData()
    << "' WHERE idcanal = '" << idcanal << "'";
    mainCompany() ->ejecuta ( query );
    dialogChanges_cargaInicial();
    pintar();
    _depura ( "END canalview::on_mui_guardar_clicked", 0 );
}


///
/**
**/
void canalview::on_mui_crear_clicked()
{
    _depura ( "canalview::on_mui_crear_clicked", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _( "Guardar canal" ),
                                    _( "Desea guardar los cambios." ),
                                    _( "&Guardar" ), _( "&Cancelar" ), 0 , 0, 1 ) == 0 )
            on_mui_guardar_clicked();
    } // end if
    QString query = "";
    QTextStream ( &query ) << "INSERT INTO canal (nombre, descripcion) VALUES ('" << _( "Nuevo canal" ) << "', '" << _( "Escriba su descripcion" ) << "')";
    mainCompany() ->begin();
    mainCompany() ->ejecuta ( query );
    query = "";
    QTextStream ( &query ) << "SELECT MAX(idcanal) AS id FROM canal";
    BlDbRecordSet *cur = mainCompany() ->cargacursor ( query, "queryy" );
    idcanal = atoi ( cur->valor ( "id" ).toAscii() );
    delete cur;
    mainCompany() ->commit();
    pintar();
    _depura ( "END canalview::on_mui_crear_clicked", 0 );
}


///
/**
**/
void canalview::on_mui_borrar_clicked()
{
    _depura ( "canalview::on_mui_borrar_clicked", 0 );
    switch ( QMessageBox::warning ( this,
                                    _( "Borrar canal" ),
                                    _( "Se va a borrar este canal.\nEsto puede ocasionar perdida de datos." ),
                                    _( "&Borrar" ), _( "&Cancelar" ), 0 , 0, 1 ) ) {
    case 0: /// Retry clicked or Enter pressed.
        QString query;
        query.sprintf ( "DELETE FROM canal WHERE idcanal = %d", idcanal );
        mainCompany() ->begin();
        mainCompany() ->ejecuta ( query );
        mainCompany() ->commit();
        idcanal = 0;
        pintar();
    } // end switch
    _depura ( "END canalview::on_mui_borrar_clicked", 0 );
}


///
/**
\param e
**/
void canalview::closeEvent ( QCloseEvent *e )
{
    _depura ( "ccosteview::closeEvent", 0 );
    if ( dialogChanges_hayCambios() ) {
        int val = QMessageBox::warning ( this,
                                         _( "Guardar canal" ),
                                         _( "Desea guardar los cambios?" ),
                                         _( "&Si" ), _( "&No" ), _( "&Cancelar" ), 0, 2 );
        if ( val == 0 )
            on_mui_guardar_clicked();
        if ( val == 2 )
            e->ignore();
    } // end if
    _depura ( "END ccosteview::closeEvent", 0 );
}

