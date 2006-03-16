//
// C++ Interface: ListLTarifaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLTARIFAVIEW_H
#define LISTLTARIFAVIEW_H

/**
@author Tomeu Borras
*/


#include <QEvent>
#include <QTableWidget>

#include "qtable2.h"
#include "subform2bf.h"




class ListLTarifaView : public SubForm2Bf {
Q_OBJECT
public:
	QString mdb_idarticulo;
	ListLTarifaView(QWidget *parent = 0, const char *name = 0);
	~ListLTarifaView() {};

public slots:
virtual void cargar(QString idarticulo) {
    _depura("ListCompArticulo::cargaListCompArticulo\n",0);

	mdb_idarticulo=idarticulo;

    QString SQLQuery = "SELECT * FROM (SELECT * FROM almacen, tarifa, (SELECT * FROM articulo WHERE idarticulo = "+mdb_idarticulo+") AS t2) AS t3 ";
	    SQLQuery+= " LEFT JOIN (SELECT * FROM ltarifa WHERE idarticulo="+mdb_idarticulo+") as t1 ON t1.idtarifa=t3.idtarifa AND t1.idalmacen=t3.idalmacen ";

    cursor2 * cur= companyact()->cargacursor(SQLQuery);
	SubForm2Bf::cargar(cur);
    delete cur;
};



};




class ListLTarifaView1 : public SubForm2Bf {
Q_OBJECT
public:
	ListLTarifaView1(QWidget *parent = 0, const char *name = 0);
	~ListLTarifaView1() {};

public slots:


virtual void cargar(QString SQLQuery) {
    _depura("ListCompArticulo::cargar\n",0);
	_depura(SQLQuery,0);
    cursor2 * cur= companyact()->cargacursor(SQLQuery);
	if (!cur->error())
	SubForm2Bf::cargar(cur);
    delete cur;
};



};


/// OJO ESta clase está definida aqui pero es el lanzador del plugin para las entradas de menu del plugin.
#include "bulmafact.h"
class myplugin1 : public QObject {
    Q_OBJECT
public:
	bulmafact *m_bges;
    company *m_companyact;
public:
    myplugin1();
    ~myplugin1();
    void inicializa(bulmafact *);
public slots:
    void elslot();
};


#endif



