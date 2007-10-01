/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QKeyEvent>
#include <QEvent>
#include <QFile>
#include <QHeaderView>
#include <QTextStream>
#include <QMenu>
#include <QShortcut>
#include <QLocale>
#include <QRegExp>

#include "subform3.h"


///
/**
\return
**/
QList<SDBRecord *> *SubForm3::lista() {
    _depura("SubForm3::lista", 0);
    return &m_lista;
    _depura("END SubForm3::lista", 0);
}


///
/**
\return
**/
QList<SHeader *> *SubForm3::cabecera() {
    _depura("SubForm3::cabecera", 0);
    return &m_lcabecera;
    _depura("END SubForm3::cabecera", 0);
}


///
/**
\param mode
**/
void SubForm3::setHorizontalScrollMode(QAbstractItemView::ScrollMode mode) {
    _depura("SubForm3::setHorizontalScrollMode", 0);
    mui_list->setHorizontalScrollMode(mode);
    _depura("END SubForm3::setHorizontalScrollMode", 0);
}


///
/**
\param f
**/
void SubForm3::setDelete(bool f) {
    _depura("SubForm3::setDelete", 0);
    m_delete = f;
    _depura("END SubForm3::setDelete", 0);
}


///
/**
\param row
\param col
\return
**/
SDBCampo *SubForm3::item(int row, int col) {
    _depura("SubForm3::item", 0);
    _depura("END SubForm3::item", 0);
    return (SDBCampo *) mui_list->item(row, col);
}


///
/**
\param row
\param col
**/
void SubForm3::setCurrentItem(int row, int col) {
    _depura("SubForm3::setCurrentItem", 0);
    mui_list->setCurrentItem( item(row, col));
    _depura("END SubForm3::setCurrentItem", 0);
}


///
/**
\return
**/
int SubForm3::columnCount() {
    _depura("SubForm3::columnCount", 0);
    return mui_list->columnCount();
    _depura("END SubForm3::columnCount", 0);
}


///
/**
\param i
**/
void SubForm3::showColumn(int i) {
    _depura("SubForm3::showColumn", 0);
    mui_list->showColumn(i);
    _depura("END SubForm3::showColumn", 0);
}


///
/**
\param i
\return
**/
int SubForm3::columnWidth(int i) {
    _depura("SubForm3::columnWidth", 0);
    _depura("END SubForm3::columnWidth", 0);
    return mui_list->columnWidth(i);
}


///
/**
\param i
\param j
**/
void SubForm3::setColumnWidth(int i, int j) {
    _depura("SubForm3::setColumnWidth", 0);
    mui_list->setColumnWidth(i, j);
    _depura("END SubForm3::setColumnWidth", 0);
}


///
/**
\param i
**/
void SubForm3::hideColumn(int i) {
    _depura("SubForm3::hideColumn", 0);
    mui_list->hideColumn(i);
    _depura("END SubForm3::hideColumn", 0);
}


///
/**
\return
**/
int SubForm3::currentRow() {
    _depura("SubForm3::currentRow", 0);
    _depura("END SubForm3::currentRow", 0);
    return mui_list->currentRow();
}


///
/**
\return
**/
int SubForm3::rowCount() {
    _depura("SubForm3::rowCount", 0);
    _depura("END SubForm3::rowCount", 0);
    return mui_list->rowCount();
}


///
/**
\return
**/
int SubForm3::currentColumn() {
    _depura("SubForm3::currentColumn", 0);
    return mui_list->currentColumn();
    _depura("END SubForm3::currentColumn", 0);
}


///
/**
\param modo
**/
void SubForm3::setResizeMode(QHeaderView::ResizeMode modo) {
    _depura("SubForm3::setResizeMode", 0);
    mui_list->horizontalHeader()->setResizeMode(modo);
    _depura("END SubForm3::setResizeMode", 0);
}


///
/**
**/
void SubForm3::resizeColumnsToContents() {
    _depura("SubForm3::resizeColumnsToContents", 0);
    mui_list->resizeColumnsToContents();
    _depura("END SubForm3::resizeColumnsToContents", 0);
}


///
/**
**/
void SubForm3::resizeRowsToContents() {
    _depura("SubForm3::resizeRowsToContents", 0);
    mui_list->resizeRowsToContents();
    _depura("END SubForm3::resizeRowsToContents", 0);
}


///
/**
\param i
**/
void SubForm3::resizeColumnToContents(int i) {
    _depura("SubForm3::resizeColumnToContents", 0);
    mui_list->resizeColumnToContents(i);
    _depura("END SubForm3::resizeColumnToContents", 0);
}


///
/**
\param i
**/
void SubForm3::resizeRowToContents(int i) {
    _depura("SubForm3::resizeRowToContents", 0);
    mui_list->resizeRowToContents(i);
    _depura("END SubForm3::resizeRowToContents", 0);
}


///
/**
**/
void SubForm3::hideConfig() {
    _depura("SubForm3::hideConfig", 0);
    mui_configurador->hide();
    _depura("END SubForm3::hideConfig", 0);
}


///
/**
**/
void SubForm3::showConfig() {
    _depura("SubForm3::showConfig", 0);
    mui_configurador->show();
    _depura("END SubForm3::showConfig", 0);
}


///
/**
\param nom
**/
void SubForm3::setDBTableName(QString nom) {
    _depura("SubForm3::setDBTableName", 0);
    m_tablename = nom;
    m_fileconfig = nom;
    _depura("END SubForm3::setDBTableName", 0);
}


///
/**
\return
**/
QString SubForm3::tableName() {
    _depura("SubForm3::tableName", 0);
    _depura("END SubForm3::tableName", 0);
    return m_tablename;
}


///
/**
\param nom
**/
void SubForm3::setFileConfig(QString nom) {
    _depura("SubForm3::setFileConfig", 0);
    m_fileconfig = nom;
    _depura("END SubForm3::setFileConfig", 0);
}


///
/**
\param nom
**/
void SubForm3::setDBCampoId(QString nom) {
    _depura("SubForm3::setDBCampoId", 0);
    m_campoid = nom;
    _depura("END SubForm3::setDBCampoId", 0);
}


///
/**
**/
void SubForm3::clear() {
    _depura("SubForm3::clear", 0);
    mui_list->clear();
    _depura("END SubForm3::clear", 0);
}


///
/**
\param i
**/
void SubForm3::setRowCount(int i) {
    _depura("SubForm3::setRowCount", 0);
    mui_list->setRowCount(i);
    _depura("END SubForm3::setRowCount", 0);
}


///
/**
\param i
**/
void SubForm3::setColumnCount(int i) {
    _depura("SubForm3::setColumnCount", 0);
    mui_list->setColumnCount(i);
    _depura("END SubForm3::setColumnCount", 0);
}


///
/**
**/
void SubForm3::creaMenu(QMenu *) {
    _depura("SubForm3:: CreaMenu", 0, "funcion para ser sobreescrita");
}


///
/**
**/
void SubForm3::procesaMenu(QAction *) {
    _depura("SubForm3:: procesaMenu", 0, "funcion para ser sobreescrita");
}


