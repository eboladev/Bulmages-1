/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QMenu>
#include <QMenuBar>

#include "pluginresumcta.h"
#include "correctorwidget.h"
#include "empresa.h"
#include "bulmacont.h"
#include "bdockwidget.h"

ResumCtaWidget *g_res;
QAction *viewCorrector;

///
/**
\param bcont
**/
void entryPoint ( Bulmacont *bcont )
{
    _depura ( "Entrada del plugin ResumCta", 10 );
    Empresa *emp = bcont->empresaactual();
    /// Vamos a probar con un docwindow.
    BDockWidget *doc1 = new BDockWidget ( "Resumen Cuenta", bcont );
    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 500 );
    doc1->resize ( 330, 250 );
    bcont->addDockWidget ( Qt::LeftDockWidgetArea, doc1 );

    doc1->show();

    g_res = new ResumCtaWidget ( emp, doc1 );
    g_res->setEmpresaBase ( emp );
    g_res->dock = doc1;

    doc1->setWidget ( g_res );


    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    viewCorrector = new QAction ( "&Resumen Cta", 0 );
    viewCorrector->setCheckable ( TRUE );
    viewCorrector->setChecked ( TRUE );
    viewCorrector->setStatusTip ( "Muestra/oculta el resumen de cuenta" );
    viewCorrector->setWhatsThis ( "Corrector.\n\nMuestra/oculta el corrector" );
    QObject::connect ( viewCorrector, SIGNAL ( toggled ( bool ) ), doc1, SLOT ( cambiaVisible ( bool ) ) );
    QObject::connect ( doc1, SIGNAL ( cambiaEstadoVisible ( bool ) ), viewCorrector, SLOT ( setChecked ( bool ) ) );

    bcont->mui_MenuVer() ->addSeparator();
    bcont->mui_MenuVer() ->addAction ( viewCorrector );

    QFile file ( confpr->valor ( CONF_DIR_USER ) + "pluginresumcta_.cfn" );
    if ( file.exists () ) {
        doc1->show();
        viewCorrector->setChecked ( TRUE );
    } else {
        doc1->hide();
        viewCorrector->setChecked ( FALSE );
    } // end if

    _depura ( "Iniciado correctamente el plugin ResumCta", 10 );
}

///
/**
\return
**/
int BusquedaCuenta_setidcuenta_post ( BusquedaCuenta * )
{
    g_res->setMensaje ( "<HTML><BODY>No hay cuenta.</BODY></HTML>" );
    return 0;
}


///
/**
\param bcta
\return
**/
int BusquedaCuenta_s_lostFocus_post ( BusquedaCuenta *bcta )
{
    _depura ( "BusquedaCuenta_s_lostFocus_post", 0 );
    QString cod = bcta->codigocuenta();
    QString nom = bcta->nomcuenta();
    QString id  = bcta->idcuenta();
    if ( id == "" ) return 0;
    cursor2 *cur = bcta->empresaBase() ->cargacursor ( "SELECT *, debe-haber AS saldo FROM cuenta WHERE idcuenta = " + id );
    if ( !cur ) return 0;
    if ( ! cur->eof() ) {
        QString mensaje = "<HTML><BODY>";
        mensaje += "<B>Código:</B> " + cod + "<BR>";
        mensaje += "<B>Nombre:</B> " + nom + "<BR>";
        mensaje += "<B>Debe:</B> " + cur->valor ( "debe" ) + "<BR>";
        mensaje += "<B>Haber:</B> " + cur->valor ( "haber" ) + "<BR>";
        mensaje += "<HR>";
        mensaje += "<B>Saldo:</B> " + cur->valor ( "saldo" ) + "<BR>";
        mensaje += "</BODY></HTML>";
        g_res->setMensaje ( mensaje );
    } // end if
    delete cur;
    _depura ( "END BusquedaCuenta_s_lostFocus_post", 0 );
    return 0;
}


///
/**
\param subform
\return
**/
int SubForm2Bc_on_mui_list_cellChanged_post ( SubForm2Bc *subform )
{
    _depura ( "SubForm2Bc_on_mui_list_cellChanged_post", 0 );
    if ( subform->currentRow() < 0 )
        return 0;
    if ( subform->existsHeader ( "codigo" ) ) {
        QString cod = subform->DBvalue ( "codigo", subform->currentRow() );
        QString nom = subform->DBvalue ( "descripcioncuenta", subform->currentRow() );
        QString id = subform->DBvalue ( "idcuenta", subform->currentRow() );

        if ( id == "" ) return 0;
        cursor2 *cur = subform->empresaBase() ->cargacursor ( "SELECT *, debe-haber AS saldo FROM cuenta WHERE idcuenta = " + id );
        if ( !cur ) return 0;
        if ( ! cur->eof() ) {
            QString mensaje = "<HTML><BODY>";
            mensaje += "<B>Código:</B> " + cod + "<BR>";
            mensaje += "<B>Nombre:</B> " + nom + "<BR>";
            mensaje += "<B>Debe:</B> " + cur->valor ( "debe" ) + "<BR>";
            mensaje += "<B>Haber:</B> " + cur->valor ( "haber" ) + "<BR>";
            mensaje += "<HR>";
            mensaje += "<B>Saldo:</B> " + cur->valor ( "saldo" ) + "<BR>";
            mensaje += "</BODY></HTML>";
            g_res->setMensaje ( mensaje );
        } // end if
    } // end if
    return 0;
}


///
/**
\param bcont
**/
int Bulmacont_closeEvent ( Bulmacont *bcont )
{
    Empresa * emp = bcont->empresaactual();
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "pluginresumcta_.cfn" );
    if ( !viewCorrector->isChecked() ) {
        file.remove();
    } else {
        file.open ( QIODevice::WriteOnly );
        file.close();
    } // end if
    return 0;
}
