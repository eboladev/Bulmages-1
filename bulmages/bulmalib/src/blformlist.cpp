/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include <QMenu>

#include "blformlist.h"
#include "blmaincompany.h"


///
/**
**/
void BlFormList::setModoConsulta()
{
    blDebug ( "BlFormList::setModoConsulta", 0 );
    m_modo = BL_SELECT_MODE;
    blDebug ( "END BlFormList::setModoConsulta", 0 );

}


///
/**
**/
void BlFormList::setModoEdicion()
{
    blDebug ( "BlFormList::setModoEdicion", 0 );
    m_modo = BL_EDIT_MODE;
    blDebug ( "END BlFormList::setModoEdicion", 0 );
}


///
/**
\return
**/
bool BlFormList::modoEdicion()
{
    blDebug ( "BlFormList::modoEdicion", 0 );
    blDebug ( "END BlFormList::modoEdicion", 0 );
    return m_modo == BL_EDIT_MODE;
}


///
/**
\return
**/
bool BlFormList::modoConsulta()
{
    blDebug ( "BlFormList::modoConsulta", 0 );
    blDebug ( "END BlFormList::modoConsulta", 0 );
    return m_modo == BL_SELECT_MODE;
}


///
/**
\param parent
\param f
\param modo
**/
BlFormList::BlFormList ( QWidget *parent, Qt::WFlags f, edmode modo ) : BlWidget ( parent, f )
{
    blDebug ( "BlFormList::BlFormList", 0 );
    m_modo = modo;
    m_listado = NULL;
    blDebug ( "END BlFormList::BlFormList", 0 );
}


///
/**
\param emp
\param parent
\param f
\param modo
**/
BlFormList::BlFormList ( BlMainCompany *emp, QWidget *parent, Qt::WFlags f, edmode modo ) : BlWidget ( emp, parent, f )
{
    blDebug ( "BlFormList::BlFormList", 0 );
    m_modo = modo;
    m_listado = NULL;
    blDebug ( "END BlFormList::BlFormList", 0 );
}


///
/**
**/
BlFormList::~BlFormList()
{
    blDebug ( "BlFormList::~BlFormList", 0, this->windowTitle() );
    sacaWindow();
    blDebug ( "END BlFormList::~BlFormList", 0 );
}


///
/**
\return
**/
int BlFormList::sacaWindow()
{
    blDebug ( "BlFormList::sacaWindow", 0 );
    mainCompany() ->sacaWindow ( this );
    blDebug ( "END BlFormList::sacaWindow", 0 );
    return 0;
}


///
/**
\param title
\return
**/
int BlFormList::meteWindow ( QString title )
{
    blDebug ( "BlFormList::meteWindow", 0 );
    mainCompany() ->meteWindow ( title, this );
    blDebug ( "END BlFormList::meteWindow", 0 );
    return 0;
}


/** SLOT que responde al doble click sobre un elemento del listado
    llama a editArticle para que actue correspondientemente.
*/
/**
\param a
**/
void BlFormList::on_mui_list_cellDoubleClicked ( int a, int )
{
    blDebug ( "ArticuloList::on_mui_list_cellDoubleClicked", 0 );
    editar ( a );
    blDebug ( "END ArticuloList::on_mui_list_cellDoubleClicked", 0 );
}


///
/**
**/
void BlFormList::editar ( int )
{
    blDebug ( "metodo para ser reimplementado en clases hijas", 2 );
}


///
/**
**/
void BlFormList::crear()
{
    blDebug ( "metodo para ser reimplementado en clases hijas", 2 );
}


///
/**
**/
void BlFormList::borrar()
{
    blDebug ( "metodo para ser reimplementado en clases hijas", 2 );
}


///
/**
**/
void BlFormList::imprimir()
{
    blDebug ( "BlFormList::imprimir", 0 );
    m_listado->imprimirPDF ( "" );
    blDebug ( "END BlFormList::imprimir", 0 );
}

