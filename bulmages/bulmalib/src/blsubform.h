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

#ifndef BLSUBFORM_H
#define BLSUBFORM_H

#include <Qt>
#include <QWidget>
#include <QHeaderView>
#include <QPalette>

#include "blfunctions.h"
#include "ui_blsubformbase.h"
#include "blpostgresqlclient.h"
#include "qtable2.h"
#include "bldbsubform.h"
#include "blfixed.h"
#include "blwidget.h"
#include "plugins.h"


/// Describe una cabecera de recordset preparada para trabajar con SubFormularios \ref BlSubForm
/**
Al crear un subformulario \ref BlSubForm, todos los registros tienen exactamente las mismas cabeceras.
Esta clase sirve para describir una cabecera. Es analoga a la clase \ref BlDbField pero en lugar de
describir un unico registro describe a varios.
*/
class BlSubFormHeader
{
public:
    /** Opciones para la columna del SubFormulario
    - DBNone . Opciones por defecto.
    - DBReadOnly . La columna es de solo lectura.
    - DBNoView . Por defecto la columna es invisible aunque puede verse si el usuario lo desea
    - DBNoWrite . El usuario no puede escribir en esta columna.
    - DBBlockView . El usuario no podrá ver esta columna ni aun configurandola
    */
    enum dboptions {DBNone = 0, DBReadOnly = 1, DBNoView = 2, DBNoWrite = 4, DBBlockView = 8};

protected:
    QString m_nomcampo;   ///< El nombre de la cabecera en el recordset
    QString m_nompresentacion;  ///< El nombre para mostrar en los errores
    unsigned int m_restricciones; ///< Las restricciones de la columna en el recordset
    BlDbField::dbtype m_tipo;  ///< El tipo de los datos
    unsigned int m_options;  ///< Las opciones para la columna en el SubFormulario

public:
    BlSubFormHeader ( QString nom, BlDbField::dbtype typ, int res, int opt, QString nomp = "" );
    ~BlSubFormHeader();
    unsigned int options();
    unsigned int restricciones();
    BlDbField::dbtype tipo();
    QString nompresentacion();
    int restrictcampo();
    QString nomcampo();
};


