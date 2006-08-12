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

#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>

#include "dialogchanges.h"
#include "funcaux.h"


void dialogChanges::dialogChanges_cargaInicial() {
    _depura("dialogChanges::dialogChanges_cargaInicial", 0);
    m_maxQText = 0;
    m_maxQLine = 0;
    m_maxQTable = 0;

    QList<QTextEdit *> l1 = m_obje->findChildren<QTextEdit *>();
    QListIterator<QTextEdit *> it1 (l1);
    while (it1.hasNext()) {
        m_listaQText[m_maxQText++] = it1.next();
    } // end while

    QList<QLineEdit *> l2 = m_obje->findChildren<QLineEdit *>();
    QListIterator<QLineEdit *> it2 (l2);
    while (it2.hasNext()) {
        m_listaQLine[m_maxQLine++] = it2.next();
    } // end while

    QList<QTableWidget *> l3 = m_obje->findChildren<QTableWidget *>();
    QListIterator<QTableWidget *> it3 (l3);
    while (it3.hasNext()) {
        m_listaQTable[m_maxQTable++] = it3.next();
    } // end while

    valorinicial = calculateValues();
    _depura(valorinicial.toAscii(), 0);
    _depura("END dialogChanges::dialogChanges_cargaInicial", 0);
}


bool dialogChanges::dialogChanges_hayCambios() {
    _depura("dialogChanges::dialogChanges_hayCambios", 0);
    QString valorfinal = calculateValues();
    _depura(valorfinal.toAscii(), 0);
    _depura(valorinicial.toAscii(), 0);
    _depura("END dialogChanges::dialogChanges_hayCambios", 0);
    return !(valorinicial == calculateValues());
}


QString dialogChanges::calculateValues() {
    _depura("dialogChanges::calculateValues", 0);
    QString values = retrieveValues("QTableWidget");
    values += retrieveValues("QLineEdit");
    values += retrieveValues("QTextEdit");
    _depura("END dialogChanges::calculateValues", 0);
    return values;
}


QString dialogChanges::retrieveValues(QString qsWidget) {
    _depura("dialogChanges::retrieveValues", 0);
    QString values = "";
    for (int i = 0; i < m_maxQLine; i++) {
        if (m_listaQLine[i] != NULL)
            values += ((QLineEdit*)m_listaQLine[i])->text();
    } //end for

    for (int i = 0; i < m_maxQText; i++) {
        if (m_listaQText[i] != NULL)
            values += ((QTextEdit*)m_listaQText[i])->text();
    } // end for

    if (qsWidget == "QTableWidget") {
        for (int i = 0; i < m_maxQTable; i++) {
            if (m_listaQTable[i] != NULL) {
                for (int k = 0; k < ((QTableWidget*)m_listaQTable[i])->rowCount(); k++) {
                    for (int l = 0; l < ((QTableWidget*)m_listaQTable[i])->columnCount(); l++) {
                        values += (((QTableWidget*)m_listaQTable[i]))->item(k, l)->text();
                    } // end for
                } // end for
            } // end if
        } // end for
    } // end if

    _depura("END dialogChanges::retrieveValues", 0);
    return values;
}

