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

#include "subform3.h"


/// SubForm3, constructor de la clase base para subformularios.
SubForm3::SubForm3(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    _depura("SubForm3::SubForm3", 0);
    m_companyact = NULL;
    mui_list->setSelectionMode(QAbstractItemView::SingleSelection);
    mui_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    mui_list->setAlternatingRowColors(TRUE);
    mui_list->setSortingEnabled(FALSE); // Hay un Bug que impide ordenar bien los elementos.
    mui_list->horizontalHeader()->setMovable(TRUE);

    /// Capturamos la secuencia de teclas para hacer aparecer o desaparecer
    /// el panel de configuracion del subform3.
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+s"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(toogleConfig()));

    m_insercion = FALSE;
    m_primero = TRUE;
    m_sorting = FALSE;
    m_orden = FALSE;

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

    setDelete(TRUE);
    _depura("END SubForm3::SubForm3", 0);
}


/// Destructor de Clase que guarda la configuracion.
SubForm3::~SubForm3() {
    _depura("SubForm3::~SubForm3", 0);
    guardaconfig();
    _depura("END SubForm3::~SubForm3", 0);
}


void SubForm3::setSortingEnabled(bool sorting) {
    _depura("SubForm3::setSortingEnabled", 0);
    mui_list->setSortingEnabled(sorting);
    m_sorting = sorting;
    _depura("END SubForm3::setSortingEnabled", 0);
}


bool SubForm3::sortingEnabled() {
    _depura("SubForm3::sortingEnabled", 0);
    return m_sorting;
}


void SubForm3::setOrdenEnabled(bool sorting) {
    _depura("SubForm3::setOrdenEnabled", 0);
    m_orden = sorting;
    _depura("END SubForm3::setOrdenEnabled", 0);
}


bool SubForm3::ordenEnabled() {
    _depura("SubForm3::ordenEnabled", 0);
    return m_orden;
}

void SubForm3::on_mui_list_itemDoubleClicked(QTableWidgetItem *item) {
    _depura("SubForm3::on_mui_list_itemDoubleClicked", 0);
    emit itemDoubleClicked(item);
    _depura("END SubForm3::on_mui_list_itemDoubleClicked", 0);
}


void SubForm3::on_mui_list_cellDoubleClicked(int row, int col) {
    _depura("SubForm3::on_mui_list_cellDoubleClicked", 0);
    emit cellDoubleClicked(row, col);
    _depura("END SubForm3::on_mui_list_cellDoubleClicked", 0);
}


/// Se encarga de crear un nuevo registro (una fila entera) y de inicializarla para que
/// tenga todos los elementos necesarios (columnas).
/// Nota: Esta funcion es de uso interno, no debe ser usada.
SDBRecord *SubForm3::newSDBRecord() {
    _depura("SubForm3::newSDBRecord", 0);
    SDBRecord *rec = new SDBRecord(m_companyact);
    rec->setDBTableName(m_tablename);
    rec->setDBCampoId(m_campoid);
    SHeader * linea;
    for (int i = 0; i < m_lcabecera.size(); ++i) {
        linea = m_lcabecera.at(i);
        rec->addDBCampo(linea->nomcampo(), linea->tipo(), linea->restricciones(), linea->nompresentacion());
    } // end for

    SDBCampo*camp;
    for (int i = 0; i < rec->lista()->size(); ++i) {
        camp = (SDBCampo *) rec->lista()->at(i);
        SHeader *head = m_lcabecera.at(i);
        Qt::ItemFlags flags = 0;
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if (!(head->options() & SHeader::DBNoWrite))
            flags |= Qt::ItemIsEditable;
        if (head->tipo() == DBCampo::DBboolean)
            flags |= Qt::ItemIsUserCheckable;
        camp->setFlags(flags);
    } // end for
    _depura("END SubForm3::newSDBRecord", 0);
    return rec;
}


/// Este metodo crea el registro final cuando se trata de subformularios con la opcion
/// de insertar nuevos registros en el subformulario.
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

    /// Activamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(m_sorting);

    _depura("END SubForm3::nuevoRegistro", 0);
}


