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

#include "subform3.h"

/** SubForm3, constructor de la clase base para subformularios
  */
SubForm3::SubForm3(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    _depura("SubForm3::SubForm3",0);
    m_companyact = NULL;
    mui_list->setSelectionMode( QAbstractItemView::SingleSelection );
    mui_list->setSelectionBehavior( QAbstractItemView::SelectRows);
    mui_list->setAlternatingRowColors(TRUE);
    mui_list->setSortingEnabled(TRUE);
    mui_list->horizontalHeader()->setMovable(TRUE);

    m_insercion = FALSE;
    m_primero = TRUE;

    /// Para el listado de  columnas hacemos una inicializacion
    QStringList headers;
    headers << "Orden" << "Nombre" << "Nombre Campo" << "Visible";
    mui_listcolumnas->setColumnCount(4);
    mui_listcolumnas->setHorizontalHeaderLabels (headers);
    mui_listcolumnas->setShowGrid(FALSE);
    mui_listcolumnas->setColumnWidth(0,25);
    mui_listcolumnas->setColumnWidth(1,100);
    mui_listcolumnas->setColumnWidth(2,175);
    mui_listcolumnas->setColumnWidth(3,0);
    mui_listcolumnas->setSelectionBehavior ( QAbstractItemView::SelectRows );
    mui_listcolumnas->verticalHeader()->hide();

    /// Siempre que arrancamos mostramos la pagina 0
    mui_paginaact->setText("1");

    /// Ocultamos la configuracion
    hideConfig();

    /// Limpiamos la lista
    m_lista.clear();
    _depura("END SubForm3::SubForm3", 0);
}


/** Destructor de Clase que guarda la configuracion  */
SubForm3::~SubForm3() {
    _depura("SubForm3::~SubForm3",0);
    guardaconfig();
    _depura("END SubForm3::~SubForm3",0);
}


/** Se encarga de crear un nuevo registro (una fila entera) y de inicializarla para que tenga todos los elementos necesarios (columnas).
  */
SDBRecord *SubForm3::newSDBRecord() {
    _depura("SubForm3::newSDBRecord\n", 0);
    SDBRecord *rec = new SDBRecord(m_companyact);
    rec->setDBTableName( m_tablename);
    rec->setDBCampoId( m_campoid);
    SHeader * linea;
    for ( int i = 0; i < m_lcabecera.size(); ++i) {
        linea = m_lcabecera.at(i);
        rec->addDBCampo(linea->nomcampo(), linea->tipo(), linea->restricciones(), linea->nompresentacion() );
    }// end for

    SDBCampo*camp;
    for( int i = 0; i < rec->lista()->size(); ++i) {
        camp = (SDBCampo *) rec->lista()->at(i);
        SHeader *head = m_lcabecera.at(i);
        Qt::ItemFlags flags = 0;
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if (!(head->options() & SHeader::DBNoWrite))
            flags |= Qt::ItemIsEditable;
        if (head->tipo() == DBCampo::DBboolean)
            flags |= Qt::ItemIsUserCheckable;
        camp->setFlags(flags);
    }// end for
    _depura("END SubForm3::newSDBRecord\n", 0);
    return rec;
}


/** Este metodo crea el registro final cuando se trata de subformularios con la opcion de insertar nuevos registros en el subformulario.
*/
void SubForm3::nuevoRegistro() {
    _depura("SubForm3::nuevoRegistro\n",0);
    if (!m_insercion)
        return;
    SDBRecord *rec = newSDBRecord();

    m_lista.append(rec);

    mui_list->insertRow(m_lista.size()-1);
    SDBCampo *camp;
    for(int i = 0; i < rec->lista()->size(); ++i) {
        camp = (SDBCampo *) rec->lista()->at(i);
        mui_list->setItem(m_lista.size()-1,i,camp);
    }// end for
    _depura("END SubForm3::nuevoRegistro\n",0);
}

/** Pinta los titulares en la tabla  */
void SubForm3::pintaCabeceras() {
    _depura("SubForm3::pintaCabeceras",0);
    QStringList headers;
    SHeader * linea;
    for (int i = 0; i < m_lcabecera.size(); ++i) {
        linea = m_lcabecera.at(i);
        headers << linea->nompresentacion();
        if (linea->options() & SHeader::DBNoView)
            mui_list->hideColumn(i);
        else
            mui_list->showColumn(i);
    } // end for
    mui_list->setHorizontalHeaderLabels(headers);
    _depura("END SubForm3::pintaCabeceras",0);
}

