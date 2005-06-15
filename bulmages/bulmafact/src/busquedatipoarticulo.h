//
// C++ Interface: busquedafamilia
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUSQUEDATIPOARTICULO_H
#define BUSQUEDATIPOARTICULO_H

#include <busquedatipoarticulobase.h>
#include <qlineedit.h>
#include <qlabel.h>

class company;
/**
@author Tomeu Borras
*/
class BusquedaTipoArticulo : public BusquedaTipoArticuloBase
{
Q_OBJECT
private:
company *companyact;
QString mdb_idtipo_articulo;
QString mdb_desctipo_articulo;
QString mdb_codtipo_articulo;
public:
    BusquedaTipoArticulo(QWidget *parent = 0, const char *name = 0);
    ~BusquedaTipoArticulo();
    void setcompany(company *comp) {companyact = comp;};
    virtual QString codtipo_articulo() {return m_codtipo_articulo->text();};
    virtual QString idtipo_articulo() {return mdb_idtipo_articulo;};
    virtual QString desctipo_articulo() {return desctipo_articulo();};
    virtual void setidtipo_articulo(QString val);
    virtual void setcodtipo_articulo(QString val);
public slots:
	virtual void s_searchTipoArticulo();
	virtual void s_codtipo_articulotextChanged(const QString &);
signals:
	void valueChanged(QString );
};

#endif
