//
// C++ Implementation: ListLinPedidoClienteView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

/*
CREATE TABLE lpedidocliente (
   numlpedidocliente serial PRIMARY KEY,
   desclpedidocliente character varying(150),
   cantlpedidocliente float,
   pvplpedidocliente float,
   prevlpedidocliente date,
   ivalpedidocliente numeric(5,2),
   descuentolpedidocliente float,   
   idpedidocliente integer NOT NULL REFERENCES pedidocliente(idpedidocliente),
   idarticulo integer REFERENCES articulo(idarticulo)
);
*/

#include <QMessageBox>
#include <Q3PopupMenu>
#include <QKeyEvent>
#include <QEvent>

#include "articulolist.h"
#include "listlinpedidoclienteview.h"
#include "funcaux.h"
#include "fixed.h"


ListLinPedidoClienteView::ListLinPedidoClienteView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lpedidocliente");
    setDBCampoId("numlpedidocliente");
    addSHeader("puntlpedidocliente", DBCampo::DBboolean, DBCampo::DBNotNull, SHeader::DBNone, "puntlpedidocliente");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idarticulo");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, "codigocompletoarticulo");
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, "nomarticulo");
    addSHeader("numlpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "numlpedidocliente");
    addSHeader("desclpedidocliente", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "desclpedidocliente");
    addSHeader("cantlpedidocliente", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "cantlpedidocliente");
    addSHeader("pvplpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "pvplpedidocliente");
    addSHeader("ivalpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "ivalpedidocliente");
    addSHeader("descuentolpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "descuentolpedidocliente");
    addSHeader("idpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idpedidocliente");

    setinsercion(TRUE);
};




void ListLinPedidoClienteView::editFinished(int row, int col) {
    _depura("ListLinPedidoClienteView::editFinished",0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='"+camp->text()+"'");
        if (!cur->eof() ) {
            rec->setDBvalue("idarticulo",cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclpedidocliente", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlpedidocliente", "1.00");
	    rec->setDBvalue("descuentolpedidocliente","0.00");
	    rec->setDBvalue("pvplpedidocliente",cur->valor("pvparticulo"));
		
        }// end if

        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva="+cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
	    rec->setDBvalue("ivalpedidocliente",cur1->valor("porcentasa_iva"));		
        }// end if
	delete cur1;
	delete cur;

    }// end if
};



void ListLinPedidoClienteView::cargar(QString idpedidocliente) {
        _depura("ListLinPedidoClienteView::cargar\n",0);
        mdb_idpedidocliente = idpedidocliente;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM lpedidocliente LEFT JOIN articulo ON lpedidocliente.idarticulo = articulo.idarticulo WHERE idpedidocliente="+mdb_idpedidocliente);
        SubForm3::cargar(cur);
        delete cur;
}

Fixed ListLinPedidoClienteView::calculabase() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplpedidocliente",i)) * Fixed(DBvalue("cantlpedidocliente",i));
		base = base + totpar;
        }// end for
	return base;
}


Fixed ListLinPedidoClienteView::calculaiva() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplpedidocliente",i)) * Fixed(DBvalue("ivalpedidocliente",i));
		base = base + totpar;
        }// end for
	return base;
}

