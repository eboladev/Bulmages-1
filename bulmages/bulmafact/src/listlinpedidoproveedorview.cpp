//
// C++ Implementation: ListLinPedidoProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <Q3Table>
#include <QMessageBox>
#include <Q3PopupMenu>
#include <QKeyEvent>
#include <QEvent>

#include "articulolist.h"
#include "listlinpedidoproveedorview.h"
#include "funcaux.h"
#include "fixed.h"


ListLinPedidoProveedorView::ListLinPedidoProveedorView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lpedidoproveedor");
    setDBCampoId("numlpedidoproveedor");
    addSHeader("puntlpedidoproveedor", DBCampo::DBboolean, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, "");
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, "");
    addSHeader("numlpedidoproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "");
    addSHeader("desclpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("cantlpedidoproveedor", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("pvplpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("ivalpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("descuentolpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("idpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "");

    setinsercion(TRUE);
};




void ListLinPedidoProveedorView::editFinished(int row, int col) {
    _depura("ListLinPedidoProveedorView::editFinished",0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='"+camp->text()+"'");
        if (!cur->eof() ) {
            rec->setDBvalue("idarticulo",cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclpedidoproveedor", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlpedidoproveedor", "1.00");
	    rec->setDBvalue("descuentolpedidoproveedor","0.00");
	    rec->setDBvalue("pvplpedidoproveedor",cur->valor("pvparticulo"));
		
        }// end if

        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva="+cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
	    rec->setDBvalue("ivalpedidoproveedor",cur1->valor("porcentasa_iva"));		
        }// end if
	delete cur1;
	delete cur;

    }// end if
};


