//
// C++ Implementation: ListLTarifaView
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
#include <QKeyEvent>
#include <QEvent>
#include <QFile>
#include <QTextStream>
#include <QMenu>

#include "company.h"
#include "listltarifaview.h"
#include "funcaux.h"


ListLTarifaView::ListLTarifaView(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("ltarifa");
    setDBCampoId("idltarifa");

    addSHeader("nomtarifa", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomtarifa");
    addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomalmacen");
    addSHeader("pvpltarifa", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "cantcomparticulo");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idarticulo");
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idarticulo");
    addSHeader("idtarifa", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idarticulo");
    addSHeader("idltarifa", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "idarticulo");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, "codigocompletoarticulo");
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, "nomarticulo");
	setinsercion(FALSE);
}



ListLTarifaView1::ListLTarifaView1(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("ltarifa");
    setDBCampoId("idltarifa");
    addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite , "nomalmacen");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "codigocompletoarticulo");
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite , "nomarticulo");
    addSHeader("pvpltarifa", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "pvpltarifa");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idarticulo");
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idalmacen");
    addSHeader("idtarifa", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idtarifa");
    addSHeader("idltarifa", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "idltarifa");
    addSHeader("nomtarifa", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, "nomtarifa");

	setinsercion(FALSE);
}