/***************************************************************************
                          qtable1.cpp  -  description
                             -------------------
    begin                : Mon Jan 13 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qtable2.h"
#include <qfont.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qdatetime.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>


#include "configuracion.h"
#include "funcaux.h"


bool QTableWidgetItem2::operator < ( const QTableWidgetItem & other) {
	_depura("QTableWidgetItem2::operator<",2);
        bool oknumero;
        bool oknumero1;
        QString cad = text();
        QString cad1 = other.text();

        if (cad != "") {
            /// Comprobamos si es un número.
            double ncad = cad.toDouble(&oknumero);
            double ncad1 = cad1.toDouble(&oknumero1);
            if (oknumero && oknumero1) {
                return ncad < ncad1;
            }// end if

            QDate fcad = normalizafecha(cad);
            QString acad = fcad.toString(Qt::ISODate);
            QDate fcad1 = normalizafecha(cad1);
            QString acad1 = fcad1.toString(Qt::ISODate);


            if (acad[2] == '/' && acad1[2]== '/') {
                return fcad < fcad1;
            }// end if
            return cad < cad1;

        } else {
            return TRUE;
        }// end if
	_depura("END QTableWidgetItem2::operator<",2);
    }



QTableWidget2::QTableWidget2(QWidget * parent ):QTableWidget(parent ) {
}

bool QTableWidget2::eventFilter( QObject *obj, QEvent *event ) {
    _depura("QTableWidget2::INIT_eventFilter()\n",0);
    static bool ctrlpulsado= FALSE;
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        int key = keyEvent->key();

        if ( key == Qt::Key_Plus) {
            emit pulsadomas(currentRow(), currentColumn(), key);
            return TRUE;
        }// end if
        if (key == Qt::Key_Asterisk) {
            emit pulsadomas(currentRow(), currentColumn(), key);
            return TRUE;
        }// end if
        if (key == Qt::Key_Enter || key == Qt::Key_Return) { // El enter
            emit pulsadomas(currentRow(), currentColumn(), 4100);
            return TRUE;
        }// end if
        if (key == 4115) {// La tecla hacia arriba
            if (ctrlpulsado)   // Solo en combinacion con el ctrl
                emit pulsadomas(currentRow(), currentColumn(), key);
        }// end if
        if (key == Qt::Key_Delete) {
            if (ctrlpulsado)   // Solo en combinacion con el ctrl
                emit pulsadomas(currentRow(), currentColumn(), key);
        }// end if
        if (key == 4117) {// La tecla hacia arriba
            if (ctrlpulsado)   // Solo en combinacion con el ctrl
                emit pulsadomas(currentRow(), currentColumn(), key);
        }// end if
        if (key == 4129) { // el Control
            ctrlpulsado = TRUE;
        }// end if
	if (key == 47) {  // El dividir /
	   emit pulsadomas(currentRow(), currentColumn(), key);
	   return TRUE;
	}// end if
    }// end if
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        int key = keyEvent->key();
        if (key == 4129) {
            ctrlpulsado = FALSE;
        }// end if
    }// end if
    _depura("QTableWidget2::END_eventFilter()\n",0);
    return QTableWidget::eventFilter(obj, event);
}// end eventFilter


void QTableWidget2::ordenar () {
    _depura("QTableWidget2::ordenar ",0);

/* A TENER EN CUENTA QUE PUEDE DAR PROBLEMAS
    if (m_insercion)
        mui_list->removeRow(mui_list->rowCount()-1);
    mui_list->sortColumn(m_colorden, (Qt::SortOrder) m_tipoorden);
*/

    sortColumn(m_colorden,(Qt::SortOrder) m_tipoorden);
    _depura("END QTableWidget2::ordenar",0);
}



void QTableWidget2::sortByColumn ( int col) {
    _depura("QTableWidget2::sortByColumn ",0);
    if (m_tipoorden == 0)
        m_tipoorden=1;
    else
        m_tipoorden = 0;
    sortColumn(col,(Qt::SortOrder) m_tipoorden);
    _depura("END QTableWidget2::sortByColumn",0);
}


void QTableWidget2::sortColumn ( int col, Qt::SortOrder tipoorden) {
	_depura("QTableWidget2::sortColumn",0);
	m_tipoorden = tipoorden;
	m_colorden = col;
    int lastcol = columnCount();

    insertColumn(lastcol);
    insertColumn(lastcol+1);
    insertColumn(lastcol+2);

    hideColumn(lastcol);
    hideColumn(lastcol + 1);
    hideColumn(lastcol + 2);
     bool oknumero = TRUE;
     bool okfecha = TRUE;

    for (int x = 0; x < rowCount(); x++) {
		QString cad = item(x,col)->text();
		if (cad != "") {
			setText(x,lastcol+0,cad);
			/// Comprobamos si es un número.
			cad.toDouble(&oknumero);
			if (oknumero) {
				while (cad.length() < 10)
					cad.insert(0,"0");
				setText(x,lastcol + 1,cad);
			}// end if

			if (okfecha) {
				if (cad[2] == '/') {
					QDate fech = normalizafecha(cad);
					cad = fech.toString(Qt::ISODate);
				} else {
					okfecha = FALSE;
				}// end if
				setText(x,lastcol + 2,cad);
			}// end if
		}// end if

    } // end for

	if (oknumero)
  		  QTableWidget::sortItems(lastcol+1, tipoorden);
	else if (okfecha) 
  		  QTableWidget::sortItems(lastcol+2, tipoorden);	
	else 
  		  QTableWidget::sortItems(lastcol+0, tipoorden);	

    removeColumn(lastcol+2);
    removeColumn(lastcol+1);
    removeColumn(lastcol+0);
	_depura("END QTableWidget2::sortColumn",0);
}