/// Este metodo carga el subformulario con un SELECT generico. Si no quiere usarse este select
/// Debe reimplementar el metodo en las clases derivadas.
/**
**/
void BlFormList::presentar()
{
    blDebug ( "BlFormList::presentar", 0 );
    QString query = "SELECT * FROM " + m_listado->tableName();
    m_listado->cargar ( query );
    blDebug ( "END BlFormList::presentar", 0 );
}


///
/**
\param list
**/
void BlFormList::setSubForm ( BlSubForm *list )
{
    blDebug ( "BlFormList::setSubForm", 0 );
    m_listado = list;
    /// Establecemos el mismo modo en el subformulario que en el listado.
    if ( m_modo == BL_EDIT_MODE ) {
        m_listado->setModoEdicion();
    } else {
        m_listado->setModoConsulta();
    } // end if
    blDebug ( "END BlFormList::setSubForm", 0 );
}



///
/**
\param list
**/
BlSubForm *BlFormList::subForm ( )
{
    blDebug ( "BlFormList::subForm", 0 );
    blDebug ( "END BlFormList::subForm", 0 );
    return m_listado;
}


///
/**
\param text
**/
void BlFormList::on_m_filtro_textChanged ( const QString &text )
{
    blDebug ( "BlFormList::on_m_filtro_textChanged", 0 );
    if ( text.size() >= 3 ) {
        on_mui_actualizar_clicked();
    } // end if
    blDebug ( "END BlFormList::on_m_filtro_textChanged", 0 );
}


///
/**
**/
void BlFormList::on_m_filtro_editingFinished()
{
    blDebug ( "BlFormList::on_m_filtro_editFinished", 0 );
    static QString valant = "";
    QLineEdit *line = findChild<QLineEdit *> ( "m_filtro" );
    if ( line->text() != valant ) {
        valant = line->text();
        on_mui_actualizar_clicked();
    } // end if
    blDebug ( "END BlFormList::on_m_filtro_editFinished", 0 );
}


/** SLOT que responde a la pulsacion de la tecla Enter
    en la caja de texto del buscador del formulario
    Comprueba que solo haya un item en el listado. Si
    es asi, abre ese item.
*/
void BlFormList::on_m_filtro_returnPressed()
{
    blDebug ( "BlFormList::on_mui_importar_clicked", 0 );
    if ( m_listado->lista()->count() == 1 ) {
        editar ( 0 );
    } // end if
    blDebug ( "END BlFormList::on_mui_importar_clicked", 0 );
}


///
/**
**/
void BlFormList::on_mui_actualizar_clicked()
{
    blDebug ( "BlFormList::on_mui_actualizar_clicked", 0 );
    presentar();
    blDebug ( "END BlFormList::on_mui_actualizar_clicked", 0 );
}


///
/**
**/
void BlFormList::on_mui_imprimir_clicked()
{
    blDebug ( "BlFormList::on_mui_imprimir_clicked", 0 );
    imprimir();
    blDebug ( "END BlFormList::on_mui_imprimir_clicked", 0 );
}


///
/**
**/
void BlFormList::on_mui_crear_clicked()
{
    blDebug ( "BlFormList::on_mui_crear_clicked", 0 );
    crear();
    blDebug ( "END BlFormList::on_mui_crear_clicked", 0 );
}


///
/**
**/
void BlFormList::on_mui_borrar_clicked()
{
    blDebug ( "BlFormList::on_mui_borrar_clicked", 0 );
    borrar();
    blDebug ( "END BlFormList::on_mui_borrar_clicked", 0 );
}


/** SLOT que responde al boton de editar articulo
    Comprueba que haya un elemento seleccionado y llama a editArticle.
*/
/**
\return
**/
void BlFormList::on_mui_editar_clicked()
{
    blDebug ( "ArticuloList::INIT_s_editArticle", 0 );
    int a = m_listado->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Tiene que seleccionar un elemento" ) );
        return;
    } // end if
    editar ( a );
    blDebug ( "ArticuloList::END_s_editArticle", 0 );
}


