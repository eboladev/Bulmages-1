/***************************************************************************
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

#include "chosemailer.h"


#ifndef Q_OS_WIN32
    /// Para unix es sencillo, simplemente indicamos el separador de PATH y que no haya extension de ejecutable
    QString PATHSEP = ":";
    QString PATH_EXTENSION = "";
#else
    /// En Windows "exploraremos" los directorios que haya dentro del clasico ProgramFiles, facilitando el uso al usuario.
    QString PATHSEP = ";";
    QString PATH_EXTENSION = ".exe";
#endif

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ChoseMailer::ChoseMailer(QWidget *parent) :
    QDialog(parent)
{
    BL_FUNC_DEBUG
    
    /// Iniciamos la UI
    try{
        setupUi( this );
    } catch ( ... ) {
        blMsgInfo ( _ ( "No se ha podido crear la ventana de seleccion" ) );
    } // end try
    
    /// Declaramos aqui los clientes de email, (sin .exe aunque estemos en windows)
    /// Ejemplo: mui_mailclients->addItem(QString("Programa Ejemplo"), QString("ejemplo") );
    
    mui_mailclients->addItem(QString("..."), QString("0") );
    mui_mailclients->addItem(QString("Mozilla Thunderbird"), QString("thunderbird") );
    mui_mailclients->addItem(QString("Kmail"), QString("kmail") );
    mui_mailclients->addItem(QString("Evolution"), QString("evolution") );
}


/** Aunque podrian indicarse, no requiere de acciones adicionales en el destructor.
*/
ChoseMailer::~ChoseMailer()
{
    /// Destructor de la clase
}

///
/**
\ void
**/
void ChoseMailer::on_mui_botonera_rejected()
{
    BL_FUNC_DEBUG
    /// Cerramos el dialogo con la botonera
    g_confpr->setValue( CONF_EMAIL_CLIENT, "" );
    this->close();
}

///
/**
\ void
**/
void ChoseMailer::on_mui_botonera_accepted()
{
    BL_FUNC_DEBUG
    
    /// Procedemos a guardar la configuracion con la botonera
    
    QString dir_email = g_confpr->value( CONF_EMAIL_CLIENT );
    
    GuardarConfig();

    this->close();
}

///
/**
\ void
**/
int ChoseMailer::GuardarConfig()
{
     BL_FUNC_DEBUG
     
     /// Guardamos la configuracion, quiza sea interesante extraer este metodo hacia bulmalib posteriormente.
     
     
    /// Detectamos las variables de entorno de usuario, para guardar la configuracion
    #ifndef Q_OS_WIN32
        QString dirusuario = getenv ( "HOME" );
    #else
        QString dirusuario = getenv ( "UserProfile" );
        dirusuario.replace('\\', '/'); // Para no tener barras de los dos tipos mezclados en la misma ruta, usaremos las normales siempre
    #endif // Q_OS_WIN32
    
    QString m_dirLocalConf = dirusuario + "/.bulmages/bulmafact.conf";
    
    
    QFile file ( m_dirLocalConf );
    if ( !file.open ( QIODevice::Append | QIODevice::Text ) )
        return 1;
    // end if
        
    QString dir_email = g_confpr->value( CONF_EMAIL_CLIENT );

    QTextStream filestr ( &file );
        if ( dir_email != "" ) {
            filestr << QString("CONF_EMAIL_CLIENT").toAscii().data();
            filestr << "   ";
            filestr << QString(dir_email).toAscii().data();
            filestr << endl;
        } // end if
    file.close();    
    return 0;
}

///
/**
\ param index
**/
void ChoseMailer::on_mui_mailclients_currentIndexChanged(int index)
{
    /// Detectamos si el index ha cambiado, y en ese caso seteamos CONF_EMAIL_CLIENT y mui_location con la posible localizacion del cliente de email
    
    BL_FUNC_DEBUG
    
    QString currentItem = mui_mailclients->itemData(mui_mailclients->currentIndex ()).toString();
    if (currentItem != "0" ) {
        int sip = SearchExecutable(currentItem);
        if (sip == 0) {
            mui_location->setText ( programLocated );
            
        } else {
            blMsgInfo(_("No se ha encontrado el programa.\nPor favor, especifique el programa manualmente."));
            mui_location->setText ( "" );
        } // end if
        
    } else {
        mui_location->setText ( "" );
    } // end if
}
///
/**
\ void
**/
void ChoseMailer::on_mui_filechoser_clicked()
{
    BL_FUNC_DEBUG
    
    /// Abrimos el buscador de archivos, por si quisieramos especificar manualmente el cliente
    QString fileName = QFileDialog::getOpenFileName ( this, _ ( "Seleccionar" ), "", _ ( "Todos los archivos") + "(* " +PATH_EXTENSION + ")");
    mui_location->setText ( fileName );
}

///
/**
\ param arg1
**/
void ChoseMailer::on_mui_location_textChanged(const QString &arg1)
{
    g_confpr->setValue( CONF_EMAIL_CLIENT, arg1 );
}

///
/**
\ param program
\ return
**/
int ChoseMailer::SearchExecutable ( QString &program )
{
    BL_FUNC_DEBUG
    
    /// Metodo para localizar ejecutables, quiza sea interesante extraer este metodo hacia bulmalib posteriormente.
    
    
    /// Cargamos el PATH y el programa que queremos buscar.
    QString GET_PATH = getenv ("PATH");
    QString currentItem = program;
    
    #ifdef Q_OS_WIN32
        /// En Windows "exploraremos" los directorios que haya dentro del clasico ProgramFiles, facilitando el uso al usuario.
        
        /// Detectamos ProgramFiles mediante getenv, quiza deba cambiarse para que explore ambos si existen.
        
        QString programFiles = getenv("ProgramFiles(x86)");
        if (programFiles==NULL)
        {
            programFiles = getenv("ProgramFiles");
        }
        
        /// Declaramos el iterador, para hacer la exploracion.
        QDirIterator iteradorDirectorios(programFiles, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotDot, QDirIterator::Subdirectories); // Descartamos enlaces, y ".."
        QStringList directoriosIterados;
        while(iteradorDirectorios.hasNext()) {
            
            // el iterador recorrera hasta que no queden mas directorios
            iteradorDirectorios.next();
            
            QString directory_name = iteradorDirectorios.filePath();
            QDir directory(directory_name);
            directoriosIterados << directory.canonicalPath(); // Solo deseamos direcciones canonicas
        } // end while
        
        directoriosIterados.removeDuplicates(); // Eliminamos posibles duplicados
        QString joined_path = directoriosIterados.join(";");
        GET_PATH.append(";"+joined_path);
        GET_PATH.replace('\\', '/');  // Para no tener barras de los dos tipos mezclados en la misma ruta, usaremos las normales siempre
    #endif // Q_OS_WIN32
    
    
    
    QString PATH_EXTEND = "/";
    QStringList SPLITED_PATH = GET_PATH.split(PATHSEP);
    programLocated = "";
    
    
    /// Iteramos el PATH (con las modificaciones correspondientes) en busqueda del ejecutable.
    for (int i = 0; i < SPLITED_PATH.size(); ++i) {
        QString posibleProgram = SPLITED_PATH.at(i).toAscii() + PATH_EXTEND +  currentItem + PATH_EXTENSION;
        posibleProgram = QUrl(posibleProgram, QUrl::TolerantMode).toString();
        
        QDir dir;
        if (dir.exists (posibleProgram)) {
            programLocated.append(posibleProgram);
            return 0;
        } // end if
    } // end for
    return 1;
    
}
