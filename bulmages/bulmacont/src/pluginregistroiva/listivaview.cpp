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

#define COL_IDIVA           0
#define COL_IDREGISTROIVA   1
#define COL_IDTIPOIVA       2
#define COL_IDCUENTA        3
#define COL_NOMBRETIPOIVA   4
#define COL_CODIGO          5
#define COL_BASEIVA         6
#define COL_IVAIVA          7

#include <QMessageBox>
#include <QKeyEvent>
#include <QEvent>
#include <QTextStream>

#include "empresa.h"
#include "listivaview.h"
#include "funcaux.h"


void ListIvaView::guardaconfig() {
    _depura("ListIvaView::guardaconfig", 0);
    QString aux = "";
    QFile file(confpr->valor(CONF_DIR_USER) + "confListIvaView.cfn");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        for (int i = 0; i < columnCount(); i++) {
            showColumn(i);
            stream << columnWidth(i) << "\n";
        } // end for
        file.close();
    } // end if
}


void ListIvaView::cargaconfig() {
    _depura("ListIvaView::cargaconfig", 0);
    QFile file( confpr->valor(CONF_DIR_USER) + "confListIvaView.cfn" );
    QString line;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        for (int i = 0; i < columnCount(); i++) {
            QString linea = stream.readLine();
            setColumnWidth(i, linea.toInt());
        } // end for
        file.close();
    } // end if
}


ListIvaView::ListIvaView(QWidget * parent) : QTableWidget(parent), ListIva() {
    _depura("ListIvaView::ListIvaView", 0);
    /// Inicializamos la tabla de lineas de presupuesto
    setColumnCount(8);
    setRowCount(0);
    QStringList etiquetas;
    etiquetas << tr("COL_IDIVA") << tr("COL_IDTIPOIVA") << tr("COL_IDCUENTA") << tr("COL_CODIGO") << tr("COL_NOMBRETIPOIVA") << tr("COL_IDREGISTROIVA") << tr("COL_BASEIVA") << tr("COL_IVAIVA");
    setHorizontalHeaderLabels(etiquetas);

    setColumnWidth(COL_IDIVA, 100);
    setColumnWidth(COL_IDTIPOIVA, 100);
    setColumnWidth(COL_IDCUENTA, 100);
    setColumnWidth(COL_CODIGO, 100);
    setColumnWidth(COL_NOMBRETIPOIVA, 100);
    setColumnWidth(COL_IDREGISTROIVA, 100);
    setColumnWidth(COL_BASEIVA, 100);
    setColumnWidth(COL_IVAIVA, 74);

    hideColumn(COL_IDIVA);
    hideColumn(COL_IDTIPOIVA);
    hideColumn(COL_IDCUENTA);
    hideColumn(COL_IDREGISTROIVA);

    setSelectionMode(QAbstractItemView::SingleSelection);
    /// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase
    /// configuraci&oacute;n que es global.
    QPalette palette;
    palette.setColor(this->backgroundRole(), confpr->valor(CONF_BG_LINPRESUPUESTOS));
    this->setPalette(palette);

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int, int)));
    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
    installEventFilter(this);
    cargaconfig();
}


ListIvaView::~ListIvaView() {
    _depura("ListIvaView::~ListIvaView()", 0);
    guardaconfig();
}


void ListIvaView::pintaListIva() {
    _depura("ListIvaView::pintaListIva\n", 0);
    setRowCount(0);
    setRowCount(m_lista.count());
    /// TODO Habra que vaciar la tabla para que el pintado fuera exacto.
    Iva *linea;
    uint i = 0;
    for (linea = m_lista.first(); linea; linea = m_lista.next()) {
        pintaIva(i);
        resizeRowToContents(i);
        i++;
    } // end for
    _depura("END ListIvaView::pintaListIva\n", 0);
}


void ListIvaView::contextMenu(int row, int, const QPoint & pos) {
    QMenu *popup = new QMenu();
    QAction *borrarlinea = popup->addAction(tr("Borrar linea"));
    QAction *opcion = popup->exec(pos);
    delete popup;
    if (opcion == borrarlinea) {
        borraIva(row);
    } // end if
}


