/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QObject>

#include "qapplication2.h"
#include "bulmafact.h"
#include "company.h"
#include "facturasplist.h"
#include "blwidget.h"

extern "C" MY_EXPORT void entryPoint ( Bulmafact * );
extern "C" MY_EXPORT int FacturaView_FacturaView ( FacturaView * );
extern "C" MY_EXPORT int FacturasProveedorList_FacturasProveedorList_Post ( FacturasProveedorList * );
extern QApplication2 *theApp;


class efacturabf : public QObject, PEmpresaBase
{
    Q_OBJECT

public:
    efacturabf();
    ~efacturabf();
    void inicializa ( Bulmafact * );
    int FacturaView_FacturaView ( FacturaView * );
    int FacturasProveedorList_FacturasProveedorList_Post ( FacturasProveedorList * );

public slots:
    void elslot();
    void elslot1();
};
