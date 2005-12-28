/***************************************************************************
                          qlistviewitem1.cpp  -  description
                             -------------------
    begin                : lun jun 23 2003
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

#include "qlistviewitem1.h"
#include <qfont.h>
#include <qpainter.h>

/*
QListViewItem1::QListViewItem1(const QListViewItem1 & a) : QListViewItem(a) {
}

QListViewItem1::~QListViewItem1(){
}
*/

void QListViewItem1::paintCell( QPainter *p, const QColorGroup &cg, int c, int w, int b ) {
    QColorGroup g( cg );
    QFont forig( p->font() );
    QFont f( p->font() );
    f.setFamily( " Helvetica " );
    // last row is the sum row - we want to make it more visible by
    // using a red background
    //    if ( row() == table()->numRows() - 1 )
 //   g.setColor( QColorGroup::Base, QColor::QColor(200,200,255) );
    if (tipolista == 2) {
        g.setColor( QColorGroup::Text, QColor::QColor(0,0,100));
        f.setWeight( QFont::Bold );
//        f.setItalic( true );
        f.setPointSize( 10 );
    } else if (tipolista < 7){
        g.setColor(QColorGroup::Text, QColor::QColor(50,50,200));
        f.setPointSize( 10 );
    } else {
        g.setColor(QColorGroup::Text, QColor::QColor(100,100,100));
        f.setPointSize( 9 );
    }// end if
    p->setFont( f );
    Q3ListViewItem::paintCell( p, g, c, w, b );
}

void QListViewItem1::width(const QFontMetrics &fm, const Q3ListView *lv, int c) {
   Q3ListViewItem::width(fm, lv, c);
}// end width
