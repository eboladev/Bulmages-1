/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
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

#include <QToolButton>
#include <QFile>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QProgressBar>
#include <QTextBrowser>

#include "importContaplus.h"
#include "pgimportfiles.h"
#include "busquedafecha.h"


QProgressBar *progress;
QTextEdit *mensajes;
QString mensajein = "";


///
/**
\param a
\param b
**/
void importContaplus::alerta ( int a, int b )
{
    _depura ( "importContaplus::alerta", 0 );
    fprintf ( stderr, "mensaje publicado" );
    progress->setRange ( a, b );
    _depura ( "END importContaplus::alerta", 0 );
}


///
/**
\param mensaje
**/
void importContaplus::mensajeria ( QString mensaje )
{
    _depura ( "importContaplus::mensajeria", 0 );
    mensajein += mensaje;
    mensajes->setText ( mensajein );
    mensajes->ensureCursorVisible();
    _depura ( "END importContaplus::mensajeria", 0 );
}


///
/**
\param con
\param parent
\param f
**/
importContaplus::importContaplus ( postgresiface2 *con, QWidget *parent, Qt::WFlags f = 0 )
        : QDialog ( parent, f ), pgimportfiles ( con )
{
    _depura ( "importContaplus::importContaplus", 0 );
    setupUi ( this );

    /// Signals and slots connections.
    QObject::connect ( pushButton33, SIGNAL ( clicked() ), this, SLOT ( botonImportar() ) );
    QObject::connect ( pushButton33_2, SIGNAL ( clicked() ), this, SLOT ( botonExportar() ) );
    QObject::connect ( pushButtonF_X, SIGNAL ( clicked() ), this, SLOT ( close() ) );
    QObject::connect ( toolButton1_2, SIGNAL ( clicked() ), this, SLOT ( botonBuscarDiario() ) );
    QObject::connect ( toolButton1, SIGNAL ( clicked() ), this, SLOT ( botonBuscarSubCta() ) );
    QObject::connect ( toolButton1_5, SIGNAL ( clicked() ), this, SLOT ( botonBuscarXML() ) );

    progress = m_progressbar;
    mensajes = m_mensajes;
    conexionbase = con;
    _depura ( "END importContaplus::importContaplus", 0 );
}


///
/**
**/
importContaplus::~importContaplus()
{
    _depura ( "importContaplus::~importContaplus", 0 );
    _depura ( "END importContaplus::~importContaplus", 0 );
}



/// Se ha pulsado sobre el boton de bsqueda de una subcuenta.
/**
**/
void importContaplus::botonBuscarXML()
{
    _depura ( "importContaplus::botonBuscarXML", 0 );
    m_XML->setText ( QFileDialog::getSaveFileName ( this, _( "Guardar archivo" ), confpr->valor ( CONF_DIR_USER ), _( "Contaplus (*.xml)" ) ) );
    ;
    _depura ( "END importContaplus::botonBuscarXML", 0 );
}


/// Se ha pulsado sobre el boton de bsqueda de una subcuenta.
/**
**/
void importContaplus::botonBuscarSubCta()
{
    _depura ( "importContaplus::botonBuscarSubCta", 0 );
    m_subCta->setText ( QFileDialog::getSaveFileName ( this, _( "Guardar archivo" ), confpr->valor ( CONF_DIR_USER ), _( "Contaplus (*.txt)" ) ) );
    ;
    _depura ( "END importContaplus::botonBuscarSubCta", 0 );
}


/// SLOT que responde a la pulsacion de seleccion de archivo.
/**
**/
void importContaplus::botonBuscarDiario()
{
    _depura ( "importContaplus::botonBuscarDiario", 0 );
    m_diario->setText ( QFileDialog::getSaveFileName ( this, _( "Guardar archivo" ), confpr->valor ( CONF_DIR_USER ), _( "Contaplus (*.txt)" ) ) );
    ;
    _depura ( "END importContaplus::botonBuscarDiario", 0 );
}


///
/**
**/
void importContaplus::botonImportar()
{
    _depura ( "importContaplus::botonImportar", 0 );
    QString finicial = m_fechainicial->text();
    QString ffinal = m_fechafinal->text();
    if ( m_subCta->text() != "" ) {
        QFile filecont ( m_subCta->text() );
        QFile fileasie ( m_diario->text() );
        filecont.open ( QIODevice::ReadOnly );
        fileasie.open ( QIODevice::ReadOnly );
        setFInicial ( finicial );
        setFFinal ( ffinal );
        if ( m_test->isChecked() ) {
            setModoTest();
        } // end if
        contaplus2Bulmages ( filecont, fileasie );
        filecont.close();
        fileasie.close();
    } else {
        QFile filexml ( m_XML->text() );
        filexml.open ( QIODevice::ReadOnly );
        XML2Bulmages ( filexml );
        filexml.close();
    } // end if
    mensajein = "";
    _depura ( "END importContaplus::botonImportar", 0 );
}


/** SLOT que responde a la pulsacion del boton de exportar */
/// Se ha pulsado sobre el boton de exportar. Lee los campos del formulario
/// mira si la opcion de exportacion es XML o contaplus y llama a las funciones apropiadas
/// de la clase \ref pgimportfiles
/// Esta funcion utiliza los punteros a funcion para inicializar \ref pgimportfiles con
/// las funciones que se van a encargar de presentacion del estado de la importacion.
/// \todo Los punteros a funcion deberian ser reemplazados por funciones virtuales y
/// haciendo derivar esta clase de pgimportfiles.
/**
**/
void importContaplus::botonExportar()
{
    _depura ( "importContaplus::botonExportar", 0 );

    /// Leemos las fechas entre las que tiene que ser el listado.
    QString finicial = m_fechainicial->text();
    QString ffinal = m_fechafinal->text();

    setFInicial ( finicial );
    setFFinal ( ffinal );
    if ( m_test->isChecked() ) {
        setModoTest();
    } // end if
    if ( m_subCta->text() != "" ) {
        QFile filecont ( m_subCta->text() );
        QFile fileasie ( m_diario->text() );
        filecont.open ( QIODevice::WriteOnly );
        fileasie.open ( QIODevice::WriteOnly );
        bulmages2Contaplus ( filecont, fileasie );
        filecont.close();
        fileasie.close();
    } // end if
    if ( m_XML->text() != "" ) {
        QFile filexml ( m_XML->text() );
        filexml.open ( QIODevice::WriteOnly );
        bulmages2XML ( filexml );
        filexml.close();
    } // end if
    mensajein = "";
    _depura ( "END importContaplus::botonExportar", 0 );
}