///
/**
\param pijama
**/
void SubForm3::setListadoPijama(bool pijama) {
    _depura("SubForm3::setListadoPijama", 0);
    if (pijama) {
        mui_list->setAlternatingRowColors(TRUE);
    } else {
        mui_list->setAlternatingRowColors(FALSE);
    } // end if
    _depura("END SubForm3::setListadoPijama", 0);
}


///
/**
\return
**/
bool SubForm3::listadoPijama() {
    _depura("SubForm3::listadoPijama", 0);
    return mui_list->alternatingRowColors();
    _depura("END SubForm3::listadoPijama", 0);
}


/// SubForm3, constructor de la clase base para subformularios.
/**
\param parent
**/
SubForm3::SubForm3(QWidget *parent) : BLWidget(parent) {
    _depura("SubForm3::SubForm3", 0);
    setupUi(this);
    /// Inicializamos los valores de columna y fila previas para que no tengan un valor indefinido.
    /// Se inicializan con -1 para considerar que no hay celda previa.
    m_prevCol = -1;
    m_prevRow = -1;

    /// Desactivamos el procesado de cambios.
    m_procesacambios = FALSE;

    m_textoceldaParaRowSpan = "";
    m_filaInicialRowSpan = -1;

    /// Disparamos los plugins.
    int res = g_plugins->lanza("SubForm3_SubForm3", this);
    if (res != 0) {
        m_procesacambios = TRUE;
        return;
    } // end if

    mui_list->setSelectionMode(QAbstractItemView::SingleSelection);
    mui_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    mui_list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    mui_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mui_list->setSortingEnabled(FALSE);
    /// TODO:Hay un Bug que impide ordenar bien los elementos.
    mui_list->horizontalHeader()->setMovable(TRUE);
    /// Valor por defecto en todos los listados.
    setListadoPijama(TRUE);
    QPalette p;
    p = mui_list->palette();
    m_colorfondo1 = p.color(QPalette::Normal, QPalette::Base);
    m_colorfondo2 = p.color(QPalette::Normal, QPalette::AlternateBase);

    /// Capturamos la secuencia de teclas para hacer aparecer o desaparecer
    /// el panel de configuracion del subform3.
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+B"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(toogleConfig()));

    m_insercion = FALSE;
    m_primero = TRUE;
    m_sorting = FALSE;
    m_orden = FALSE;
    m_ordenporquery = FALSE;

    /// Para el listado de columnas hacemos una inicializacion.
    QStringList headers;
    headers << "" << tr("Nombre") << tr("Nombre de campo") << tr("Visible");
    mui_listcolumnas->setColumnCount(4);
    mui_listcolumnas->setHorizontalHeaderLabels(headers);
    mui_listcolumnas->setShowGrid(FALSE);
    mui_listcolumnas->setColumnWidth(0, 25);
    mui_listcolumnas->setColumnWidth(1, 100);
    mui_listcolumnas->setColumnWidth(2, 175);
    mui_listcolumnas->setColumnWidth(3, 0);
    mui_listcolumnas->setSelectionBehavior(QAbstractItemView::SelectRows);
    mui_listcolumnas->verticalHeader()->hide();
    mui_listcolumnas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mui_listcolumnas->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);

    /// Siempre que arrancamos mostramos la pagina 0.
    mui_paginaact->setValue(1);
    /// Ocultamos la configuracion.
    hideConfig();
    /// Limpiamos la lista.
    m_lista.clear();
    m_listaborrar.clear();

    /// Inicializamos los valores de columna y fila previas para que no tengan un valor indefinido.
    /// Se inicializan con -1 para considerar que no hay celda previa.
    m_prevCol = -1;
    m_prevRow = -1;
    setDelete(TRUE);

    /// Disparamos los plugins.
    g_plugins->lanza("SubForm3_SubForm3_Post", this);
    m_procesacambios = TRUE;
    _depura("END SubForm3::SubForm3", 0);
}


/// Destructor de Clase que guarda la configuracion.
/**
**/
SubForm3::~SubForm3() {
    _depura("SubForm3::~SubForm3", 0);
    /// PAra destruir desactivamos el control de cambios.
    m_procesacambios = FALSE;
    guardaconfig();
    _depura("END SubForm3::~SubForm3", 0);
}


/// Habilita o inhabilita el ordenado de columnas mediante el pulsar sobre ellas.
/**
\param sorting
**/
void SubForm3::setSortingEnabled(bool sorting) {
    _depura("SubForm3::setSortingEnabled", 0);
    mui_list->setSortingEnabled(sorting);
    m_sorting = sorting;
    _depura("END SubForm3::setSortingEnabled", 0);
}


///
/**
\return
**/
bool SubForm3::sortingEnabled() {
    _depura("SubForm3::sortingEnabled", 0);
    _depura("END SubForm3::sortingEnabled", 0);
    return m_sorting;
}


/// Establece si el subformulario se ordena mediante un campo orden en la base de datos.
/**
\param sorting
**/
void SubForm3::setOrdenEnabled(bool sorting) {
    _depura("SubForm3::setOrdenEnabled", 0);
    m_orden = sorting;
    _depura("END SubForm3::setOrdenEnabled", 0);
}


///
/**
\return
**/
bool SubForm3::ordenEnabled() {
    _depura("SubForm3::ordenEnabled", 0);
    return m_orden;
    _depura("END SubForm3::ordenEnabled", 0);
}


///
/**
\param item
**/
void SubForm3::on_mui_list_itemDoubleClicked(QTableWidgetItem *item) {
    _depura("SubForm3::on_mui_list_itemDoubleClicked", 0);
    emit itemDoubleClicked(item);
    _depura("END SubForm3::on_mui_list_itemDoubleClicked", 0);
}

///
/**
\param item
**/
void SubForm3::on_mui_list_itemClicked(QTableWidgetItem *item) {
    _depura("SubForm3::on_mui_list_itemClicked", 0);
    emit itemClicked(item);
    _depura("END SubForm3::on_mui_list_itemClicked", 0);
}


///
/**
\param row
\param col
**/
void SubForm3::on_mui_list_cellDoubleClicked(int row, int col) {
    _depura("SubForm3::on_mui_list_cellDoubleClicked", 0);
    emit cellDoubleClicked(row, col);
    _depura("END SubForm3::on_mui_list_cellDoubleClicked", 0);
}


///
/**
\param head
\return
**/
bool SubForm3::existsHeader(const QString &head) {
    _depura("SubForm3::existsHeader", 0);
    SHeader *linea;
    for (int i = 0; i < m_lcabecera.size(); ++i) {
        linea = m_lcabecera.at(i);
        if (linea->nomcampo() == head) {
            _depura("END SubForm3::existsHeader", 0);
            return TRUE;
        } // end if
    } // end for
    _depura("END SubForm3::existsHeader", 0);
    return FALSE;
}


