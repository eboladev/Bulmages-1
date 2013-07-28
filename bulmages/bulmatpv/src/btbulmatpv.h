/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef BTBULMATPV_H
#define BTBULMATPV_H

#include <QtWidgets/QStackedWidget>

#include "blfunctions.h"
#include "blworkspace.h"
#include "ui_btbulmatpvbase.h"
#include "btcompany.h"
#include "blwindowlistdock.h"
#include "blsplashscreen.h"
#include "blmainwindow.h"


/** Esta es la clase principal del programa ya que es la que deriva de QMainWindow.
    Su funcionalidad es servir de base como aplicacion Qt y inicializar los componentes clave.
    Tambien sirve de soporte al dise&ntilde;o especificado en bfbulmafactbase.ui con sus menus
    y ToolBoxes.
    Hace todo el traspaso de mensajes de los menus a company y captura algunos que no son
    propiamente de la facturacion como pueda ser el FullScreen o el About.
*/
class BT_EXPORT BtBulmaTPV: public BlMainWindow, public Ui_BtBulmaTPVBase
{
    Q_OBJECT

private:
    /// El workSpace que se va a usar con la aplicacion.
    BlWorkspace *pWorkspace;
    /// La clase corazon de la aplicacion y centralizadora de mensajes y componentes.
    BtCompany *m_company;
    QStackedWidget *m_stackedWidget;

public:
    BtBulmaTPV ( QString bd );
    ~BtBulmaTPV();
    BlWorkspace *workspace();
    void createMainWindows ( BlSplashScreen * );
    BtCompany *company();
    virtual void keyReleaseEvent ( QKeyEvent *e );

public slots:
    virtual void closeEvent ( QCloseEvent * );
    virtual void s_ventanaCompleta();
    virtual void s_About();
    virtual void setCentralBtWidget(QWidget *wid);
    virtual QWidget *centralBtWidget();
    virtual void on_actionVentana_Completa_triggered();
};

#endif

