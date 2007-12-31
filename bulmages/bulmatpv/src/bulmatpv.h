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

#ifndef BULMATPV_H
#define BULMATPV_H

#include <QMainWindow>
#include <QWorkspace>

#include "qworkspace2.h"
#include "ui_bulmatpvbase.h"
#include "empresatpv.h"
#include "listventanas.h"
#include "funcaux.h"
#include "splashscreen.h"


/** Esta es la clase principal del programa ya que es la que deriva de QMainWindow.
    Su funcionalidad es servir de base como aplicacion Qt y inicializar los componentes clave.
    Tambien sirve de soporte al dise&ntilde;o especificado en bulmafactbase.ui con sus menus
    y ToolBoxes.
    Hace todo el traspaso de mensajes de los menus a company y captura algunos que no son
    propiamente de la facturacion como pueda ser el FullScreen o el About.
*/
class BulmaTPV: public QMainWindow, public Ui_BulmaTPVBase
{
    Q_OBJECT

private:
    /// El workSpace que se va a usar con la aplicacion.
    QWorkspace2 *pWorkspace;
    /// La clase corazon de la aplicacion y centralizadora de mensajes y componentes.
    EmpresaTPV *m_empresaTPV;

public:
    BulmaTPV ( QString bd );
    ~BulmaTPV();
    QWorkspace2 *workspace();
    void createMainWindows ( Splash * );
    EmpresaTPV *empresaTPV();
    virtual void keyPressEvent ( QKeyEvent *e );

public slots:
    virtual void closeEvent ( QCloseEvent * );
    virtual void s_ventanaCompleta();
    virtual void s_About();

};

#endif

