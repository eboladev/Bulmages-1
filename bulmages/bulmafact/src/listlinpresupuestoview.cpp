//
// C++ Implementation: listlinpresupuestoview
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

#include "articulolist.h"
#include "listlinpresupuestoview.h"
#include "funcaux.h"
#include "fixed.h"


listlinpresupuestoview::listlinpresupuestoview(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lpresupuesto");
    setDBCampoId("idlpresupuesto");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idarticulo");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, "codigocompletoarticulo");
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, "nomarticulo");
    addSHeader("idlpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "idlpresupuesto");
    addSHeader("desclpresupuesto", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "desclpresupuesto");
    addSHeader("cantlpresupuesto", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "cantlpresupuesto");
    addSHeader("pvplpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "pvplpresupuesto");
    addSHeader("ivalpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "ivalpresupuesto");
    addSHeader("descuentolpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, "descuentolpresupuesto");
    addSHeader("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idpresupuesto");
    setinsercion(TRUE);
}




void listlinpresupuestoview::on_mui_list_editFinished(int row, int col) {
    _depura("listlinpresupuestoview::editFinished",0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='"+camp->text()+"'");
        if (!cur->eof() ) {
            rec->setDBvalue("idarticulo",cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclpresupuesto", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlpresupuesto", "1.00");
	    rec->setDBvalue("descuentolpresupuesto","0.00");
	    rec->setDBvalue("pvplpresupuesto",cur->valor("pvparticulo"));
        }// end if

        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva="+cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
	    rec->setDBvalue("ivalpresupuesto",cur1->valor("porcentasa_iva"));		
        }// end if
	delete cur1;
	delete cur;

    }// end if
    _depura("END listlinpresupuestoview::editFinished",0);
}



void listlinpresupuestoview::cargar(QString idpresupuesto) {
        _depura("listlinpresupuestoview::cargar\n",0);
        mdb_idpresupuesto = idpresupuesto;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE idpresupuesto="+mdb_idpresupuesto);
        SubForm3::cargar(cur);
        delete cur;
}

Fixed listlinpresupuestoview::calculabase() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplpresupuesto",i)) * Fixed(DBvalue("cantlpresupuesto",i));
		base = base + totpar;
        }// end for
	return base;
}


Fixed listlinpresupuestoview::calculaiva() {
	Fixed base("0.0");
        for (int i=0; i < rowCount()-1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplpresupuesto",i)) * Fixed(DBvalue("ivalpresupuesto",i));
		base = base + totpar;
        }// end for
	return base;
}


