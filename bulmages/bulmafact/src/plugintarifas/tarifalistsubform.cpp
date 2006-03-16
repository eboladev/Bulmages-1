//
// C++ Implementation: TarifaListSubform
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
#include "tarifalistsubform.h"
#include "funcaux.h"





TarifaListSubform::TarifaListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("tarifa");
    setDBCampoId("idtarifa");
    addSHeader("nomtarifa", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomarticulo");
    addSHeader("idtarifa", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, "idtarifa");
	setinsercion(FALSE);
};
