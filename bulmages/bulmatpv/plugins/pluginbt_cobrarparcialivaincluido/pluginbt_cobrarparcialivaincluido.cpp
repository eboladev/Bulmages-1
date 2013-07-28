/***************************************************************************
 *   Copyright (C) 2011 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *   Modificado 2012 - Tomeu Borras                                        *
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

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>

#include <cstdio>

#include "pluginbt_cobrarparcialivaincluido.h"
#include "btcompany.h"
#include "blfunctions.h"
#include "cobrarparcialview.h"


PluginBt_CobrarParcialIVAIncluido *g_PluginBt_CobrarParcialIVAIncluido;

///
/**
**/
PluginBt_CobrarParcialIVAIncluido::PluginBt_CobrarParcialIVAIncluido()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
PluginBt_CobrarParcialIVAIncluido::~PluginBt_CobrarParcialIVAIncluido()
{
    BL_FUNC_DEBUG
}


///
/**
**/
void PluginBt_CobrarParcialIVAIncluido::inicializa( BtBulmaTPV *btpv)
{
    BL_FUNC_DEBUG
    setMainCompany( btpv->company() );
}


///
/**
\param bges
**/
void PluginBt_CobrarParcialIVAIncluido::cobrarParcial ()
{
    BL_FUNC_DEBUG
    QDialog *diag = new QDialog();
    diag->setWindowTitle(_("Cobrar parcial"));
    CobrarParcialView *cobra = new CobrarParcialView((BtCompany *) mainCompany(), diag);
    
    diag->setModal(true);
    diag->exec();
}


///
/**
\param bges
**/
int entryPoint ( BtBulmaTPV *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_cobrarparcialivaincluido", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    g_PluginBt_CobrarParcialIVAIncluido = new PluginBt_CobrarParcialIVAIncluido();
    g_PluginBt_CobrarParcialIVAIncluido->inicializa(bges);
    return 0;
}



int Cambio_ivainc_pre ( Cambio *cambio )
{
    BL_FUNC_DEBUG
    cambio->setMinimumWidth(cambio->geometry().width() + 100);

    cambio->mui_pluginFrame->setMinimumWidth(110);

    QPushButton *boton = new QPushButton( cambio->mui_pluginFrame );
    boton->setObjectName("mui_botoncobrarparcial");
    boton->setText( _("Cobro parcial") );
    boton->setMinimumSize(100, 100);

    QObject::connect(boton, SIGNAL(clicked()), cambio, SLOT(on_mui_cancelar_clicked()));
    QObject::connect(boton, SIGNAL(clicked()), g_PluginBt_CobrarParcialIVAIncluido, SLOT(cobrarParcial()));

    return 0;
}

