//
// C++ Interface: facturap
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INVENTARIOVIEW_H
#define INVENTARIOVIEW_H

#include "inventariobase.h"
#include "inventario.h"
#include "busquedafecha.h"

#include <qlineedit.h>
#include <q3textedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

#include "postgresiface2.h"

/** @author Tomeu Borras */

class InventarioView : public InventarioBase, public Inventario {
    Q_OBJECT

public:
    InventarioView(company *, QWidget *parent = 0, const char *name = 0);
    ~InventarioView();

    void	 pintaidinventario(QString) {}
    ;
    void    pintafechainventario(QString id) {
        m_fechainventario->setText(id);
    };
    void    pintanominventario(QString id) {
        m_nominventario->setText(id);
    };

public slots:
    virtual void s_fechainventariovalueChanged(QString val) {
        setfechainventario(val);
    };
    virtual void s_nominventariotextChanged(const QString &val) {
        setnominventario(val);
    };
   virtual void s_save() {
	guardaInventario();
   }

   virtual void s_delete();
   virtual void s_pregenerar() {pregenerar();};

};

#endif
