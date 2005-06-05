/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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
#ifndef ALBARANCLIENTEVIEW_H
#define ALBARANCLIENTEVIEW_H

// ALBARANES.
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>

#include "albaranclientebase.h"
#include "albarancliente.h"
#include "postgresiface2.h"

/** @author Tomeu Borrás & Alvaro de Miguel */
class company;


class AlbaranClienteView : public AlbaranClienteBase, public AlbaranCliente {
Q_OBJECT
private:
	cursor2 *m_cursorcombo;
	
public:
	AlbaranClienteView(company *, QWidget *, const char *);
	~AlbaranClienteView();
	void inicialize();
	
	
    void pintaIdAlbaran(QString) {};
    void pintaNumAlbaran(QString val) {m_numalbaran->setText(val);};
    void pintaFechaAlbaran(QString val) {m_fechaalbaran->setText(val);};
    void pintaIdUsuario(QString) {};
    void pintaComentAlbaran(QString val) {m_comentalbaran->setText(val);};
    void pintaIdCliente(QString ) {};
    void pintaIdForma_Pago(QString) {};
    void pintaIdFactura(QString){};
    void pintaIdAlmacen(QString){};
    void pintaCodigoAlmacen(QString val) {m_codigoalmacen->setText(val);};
    void pintaNomAlmacen(QString val) {m_nomalmacen->setText(val);};
    void pintaCifCliente(QString val) {m_cifcliente->setText(val);};
    void pintaNomCliente(QString val) {m_nomcliente->setText(val);};
    void pintaNumFactura(QString) {};
    
    void pintatotales(float, float) {};	
};

#endif