/// Pinta los titulares en la tabla.
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
void SubForm3::situarse(unsigned int row, unsigned int col) {
    _depura("SubForm3::situarse", 0, QString::number(row)+" "+QString::number(col));
    unsigned int nrow = row;
    unsigned int ncol = col;
    SHeader *linea = m_lcabecera.at(ncol);
    if(!linea)
        return;
    bool invalido = TRUE;

    /// Mientras no se encuentre un candidato y haya candidatos
    while (invalido && nrow < row +2) {
        ncol++;
        if (ncol == (unsigned int ) m_lcabecera.count()) {
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
    _depura("END SubForm3::situarse", 0);
}

/// Se situa en una celda especifica del subformulario.
/** Se incluye la celda incluye la actual */
void SubForm3::situarse1(unsigned int row, unsigned int col) {
    _depura("SubForm3::situarse1", 0, QString::number(row)+" "+QString::number(col));
    unsigned int nrow = row;
    unsigned int ncol = col;
    SHeader *linea = m_lcabecera.at(ncol);
    if(!linea)
        return;
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
void SubForm3::pintar() {
    _depura("SubForm3::pintar", 0);
    mui_list->setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero)
        cargaconfig();
    nuevoRegistro();
    _depura("END SubForm3::pintar", 0);
}


/// Carga una tabla a partir del recordset que se le ha pasado.
int SubForm3::inicializar() {
    _depura("SubForm3::inicializar", 0);
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
    if (m_primero)
        cargaconfig();

    nuevoRegistro();
    /// Ordenamos la tabla.
    mui_list->ordenar();
    /// configuramos que registros son visibles y que registros no lo son.
    on_mui_confcol_clicked();
    _depura("END SubForm3::inicializar", 0);
    return 0;
}


/// Carga una tabla a partir del recordset que se le ha pasado.
/** Este m&eacute;todo genera, a partir del recordset pasado como par&aacute;metro el listado y lo muestra. */
void SubForm3::cargar(cursor2 *cur) {
    _depura("SubForm3::cargar", 0);
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(FALSE);
    /// Ponemos la consulta a la vista para que pueda ser editada.
    mui_query->setPlainText(cur->query());
    SDBRecord *rec;

    /// Tratramos con la paginacion.
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
    /// Ponemos los datos sobre la consulta.
    mui_numfilas->setText(QString::number(cur->numregistros()));
    int numpag = cur->numregistros() / filpag + 1;
    mui_numpaginas->setText(QString::number(numpag));
    /// Desplazamos hasta encontrar la p&aacute;gina adecuada.
    int nr = filpag * (pagact - 1);
    while (nr > 0  && !cur->eof()) {
        cur->siguienteregistro();
        nr--;
    } // end while
    /// Recorremos el recordset y ponemos los registros en un orden determinado.
    while (!cur->eof() && m_lista.count() < filpag) {
        SDBRecord *rec = newSDBRecord();
        rec->DBload(cur);
        m_lista.append(rec);
        cur->siguienteregistro();
    } // end while
    /// Inicializamos las columnas y pintamos las cabeceras.
    mui_list->setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero)
        cargaconfig();
    /// Inicializamos la tabla con las filas necesarias.
    mui_list->setRowCount(m_lista.count());

    SDBRecord *reg;
    SDBCampo *camp;
    for (int i = 0; i < m_lista.size(); ++i) {
        reg = m_lista.at(i);
	QRegExp rx("^.*00:00:00.*$"); // Para emparejar los valores fechas
        for (int j = 0; j < reg->lista()->size(); ++j) {
            camp = (SDBCampo *) reg->lista()->at(j);
	    // Si es una fecha lo truncamos a 10 caracteres para presentar solo la fecha
	    if(rx.exactMatch(camp->valorcampo()))
		camp->set(camp->valorcampo().left(10));
            mui_list->setItem(i, j, camp);
        } // end for
    } // end for
    nuevoRegistro();
    /// Ordenamos la tabla.
    mui_list->ordenar();
    /// configuramos que registros son visibles y que registros no lo son.
    on_mui_confcol_clicked();
    /// Reactivamos el sorting
    mui_list->setSortingEnabled(m_sorting);
    resizeColumnsToContents();
    resizeRowsToContents();
    _depura("END SubForm3::cargar", 0);
}


void SubForm3::cargar(QString query) {
    cursor2 *cur = m_companyact->cargacursor(query);
    cargar(cur);
    delete cur;
}

/// Devuelve la linea que se esta tratando actualmente.
SDBRecord *SubForm3::lineaact() {
    _depura("SubForm3::lineaact()\n", 0);
    return lineaat(mui_list->currentRow());
}


/// Devuelve la linea especificada o NULL si ésta no existe.
SDBRecord *SubForm3::lineaat(int row) {
    _depura("SubForm3::lineaat()", 0);

    /// Si la lista no tiene suficientes elementos devolvemos NULL
    if (mui_list->rowCount() < row || row < 0) {
        return NULL;
    } // end if

    /// Seleccionamos el campo especificado y lo devolvemos.
    SDBCampo *camp = (SDBCampo*) mui_list->item(row, 0);
    if (!camp) {
        return NULL;
    } // end if
    SDBRecord *rec = (SDBRecord *) camp->pare();
    _depura("END SubForm3::lineaat()", 0);
    return rec;
}

/// Devuelve TRUE si el registro ha sido completamente rellenado.
bool SubForm3::campoCompleto(int row) {
    _depura("SubForm3::campoCompleto", 0);
    SDBCampo *camp;
    SHeader *header;
    /// Sacamos celda a celda toda la fila
    for (int i = 0; i < mui_list->columnCount(); i++) {
        camp = (SDBCampo *) mui_list->item(row,i);
        header = m_lcabecera.at(i);
        if (camp->restrictcampo() & DBCampo::DBNotNull
                && camp->text() == ""
                && !(header->options() & SHeader::DBNoView)
                && camp->tipo() != DBCampo::DBboolean
           ) {
            _depura("SubForm3::campoCompleto", 0, "El campo no es completo.");
            return FALSE;
        } // end if
    } // end for
    return TRUE;
    _depura("END SubForm3::campoCompleto", 0);
}


/// M&eacute;todo que se dispara cuando se termina de editar un campo del Subformulario.
void SubForm3::on_mui_list_editFinished(int row, int col, int key) {
    _depura("SubForm3::on_mui_list_editFinished", 0);
    emit editFinish(row, col);
    bool creado = FALSE;

    if (row == mui_list->rowCount() - 1 && campoCompleto( row)) {
        nuevoRegistro();
        creado = TRUE;
    } // end if

    switch(key) {

    case Qt::Key_Return:
    case Qt::Key_Enter:
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


    _depura("END SubForm3::on_mui_list_editFinished", 0);
}


/// M&eacute;todo para agregar cabeceras al listado
/** Cada columna de un subformulario tiene una instancia de la clase SHeader. Este m&eacute;todo
    se encarga de crear e inicializar cada una de dichas clases. */
int SubForm3::addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
    _depura("SubForm3::addSHeader (" + nom + ")", 0);
    SHeader *camp = new SHeader(nom, typ, res, opt, nomp);
    camp->set
    ("");
    m_lcabecera.append(camp);
    mui_listcolumnas->insertRow(mui_listcolumnas->rowCount());
    QTableWidgetItem *it = new QTableWidgetItem("");
    it->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    it->setCheckState(Qt::Checked);
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount() - 1, 0, it);
    it = new QTableWidgetItem2(nom);
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount() - 1, 1, it);
    it = new QTableWidgetItem2(nomp);
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount() - 1, 2, it);
    it = new QTableWidgetItem2("");
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount() - 1, 3, it);
    return 0;
}



