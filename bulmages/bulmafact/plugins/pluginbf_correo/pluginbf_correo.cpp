/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *   Copyright (C) 2012 by Daniel Ripoll Osma.                             *
 *   info@danielripoll.es                                                  *
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
#include <QToolButton>

#include "pluginbf_correo.h"
#include "blfunctions.h"
#include "facturaview.h"
#include "presupuestoview.h"
#include "impqtoolbutton.h"
#include "blplugins.h"
#include "chosemailer.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_correo", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    
    
    QString conf_email = g_confpr->value( CONF_EMAIL_CLIENT );
    if (  conf_email == "") {
        LaunchChoseMailer();
        }

    return 0;
}


///
/**
\param l
\return
**/
int PresupuestoView_PresupuestoView ( PresupuestoView *l )
{
    BL_FUNC_DEBUG

    EmailQToolButton *mui_exporta_efactura2 = new EmailQToolButton ( l, NULL, NULL,  NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    
    return 0;
}


///
/**
\param l
\return
**/
int PedidoClienteView_PedidoClienteView ( PedidoClienteView *l )
{
    BL_FUNC_DEBUG

    EmailQToolButton *mui_exporta_efactura2 = new EmailQToolButton ( NULL, l, NULL, NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    
    return 0;
}


///
/**
\param l
\return
**/
int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView *l )
{
    BL_FUNC_DEBUG

    EmailQToolButton *mui_exporta_efactura2 = new EmailQToolButton ( NULL, NULL, l, NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    
    return 0;
}


///
/**
\param l
\return
**/
int FacturaView_FacturaView ( FacturaView *l )
{
    BL_FUNC_DEBUG

    EmailQToolButton *mui_exporta_efactura2 = new EmailQToolButton ( NULL, NULL, NULL, l, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    
    return 0;
}


///
/**
\ return
**/
int LaunchChoseMailer ()
{
    BL_FUNC_DEBUG
    
    ChoseMailer *diag = new ChoseMailer ( );
    diag->show();
        
    return 0;
}


<<<<<<< HEAD
=======
///
/**
\ param recipient
\ param subject
\ param body
\ param attached
\ return
**/
int Thunderbird ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached )
{
    BL_FUNC_DEBUG
    QString dir_email = g_confpr->value( CONF_EMAIL_CLIENT );
    
    QString runcommand = QString(CAD_COMILLAS + dir_email + CAD_COMILLAS);
    runcommand += " -compose ";
    runcommand += "to='" + recipient + "',";
    runcommand += "bcc='" + bcc + "',";
    runcommand += "subject='" + subject + "',";
    body.replace(",","&#44;"); // convertim les comes amb el seu valor html, del contrari thunderbird ens talla el missatge.
    runcommand += "body='" + body + "',";
    if (attached != "") {
        runcommand += "attachment='file://" + attached + "'";
    }
            
    system(QString( runcommand + background).toAscii());
    
    return 0;
}

///
/**
\ param recipient
\ param subject
\ param body
\ param attached
\ return
**/
int Kmail ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached )
{
    BL_FUNC_DEBUG
    QString dir_email = g_confpr->value( CONF_EMAIL_CLIENT );
    
    QString runcommand = QString(CAD_COMILLAS + dir_email + CAD_COMILLAS);
    if (subject != "")
      runcommand += " -s \"" + subject + "\"";
    if (bcc != "") 
      runcommand += " -b " + bcc;
    if (body != "")
      runcommand += " --body \"" + body + "\"";
    if (attached != "") 
      runcommand += " --attach " + attached;
  
    runcommand += " " + recipient;
            
    system(QString( runcommand + background).toAscii());
    
    return 0;
}
///
/**
\ param recipient
\ param subject
\ param body
\ param attached
\ return
**/
int Evolution ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached )
{
    BL_FUNC_DEBUG
    QString dir_email = g_confpr->value( CONF_EMAIL_CLIENT );
    
    
    QString urlmail = " \"mailto:" + recipient;
    urlmail += "?subject=" + subject;
    urlmail += "&bcc=" + bcc;
    urlmail += "&body=" + body;
    if (attached != "") {
        urlmail += "&attach=" + attached + "\"";
    }
    QString runcommand = QString(CAD_COMILLAS + dir_email + QUrl(urlmail, QUrl::TolerantMode).toString() + CAD_COMILLAS);
    system(QString( runcommand + background).toAscii());
    return 0;

}

///
/**
\ param recipient
\ param subject
\ param body
\ param attached
\ return
**/
#ifdef Q_OS_WIN32
int Outlook ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached )
{
    BL_FUNC_DEBUG
    QString dir_email = g_confpr->value( CONF_EMAIL_CLIENT );
    QString barra2;
    
    QString urlmail = " /c ipm.note";
    if (attached != "") {
        urlmail += " /a " + attached;
    }
    urlmail += " /m " + recipient;
    QByteArray barray = QUrl(subject, QUrl::TolerantMode).toEncoded();
    QByteArray barray2 = QUrl(body, QUrl::TolerantMode).toEncoded();
    QString stringed(barray);
    QString stringed2(barray);
    urlmail += "?subject=" + stringed;
    urlmail += "&body=" + stringed2;
    
    QString runcommand = QString(CAD_COMILLAS + dir_email + CAD_COMILLAS + CAD_COMILLAS + urlmail + CAD_COMILLAS);
    
    system(QString( runcommand + background).toAscii());
    return 0;

}
#endif

///
/**
\ param recipient
\ param subject
\ param body
\ param attached
\ return
**/
int bfSendEmail ( QString &recipient, QString &bcc, QString &subject, QString &body, QString &attached )
{
    BL_FUNC_DEBUG
    
    QString dir_email = g_confpr->value( CONF_EMAIL_CLIENT );
      
    QFileInfo fileinfo(dir_email);
    QString program_name = fileinfo.fileName();
    #ifdef Q_OS_WIN32
        program_name.replace(".exe", "");
    #endif
    
    if (program_name == "thunderbird") {
        Thunderbird (  recipient, bcc, subject, body, attached );
    } else if (program_name == "kmail") {
        Kmail (  recipient, bcc, subject, body, attached );
    } else if (program_name == "evolution") {
        Evolution (  recipient, bcc, subject, body, attached );
    }
    #ifdef Q_OS_WIN32
     else if (program_name == "Outlook") {
        Outlook (  recipient, bcc, subject, body, attached );
    } // end if
    #endif
    
    return 0;
    
}
>>>>>>> 44fc05b... Arreglo problemas con kmail en el plugin de correo y mejoro el texto del README
