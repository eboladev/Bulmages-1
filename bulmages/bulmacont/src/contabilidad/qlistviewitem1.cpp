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

#include <QFont>
#include <QPainter>

#include "qlistviewitem1.h"


void QListViewItem1::paintCell(QPainter *p, const QColorGroup &cg, int c, int w, int b) {
    QColorGroup g(cg);
    QFont forig(p->font());
    QFont f(p->font());
    f.setFamily(" Helvetica ");
    if (tipolista == 2) {
        g.setColor(QColorGroup::Text, QColor::QColor(0, 0, 100));
        f.setWeight(QFont::Bold);
        f.setPointSize(10);
    } else if (tipolista < 7){
        g.setColor(QColorGroup::Text, QColor::QColor(50, 50, 200));
        f.setPointSize(10);
    } else {
        g.setColor(QColorGroup::Text, QColor::QColor(100, 100, 100));
        f.setPointSize(9);
    } // end if
    p->setFont(f);
    Q3ListViewItem::paintCell(p, g, c, w, b);
}


void QListViewItem1::width(const QFontMetrics &fm, const Q3ListView *lv, int c) {
   Q3ListViewItem::width(fm, lv, c);
} // end width

