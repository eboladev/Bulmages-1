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


#include "subform.h"

void SDBRecord::refresh() {
    SDBCampo *camp;
    for (camp=(SDBCampo *) m_lista.first(); camp; camp= (SDBCampo *) m_lista.next())
        camp->refresh();

}


int SDBRecord::addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp) {
    SDBCampo *camp = new SDBCampo(this, m_conexionbase, nom, typ, res, nomp);
    camp->set
    ("");
    m_lista.append(camp);
    return 0;
}

/// -------------------------------------------

void SDBCampo::refresh() {
    if(tipo() == DBCampo::DBboolean) {
        DBCampo::set
            (checkState()==Qt::Checked?"TRUE":"FALSE");
    } else
        DBCampo::set
            (text());
}


int SDBCampo::set(QString val) {
    if(tipo() == DBCampo::DBboolean) {
        if (val == "TRUE" || val == "t")
            setCheckState(Qt::Checked);
        else
            setCheckState(Qt::Unchecked);

    } else
        QTableWidgetItem2::setText(val);

    DBCampo::set
        (val);
    return 0;
}



/// ------------------------------------------------------------

SHeader::SHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
    m_nomcampo = nom;
    m_valorcampo="";
    m_nompresentacion = nomp;
    m_restricciones = res;
    m_options = opt;
    m_tipo = typ;
}



/// ---------------------------------------------------------



SubForm2::SubForm2(QWidget *parent) : QTableWidget(parent) {
    setSelectionMode( QAbstractItemView::SingleSelection );
    setSelectionBehavior( QAbstractItemView::SelectRows);
    setAlternatingRowColors(TRUE);
    setSortingEnabled(TRUE);
    installEventFilter(this);
    m_insercion = FALSE;
    m_primero = TRUE;
    m_colorden = 0;
    m_tipoorden = 0;
}


SDBRecord *SubForm2::newSDBRecord() {
    _depura("SubForm2::newSDBRecord",0);
    SDBRecord *rec = new SDBRecord(m_companyact);
    rec->setDBTableName( m_tablename);
    rec->setDBCampoId( m_campoid);
    SHeader * linea;
    for ( int i = 0; i < m_lcabecera.size(); ++i) {
	linea = m_lcabecera.at(i);
        _depura("agregamos una cabecera",0);
        rec->addDBCampo(linea->nomcampo(), linea->tipo(), linea->restricciones(), linea->nompresentacion() );
    }// end for
    int j=0;
    SDBCampo*camp;
    for(camp =(SDBCampo *) rec->lista()->first(); camp; camp= (SDBCampo *) rec->lista()->next()) {
        SHeader *head = m_lcabecera.at(j);
        Qt::ItemFlags flags=0;
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if (!(head->options() & SHeader::DBNoWrite))
            flags |= Qt::ItemIsEditable;
        if (head->tipo() == DBCampo::DBboolean)
            flags |= Qt::ItemIsUserCheckable;
        camp->setFlags(flags);
        j++;
    }// end for
    _depura("END SubForm2::newSDBRecord",0);

    return rec;
}


/** Este metodo crea el registro final cuando se trata de subformularios con la opcion de insertar nuevos registros en el subformulario.
**/
void SubForm2::nuevoRegistro() {
    _depura("SubForm2::nuevoRegistro",0);
    if (!m_insercion)
        return;
    SDBRecord *rec = newSDBRecord();
    m_lista.append(rec);
    insertRow(rowCount());
    SDBCampo *camp;
    int j=0;
    for(camp =(SDBCampo *) rec->lista()->first(); camp; camp= (SDBCampo *) rec->lista()->next()) {
        setItem(rowCount()-1,j,camp);
        j++;
    }// end for
}



void SubForm2::pintaCabeceras() {
    _depura("SubForm2::pintaCabeceras",0);
    QStringList headers;
    SHeader * linea;
    int i=0;
    for ( int i = 0; i < m_lcabecera.size(); ++i) {
	linea = m_lcabecera.at(i);
        headers << linea->nompresentacion();
        if (linea->options() & SHeader::DBNoView)
            hideColumn(i);
        else
            showColumn(i);
    }// end for
    setHorizontalHeaderLabels (headers);
}

void SubForm2::situarse(unsigned int row, unsigned int col) {
    unsigned int nrow=row;
    unsigned int ncol = col;
    SHeader *linea= m_lcabecera.at(ncol);
    bool invalido=TRUE;
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
    setCurrentCell(nrow, ncol);
}


/** Cuando tenemos un registro que no se tiene que cargar (pq es nuevo o algo
 ** así) de la base de datos, con la funcion pintar lo dejamos en un estado que
 ** se podria considerar presentable para poder operar con el subformulario.
 **/
void SubForm2::pintar() {
    _depura("SubForm2::pintar",0);
    setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero) {
        cargaconfig();
        m_primero = FALSE;
    }// end if
    nuevoRegistro();
    _depura("END SubForm2::pintar",0);
}


int SubForm2::cargar(cursor2 *cur) {
    _depura("SubForm2::cargar",0);
    while (rowCount() ) {
        m_lista.removeFirst();
        removeRow(0);
    }// end while
    while (!cur->eof()) {
        SDBRecord *rec = newSDBRecord();
        rec->DBload(cur);
        m_lista.append(rec);
        cur->siguienteregistro();
    }// end while
    setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    if (m_primero) {
        cargaconfig();
        m_primero = FALSE;
    }// end if
    setRowCount(m_lista.count());
    SDBRecord *reg;
    for ( int i = 0; i < m_lista.size(); ++i) {
	reg = m_lista.at(i);
        _depura("pintamos un SDBRecord",0);
        int j=0;
        SDBCampo *camp;
        for(camp =(SDBCampo *) reg->lista()->first(); camp; camp= (SDBCampo *) reg->lista()->next()) {
            _depura("ponemos un item"+camp->valorcampo(),0);
            setItem(i,j,camp);
            j++;
        }// end for
    }// end for
    nuevoRegistro();

    /// Ordenamos la tabla.
    ordenar();

    return 0;
}