/// Se encarga de crear un nuevo registro (una fila entera) y de inicializarla para que
/// tenga todos los elementos necesarios (columnas).
/// Nota: Esta funcion es de uso interno, no debe ser usada.
/**
\return
**/
SDBRecord *SubForm3::newSDBRecord() {
    _depura("SubForm3::newSDBRecord", 0);
    SDBRecord *rec = new SDBRecord(empresaBase());
    rec->setDBTableName(m_tablename);
    rec->setDBCampoId(m_campoid);

    SHeader *linea;
    for (int i = 0; i < m_lcabecera.size(); ++i) {
        linea = m_lcabecera.at(i);
        rec->addDBCampo(linea->nomcampo(), linea->tipo(), linea->restricciones(), linea->nompresentacion());
    } // end for

    SDBCampo *camp;
    for (int i = 0; i < rec->lista()->size(); ++i) {
        camp = (SDBCampo *) rec->lista()->at(i);
        SHeader *head = m_lcabecera.at(i);
        Qt::ItemFlags flags = 0;
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if (!(head->options() & SHeader::DBNoWrite))
            flags |= Qt::ItemIsEditable;
        if (head->tipo() == DBCampo::DBboolean) {
            flags |= Qt::ItemIsUserCheckable;
        } // end if

        camp->setFlags(flags);

        /// Tratamos el tema de la alineacion dependiendo del tipo.
        if (head->tipo() == DBCampo::DBint || head->tipo() == DBCampo::DBnumeric || head->tipo() == DBCampo::DBdate) {
            camp->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        } else {
            camp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        } // end if
    } // end for
    _depura("END SubForm3::newSDBRecord", 0);
    return rec;
}


/// Este metodo crea el registro final cuando se trata de subformularios con la opcion
/// de insertar nuevos registros en el subformulario.
/**
\return
**/
void SubForm3::nuevoRegistro() {
    _depura("SubForm3::nuevoRegistro", 0);
    if (!m_insercion)
        return;

    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(FALSE);

    SDBRecord *rec = newSDBRecord();

    m_lista.append(rec);

    mui_list->insertRow(m_lista.size() - 1);
    SDBCampo *camp;
    for (int i = 0; i < rec->lista()->size(); ++i) {
        camp = (SDBCampo *) rec->lista()->at(i);
        mui_list->setItem(m_lista.size() - 1, i, camp);
    } // end for

    /// Nos posicionamos en una celda del nuevo registro ya que si no un bug no nos permite usar currentRow()
    mui_list->setCurrentCell(m_lista.size() - 1, 2);

    /// Activamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(m_sorting);
    _depura("END SubForm3::nuevoRegistro", 0);
}


/// Pinta los plantillaes en la tabla.
/**
**/
void SubForm3::pintaCabeceras() {
    _depura("SubForm3::pintaCabeceras", 0);
    QStringList headers;
    SHeader *linea;
    for (int i = 0; i < m_lcabecera.size(); ++i) {
        linea = m_lcabecera.at(i);
        headers << linea->nompresentacion();
        if (linea->options() & SHeader::DBNoView)
            mui_list->hideColumn(i);
        else
            mui_list->showColumn(i);
    } // end for
    mui_list->setHorizontalHeaderLabels(headers);
    mui_list->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
    _depura("END SubForm3::pintaCabeceras", 0);
}


/// Se situa en una celda especifica del subformulario.
/**
\param row
\param col
**/
void SubForm3::situarse(unsigned int row, unsigned int col) {
    _depura("SubForm3::situarse", 0, QString::number(row) + " " + QString::number(col));
    unsigned int nrow = row;
    unsigned int ncol = col;
    SHeader *linea = m_lcabecera.at(ncol);
    if (!linea)
        return;
    bool invalido = TRUE;

    /// Mientras no se encuentre un candidato y haya candidatos
    while (invalido && nrow < row + 2) {
        ncol++;
        if (ncol == (unsigned int) m_lcabecera.count()) {
            ncol = 0;
            nrow++;
        } // end if
        linea = m_lcabecera.at(ncol);
        invalido = FALSE;
        if (linea->options() & SHeader::DBNoView)
            invalido = TRUE;
        if (linea->options() & SHeader::DBNoWrite)
            invalido = TRUE;
    } // end while
    mui_list->setCurrentCell(nrow, ncol);
    _depura("END SubForm3::situarse", 0, QString::number(nrow) + " " + QString::number(ncol));
}


/// Se situa en una celda especifica del subformulario.
/** Se incluye la celda incluye la actual */
/**
\param row
\param col
\return
**/
void SubForm3::situarse1(unsigned int row, unsigned int col) {
    _depura("SubForm3::situarse1", 0, QString::number(row) + " " + QString::number(col));
    unsigned int nrow = row;
    unsigned int ncol = col;
    SHeader *linea = m_lcabecera.at(ncol);
    if (!linea) {
        return;
    } // end if
    bool invalido = TRUE;
    while (invalido) {
        if (ncol == (unsigned int) m_lcabecera.count()) {
            ncol = 0;
            nrow++;
        } // end if
        linea = m_lcabecera.at(ncol);
        invalido = FALSE;
        if (linea->options() & SHeader::DBNoView)
            invalido = TRUE;
        if (linea->options() & SHeader::DBNoWrite)
            invalido = TRUE;
        if (invalido)
            ncol++;
    } // end while
    mui_list->setCurrentCell(nrow, ncol);
    _depura("END SubForm3::situarse1", 0);
}


/// Cuando tenemos un registro que no se tiene que cargar (pq es nuevo o algo
/// asi) de la base de datos, con la funcion pintar lo dejamos en un estado que
/// se podria considerar presentable para poder operar con el subformulario.
/**
**/
void SubForm3::pintar() {
    _depura("SubForm3::pintar", 0);
    m_procesacambios = FALSE;
    mui_list->setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero) {
        cargaconfig();
    } // end if
    nuevoRegistro();
    m_procesacambios = TRUE;
    _depura("END SubForm3::pintar", 0);
}


/// Carga una tabla a partir del recordset que se le ha pasado.
/**
\return
**/
int SubForm3::inicializar() {
    _depura("SubForm3::inicializar", 0);
    m_procesacambios = FALSE;
    mui_query->setPlainText("");
    SDBRecord *rec;

    int filpag = mui_filaspagina->text().toInt();
    if (filpag <= 0)
        filpag = 500;

    int pagact = mui_paginaact->text().toInt();
    if (pagact <= 0)
        pagact = 1;

    /// Vaciamos la tabla para que no contenga registros.
    mui_list->clear();
    mui_list->setRowCount(0);

    /// Vaciamos el recordset para que no contenga registros.
    while (m_lista.count()) {
        rec = m_lista.takeFirst();
        if (rec)
            delete rec;
    } // end while

    /// Ponemos los datos sobre el query.
    mui_numfilas->setText("0");
    int numpag = 0;
    mui_numpaginas->setText(QString::number(numpag));
    /// Inicializamos las columnas y pintamos las cabeceras.
    mui_list->setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero) {
        cargaconfig();
    } // end if

    nuevoRegistro();
    /// Ordenamos la tabla.
    mui_list->ordenar();
    /// configuramos que registros son visibles y que registros no lo son.
    on_mui_confcol_clicked();
    m_procesacambios = TRUE;
    _depura("END SubForm3::inicializar", 0);
    return 0;
}


///
/**
\param campo
**/
void SubForm3::setColumnToRowSpan(QString campo) {
    _depura("SubForm3::setColumnToRowSpan", 0);
    m_columnaParaRowSpan = campo;
    _depura("END SubForm3::setColumnToRowSpan", 0);
}


