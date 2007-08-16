/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef CANUALESSPRINTVIEW_H
#define CANUALESPRINTVIEW_H

#include <QWidget>

#include "ui_canualesprintbase.h"

#include <errno.h>
#include <sys/types.h>

#include <QDomNode>
#include <QDomDocument>

#ifndef WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif


class Empresa;


class CAnualesPrintView : public QDialog, public Ui_CAnualesPrintBase {
    Q_OBJECT

private:
    char *fichero;
    QString idbalance;
    Empresa *empresaactual;
    QDomDocument m_doc;

public:
    CAnualesPrintView(Empresa *emp,QWidget *parent = 0);
    ~CAnualesPrintView();
    void setidbalance(QString); /// Establece cual ser&aacute; el balance a imprimir.
    bool procesaFormula(const QDomNode &);
    bool procesaOperador(const QDomNode &);
    bool valorItem(const QDomNode &, QString &, QString &);
    void agregaValores(const QDomNode &, const QString &, const QString &);
    void imprimir(QString, QString, QString, QString);

public slots:
    virtual void on_mui_aceptar_clicked();
};

#endif

