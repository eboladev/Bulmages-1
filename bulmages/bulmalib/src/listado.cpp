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
#include "listado.h"
#include "empresabase.h"


///
/**
**/
void Listado::setModoConsulta()
{
    _depura ( "Listado::setModoConsulta", 0 );
    m_modo = SelectMode;
    _depura ( "END Listado::setModoConsulta", 0 );

}


///
/**
**/
void Listado::setModoEdicion()
{
    _depura ( "Listado::setModoEdicion", 0 );
    m_modo = EditMode;
    _depura ( "END Listado::setModoEdicion", 0 );
}


///
/**
\return
**/
bool Listado::modoEdicion()
{
    _depura ( "Listado::modoEdicion", 0 );
    _depura ( "END Listado::modoEdicion", 0 );
    return m_modo == EditMode;
}


///
/**
\return
**/
bool Listado::modoConsulta()
{
    _depura ( "Listado::modoConsulta", 0 );
    _depura ( "END Listado::modoConsulta", 0 );
    return m_modo == SelectMode;
}


///
/**
\param parent
\param f
\param modo
**/
Listado::Listado ( QWidget *parent, Qt::WFlags f, edmode modo ) : BLWidget ( parent, f )
{
    _depura ( "Listado::Listado", 0 );
    m_modo = modo;
    _depura ( "END Listado::Listado", 0 );
}


///
/**
\param emp
\param parent
\param f
\param modo
**/
Listado::Listado ( EmpresaBase *emp, QWidget *parent, Qt::WFlags f, edmode modo ) : BLWidget ( emp, parent, f )
{
    _depura ( "Listado::Listado", 0 );
    m_modo = modo;
    _depura ( "END Listado::Listado", 0 );
}


///
/**
**/
Listado::~Listado()
{
    _depura ( "Listado::~Listado", 0, this->windowTitle() );
    sacaWindow();
    _depura ( "END Listado::~Listado", 0 );
}


///
/**
\return
**/
int Listado::sacaWindow()
{
    _depura ( "Listado::sacaWindow", 0 );
    empresaBase() ->sacaWindow ( this );
    _depura ( "END Listado::sacaWindow", 0 );
    return 0;
}


///
/**
\param title
\return
**/
int Listado::meteWindow ( QString title )
{
    _depura ( "Listado::meteWindow", 0 );
    empresaBase() ->meteWindow ( title, this );
    _depura ( "END Listado::meteWindow", 0 );
    return 0;
}


/** SLOT que responde al doble click sobre un elemento del listado
    llama a editArticle para que actue correspondientemente.
*/
/**
\param a
**/
void Listado::on_mui_list_cellDoubleClicked ( int a, int )
{
    _depura ( "ArticuloList::on_mui_list_cellDoubleClicked", 0 );
    editar ( a );
    _depura ( "END ArticuloList::on_mui_list_cellDoubleClicked", 0 );
}


///
/**
**/
void Listado::editar ( int )
{
    _depura ( "metodo para ser reimplementado en clases hijas", 2 );
}


///
/**
**/
void Listado::crear()
{
    _depura ( "metodo para ser reimplementado en clases hijas", 2 );
}


///
/**
**/
void Listado::borrar()
{
    _depura ( "metodo para ser reimplementado en clases hijas", 2 );
}


///
/**
**/
void Listado::imprimir()
{
    _depura ( "Listado::imprimir", 0 );
    m_listado->imprimirPDF ( "" );
    _depura ( "END Listado::imprimir", 0 );
}

/// Este metodo carga el subformulario con un SELECT generico. Si no quiere usarse este select
/// Debe reimplementar el metodo en las clases derivadas.
/**
**/
void Listado::presentar()
{
    _depura ( "Listado::presentar", 0 );
    QString query = "SELECT * FROM " + m_listado->tableName();
    m_listado->cargar ( query );
    _depura ( "END Listado::presentar", 0 );
}


///
/**
\param list
**/
void Listado::setSubForm ( SubForm3 *list )
{
    _depura ( "Listado::setSubForm", 0 );
    m_listado = list;
    _depura ( "END Listado::setSubForm", 0 );
}


///
/**
\param text
**/
void Listado::on_m_filtro_textChanged ( const QString &text )
{
    _depura ( "Listado::on_m_filtro_textChanged", 0 );

    if ( text.size() >= 3 ) {
        on_mui_actualizar_clicked();
    } // end if
    _depura ( "END Listado::on_m_filtro_textChanged", 0 );
}


///
/**
**/
void Listado::on_m_filtro_editingFinished()
{
    _depura ( "Listado::on_m_filtro_editFinished", 0 );
    static QString valant = "";
    QLineEdit *line = findChild<QLineEdit *> ( "m_filtro" );
    if ( line->text() != valant ) {
        valant = line->text();
        on_mui_actualizar_clicked();
    } // end if
    _depura ( "END Listado::on_m_filtro_editFinished", 0 );
}


/** SLOT que responde a la pulsacion de la tecla Enter
    en la caja de texto del buscador del formulario
    Comprueba que solo haya un item en el listado. Si
    es asi, abre ese item.
*/
void Listado::on_m_filtro_returnPressed() {
    _depura ("Listado::on_mui_importar_clicked", 0);
        
    if (m_listado->lista()->count() == 1) {
        editar(0);
    }
        
    _depura("END Listado::on_mui_importar_clicked", 0);
}


