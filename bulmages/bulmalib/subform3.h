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

#ifndef SUBFORM3_H
#define SUBFORM3_H

#include <Qt>
#include <QWidget>
#include <QHeaderView>

#include "ui_subform3base.h"
#include "postgresiface2.h"
#include "funcaux.h"
#include "qtable2.h"
#include "subform.h"
#include "fixed.h"


/// SubForm3, constructor de la clase base para subformularios.
/** Proporciona un widget que combina la clase QTable con los
registros de la base de datos proporcionando un potente manejador
tabular de datos que puede generar desde listados hasta subformularios
*/
class SubForm3: public QWidget, public Ui_SubForm3Base {
    Q_OBJECT

protected:
    /// Nombre de la tabla en la BD.
    QString m_tablename;
    /// Lista de encabezados del listado. Coordenadas X de la tabla.
    QList<SHeader *> m_lcabecera;
    /// Filas y contenido. Coordenadas Y de la tabla.
    QList<SDBRecord *> m_lista;
    /// Lista de subformularios marcados para ser borrados.
    QList<SDBRecord *> m_listaborrar;
    /// Clase padre y acceso a base de datos.
    postgresiface2 *m_companyact;
    /// Fichero de configuracion del subformulario.
    QString m_fileconfig;
    /// Campo Identificador de la tabla en la BD.
    QString m_campoid;
    /// Indica si se pueden insertar nuevas filas o no.
    bool m_insercion;
    /// Indica si ya se ha cargado la configuracion o no.
    bool m_primero;
    /// Indica si se pueden borrar registros.
    bool m_delete;
    /// Indica si el subformulario es ordenable o no.
    bool m_sorting;
    /// Indica si se usa ordenacion por la base de datos. (Su uso implica que exista en la tabla el campo orden)
    bool m_orden;

protected:
    /// Guarda la configuracion de presentacion del subformulario
    /// en el directorio .bulmages del home del usuario.
    void guardaconfig();
    /// Carga la configuracion que tenia el formulario la ultima
    /// vez que fue utilizado por el usuario.
    void cargaconfig();
    /// Prepara las columnas de la tabla del formulario y pone en
    /// ella los encabezados.
    virtual void pintaCabeceras();

public:
    /// Devuelve la lista de registros de la base de datos que
    /// se presentan en el subformulario.
    QList<SDBRecord *> *lista();
    /// Devuelve la lista de cabeceras que forman el subformulario.
    QList<SHeader *> *cabecera();

public:
    /// Establece el modo de funcionamiento del scroll horizontal.
    void setHorizontalScrollMode(QAbstractItemView::ScrollMode mode);
    /// Establece que los elementos del subformulario son ordenables
    void setSortingEnabled(bool);
    /// Indica si los elementos del subformulario son ordenables.
    bool sortingEnabled();
    /// Establece en el subformulario un modo de funcionamiento mediante
    /// el cual el orden de los elementos se almacena en la base de datos
    /// mediante un campo orden"tabla" de tipo numerico.
    void setOrdenEnabled(bool);
    /// Indica si esta establecido el modo de ordenacion en la base
    /// de datos.
    bool ordenEnabled();
    /// Indica si una fila determinada del subformulario esta lista
    /// para intentar ser guardada en la base de datos. O lo que es lo mismo
    /// si todos los campos necesarios para guardar el registro estan
    /// rellenados
    bool campoCompleto(int row);
    /// Establece el modo de borrado de registros que permite al subformulario
    /// borrar filas con su correspondiente borrado en la base de datos.
    void setDelete(bool f);
    SubForm3(QWidget *parent);
    virtual ~SubForm3();
    /// Devuelve el elemento del subformulario ubicado en la posicion
    /// indicada
    SDBCampo *item(int row, int col);
    /// Devuelve el numero total de columnas del subformulario.
    int columnCount();
    /// Muestra la columna indicada en el subformulario.
    void showColumn(int i);
    /// Devuelve el ancho de la columna indicada.
    int columnWidth(int i);
    /// Establece el ancho de la columna indicada.
    void setColumnWidth(int i, int j);
    /// Oculta la columna indicada.
    void hideColumn(int i);
    /// Devuelve la fila que en la actualidad poee el foco en el subformulario.
    int currentRow();
    /// Devuelve el numero de filas en el subformulario.
    int rowCount();
    /// Devuelve la columna que en la actualidad posee el foco del subformulario.
    int currentColumn();
    /// Establece la forma de redimensionado de las columnas del subformulario.
    void setResizeMode(QHeaderView::ResizeMode modo);
    /// Redimensiona las columnas del subformulario para ajustarlas
    /// a los contenidos.
    void resizeColumnsToContents();
    /// Redimensiona las filas del subformulario para ajustarlas
    /// a los contenidos.
    void resizeRowsToContents();
    /// Redimensiona las columnas del subformulario para ajustarlas a los contenidos
    void resizeColumnToContents(int i);
    /// Ajusta la fila indicada a sus contenidos.
    void resizeRowToContents(int i);
    /// Oculta el configurador del subformulario
    void hideConfig();
    /// Muestra el configurador del subformulario
    void showConfig();
    /// Hace la impresion del subformulario en formato de tabla RML
    QString imprimir();
    void sortItems(int col, Qt::SortOrder orden);
    void setinsercion(bool b);
    void setcompany(postgresiface2 *c);
    postgresiface2 *companyact();
    void setDBTableName(QString nom);
    QString tableName();
    void setFileConfig(QString nom);
    void setDBCampoId(QString nom);
    void clear();
    void setRowCount(int i);
    void setColumnCount(int i);
    void situarse(unsigned int, unsigned int);
    void situarse1(unsigned int, unsigned int);
    int addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp);
    SDBRecord *newSDBRecord();
    void setColumnValue(QString, QString);
    QString DBvalue(QString campo, int row = -1);
    SDBRecord *lineaact();
    SDBRecord *lineaat(int row);
    virtual void imprimirPDF(const QString &);
    virtual void nuevoRegistro();
    virtual int borrar();
    virtual int borrar(int);
    virtual int guardar();
    virtual void cargar(cursor2 *cur);
    virtual void cargar(QString query);
    virtual int inicializar();
    virtual void pintar();
    virtual void creaMenu(QMenu *);
    virtual void procesaMenu(QAction *);
    virtual Fixed sumarCampo(QString);
    bool existsHeader(const QString &);


