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

#define COL_FECHA       0
#define COL_CODIGO   1
#define COL_CONCEPTO    6
#define COL_DEBE        4
#define COL_HABER       5
#define COL_NOMCUENTA   2
#define COL_CONTRAPARTIDA 3
#define COL_IDBORRADOR    7
#define COL_IDREGISTROIVA       8
#define COL_IVA           9
#define COL_NOMBREC_COSTE       10
#define COL_NOMBRECANAL        11
#define COL_IDCUENTA     12
#define COL_IDCONTRAPARTIDA 13
#define COL_IDCANAL         14
#define COL_IDCCOSTE        15
#define COL_ORDEN	    16

#include "calendario.h"
#include "listlinasiento1view.h"
#include "funcaux.h"
#include "asiento1view.h"
#include "cuentaview.h"
#include "plugins.h"

#include <q3table.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>

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


/*
        QString query;
        popup = new Q3PopupMenu;
        popup->insertItem(tr(tr("Igual que la anterior (*)")),4);
        switch (col) {
        case COL_CANAL:
            query = "SELECT * FROM canal";
            conexionbase->begin();
            cur = conexionbase->cargacursor(query,"canales");
            menucanal->insertItem(tr("Ninguno"), 1000);
            conexionbase->commit();
            while (!cur->eof()) {
                menucanal->insertItem(cur->valor("nombre"),1000+atoi(cur->valor("idcanal").ascii()));
                cur->siguienteregistro();
            }// end while
            delete cur;
            popup->insertItem(tr("&Seleccionar Canal"),menucanal);
            break;
        case COL_CCOSTE:
            query = "SELECT * FROM c_coste";
            conexionbase->begin();
            cur  = conexionbase->cargacursor(query,"costes");
            menucoste->insertItem(tr("Ninguno"), 1000);
            conexionbase->commit();
            while (!cur->eof()) {
                menucoste->insertItem(cur->valor("nombre"), 1000+atoi(cur->valor("idc_coste").ascii()));
                cur->siguienteregistro();
            }// end while
            delete cur;
            popup->insertItem(tr("&Seleccionar Centro de Coste"),menucoste);
            break;
        case COL_SUBCUENTA:
        case COL_CONTRAPARTIDA:
        case COL_FECHA:
        case COL_NOMCUENTA:
            popup->insertItem(tr("Seleccionar Valor (+)"),5);
            break;
        case COL_DEBE:
        case COL_HABER:
            popup->insertItem(tr("Introducir Descuadre (+)"),5);
            break;
        }// end switch
        popup->insertSeparator();
        popup->insertItem(tr("Duplicar Apunte"),1);
        popup->insertItem(tr("Borrar Apunte"),6);
        popup->insertItem(tr("Vaciar Asiento"),7);
        popup->insertSeparator();
        popup->insertItem(tr("Subir (Ctrl Arriba)"),2);
        popup->insertItem(tr("Bajar (Ctrl Abajo)"),3);
        opcion = popup->exec(poin);
        delete popup;
        delete menucanal;
        delete menucoste;
 
        switch(opcion) {
        case 1:
            duplicarapunte();
            break;
        case 2:
            subirapunte(row);
            repinta(idAsiento().toInt());
            tapunts3->setCurrentCell(row-1,col);
            break;
        case 3:
            bajarapunte(row);
            repinta(idAsiento().toInt());
            tapunts3->setCurrentCell(row+1,col);
            break;
        case 4:
            duplicar(col);
            break;
        case 5:
            switch(col) {
            case COL_FECHA: {
                    int dia, mes, ano;
                    Q3PtrList<QDate> a;
                    QString cadena;
                    calendario *cal = new calendario(0,0);
                    cal->exec();
                    a = cal->dn->selectedDates();
                    dia = a.first()->day();
                    mes = a.first()->month();
                    ano = a.first()->year();
                    cadena.sprintf("%2.2d/%2.2d/%d",dia, mes, ano);
                    fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
                    tapunts3->setText(row, COL_FECHA, cadena);
                    delete cal;
                    break;
                }// end case
            case COL_NOMCUENTA:
            case COL_SUBCUENTA:
                // Hacemos aparecer la ventana de cuentas
                tapunts3->setText(row,COL_SUBCUENTA,"");
                cambiadasubcuenta(row);
                break;
            case COL_CONTRAPARTIDA:
                tapunts3->setText(row,COL_CONTRAPARTIDA,"");
                cambiadacontrapartida(row);
                break;
            case COL_DEBE:
                tapunts3->setText(row,COL_DEBE, descuadre->text());
                calculadescuadre();
                break;
            case COL_HABER:
                tapunts3->setText(row,COL_HABER, descuadre->text());
                calculadescuadre();
                break;
            }// end switch
            break;
        case 6:
            borraborrador(row);
            break;
        case 7:
            if(QMessageBox::question(this, tr("Cuidado!"), tr("Seguro que quiere borrar todos los apuntes del asiento?"),tr("&NO"), tr("&SI"), QString::null, 1, 0)) {
                for(; tapunts3->text(0,COL_IDBORRADOR) != ""; borraborrador(0))
                    ;
            }// end if
            break;
        default:
            switch(col) {
            case COL_CANAL:
                if (opcion == 1000) {
                    tapunts3->setText(row, COL_CANAL, "");
                    tapunts3->setText(row, COL_IDCANAL, "");
                }// end if
                if (opcion > 1000) {
                    QString query1;
                    opcion -= 1000;
                    query1.sprintf("SELECT * FROM canal WHERE idcanal=%d", opcion);
                    conexionbase->begin();
                    cur = conexionbase->cargacursor(query1.ascii(),"canales1");
                    conexionbase->commit();
                    if (!cur->eof()) {
                        tapunts3->setText(row,COL_CANAL, cur->valor("nombre"));
                        tapunts3->setText(row,COL_IDCANAL, cur->valor("idcanal"));
                    }// end if
                    delete cur;
                }// end if
                break;
            case COL_CCOSTE:
                if (opcion == 1000) {
                    tapunts3->setText(row, COL_CCOSTE, "");
                    tapunts3->setText(row, COL_IDCCOSTE, "");
                }// end if
                if (opcion > 1000) {
                    QString query1;
                    opcion -= 1000;
                    query1.sprintf("SELECT * FROM c_coste WHERE idc_coste=%d", opcion);
                    conexionbase->begin();
                    cur = conexionbase->cargacursor(query1.ascii(),"canales1");
                    conexionbase->commit();
                    if (!cur->eof()) {
                        tapunts3->setText(row,COL_CCOSTE, cur->valor("nombre"));
                        tapunts3->setText(row,COL_IDCCOSTE, cur->valor("idc_coste"));
                    }// end if
                    delete cur;
                }// end if
                break;
            }// end switch
            break;
        }// end switch
    } else {
 
 
*/

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