/** Establece el valor de toda la columna.
*/
void SubForm3::setColumnValue(QString campo, QString valor) {
    _depura("SubForm3::setColumnValue", 0);
    SDBRecord *rec;
    int restaporinsercion;

    if (!m_insercion) {
        restaporinsercion=0;
    } else {
        restaporinsercion = 1;
    } // end if

    for (int i = 0; i < mui_list->rowCount() - restaporinsercion; ++i) {
        rec =  lineaat(i);
        if (rec)
            rec->setDBvalue(campo, valor);
    } // end for
    _depura("END SubForm3::setColumnValue", 0);
}

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

        /// Si no hay elementos que guardar salimos.
        if(mui_list->rowCount() == 0 || ( (mui_list->rowCount() == 1) && m_insercion)) {
            return 0;
        } // end if

        /// Hacemos el guardado
        for (int j = 0; j < mui_list->rowCount() - 1; ++j) {
            rec = lineaat(j);
            if (rec) {
                if (m_orden)
                    rec->setDBvalue("orden"+m_tablename, QString::number(j));
                rec->refresh();
                rec->guardar();
            } // end if
        } // end for

        /// Si no hay modo insercion hacemos el guardado de la ultima linea.
        if (!m_insercion) {
            rec = lineaat(mui_list->rowCount() - 1);
            if (m_orden)
                rec->setDBvalue("orden"+m_tablename, QString::number(mui_list->rowCount() - 1));
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
    } catch (...) {
        mensajeInfo("error inesperado en el guardado, salimos devolviento -1");
        _depura("Error inesperado en el guardado,", 3);
        throw -1;
    } // end try
}


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
    } catch(...) {
        _depura("SubForm3::borrar() Error al borrar", 3);
        return -1;
    } // end try
}


