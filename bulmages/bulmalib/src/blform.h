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

#ifndef BLFORM_H
#define BLFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QtScript>
#include <QScriptEngine>
#include <QDomElement>

#include "bldialogchanges.h"
#include "bldoublespinbox.h"
#include "blfunctions.h"
#include "blmaincompany.h"
#include "blwidget.h"
#include "bldb.h"


class BL_EXPORT BlForm : public BlWidget, public BlDbRecord, public BlDialogChanges
{
    Q_OBJECT

private:
    /// Indica si es modo consulta o modo edicion. (altera el comportamiento del
    /// doble click sobre la lista)
    edmode m_modo;
    QString m_title;
    QScriptEngine m_myEngine;
    
protected:
    void setSelectMode();
    void setEditMode();
    bool editMode();
    bool selectMode();

public:
    BlForm ( QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = BL_EDIT_MODE );
    BlForm ( BlMainCompany *emp = NULL, QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = BL_EDIT_MODE );
    void substrVars(QString &buff, int tipoEscape = 0);
    virtual ~BlForm();
    virtual int save();
    virtual int beforeSave();
    virtual int afterSave();
    virtual int beforeDelete();
    virtual int remove();
    virtual int removeWindow();
    virtual void insertWindow ( QString , QObject *, bool compdup = TRUE, QString titulo = "" );
    void setDbTableName ( QString );
    void setTitleName ( QString nom );
    virtual void pintar();
    virtual void pintarPost();
    virtual int load ( QString id, bool paint = TRUE );
    virtual int cargarPost ( QString id );
    virtual void recogeValores();
    virtual void createMenu ( QMenu * );
    virtual void execMenuAction ( QAction * );
    virtual int generateRML ( const QString & );
    virtual int generateRML ( void );
    virtual int parseTags ( QString &buff, int tipoEscape = 0 );
    virtual QString parseIncludeFile ( const QString &file, int tipoEscape = 0 );
    virtual QString parseQuery ( const QString &query, const QString &datos, int tipoEscape = 0 );
    QString parseRecordset ( BlDbRecordSet *cur, const QString &datos, int tipoEscape = 0 );
    virtual QString parseIfQuery ( const QString &query, const QString &datos );
    virtual QString parseIf ( const QString &query, const QString &datos, const QString &datos1 );
    virtual QString parseExists ( const QString &query, const QString &datos );
    void setVar(const QString &varname, const QString &varvalue);
    void clearVars();
    virtual void loadSpecs();
    virtual void generaCampo ( const QString &objname, const QString &textname, const QString &type, const QDomElement &e1=NULL );
    virtual void activateDocuments();
    virtual void deactivateDocuments();
    void blScript(QObject * obj);
protected:
    virtual void closeEvent ( QCloseEvent * );

public slots:
    virtual void on_mui_aceptar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_cancelar_clicked();
    virtual void on_mui_guardar_clicked();
    virtual void on_customContextMenuRequested ( const QPoint &pos );
    virtual void on_mui_imprimir_clicked();

signals:
    void pintaMenu ( QMenu * );
    void trataMenu ( QAction * );
};

#endif

