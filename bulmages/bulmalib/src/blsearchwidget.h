/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef BLSEARCHWIDGET_H
#define BLSEARCHWIDGET_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>

#include "blfunctions.h"
#include "blwidget.h"
#include "blcombobox.h"
#include "ui_blsearchwidgetbase.h"


/// Permite buscar y seleccionar un profesor.
/** Implementa un Widget que permite buscar y seleccionar un
    profesor de BulmaFact de forma generica. */
class BL_EXPORT BlSearchWidget : public BlWidget, public Ui_BlSearchWidgetBase
{
    Q_OBJECT

public:
    /// QHash de los valores a comprobar
    QMap <QString, QString> m_valores;

private:
    /// Almacena la tabla sobre la que vamos a buscar.
    QString m_tabla;
    QString m_campoid;
    /// Almacena el id de la tabla seleccionada.
    QString mdb_id;
    /// Impide que se produzca un dead-lock entre pintar y on_mui_text_changed.
    bool m_semaforo;
    /// Es la mascara de presentacion, indica en que forma se presentan los
    /// Campos en el cuadro de texto inferior. Si no esta establecido entonces no se
    /// utiliza
    QString m_mask;

public:
    BlSearchWidget ( QWidget *parent = 0 );
    ~BlSearchWidget();
    void pinta();
    virtual QString id();
    virtual QString fieldValue();
    virtual QString fieldValue(QString campo);
    virtual QString tableName();
    virtual void setId ( QString val, bool cargarvalores = true );
    virtual void setFieldValue ( QString val );
    virtual void setFieldValue ( QString campo, QString val );
    virtual void setLabel(QString label);
    virtual void setTableName(QString tableName);
    virtual void setText(const QString &);
    void setMask(const QString &);
    QString mask();
    QString text();
    void setFieldId ( const QString & );
    QString fieldId();
    void selectAll();
    void setFocus();
    void hideLabel();
    void hideDescripcion();
    void hideBotonBorrar();
    void hideBotonBusqueda();


public slots:
    virtual void on_m_buscarWidget_clicked();
    virtual void on_m_clearSearchWidget_clicked();
    virtual void on_m_inputBusqueda_textChanged ( const QString & );
    virtual void on_m_inputBusqueda_editingFinished();

signals:
    void valueChanged ( QString );
};



/// Delegate Generico
class BL_EXPORT BlDbCompleterComboBox : public BlComboBox
{
    Q_OBJECT

public:
    /// QHash de los valores a comprobar
    QMap <QString, QString> m_valores;
    /// Almacena la tabla sobre la que vamos a buscar.
    QString m_tabla;
    /// Almacena el id de la tabla seleccionada.
    QString mdb_id;
    /// Este cursor almacena el listado de elementos.
    BlDbRecordSet *m_cursorcombo;
    /// Texto entrado por el usuario (para uso de los plugins)
    QString m_entrada; 
public:
    BlDbCompleterComboBox ( QWidget *parent = 0 );
    ~BlDbCompleterComboBox();
    QString entrada();
    QString unicaEleccion(void) ;
    QString eligeUnico(void) ;
    
public slots:
    virtual void on_customContextMenuRequested ( const QPoint & );
    virtual void s_editTextChanged ( const QString & );
    virtual void focusOutEvent ( QFocusEvent * event );

    
signals:
    void pintaMenu ( QMenu * );
    void trataMenu ( QAction * );
};




/// Delegate Generico
class BL_EXPORT BlDbEditComboBox : public BlComboBox
{
    Q_OBJECT

public:
    /// QHash de los valores a comprobar
    QMap <QString, QString> m_valores;
    /// Almacena la tabla sobre la que vamos a buscar.
    QString m_tabla;
    
public:
    /// Almacena el id de la tabla seleccionada.
    QString mdb_id;
    /// Este cursor almacena el listado de elementos.
    BlDbRecordSet *m_cursorcombo;
    /// Texto entrado por el usuario (para uso de los plugins)
    QString m_entrada; 
public:
    BlDbEditComboBox ( QWidget *parent = 0 );
    ~BlDbEditComboBox();
    QString entrada();
    QString unicaEleccion(void) ;
    QString eligeUnico(void) ;
    virtual void setQuery ( const QString & );
public slots:
    virtual void focusOutEvent ( QFocusEvent * event );
    virtual void popMenu ( const QPoint &pos );
    
signals:
    void pintaMenu ( QMenu * );
    void trataMenu ( QAction * );
};



#endif