/**
 * Esta función se encarga de hacer las inicializaciones en un asiento nuevo
 */
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


/** \brief SLOT que responde a la pulsación del botón de iva.
  * Crea la clase \ref ivaview y la inicializa con el identificador de borrador para que se presente con los datos ya introducidos.
  * La clase ivaview hace una inserción o una modificación segun exista o no una entrada de iva para dicho borrador.
  */
void ListLinAsiento1View::boton_iva() {
    int res = g_plugins->lanza("ListLinAsiento1View_boton_iva", this);
/*
    guardaListLinAsiento1();
    LinAsiento1 *linea = lineaact();
    if (linea->DBvalue("idborrador") != "") {
        int idborrador = linea->DBvalue("idborrador").toInt();
        ivaview *nuevae=new ivaview(companyact, 0,"");
        nuevae->inicializa1(idborrador);
        nuevae->exec();
        delete nuevae;
        pintaListLinAsiento1();
    }// end if
*/
}// end boton_iva




/**
  * Si el parametro pasado es un:
  * 0 -> del día actual
  * 1 -> del mes actual
  * 2 -> del año actual
  */
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


/**
  * Si el parametro pasado es un:
  * 0 -> del día actual
  * 1 -> del mes actual
  * 2 -> del año actual
  */
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


/**
  * Si el parametro pasado es un:
  * 0 -> del día actual
  * 1 -> del mes actual
  * 2 -> del año actual
  */
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

