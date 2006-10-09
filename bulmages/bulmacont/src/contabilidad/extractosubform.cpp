
#include "extractosubform.h"
#include <QMenu>




ExtractoSubForm::ExtractoSubForm(QWidget *parent, const char *) : SubForm2Bc(parent) {
    setDBTableName("apunte");
    setFileConfig( "ExtractoSubForm");
    setDBCampoId("idapunte");
    addSHeader("punteo", DBCampo::DBboolean, DBCampo::DBNotNull,SHeader::DBNone, tr("P"));
    addSHeader("idapunte", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("P"));
    addSHeader("codigoborrador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idapunte"));
    addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigoborrador"));
    addSHeader("iddiario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idasiento"));
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Concepto"));
    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcuenta"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigo"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("tipocuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Cuenta"));
    addSHeader("debe", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Debe"));
    addSHeader("haber", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Haber"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("contrapartida"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("comentario"));
    addSHeader("idcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcanal"));
    addSHeader("nombrecanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("marcaconciliacion"));
    addSHeader("idc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idc_coste"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idtipoiva"));
    addSHeader("orden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("orden"));
    setinsercion(FALSE);
}


void ExtractoSubForm::contextMenuEvent (QContextMenuEvent *) {
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
    QAction *mostdiariodia = popup->addAction("Mostrar Diario (dia)");
    QAction *mostdiariomes = popup->addAction("Mostrar Diario (mes)");
    QAction *mostdiarioano = popup->addAction("Mostrar Diario (ano)");
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

    if (opcion == mostapunte)
	boton_asiento();

    if (opcion == del)
        borrar(row);

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

    if (opcion == mostdiariodia)
	boton_diario1(0);
    if (opcion == mostdiariomes)
	boton_diario1(1);
    if (opcion == mostdiarioano)
	boton_diario1(2);

    if (opcion == mostbalancedia)
	boton_balance1(0);
    if (opcion == mostbalancemes)
	boton_balance1(1);
    if (opcion == mostbalanceano)
	boton_balance1(2);


    if (opcion == mostbalancejdia)
	boton_balancetree(0);
    if (opcion == mostbalancejmes)
	boton_balancetree(1);
    if (opcion == mostbalancejano)
	boton_balancetree(2);


    delete popup;
}


