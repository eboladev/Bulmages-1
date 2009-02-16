/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <fstream>

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <QFileDialog>

#include "blmaincompany.h"
#include "archivo.h"
#include "blconfiguration.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
Archivo::Archivo ( BlMainCompany *comp, QWidget *parent )
        : BlWidget ( comp, parent )
{
    _depura ( "Archivo::Archivo", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear la ventana de importacion" ) );
    } // end try
    _depura ( "END Archivo::Archivo", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
Archivo::~Archivo()
{
    _depura ( "Archivo::~Archivo", 0 );
    /// ATENCION: Hacer esto es un error ya que puede machacar procesos dependientes del listado.
    // ((Company *)empresaBase())->refreshCobrosCliente();
    _depura ( "END Archivo::~Archivo", 0 );
}

void Archivo::on_mui_buscarArchivo_clicked()
{

    QString fileName = QFileDialog::getOpenFileName ( this, _( "Open file" ), "", _( "All files (*)" ) );
    mui_archivo->setText ( fileName );
}

/** No precisa acciones adicionales en el destructor.
*/
void Archivo::on_mui_aceptar_clicked()
{
    _depura ( "Archivo::on_mui_aceptar_clicked", 0 );
/*
    QFile file ( mui_archivo->text() );
    if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
        return;

    if ( mui_cabeceras->isChecked() ) {
        file.readLine();
    } // end if

    empresaBase()->begin();
    while ( !file.atEnd() ) {
        QByteArray line = file.readLine();
        procesarLinea ( line );
    } // end while
    empresaBase()->commit();
    file.close();
*/
    _depura ( "END Archivo::on_mui_aceptar_clicked", 0 );
}
