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

SubForm2::SubForm2(QWidget *parent) : QTableWidget2(parent) {
    setSelectionMode( QAbstractItemView::SingleSelection );
    setSelectionBehavior( QAbstractItemView::SelectRows);
    setAlternatingRowColors(TRUE);
    setSortingEnabled(TRUE);
    installEventFilter(this);
    m_insercion = FALSE;
}


SDBRecord *SubForm2::newSDBRecord() {
    _depura("SubForm2::newSDBRecord",0);
    SDBRecord *rec = new SDBRecord(m_companyact);
    rec->setDBTableName( m_tablename);
    rec->setDBCampoId( m_campoid);
    SHeader * linea;
    for (linea=m_lcabecera.first(); linea; linea=m_lcabecera.next()) {
        _depura("agregamos una cabecera",0);
        rec->addDBCampo(linea->m_nomcampo, linea->m_type, linea->m_restrict, linea->m_nompresentacion );
    }// end for


    int j=0;
    SDBCampo*camp;
    for(camp =(SDBCampo *) rec->lista()->first(); camp; camp= (SDBCampo *) rec->lista()->next()) {
        SHeader *head = m_lcabecera.at(j);
        if (head->m_options & SHeader::DBNoWrite) {
            _depura("cambiamos propieades de flag",0);
            camp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            //camp->setBackgroundColor(QColor("#FF0000"));
        }
        j++;
    }// end for



    return rec;
}


void SubForm2::pintaCabeceras() {
	_depura("SubForm2::pintaCabeceras",0);
    QStringList headers;
    SHeader * linea;

    int i=0;
    for (linea=m_lcabecera.first(); linea; linea=m_lcabecera.next()) {
        headers << linea->m_nomcampo;
        if (linea->m_options & SHeader::DBNoView)
            hideColumn(i);
        else
            showColumn(i);
        i++;
    }// end for

    setHorizontalHeaderLabels (headers);
}

void SubForm2::situarse(int row, int col) {
    int nrow=row;
    int ncol = col;
    SHeader *linea= m_lcabecera.at(ncol);
    bool invalido=TRUE;
    while(invalido) {
        ncol++;
        if (ncol == m_lcabecera.count()) {
            ncol=0;
            nrow++;
        }
        linea = m_lcabecera.at(ncol);
        invalido = FALSE;
        if(linea->m_options & SHeader::DBNoView)
            invalido = TRUE;
        if (linea->m_options & SHeader::DBNoWrite)
            invalido = TRUE;
    }// end while
    setCurrentCell(nrow, ncol);
}



int SubForm2::cargar(cursor2 *cur) {
    _depura("SubForm2::cargar",0);
    while (rowCount() ) {
        m_lista.removeFirst();
        removeRow(0);
    }
    while (!cur->eof()) {
        SDBRecord *rec = newSDBRecord();
        rec->DBload(cur);
        m_lista.append(rec);
        cur->siguienteregistro();
    }// end while
    setColumnCount(m_lcabecera.count());
    pintaCabeceras();
    cargaconfig();
    setRowCount(m_lista.count());
    SDBRecord *reg;
    int i=0;
    for(reg = m_lista.first(); reg; reg=m_lista.next()) {
        _depura("pintamos un SDBRecord",0);
        int j=0;
        SDBCampo *camp;
        for(camp =(SDBCampo *) reg->lista()->first(); camp; camp= (SDBCampo *) reg->lista()->next()) {
            _depura("ponemos un item",0);
            setItem(i,j,camp);
            j++;
        }
        i++;
    }// end for
    nuevoRegistro();
    return 0;
}


/// Devuelve la linea que se esta tratando actualmente
SDBRecord *SubForm2::lineaact() {
    _depura("SubForm2::lineaact()\n",0);
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
SDBRecord *SubForm2::lineaat(int row) {
    _depura("SubForm2::lineaat()\n", 0);
    return(m_lista.at(row));
    _depura("SubForm2::lineaat()\n", 0);
}// end lineaat


bool SubForm2::eventFilter( QObject *obj, QEvent *ev ) {
    _depura("SubForm2::INIT_eventFilter()\n",0);
    SDBRecord *linea;
    if ( ev->type() == QEvent::KeyRelease ) {
        QKeyEvent *k = (QKeyEvent *)ev;
        int col=currentColumn();
        int row=currentRow();
        switch (k->key()) {
        case Qt::Key_Plus:
            break;
        case Qt::Key_Asterisk:
            pressedAsterisk(row, col);
            return TRUE;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            editFinished(row, col);
            if (row == rowCount()-1)
                nuevoRegistro();
            situarse(row,col);
            return TRUE;
            break;
        }// end switch
    }// end if
    _depura("SubForm2::END_eventFilter()\n",0);
    return QTableWidget2::eventFilter( obj, ev );
} //end eventFilter



int SubForm2::addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
	_depura("SubForm2::addSHeader ("+nom+")",0);
    SHeader *camp = new SHeader( nom, typ, res, opt, nomp);
    camp->set("");
    m_lcabecera.append(camp);
    return 0;
};


void SubForm2::nuevoRegistro() {
    _depura("nuevoRegistro",0);
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

void SubForm2::setColumnValue(QString campo, QString valor) {
    SDBRecord *rec;
    for (rec = m_lista.first(); rec; rec = m_lista.next())
        rec->setDBvalue(campo, valor);
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

void SubForm2::guardar() {
    SDBRecord *rec;
    int i=0;
    for(rec=m_lista.at(i++); i<m_lista.count() ;rec=m_lista.at(i++))
        rec->guardar();

    if(!m_insercion) {
        rec = m_lista.at(m_lista.count()-1);
        rec->guardar();
    }
}

int SubForm2::borrar() {
    SDBRecord *rec;
    int i=0;
	int error = 0;
    for(rec=m_lista.at(i++); i<m_lista.count() ;rec=m_lista.at(i++)) {
        error = rec->borrar();
	if (error) return -1;
	}

    if(!m_insercion) {
        rec = m_lista.at(m_lista.count()-1);
        error = rec->borrar();
    }
	return error;
}




int SubForm2::borrar(int row) {
    SDBRecord *rec;
    rec = m_lista.at(row);
    int error = rec->borrar();
	if (error) return error;
    m_lista.remove(row);
    removeRow(row);
	return 0;
}


void SubForm2::guardaconfig() {
    _depura("SubForm2::guardaconfig",0);
    QString aux = "";

    QFile file( confpr->valor(CONF_DIR_USER)+m_tablename+"tablecfn.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
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
        for (int i = 0; i < columnCount(); i++) {
            QString linea = stream.readLine();
            setColumnWidth(i, linea.toInt());
        }// end for
        file.close();
    }// end if
}

