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
/*
Estas dos clases proporcionan la funcionalidad de la lista de ventanas abiertas.
Es un dockwidget que integra un QListWidget donde cada ventana abierta inserta un elemento.
*/
#include <typeinfo>

#include <QtCore/QObject>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtWidgets/QVBoxLayout>

#include <map>

#include "blwindowlistdock.h"
#include "local_blI18n.h"
#include "blwidget.h"

///
/**
\param parent
**/
BlListWidget::BlListWidget ( QWidget * parent ) : QListWidget ( parent )
{
    BL_FUNC_DEBUG
    setItemDelegate(new ListDelegate(this));
    setContextMenuPolicy(Qt::CustomContextMenu);   
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
BlListWidgetItem::BlListWidgetItem ( BlListWidget *l, QPixmap &p, const QString &title, const QString &desc ) : QListWidgetItem ( l )
{
    BL_FUNC_DEBUG
    
    setIcon ( QIcon ( p ) );
    setData(Qt::DisplayRole, title);
    setData(Qt::UserRole + 1, desc);
    setData(Qt::DecorationRole, p);

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
    m_delete = ((QWidget *)m)->testAttribute(Qt::WA_DeleteOnClose);
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
    setToolTip(titulo);
    
}


///
/**
\param m
**/
void BlListWidgetItem::setDesc (const QString &desc )
{
    BL_FUNC_DEBUG
    setData(Qt::UserRole + 1, desc); 
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
        if ( widget->isMaximized() == true ) {
            widget->showNormal();
        } else {
            widget->showMaximized();
        } // end if    
        widget->parentWidget()->activateWindow();

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
	if (widget->parentWidget()) widget->parentWidget() ->raise();
	if (widget->parentWidget()) widget->parentWidget()->activateWindow();

	/// Buscamos la ventana correspondiente y la borramos.
	int i = 0;
	bool maximized = false;
	while ( i < m_listBox->count() ) {
	    BlListWidgetItem * m = ( BlListWidgetItem * ) m_listBox->item ( i );
	    if ( ( ( QWidget * ) m->object() ) ->isMaximized ( ) ) {
	        maximized = true;
	    } // end if
	    i++;
	} // end while
	
	if (maximized) {
	  widget->showMaximized();
	} else {
	  widget->show();
	} // end if
	
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
		listWidgetItem->setDesc( ((BlWidget *)object)->descripcion() );

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
	    BlListWidgetItem *listWidgetItem=NULL;
            QPixmap icon = ( ( QWidget * ) object ) ->windowIcon().pixmap ( 32, 32 );
	    listWidgetItem = new BlListWidgetItem (  NULL, icon, title, ((BlWidget *)object)->descripcion()  );
	    m_listBox->insertItem(m_listBox->currentRow()+1, listWidgetItem);
            listWidgetItem->setObject ( object );
            listWidgetItem->setName ( name );
	    //listWidgetItem->setTitle ( title);
	    //listWidgetItem->setDesc( ( );
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

  int BlWindowListDock::selectWindow ( QString nombre, QMdiSubWindow *obj )
{
    BL_FUNC_DEBUG
    try {
        int i = 0;
        while ( i < m_listBox->count() ) {
            BlListWidgetItem * m = ( BlListWidgetItem * ) m_listBox->item ( i );
            /// Encuentra la ventana en la lista.
	    if ( m->object() == obj->widget() ) {
                m_listBox->setCurrentItem ( m );
                return 0;
            } // end if
            i++;
        } // end while
    } catch ( ... ) {
        BlDebug::blDebug ( "BlWindowListDock::selectWindow", 2, "Error en la Seleccion" );
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
    if ( visible == true ) {
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
    emit ( visibilityStateChanged ( false ) );
    
}

// ============================================


	ListDelegate::ListDelegate(QObject *parent)
{
 
}
 
void ListDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
      
    BlListWidgetItem * item = (BlListWidgetItem *) index.internalPointer();
    QRect r = option.rect;
    
    //Color: #C4C4C4
    QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);
    
    //Color: #005A83
    QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);
    
    //Color: #333
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);
    
    //Color: #fff
    QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);
    
    if(option.state & QStyle::State_Selected){
	QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
	gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
	gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
	gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
	painter->setBrush(gradientSelected);
	painter->drawRect(r);
	
	//BORDER
	painter->setPen(lineMarkedPen);
	painter->drawLine(r.topLeft(),r.topRight());
	painter->drawLine(r.topRight(),r.bottomRight());
	painter->drawLine(r.bottomLeft(),r.bottomRight());
	painter->drawLine(r.topLeft(),r.bottomLeft());
	
	painter->setPen(fontMarkedPen);
    
    } else {
	//BACKGROUND
	//ALTERNATING COLORS
	// painter->setBrush( (index.row() % 2) ? Qt::white : QColor(252,252,252) );
	// painter->drawRect(r);


	if (item->m_delete) {
	    painter->setBrush(  Qt::white  );
	    painter->drawRect(r);
	} else {
	    painter->setBrush(  QColor(222,222,222) );
	    painter->drawRect(r);
	} // end if

	
	//BORDER
	painter->setPen(linePen);
	painter->drawLine(r.topLeft(),r.topRight());
	painter->drawLine(r.topRight(),r.bottomRight());
	painter->drawLine(r.bottomLeft(),r.bottomRight());
	painter->drawLine(r.topLeft(),r.bottomLeft());
	
	painter->setPen(fontPen);
    }
    
    //GET TITLE, DESCRIPTION AND ICON
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 1).toString();
    
    int imageSpace = 10;
    if (!ic.isNull()) {
    //ICON
    //r = option.rect.adjusted(5, 10, -10, -10);
    if (item->m_delete) {
	r = option.rect.adjusted(5, -5, -5, -15);
    } else {
	r = option.rect.adjusted(5, 0, -5, -10);
    } // end if

    ic.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
    imageSpace = 50;
}
 
//TITLE
//r = option.rect.adjusted(imageSpace, 0, -10, -30);

if (item->m_delete) {
    r = option.rect.adjusted(imageSpace, 0, -5, -40);
    painter->setFont( QFont( "Lucida Grande", 11, QFont::Bold ) );
} else {
    r = option.rect.adjusted(imageSpace, 10, -5, -10);
    painter->setFont( QFont( "Lucida Grande", 12, QFont::Bold ) );
} // end if

painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, title, &r);
 
//DESCRIPTION
//r = option.rect.adjusted(imageSpace, 30, -10, 0);
r = option.rect.adjusted(imageSpace, 20, -5, 0);
painter->setFont( QFont( "Lucida Grande", 9, QFont::Normal ) );
painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);
}
 
QSize ListDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
BlListWidgetItem * item = (BlListWidgetItem *) index.internalPointer();
if (item->m_delete) {
    return QSize(200, 60); // very dumb value
} else {
    return QSize(200, 40); // very dumb value
} // end if

}
 
ListDelegate::~ListDelegate()
{
 
}
