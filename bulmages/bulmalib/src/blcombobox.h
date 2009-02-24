/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef BLCOMBOBOX_H
#define BLCOMBOBOX_H

#include <QComboBox>

#include "blwidget.h"
#include "blform.h"


class BlComboBox : public QComboBox, public BlMainCompanyPointer
{
    Q_OBJECT

public:
	/// QHash de los valores a comprobar
	QMap <QString, QString> m_valores;

private:
    /// Indica cual es el identificador para la tabla.
    QString m_id;
    QString m_query;
    /// Almacena el nombreprofesor del profesor seleccionado.
    /// Impide que se produzca un dead-lock entre pintar y on_mui_text_changed.
    bool m_semaforo;


protected:
    QString m_tabla;
    /// Cursor que almacena en memoria la lista de provincias.
    BlDbRecordSet *m_cursorcombo;
    /// Indica si adminte nulos o no
    bool m_null;

public:
    BlComboBox ( QWidget *parent );
    ~BlComboBox();
    QString fieldValue();
    void setFieldValue(QString);
    virtual QString id();
    virtual void setId(QString);
    void setQuery(QString);
    virtual void setTableName(QString tableName);
    virtual void setCampoId(QString cid);
    void allowNull(bool v);

public slots:
    virtual void m_activated(int);
    virtual void onTablaCambiada(const QString &);

signals:
    void valueChanged(QString);

};

#endif

