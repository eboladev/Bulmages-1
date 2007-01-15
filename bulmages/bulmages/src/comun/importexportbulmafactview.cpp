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

#include <QCheckBox>
#include <QLineEdit>
#include <QFile>
#include <QToolButton>
#include <q3filedialog.h>
#include <q3progressbar.h>
#include <q3textbrowser.h>

#include "importexportbulmafactview.h"
#include "pgimportfiles.h"
#include "busquedafecha.h"
#include "calendario.h"


void ImportExportBulmafactView::alerta(int a, int b) {
    m_progressbar->setProgress(a, b);
}


void ImportExportBulmafactView::mensajeria(QString mensaje) {
    fprintf(stderr,"MENSAJE:\n\n\n%s\n\n\n", mensaje.ascii());
    mensajein += mensaje + "<BR>";
    m_mensajes->setText("<HTML><BODY BGCOLOR='#CCCCCC'>" + QString(mensajein.latin1()) + "</BODY></HTML>");
    // m_mensajes->scrollBy(0,400);
}


ImportExportBulmafactView::ImportExportBulmafactView(postgresiface2 *con, QWidget *parent, const char *name, Qt::WFlags f = 0)
        : QDialog(parent, name, f), pgimportfiles(con) {
    setupUi(this);

    /// Signals and slots connections.
    QObject::connect(pushButton33, SIGNAL(clicked()), this, SLOT(botonImportar()));
    QObject::connect(pushButton33_2, SIGNAL(clicked()), this, SLOT(botonExportar()));
    QObject::connect(pushButtonF_X, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(toolButton1_5, SIGNAL(clicked()), this, SLOT(botonBuscarXML()));

    conexionbase = con;
    inicializaMensajeria();
}


/// Se ha pulsado sobre el boton de bsqueda de una subcuenta.
void ImportExportBulmafactView::botonBuscarXML() {
    m_XML->setText(Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER), "Contaplus (*.xml)", this, "select file", "Elija el archivo"));
}


void ImportExportBulmafactView::botonImportar() {
    inicializaMensajeria();
    fprintf(stderr, "ImportExportBulmafactView::botonImportar()\n");
    QString finicial = m_fechainicial->text();
    QString ffinal = m_fechafinal->text();
    QFile filexml (m_XML->text());
    filexml.open(QIODevice::ReadOnly);
    XML2BulmaFact(filexml);
    filexml.close();
    // m_mensajes->setText("");
}


/** SLOT que responde a la pulsacion del boton de exportar. */
/// Se ha pulsado sobre el boton de exportar. Lee los campos del formulario
/// mira si la opcion de exportaci� es XML o contaplus y llama a las funciones apropiadas
/// de la clase \ref pgimportfiles 
/// Esta funcion utiliza los punteros a funcion para inicializar \ref pgimportfiles
/// con las funciones que se van a encargar de presentacion del estado de la importacion.
/// \todo Los punteros a funcion deberian ser reemplazados por funciones virtuales y
/// haciendo derivar esta clase de pgimportfiles.
void ImportExportBulmafactView::botonExportar() {
    /// Leemos las fechas entre las que tiene que ser el listado.
    inicializaMensajeria();
    QString finicial = m_fechainicial->text();
    QString ffinal = m_fechafinal->text();

    setFInicial(finicial);
    setFFinal(ffinal);
    if (m_test->isChecked() ) {
        setModoTest();
    } // end if
    if (m_XML->text() != "") {
        QFile filexml (m_XML->text());
        filexml.open(QIODevice::WriteOnly);
        bulmafact2XML(filexml);
        filexml.close();
    } // end if
    //	m_mensajes->setText("");
}