///
/**
\param checked
**/
void BlFormList::on_mui_configurar_toggled ( bool checked )
{
    blDebug ( "BlFormList::on_mui_configurar_toggled", 0 );
    if ( checked ) {
        m_listado->showConfig();
    } else {
        m_listado->hideConfig();
    } // end if
    blDebug ( "END BlFormList::on_mui_configurar_toggled", 0 );
}


/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
/**
\param p
**/
void BlFormList::on_mui_list_customContextMenuRequested ( const QPoint &p )
{
    blDebug ( "BlFormList::on_mui_list_customContextMenuRequested", 0 );
    submenu ( p );
    blDebug ( "END BlFormList::on_mui_list_customContextMenuRequested", 0 );
}


/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
/**
\return
**/
void BlFormList::submenu ( const QPoint & )
{
    blDebug ( "ArticuloList::on_mui_list_customContextMenuRequested", 0 );
    int a = m_listado->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar" ) );
    QAction *del = popup->addAction ( _ ( "Borrar" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;
}


///
/**
\param check
**/
void BlFormList::on_mui_list_toogledConfig ( bool check )
{
    blDebug ( "BlFormList::on_mui_list_toogledConfig", 0 );

    QToolButton *botonconfigurar = findChild<QToolButton *> ( "mui_configurar" );
    if ( botonconfigurar )
        botonconfigurar->setChecked ( check );
    blDebug ( "END BlFormList::on_mui_list_toogledConfig", 0 );
}


///
/**
**/
void BlFormList::hideBotonera()
{
    blDebug ( "BlFormList::hideBotonera", 0 );
    QWidget *botonera = findChild<QWidget *> ( "m_botonera" );
    if ( botonera )
        botonera->hide();
    blDebug ( "END BlFormList::hideBotonera", 0 );
}


///
/**
**/
void BlFormList::showBotonera()
{
    blDebug ( "BlFormList::showBotonera", 0 );
    QWidget *botonera = findChild<QWidget *> ( "m_botonera" );
    if ( botonera )
        botonera->show();
    blDebug ( "END BlFormList::showBotonera", 0 );
}


///
/**
**/
void BlFormList::hideBusqueda()
{
    blDebug ( "BlFormList::hideBusqueda", 0 );
    QWidget *busqueda = findChild<QWidget *> ( "m_busqueda" );
    if ( busqueda )
        busqueda->hide();
    blDebug ( "END BlFormList::hideBusqueda", 0 );
}


///
/**
**/
void BlFormList::showBusqueda()
{
    blDebug ( "BlFormList::showBusqueda", 0 );
    QWidget *busqueda = findChild<QWidget *> ( "m_busqueda" );
    if ( busqueda )
        busqueda->show();
    blDebug ( "END BlFormList::showBusqueda", 0 );
}

///
/**
\param nomtabla
**/
void BlFormList::trataPermisos ( QString nomtabla )
{
    blDebug ( "BlFormList::trataPermisos", 0 );

    QToolButton * b;

    if ( !mainCompany() ->hasTablePrivilege ( nomtabla, "INSERT" ) ) {
        /// Buscamos los permisos que tiene el usuario y desactivamos botones.
	  b = findChild<QToolButton *> ( "mui_crear" );
	  if ( b ) b->setDisabled ( TRUE );
	  b = findChild<QToolButton *> ( "mui_importar" );
	  if ( b ) b->setDisabled ( TRUE );
	  b = findChild<QToolButton *> ( "mui_exportar" );
	  if ( b ) b->setDisabled ( TRUE );
    } // end if


    if ( !mainCompany() ->hasTablePrivilege ( nomtabla, "DELETE" ) ) {
	  /// Buscamos los permisos que tiene el usuario y desactivamos botones.
	  b = findChild<QToolButton *> ( "mui_borrar" );
	  if ( b ) b->setDisabled ( TRUE );
    } // end if

    blDebug ( "END BlFormList::trataPermisos", 0 );
}

