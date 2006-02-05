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
#ifndef COBROVIEW_H
#define COBROVIEW_H

// PRESUPUESTOS.

#include "cobrobase.h"
#include "postgresiface2.h"
#include "cobro.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "dialogchanges.h"

#include <qlineedit.h>
#include <q3textedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

/** @author Tomeu Borr� Riera */
class company;

class CobroView : public CobroBase , public Cobro, public dialogChanges  {
    Q_OBJECT
public:
    CobroView(company *, QWidget *, const char *);
    ~CobroView();
    void inicialize();
    void manageArticle(int);
    void closeEvent( QCloseEvent *);

    void	pintafechacobro(QString id) {
        m_fechacobro->setText(id);
    };
    void	pintacomentcobro(QString id) {
        m_comentcobro->setText(id);
    };
    void    pintaidcliente(QString id) {
        m_cliente->setidcliente(id);
    };
    void    pintarefcobro(QString id) {
        m_refcobro->setText(id);
    };
    void    pintacantcobro(QString id) {
        m_cantcobro->setText(id);
    };
    void    pintaprevisioncobro(QString id) {
        if (id == "t" || id == "TRUE")
            m_previsioncobro->setChecked(TRUE);
        else
            m_previsioncobro->setChecked(FALSE);
    };


public slots:
    virtual void s_saveCobroView() {
        guardaCobro();
    };
    virtual void s_cargaCobroView(QString id) {
        Cobro::cargaCobro(id);
        setCaption("Cobro "+DBvalue("refcobro"));
    };
    virtual void s_removeCobroView();
    virtual void s_comentcobrotextChanged(const QString &str) {
        setcomentcobro(str);
    };
    virtual void s_refcobrotextChanged(const QString &str) {
        setrefcobro(str);
    };
    virtual void s_cantcobrotextChanged(const QString &str) {
        setcantcobro(str);
    };

    virtual void s_previsioncobrostateChanged(int i) {
        if (i)
            setprevisioncobro("TRUE");
        else
            setprevisioncobro("FALSE");
    }
    virtual void s_clientevalueChanged(QString id) {
        setidcliente(id);
    };
    virtual void s_fechacobrovalueChanged(QString id) {
        setfechacobro(id);
    };
};

#endif
