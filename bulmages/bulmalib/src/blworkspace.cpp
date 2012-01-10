/***************************************************************************
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
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

#include <QWidget>
#include <QAction>
#include <QList>

#include "blworkspace.h"
#include "local_blI18n.h"


///
/**
**/
BlWorkspace::BlWorkspace ( QWidget * )
{
    BL_FUNC_DEBUG
#ifdef AREA_QMDI
    fprintf(stderr, "AREA_QMDI active\n");
#endif
}


///
/**
**/
BlWorkspace::~BlWorkspace()
{
    BL_FUNC_DEBUG
    
}



#ifdef AREA_QMDI    
///
/**
\param w
**/
void BlWorkspace::addSubWindow ( QWidget * w )
{
    BL_FUNC_DEBUG
    int tamdispW;
    int tamdispH;
    int tamventanadecoW;
    int tamventanadecoH;
    int tamventanaW;
    int tamventanaH;
    int margen = 10;

    QMdiSubWindow *sw = new QMdiSubWindow;
    sw->setWidget(w);
    connect ( w, SIGNAL(destroyed(QObject *)), sw, SLOT(close()));
    connect ( w, SIGNAL(hided(QObject *)), sw, SLOT(hide()));
//    connect ( w, SIGNAL(hided(QObject *)), this, SIGNAL(deselectDockAll()));
    connect ( w, SIGNAL(showed(QObject *)), sw, SLOT(show()));
    
    QMdiArea::addSubWindow (sw);


    /// Se comprueba el tama&ntilde;o de la ventana que esta dise&ntilde;ada con Designer:
    /// S&oacute;lo si la ventana es m&aacute;s grande que el espacio de representaci&oacute;n
    /// se ajusta para caber dentro.

    /// Captura el tama&ntilde;o disponible. Restamos el margen que dejamos a cada lado (x2).
    tamdispW = this->width() - ( margen * 2 );
    tamdispH = this->height() - ( margen * 2 );

    /// Captura el tama&ntilde;o con decoraci&oacute;n de la ventana a insertar.
    tamventanadecoW = w->parentWidget() ->frameGeometry().width();
    tamventanadecoH = w->parentWidget() ->frameGeometry().height();
    /// Captura el tama&ntilde;o sin decoraci&oacute;n de la ventana a insertar.
    tamventanaW = w->geometry().width();
    tamventanaH = w->geometry().height();

    /// Comprobamos si es necesario cambiar el tama&ntilde;o a la ventana.
    if ( tamventanadecoW > tamdispW )
        tamventanaW = tamdispW - ( tamventanadecoW - tamventanaW );
    if ( tamventanadecoH > tamdispH )
        tamventanaH = tamdispH - ( tamventanadecoH - tamventanaH );

    w->setGeometry ( 0, 0, tamventanaW, tamventanaH );
    w->parentWidget() ->move ( margen, margen );

    /// Crea un QAction para manejar la tecla rapida ESC para cerrar la ventana.
    QAction *accionEsc = new QAction ( w );
    accionEsc->setShortcut ( _ ( "Esc" ) );
    connect ( accionEsc, SIGNAL ( triggered() ), w, SLOT ( close() ) );
    w->addAction ( accionEsc );
    
}




QWidget *BlWorkspace::activeWindow () const
{
    return QApplication::activeWindow ();
}

#else

///
/**
\param w
**/
void BlWorkspace::addWindow ( QWidget * w )
{
    BL_FUNC_DEBUG
    int tamdispW;
    int tamdispH;
    int tamventanadecoW;
    int tamventanadecoH;
    int tamventanaW;
    int tamventanaH;
    int margen = 10;

    QWorkspace::addWindow ( w );

    /// Se comprueba el tama&ntilde;o de la ventana que esta dise&ntilde;ada con Designer:
    /// S&oacute;lo si la ventana es m&aacute;s grande que el espacio de representaci&oacute;n
    /// se ajusta para caber dentro.
    /// Crea un QAction para manejar la tecla rapida ESC para cerrar la ventana.
    QAction *accionEsc = new QAction ( w );
    accionEsc->setShortcut ( _ ( "Esc" ) );
    connect ( accionEsc, SIGNAL ( triggered() ), w, SLOT ( close() ) );
    w->addAction ( accionEsc );
    
}


///
/**
\param w
**/
void BlWorkspace::addSubWindow ( QWidget * w )
{
    BL_FUNC_DEBUG
    BlWorkspace::addWindow ( w );
    
}

#endif
