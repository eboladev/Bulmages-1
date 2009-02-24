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

#include "blfunctions.h"
#include "propiedadesempresa.h"
#include "bccompany.h"


///
/**
\param emp
\param parent
**/
propiedadesempresa::propiedadesempresa ( BcCompany *emp, QWidget *parent )
        : FichaBc ( emp, parent )
{
    _depura ( "propiedadesempresa::propiedadesempresa", 0 );

    setTitleName ( _( "Configuracion" ) );
    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDbTableName ( "configuracion" );

    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    inicializa();

    mui_subform->setMainCompany ( mainCompany() );
    mui_subform->cargar();
    //mui_subform->setResizeMode(QHeaderView::Stretch);

    mainCompany() ->meteWindow ( windowTitle(), this );
    _depura ( "END propiedadesempresa::propiedadesempresa", 0 );
}


///
/**
**/
propiedadesempresa::~propiedadesempresa()
{
    _depura ( "propiedadesempresa::~propiedadesempresa", 0 );
    mainCompany() ->sacaWindow ( this );
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
    BlDbRecordSet *curs = mainCompany() ->loadQuery ( query );
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
    mainCompany() ->begin();
    mui_subform->guardar();

    /// Procesamos la transaccion.
    mainCompany() ->commit();
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
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    while ( !cur->eof() ) {
        codigo = cur->valor ( "codigo" );
        codigo = ajustacodigo ( codigo, nlong );
        mainCompany() ->begin();
        query = "UPDATE cuenta SET codigo = '" + codigo + "' WHERE idcuenta = " + cur->valor ( "idcuenta" );
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        cur->nextRecord();
    } // end while
    delete cur;
    query = "UPDATE configuracion SET valor = '" + modcodigo->text() + "' WHERE nombre = 'CodCuenta'";
    mainCompany() ->begin();
    mainCompany() ->runQuery ( query );
    mainCompany() ->commit();
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
ListConfiguracionSubForm::ListConfiguracionSubForm ( QWidget *parent ) : BcSubForm ( parent )
{
    _depura ( "ListConfiguracionSubForm::ListConfiguracionSubForm", 0 );
    setDbTableName ( "configuracion" );
    setDbFieldId ( "nombre" );
    addSubFormHeader ( "nombreorig", BlDbField::DbVarChar, BlDbField::DbDupPrimaryKey | BlDbField::DbNoSave, BlSubFormHeader::DbNoView, "Nombre" );
    addSubFormHeader ( "nombre", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _( "Nombre" ) );
    addSubFormHeader ( "valor", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Valor" ) );
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

