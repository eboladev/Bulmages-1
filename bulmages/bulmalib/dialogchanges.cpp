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

    /* ASI ES CON QT4 */
    QList<QTextEdit *> l1 = m_obje->findChildren<QTextEdit *>();
    QListIterator<QTextEdit *> it1 (l1);
    while (it1.hasNext()) {
        m_listaQText[m_maxQText++] = it1.next();
    }

    QList<QLineEdit *> l2 = m_obje->findChildren<QLineEdit *>();
    QListIterator<QLineEdit *> it2 (l2);
    while (it2.hasNext()) {
        m_listaQLine[m_maxQLine++] = it2.next();
    }

    QList<QTableWidget *> l3 = m_obje->findChildren<QTableWidget *>();
    QListIterator<QTableWidget *> it3 (l3);
    while (it3.hasNext()) {
        m_listaQTable[m_maxQTable++] = it3.next();
    }

    /* ASI ERA CON QT3 */

    /*
    	QObjectList l = m_obje->queryList("QTextEdit");
        QListIterator<QObject *> it1 (l);
        while ( it1.hasNext() ) {
            listaQText[maxQText++] = it1.next();
    	}
     
    	QObjectList l = m_obje->queryList("QLineEdit");
        QListIterator<QObject *> it2 (l);
        while ( it2.hasNext() ) {
            m_listaQLine[m_maxQLine++] = it2.next();
    	}
     
    	l = m_obje->queryList("QTableWidget");
        QListIterator<QObject *> it3 (l);
        while ( it3.hasNext() ) {
            m_listaQTable[m_maxQTable++] = it3.next();
    	}
    */

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
                    }
                }
            }
        }
    } // end if

    /*    QObjectList l = obje->queryList( qsWidget );
        QListIterator<QObject *> it (l);
        QObject *obj;
        QString values="";
        while ( it.hasNext() ) {
            obj = it.next();
    		if (obj->objectName()!="mui_query") {
    		_depura(obj->objectName(),0);
    		_depura(qsWidget,0);
           	 //		++it;
           	 if (qsWidget=="QLineEdit") {
           	     //qDebug("QLineEdit Name = %s",obj->name());
           	     values += ((QLineEdit*)obj)->text();
           	 }
     
           	 if (qsWidget=="QTextEdit") {
           	     //qDebug("QTextEdit Name = %s",obj->name());
           	     values += ((QTextEdit*)obj)->text();
           	 }
     
    		if (qsWidget=="QTableWidget") {
           	    // values += ((QTableWidgetItem*)obj)->text();
    			_depura(obj->objectName(),0);
    			for (int i=0; i < ((QTableWidget*)obj)->rowCount(); i++) {
    				for (int j=0; j < ((QTableWidget*)obj)->columnCount(); j++) {
    					values += (((QTableWidget*)obj))->item(i, j)->text();
    				}
    			}
           	 }
    		}
        }
    */
    //	delete l; // delete the list, not the objects

    _depura("END dialogChanges::retrieveValues", 0);
    return values;
}