/** Se situa en una celda especifica del subformulario */
void SubForm3::situarse(unsigned int row, unsigned int col) {
    _depura("SubForm3::situarse",0);
    unsigned int nrow = row;
    int ncol = col;
    SHeader *linea = m_lcabecera.at(ncol);
    bool invalido = TRUE;
    while(invalido) {
        ncol++;
        if (ncol == m_lcabecera.count()) {
            ncol=0;
            nrow++;
        }// end if
        linea = m_lcabecera.at(ncol);
        invalido = FALSE;
        if(linea->options() & SHeader::DBNoView)
            invalido = TRUE;
        if (linea->options() & SHeader::DBNoWrite)
            invalido = TRUE;
    }// end while
    mui_list->setCurrentCell(nrow, ncol);
    _depura("END SubForm3::situarse",0);
}

/** Cuando tenemos un registro que no se tiene que cargar (pq es nuevo o algo
 ** así) de la base de datos, con la funcion pintar lo dejamos en un estado que
 ** se podria considerar presentable para poder operar con el subformulario.
 **/
void SubForm3::pintar() {
    _depura("SubForm3::pintar",0);
    mui_list->setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero)
        cargaconfig();
    nuevoRegistro();
    _depura("END SubForm3::pintar",0);
}

/** Carga una tabla a partir del recordset que se le ha pasado */
int SubForm3::cargar(cursor2 *cur) {
    _depura("SubForm3::cargar",0);
    mui_query->setText(cur->query());
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
    while (m_lista.count() ) {
        rec = m_lista.takeFirst();
        if (rec)
            delete rec;
    }// end while

    /// Ponemos los datos sobre el query
    mui_numfilas->setText(QString::number(cur->numregistros()));
    int numpag = cur->numregistros() / filpag + 1;
    mui_numpaginas->setText(QString::number(numpag));

    /// Desplazamos hasta encontrar la página adecuada.
    int nr = filpag * (pagact - 1);
    while (nr > 0  && !cur->eof()) {
        cur->siguienteregistro();
	nr--;
    }// end while

    /// Recorremos el recordset y ponemos los registros en un orden determinado.
    while (!cur->eof() && m_lista.count() < filpag) {
        SDBRecord *rec = newSDBRecord();
        rec->DBload(cur);
        m_lista.append(rec);
        cur->siguienteregistro();
    }// end while

    /// Inicializamos las columnas y pintamos las cabeceras.
    mui_list->setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero)
        cargaconfig();

    /// Inicializamos la tabla con las filas necesarias.
    mui_list->setRowCount(m_lista.count());
    SDBRecord *reg;
    for ( int i = 0; i < m_lista.size(); ++i) {
        reg = m_lista.at(i);
        SDBCampo *camp;
        for ( int j =0; j < reg->lista()->size(); ++j) {
            camp = (SDBCampo *) reg->lista()->at(j);
            mui_list->setItem(i,j,camp);
        }// end for
    }// end for
    nuevoRegistro();

    /// Ordenamos la tabla.
    mui_list->ordenar();

    /// configuramos que registros son visibles y que registros no lo son.
    on_mui_confcol_clicked();
    _depura("END SubForm3::cargar",0);
    return 0;
}

/// Devuelve la linea que se esta tratando actualmente
SDBRecord *SubForm3::lineaact() {
    _depura("SubForm3::lineaact()\n",0);
    return lineaat(mui_list->currentRow());
}

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
SDBRecord *SubForm3::lineaat(int row) {
    _depura("SubForm3::lineaat()\n", 0);
    SDBCampo *camp =(SDBCampo*) mui_list->item(row,0);
    SDBRecord *rec = (SDBRecord *) camp->pare();
    _depura("END SubForm3::lineaat()\n", 0);

    return rec;
}

void SubForm3::on_mui_list_editFinished(int row, int col) {
    _depura("SubForm3::on_mui_list_editFinished",0);
    emit editFinish( row, col);
    if (row == mui_list->rowCount()-1)
        nuevoRegistro();
    situarse(row,col);
    _depura("END SubForm3::on_mui_list_editFinished",0);
}