///
/**
\param color
**/
void SubForm3::setColorFondo1(QString color) {
    _depura("SubForm3::setColorFondo1", 0);
    m_colorfondo1 = QColor(color);
    _depura("END SubForm3::setColorFondo1", 0);
}


///
/**
\param color
**/
void SubForm3::setColorFondo2(QString color) {
    _depura("SubForm3::setColorFondo2", 0);
    m_colorfondo2 = QColor(color);
    _depura("END SubForm3::setColorFondo1", 0);
}


///
/**
\param twidget
\param filainicial
\param totalfilas
\param colorfondo
**/
void SubForm3::ponItemColorFondo(QTableWidget *twidget, int filainicial, int totalfilas, QColor colorfondo) {
    SDBRecord *reg3;
    /// Si hay que poner color al fondo de las filas se pone el que toque.
    if (listadoPijama() == FALSE) {
        /// Recorre todos los items de las filas afectadas.
        for (int k = filainicial; k < filainicial + totalfilas; ++k) {
            reg3 = m_lista.at(k);
            for (int r = 0; r < reg3->lista()->size(); ++r) {
                /// Pone el color
                twidget->item(k, r)->setBackground(QBrush(colorfondo));
            } // end for
        } // end for
    } // end if
}


/// Carga una tabla a partir del recordset que se le ha pasado.
/** Este m&eacute;todo genera, a partir del recordset pasado como par&aacute;metro el listado y lo muestra. */
/**
\param cur
**/
void SubForm3::cargar(cursor2 *cur) {
    _depura("SubForm3::cargar", 0, objectName());
    m_procesacambios = FALSE;

    SDBRecord *reg;
    SDBRecord *reg2;
    SDBCampo *camp;
    SDBCampo *camp2;
    QColor colorfondo = m_colorfondo1;
    bool coloraponerfondo = FALSE;

    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(FALSE);

    /// Reseteamos el "rowSpan" de la tabla antes de borrar las filas.
    for (int i = 0; i < m_lista.size(); ++i) {
        reg = m_lista.at(i);
        for (int j = 0; j < reg->lista()->size(); ++j) {
            SHeader *head = m_lcabecera.at(j);
            if (head->nomcampo() == m_columnaParaRowSpan) {
                mui_list->setSpan(i, j, 1, 1);
            } // end if
        } // end for
    } // end for

    /// Vaciamos la tabla para que no contenga registros.
    mui_list->clear();
    mui_list->setRowCount(0);

    /// Vaciamos el recordset para que no contenga registros.
    SDBRecord *rec;
    while (m_lista.count()) {
        rec = m_lista.takeFirst();
        if (rec)
            delete rec;
    } // end while

    /// Inicializamos las columnas y pintamos las cabeceras.
    mui_list->setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero) {
        cargaconfig();
    } // end if

    /// Si hay un problema con el cursor, se sale antes de generar Segmentation Fault.
    if (cur == NULL) return;

    /// Ponemos la consulta a la vista para que pueda ser editada.
    mui_query->setPlainText(cur->query());

    /// Tratramos con la paginacion.
    int filpag = mui_filaspagina->text().toInt();
    if (filpag <= 0) {
        filpag = 500;
    } // end if

    int pagact = mui_paginaact->text().toInt();
    if (pagact <= 0) {
        pagact = 1;
    } // end if

    /// Ponemos los datos sobre la consulta.
    mui_numfilas->setText(QString::number(cur->numregistros()));
    int numpag = cur->numregistros() / filpag + 1;
    mui_numpaginas->setText(QString::number(numpag));

    /// Si el numero de elementos es igual al numero de filas por pagina abilitamos la pagina siguiente
    if (filpag == cur->numregistros()) {
        mui_pagsiguiente->setEnabled(TRUE);
    } else {
        mui_pagsiguiente->setDisabled(TRUE);
    } // end if

    /// Si la pagina actual es 1 desabilitamos la pagina anterior
    if (pagact ==  1) {
        mui_paganterior->setDisabled(TRUE);
    } else {
        mui_paganterior->setEnabled(TRUE);
    } // end if

    /// Recorremos el recordset y ponemos los registros en un orden determinado.
    int porcentajecarga = 0;
    while (!cur->eof() && m_lista.count() < filpag) {
        SDBRecord *rec = newSDBRecord();
        rec->DBload(cur);
        m_lista.append(rec);
        cur->siguienteregistro();
        porcentajecarga++;
    } // end while

    /// Inicializamos la tabla con las filas necesarias.
    mui_list->setRowCount(m_lista.count());
    for (int i = 0; i < m_lista.size(); ++i) {
        reg = m_lista.at(i);
        QRegExp patronFecha("^.*00:00:00.*$"); /// Para emparejar los valores fechas.
        for (int j = 0; j < reg->lista()->size(); ++j) {
            camp = (SDBCampo *) reg->lista()->at(j);
            /// Si es una fecha lo truncamos a 10 caracteres para presentar solo la fecha.
            if (patronFecha.exactMatch(camp->valorcampo())) {
                camp->set
                (camp->valorcampo().left(10));
            } // end if
            /// Rellena la tabla con los datos.
            mui_list->setItem(i, j, camp);
        } // end for
    } // end for

    /// Establece el "rowSpan" de la tabla.
    QString textoCeldaAnterior;
    QString textoCeldaActual;

    /// Recorre las filas.
    m_filaInicialRowSpan = -1;

    /// Pone el 'rowSpan' a las filas que son iguales.
    for (int i = 0; i < m_lista.size(); ++i) {
        reg = m_lista.at(i);
        for (int j = 0; j < reg->lista()->size(); ++j) {
            SHeader *head = m_lcabecera.at(j);
            if (head->nomcampo() == m_columnaParaRowSpan) {
                camp = (SDBCampo *) reg->lista()->at(j);
                textoCeldaActual = camp->valorcampo();
                /// Mira lo que hay en la fila anterior si existe.
                if (i > 0) {
                    reg2 = m_lista.at(i - 1);
                    camp2 = (SDBCampo *) reg2->lista()->at(j);
                    textoCeldaAnterior = camp2->valorcampo();
                    if (textoCeldaActual == textoCeldaAnterior) {
                        /// activamos el indice de celdas iguales
                        if (m_filaInicialRowSpan == -1) {
                            m_filaInicialRowSpan = i - 1;
                        } // end if
                        /// hay un registro despu&eacute;s. No, dibuja 'rowSpan'.
                        if (i == (m_lista.size() - 1)) {
                            ponItemColorFondo(mui_list, m_filaInicialRowSpan, i - m_filaInicialRowSpan + 1, colorfondo);
                            mui_list->setSpan(m_filaInicialRowSpan, j, i - m_filaInicialRowSpan + 1, 1);

                            if (coloraponerfondo == FALSE) {
                                colorfondo = m_colorfondo2;
                                coloraponerfondo = TRUE;
                            } else {
                                colorfondo = m_colorfondo1;
                                coloraponerfondo = FALSE;
                            } // end if

                        } // end if
                    } else {
                        /// Comprobamos si queda algo pendiente de hacer 'rowSpan'.
                        if (m_filaInicialRowSpan != -1) {
                            /// 'rowSpan' desde inicio iguales hasta fila anterior.
                            ponItemColorFondo(mui_list, m_filaInicialRowSpan, i - m_filaInicialRowSpan, colorfondo);
                            mui_list->setSpan(m_filaInicialRowSpan, j, i - m_filaInicialRowSpan, 1);

                            if (coloraponerfondo == FALSE) {
                                colorfondo = m_colorfondo2;
                                coloraponerfondo = TRUE;
                            } else {
                                colorfondo = m_colorfondo1;
                                coloraponerfondo = FALSE;
                            } // end if
                        } else {
                            /// El registro s&oacute;lo tiene una fila.
                            ponItemColorFondo(mui_list, i - 1, 1, colorfondo);
                            if (coloraponerfondo == FALSE) {
                                colorfondo = m_colorfondo2;
                                coloraponerfondo = TRUE;
                            } else {
                                colorfondo = m_colorfondo1;
                                coloraponerfondo = FALSE;
                            } // end if
                        } // end if
                        m_filaInicialRowSpan = -1;
                    } // end if
                } // end if
            } // end if
        } // end for
    } // end for

    /// Si est&aacute; definido no aplicamos ninguna ordenaci&oacute;n.
    if (!m_ordenporquery) {
        if (m_orden) {
            /// Si estamos con campos de ordenacion ordenamos tras la carga el listado
            for (int i = 0; i < m_lcabecera.size(); ++i) {
                if (m_lcabecera.at(i)->nomcampo() == "orden" + m_tablename)
                    mui_list->sortItems(i);
            } // end for
        } else {
            /// Si no estamos con campos de ordenaci&oacute;n ordenamos por lo que toca.
            /// Ordenamos la tabla.
            mui_list->ordenar();
        } // end if
    } // end if

    /// Generamos el registro de insercion.
    nuevoRegistro();

    /// Configuramos que registros son visibles y que registros no lo son
    on_mui_confcol_clicked();

    /// Reactivamos el sorting
    mui_list->setSortingEnabled(m_sorting);

    m_procesacambios = TRUE;

    _depura("END SubForm3::cargar", 0);
}


