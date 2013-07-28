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

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFileDialog>

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
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear la ventana de importacion" ) );
    } // end try
    
}

/** No precisa acciones adicionales en el destructor.
*/
Archivo::~Archivo()
{
    BL_FUNC_DEBUG
    /// ATENCION: Hacer esto es un error ya que puede machacar procesos dependientes del listado.
    // ((BfCompany *)mainCompany())->refreshCobrosCliente();
    
}

void Archivo::on_mui_buscarArchivo_clicked()
{

    QString fileName = QFileDialog::getOpenFileName ( this, _ ( "Open file" ), "", _ ( "All files (*)" ) );
    mui_archivo->setText ( fileName );
}

/** No precisa acciones adicionales en el destructor.
*/
void Archivo::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    /*
        QFile file ( mui_archivo->text() );
        if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
            return;

        if ( mui_cabeceras->isChecked() ) {
            file.readLine();
        } // end if

        mainCompany()->begin();
        while ( !file.atEnd() ) {
            QByteArray line = file.readLine();
            procesarLinea ( line );
        } // end while
        mainCompany()->commit();
        file.close();
    */
    
}
