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

#include "bcasientoview.h"
#include "pluginbc_asiento.h"
#include "correctorwidget.h"


/** Inicializacion de la clase.
    En el constructor del corrector no se hacen tareas especiales. */
/**
\param emp
\param parent
\param fl
**/
ResumCtaWidget::ResumCtaWidget ( BcCompany *emp, QWidget* parent, Qt::WFlags fl )
        : QWidget ( parent, fl ), BlMainCompanyPointer ( emp )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setupUi ( this );
    QObject::connect ( mui_browser, SIGNAL ( anchorClicked ( const QUrl ) ), this, SLOT ( alink ( const QUrl ) ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
ResumCtaWidget::~ResumCtaWidget()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}



/** Detecta la pulsacion de un enlace en el widget de resultados.
    Se activa esta funcion cuando se ha pulsado un link en el Widget de resultados
    QTextBrowser.
    De esta forma la aplicacion puede interactuar con la pagina web que se muestra. */
/**
\param url
**/
void ResumCtaWidget::alink ( const QUrl &url )
{
    blDebug ( Q_FUNC_INFO, 0 );
    QString linker = url.fragment();

    ///TODO: REVISAR ESTA FUNCION QUE NO HACE BIEN SU TRABAJO.
    if ( linker == "ver" ) {
        ( ( BcCompany * ) mainCompany() ) ->muestracuentas();
    } else if ( linker == "asiento" ) {
        //QString ordenasiento = l.right(l.length() - 2);
        //( ( BcCompany * ) mainCompany() ) ->intapuntsempresa();
    } else {
        BcAsientoView *view = g_asiento;
        bool ok;
        view->muestraasiento ( linker.toInt ( &ok ) );
    } // endif
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param a
**/
void ResumCtaWidget::cambia ( bool a )
{
    blDebug ( Q_FUNC_INFO, 0 );
    if ( a ) {
        dock->hide();
        dock->show();
        dock->showMaximized();
    } else {
        dock->hide();
    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param mensaje
**/
void ResumCtaWidget::setMensaje ( QString mensaje )
{
    blDebug ( Q_FUNC_INFO, 0 );
    mui_browser->setHtml ( mensaje );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );

}