///
/**
\param ordenactivado
**/
void SubForm3::setOrdenPorQuery(bool ordenactivado) {
    _depura("SubForm3::setOrdenPorQuery", 0);
    m_ordenporquery = ordenactivado;
    _depura("END SubForm3::setOrdenPorQuery", 0);
}


///
/**
\param query La consulta SQL a cargar en el subformulario.
**/
void SubForm3::cargar(QString query) {
    _depura("SubForm3::cargar", 0);
    /// Si el query no existe no hacemos nada.
    if (query == "") return;

    try {
        m_query = query;

        /// Tratramos con la paginacion.
        int limit = mui_filaspagina->text().toInt();
        if (limit <= 0) {
            limit = 500;
        } // end if

        int pagact = mui_paginaact->text().toInt();
        if (pagact <= 0) {
            pagact = 1;
        } // end if
        int offset = limit * (pagact -1);

        cursor2 *cur = empresaBase()->cargacursor(query, "", limit, offset);
        cargar(cur);
        delete cur;
    } catch (...) {
        _depura("SubForm3::cargar", 2, "Error en la carga de datos");
    } // end try
    _depura("END SubForm3::cargar", 0);
}


/// Devuelve la linea que se esta tratando actualmente.
/**
\return
**/
SDBRecord *SubForm3::lineaact() {
    _depura("SubForm3::lineaact", 0);
    _depura("END SubForm3::lineaact", 0);
    return lineaat(mui_list->currentRow());
}


/// Devuelve la linea especificada o NULL si ésta no existe.
/**
\param row
\return
**/
SDBRecord *SubForm3::lineaat(int row) {
    _depura("SubForm3::lineaat()", 0, QString::number(row));
    try {
        /// Si la lista no tiene suficientes elementos devolvemos NULL
        if (mui_list->rowCount() < row || row < 0) {
            throw -1;
        } // end if

        /// Seleccionamos el campo especificado y lo devolvemos.
        SDBCampo *camp = (SDBCampo*) mui_list->item(row, 0);
        if (!camp) {
            throw -1;
        } // end if
        SDBRecord *rec = (SDBRecord *) camp->pare();

        m_procesacambios = TRUE;

        _depura("END SubForm3::lineaat()", 0);
        return rec;
    } catch (...) {
        _depura ("SubForm3::lineaat linea inexistente", 2, QString::number(row));
        m_procesacambios = TRUE;
        return NULL;
    }
}


/// Devuelve TRUE si el registro ha sido completamente rellenado.
/**
\param row
\return
**/
bool SubForm3::campoCompleto(int row) {
    _depura("SubForm3::campoCompleto", 0);
    SDBCampo *camp;
    SHeader *header;
    /// Sacamos celda a celda toda la fila
    for (int i = 0; i < mui_list->columnCount(); i++) {
        camp = (SDBCampo *) mui_list->item(row, i);

        /// Si el dato no es valido se sale
        if (!camp) return FALSE;

        header = m_lcabecera.at(i);
        if (camp->restrictcampo() & DBCampo::DBNotNull
                && camp->text() == ""
                && !(header->options() & SHeader::DBNoView)
                && camp->tipo() != DBCampo::DBboolean) {
            _depura("SubForm3::campoCompleto", 0, "El campo no es completo.");
            return FALSE;
        } // end if
        if (camp->restrictcampo() & DBCampo::DBRequired
                && camp->text() == "") {
            _depura("SubForm3::campoCompleto", 0, "El campo no es completo.");
            return FALSE;
        } // end if

    } // end for
    _depura("END SubForm3::campoCompleto", 0);
    return TRUE;
}


/// M&eacute;todo que se dispara cuando se va a reposicionar en el formulario.
/// Se encarga de resituar el cursor al lugar que se haya indicado.
/**
\param row
\param col
**/
void SubForm3::on_mui_list_cellRePosition(int row, int col) {
    _depura("SubForm3::on_mui_list_cellReposition", 0, "Row: " + QString::number(row) + " col: " + QString::number(col));

    bool creado = FALSE;

    int key = mui_list->m_teclasalida;

    if (row == mui_list->rowCount() - 1 && campoCompleto(row)) {
        nuevoRegistro();
        creado = TRUE;
    } // end if

    switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Tab:
            if (!m_insercion) {
                /// Se ha hecho un enter sobre una tabla sin insercion con lo que lanzamos un doble click para que sea
                /// La accion simulada.
                QTableWidgetItem *item = mui_list->currentItem();
                emit itemDoubleClicked(item);
                emit cellDoubleClicked(row, col);
            } else {
                situarse(row, col);
            } // end if
            break;
        case Qt::Key_Down:
            situarse(row, col);
            situarse1(row, col);
            if (creado) {
                mui_list->setCurrentCell(row + 1, col);
            } // end if
            break;
    } // end switch

    _depura("END SubForm3::on_mui_list_cellRePosition", 0);
}


/// M&eacute;todo que se dispara cuando se termina de editar un campo del Subformulario.
/**
\param row
\param col
**/
void SubForm3::on_mui_list_cellChanged(int row, int col) {
    _depura("SubForm3::on_mui_list_cellChanged", 0, "Row: "+QString::number(row) + " col: "+QString::number(col));
    emit editFinish(row, col);
    _depura("END SubForm3::on_mui_list_cellChanged", 0);
}


