//
// C++ Interface: inventario
// Description:
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "listcontrolstock.h"
#include "company.h"
#include "controlstock.h"
#include "funcaux.h"
#include "dbrecord.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de facturap de la base de datos y el programa.
*/


class Inventario : public DBRecord {
protected:
    ListControlStock *listalineas;

    company *companyact;




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
    virtual int cargaInventario(QString );
    void pintaInventario();
    void guardaInventario();
    void borraInventario();
    void imprimirInventario();

    virtual void pintaidinventario(QString) {};
    virtual void pintafechainventario(QString ) {};
    virtual void pintanominventario(QString ) {};
    virtual void pregenerar();

    void setidinventario(QString val) { setDBvalue("idinventario",val); listalineas->setidinventario(val);};
    void setfechainventario(QString val) { setDBvalue("fechainventario",val);};
    void setnominventario(QString val) {setDBvalue("nominventario",val);};

    void vaciaInventario();

};
#endif
