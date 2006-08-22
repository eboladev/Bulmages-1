#include "asientolistsubform.h"

#include <QMenu>

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
AsientoListSubform::AsientoListSubform(QWidget *parent, const char *) : SubForm2Bc(parent) {
    setDBTableName("asiento");
    setFileConfig("AsientoListSubform");
    setDBCampoId("idasiento");
    addSHeader("idasiento", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("idasiento"));
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha"));
    addSHeader("ordenasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Orden"));
    addSHeader("numap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Apuntes"));
    addSHeader("numborr", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Borrador"));
    addSHeader("totaldebe", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Debe"));
    addSHeader("totalhaber", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Haber"));
    addSHeader("comentariosasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("clase", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Clase"));
    setinsercion(FALSE);
}

void AsientoListSubform::contextMenuEvent (QContextMenuEvent *) {
    _depura("SubForm2Bc::contextMenuEvent",0);
    QAction *del= NULL;
    int row = currentRow();
    if ( row < 0)
        return;

    int col = currentColumn();
    if ( row < 0)
        return;

    QMenu *popup = new QMenu(this);

    QAction *mostapunte = popup->addAction("Mostrar Asiento");
    popup->addSeparator();
    QAction *mostextractodia = popup->addAction("Mostrar Extracto (dia)");
    QAction *mostextractomes = popup->addAction("Mostrar Extracto (mes)");
    QAction *mostextractoano = popup->addAction("Mostrar Extracto (ano)");
    popup->addSeparator();
    QAction *mostbalancedia = popup->addAction("Mostrar Balance (dia)");
    QAction *mostbalancemes = popup->addAction("Mostrar Balance (mes)");
    QAction *mostbalanceano = popup->addAction("Mostrar Balance (ano)");
    popup->addSeparator();
    QAction *mostbalancejdia = popup->addAction("Mostrar Balance Jerarquico (dia)");
    QAction *mostbalancejmes = popup->addAction("Mostrar Balance Jerarquico (mes)");
    QAction *mostbalancejano = popup->addAction("Mostrar Balance Jerarquico (ano)");


    if(m_delete)
        del = popup->addAction(tr("Borrar registro"));
    popup->addSeparator();
    QAction *ajustc = popup->addAction(tr("Ajustar columa"));
    QAction *ajustac = popup->addAction(tr("Ajustar altura"));

    QAction *ajust = popup->addAction(tr("Ajustar columnas"));
    QAction *ajusta = popup->addAction(tr("Ajustar alturas"));

    popup->addSeparator();
    QAction *verconfig = popup->addAction(tr("Ver configurador de subformulario"));

    QAction *opcion = popup->exec(QCursor::pos());

    if (opcion == ajust)
        resizeColumnsToContents();

    if (opcion == ajusta)
        resizeRowsToContents();

    if (opcion == ajustc)
        resizeColumnToContents(col);

    if (opcion == ajustac)
        resizeRowToContents(row);

    if(opcion == verconfig)
        showConfig();

    delete popup;
}


