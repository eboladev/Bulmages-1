
#include "extractosubform.h"
#include <QMenu>




ExtractoSubForm::ExtractoSubForm(QWidget *parent, const char *) : SubForm2Bc(parent) {
    setDBTableName("apunte");
    setDBCampoId("idapunte");
    addSHeader("punteo", DBCampo::DBboolean, DBCampo::DBNotNull,SHeader::DBNone, tr("P"));
    addSHeader("idapunte", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("codigoborrador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de cliente"));
    addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre alternativo del cliente"));
    addSHeader("iddiario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("C.I.F."));
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero cuenta corriente"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion"));
    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcuenta"));
    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcuenta"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigo"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("tipocuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo postal"));
    addSHeader("debe", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de telefono"));
    addSHeader("haber", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de fax"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion de correo electronica"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion URL"));
    addSHeader("idcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de alta del cliente"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de baja del cliente"));
    addSHeader("idc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("idapunte", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("orden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
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


