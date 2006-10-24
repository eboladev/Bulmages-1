/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <q3table.h>

#include "mpatrimonialesview.h"
#include "mpatrimonialview.h"


mpatrimonialesview::mpatrimonialesview(QWidget *parent, Qt::WFlags fl)
        : QDialog (parent, fl) {
    setupUi(this);
    modo = 0;
}


mpatrimonialesview::~mpatrimonialesview() {}


/// Esta funci&oacute;n se encarga de hacer las inicializaciones de todo el formulario.
/** Se llama as&iacute; y no desde el constructor porque as&iacute; la podemos llamar
    desde dentro de la misma clase, etc, etc, etc. */
int mpatrimonialesview::inicializa(postgresiface2 *conn) {
    cursor2 *cursoraux1;

    conexionbase = conn;
    conexionbase->begin();
    /// Vamos a cargar el n&uacute;mero de d&iacute;gitos de cuenta para poder hacer
    /// una introducci&oacute;n de numeros de cuenta m&aacute;s pr&aacute;ctica.
    conexionbase->begin();
    QString query = "SELECT valor FROM configuracion WHERE nombre = 'CodCuenta'";
    cursoraux1 = conexionbase->cargacursor(query, "codcuenta");
    conexionbase->commit();
    numdigitos = cursoraux1->valor("valor").length();
    delete cursoraux1;
    fprintf(stderr,"las cuentas tienen %d digitos\n", numdigitos);
    inicializatabla();
    return(0);
}


void mpatrimonialesview::inicializatabla()  {
    tabla->setNumRows(0);
    tabla->setNumCols(2);
    tabla->horizontalHeader()->setLabel(0, tr("CODIGO"));
    tabla->horizontalHeader()->setLabel(1, tr("Masa patrimonial"));
    tabla->hideColumn(2);
    tabla->hideColumn(0);
    tabla->setColumnWidth(1, 400);
    QString query = "SELECT * FROM mpatrimonial WHERE idbalance ISNULL";
    conexionbase->begin();
    cursor2 *cursoraux1 = conexionbase->cargacursor(query, "elquery");
    conexionbase->commit();
    tabla->setNumRows(cursoraux1->numregistros());
    int i = 0;
    while (!cursoraux1->eof()) {
        tabla->setText(i, 0, cursoraux1->valor("idmpatrimonial"));
        tabla->setText(i, 1, cursoraux1->valor("descmpatrimonial"));
        cursoraux1->siguienteregistro();
        i++;
    } // end while
    delete cursoraux1;
    tabla->setReadOnly(TRUE);
}


void mpatrimonialesview::dbtabla(int row, int colummn, int button, const QPoint &mouse) {
    fprintf(stderr, "Se ha hecho doble click sobre la tabla\n");
    /// Dependiendo del modo hacemos una cosa u otra.
    if (modo == 0) {
        QString idmpatrimonial = tabla->text(row, 0).toAscii();
        /// Creamos el objeto mpatrimonialview, y lo lanzamos.
        mpatrimonialview *masa = new mpatrimonialview(this);
        masa->inicializa(conexionbase);
        masa->inicializa1(idmpatrimonial);
        masa->exec();
        delete masa;
        /// Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado.
        inicializatabla();
    } else {
        idmasa = tabla->text(tabla->currentRow(), 0).toAscii();
        nommasa = tabla->text(tabla->currentRow(), 1).toAscii();
        close();
    } // end if

    /// Para quitar el warning.
    colummn = button = 0;
    mouse.isNull();
}


void mpatrimonialesview::editarmasa() {
    int row = tabla->currentRow();
    int col = tabla->currentColumn();
    QPoint mouse;
    dbtabla(row, col, 0, mouse);
    inicializatabla();
}


void mpatrimonialesview::borrarmasa() {
    int row;
    row = tabla->currentRow();
    idmasa = tabla->text(tabla->currentRow(), 0).toAscii();
    QString query;
    query.sprintf("DELETE FROM compmasap WHERE idmpatrimonial   = %s", idmasa.toAscii().constData());
    conexionbase->begin();
    conexionbase->ejecuta(query);
    query.sprintf("DELETE FROM mpatrimonial WHERE idmpatrimonial = %s", idmasa.toAscii().constData());
    conexionbase->ejecuta(query);
    conexionbase->commit();
    inicializatabla();
}


void mpatrimonialesview::nuevamasa() {
    mpatrimonialview *masa = new mpatrimonialview(this);
    masa->inicializa(conexionbase);
    masa->exec();
    delete masa;
    /// Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado.
    inicializatabla();
}