/// M&eacute;todo para agregar cabeceras al listado
/** Cada columna de un subformulario tiene una instancia de la clase SHeader. Este m&eacute;todo
    se encarga de crear e inicializar cada una de dichas clases. */
/**
\param nom
\param typ
\param res
\param opt
\param nomp
\return
**/
int SubForm3::addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
    _depura("SubForm3::addSHeader (" + nom + ")", 0);
    SHeader *camp = new SHeader(nom, typ, res, opt, nomp);
    camp->set
    ("");
    m_lcabecera.append(camp);
    mui_listcolumnas->insertRow(mui_listcolumnas->rowCount());
    /// 19/07/2007
    /// Cambiado QTableWidgetItem por QTableWidgetItem2 para usar nuevas funcionalidades.
    QTableWidgetItem2 *it = new QTableWidgetItem2("");
    it->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

    if (opt & SHeader::DBNoView) {
        mui_list->hideColumn(mui_listcolumnas->rowCount() -1);
        it->setCheckState(Qt::Unchecked);
    } else {
        it->setCheckState(Qt::Checked);
    } // end if

    if (opt & SHeader::DBBlockView) {
        it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    } // end if

    mui_listcolumnas->setItem(mui_listcolumnas->rowCount() - 1, 0, it);
    it = new QTableWidgetItem2(nom);
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount() - 1, 1, it);
    it = new QTableWidgetItem2(nomp);
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount() - 1, 2, it);
    it = new QTableWidgetItem2("");
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount() - 1, 3, it);

    _depura("END SubForm3::addSHeader (" + nom + ")", 0);
    return 0;
}


/** Establece el valor de toda la columna.
*/
/**
\param campo
\param valor
**/
void SubForm3::setColumnValue(QString campo, QString valor) {
    _depura("SubForm3::setColumnValue", 0, campo +" -- "+valor);
    SDBRecord *rec;

    for (int i = 0; i < mui_list->rowCount(); ++i) {
        rec = lineaat(i);
        if (rec) {
            rec->setDBvalue(campo, valor);
        } // end if
    } // end for
    _depura("END SubForm3::setColumnValue", 0);
}


///
/**
\param campo
\return
**/
Fixed SubForm3::sumarCampo(QString campo) {
    _depura("SubForm3::sumarCampo", 0);
    Fixed total;
    SDBRecord *rec;
    for (int i = 0; i < mui_list->rowCount(); ++i) {
        rec =  lineaat(i);
        if (rec) {
            Fixed subtotal = Fixed(rec->DBvalue(campo));
            total = total + subtotal;
        } // end if
    } // end for
    _depura("END SubForm3::sumarCampo", 0);
    return total;
}


///
/**
\param campo
\param row
**/
QString SubForm3::DBvalue(QString campo, int row) {
    _depura("SubForm3::DBvalue", 0);
    try {
        SDBRecord *rec;
        if (row == -1)
            rec = lineaact();
        else
            rec=lineaat(row);
        if (rec == NULL)
            throw -1;
        _depura("END SubForm3::DBvalue", 0);
        return rec->DBvalue(campo);
    } catch (...) {
        mensajeInfo("Fila inexistente");
        throw -1;
    }
}


///
/**
\param campo Nombre de la columna que debe cambiarse.
\param row   Fila correspondiente a la casilla
\param valor Valor que tomará la casilla
**/
void SubForm3::setDBvalue(QString campo, int row, QString valor) {
    _depura("SubForm3::setDBvalue", 0);
    try {
        SDBRecord *rec;
        if (row == -1)
            rec = lineaact();
        else
            rec=lineaat(row);
        if (rec == NULL)
            throw -1;
        rec->setDBvalue(campo, valor);
        _depura("END SubForm3::setDBvalue", 0);
    } catch (...) {
        mensajeInfo("Fila inexistente");
        throw -1;
    }
}


///
/**
\return
**/
int SubForm3::guardar() {
    _depura("SubForm3::guardar", 0);
    try {
        SDBRecord *rec;
        /// Borramos los elementos marcados para ser borrados.
        while (!m_listaborrar.isEmpty()) {
            rec = m_listaborrar.takeFirst();
            if (rec) {
                rec->borrar();
            } // end if
        } // end while

        /// Asegura que siempre la ultima linea se valide antes de guardar.
        /// Esto evita que se pueda perder informacion.
        if (campoCompleto(mui_list->rowCount() - 1)) {
            nuevoRegistro();
        } // end if

        /// Si no hay elementos que guardar salimos.
        if (mui_list->rowCount() == 0 || ((mui_list->rowCount() == 1) && m_insercion)) {
            return 0;
        } // end if

        /// Hacemos el guardado
        for (int j = 0; j < mui_list->rowCount() - 1; ++j) {
            rec = lineaat(j);
            if (rec) {
                if (m_orden) {
                    rec->setDBvalue("orden" + m_tablename, QString::number(j));
                } // end if
                rec->refresh();
                rec->guardar();
            } // end if
        } // end for

        /// Si no hay modo insercion hacemos el guardado de la ultima linea.
        if (!m_insercion) {
            rec = lineaat(mui_list->rowCount() - 1);
            if (m_orden)
                rec->setDBvalue("orden" + m_tablename, QString::number(mui_list->rowCount() - 1));
            rec->refresh();
            rec->guardar();
        } // end if

        /// Liberamos memoria
        while (!m_listaborrar.isEmpty()) {
            rec = m_listaborrar.takeFirst();
            if (rec) {
                delete rec;
            } // end if
        } // end while

        _depura("END SubForm3::guardar", 0);
        return 0;
    } catch (int e) {
        if (e == 100) {
            throw -1;
        } // end if
    } catch (...) {
        mensajeError("Error inesperado en el guardado. [SubForm3::guardar]");
        throw -1;
    } // end try
    return -1;
}


///
/**
\return
**/
int SubForm3::borrar() {
    _depura("SubForm3::borrar", 0);
    SDBRecord *rec;
    int i = 0;
    int error = 0;
    try {
        for (rec = m_lista.at(i++); i < m_lista.count(); rec = m_lista.at(i++)) {
            error = rec->borrar();
            if (error)
                return -1;
        } // end for
        if (!m_insercion) {
            rec = m_lista.at(m_lista.count() - 1);
            error = rec->borrar();
        } // end if
        _depura("END SubForm3::borrar", 0);
        return error;
    } catch (...) {
        mensajeError("Error al borrar. [SubForm3::borrar]");
        return -1;
    } // end try
}