/// Devuelve la linea que se esta tratando actualmente
SDBRecord *SubForm2::lineaact() {
    _depura("SubForm2::lineaact()\n",0);
    return lineaat(currentRow());
}


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
SDBRecord *SubForm2::lineaat(int row) {
    _depura("SubForm2::lineaat()\n", 0);
    return(((SDBCampo*) item(row,0))->pare());
}


bool SubForm2::eventFilter( QObject *obj, QEvent *ev ) {
    _depura("SubForm2::INIT_eventFilter()\n",0);
    if ( ev->type() == QEvent::KeyRelease ) {
        QKeyEvent *k = (QKeyEvent *)ev;
        int col=currentColumn();
        int row=currentRow();
        switch (k->key()) {
        case Qt::Key_Slash:
            pressedSlash(row, col);
            return TRUE;
            break;
        case Qt::Key_Plus:
            pressedPlus(row, col);
            return TRUE;
            break;
        case Qt::Key_Asterisk:
            pressedAsterisk(row, col);
            return TRUE;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            editFinished(row, col);
            emit editFinish( row, col);
            if (row == rowCount()-1)
                nuevoRegistro();
            situarse(row,col);
            return TRUE;
            break;
        }// end switch
    }// end if
    _depura("SubForm2::END_eventFilter()\n",0);
    return QTableWidget::eventFilter( obj, ev );
} //end eventFilter



int SubForm2::addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
    _depura("SubForm2::addSHeader ("+nom+")",0);
    SHeader *camp = new SHeader( nom, typ, res, opt, nomp);
    camp->set
    ("");
    m_lcabecera.append(camp);
    return 0;
};




void SubForm2::setColumnValue(QString campo, QString valor) {
    SDBRecord *rec;
    for (int i = 0; i < m_lista.size(); ++i) {
	rec = m_lista.at(i);
        rec->setDBvalue(campo, valor);
   }// end for
}



QString SubForm2::DBvalue(QString campo, int row) {
    _depura("SubForm2::DBvalue",0);
    SDBRecord *rec;
    if (row == -1)
        rec = lineaact();
    else
        rec=lineaat(row);
    return rec->DBvalue(campo);
}

int SubForm2::guardar() {
    _depura("SubForm2::guardar",0);
    SDBRecord *rec;
    int i=0;
    int error=0;
    for (int j=0; j < rowCount()-1; ++j) {
	rec = lineaat(j);
        rec->refresh();
        error = rec->guardar();
        if (error)
            return -1;
    }// end for

    if(!m_insercion) {
        rec = lineaat(rowCount()-1);
        error = rec->guardar();
    }// end if
    _depura("END SubForm2::guardar",0);
    return error;
}

int SubForm2::borrar() {
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




int SubForm2::borrar(int row) {
    SDBRecord *rec;
    rec = m_lista.at(row);
    int error = rec->borrar();
    if (error)
        return error;
    m_lista.takeAt(row);
    removeRow(row);
    emit editFinish( row, 0);
    return 0;
}


void SubForm2::guardaconfig() {
    _depura("SubForm2::guardaconfig",0);
    QString aux = "";
    QFile file( confpr->valor(CONF_DIR_USER)+m_tablename+"tablecfn.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << m_colorden << "\n";
        stream << m_tipoorden << "\n";

        for (int i = 0; i < columnCount(); i++) {
            showColumn(i);
            stream << columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}

void SubForm2::cargaconfig() {
    _depura("SubForm2::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+m_tablename+"tablecfn.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        QString linea = stream.readLine();
        m_colorden = linea.toInt();

        linea = stream.readLine();
        m_tipoorden = linea.toInt();

        for (int i = 0; i < columnCount(); i++) {
            linea = stream.readLine();
            if (linea.toInt() > 0)
                setColumnWidth(i, linea.toInt());
            else
                setColumnWidth(i, 2.5);
        }// end for
        file.close();
    } else {
	resizeColumnsToContents();
    }// end if
    _depura("END SubForm2::cargaconfig",0);
}

void SubForm2::pressedSlash(int , int ) {
    _depura ("pulsadoSlash aun no implementado",2);
}


void SubForm2::pressedAsterisk(int , int ) {
    _depura ("pressedAsterisk aun no implementado",1);
}


void SubForm2::pressedPlus(int , int ) {
    _depura ("pulsadoPlus aun no implementado",1);
}


void SubForm2::editFinished(int row, int col) {
    _depura ("editFinished aun no implementado",1);
    emit(editFinish(row, col));
}


void SubForm2::sortItems(int col, Qt::SortOrder orden) {
    _depura("ordenacion desabilitada",2);
}


void SubForm2::sortByColumn(int col) {
    _depura("SubForm2::sortByColumn "+QString::number(col),0);
    if (m_tipoorden == 0) m_tipoorden=1;
    else m_tipoorden = 0;
    m_colorden= col;
    ordenar();
    _depura("END SubForm2::sortByColumn",0);
}


void SubForm2::ordenar() {
    _depura("SubForm2::ordenar ",0);
    if (m_insercion)
        removeRow(rowCount()-1);
    QTableWidget::sortItems(m_colorden, (Qt::SortOrder) m_tipoorden);
    nuevoRegistro();
    _depura("END SubForm2::ordenar",0);
}

