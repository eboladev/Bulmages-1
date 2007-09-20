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
#include <QLocale>
#include <QRegExp>

#include "subform.h"


///
/**
\param con
**/
SDBRecord::SDBRecord(postgresiface2 *con) : DBRecord(con) {
  _depura("SDBRecord::SDBRecord", 0);
  static int creaciones = 0;
  creaciones++;
  _depura("SDBrecord::creados: ",0,  QString::number(creaciones));
  _depura("END SDBRecord::SDBRecord", 0);
}


///
/**
**/
SDBRecord::~SDBRecord() {
  static int destrucciones = 0;
  _depura("SDBRecord::~SDBRecord", 0);
  destrucciones++;
  _depura("SDBrecord::destruidos: ",0 ,  QString::number(destrucciones));
  _depura("SDBRecord::~SDBRecord", 0);
}


///
/**
\param id
\return
**/
int SDBRecord::DBsave(QString &id) {
  _depura("SDBRecord::DBsave", 0);
  refresh();
  _depura("END SDBRecord::DBsave", 0);
  return DBRecord::DBsave(id);
}


///
/**
**/
void SDBRecord::refresh() {
  _depura("SDBRecord::refresh", 0);
  SDBCampo *camp;
  for (int i = 0; i < m_lista.size(); ++i) {
    camp = (SDBCampo *) m_lista.at(i);
    camp->refresh();
  } // end for
  _depura("END SDBRecord::refresh", 0);
}


///
/**
\param nom
\param typ
\param res
\param nomp
\return
**/
int SDBRecord::addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp) {
  _depura("SDBRecord::addDBCampo", 0);
  SDBCampo *camp = new SDBCampo(this, m_conexionbase, nom, typ, res, nomp);
  camp->set("");
  m_lista.append(camp);
  _depura("END SDBRecord::addDBCampo", 0);
  return 0;
}


///
/**
\param par
\param com
\param nom
\param typ
\param res
\param nomp
**/
SDBCampo::SDBCampo(SDBRecord *par, postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp)
  :QTableWidgetItem2(), DBCampo(com, nom, typ, res, nomp) {
  _depura("SDBCampo::SDBCampo", 0);
  m_pare = par;
  _depura("SDBCampo::SDBCampo", 0);
}


///
/**
**/
SDBCampo::~SDBCampo() {
  _depura("SDBCampo::~SDBCampo()", 0);
  _depura("END SDBCampo::~SDBCampo()", 0);
}


///
/**
**/
void SDBCampo::refresh() {
  _depura("SDBCampo::refresh", 0);
  if (this->tipo() == DBCampo::DBboolean)
    DBCampo::set(checkState() == Qt::Checked ? "TRUE" : "FALSE");
  else
    DBCampo::set(text());
  // end if
  _depura("END SDBCampo::refresh", 0);
}


///
/**
\param val
\return
**/
int SDBCampo::set(QString val) {
  _depura("SDBCampo::set", 0, nomcampo() + " = " + val);
  QRegExp importe("^\\d*\\.\\d{2}$"); /// Para emparejar los valores numericos con decimales
  if (tipo() == DBCampo::DBboolean) {
    if (restrictcampo() == SHeader::DBNoWrite) {
      setFlags(this->flags() & (~Qt::ItemIsUserCheckable));
    } // end if
    if (val == "TRUE" || val == "t") {
      setCheckState(Qt::Checked);
    } else {
      setCheckState(Qt::Unchecked);
    } // end if
  } else if (tipo() == DBCampo::DBnumeric && importe.exactMatch(val)) {
    setText(val);
  } else if (tipo() == DBCampo::DBdate) {
    setText(val.left(10));
  } else {
    setText(val);
  } // end if
  DBCampo::set(val);
  _depura("END SDBCampo::set", 0, val);
  return 0;
}


///
/**
\param other
\return
**/
bool SDBCampo::operator< (const QTableWidgetItem &other) {
  _depura("SDBCampo::operator <", 0, text());
  SDBCampo *ot = (SDBCampo *) &other;
  dbtype tip = ot->tipo();
  if (tip == this->tipo()) {
    QString val = ot->valorcampo();

    if (this->tipo() == DBCampo::DBnumeric || this->tipo() == DBCampo::DBint) {
      _depura("SDBCampo::operator < es del tipo numerico:", 0, this->nomcampo() + QString::number(this->tipo()));
      double db1 = this->valorcampo().toDouble();
      double db2 = val.toDouble();
      return (db1 < db2);
    } // end if

    if (this->tipo() == DBCampo::DBdate) {
      _depura("SDBCampo::operator < es del tipo fecha:", 0, this->nomcampo() + QString::number(this->tipo()));
      QDate fech = normalizafecha(this->valorcampo());
      QString db1 = fech.toString(Qt::ISODate);
      QDate fech1 = normalizafecha(val);
      QString db2 = fech1.toString(Qt::ISODate);
      return (db1 < db2);
    } // end if

    if (this->tipo() == DBCampo::DBvarchar) {
      _depura("SDBCampo::operator < es del tipo varchar:", 0, this->nomcampo() + QString::number(this->tipo()));
      return (this->valorcampo() < val);
    } // end if
    _depura("tipo desconocido", 0);
  }
  _depura("END SDBCampo::operator <", 0, text());
  return FALSE;
}


///
/**
\return
**/
SDBRecord *SDBCampo::pare() {
  _depura("SDBCampo::pare", 0);
  _depura("END SDBCampo::pare", 0);
  return m_pare;
}


///
/**
\param nom
\param typ
\param res
\param opt
\param nomp
**/
SHeader::SHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
  _depura("SHeader::SHeader", 0);
  m_nomcampo = nom;
  m_tipo = typ;
  m_restricciones = res;
  m_options = opt;
  m_nompresentacion = nomp;
  m_valorcampo = "";
  _depura("END SHeader::SHeader", 0);
}


///
/**
**/
SHeader::~SHeader() {
  _depura("SHeader::~SHeader", 0);
  _depura("END SHeader::~SHeader", 0);
}


///
/**
\param val
\return
**/
int SHeader::set(QString val) {
  _depura("SHeader::set", 0); 
  m_valorcampo = val;
  _depura("END SHeader::set", 0); 
  return 0;
}


///
/**
\return
**/
unsigned int SHeader::options() {
  _depura("SHeader::options", 0);
  _depura("END SHeader::options", 0);
  return m_options;
}


///
/**
\return
**/
unsigned int SHeader::restricciones() {
  _depura("SHeader::restricciones", 0);
  _depura("END SHeader::restricciones", 0);
  return m_restricciones;
}


///
/**
\return
**/
DBCampo::dbtype SHeader::tipo() {
  _depura("DBCampo::dbtype SHeader::tipo", 0);
  _depura("END DBCampo::dbtype SHeader::tipo", 0);
  return m_tipo;

}


///
/**
\return
**/
QString SHeader::nompresentacion() {
  _depura("SHeader::nompresentacion", 0);
  _depura("EDN SHeader::nompresentacion", 0);
  return m_nompresentacion;
}


///
/**
\return
**/
int SHeader::restrictcampo() {
  _depura("SHeader::restrictcampo", 0);
  _depura("END SHeader::restrictcampo", 0);
  return m_restricciones;
}


///
/**
\return
**/
QString SHeader::nomcampo() {
  _depura("SHeader::nomcampo", 0); 
  _depura("END SHeader::nomcampo", 0); 
  return m_nomcampo;
}