void ListIvaView::borraIvaAct() {
    borraIva(currentRow());
}


void ListIvaView::pintaIva(int pos) {
    _depura("ListIvaView::pintaIva\n", 0);
    Iva *linea;
    linea = m_lista.at(pos);

    item(pos, COL_IDIVA)->setText(linea->idiva());
    item(pos, COL_IDTIPOIVA)->setText(linea->idtipoiva());
    item(pos, COL_IDCUENTA)->setText(linea->idcuenta());
    item(pos, COL_CODIGO)->setText(linea->codigo());
    item(pos, COL_NOMBRETIPOIVA)->setText(linea->nombretipoiva());
    item(pos, COL_IDREGISTROIVA)->setText(linea->idregistroiva());
    item(pos, COL_BASEIVA)->setText(linea->baseiva());
    item(pos, COL_IVAIVA)->setText(linea->ivaiva());
    resizeRowToContents(pos);

    _depura("END ListIvaView::pintaIva\n", 0);
}


bool ListIvaView::eventFilter(QObject *obj, QEvent *ev) {
    _depura("ListIvaView::eventFilter()\n", 1);
    /*
        QString idArticle;
        //    linpresupuesto *linea=lineaact();
        Iva *linea;//=m_lista.at(currentRow());
     
        if ( ev->type() == QEvent::KeyRelease ) {
            QKeyEvent *k = (QKeyEvent *)ev;
            int col=currentColumn();
            int row=currentRow();
            switch (k->key()) {
            case Qt::Key_Plus:
                if (col == COL_DESCLPRESUPUESTO) {
                    setText(row,col,editaTexto(text(row,col)));
                    valueBudgetLineChanged(row,col);
                    return TRUE;
                }// end if
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
                switch (col) {
                case COL_CODARTICULO:
                    setCurrentCell(row, COL_DESCLPRESUPUESTO);
                    break;
                case COL_DESCLPRESUPUESTO:
                    setCurrentCell(row, COL_CANTLPRESUPUESTO);
                    break;
                case COL_CANTLPRESUPUESTO:
                    setCurrentCell(row, COL_PVPLPRESUPUESTO);
                    break;
                case COL_PVPLPRESUPUESTO:
                    setCurrentCell(row+1, COL_CODARTICULO);
                    break;
                }// end switch
                return TRUE;
                break;
            }// end switch
        }// end if
    */
    return QTableWidget::eventFilter(obj, ev);
}


void ListIvaView::valueBudgetLineChanged(int row, int col) {
    _depura("valueBudgetLineChanged \n", 0);
    Iva *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_BASEIVA: {
                float baseiva = item(row, COL_BASEIVA)->text().replace(",", ".").toFloat();
                linea->setbaseiva(QString::number(baseiva));
                break;
            }
        case COL_IVAIVA: {
                float ivaiva = item(row, COL_IVAIVA)->text().replace(",", ".").toFloat();
                linea->setivaiva(QString::number(ivaiva));
                break;
            } // end case
        } // end switch
        pintaIva(row);
    } // end if
    _depura("END valueBudgetLineChanged \n", 0);
}


/// Devuelve la l&iacute;nea que se esta tratando actualmente.
Iva *ListIvaView::lineaact() {
    _depura("ListIvaView::lineaact()\n", 0);
    return lineaat(currentRow());
}


/// Devuelve la l&iacute;nea especificada, y si no existe se van creando
/// l&iacute;neas hasta que exista.
Iva *ListIvaView::lineaat(int row) {
    _depura("ListIvaView::lineaat\n", 0);
    Iva *linea;
    if (row >= 0) {
        while (m_lista.at(row) == 0) {
            fprintf(stderr, "Creamos la linea\n");
            linea = new Iva(companyact);
            linea->setidregistroiva(mdb_idregistroiva);
            m_lista.append(linea);
        } // end while
        return(m_lista.at(row));
    } else {
        _depura("Linea inexistente\n", 0);
        return NULL;
    }// end if
}