/// BlSubForm, constructor de la clase base para subformularios.
/** Proporciona un widget que combina la clase QTable con los
registros de la base de datos proporcionando un potente manejador
tabular de datos que puede generar desde listados hasta subformularios
*/
class BlSubForm: public BlWidget, public Ui_BlSubFormBase
{
    Q_OBJECT

public:
    enum edmode {
        EditMode = 0, SelectMode = 1
    };

private:
    virtual void cargar ( BlDbRecordSet *cur );

private:
    QString m_query;
    /// Indica si es modo consulta o modo edicion. (altera el comportamiento del
    /// doble click sobre la lista)
    edmode m_modo;

protected:
    /// Nombre de la tabla en la BD.
    QString m_tablename;
    /// Lista de encabezados del listado. Coordenadas X de la tabla.
    QList<BlSubFormHeader *> m_lcabecera;
    /// Filas y contenido. Coordenadas Y de la tabla.
    QList<BlDbSubFormRecord *> m_lista;
    /// Lista de subformularios marcados para ser borrados.
    QList<BlDbSubFormRecord *> m_listaborrar;
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
    /// Indica que no se va aplicar ninguna ordenaci&oacute;n a la consulta de la base de datos.
    bool m_ordenporquery;
    /// Indica si procesar cambios en las celdas o no
    bool m_procesacambios;
    /// Texto de la celda que se coge para saber si se tienen que unir con RowSpan.
    QString m_textoceldaParaRowSpan;
    /// Define que columna se va a coger para unir las filas que sean iguales.
    QString m_columnaParaRowSpan;
    /// Se utiliza para saber en que fila comenzamos a contar para unir con RowSpan las filas
    /// que son iguales.
    int m_filaInicialRowSpan;
    QColor m_colorfondo1;
    QColor m_colorfondo2;
    void ponItemColorFondo ( QTableWidget *twidget, int filainicial, int totalfilas, QColor colorfondo );

public:
    /// Indica cual es el &uacute;ltimo campo (fila) que se ha editado.
    int m_prevRow;
    /// Indica cual es el &uacute;ltimo campo (columna) que se ha editado.
    int m_prevCol;

protected:
    /// Guarda la configuraci&oacute;n de presentaci&oacute;n del subformulario
    /// en el directorio .bulmages situado dentro del directorio 'home' del usuario.
    void guardaconfig();
    /// Carga la configuraci&oacute;n que ten&iacute;a el formulario la &uacute;ltima
    /// vez que fue utilizado por el usuario.
    void cargaconfig();
    /// Carga la configuración de especificaciones.
    /// Los specs son archivos en /etc/bulmages que suplen o Incrementan los campos presentados en un subformulario.
    /// Son utiles para hacer ampliaciones sin programacion.
    void cargaSpecs();
    /// Prepara las columnas de la tabla del formulario y pone en
    /// ella los encabezados.
    virtual void pintaCabeceras();

public:
    /// Devuelve la lista de registros de la base de datos que
    /// se presentan en el subformulario.
    QList<BlDbSubFormRecord *> *lista();
    /// Devuelve la lista de cabeceras que forman el subformulario.
    QList<BlSubFormHeader *> *cabecera();

public:
    BlSubForm ( QWidget *parent );
    virtual ~BlSubForm();
    virtual void setMainCompany ( BlMainCompany * );
    void setProcesarCambios ( bool );
    bool procesaCambios();
    /// Define si el listado presentar&aacute; las filas con colores alternativos.
    void setListadoPijama ( bool pijama );
    bool listadoPijama();
    /// Establece el modo de funcionamiento del scroll horizontal.
    void setHorizontalScrollMode ( QAbstractItemView::ScrollMode mode );
    /// Establece los colores para dibujar el fondo de las filas.
    /// Si la propiedad 'alternatingRowColors' est&aacute; en TRUE entonces de usan
    /// los dos colores, si est&aacute; en FALSE s&oacute;lo se usa 'ColorFondo1'.
    /// Estos valores corresponden a QPalette::Base y QPalette::AlternateBase.
    void setColorFondo1 ( QString color );
    void setColorFondo2 ( QString color );
    /// Establece que los elementos del subformulario son ordenables
    void setSortingEnabled ( bool );
    /// Indica si los elementos del subformulario son ordenables.
    bool sortingEnabled();
    /// Establece en el subformulario un modo de funcionamiento mediante
    /// el cual el orden de los elementos se almacena en la base de datos
    /// mediante un campo orden"tabla" de tipo num&eacute;rico.
    void setOrdenEnabled ( bool );
    void setOrdenPorQuery ( bool );
    /// Indica si esta establecido el modo de ordenaci&oacute;n en la base
    /// de datos.
    bool ordenEnabled();
    /// Indica si una fila determinada del subformulario esta lista
    /// para intentar ser guardada en la base de datos. O lo que es lo mismo
    /// si todos los campos necesarios para guardar el registro est&aacute;n
    /// rellenados
    bool campoCompleto ( int row );
    /// Establece el modo de borrado de registros que permite al subformulario
    /// borrar filas con su correspondiente borrado en la base de datos.
    void setDelete ( bool f );
    /// Devuelve el elemento del subformulario ubicado en la posici&oacute;n
    /// indicada
    BlDbSubFormField *item ( int row, int col );
    /// Pone el foco en una celda determinada.
    void setCurrentItem ( int row, int col );
    /// Devuelve el n&uacute;mero total de columnas del subformulario.
    int columnCount();
    /// Muestra la columna indicada en el subformulario.
    void showColumn ( int i );
    /// Devuelve el ancho de la columna indicada.
    int columnWidth ( int i );
    /// Establece el ancho de la columna indicada.
    void setColumnWidth ( int i, int j );
    /// Oculta la columna indicada.
    void hideColumn ( int i );
    /// Devuelve la fila que en la actualidad posee el foco en el subformulario.
    int currentRow();
    /// Devuelve el n&uacute;mero de filas en el subformulario.
    int rowCount();
    /// Devuelve la columna que en la actualidad posee el foco del subformulario.
    int currentColumn();
    /// Establece la forma de redimensionado de las columnas del subformulario.
    void setResizeMode ( QHeaderView::ResizeMode modo );
    /// Redimensiona las columnas del subformulario para ajustarlas
    /// a los contenidos.
    void resizeColumnsToContents();
    /// Redimensiona las filas del subformulario para ajustarlas
    /// a los contenidos.
    void resizeRowsToContents();
    /// Redimensiona las columnas del subformulario para ajustarlas a los contenidos
    void resizeColumnToContents ( int i );
    /// Ajusta la fila indicada a sus contenidos.
    void resizeRowToContents ( int i );
    /// Oculta el configurador del subformulario
    void hideConfig();
    /// Muestra el configurador del subformulario
    void showConfig();
    /// Hace la impresion del subformulario en formato de tabla RML.
    QString imprimir();
    void sortItems ( int col, Qt::SortOrder orden );
    void setinsercion ( bool b );
    /// Define que campo se utilizar&aacute; para unir las filas que tengan el mismo texto (rowSpan).
    /// Es &uacute;til en contabilidad para unir filas del mismo asiento.
    void setColumnToRowSpan ( QString campo );
    void setDbTableName ( QString nom );
    QString tableName();
    void setFileConfig ( QString nom );
    void setDbFieldId ( QString nom );
    void clear();
    void setRowCount ( int i );
    void setColumnCount ( int i );
    void situarse ( unsigned int, unsigned int );
    void situarse1 ( unsigned int, unsigned int );
    int addSubFormHeader ( QString nom, BlDbField::dbtype typ, int res, int opt, QString nomp );
    BlDbSubFormRecord *newDbSubFormRecord();
    virtual void setColumnValue ( QString, QString );
    QString DBvalue ( const QString &campo, int row = -1 );
    void setDbValue ( const QString &campo, int row, const QString &valor );
    BlDbSubFormRecord *lineaact();
    BlDbSubFormRecord *lineaat ( int row );
    virtual void imprimirPDF ( const QString & );
    virtual void nuevoRegistro();
    virtual int borrar();
    virtual int borrar ( int );
    virtual int guardar();
    virtual void cargar ( QString query );
    virtual int inicializar();
    virtual void pintar();
    virtual void creaMenu ( QMenu * );
    virtual void procesaMenu ( QAction * );
    virtual BlFixed sumarCampo ( QString );
    bool existsHeader ( const QString & );
    virtual int cerrarEditor();
    virtual void confquery();
    QString columnDBfieldName ( int columna );
    /// Funciones para manipular los modos.
    void setModoConsulta();
    void setModoEdicion();
    bool modoEdicion();
    bool modoConsulta();

public slots:
    virtual void on_mui_list_cellRePosition ( int, int );
    void on_mui_list_cellChanged ( int, int );
    virtual void contextMenuEvent ( QContextMenuEvent * );
    virtual void on_mui_confcol_clicked();
    virtual void on_mui_confquery_clicked();
    void on_mui_list_pressedSlash ( int row, int col );
    void on_mui_list_pressedAsterisk ( int row, int col );
    void on_mui_list_pressedPlus ( int row, int col );
    void on_mui_list_pressedMinus ( int row, int col );
    virtual void on_mui_list_ctrlSubir ( int row, int col );
    virtual void on_mui_list_ctrlBajar ( int row, int col );
    virtual void on_mui_pagsiguiente_clicked();
    virtual void on_mui_paganterior_clicked();
    virtual void on_mui_list_itemDoubleClicked ( QTableWidgetItem *item );
    virtual void on_mui_list_itemClicked ( QTableWidgetItem *item );
    virtual void on_mui_list_cellDoubleClicked ( int row, int col );
    virtual void toogleConfig();
    virtual void on_mui_botonCerrar_clicked();
    virtual void on_mui_list_itemChanged ( QTableWidgetItem *it );
    virtual void editFinished ( int, int, BlDbSubFormRecord *, BlDbSubFormField * );
    virtual void pressedAsterisk ( int, int, BlDbSubFormRecord *, BlDbSubFormField * );
    virtual void pressedPlus ( int, int, BlDbSubFormRecord *, BlDbSubFormField * );
    virtual void pressedMinus ( int, int, BlDbSubFormRecord *, BlDbSubFormField * );
    virtual void pressedSlash ( int, int, BlDbSubFormRecord *, BlDbSubFormField * );

signals:
    void toogledConfig ( bool );
    void editFinish ( int, int );
    void pushAsterisk ( int, int );
    void pushPlus ( int, int );
    void pushMinus ( int, int );
    void pushSlash ( int, int );
    void itemDoubleClicked ( QTableWidgetItem * );
    void itemClicked ( QTableWidgetItem * );
    void cellDoubleClicked ( int row, int col );
    void pintaMenu ( QMenu * );
    void trataMenu ( QAction * );
};

#endif

