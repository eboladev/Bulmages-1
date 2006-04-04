//
// C++ Implementation: ListLinFacturaProveedorView
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

#include "listlinfacturapview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "fixed.h"


ListLinFacturaProveedorView::ListLinFacturaProveedorView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lfacturap");
    setDBCampoId("idlfacturap");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, "");
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, "");
    addSHeader("idlfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "");
    addSHeader("desclfacturap", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("cantlfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("pvplfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("ivalfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("descuentolfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "");
    addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "");
    setinsercion(TRUE);
}




void ListLinFacturaProveedorView::editFinished(int row, int col) {
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
            rec->setDBvalue("desclfacturap", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlfacturap", "1.00");
	    rec->setDBvalue("descuentolfacturap","0.00");
	    rec->setDBvalue("pvplfacturap",cur->valor("pvparticulo"));
        }// end if
        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva="+cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
	    rec->setDBvalue("ivalfacturap",cur1->valor("porcentasa_iva"));		
        }// end if
	delete cur1;
	delete cur;
    }// end if
}


void ListLinFacturaProveedorView::cargar(QString idfacturap) {
        _depura("ListLinFacturaProveedorView::cargar\n",0);
        mdb_idfacturap = idfacturap;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM lfacturap LEFT JOIN articulo ON lfacturap.idarticulo = articulo.idarticulo WHERE idfacturap="+mdb_idfacturap);
        SubForm2::cargar(cur);
        delete cur;
}


Fixed ListLinFacturaProveedorView::calculabase() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplfacturap",i)) * Fixed(DBvalue("cantlfacturap",i));
		base = base + totpar;
        }// end for
	return base;
}


Fixed ListLinFacturaProveedorView::calculaiva() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplfacturap",i)) * Fixed(DBvalue("ivalfacturap",i));
		base = base + totpar;
        }// end for
	return base;
}