///
/**
\param row
\return
**/
int SubForm3::borrar(int row) {
    _depura("SubForm3::borrar", 0);
    try {
        SDBRecord *rec, *rac;
        SDBCampo *camp;

        rac = new SDBRecord(empresaBase());

        /// Cogemos el elemento correspondiente, partimos de mui_list, tb podriamos usar lineaat
        rec = lineaat(row);
        if (!rec)
            return -1;

        /// Agregamos el elemento a la lista de borrados
        m_listaborrar.append(rac);
        m_lista.takeAt(m_lista.indexOf(rec));

        rac->setDBTableName(rec->tableName());
        rac->setDBCampoId(rec->campoId());
        rac->setNuevo(FALSE);

        /// Sacamos celda a celda toda la fila
        for (int i = 0; i < mui_list->columnCount(); i++) {
            camp = (SDBCampo *) mui_list->item(row, i);
            SDBCampo *it = new SDBCampo(rac, empresaBase(), camp->nomcampo(), camp->tipo(), camp->restrictcampo(), camp->nompresentacion());
            rac->lista()->append(it);
            it->set(camp->valorcampo());
        } // end for

        /// Nos aseguramos que ningun campo de la fila a borrar este en modo edicion.
        /// Evitamos que falle el programa.
        cerrarEditor();

        mui_list->removeRow(row);
        delete rec;

        /// Comprueba que no haya ninguna linea en el subformulario y crea una en blanco.
        if (m_insercion == TRUE && rowCount() == 0) {
            nuevoRegistro();
        } // end if

        /// Terminamos
        _depura("END SubForm3::borrar", 0);
        return 0;

    } catch (...) {
        mensajeInfo( "Error al intentar borrar");
        _depura("SubForm3::borrar error al borrar", 3);
        throw -1;
    } // end try
}


/// Metodo para ser derivado.
/**
\return
**/
int SubForm3::cerrarEditor() {
    _depura("END SubForm3::cerrarEditor", 0);
    return 0;
}


/// Guardamos el archivo de configuracion.
/**
**/
void SubForm3::guardaconfig() {
    _depura("SubForm3::guardaconfig", 0);
    QString aux = "";
    QFile file(confpr->valor(CONF_DIR_USER) + m_fileconfig + "tablecfn.cfn");
    /// Guardado del orden y de configuraciones varias.
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << mui_list->colorden() << "\n";
        stream << mui_list->tipoorden() << "\n";
        stream << mui_filaspagina->text() << "\n";

        /// Guardado del ancho de las columnas
        for (int i = 0; i < mui_list->columnCount(); i++) {
            mui_list->showColumn(i);
            stream << mui_list->columnWidth(i) << "\n";
        } // end for

        /// Guarda la visibilidad de los elementos consultando la tabla de configuracion.
        for (int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
            if (mui_listcolumnas->item(i, 0)->checkState() == Qt::Checked)
                stream << "1" << "\n";
            else
                stream << "0" << "\n";
        } // end for
        file.close();
    } // end if
    _depura("END SubForm3::guardaconfig", 0);
}


///
/**
**/
void SubForm3::cargaconfig() {
    _depura("SubForm3::cargaconfig", 0);
    QFile file(confpr->valor(CONF_DIR_USER) + m_fileconfig + "tablecfn.cfn");
    QString line;
    int error = 1;
    if (file.open(QIODevice::ReadOnly)) {
        error = 0;
        QTextStream stream(&file);
        /// Establecemos la columna de ordenaci&oacute;n
        QString linea = stream.readLine();
        mui_list->setcolorden(linea.toInt());

        /// Establecemos el tipo de ordenaci&oacute;n
        linea = stream.readLine();
        mui_list->settipoorden(linea.toInt());

        /// Establecemos el n&uacute;mero de filas por p&aacute;gina
        linea = stream.readLine();
        if (linea.toInt() > 0) {
            mui_filaspagina->setValue(linea.toInt());
        } // end if

        /// Establecemos el ancho de las columnas.
        for (int i = 0; i < mui_list->columnCount(); i++) {
            linea = stream.readLine();
            if (linea.toInt() > 0) {
                mui_list->setColumnWidth(i, linea.toInt());
            } else {
                mui_list->setColumnWidth(i, 30);
                error = 1;
            } // end if
        } // end for

        /// Leemos el status de las columnas.
        for (int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
            linea = stream.readLine();
            if (linea == "1") {
                mui_listcolumnas->item(i, 0)->setCheckState(Qt::Checked);
            } else if (linea == "0") {
                mui_listcolumnas->item(i, 0)->setCheckState(Qt::Unchecked);
            } else {
                error = 1;
            } // end if
        } // end for
        file.close();
        on_mui_confcol_clicked();
    } // end if

    /// Si se ha producido alg&uacute;n error en la carga hacemos un maquetado autom&aacute;tico.
    if (error)
        mui_list->resizeColumnsToContents();

    m_primero = FALSE;
    _depura("END SubForm3::cargaconfig", 0);
}


///
/**
**/
void SubForm3::on_mui_confcol_clicked() {
    _depura("SubForm3::on_mui_confcol_clicked", 0);
    for (int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
        if (mui_listcolumnas->item(i, 0)->checkState() == Qt::Checked)
            mui_list->showColumn(i);
        else
            mui_list->hideColumn(i);
    } // end for
    _depura("END SubForm3::on_mui_confcol_clicked", 0);
}


///
/**
**/
void SubForm3::on_mui_list_pressedSlash(int, int) {
    _depura ("SubForm3::on_mui_list_pressedSlash", 1, "pulsadoSlash aun no implementado");
}


///
/**
**/
void SubForm3::on_mui_list_pressedAsterisk(int, int) {
    _depura ("SubForm3::on_mui_list_pressedAsterisk", 1, "pressedAsterisk aun no implementado");
}


///
/**
**/
void SubForm3::on_mui_list_pressedPlus(int, int) {
    _depura ("SubForm3::on_mui_list_pressedPlus", 1, "pulsadoPlus aun no implementado");
}


///
/**
**/
void SubForm3::on_mui_list_pressedMinus(int, int) {
    _depura ("SubForm3::on_mui_list_pressedMinus", 1, "pressedMinus aun no implementado");
}


///
/**
\return
**/
QString SubForm3::imprimir() {
    _depura("SubForm3::imprimir", 0);
    QString fitxersortidarml = "<tr>\n";
    for (int h = 0; h < mui_listcolumnas->rowCount(); ++h) {
        if (mui_listcolumnas->item(h, 0)->checkState() == Qt::Checked) {
            fitxersortidarml += "    <td>" + XMLProtect(mui_listcolumnas->item(h, 2)->text()) + "</td>\n";
        } // end if
    } // end for
    fitxersortidarml += "</tr>\n";
    for (int i = 0; i < mui_list->rowCount(); ++i) {
        fitxersortidarml += "<tr>\n";
        for (int j = 0; j < mui_listcolumnas->rowCount(); ++j) {
            if (mui_listcolumnas->item(j, 0)->checkState() == Qt::Checked) {
                QString restante;
                fitxersortidarml += "    <td>" + XMLProtect(mui_list->item(i, j)->text()) + "</td>\n";
            } // end if
        } // end for
        fitxersortidarml += "</tr>\n";
    } // end for
    _depura("END SubForm3::imprimir", 0);
    return fitxersortidarml;
}


///
/**
\return
**/
void SubForm3::on_mui_confquery_clicked() {
    _depura("SubForm3::on_mui_confquery_clicked ", 0);
    if (empresaBase() == NULL) {
        mensajeInfo("no se ha inicializado bien la clase");
        return;
    } // end if
    mui_paginaact->setValue(1);
    cargar(mui_query->toPlainText());
//  cargar(m_query);
    _depura("END SubForm3::on_mui_confquery_clicked ", 0);
}


