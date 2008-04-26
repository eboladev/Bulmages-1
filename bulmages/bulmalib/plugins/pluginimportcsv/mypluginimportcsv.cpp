/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
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

#include "funcaux.h"
#include "mypluginimportcsv.h"
#include "importcsv.h"
#include "empresabase.h"


extern QMainWindow *g_bges_importcsv;
extern EmpresaBase *g_emp_importcsv;



///
/**
**/
mypluginimportcsv::mypluginimportcsv()
{
    _depura ( "mypluginimportcsv::mypluginimportcsv", 0 );
    _depura ( "END mypluginimportcsv::mypluginimportcsv", 0 );
}

///
/**
**/
mypluginimportcsv::~mypluginimportcsv()
{
    _depura ( "mypluginimportcsv::~mypluginimportcsv", 0 );
    _depura ( "END mypluginimportcsv::~mypluginimportcsv", 0 );
}


///
/**
**/
void mypluginimportcsv::elslot()
{
    _depura ( "mypluginimportcsv::elslot", 0 );

    ImportCSV *imp = new ImportCSV ( g_emp_importcsv, 0 );
    g_emp_importcsv->pWorkspace() ->addWindow ( imp );
    imp->show();

    _depura ( "END mypluginimportcsv::elslot", 0 );
}