public slots:
    virtual void contextMenuEvent(QContextMenuEvent *);
    virtual void on_mui_confcol_clicked();
    virtual void on_mui_confquery_clicked();
    virtual void on_mui_appag_clicked();
    virtual void on_mui_list_editFinished(int row, int col, int key);
    virtual void on_mui_list_pressedSlash(int row, int col);
    virtual void on_mui_list_pressedAsterisk(int row, int col);
    virtual void on_mui_list_pressedPlus(int row, int col);
    virtual void on_mui_list_pressedMinus(int row, int col);
    virtual void on_mui_list_ctrlSubir(int row, int col);
    virtual void on_mui_list_ctrlBajar(int row, int col);
    virtual void on_mui_pagsiguiente_clicked();
    virtual void on_mui_paganterior_clicked();
    virtual void on_mui_list_itemDoubleClicked(QTableWidgetItem *item);
    virtual void on_mui_list_cellDoubleClicked(int row, int col);
    virtual void toogleConfig();
    virtual void on_mui_botonCerrar_clicked();
    virtual void on_mui_list_cellChanged(int, int);
    virtual void on_mui_list_itemChanged(QTableWidgetItem *it);
    virtual void on_mui_list_currentCellChanged(int, int, int, int);

signals:
    void toogledConfig(bool);
    void editFinish(int, int);
    void itemDoubleClicked(QTableWidgetItem *);
    void cellDoubleClicked(int row, int col);
    void pintaMenu(QMenu *);
    void trataMenu(QAction *);
};

#endif

