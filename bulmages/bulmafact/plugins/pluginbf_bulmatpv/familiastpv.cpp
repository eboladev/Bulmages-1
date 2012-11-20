/***************************************************************************
 *   Copyright (C) 2011 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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


#include <QColorDialog>

#include "familiastpv.h"


FamiliasTPV::FamiliasTPV(QWidget *parent, Qt::WFlags fl) : BlWidget (parent, fl)
{
    setupUi(this);
    /// Por defecto todos los campos estan desactivados.
    mui_visibletpvfamilia->setEnabled(FALSE);
    mui_ordentpvfamilia->setEnabled(FALSE);
    mui_colortpvfamilia->setEnabled(FALSE);
    mui_seleccionarcolor->setEnabled(FALSE);
    mui_visibletpvfamilia->setCheckState(Qt::Checked);
    mui_ordentpvfamilia->setText("");
    mui_colortpvfamilia->setText("");
    
    m_familiasview = (FamiliasView *) parent;
    
    //connect ( parent->findChild<QWidget *>("m_listFamilias"), SIGNAL(currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * )), this, SLOT( on_m_listFamilias_currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * ) ) );

}


FamiliasTPV::~FamiliasTPV()
{
}


void FamiliasTPV::on_mui_seleccionarcolor_clicked()
{

    QColor coloractual(mui_colortpvfamilia->text());
    if (! coloractual.isValid()){
	coloractual.setNamedColor("#FFFFFF");
    } // end if

    QColorDialog colordialog;
    QColor color = colordialog.getColor(coloractual, this);
    mui_colortpvfamilia->setText(color.name());
    
}

