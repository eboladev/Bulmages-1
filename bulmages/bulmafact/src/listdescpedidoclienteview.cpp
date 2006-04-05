//
// C++ Implementation: ListDescuentoPedidoClienteView
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

#include "listdescpedidoclienteview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListDescuentoPedidoClienteView::ListDescuentoPedidoClienteView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("dpedidocliente");
    setDBCampoId("iddpedidocliente");
    addSHeader("iddpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "iddpedidocliente");
    addSHeader("conceptdpedidocliente", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "conceptdpedidocliente");
    addSHeader("proporciondpedidocliente", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "proporciondpedidocliente");
    addSHeader("idpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idpedidocliente");
    setinsercion(TRUE);
};