int SubForm3::borrar(int row) {
    _depura("SubForm3::borrar", 0);
    try {
        SDBRecord *rec, *rac;
        SDBCampo *camp;

        rac = new SDBRecord(m_companyact);

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
            camp = (SDBCampo *) mui_list->item(row,i);
            SDBCampo * it= new SDBCampo(rac, m_companyact, camp->nomcampo(), camp->tipo(), camp->restrictcampo(), camp->nompresentacion());
            rac->lista()->append(it);
            it->set
            ( camp->valorcampo());
        } // end for

        _depura("quitamos la columna del listado", 0);
        mui_list->removeRow(row);

        delete rec;

        /// Terminamos
        _depura("END SubForm3::borrar", 0);
        return 0;

    } catch (...) {
        mensajeInfo( "Error al intentar borrar");
        _depura("SubForm3::borrar error al borrar", 3);
        throw -1;
    } // end try
}


/// Guardamos el archivo de configuracion.

void SubForm3::guardaconfig() {
    _depura("SubForm3::guardaconfig", 0);
    QString aux = "";
    QFile file(confpr->valor(CONF_DIR_USER) + m_fileconfig + "tablecfn.cfn");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << mui_list->colorden() << "\n";
        stream << mui_list->tipoorden() << "\n";
        stream << mui_filaspagina->text() << "\n";
        for (int i = 0; i < mui_list->columnCount(); i++) {
            mui_list->showColumn(i);
            stream << mui_list->columnWidth(i) << "\n";
        } // end for
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


void SubForm3::cargaconfig() {
    _depura("SubForm3::cargaconfig", 0);
    QFile file(confpr->valor(CONF_DIR_USER) + m_fileconfig + "tablecfn.cfn");
    QString line;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString linea = stream.readLine();
        mui_list->setcolorden(linea.toInt());
        linea = stream.readLine();
        mui_list->settipoorden(linea.toInt());
        linea = stream.readLine();
        //mui_filaspagina->setText(linea);
        mui_filaspagina->setValue(linea.toInt());
        for (int i = 0; i < mui_list->columnCount(); i++) {
            linea = stream.readLine();
            if (linea.toInt() > 0)
                mui_list->setColumnWidth(i, linea.toInt());
            else
                mui_list->setColumnWidth(i, 3);
        } // end for
        for (int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
            linea = stream.readLine();
            if (linea == "1")
                mui_listcolumnas->item(i, 0)->setCheckState(Qt::Checked);
            else
                mui_listcolumnas->item(i, 0)->setCheckState(Qt::Unchecked);
        } // end for
        on_mui_confcol_clicked();
        file.close();
    } else {
        mui_list->resizeColumnsToContents();
    } // end if
    m_primero = FALSE;
    _depura("END SubForm3::cargaconfig", 0);
}


void SubForm3::on_mui_confcol_clicked() {
    for (int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
        if (mui_listcolumnas->item(i, 0)->checkState() == Qt::Checked)
            mui_list->showColumn(i);
        else
            mui_list->hideColumn(i);
    } // end for
}


void SubForm3::on_mui_list_pressedSlash(int, int) {
    _depura ("pulsadoSlash aun no implementado", 2);
}


void SubForm3::on_mui_list_pressedAsterisk(int, int) {
    _depura ("pressedAsterisk aun no implementado", 1);
}


void SubForm3::on_mui_list_pressedPlus(int, int) {
    _depura ("pulsadoPlus aun no implementado", 1);
}


void SubForm3::on_mui_list_pressedMinus(int, int) {
    _depura ("pressedMinus aun no implementado", 1);
}


QString SubForm3::imprimir() {
    QString fitxersortidatxt = "<tr>\n";
    for (int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
        if (mui_listcolumnas->item(i, 0)->checkState() == Qt::Checked)
            fitxersortidatxt += "    <td>" + XMLProtect(mui_listcolumnas->item(i, 2)->text()) + "</td>\n";
    } // end for
    fitxersortidatxt += "</tr>\n";
    for (int i = 0; i < mui_list->rowCount(); ++i) {
        fitxersortidatxt += "<tr>\n";
        for (int j = 0; j < mui_listcolumnas->rowCount(); ++j) {
            if (mui_listcolumnas->item(j, 0)->checkState() == Qt::Checked) {
                fitxersortidatxt += "    <td>" + XMLProtect(mui_list->item(i, j)->text()) + "</td>\n";
            } // end if
        } // end for
        fitxersortidatxt += "</tr>\n";
    } // end for
    return fitxersortidatxt;
}


void SubForm3::on_mui_confquery_clicked() {
    _depura("SubForm3::on_mui_confquery_clicked ", 0);
    if (m_companyact == NULL) {
        mensajeInfo("no se ha inicializado bien la clase");
        return;
    } // end if
    cursor2 *cur = m_companyact->cargacursor(mui_query->toPlainText());
    cargar(cur);
    delete cur;
    _depura("END SubForm3::on_mui_confquery_clicked ", 0);
}


