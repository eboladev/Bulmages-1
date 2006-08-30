//
// C++ Implementation: ListLinAsiento1View
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "listlinasiento1view.h"
#include <QMenu>

#include "plugins.h"


// Construye la cabecera del asiento (nombra cada columna de la tabla)
ListLinAsiento1View::ListLinAsiento1View(QWidget *parent, const char *) : SubForm2Bc(parent) {
    _depura("ListLinAsiento1View::ListLinAsiento1View", 0);
    setDBTableName("borrador");
    setDBCampoId("idborrador");
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone , tr("Fecha"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone , tr("Concepto Contable"));
    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite | SHeader::DBNoView, tr("idcuenta"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("codigo"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("tipocuenta"));
    addSHeader("descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("descripcioncuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone , tr("descripcion"));
    addSHeader("debe", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("Debe"));
    addSHeader("haber", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("Haber"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("Contrapartida"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("Comentario"));
    addSHeader("idcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("ID Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("Conciliacion"));
    addSHeader("idc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("idc_coste"));
    addSHeader("idapunte", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("idapunte"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("idtipoiva"));
    addSHeader("orden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("orden"));
    addSHeader("idborrador", DBCampo::DBint,  DBCampo::DBPrimaryKey, SHeader::DBNoView , tr("idborrador"));
    addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone , tr("idasiento"));
    setinsercion(TRUE);
    _depura("END ListLinAsiento1View::ListLinAsiento1View", 0);
}


// Fabrica el menu contextual
void ListLinAsiento1View::contextMenuEvent (QContextMenuEvent *) {
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


/// Carga lineas de asiento (apuntes)
void ListLinAsiento1View::cargar(QString idasiento) {
    _depura("AsientoSubForm::cargar", 0);
    QString SQLQuery = "SELECT * FROM borrador ";
    SQLQuery+= " LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta  FROM  cuenta) AS t1 ON t1.idcuenta=borrador.idcuenta ";
    SQLQuery += " LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal ";
    SQLQuery += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste ";
    SQLQuery += " LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador  FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador ";
    SQLQuery+= "WHERE idasiento="+idasiento+" ORDER BY orden";
    cursor2 * cur= m_companyact->cargacursor(SQLQuery);
    SubForm2Bc::cargar(cur);
    delete cur;
    _depura("END AsientoSubForm::cargar", 0);
}


void ListLinAsiento1View::boton_iva() {
    _depura("ListLinAsiento1View::boton_iva", 0);
    int res = g_plugins->lanza("ListLinAsiento1View_boton_iva", this);
    _depura("END ListLinAsiento1View::boton_iva", 0);

}


// Devuelve el DEBE de un asiento
Fixed ListLinAsiento1View::totaldebe(QString idasiento) {
    QString SQLQuery = "SELECT  sum(debe) FROM borrador LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta  FROM  cuenta) AS t1 ON t1.idcuenta=borrador.idcuenta LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador  FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador WHERE idasiento="+idasiento;
    cursor2 * cur= m_companyact->cargacursor(SQLQuery);
    QString resultado = cur->valor(0);
    delete cur;
    return Fixed(resultado);
}


// Devuelve el HABER de un asiento
Fixed ListLinAsiento1View::totalhaber(QString idasiento) {
    QString SQLQuery = "SELECT  sum(haber) FROM borrador LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta  FROM  cuenta) AS t1 ON t1.idcuenta=borrador.idcuenta LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador  FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador WHERE idasiento="+idasiento;
    cursor2 * cur= m_companyact->cargacursor(SQLQuery);
    QString resultado = cur->valor(0);
    delete cur;
    return Fixed(resultado);
}


/*


void ListLinAsiento1View::guardaconfig() {
    _depura("ListLinAsiento1View::guardaconfig",0);
    QString aux = "";
    QFile file( confpr->valor(CONF_DIR_USER)+"conflistlinasiento1view.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            showColumn(i);
            stream << columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()


void ListLinAsiento1View::cargaconfig() {
    _depura("ListLinAsiento1View::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"conflistlinasiento1view.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            QString linea = stream.readLine();
            setColumnWidth(i, linea.toInt());
        }// end for
        file.close();
    }
}// end cargaconfig


ListLinAsiento1View::ListLinAsiento1View(QWidget * parent, const char * name) : Q3Table(parent, name), ListLinAsiento1() {
    /// Inicializamos la tabla de lineas de Borrador
    setNumCols(17);
    setNumRows(10000);
    horizontalHeader()->setLabel( COL_FECHA, tr( "COL_FECHA" ) );
    horizontalHeader()->setLabel( COL_CODIGO, tr( "COL_CODIGO" ) );
    horizontalHeader()->setLabel( COL_CONCEPTO, tr( "COL_CONCEPTO" ) );
    horizontalHeader()->setLabel( COL_DEBE, tr( "COL_DEBE" ) );
    horizontalHeader()->setLabel( COL_HABER, tr( "COL_HABER" ) );
    horizontalHeader()->setLabel( COL_NOMCUENTA, tr( "COL_NOMCUENTA" ) );
    horizontalHeader()->setLabel( COL_CONTRAPARTIDA, tr( "COL_CONTRAPARTIDA" ) );
    horizontalHeader()->setLabel( COL_IDBORRADOR, tr( "COL_IDBORRADOR" ) );
    horizontalHeader()->setLabel( COL_IDREGISTROIVA, tr( "COL_IDREGISTROIVA" ) );
    horizontalHeader()->setLabel( COL_IVA, tr( "COL_IVA" ) );
    horizontalHeader()->setLabel( COL_NOMBREC_COSTE, tr( "COL_NOMBREC_COSTE" ) );
    horizontalHeader()->setLabel( COL_NOMBRECANAL, tr( "COL_NOMBRECANAL" ) );
    horizontalHeader()->setLabel( COL_IDCUENTA, tr( "COL_IDCUENTA" ) );
    horizontalHeader()->setLabel( COL_IDCONTRAPARTIDA, tr( "COL_IDCONTRAPARTIDA" ) );
    horizontalHeader()->setLabel( COL_IDCANAL, tr( "COL_IDCANAL" ) );
    horizontalHeader()->setLabel( COL_IDCCOSTE, tr( "COL_IDCCOSTE" ) );
    horizontalHeader()->setLabel( COL_ORDEN, tr( "COL_ORDEN" ) );
    setSelectionMode( Q3Table::SingleRow );
    setColumnReadOnly(COL_NOMCUENTA,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor("#DDAAAA");
    setRowMovingEnabled( TRUE );
    setColumnMovingEnabled( TRUE );
    setSorting( TRUE );
    setSelectionMode( Q3Table::SingleRow );

    QFont tapunts_font(  font() );
    tapunts_font.setPointSize(atoi(confpr->valor(CONF_FONTSIZE_APUNTES)));
    tapunts_font.setFamily(confpr->valor(CONF_FONTFAMILY_APUNTES));
    setFont( tapunts_font );

    hideColumn(COL_IDBORRADOR);
    hideColumn(COL_IDREGISTROIVA);
    hideColumn(COL_IDCUENTA);
    hideColumn(COL_IDCONTRAPARTIDA);
    hideColumn(COL_IDCANAL);
    hideColumn(COL_IDCCOSTE);
    hideColumn(COL_ORDEN);

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));
    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
    cargaconfig();
}// ListLinAsiento1View


ListLinAsiento1View::~ListLinAsiento1View() {
    guardaconfig();
}


void ListLinAsiento1View::pintaListLinAsiento1() {
    _depura("INICIO de pintaListLinAsiento1\n",0);
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LinAsiento1 *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        pintalinListLinAsiento1(i);
        adjustRow(i);
        i++;
    }// end for
    _depura("FIN de pintaListLinAsiento1\n",0);
}



void ListLinAsiento1View::contextMenu ( int row, int col, const QPoint & pos ) {
    _depura("ListLinAsiento1View::contextMenu",0);
    if (companyact->intapuntsempresa()->estadoAsiento1() == Asiento1::ASCerrado) {
        contextMenuCerrado(row, col, pos);
        return;
    }
    LinAsiento1 *linea = lineaact();
    Q3PopupMenu *popup;
    QString query;
    int opcion;
    cursor2 *cur;


    /// Confeccionamos el menu dependiendo del estado y de la casilla pulsada.
    popup = new Q3PopupMenu;
    Q3PopupMenu *menucanal = new Q3PopupMenu( this );
    Q3PopupMenu *menucoste = new Q3PopupMenu( this );
    popup->insertItem(tr("Borrar Linea"),1);
    popup->insertItem(tr("Subir"),2);
    popup->insertItem(tr("Bajar"),3);
    popup->insertItem(tr("Igual que la anterior (*)"),4);

    switch (col) {
    case COL_NOMBRECANAL:
        query = "SELECT * FROM canal";
        cur = companyact->cargacursor(query,"canales");
        menucanal->insertItem(tr("Ninguno"), 1000);
        while (!cur->eof()) {
            menucanal->insertItem(cur->valor("nombre"),1000+atoi(cur->valor("idcanal")));
            cur->siguienteregistro();
        }// end while
        delete cur;
        popup->insertItem(tr("&Seleccionar Canal"),menucanal);
        break;

    case COL_NOMBREC_COSTE:
        query = "SELECT * FROM c_coste";
        cur = companyact->cargacursor(query,"canales");
        menucoste->insertItem(tr("Ninguno"), 1100);
        while (!cur->eof()) {
            menucoste->insertItem(cur->valor("nombre"),1100+atoi(cur->valor("idc_coste")));
            cur->siguienteregistro();
        }// end while
        delete cur;
        popup->insertItem(tr("&Seleccionar Centro Coste"),menucoste);
        break;

    }// end switch
    opcion = popup->exec(pos);

    /// Dependiendo de la opcion que hayamos seleccionado actuamos.
    switch(opcion) {
    case 1:
        borraLinAsiento1(row);
        break;
    case 2:
        if (row > 0) {
            LinAsiento1 *linea1= m_lista.take(row-1);
            LinAsiento1 *linea = m_lista.take(row-1);
            m_lista.insert(row-1, linea1);
            m_lista.insert(row-1, linea);
            pintaListLinAsiento1();
        }// end if
        break;
    case 3:
        if (row < (int)m_lista.count()-1) {
            LinAsiento1 *linea1= m_lista.take(row);
            LinAsiento1 *linea = m_lista.take(row);
            m_lista.insert(row, linea1);
            m_lista.insert(row, linea);
            pintaListLinAsiento1();
        }// end if
        break;

    default:
        switch(col) {
        case COL_NOMBRECANAL:
            if (opcion == 1000) {
                linea->setidcanal("");
            }// end if
            if (opcion > 1000) {
                QString query1;
                opcion -= 1000;
                query1.sprintf("SELECT * FROM canal WHERE idcanal=%d", opcion);
                cursor2 *cur = companyact->cargacursor(query1);
                if (!cur->eof()) {
                    linea->setidcanal(cur->valor("idcanal"));
                }// end if
                delete cur;
            }// end if
            pintalinListLinAsiento1(row);
            break;
        case COL_NOMBREC_COSTE:
            if (opcion == 1100) {
                linea->setidcanal("");
            }// end if
            if (opcion > 1100) {
                QString query1;
                opcion -= 1100;
                query1.sprintf("SELECT * FROM c_coste WHERE idc_coste=%d", opcion);
                cursor2 *cur = companyact->cargacursor(query1);
                if (!cur->eof()) {
                    linea->setidc_coste(cur->valor("idc_coste"));
                }// end if
                delete cur;
            }// end if
            pintalinListLinAsiento1(row);
            break;
        }// end switch
        break;
    }// end switch
    delete popup;
    delete menucoste;
    delete menucanal;
}// end contextMenuRequested



void ListLinAsiento1View::contextMenuCerrado ( int , int col, const QPoint & pos ) {
    LinAsiento1 *linea = lineaact();
    Q3PopupMenu *popup;
    QString query;
    int opcion;

    // Si el asiento esta cerrado el menú a mostrar es diferente
    popup = new Q3PopupMenu;
    popup->insertItem(tr("Ver Diario (Este día)"),101);
    popup->insertItem(tr("Ver Diario (Este mes)"),103);
    popup->insertItem(tr("Ver Diario (Este año)"),104);
    popup->insertSeparator();
    popup->insertItem(tr("Ver Extracto (Este día)"),111);
    popup->insertItem(tr("Ver Extracto (Este mes)"),113);
    popup->insertItem(tr("Ver Extracto (Este año)"),114);
    popup->insertSeparator();
    popup->insertItem(tr("Ver Balance (Este día)"),121);
    popup->insertItem(tr("Ver Balance (Este mes)"),123);
    popup->insertItem(tr("Ver Balance (Este año)"),124);
    popup->insertSeparator();
    if (col == COL_NOMCUENTA || col == COL_CONTRAPARTIDA || col == COL_CODIGO) {
        popup->insertItem(tr("Editar Cuenta"),130);
        ///            popup->insertItem(tr("Sustituir Cuenta"), 140);
    }// end if
    opcion = popup->exec(pos);
    switch(opcion) {

    case 101:
        boton_diario1(0);
        break;
    case 103:
        boton_diario1(1);
        break;
    case 104:
        boton_diario1(2);
        break;
    case 111:
        boton_extracto1(0);
        break;
    case 113:
        boton_extracto1(1);
        break;
    case 114:
        boton_extracto1(2);
        break;
    case 121:
        boton_balance1(0);
        break;
    case 123:
        boton_balance1(1);
        break;
    case 124:
        boton_balance1(2);
        break;
    case 140:
        /// Aun no esta implementada la sustitución de cuentas desde el menú contextual.
        fprintf(stderr,"Aun no esta implementada la sustitución de cuentas desde el menú contextual\n");
        break;
    case 130:
        // Se ha elegido la opción de editar cuenta.
        // Abrimos la ventana de edición de cuentas.
        QString idcuenta;
        if (col == COL_CODIGO || col == COL_NOMCUENTA)
            idcuenta = linea->idcuenta();
        else
            idcuenta = linea->contrapartida();
        cuentaview *nuevae = new cuentaview(companyact,0,"",true);
        nuevae->cargacuenta(idcuenta.toInt());
        nuevae->exec();
        delete nuevae;
        pintaListLinAsiento1();
        break;
    }// end switch
    delete popup;
}// end contextmenu






void ListLinAsiento1View::borraLinAsiento1act() {
    borraLinAsiento1(currentRow());
}// end borraLinAsiento1act


void ListLinAsiento1View::pintalinListLinAsiento1(int pos) {
    _depura("ListLinAsiento1View::pintalinListLinAsiento1(int)\n",0);
    LinAsiento1 *linea;
    linea = m_lista.at(pos);
    setText(pos, COL_FECHA, linea->fecha());
    setText(pos, COL_CODIGO, linea->codigo());
    setText(pos, COL_CONCEPTO, linea->conceptocontable());
    setText(pos, COL_DEBE, linea->debe());
    setText(pos, COL_HABER, linea->haber());
    setText(pos, COL_NOMCUENTA, linea->descripcioncuenta());
    setText(pos, COL_CONTRAPARTIDA, linea->contrapartida());
    setText(pos, COL_IDBORRADOR, linea->idborrador());
    setText(pos, COL_IDREGISTROIVA, linea->idregistroiva());
    setText(pos, COL_IVA, linea->idtipoiva());
    setText(pos, COL_NOMBREC_COSTE, linea->nombrec_coste());
    setText(pos, COL_NOMBRECANAL, linea->nombrecanal());
    setText(pos, COL_IDCUENTA, linea->idcuenta());
    setText(pos, COL_IDCONTRAPARTIDA, linea->contrapartida());
    setText(pos, COL_IDCANAL, linea->idcanal());
    setText(pos, COL_IDCCOSTE, linea->idc_coste());
    setText(pos, COL_ORDEN, linea->orden());
    if (linea->idregistroiva() != "")
        setText(pos, COL_IVA, "Fact: "+linea->factura()+" Fecha: "+linea->ffactura());
    adjustRow(pos);

}


bool ListLinAsiento1View::eventFilter( QObject *obj, QEvent *ev ) {
    _depura("ListLinAsiento1View::eventFilter()\n",1);
    int dia, mes, ano;
    calendario *cal;
    LinAsiento1 *linea;
    BusquedaCuenta *cuent;
    QString cadena;
    if ( ev->type() == QEvent::KeyRelease ) {
        QKeyEvent *k = (QKeyEvent *)ev;
        int col=currentColumn();
        int row=currentRow();
        linea = lineaact();
        Q3PtrList<QDate> a;
        switch (k->key()) {
        case Qt::Key_Plus:
            switch(col) {
            case COL_CODIGO:
                cuent = new BusquedaCuenta(0,"Busca Cuenta");
                cuent->setempresa(companyact);
                cuent->s_searchCuenta();
                linea->setidcuenta(cuent->idcuenta());
                delete cuent;
                pintalinListLinAsiento1(currentRow());
                return TRUE;
            case COL_FECHA:
                cal = new calendario(0,0);
                cal->exec();
                a = cal->dn->selectedDates();
                dia = a.first()->day();
                mes = a.first()->month();
                ano = a.first()->year();
                cadena.sprintf("%2.2d/%2.2d/%d",dia, mes, ano);
                linea->setfecha(cadena);
                delete cal;
                pintalinListLinAsiento1(currentRow());
                return TRUE;
            }// end switch
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
            switch (col) {
            case COL_FECHA:
                setCurrentCell(row, COL_CODIGO);
                break;
            case COL_CODIGO:
                setCurrentCell(row, COL_CONTRAPARTIDA);
                break;
            case COL_CONTRAPARTIDA:
                setCurrentCell(row, COL_DEBE);
                break;
            case COL_DEBE:
                setCurrentCell(row, COL_HABER);
                break;
            case COL_HABER:
                setCurrentCell(row, COL_CONCEPTO);
                break;
            case COL_CONCEPTO:
                setCurrentCell(row+1, COL_FECHA);
                break;

            }// end switch
            return TRUE;

            break;
        }// end switch
    }// end if
    _depura("END ListLinAsiento1View::eventFilter",1);
    return Q3Table::eventFilter( obj, ev );
} //end eventFilter





void ListLinAsiento1View::valueBudgetLineChanged(int row, int col) {
    _depura("valueBudgetLineChanged \n",0);
    LinAsiento1 *linea;
    linea = lineaat(row);
    Fixed descuadre;
    if (linea != NULL) {
        switch (col) {
        case COL_FECHA:
            if (text(row,col) == QString("*")) {
                if (row > 0) {
                    LinAsiento1 *linant = lineaat(row-1);
                    linea->setfecha(linant->fecha());
                }// end if
            } else
                linea->setfecha(normalizafecha(text(row,COL_FECHA)).toString("dd/MM/yyyy"));
            break;
        case COL_CODIGO:
            if (text(row,col) == QString("*")) {
                if (row > 0) {
                    LinAsiento1 *linant = lineaat(row-1);
                    linea->setcodigo(linant->codigo());
                }// end if
            } else
                linea->setcodigo(text(row, COL_CODIGO));
            break;
        case COL_DEBE: {
                if (text(row,col) == QString("*")) {
                    if (row > 0) {
                        LinAsiento1 *linant = lineaat(row-1);
                        linea->setdebe(linant->debe());
                    }// end if
                } else if (text(row,col) == "+") {
                    linea->setdebe("0.00");
                    linea->sethaber("0.00");
                    descuadre = totaldebe() - totalhaber();
                    if (descuadre < 0) {
                        descuadre = -descuadre;
                        linea->setdebe(descuadre.toQString());
                        linea->sethaber("0.00");
                    } else {
                        linea->setdebe("0.00");
                        linea->sethaber(descuadre.toQString());
                    }// end if
                } else {
                    Fixed val = Fixed(text(row, COL_DEBE));
                    linea->setdebe(val.toQString());
                    if (val != 0)
                        linea->sethaber("0.00");
                    break;
                }// end if
            }// end case
        case COL_HABER: {
                if (text(row,col) == QString("*")) {
                    if (row > 0) {
                        LinAsiento1 *linant = lineaat(row-1);
                        linea->sethaber(linant->haber());
                    }// end if
                } else  if (text(row,col) == "+") {
                    linea->setdebe("0.00");
                    linea->sethaber("0.00");
                    descuadre = totaldebe() - totalhaber();
                    if (descuadre < 0) {
                        descuadre = -descuadre;
                        linea->setdebe(descuadre.toQString());
                        linea->sethaber("0.00");
                    } else {
                        linea->setdebe("0.00");
                        linea->sethaber(descuadre.toQString());
                    }// end if
                } else {
                    Fixed val = Fixed(text(row, COL_HABER));
                    linea->sethaber(val.toQString());
                    if (val != 0)
                        linea->setdebe("0.00");
                }// end if
                break;
            }// end case
        case COL_NOMBRECANAL:
            if (text(row,col) == QString("*")) {
                if (row > 0) {
                    LinAsiento1 *linant = lineaat(row-1);
                    linea->setidcanal(linant->idcanal());
                }// end if
            } else {
                linea->setnombrecanal(text(row, COL_NOMBRECANAL));
            }
            break;
        case COL_CONCEPTO:
            if (text(row,col) == QString("*")) {
                if (row > 0) {
                    LinAsiento1 *linant = lineaat(row-1);
                    linea->setdescripcion(linant->descripcion());
                    linea->setconceptocontable(linant->conceptocontable());
                }// end if
            } else {
                linea->setdescripcion(text(row, COL_CONCEPTO));
                linea->setconceptocontable(text(row, COL_CONCEPTO));
            }// end if
            break;
        case COL_NOMBREC_COSTE:
            if (text(row,col) == QString("*")) {
                if (row > 0) {
                    LinAsiento1 *linant = lineaat(row-1);
                    linea->setidc_coste(linant->idc_coste());
                }// end if
            } else {
                linea->setnombrec_coste(text(row, COL_NOMBREC_COSTE));
            }// end if
            break;
        }// end switch
        pintalinListLinAsiento1(row);
    }// end if

} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LinAsiento1 *ListLinAsiento1View::lineaact() {
    _depura("ListLinAsiento1View::lineaact()\n",0);
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LinAsiento1 *ListLinAsiento1View::lineaat(int row) {
    _depura("ListLinAsiento1::lineaat(%d)\n", 0);
    LinAsiento1 *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
            fprintf(stderr,"Creamos la linea\n");
            linea=new LinAsiento1(companyact);
            linea->setidasiento(mdb_idasiento);
            m_lista.append(linea);
        }// end while
        return(m_lista.at(row));
    } else {
        return NULL;
    }// end if
}// end lineaat

///
/// Esta función se encarga de hacer las inicializaciones en un asiento nuevo
///
void ListLinAsiento1View::iniciar_asiento_nuevo(QString fecha) {
    LinAsiento1 *linea = lineaat(0);
    linea->setDBvalue("fecha",fecha);
    /// Comprobamos si existe un centro de coste por defecto y lo usamos
    selectccosteview *selccostes = companyact->getselccostes();
    QString ccoste = QString::number(selccostes->firstccoste());
    if ( ccoste != "0") {
        linea->setDBvalue("idc_coste", ccoste);
    }// end if

    /// Comprobamos si existe un canal por defecto y lo usamos
    selectcanalview *selcanales = companyact->getselcanales();
    QString idcanal = QString::number(selcanales->firstcanal());
    if ( idcanal != "0") {
        linea->setDBvalue("idcanal",idcanal);
    }// end if
    pintaListLinAsiento1();
    setCurrentCell(0,0);
    setFocus();
}// end iniciar_asiento_nuevo






void ListLinAsiento1View::boton_extracto1(int tipo) {
    _depura("ListLinAsiento1View::boton_extracto1",0);
    QDate fecha1, fecha2, fechaact;
    LinAsiento1 *linea = lineaact();
    if(linea->fecha() != "") {
        fechaact = normalizafecha(linea->fecha());
        switch(tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(),fechaact.day());
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(),1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1,1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        }// end switch
        companyact->extractoempresa()->inicializa1(linea->codigo(), linea->codigo(),fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
        companyact->extractoempresa()->accept();
        companyact->libromayor();
    }// end if

}// end boton_extracto1



void ListLinAsiento1View::boton_diario1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    LinAsiento1 *linea = lineaact();
    if(linea->fecha() != "") {
        fechaact = normalizafecha(linea->fecha());
        switch(tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(),fechaact.day());
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(),1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1,1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        }// end switch
        companyact->diarioempresa()->inicializa1( fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
        companyact->diarioempresa()->accept();
        companyact->librodiario();
    }// end if

}// end boton_diario1



void ListLinAsiento1View::boton_balance1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    LinAsiento1 *linea = lineaact();
    if(	linea->fecha() != "") {
        fechaact = normalizafecha(linea->fecha());
        switch(tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(),fechaact.day());
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(),1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1,1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        }// end switch
        companyact->balanceempresa()->inicializa1(linea->codigo(), linea->codigo(), fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
        companyact->balanceempresa()->accept();
        // La presentación que la haga la clase empresa. Que es quien se encarga de ello.
        companyact->librobalance();
    }// end if

}// end boton_balance1



*/
