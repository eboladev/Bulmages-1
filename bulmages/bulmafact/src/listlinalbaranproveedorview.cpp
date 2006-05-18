//
// C++ Implementation: ListLinAlbaranProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QMessageBox>
#include <Q3PopupMenu>
#include <QKeyEvent>
#include <QEvent>

#include "listlinalbaranproveedorview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "fixed.h"


ListLinAlbaranProveedorView::ListLinAlbaranProveedorView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lalbaranp");
    setDBCampoId("numlalbaranp");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("idarticulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("codigocompletoarticulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("nomarticulo"));
    addSHeader("numlalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("numlalbaranp"));
    addSHeader("desclalbaranp", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("desclalbaranp"));
    addSHeader("cantlalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("cantlalbaranp"));
    addSHeader("pvplalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("pvplalbaranp"));
    addSHeader("ivalalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("ivalalbaranp"));
    addSHeader("descontlalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("descontlalbaranp"));
    addSHeader("idalbaranp", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("idalbaranp"));
    setinsercion(TRUE);
}




void ListLinAlbaranProveedorView::on_mui_list_editFinished(int row, int col) {
    _depura("ListLinAlbaranProveedorView::editFinished",0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='"+camp->text()+"'");
        if (!cur->eof() ) {
            rec->setDBvalue("idarticulo",cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclalbaranp", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlalbaranp", "1.00");
	    rec->setDBvalue("descontlalbaranp","0.00");
	    rec->setDBvalue("pvplalbaranp",cur->valor("pvparticulo"));
        }// end if
        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva="+cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
	    rec->setDBvalue("ivalalbaranp",cur1->valor("porcentasa_iva"));		
        }// end if
	delete cur1;
	delete cur;
    }// end if
}


void ListLinAlbaranProveedorView::cargar(QString idalbaranp) {
        _depura("ListLinPedidoProveedorView::cargar\n",0);
        mdb_idalbaranp = idalbaranp;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM lalbaranp LEFT JOIN articulo ON lalbaranp.idarticulo = articulo.idarticulo WHERE idalbaranp="+mdb_idalbaranp);
        SubForm3::cargar(cur);
        delete cur;
}


Fixed ListLinAlbaranProveedorView::calculabase() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplalbaranp",i)) * Fixed(DBvalue("cantlalbaranp",i));
		base = base + totpar;
        }// end for
	return base;
}


Fixed ListLinAlbaranProveedorView::calculaiva() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplalbaranp",i)) * Fixed(DBvalue("ivalalbaranp",i));
		base = base + totpar;
        }// end for
	return base;
}

