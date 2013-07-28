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

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>

#include "pluginbf_cuentaresumen.h"
#include "correctorwidget.h"
#include "bfcompany.h"
#include "bfbulmafact.h"
#include "bldockwidget.h"
#include "blfile.h"


ResumCtaWidget *g_res;
QAction *viewCorrector;


///
/**
\param bfact
**/
int entryPoint ( BfBulmaFact *bfact )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_cuentaresumen", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    BfCompany *emp = bfact->company();
    /// Vamos a probar con un docwindow.
    BlDockWidget *doc1 = new BlDockWidget ( _ ( "Resumen cuenta" ), bfact );
    doc1->setObjectName("mui_resumenCuenta");
    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 500 );
    doc1->resize ( 330, 250 );
    bfact->addDockWidget ( Qt::LeftDockWidgetArea, doc1 );

    doc1->show();

    g_res = new ResumCtaWidget ( emp, doc1 );
    g_res->setMainCompany ( emp );
    g_res->dock = doc1;

    doc1->setWidget ( g_res );

    QMenu *pPluginMenu = bfact->newMenu( "&Ver", "menuVer", "menuMaestro");


    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    viewCorrector = new QAction ( _ ( "&Resumen Cta" ), 0 );
    viewCorrector->setCheckable ( true );
    viewCorrector->setChecked ( true );
    viewCorrector->setStatusTip ( _ ( "Muestra/oculta el resumen de cuenta" ) );
    viewCorrector->setWhatsThis ( _ ( "Corrector.\n\nMuestra/oculta el corrector" ) );
    QObject::connect ( viewCorrector, SIGNAL ( toggled ( bool ) ), doc1, SLOT ( setVisibilityState ( bool ) ) );
    QObject::connect ( doc1, SIGNAL ( visibilityStateChanged ( bool ) ), viewCorrector, SLOT ( setChecked ( bool ) ) );

    pPluginMenu ->addSeparator();
    pPluginMenu ->addAction ( viewCorrector );

    BlFile file ( g_confpr->value( CONF_DIR_USER ) + "pluginresumcta_.cfn" );
    if ( file.exists () ) {
        doc1->show();
        viewCorrector->setChecked ( true );
    } else {
        doc1->hide();
        viewCorrector->setChecked ( false );
    } // end if

    
    return 0;
}

///
/**
\return
**/
int Busqueda_on_m_inputBusqueda_textChanged ( BlSearchWidget * )
{
    g_res->setMensaje ( "<HTML><BODY>No hay cuenta.</BODY></HTML>" );
    return 0;
}


///
/**
\param bfta
\return
**/

int Busqueda_on_m_inputBusqueda_editingFinished_Post ( BlSearchWidget *bfta )
{
    BL_FUNC_DEBUG
    QString cod = bfta->fieldValue("codigo");
    QString nom = bfta->fieldValue("descripcion");
    QString id  = bfta->id();
    if ( id == "" ) return 0;
    BlDbRecordSet *cur = bfta->mainCompany() ->loadQuery ( "SELECT *, debe-haber AS saldo FROM cuenta WHERE idcuenta = " + id );
    if ( !cur ) return 0;
    if ( ! cur->eof() ) {
        QString mensaje = "<HTML><BODY>";
        mensaje += "<B>Codigo:</B> " + cod + "<BR>";
        mensaje += "<B>Nombre:</B> " + nom + "<BR>";
        mensaje += "<B>Debe:</B> " + cur->value( "debe" ) + "<BR>";
        mensaje += "<B>Haber:</B> " + cur->value( "haber" ) + "<BR>";
        mensaje += "<HR>";
        mensaje += "<B>Saldo:</B> " + cur->value( "saldo" ) + "<BR>";
        mensaje += "</BODY></HTML>";
        g_res->setMensaje ( mensaje );
    } // end if
    delete cur;
    
    return 0;
}


///
/**
\param subform
\return
**/
int BfSubForm_on_mui_list_editFinished ( BfSubForm *subform )
{
    BL_FUNC_DEBUG
    if ( subform->currentRow() < 0 )
        return 0;
    if ( subform->existsHeader ( "codigo" ) ) {
        QString cod = subform->dbValue ( "codigo", subform->currentRow() );
        QString nom = subform->dbValue ( "descripcioncuenta", subform->currentRow() );
        QString id = subform->dbValue ( "idcuenta", subform->currentRow() );

        if ( id == "" ) return 0;
        BlDbRecordSet *cur = subform->mainCompany() ->loadQuery ( "SELECT *, debe-haber AS saldo FROM cuenta WHERE idcuenta = " + id );
        if ( !cur ) return 0;
        if ( ! cur->eof() ) {
            QString mensaje = "<HTML><BODY>";
            mensaje += "<B>Codigo:</B> " + cod + "<BR>";
            mensaje += "<B>Nombre:</B> " + nom + "<BR>";
            mensaje += "<B>Debe:</B> " + cur->value( "debe" ) + "<BR>";
            mensaje += "<B>Haber:</B> " + cur->value( "haber" ) + "<BR>";
            mensaje += "<HR>";
            mensaje += "<B>Saldo:</B> " + cur->value( "saldo" ) + "<BR>";
            mensaje += "</BODY></HTML>";
            g_res->setMensaje ( mensaje );
        } // end if
    } // end if
    return 0;
}


///
/**
\param bfact
**/
int BfBulmaFact_closeEvent ( BfBulmaFact *bfact )  {
    BL_FUNC_DEBUG
    BlFile file ( g_confpr->value( CONF_DIR_USER ) + "pluginresumcta_.cfn" );
    if ( !viewCorrector->isChecked() ) {
        file.remove();
    } else {
        file.open ( QIODevice::WriteOnly );
        file.close();
    } // end if
    return 0;
}