int SubForm3::addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
    _depura("SubForm3::addSHeader ("+nom+")",0);
    SHeader *camp = new SHeader( nom, typ, res, opt, nomp);
    camp->set
    ("");
    m_lcabecera.append(camp);

    mui_listcolumnas->insertRow(mui_listcolumnas->rowCount());

    QTableWidgetItem * it = new QTableWidgetItem("");
    it->setFlags(Qt::ItemIsUserCheckable |Qt::ItemIsEnabled |Qt::ItemIsSelectable | Qt::ItemIsEditable);
    it->setCheckState(Qt::Checked);
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount()-1, 0, it);

    it = new QTableWidgetItem2(nom);
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount()-1, 1, it);

    it = new QTableWidgetItem2(nomp);
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount()-1, 2, it);

    it = new QTableWidgetItem2("");
    mui_listcolumnas->setItem(mui_listcolumnas->rowCount()-1, 3, it);

    return 0;
}

void SubForm3::setColumnValue(QString campo, QString valor) {
    _depura("SubForm3::setColumnValue", 0);
    SDBRecord *rec;
    for (int i = 0; i < mui_list->rowCount(); ++i) {
        rec =  lineaat(i);
        if (rec)
            rec->setDBvalue(campo,valor);
    }// end for
    _depura("END SubForm3::setColumnValue", 0);
}

QString SubForm3::DBvalue(QString campo, int row) {
    _depura("SubForm3::DBvalue", 0);
    SDBRecord *rec;
    if (row == -1)
        rec = lineaact();
    else
        rec=lineaat(row);
    _depura("END SubForm3::DBvalue", 0);
    return rec->DBvalue(campo);
}

int SubForm3::guardar() {
    _depura("SubForm3::guardar",0);
    try {
        SDBRecord *rec;
        int error=0;
        for (int j=0; j < mui_list->rowCount()-1; ++j) {
            rec = lineaat(j);
            rec->refresh();
            error = rec->guardar();
            if (error)
                throw(-1);
        }// end for
        if(!m_insercion) {
            rec = lineaat(mui_list->rowCount()-1);
            error = rec->guardar();
        }// end if
        _depura("END SubForm3::guardar",0);
        return error;
    } catch(...) {
        _depura("error inesperado en el guardado, salimos devolviento -1",1);
        throw -1;
    }
}

int SubForm3::borrar() {
    SDBRecord *rec;
    int i=0;
    int error = 0;
    for(rec=m_lista.at(i++); i<m_lista.count() ;rec=m_lista.at(i++)) {
        error = rec->borrar();
        if (error)
            return -1;
    }// end for
    if(!m_insercion) {
        rec = m_lista.at(m_lista.count()-1);
        error = rec->borrar();
    }// end if
    return error;
}

int SubForm3::borrar(int row) {
    SDBRecord *rec;
    rec = m_lista.at(row);
    int error = rec->borrar();
    if (error)
        return error;
    m_lista.takeAt(row);
    mui_list->removeRow(row);
    emit editFinish( row, 0);
    return 0;
}


/** Guardamos el archivo de configuracion */
void SubForm3::guardaconfig() {
    _depura("SubForm3::guardaconfig",0);
    QString aux = "";
    QFile file( confpr->valor(CONF_DIR_USER)+m_fileconfig+"tablecfn.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << mui_list->colorden() << "\n";
        stream << mui_list->tipoorden() << "\n";
        stream << mui_filaspagina->text() << "\n";

        for (int i = 0; i < mui_list->columnCount(); i++) {
            mui_list->showColumn(i);
            stream << mui_list->columnWidth(i) << "\n";
        }// end for

        for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
            if ( mui_listcolumnas->item(i,0)->checkState() == Qt::Checked)
                stream << "1" << "\n";
            else
                stream << "0" << "\n";
        }// end for
        file.close();
    }// end if
    _depura("END SubForm3::guardaconfig",0);
}

