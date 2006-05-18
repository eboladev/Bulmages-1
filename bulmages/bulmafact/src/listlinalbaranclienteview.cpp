//
// C++ Implementation: ListLinAlbaranClienteView
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

#include "listlinalbaranclienteview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "fixed.h"


ListLinAlbaranClienteView::ListLinAlbaranClienteView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lalbaran");
    setDBCampoId("numlalbaran");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, "");
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, "");
    addSHeader("numlalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "");
    addSHeader("desclalbaran", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("cantlalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("pvplalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("ivalalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("descontlalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("idalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "");

    setinsercion(TRUE);
}




void ListLinAlbaranClienteView::on_mui_list_editFinished(int row, int col) {
    _depura("ListLinAlbaranClienteView::editFinished",0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='"+camp->text()+"'");
        if (!cur->eof() ) {
            rec->setDBvalue("idarticulo",cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclalbaran", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlalbaran", "1.00");
	    rec->setDBvalue("descontlalbaran","0.00");
	    rec->setDBvalue("pvplalbaran",cur->valor("pvparticulo"));
        }// end if
        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva="+cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
	    rec->setDBvalue("ivalalbaran",cur1->valor("porcentasa_iva"));		
        }// end if
	delete cur1;
	delete cur;
    }// end if
}


void ListLinAlbaranClienteView::cargar(QString idalbaran) {
        _depura("ListLinAlbaranClienteView::cargar\n",0);
        mdb_idalbaran = idalbaran;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran="+mdb_idalbaran);
        SubForm3::cargar(cur);
        delete cur;
}


Fixed ListLinAlbaranClienteView::calculabase() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplalbaran",i)) * Fixed(DBvalue("cantlalbaran",i));
		base = base + totpar;
        }// end for
	return base;
}


Fixed ListLinAlbaranClienteView::calculaiva() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplalbaran",i)) * Fixed(DBvalue("ivalalbaran",i));
		base = base + totpar;
        }// end for
	return base;
}


