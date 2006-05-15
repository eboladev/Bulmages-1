//
// C++ Implementation: ListDescuentoPedidoProveedorView
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

#include "listdescpedidoproveedorview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListDescuentoPedidoProveedorView::ListDescuentoPedidoProveedorView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("dpedidoproveedor");
    setDBCampoId("iddpedidoproveedor");
    addSHeader("iddpedidoproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "iddpedidoproveedor");
    addSHeader("conceptdpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "conceptdpedidoproveedor");
    addSHeader("proporciondpedidoproveedor", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "proporciondpedidoproveedor");
    addSHeader("idpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idpedidoproveedor");
    setinsercion(TRUE);
};