void SubForm3::cargaconfig() {
    _depura("SubForm3::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+m_fileconfig+"tablecfn.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        QString linea = stream.readLine();
        mui_list->setcolorden (linea.toInt());

        linea = stream.readLine();
        mui_list->settipoorden ( linea.toInt());

        linea = stream.readLine();
        mui_filaspagina->setText(linea);

        for (int i = 0; i < mui_list->columnCount(); i++) {
            linea = stream.readLine();
            if (linea.toInt() > 0)
                mui_list->setColumnWidth(i, linea.toInt());
            else
                mui_list->setColumnWidth(i, 3);
        }// end for

        for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
            linea = stream.readLine();
            if (linea == "1")
                mui_listcolumnas->item(i,0)->setCheckState(Qt::Checked);
            else
                mui_listcolumnas->item(i,0)->setCheckState(Qt::Unchecked);
        }// end for
        on_mui_confcol_clicked();
        file.close();
    } else {
        mui_list->resizeColumnsToContents();
    }// end if
    m_primero = FALSE;
    _depura("END SubForm3::cargaconfig",0);
}


void SubForm3::on_mui_confcol_clicked() {
    for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
        if (mui_listcolumnas->item(i,0)->checkState() == Qt::Checked)
            mui_list->showColumn(i);
        else
            mui_list->hideColumn(i);
    }// end for
}

void SubForm3::on_mui_list_pressedSlash(int , int ) {
    _depura ("pulsadoSlash aun no implementado",2);
}

void SubForm3::on_mui_list_pressedAsterisk(int , int ) {
    _depura ("pressedAsterisk aun no implementado",1);
}

void SubForm3::on_mui_list_pressedPlus(int , int ) {
    _depura ("pulsadoPlus aun no implementado",1);
}

QString SubForm3::imprimir() {
    QString fitxersortidatxt = "<tr>\n";
    for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i) {
        if (mui_listcolumnas->item(i,0)->checkState() == Qt::Checked)
            fitxersortidatxt += "   <td>"+XMLProtect(mui_listcolumnas->item(i,2)->text())+"</td>\n";
    }// end for
    fitxersortidatxt += "</tr>\n";

    for ( int i = 0; i < mui_list->rowCount(); ++i) {
        fitxersortidatxt += "<tr>\n";
        for (int j=0; j < mui_listcolumnas->rowCount(); ++j) {
            if (mui_listcolumnas->item(j,0)->checkState() == Qt::Checked) {
                fitxersortidatxt += "   <td>"+XMLProtect(mui_list->item(i,j)->text())+"</td>\n";
            }// end if
        }// end for
        fitxersortidatxt += "</tr>\n";
    }// end for
    return fitxersortidatxt;
}

void SubForm3::on_mui_confquery_clicked() {
    _depura("SubForm3::on_mui_confquery_clicked ",0);
    if (m_companyact == NULL) {
        mensajeInfo("no se ha inicializado bien la clase");
        return;
    }// end if
    cursor2 *cur = m_companyact->cargacursor(mui_query->text());
    cargar(cur);
    delete cur;
    _depura("END SubForm3::on_mui_confquery_clicked ", 0);
}

/** Disparador que se activa al haber pulsado ctrl+Arriba en la tabla
    Hace el intercambio con la fila inmediatamente superior */
void SubForm3::on_mui_list_ctrlSubir(int row, int col) {
    mui_list->setCurrentCell(0,0);
    row++;
    _depura("SubForm3::on_mui_list_ctrlSubir ("+QString::number(row)+","+QString::number(col)+")",0);
    if (row > 0) {
        for (int i=0; i < mui_list->columnCount(); ++i) {
            QTableWidgetItem *it = mui_list->takeItem(row,i);
            QTableWidgetItem *ot = mui_list->takeItem(row-1,i);
            mui_list->setItem(row  , i, ot);
            mui_list->setItem(row-1, i, it);
        }// end for
    }// end if
    mui_list->setCurrentCell(row-1,col);
    _depura("END SubForm3::on_mui_list_ctrlSubir",0);
}

/** Disparador que se activa al haber pulsado ctrl+Abajo en la tabla
    Hace el intercambio con la fila inmediatamente inferior*/
void SubForm3::on_mui_list_ctrlBajar(int row, int col) {
    mui_list->setCurrentCell(0,0);
    row--;
    _depura("SubForm3::on_mui_list_ctrlBajar",0);
    if (row < mui_list->rowCount()-1) {
        for (int i=0; i < mui_list->columnCount(); ++i) {
            QTableWidgetItem *it = mui_list->takeItem(row,i);
            mui_list->setItem(row,i, mui_list->takeItem(row+1,i));
            mui_list->setItem(row+1, i, it);
        }// end for
    }// end if
    mui_list->setCurrentCell(row+1,col);
    _depura("END SubForm3::on_mui_list_ctrlBajar",0);
}

