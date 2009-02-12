/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#include "funcaux.h"
#include "propiedadesempresa.h"
#include "empresa.h"


///
/**
\param emp
\param parent
**/
propiedadesempresa::propiedadesempresa ( Empresa *emp, QWidget *parent )
        : FichaBc ( emp, parent )
{
    _depura ( "propiedadesempresa::propiedadesempresa", 0 );

    setTitleName ( _( "Configuracion" ) );
    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDBTableName ( "configuracion" );

    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    inicializa();

    mui_subform->setEmpresaBase ( empresaBase() );
    mui_subform->cargar();
    //mui_subform->setResizeMode(QHeaderView::Stretch);

    empresaBase() ->meteWindow ( windowTitle(), this );
    _depura ( "END propiedadesempresa::propiedadesempresa", 0 );
}


///
/**
**/
propiedadesempresa::~propiedadesempresa()
{
    _depura ( "propiedadesempresa::~propiedadesempresa", 0 );
    empresaBase() ->sacaWindow ( this );
    _depura ( "END propiedadesempresa::~propiedadesempresa", 0 );

}


///
/**
\return
**/
int propiedadesempresa::inicializa()
{
    _depura ( "propiedadesempresa::inicializa", 0 );
    int num;

    QString query = "SELECT * FROM configuracion WHERE nombre = 'CodCuenta'";
    cursor2 *curs = empresaBase() ->cargacursor ( query );
    num = curs->numregistros();
    if ( num > 0 ) {
        modcodigo->setText ( curs->valor ( "valor" ) );
    } // end if
    delete curs;

    _depura ( "END propiedadesempresa::inicializa", 0 );
    return 0;
}


///
/**
**/
void propiedadesempresa::on_mui_guardar_clicked()
{
    _depura ( "propiedadesempresa::on_mui_guardar_clicked", 0 );
    /// Iniciamos transaccion.
    empresaBase() ->begin();
    mui_subform->guardar();

    /// Procesamos la transaccion.
    empresaBase() ->commit();
    dialogChanges_cargaInicial();
    _depura ( "END propiedadesempresa::on_mui_guardar_clicked", 0 );
}


///
/**
\return
**/
bool propiedadesempresa::close()
{
    _depura ( "propiedadesempresa::close", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::question ( this,
                                     _( "Guardar cambios" ),
                                     _( "Desea guardar los cambios?" ),
                                     _( "&Guardar" ), _( "&No guardar" ), 0, 0, 1 ) == 0 ) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    _depura ( "END propiedadesempresa::close", 0 );
    return QWidget::close();
}


/// Este SLOT corresponde a la pulsaci&oacute;n del bot&oacute;n de modificar plan contable
/// de la pantalla de configuraci&oacute;n.
/** Coge todas las cuentas del plan contable y trata sus c&oacute;digos de forma que los
    extiende o reduce al tama&ntilde;o especificado en el par&aacute;metro modcodigo.
    De momento esta funci&oacute;n est&aacute; incompleta y hace varias consideraciones
    que deber&iacute;an ir solucion&aacute;ndose (no trata los d&iacute;gitos de cuenta
    y subcuenta) por lo que siempre considera que las cuentas son 4 d&iacute;gitos. */
/**
**/
void propiedadesempresa::on_mui_modificarplan_clicked()
{
    _depura ( "propiedadesempresa::on_mui_modificarplan_clicked", 0 );
    unsigned int nlong = modcodigo->text().length();
    QString codigo;
    QString query = "SELECT * FROM cuenta";
    cursor2 *cur = empresaBase() ->cargacursor ( query );
    while ( !cur->eof() ) {
        codigo = cur->valor ( "codigo" );
        codigo = ajustacodigo ( codigo, nlong );
        empresaBase() ->begin();
        query = "UPDATE cuenta SET codigo = '" + codigo + "' WHERE idcuenta = " + cur->valor ( "idcuenta" );
        empresaBase() ->ejecuta ( query );
        empresaBase() ->commit();
        cur->siguienteregistro();
    } // end while
    delete cur;
    query = "UPDATE configuracion SET valor = '" + modcodigo->text() + "' WHERE nombre = 'CodCuenta'";
    empresaBase() ->begin();
    empresaBase() ->ejecuta ( query );
    empresaBase() ->commit();
    if ( QMessageBox::warning ( this,
                                _( "Salir del programa" ),
                                _( "Para que los cambios tengan efecto\ndebe salir del programa y volver a entrar.\n\nSalir ahora?" ),
                                _( "&Salir" ), _( "&No salir" ), 0, 0, 1 ) == 0 ) {
        exit ( 1 );
    } // end if
    dialogChanges_cargaInicial();
    _depura ( "END propiedadesempresa::on_mui_modificarplan_clicked", 0 );
}


/// ===================================== SUBFORMULARIO ===============================================


///
/**
**/
ListConfiguracionSubForm::~ListConfiguracionSubForm()
{
    _depura ( "ListConfiguracionSubForm::~ListConfiguracionSubForm", 0 );
    _depura ( "END ListConfiguracionSubForm::~ListConfiguracionSubForm", 0 );
}


///
/**
\param parent
**/
ListConfiguracionSubForm::ListConfiguracionSubForm ( QWidget *parent ) : SubForm2Bc ( parent )
{
    _depura ( "ListConfiguracionSubForm::ListConfiguracionSubForm", 0 );
    setDBTableName ( "configuracion" );
    setDBCampoId ( "nombre" );
    addSHeader ( "nombreorig", DBCampo::DBvarchar, DBCampo::DBDupPrimaryKey | DBCampo::DBNoSave, BlSubFormHeader::DBNoView, "Nombre" );
    addSHeader ( "nombre", DBCampo::DBvarchar, DBCampo::DBNotNull, BlSubFormHeader::DBNoWrite, _( "Nombre" ) );
    addSHeader ( "valor", DBCampo::DBvarchar, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "Valor" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    _depura ( "END ListConfiguracionSubForm::ListConfiguracionSubForm", 0 );
}


///
/**
**/
void ListConfiguracionSubForm::cargar()
{
    _depura ( "ListConfiguracionSubForm::cargar", 0 );
    BlSubForm::cargar ( "SELECT *, nombre AS nombreorig FROM configuracion" );
}

