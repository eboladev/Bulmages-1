//
// C++ Implementation: ListDescuentoAlbaranProvView
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
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "listdescalbaranprovview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListDescuentoAlbaranProvView::ListDescuentoAlbaranProvView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("dalbaranp");
    setDBCampoId("iddalbaranp");
    addSHeader("iddalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "iddalbaranp");
    addSHeader("conceptdalbaranp", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "conceptdalbaranp");
    addSHeader("proporciondalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "proporciondalbaranp");
    addSHeader("idalbaranp", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idalbaranp");
    setinsercion(TRUE);
};