/// Disparador que se activa al haber pulsado ctrl+Arriba en la tabla
/// Hace el intercambio con la fila inmediatamente superior.
void SubForm3::on_mui_list_ctrlSubir(int row, int col) {
    _depura("SubForm3::on_mui_list_ctrlSubir", 0," (" + QString::number(row) + "," + QString::number(col) + ")");
    if ( ! m_orden)
        return;
    if (row >= mui_list->rowCount() -1 || row == 0)
        return;
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(FALSE);
    mui_list->setCurrentCell(0, 0);

    for (int i = 0; i < mui_list->columnCount(); ++i) {
        QTableWidgetItem *it = mui_list->takeItem(row, i);
        QTableWidgetItem *ot = mui_list->takeItem(row - 1, i);
        mui_list->setItem(row, i, ot);
        mui_list->setItem(row - 1, i, it);
    } // end for
    mui_list->setCurrentCell(row - 1, col);
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(m_sorting);
    _depura("END SubForm3::on_mui_list_ctrlSubir", 0);
}


/// Disparador que se activa al haber pulsado ctrl+Abajo en la tabla
/// Hace el intercambio con la fila inmediatamente inferior.
void SubForm3::on_mui_list_ctrlBajar(int row, int col) {
    _depura("SubForm3::on_mui_list_ctrlBajar", 0);
    _depura("SubForm3::on_mui_list_ctrlBajar", 0, " (Row, col)" + QString::number(row) + "--" + QString::number(col));
    _depura("SubForm3::on_mui_list_ctrlBajar", 0, " rowCount" + QString::number(mui_list->rowCount()) );

    if (! m_orden)
        return;
    if (row >= mui_list->rowCount() -2 )
        return;
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(FALSE);

    if (row != 0)
        mui_list->setCurrentCell(0, 0);
    else
        mui_list->setCurrentCell(mui_list->rowCount()-1, 0);

    for (int i = 0; i < mui_list->columnCount(); ++i) {
        QTableWidgetItem *it = mui_list->takeItem(row, i);
        mui_list->setItem(row, i, mui_list->takeItem(row + 1, i));
        mui_list->setItem(row + 1, i, it);
    } // end for
    mui_list->setCurrentCell(row + 1, col);
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled(m_sorting);
    _depura("END SubForm3::on_mui_list_ctrlBajar", 0);
}


void SubForm3::on_mui_pagsiguiente_clicked() {
    int pag = mui_paginaact->text().toInt();
    pag++;
    mui_paginaact->setValue(pag);
    on_mui_appag_clicked();
}


void SubForm3::on_mui_paganterior_clicked() {
    int pag = mui_paginaact->text().toInt();
    if (pag > 1)
        pag--;
    mui_paginaact->setValue(pag);
    on_mui_appag_clicked();
}

void SubForm3::imprimirPDF(const QString &titular) {
    _depura("SubForm3::imprimir", 0);
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

    /// Linea de totales del presupuesto
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]", fitxersortidatxt);
    buff.replace("[titulo]", titular);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    invocaPDF("listado");
    _depura("END SubForm3::imprimir", 0);
}


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

    if (m_delete)
        del = popup->addAction(tr("Borrar registro"));
    popup->addSeparator();
    QAction *ajustc = popup->addAction(tr("Ajustar columa"));
    QAction *ajustac = popup->addAction(tr("Ajustar altura"));

    QAction *ajust = popup->addAction(tr("Ajustar columnas"));
    QAction *ajusta = popup->addAction(tr("Ajustar alturas"));

    popup->addSeparator();
    QAction *verconfig = popup->addAction(tr("Ver/Ocultar configurador de subformulario"));

    QAction *opcion = popup->exec(QCursor::pos());
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
        toogleConfig();

    emit trataMenu(opcion);

    /// Activamos las herederas.
    procesaMenu(opcion);

    delete popup;
}

void SubForm3::on_mui_list_cellChanged(int, int) {}
void SubForm3::on_mui_list_itemChanged(QTableWidgetItem *) {}

void SubForm3::on_mui_list_currentCellChanged(int , int , int row, int col) {
    _depura("SubForm3::on_mui_list_currentCellChanged", 0);
    if (row >= 0 && col >= 0)   {
        on_mui_list_editFinished( row,  col, 0);
    } // end if
    _depura("END SubForm3::on_mui_list_currentCellChanged", 0);
}

void SubForm3::setinsercion(bool b) {
        m_insercion = b;
}
