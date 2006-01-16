/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borr� Riera                              *
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
#ifndef PAGOVIEW_H
#define PAGOVIEW_H

// PRESUPUESTOS.

#include "pagobase.h"
#include "postgresiface2.h"
#include "pago.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "dialogchanges.h"

#include <qlineedit.h>
#include <q3textedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

/** @author Tomeu Borr� Riera */
class company;

class PagoView : public PagoBase , public Pago, public dialogChanges  {
    Q_OBJECT
public:
    PagoView(company *, QWidget *, const char *);
    ~PagoView();
    void inicialize();
    void manageArticle(int);  
	void closeEvent( QCloseEvent *); 

void	pintafechapago(QString id) {	m_fechapago->setText(id);};
void	pintacomentpago(QString id) {m_comentpago->setText(id);};
void    pintaidproveedor(QString id) {m_proveedor->setidproveedor(id);};
void    pintarefpago(QString id) {m_refpago->setText(id);};
void    pintacantpago(QString id) {m_cantpago->setText(id);};
void    pintaprevisionpago(QString id) {
	if (id == "t" || id == "TRUE") m_previsionpago->setChecked(TRUE);
	else m_previsionpago->setChecked(FALSE);
};


public slots:
    virtual void s_savePagoView() {guardaPago();};
    virtual void s_cargaPagoView(QString id) {Pago::cargaPago(id);setCaption("Pago "+mdb_refpago);};
    virtual void s_removePagoView();
    virtual void s_comentpagotextChanged(const QString &str) {setcomentpago(str);};
    virtual void s_refpagotextChanged(const QString &str) {setrefpago(str);};
    virtual void s_cantpagotextChanged(const QString &str) {setcantpago(str);};
    
    virtual void s_previsionpagostateChanged(int i) {
    	if (i) setprevisionpago("TRUE");
	else setprevisionpago("FALSE");
    }    
    virtual void s_proveedorvalueChanged(QString id) {setidproveedor(id);};
    virtual void s_fechapagovalueChanged(QString id) {setfechapago(id);};
};

#endif