///
/**
\return
**/
void SubForm3::confquery() {
    _depura("SubForm3::confquery", 0);
    if (empresaBase() == NULL) {
        mensajeInfo("no se ha inicializado bien la clase");
        return;
    } // end if

    cargar(m_query);

    _depura("END SubForm3::confquery ", 0);
}


/// Disparador que se activa al haber pulsado ctrl+Arriba en la tabla
/// Hace el intercambio con la fila inmediatamente superior.
/**
\param row
\param col
\return
**/
void SubForm3::on_mui_list_ctrlSubir(int row, int col) {
    _depura("SubForm3::on_mui_list_ctrlSubir", 0," (" + QString::number(row) + "," + QString::number(col) + ")");
    if ( ! m_orden)
        return;
    if (row >= mui_list->rowCount() -1 || row == 0)
        return;


    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(FALSE);
    mui_list->setCurrentCell(0, 0);

    m_procesacambios = FALSE;

    for (int i = 0; i < mui_list->columnCount(); ++i) {
        QTableWidgetItem *it = mui_list->takeItem(row, i);
        QTableWidgetItem *ot = mui_list->takeItem(row - 1, i);
        mui_list->setItem(row, i, ot);
        mui_list->setItem(row - 1, i, it);
    } // end for
    mui_list->setCurrentCell(row - 1, col);
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(m_sorting);

    m_procesacambios = TRUE;
    _depura("END SubForm3::on_mui_list_ctrlSubir", 0);
}


/// Disparador que se activa al haber pulsado ctrl+Abajo en la tabla
/// Hace el intercambio con la fila inmediatamente inferior.
/**
\param row
\param col
\return
**/
void SubForm3::on_mui_list_ctrlBajar(int row, int col) {
    _depura("SubForm3::on_mui_list_ctrlBajar", 0);
    if (! m_orden)
        return;
    if (row >= mui_list->rowCount() -2)
        return;
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(FALSE);
    m_procesacambios = FALSE;

    if (row != 0)
        mui_list->setCurrentCell(0, 0);
    else
        mui_list->setCurrentCell(mui_list->rowCount() - 1, 0);

    for (int i = 0; i < mui_list->columnCount(); ++i) {
        QTableWidgetItem *it = mui_list->takeItem(row, i);
        mui_list->setItem(row, i, mui_list->takeItem(row + 1, i));
        mui_list->setItem(row + 1, i, it);
    } // end for
    mui_list->setCurrentCell(row + 1, col);
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(m_sorting);
    m_procesacambios = TRUE;

    _depura("END SubForm3::on_mui_list_ctrlBajar", 0);
}


///
/**
**/
void SubForm3::on_mui_pagsiguiente_clicked() {
    _depura("SubForm3::on_mui_pagsiguiente_clicked", 0);
    int pag = mui_paginaact->text().toInt();
    pag++;
    mui_paginaact->setValue(pag);
    confquery();
    _depura("END SubForm3::on_mui_pagsiguiente_clicked", 0);
}

///
/**
**/
void SubForm3::on_mui_paganterior_clicked() {
    _depura("SubForm3::on_mui_paganterior_clicked", 0);
    int pag = mui_paginaact->text().toInt();
    if (pag > 1)
        pag--;
    mui_paginaact->setValue(pag);
    confquery();
    _depura("END SubForm3::on_mui_paganterior_clicked", 0);
}


///
/**
\param titular
**/
void SubForm3::imprimirPDF(const QString &titular) {
    _depura("SubForm3::imprimir", 0);

    /// Los listados siempre usan la misma plantilla para imprimir listado.
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "listado.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "listado.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system(archivo.toAscii());
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii());

    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);

    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;

    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += imprimir();
    fitxersortidatxt += "</blockTable>";

    /// Reemplazamos en la impresion el parametro [story] por el resultado de la impresion
    buff.replace("[story]", fitxersortidatxt);
    /// La plantilla de listados admite unicamente el parametro titular que es lo que cambia
    /// en todos los listados
    buff.replace("[titulo]", titular);

    if (file.open(QIODevice::WriteOnly)) {

        QTextStream stream(&file);
        stream << buff;

        file.close();
    } // end if

    invocaPDF("listado");
    _depura("END SubForm3::imprimir", 0);
}

///
/**
\return
**/
void SubForm3::contextMenuEvent(QContextMenuEvent *) {
    _depura("SubForm3::contextMenuEvent", 0);
    QAction *del = NULL;
    int row = currentRow();
    if (row < 0)
        return;
    int col = currentColumn();
    if (row < 0)
        return;

    QMenu *popup = new QMenu(this);

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu(popup);

    /// Lanzamos la propagacion del menu a traves de las clases derivadas.
    creaMenu(popup);

    if (m_delete) {
        del = popup->addAction(tr("Borrar registro"));
        popup->addSeparator();
    } // end if
    QAction *ajustc = popup->addAction(tr("Ajustar columa"));
    QAction *ajustac = popup->addAction(tr("Ajustar altura"));

    QAction *ajust = popup->addAction(tr("Ajustar columnas"));
    QAction *ajusta = popup->addAction(tr("Ajustar alturas"));

    popup->addSeparator();
    QAction *verconfig = popup->addAction(tr("Ver/Ocultar configurador de subformulario"));

    QAction *opcion = popup->exec(QCursor::pos());

    /// Si no hay ninguna opcion pulsada se sale sin hacer nada
    if (!opcion) return;
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
    if (opcion == verconfig)
        toogleConfig();

    emit trataMenu(opcion);

    /// Activamos las herederas.
    procesaMenu(opcion);

    delete popup;
    _depura("END SubForm3::contextMenuEvent", 0);
}

///
/**
**/
void SubForm3::toogleConfig() {
    _depura("SubForm3::toogleConfig", 0);
    mui_configurador->setVisible(mui_configurador->isHidden());
    emit toogledConfig(mui_configurador->isVisible());
    _depura("END SubForm3::toogleConfig", 0);
}


///
/**
**/
void SubForm3::on_mui_botonCerrar_clicked() {
    _depura("SubForm3::on_mui_botonCerrar_clicked", 0);
    toogleConfig();
    _depura("END SubForm3::on_mui_botonCerrar_clicked", 0);
}


///
/**
**/
void SubForm3::on_mui_list_itemChanged(QTableWidgetItem *) {
    _depura("SubForm3::on_mui_list_itemChanged", 0);
    _depura("END SubForm3::on_mui_list_itemChanged", 0);
}


///
/**
\param b
**/
void SubForm3::setinsercion(bool b) {
    _depura("SubForm3::setinsercion", 0);
    m_insercion = b;
    _depura("END SubForm3::setinsercion", 0);
}


/// Establece si el subformulario debe considerar y tratar los cambios de celdas
/**
\param proc
**/
void SubForm3::setProcesarCambios(bool proc) {
    _depura("SubForm3::setProcesarCambios", 0);
    m_procesacambios = proc;
    _depura("END SubForm3::setProcesarCambios", 0);
}


/// Indica si el subformulario esta considerando el tratamiento de celdas modificadas
/**
\return
**/
bool SubForm3::procesaCambios() {
    _depura("SubForm3::procesaCambios", 0);
    _depura("END SubForm3::procesaCambios", 0);
    return m_procesacambios;
}

