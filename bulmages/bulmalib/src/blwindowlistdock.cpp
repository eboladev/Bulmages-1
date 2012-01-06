/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <typeinfo>

#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QVBoxLayout>

#include <map>

#include "blwindowlistdock.h"
#include "local_blI18n.h"

///
/**
\param parent
**/
BlListWidget::BlListWidget ( QWidget * parent ) : QListWidget ( parent )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
BlListWidget::~BlListWidget()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param l
\param p
**/
BlListWidgetItem::BlListWidgetItem ( BlListWidget *l, QPixmap &p ) : QListWidgetItem ( l )
{
    BL_FUNC_DEBUG
    setIcon ( QIcon ( p ) );
    m_list = l;
    
}


///
/**
**/
BlListWidgetItem::~BlListWidgetItem()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param m
**/
void BlListWidgetItem::setObject ( QObject *m )
{
    BL_FUNC_DEBUG
    m_obj = m;
    
}


///
/**
\param m
**/
void BlListWidgetItem::setName ( QString m )
{
    BL_FUNC_DEBUG
    m_nombre = m;    
}


///
/**
\param m
**/
void BlListWidgetItem::setTitle ( QString titulo )
{
    BL_FUNC_DEBUG
    m_titulo = titulo;
    setText ( titulo );
    
}

///
/**
\return
**/
QObject *BlListWidgetItem::object()
{
    BL_FUNC_DEBUG
    return m_obj;
}


///
/**
\return
**/
QString BlListWidgetItem::name()
{
    BL_FUNC_DEBUG
    
    return m_nombre;
}


///
/**
\param w
**/
void BlWindowListDock::setWorkspace ( BlWorkspace *w )
{
    BL_FUNC_DEBUG
    m_pWorkspace = w;
    
}


///
/**
\param a
**/
BlWindowListDock::BlWindowListDock ( QWidget *a ) : QDockWidget ( a )
{
    BL_FUNC_DEBUG
    m_listBox = new BlListWidget ( this );
    m_listBox->setObjectName("indexador");
    m_listBox->setIconSize ( QSize ( 32, 32 ) );
    m_listBox->setContentsMargins ( 0, 0, 0, 0 );
    m_listBox->setSpacing ( 0 );
    m_listBox->setWindowTitle ( _ ( "Indexador" ) );
    m_listBox->setMinimumSize(260, 220);
    setWindowTitle ( _ ( "Indexador" ) );
    setFocusPolicy ( Qt::StrongFocus );
    setWidget ( m_listBox );
    setObjectName ( "Indexador" );
    connect ( m_listBox, SIGNAL ( itemDoubleClicked ( QListWidgetItem * ) ), this, SLOT ( dclicked() ) );
    connect ( m_listBox, SIGNAL ( itemClicked ( QListWidgetItem * ) ), this, SLOT ( clicked() ) );
    
}


///
/**
**/
void BlWindowListDock::dclicked()
{
    BL_FUNC_DEBUG
    QWidget *widget = ( QWidget * ) ( ( BlListWidgetItem * ) m_listBox->currentItem() ) ->object();
    if ( widget != NULL ) {
        if ( widget->isMaximized() == TRUE ) {
            widget->showNormal();
        } else {
            widget->showMaximized();
        } // end if
#ifdef AREA_QMDI        
        widget->parentWidget()->activateWindow();
#endif
    } // end if
    
}


///
/**
**/
void BlWindowListDock::clicked()
{
    BL_FUNC_DEBUG
    QWidget *widget = ( QWidget * ) ( ( BlListWidgetItem * ) m_listBox->currentItem() ) ->object();
    if ( widget != NULL ) {
        widget->show();
	if (widget->parentWidget()) widget->parentWidget() ->raise();

#ifdef AREA_QMDI
	if (widget->parentWidget()) widget->parentWidget()->activateWindow();
#else
        m_pWorkspace->setActiveWindow ( widget );
#endif
     
    } // end if
    
}


///
/**
**/
BlWindowListDock::~BlWindowListDock()
{
    BL_FUNC_DEBUG
    delete m_listBox;
    
}


///
/**
**/
void BlWindowListDock::vaciar()
{
    BL_FUNC_DEBUG
    /// Buscamos la ventana correspondiente y la borramos.
    int i = 0;
    while ( i < m_listBox->count() ) {
        BlListWidgetItem * m = ( BlListWidgetItem * ) m_listBox->item ( i );
        if ( ( ( QWidget * ) m->object() ) ->testAttribute ( Qt::WA_DeleteOnClose ) ) {
            delete m->object();
        } else {
            i++;
        } // end if
    } // end while
    
}


///
/**
\return
**/
int BlWindowListDock::numVentanas()
{
    BL_FUNC_DEBUG
    
    return m_listBox->count();
}