///
/**
**/
void Listado::on_mui_actualizar_clicked()
{
    _depura ( "Listado::on_mui_actualizar_clicked", 0 );
    presentar();
    _depura ( "END Listado::on_mui_actualizar_clicked", 0 );
}


///
/**
**/
void Listado::on_mui_imprimir_clicked()
{
    _depura ( "Listado::on_mui_imprimir_clicked", 0 );
    imprimir();
    _depura ( "END Listado::on_mui_imprimir_clicked", 0 );
}


///
/**
**/
void Listado::on_mui_crear_clicked()
{
    _depura ( "Listado::on_mui_crear_clicked", 0 );
    crear();
    _depura ( "END Listado::on_mui_crear_clicked", 0 );
}


///
/**
**/
void Listado::on_mui_borrar_clicked()
{
    _depura ( "Listado::on_mui_borrar_clicked", 0 );
    borrar();
    _depura ( "END Listado::on_mui_borrar_clicked", 0 );
}


/** SLOT que responde al boton de editar articulo
    Comprueba que haya un elemento seleccionado y llama a editArticle.
*/
/**
\return
**/
void Listado::on_mui_editar_clicked()
{
    _depura ( "ArticuloList::INIT_s_editArticle", 0 );
    int a = m_listado->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( tr ( "Tiene que seleccionar un elemento" ) );
        return;
    } // end if
    editar ( a );
    _depura ( "ArticuloList::END_s_editArticle", 0 );
}


///
/**
\param checked
**/
void Listado::on_mui_configurar_toggled ( bool checked )
{
    _depura ( "Listado::on_mui_configurar_toggled", 0 );
    if ( checked ) {
        m_listado->showConfig();
    } else {
        m_listado->hideConfig();
    } // end if
    _depura ( "END Listado::on_mui_configurar_toggled", 0 );
}


/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
/**
\param p
**/
void Listado::on_mui_list_customContextMenuRequested ( const QPoint &p )
{
    _depura ( "Listado::on_mui_list_customContextMenuRequested", 0 );
    submenu ( p );
    _depura ( "END Listado::on_mui_list_customContextMenuRequested", 0 );
}


/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
/**
\return
**/
void Listado::submenu ( const QPoint & )
{
    _depura ( "ArticuloList::on_mui_list_customContextMenuRequested", 0 );
    int a = m_listado->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( tr ( "Editar" ) );
    QAction *del = popup->addAction ( tr ( "Borrar" ) );
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
void Listado::on_mui_list_toogledConfig ( bool check )
{
    _depura ( "Listado::on_mui_list_toogledConfig", 0 );

    QToolButton *botonconfigurar = findChild<QToolButton *> ( "mui_configurar" );
    if ( botonconfigurar )
        botonconfigurar->setChecked ( check );
    _depura ( "END Listado::on_mui_list_toogledConfig", 0 );
}


///
/**
**/
void Listado::hideBotonera()
{
    _depura ( "Listado::hideBotonera", 0 );
    QWidget *botonera = findChild<QWidget *> ( "m_botonera" );
    if ( botonera )
        botonera->hide();
    _depura ( "END Listado::hideBotonera", 0 );
}


///
/**
**/
void Listado::showBotonera()
{
    _depura ( "Listado::showBotonera", 0 );
    QWidget *botonera = findChild<QWidget *> ( "m_botonera" );
    if ( botonera )
        botonera->show();
    _depura ( "END Listado::showBotonera", 0 );
}


///
/**
**/
void Listado::hideBusqueda()
{
    _depura ( "Listado::hideBusqueda", 0 );
    QWidget *busqueda = findChild<QWidget *> ( "m_busqueda" );
    if ( busqueda )
        busqueda->hide();
    _depura ( "END Listado::hideBusqueda", 0 );
}


///
/**
**/
void Listado::showBusqueda()
{
    _depura ( "Listado::showBusqueda", 0 );
    QWidget *busqueda = findChild<QWidget *> ( "m_busqueda" );
    if ( busqueda )
        busqueda->show();
    _depura ( "END Listado::showBusqueda", 0 );
}

///
/**
\param nomtabla
**/
void Listado::trataPermisos ( QString nomtabla )
{
    _depura ( "Listado::trataPermisos", 0 );

    if ( !empresaBase() ->has_table_privilege ( nomtabla, "INSERT" ) ) {
        /// Buscamos los permisos que tiene el usuario y desactivamos botones.
        QToolButton * b = findChild<QToolButton *> ( "mui_crear" );
        if ( b ) b->setDisabled ( TRUE );
        QToolButton *c = findChild<QToolButton *> ( "mui_importar" );
        if ( c ) c->setDisabled ( TRUE );
        QToolButton *d = findChild<QToolButton *> ( "mui_exportar" );
        if ( d ) d->setDisabled ( TRUE );
    } // end if



    if ( !empresaBase() ->has_table_privilege ( nomtabla, "UPDATE" ) ) {
        /// Buscamos los permisos que tiene el usuario y desactivamos botones.
        QToolButton * b = findChild<QToolButton *> ( "mui_editar" );
        if ( b ) b->setDisabled ( TRUE );
        QToolButton *c = findChild<QToolButton *> ( "mui_borrar" );
        if ( c ) c->setDisabled ( TRUE );
        QToolButton *e = findChild<QToolButton *> ( "mui_importar" );
        if ( e ) e->setDisabled ( TRUE );
        QToolButton *d = findChild<QToolButton *> ( "mui_exportar" );
        if ( d ) d->setDisabled ( TRUE );
    } // end if


    _depura ( "END Listado::trataPermisos", 0 );
}

