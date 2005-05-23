//
// C++ Interface: presupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include <qstring.h>
#include <qtable.h>

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de presupuesto de la base de datos y el programa.
*/
#include "company.h"


class linpresupuesto {
public:
    QString mdb_idlpresupuesto;
    QString mdb_desclpresupuesto;
    QString mdb_cantlpresupuesto;
    QString mdb_pvplpresupuesto;
    QString mdb_descuentolpresupuesto;
    QString mdb_idpresupuesto;
    QString mdb_idarticulo;
    company *companyact;
    linpresupuesto(company *comp) {
        companyact = comp;
    };
    linpresupuesto(company *comp, QString idlinpresupuesto) {}
    ;
    linpresupuesto(company *comp, QString a, QString b, QString c, QString d, QString e, QString f, QString g) {
        mdb_idlpresupuesto = a;
        mdb_desclpresupuesto = b;
        mdb_cantlpresupuesto = c;
        mdb_pvplpresupuesto =d;
        mdb_descuentolpresupuesto = e;
        mdb_idpresupuesto = f;
        mdb_idarticulo = g;
    };
    virtual ~linpresupuesto() {}
    ;
    virtual void pintalinpresupuesto() {}
    ;
    void guardalinpresupuesto() {}
    ;
};


class listlinpresupuesto {
public:
    QPtrList<linpresupuesto> m_lista;
    company *companyact;
    listlinpresupuesto(company *comp) {
        companyact = comp;
    };
    virtual ~listlinpresupuesto() {}
    ;
    void cargalistlinpresupuesto() {}
    ;
    virtual void pintalistlinpresupuesto() {fprintf(stderr,"La función pintalistlinpresupuesto aun no ha sido implementada\n");}
    ;
    void chargeBudgetLines(QString);
};

#define COL_IDLPRESUPUESTO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPRESUPUESTO 4
#define COL_CANTLPRESUPUESTO 5
#define COL_PVPLPRESUPUESTO 6
#define COL_DESCUENTOLPRESUPUESTO 7
#define COL_IDPRESUPUESTO 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11


class listlinpresupuestoview : public QTable, public listlinpresupuesto {
public:
listlinpresupuestoview(company *comp,  QWidget * parent = 0, const char * name = 0) : QTable(parent, name) , listlinpresupuesto (comp) {

	/// Inicializamos la tabla de lineas de presupuesto
	setNumRows( 0 );
	setNumCols( 0 );
	setSelectionMode( QTable::SingleRow );
	setSorting( TRUE );
	setSelectionMode( QTable::SingleRow );
	setColumnMovingEnabled( TRUE );
	setNumCols(12);
	horizontalHeader()->setLabel( COL_IDLPRESUPUESTO, tr( "Nº Línea" ) );
	horizontalHeader()->setLabel( COL_DESCLPRESUPUESTO, tr( "Descripción" ) );
	horizontalHeader()->setLabel( COL_CANTLPRESUPUESTO, tr( "Cantidad" ) );
	horizontalHeader()->setLabel( COL_PVPLPRESUPUESTO, tr( "Precio" ) );
	horizontalHeader()->setLabel( COL_DESCUENTOLPRESUPUESTO, tr( "Descuento" ) );
	horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "Nº Pedido" ) );
	horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artículo" ) );
	horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código Artículo" ) );
	horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripción Artículo" ) );
	horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
	horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );
   
	setColumnWidth(COL_IDLPRESUPUESTO,100);
	setColumnWidth(COL_DESCLPRESUPUESTO,300);
	setColumnWidth(COL_CANTLPRESUPUESTO,100);
	setColumnWidth(COL_PVPLPRESUPUESTO,100);
	setColumnWidth(COL_DESCUENTOLPRESUPUESTO,100);
	setColumnWidth(COL_IDPRESUPUESTO,100);
	setColumnWidth(COL_IDARTICULO,100);
	setColumnWidth(COL_CODARTICULO,100);
	setColumnWidth(COL_NOMARTICULO,300);
	setColumnWidth(COL_TASATIPO_IVA,50);
	setColumnWidth(COL_TIPO_IVA,50);
	
	hideColumn(COL_IDLPRESUPUESTO);
	hideColumn(COL_IDPRESUPUESTO);
	hideColumn(COL_IDARTICULO);
	hideColumn(COL_REMOVE);
	hideColumn(COL_TASATIPO_IVA);
	hideColumn(COL_TIPO_IVA);
	
	setNumRows(10);
	
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
	setColumnReadOnly(COL_NOMARTICULO,true);
	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	setPaletteBackgroundColor("#AFFAFA");   
	setReadOnly(FALSE);
};
    ~listlinpresupuestoview() {}
    ;
    void pintalistlinpresupuesto() {
    	fprintf(stderr,"INICIO de pintalistlinpresupuesto\n");

        linpresupuesto *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            setText(i++, 1,"hola mundo");
	    fprintf(stderr,"hola mundo\n");
        }// end for
	setText(3,3,"una prueba más fina");
	fprintf(stderr,"FIN de pintalistlinpresupuesto\n");

    };
};


class presupuesto {
protected:
    listlinpresupuesto *listalineas;

    company *companyact;
    QString m_idpresupuesto;
    QString m_idclient;
    QString m_idalmacen;
    QString m_initialValues;
    cursor2 *m_cursorcombo;

    QString mdb_idcliente;
    QString mdb_idalmacen;
    QString mdb_numpresupuesto;
    QString mdb_fpresupuesto;
    QString mdb_vencpresupuesto;
    QString mdb_contactpresupuesto;
    QString mdb_telpresupuesto;
    QString mdb_comentpresupuesto;
    QString mdb_nomcliente;
    QString mdb_cifcliente;
    QString mdb_codigoalmacen;
    QString mdb_nomalmacen;
    QString mdb_idpresupuesto;

public:
    // Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setlislinpresupuesto ( listlinpresupuesto *a) {
        listalineas =a;
    };
    presupuesto(company *);
    virtual ~presupuesto();
    void chargeBudget(QString );
    void pintaPresupuesto();
    virtual void	pintaIdClient(QString id) {}
    ;
    virtual void	pintaIdAlmacen(QString id) {}
    ;
    virtual void	pintaNumPresupuesto(QString id) {}
    ;
    virtual void	pintaFPresupuesto(QString id) {}
    ;
    virtual void	pintaVencPresupuesto(QString id) {}
    ;
    virtual void	pintaContractPresupuesto(QString id) {}
    ;
    virtual void	pintaTelPresupuesto(QString id) {}
    ;
    virtual void	pintaComentPresupuesto(QString id) {}
    ;
    virtual void	pintaNomClient(QString id) {}
    ;
    virtual void	pintaCifClient(QString id) {}
    ;
    virtual void	pintaCodigoAlmacen(QString id) {}
    ;
    virtual void	pintaNomAlmacen(QString id) {}
    ;



    virtual void chargeBudgetDiscounts(QString) {}
    ;
    virtual void calculateImports() {}
    ;
    virtual     void inicialize() {}
    ;
    virtual     QString calculateValues() {}
    ;
};
#endif
