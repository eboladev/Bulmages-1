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











/*
QTableItem1(QTable *table ,EditType et ,const QString &text, int mode): QTableItem(table, et, text) {
  modo = mode;
}// end if
*/

/*
void QTableItem1::paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected ) {
    QColorGroup g( cg );
    // last row is the sum row - we want to make it more visible by
    // using a red background
    //    if ( row() == table()->numRows() - 1 )
//    g.setColor( QColorGroup::Base, QColor::QColor(200,200,255) );
    QFont f(p->font());
    // Establecemos el color de fondo de este item como el color de fondo del diario.
    g.setColor( QColorGroup::Base, QColor::QColor(confpr->valor(CONF_BG_DIARIO).toAscii().data()) );

    // Establecemos la fuente segun las preferencias del diario.
    f.setPointSize(atoi(confpr->valor(CONF_FONTSIZE_DIARIO).toAscii().data()));
	 f.setFamily(confpr->valor(CONF_FONTFAMILY_DIARIO).toAscii().data());
    p->setFont( f );

    if (modo == 1) {
        g.setColor( QColorGroup::Text, QColor::QColor(confpr->valor(CONF_FG_DIARIO1).toAscii().data()));
    } else {
        g.setColor(QColorGroup::Text, QColor::QColor(confpr->valor(CONF_FG_DIARIO2).toAscii().data()));
    }// end if
    
    // MODO 10
    if (modo == 10) {
      g.setColor( QColorGroup::Base, QColor::QColor("#FFFFFF") );
      // Establecemos la fuente segun las preferencias del diario.
      f.setPointSize(atoi(confpr->valor(CONF_FONTSIZE_DIARIO).toAscii().data()));
            f.setFamily(confpr->valor(CONF_FONTFAMILY_DIARIO).toAscii().data());
      p->setFont( f );
      g.setColor(QColorGroup::Text, QColor::QColor("#FF0000"));
    }// end if
    // FIN DEL MODO 10
    
    Q3TableItem::paint( p, g, cr, selected );
}

*/

