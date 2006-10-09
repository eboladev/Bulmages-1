
#include "diariosubform.h"
#include <QMenu>




DiarioSubForm::DiarioSubForm(QWidget *parent, const char *) : SubForm2Bc(parent) {
    setDBTableName("borrador");
    setDBCampoId("idborrador");
    addSHeader("idborrador", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("idborrador"));
    addSHeader("codigoborrador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigoborrador"));
    addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idasiento"));
    addSHeader("iddiario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("iddiario"));
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Concepto"));
    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcuenta"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigo"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("tipocuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("descripcion"));
    addSHeader("debe", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Debe"));
    addSHeader("haber", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Haber"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Contrapartida"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoWrite, tr("Comentario"));
    addSHeader("idcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView , tr("idcanal"));
    addSHeader("nombrecanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("MarcaConciliacion"));
    addSHeader("idc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("idc_coste"));
    addSHeader("nombrec_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Centro Coste"));
    addSHeader("idapunte", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idapunte"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idtipoiva"));
    addSHeader("idregistroiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, tr("idregistroiva"));
    addSHeader("factura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Factura"));
    addSHeader("orden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("orden"));
    setinsercion(FALSE);
}


void DiarioSubForm::contextMenuEvent (QContextMenuEvent *) {
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



    if (opcion == mostextractodia)
	boton_extracto1(0);
    if (opcion == mostextractomes)
	boton_extracto1(1);
    if (opcion == mostextractoano)
	boton_extracto1(2);



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


