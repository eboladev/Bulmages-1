/***************************************************************************
                          qtable1.cpp  -  description
                             -------------------
    begin                : Mon Jan 13 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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

#include "qtable1.h"
#include <qfont.h>
#include <qpainter.h>
#include <qcolor.h>

#include "configuracion.h"

QTable1::QTable1(QWidget * parent, const char * name ):QTable(parent, name ) {
    // 	qApp->installEventFilter( this );
    // 	connect( this, SIGNAL(found()), this, name );

}

void QTable1::sortColumn ( int col, bool ascending, bool wholeRows) {
    QTable::sortColumn(col,ascending,true);
    
    // Para quitar el warning
    wholeRows=FALSE;
}

QWidget *QTable1::beginEdit(int row, int col, bool type) {
    return(QTable::beginEdit(row,col,type));
}

bool QTable1::eventFilter( QObject *obj, QEvent *event ) {
    static bool ctrlpulsado= FALSE;
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        int key = keyEvent->key();
        fprintf(stderr,"Key event %d %c\n", key, key);
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
    }// end if
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        int key = keyEvent->key();
        if (key == 4129) {
            ctrlpulsado = FALSE;
        }// end if
    }// end if
    return QTable::eventFilter(obj, event);
}// end eventFilter


void QTable1::setItem(int row, int col, QTableItem *it) {
    //   fprintf(stderr,"Creacion del item para %d %d",row, col);
    //   QTableItem1 *ot = new QTableItem1(this, QTableItem::OnTyping, it->text(), 0);
    QTable::setItem(row, col, it);
}// end setItem





void QTable1::paintCell ( QPainter * p, int row, int col, const QRect &
                          cr, bool selected ) {
    /*
        QFont forig( p->font() );
        QPen color( p->pen());
        if( modo==1){
           fprintf(stderr,"Modo = 1\n");
            QFont f( font() );
            QColor c;
            f.setFamily( " Helvetica " );
            f.setPointSize( 9 );
            f.setWeight( QFont::Bold );
            f.setItalic( true );
            c.setRgb(200,0,200);
            p->setFont( f );
    //        p->setBackgroundColor(QColor::QColor(100,0,0));
    //        p->setBrush(c);
            p->setPen(c);
        } else {
           fprintf(stderr,"Modo = 0\n");
            QFont f( font() );
            QColor c(0,0,200);
            f.setFamily( " Arial " );
            f.setPointSize( 8 );
            f.setWeight( QFont::Bold );
            f.setItalic( true );
            p->setFont( f );
    //        p->setBackgroundColor(QColor::QColor(100,0,0));
    //        p->setBrush(c);
            p->setPen(c);
        }
        QTable::paintCell ( p, row, col, cr, selected );
        p->setFont( forig );
        p->setPen(color);
    */
    QTable::paintCell ( p, row, col, cr, selected );
}

/*
QTableItem1(QTable *table ,EditType et ,const QString &text, int mode): QTableItem(table, et, text) {
  modo = mode;
}// end if
*/

void QTableItem1::paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected ) {
    QColorGroup g( cg );
    // last row is the sum row - we want to make it more visible by
    // using a red background
    //    if ( row() == table()->numRows() - 1 )
//    g.setColor( QColorGroup::Base, QColor::QColor(200,200,255) );
    QFont f(p->font());
    // Establecemos el color de fondo de este item como el color de fondo del diario.
    g.setColor( QColorGroup::Base, QColor::QColor(confpr->valor(CONF_BG_DIARIO).c_str()) );

    // Establecemos la fuente segun las preferencias del diario.
    f.setPointSize(atoi(confpr->valor(CONF_FONTSIZE_DIARIO).c_str()));
	 f.setFamily(confpr->valor(CONF_FONTFAMILY_DIARIO).c_str());
    p->setFont( f );

    
    if (modo == 1) {
        g.setColor( QColorGroup::Text, QColor::QColor(confpr->valor(CONF_FG_DIARIO1).c_str()));
    } else {
        g.setColor(QColorGroup::Text, QColor::QColor(confpr->valor(CONF_FG_DIARIO2).c_str()));
    }// end if
    QTableItem::paint( p, g, cr, selected );
}