///
/**
\param index
\return
**/
QObject *BlWindowListDock::ventana ( int index )
{
    BL_FUNC_DEBUG
    BlListWidgetItem *m = ( BlListWidgetItem * ) m_listBox->item ( index );
    
    return m->object();
}


///
/**
**/
void BlWindowListDock::vaciarCompleto()
{
    BL_FUNC_DEBUG
    /// Buscamos la ventana correspondiente y la borramos.
    int i = 0;
    while ( i < m_listBox->count() ) {
        BlListWidgetItem * m = ( BlListWidgetItem * ) m_listBox->item ( i );
        delete m->object();
    } // end while
    
}


/// This function provides the ability of push a window in the dock window
/// name This QString is the name of the window that was shown in the listbox
/// object This QObject * contains the pointer of the window for furtner reference.
/**
\param name
\param object
\param checkDuplication
\return
**/
int BlWindowListDock::insertWindow ( QString name, QObject *object, bool checkDuplication, QString title )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlWindowListDock::insertWindow", 0, name );
    try {
	/// Comprobamos que haya un titulo y de no ser el caso se pone el nombre.
	if (title == "") 
	  title = name;

        int i = 0;

        while ( i < m_listBox->count() ) {
            BlListWidgetItem * listWidgetItem = ( BlListWidgetItem * ) m_listBox->item ( i );
            /// Si la ventana ya esta en la lista.
            if ( listWidgetItem->object() == object ) {
                listWidgetItem->setName ( name );
		listWidgetItem->setTitle ( title );
                return 0;
            } // end if

            /// Comprobamos ventanas duplicadas.
            if ( listWidgetItem->name() == name && checkDuplication ) {
                ( ( QWidget * ) listWidgetItem->object() ) ->hide();
                ( ( QWidget * ) listWidgetItem->object() ) ->show();
                name = name + "(i)";
            } // end if
            i++;
        } // end while

        if ( i >= m_listBox->count() ) {
            QPixmap icon = ( ( QWidget * ) object ) ->windowIcon().pixmap ( 32, 32 );
            BlListWidgetItem *listWidgetItem = new BlListWidgetItem ( m_listBox, icon );
            listWidgetItem->setObject ( object );
            listWidgetItem->setName ( name );
	    listWidgetItem->setTitle ( title );
        } // end if

    } catch ( ... ) {
        return - 1;
    } // end try

    return 0;
}


/// Sirve para seleccionar una ventana listada en el Indexador.
/**
\param nombre
\param obj
\return
**/
#ifdef AREA_QMDI
  int BlWindowListDock::selectWindow ( QString nombre, QMdiSubWindow *obj )
#else
  int BlWindowListDock::selectWindow ( QString nombre, QObject *obj )
#endif
{
    BL_FUNC_DEBUG
    try {
        int i = 0;
        while ( i < m_listBox->count() ) {
            BlListWidgetItem * m = ( BlListWidgetItem * ) m_listBox->item ( i );
            /// Encuentra la ventana en la lista.
#ifdef AREA_QMDI
	    if ( m->object() == obj->widget() ) {
#else
	    if ( m->object() == obj ) {
#endif
                m_listBox->setCurrentItem ( m );
		
                return 0;
            } // end if
            i++;
        } // end while
    } catch ( ... ) {
        throw - 1;
    } // end try
    
    return 0;
}


/// Deselecciona todas las entradas del Indexador.
/**
\return
**/
int BlWindowListDock::deselectWindow()
{
    BL_FUNC_DEBUG
    try {
        m_listBox->clearSelection();
    } catch ( ... ) {
        BlDebug::blDebug ( "BlWindowListDock::deselectWindow", 2, "Error en la Seleccion" );
        throw - 1;
    } // end try
    
    return 0;
}


///
/**
\param obj
**/
void BlWindowListDock::removeWindow ( QObject *obj )
{
    BL_FUNC_DEBUG
    /// Buscamos la entrada correspondiente dentro del Indexador y la borramos.
    int i = 0;
    while ( i < m_listBox->count() ) {
        BlListWidgetItem * m = ( BlListWidgetItem * ) m_listBox->item ( i );
        if ( m->object() == obj ) {
            BlDebug::blDebug ( "Ventana encontrada y vamos a sacarla", 0, m->name() );
            m_listBox->takeItem ( i );
            delete m;
            break;
        } // end if
        i++;
    } // end while
    /// Deseleccionamos cualquier elemento del listado para que no se quede marcado si
    /// ninguna otra ventana recoge el foco.
    deselectWindow();
}


///
/**
\param visible
**/
void BlWindowListDock::setVisibilityState ( bool visible )
{
    BL_FUNC_DEBUG
    if ( visible == TRUE ) {
        this->show();
    } else {
        this->hide();
    } // end if
    
}


///
/**
**/
void BlWindowListDock::closeEvent ( QCloseEvent * )
{
    BL_FUNC_DEBUG
    emit ( visibilityStateChanged ( FALSE ) );
    
}

