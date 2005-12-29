//
// C++ Interface: inventario
// Description:
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <qstring.h>
#include <q3table.h>
#include <q3ptrlist.h>

#include "listcontrolstock.h"
#include "company.h"
#include "controlstock.h"
#include "funcaux.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de facturap de la base de datos y el programa.
*/


class Inventario {
protected:
    ListControlStock *listalineas;

    company *companyact;

    QString mdb_idinventario;
    QString mdb_nominventario;
    QString mdb_fechainventario;


public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.

    void setListControlStock ( ListControlStock *a) {
	_depura("Inventario::setListControlStock",0);
        listalineas =a;
	listalineas->setcompany(companyact);
    };
    ListControlStock* getlistalineas() {return listalineas;};

    Inventario(company *);
    virtual ~Inventario();
    virtual void cargaInventario(QString );
    void pintaInventario();
    void guardaInventario();
    void borraInventario();
    void imprimirInventario();

    virtual void pintaidinventario(QString) {};
    virtual void pintafechainventario(QString ) {};
    virtual void pintanominventario(QString ) {};

    void setidinventario(QString val) { mdb_idinventario=val; listalineas->setidinventario(val);};
    void setfechainventario(QString val) { mdb_fechainventario=val;};
    void setnominventario(QString val) {mdb_nominventario=val;};

    void vaciaInventario();

};
#endif
