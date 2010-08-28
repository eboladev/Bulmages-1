/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2010 by Aron Galdon                                     *
 *   auryn@wanadoo.es                                                      *
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

#include <QKeyEvent>
#include <QEvent>
#include <QFile>
#include <QHeaderView>
#include <QTextStream>
#include <QMenu>
#include <QShortcut>
#include <QLocale>
#include <QRegExp>
#include <QDomDocument>
#include <QDomNode>
#include <QSpinBox>

#include <QDebug>

#include "blsubform.h"
#include "blprogressbar.h"
#include "bldoublespinbox.h"
#include "bltexteditdelegate.h"

// Necesito exportar algunos datos.
QModelIndex BL_EXPORT g_index;
QWidget BL_EXPORT *g_editor;
QAbstractItemModel BL_EXPORT *g_model;
QString BL_EXPORT g_nomcampo;


/// Construye una columna de la descripcion del recordset
/**
\param nom Nombre de la columna
\param typ Tipo de los datos que alberga
\param res Restricciones de la columna
\param opt Opciones de presentacion
\param nomp Nombre a presentar en caso necesario para referirse a la columna
**/
BlSubFormHeader::BlSubFormHeader ( QString nom, BlDbField::DbType typ, int res, int opt, QString nomp )
{
    blDebug ( "BlSubFormHeader::BlSubFormHeader", 0 );
    m_nomcampo = nom;
    m_tipo = typ;
    m_restricciones = res;
    m_options = opt;
    m_nompresentacion = nomp;
    m_numericPrecision = 2;
    blDebug ( "END BlSubFormHeader::BlSubFormHeader", 0 );
}


///
/**
**/
BlSubFormHeader::~BlSubFormHeader()
{
    blDebug ( "BlSubFormHeader::~BlSubFormHeader", 0 );
    blDebug ( "END BlSubFormHeader::~BlSubFormHeader", 0 );
}


///
/**
\return
**/
unsigned int BlSubFormHeader::options()
{
    blDebug ( "BlSubFormHeader::options", 0 );
    blDebug ( "END BlSubFormHeader::options", 0 );
    return m_options;
}


///
/**
\return
**/
unsigned int BlSubFormHeader::restricciones()
{
    blDebug ( "BlSubFormHeader::restricciones", 0 );
    blDebug ( "END BlSubFormHeader::restricciones", 0 );
    return m_restricciones;
}


///
/**
\return
**/
BlDbField::DbType BlSubFormHeader::dbFieldType()
{
    blDebug ( "BlSubFormHeader::dbFieldType", 0 );
    blDebug ( "END BlSubFormHeader::dbFieldType", 0 );
    return m_tipo;

}


///
/**
\return
**/
QString BlSubFormHeader::nompresentacion()
{
    blDebug ( "BlSubFormHeader::nompresentacion", 0 );
    blDebug ( "END BlSubFormHeader::nompresentacion", 0 );
    return m_nompresentacion;
}


///
/**
\return
**/
int BlSubFormHeader::restrictcampo()
{
    blDebug ( "BlSubFormHeader::restrictcampo", 0 );
    blDebug ( "END BlSubFormHeader::restrictcampo", 0 );
    return m_restricciones;
}


///
/**
\return
**/
QString BlSubFormHeader::nomcampo()
{
    blDebug ( "BlSubFormHeader::nomcampo", 0 );
    blDebug ( "END BlSubFormHeader::nomcampo", 0 );
    return m_nomcampo;
}


/// Hace la exportacion del campo a XML
QString BlSubFormHeader::exportXML() {
    blDebug ( "BlSubFormHeader::exportXML", 0 );
    QString val;
    int error;

    val = "<BLSUBFORMHEADER>\n";
    val += "\t<NOMCAMPO>" + blXMLEncode(m_nomcampo) + "</NOMCAMPO>\n";
    val += "\t<NOMPRESENTACION>"+ blXMLEncode(m_nompresentacion)+"</NOMPRESENTACION>\n";
    val += "\t<RESTRICCIONES>"+ blXMLEncode(QString::number(m_restricciones))+"</RESTRICCIONES>\n";
    val += "\t<TIPO>"+ blXMLEncode(QString::number(m_tipo))+"</TIPO>\n";
    val += "\t<OPCIONES>"+ blXMLEncode(QString::number(m_options))+"</OPCIONES>\n";
    val += "</BLSUBFORMHEADER>\n";
    return val;
    blDebug ( "END BlSubFormHeader::exportXML", 0 );
}



/// BlSubForm, constructor de la clase base para subformularios.
/**
\param parent
**/
BlSubForm::BlSubForm ( QWidget *parent ) : BlWidget ( parent )
{
    blDebug ( "BlSubForm::BlSubForm", 0 );
    setupUi ( this );

    /// Por defecto es modo edicion
    m_modo = BL_EDIT_MODE;

    /// Inicializamos los valores de columna y fila previas para que no tengan un valor indefinido.
    /// Se inicializan con -1 para considerar que no hay celda previa.
    m_prevCol = -1;
    m_prevRow = -1;

    /// Desactivamos el procesado de cambios.
    m_procesacambios = FALSE;

    m_textoceldaParaRowSpan = "";
    m_filaInicialRowSpan = -1;

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BlSubForm_BlSubForm", this );
    if ( res != 0 ) {
        m_procesacambios = TRUE;
        return;
    } // end if

    mui_list->setSelectionMode ( QAbstractItemView::SingleSelection );
    mui_list->setSelectionBehavior ( QAbstractItemView::SelectRows );
//    mui_list->setHorizontalScrollMode ( QAbstractItemView::ScrollPerPixel );
//    mui_list->setVerticalScrollMode ( QAbstractItemView::ScrollPerPixel );
    mui_list->setSortingEnabled ( FALSE );
    /// \TODO Hay un Bug que impide ordenar bien los elementos.
    mui_list->horizontalHeader() ->setMovable ( TRUE );
    /// Valor por defecto en todos los listados.
    setListadoPijama ( TRUE );
    QPalette p;
    p = mui_list->palette();
    m_colorfondo1 = p.color ( QPalette::Normal, QPalette::Base );
    m_colorfondo2 = p.color ( QPalette::Normal, QPalette::AlternateBase );


    QHeaderView *hh = mui_list->horizontalHeader();

    /// Para conocer cuando el usuario altera el orden de las columnas.
    connect ( ( QObject * ) hh, SIGNAL ( sectionMoved ( int, int, int ) ), this, SLOT ( columnMovedByUser ( int, int, int ) ) );


    /// Capturamos la secuencia de teclas para hacer aparecer o desaparecer
    /// el panel de configuracion del BlSubForm.
    QShortcut *shortcut = new QShortcut ( QKeySequence ( "Ctrl+B" ), this );
    connect ( shortcut, SIGNAL ( activated() ), this, SLOT ( toogleConfig() ) );

    m_insercion = FALSE;
    m_primero = TRUE;
    m_sorting = FALSE;
    m_orden = FALSE;
    m_ordenporquery = FALSE;

    /// Para el listado de columnas hacemos una inicializacion.
    QStringList headers;
    headers << _ ( "Nombre de campo" ) << _ ( "Nombre" ) << _ ( "Visible" ) << _ ( "Orden" );
    mui_listcolumnas->setColumnCount ( 4 );
    mui_listcolumnas->setHorizontalHeaderLabels ( headers );
    mui_listcolumnas->setShowGrid ( FALSE );
    mui_listcolumnas->setSelectionBehavior ( QAbstractItemView::SelectRows );
    mui_listcolumnas->verticalHeader() ->hide();
    mui_listcolumnas->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    mui_listcolumnas->horizontalHeader() ->setResizeMode ( 0, QHeaderView::ResizeToContents );
    mui_listcolumnas->setColumnWidth ( 0, 175 );
    mui_listcolumnas->setColumnWidth ( 1, 100 );
    mui_listcolumnas->setColumnWidth ( 2, 0 );
    mui_listcolumnas->setColumnWidth ( 3, 0 );
    mui_listcolumnas->hideColumn ( 2 );
    mui_listcolumnas->hideColumn ( 3 );

    /// Siempre que arrancamos mostramos la pagina 0.
    mui_paginaact->setValue ( 1 );
    /// Ocultamos la configuracion.
    hideConfig();
    mui_menusubform->setVisible(FALSE);
    /// Limpiamos la lista.
    m_lista.clear();
    m_listaborrar.clear();

    /// Inicializamos los valores de columna y fila previas para que no tengan un valor indefinido.
    /// Se inicializan con -1 para considerar que no hay celda previa.
    m_prevCol = -1;
    m_prevRow = -1;
    setDelete ( TRUE );
    
    /// Disparamos los plugins.
    g_plugins->lanza ( "BlSubForm_BlSubForm_Post", this );
    m_procesacambios = TRUE;
    blDebug ( "END BlSubForm::BlSubForm", 0 );
}


void BlSubForm::columnMovedByUser ( int, int oldIndex, int newIndex )
{
    blDebug ( "BlSubForm::columnMovedByUser", 0 );
    mui_listcolumnas->moveRow ( oldIndex, newIndex );
    blDebug ( "END BlSubForm::columnMovedByUser", 0 );
}


/// Destructor de Clase que guarda la configuracion.
/**
**/
BlSubForm::~BlSubForm()
{
    blDebug ( "BlSubForm::~BlSubForm", 0 );
    /// PAra destruir desactivamos el control de cambios.
    m_procesacambios = FALSE;
    guardaconfig();
    blDebug ( "END BlSubForm::~BlSubForm", 0 );
}


///
/**
\param emp
**/
void BlSubForm::setMainCompany ( BlMainCompany *emp )
{
    blDebug ( "BlMainCompanyPointer::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( emp );


    if ( tableName() != "" && mainCompany() != NULL ) {
      /// Averiguamos de los campos que ya estan introducidos la precision de los de tipo numerico.
      BlSubFormHeader *linea;
      for ( int i = 0; i < m_lcabecera.size(); ++i ) {
          linea = m_lcabecera.at ( i );

          /// Comprobamos la precision del campo (si este es del tipo numerico) y la guardamos.
          /// Lo hacemos aqui y no en el constructor de BlDbField porque precisamos del nombre de la tabla para
          /// poder hacer la consulta sobre la precisión del campo y ese dato solo esta en BlDbRecord.
          /// Buscamos los decimales que tiene el campo y establecemos el numero de decimales a ese valor.
          /// NOTA: En caso de que no haya sido posible calcular la precisión esta se establece por defecto como 2.
          if (linea->dbFieldType() == BlDbField::DbNumeric ) {

                QString query2 = "SELECT numeric_scale FROM information_schema.columns WHERE table_name = '" + tableName() + "' and column_name = '" + linea->nomcampo() + "';";
                BlDbRecordSet *cur = mainCompany() ->loadQuery ( query2 );
                if ( !cur->eof() ) {
                    linea->setNumericPrecision(cur->valor("numeric_scale").toInt());
                } // end if
                delete cur;
          } // end if
      } // end for
    } // end if

    cargaSpecs();
    blDebug ( "END BlMainCompanyPointer::setMainCompany", 0 );
}


///
/**
**/
void BlSubForm::cargaSpecs()
{
    blDebug ( "BlSubForm::cargaSpecs", 0 );
//    QFile file(g_confpr->valor(CONF_DIR_USER) + m_fileconfig + "_" + mainCompany()->dbName() + "_specs.spc");
    QFile file ( CONFIG_DIR_CONFIG + m_fileconfig + "_" + mainCompany() ->dbName() + "_specs.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        blDebug ( "END BlSubForm::cargaSpecs", 0, "No se pudo abrir archivo" );
        return;
    }
    if ( !doc.setContent ( &file ) ) {
        file.close();
        blDebug ( "END BlSubForm::cargaSpecs", 0, "XML no valido" );
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "SUBFORM" );
    /// Cogemos la coordenada X
    QString tablename = principal.firstChildElement ( "TABLENAME" ).toElement().text();
    QString campoid = principal.firstChildElement ( "CAMPOID" ).toElement().text();

    QDomNodeList nodos = docElem.elementsByTagName ( "HEADER" );
    for ( int i = 0; i < nodos.count(); i++ ) {
        QDomNode ventana = nodos.item ( i );
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
            BlDbField::DbType type = BlDbField::DbVarChar;
            QString nomheader = e1.firstChildElement ( "NOMHEADER" ).toElement().text();
            QString nompheader = e1.firstChildElement ( "NOMPHEADER" ).toElement().text();
            QString typeheader = e1.firstChildElement ( "DBTYPEHEADER" ).toElement().text();
            if ( typeheader == "DBVARCHAR" ) {
                type = BlDbField::DbVarChar;
            } else if ( typeheader == "DBINT" ) {
                type = BlDbField::DbInt;
            } else if ( typeheader == "DBNUMERIC" ) {
                type = BlDbField::DbNumeric;
            } else if ( typeheader == "DBBOOLEAN" ) {
                type = BlDbField::DbBoolean;
            } else if ( typeheader == "DBDATE" ) {
                type = BlDbField::DbDate;
            } // end if

            int restricciones = ( int ) BlDbField::DbNothing;
            QDomElement restrict = e1.firstChildElement ( "RESTRICTIONSHEADER" );
            while ( !restrict.isNull() ) {
                QString trestrict = restrict.text();
                if ( trestrict == "DBNOTHING" ) {
                    restricciones |= BlDbField::DbVarChar;
                } else if ( trestrict == "DBNOTNULL" ) {
                    restricciones |= BlDbField::DbNotNull;
                } else if ( trestrict == "DBPRIMARYKEY" ) {
                    restricciones |= BlDbField::DbPrimaryKey;
                } else if ( trestrict == "DBNOSAVE" ) {
                    restricciones |= BlDbField::DbNoSave;
                } else if ( trestrict == "DBAUTO" ) {
                    restricciones |= BlDbField::DbAuto;
                } else if ( trestrict == "DBDUPPRIMARYKEY" ) {
                    restricciones |= BlDbField::DbDupPrimaryKey;
                } else if ( trestrict == "DBREQUIRED" ) {
                    restricciones |= BlDbField::DbRequired;
                } else if ( trestrict == "DBNOLOAD" ) {
                    restricciones |= BlDbField::DbNoLoad;
                } // end if
                restrict = restrict.nextSiblingElement ( "RESTRICTIONSHEADER" );
            } // end while

            int opciones = ( int ) BlSubFormHeader::DbNone;
            QDomElement opci = e1.firstChildElement ( "OPTIONSHEADER" );
            while ( !opci.isNull() ) {
                QString topci = opci.text();
                if ( topci == "DBNONE" ) {
                    opciones |= BlSubFormHeader::DbNone;
                } else if ( topci == "DBREADONLY" ) {
                    opciones |= BlSubFormHeader::DbReadOnly;
                } else if ( topci == "DBHIDEVIEW" ) {
                    opciones |= BlSubFormHeader::DbHideView;
                } else if ( topci == "DBNOWRITE" ) {
                    opciones |= BlSubFormHeader::DbNoWrite;
                } else if ( topci == "DBDISABLEVIEW" ) {
                    opciones |= BlSubFormHeader::DbDisableView;
                } // end if
                opci = opci.nextSiblingElement ( "OPTIONSHEADER" );
            } // end while

            addSubFormHeader ( nomheader, type, ( BlDbField::DbRestrict ) restricciones, ( BlSubFormHeader::DbOptions ) opciones, nompheader );
        } // end if
    } // end for

    blDebug ( "END BlSubForm::cargaSpecs", 0 );
}


///
/**
\return
**/
QList<BlDbSubFormRecord *> *BlSubForm::lista()
{
    blDebug ( "BlSubForm::lista", 0 );
    blDebug ( "END BlSubForm::lista", 0 );
    return &m_lista;
}


///
/**
\return
**/
QList<BlSubFormHeader *> *BlSubForm::cabecera()
{
    blDebug ( "BlSubForm::cabecera", 0 );
    blDebug ( "END BlSubForm::cabecera", 0 );
    return &m_lcabecera;
}


///
/**
\param mode
**/
void BlSubForm::setHorizontalScrollMode ( QAbstractItemView::ScrollMode mode )
{
    blDebug ( "BlSubForm::setHorizontalScrollMode", 0 );
    mui_list->setHorizontalScrollMode ( mode );
    blDebug ( "END BlSubForm::setHorizontalScrollMode", 0 );
}


///
/**
\param f
**/
void BlSubForm::setDelete ( bool f )
{
    blDebug ( "BlSubForm::setDelete", 0 );
    m_delete = f;
    blDebug ( "END BlSubForm::setDelete", 0 );
}


///
/**
**/
bool BlSubForm::isDelete() {
    blDebug ( "BlSubForm::isDelete", 0 );
    blDebug ( "END BlSubForm::isDelete", 0 );
    return m_delete;
}


///
/**
\param row
\param col
\return
**/
BlDbSubFormField *BlSubForm::item ( int row, int col )
{
    blDebug ( "BlSubForm::item", 0 );
    blDebug ( "END BlSubForm::item", 0 );
    return ( BlDbSubFormField * ) mui_list->item ( row, col );
}


///
/**
\param row
\param col
**/
void BlSubForm::setCurrentItem ( int row, int col )
{
    blDebug ( "BlSubForm::setCurrentItem", 0 );
    mui_list->setCurrentItem ( item ( row, col ) );
    blDebug ( "END BlSubForm::setCurrentItem", 0 );
}


///
/**
\return
**/
int BlSubForm::columnCount()
{
    blDebug ( "BlSubForm::columnCount", 0 );
    blDebug ( "END BlSubForm::columnCount", 0 );
    return mui_list->columnCount();
}


///
/**
\param i
**/
void BlSubForm::showColumn ( int i )
{
    blDebug ( "BlSubForm::showColumn", 0 );
    mui_list->showColumn ( i );
    blDebug ( "END BlSubForm::showColumn", 0 );
}


///
/**
\param i
\return
**/
int BlSubForm::columnWidth ( int i )
{
    blDebug ( "BlSubForm::columnWidth", 0 );
    blDebug ( "END BlSubForm::columnWidth", 0 );
    return mui_list->columnWidth ( i );
}


///
/**
\param i
\param j
**/
void BlSubForm::setColumnWidth ( int i, int j )
{
    blDebug ( "BlSubForm::setColumnWidth", 0 );
    mui_list->setColumnWidth ( i, j );
    blDebug ( "END BlSubForm::setColumnWidth", 0 );
}


///
/**
\param i
**/
void BlSubForm::hideColumn ( int i )
{
    blDebug ( "BlSubForm::hideColumn", 0 );
    mui_list->hideColumn ( i );
    blDebug ( "END BlSubForm::hideColumn", 0 );
}


///
/**
\return
**/
int BlSubForm::currentRow()
{
    blDebug ( "BlSubForm::currentRow", 0 );
    blDebug ( "END BlSubForm::currentRow", 0 );
    return mui_list->currentRow();
}


///
/**
\return
**/
int BlSubForm::rowCount()
{
    blDebug ( "BlSubForm::rowCount", 0 );
    blDebug ( "END BlSubForm::rowCount", 0 );
    return mui_list->rowCount();
}


///
/**
\return
**/
int BlSubForm::currentColumn()
{
    blDebug ( "BlSubForm::currentColumn", 0 );
    blDebug ( "END BlSubForm::currentColumn", 0 );
    return mui_list->currentColumn();
}


///
/**
\param modo
**/
void BlSubForm::setResizeMode ( QHeaderView::ResizeMode modo )
{
    blDebug ( "BlSubForm::setResizeMode", 0 );
    mui_list->horizontalHeader() ->setResizeMode ( modo );
    blDebug ( "END BlSubForm::setResizeMode", 0 );
}


///
/**
**/
void BlSubForm::resizeColumnsToContents()
{
    blDebug ( "BlSubForm::resizeColumnsToContents", 0 );
    mui_list->resizeColumnsToContents();
    blDebug ( "END BlSubForm::resizeColumnsToContents", 0 );
}


///
/**
**/
void BlSubForm::resizeRowsToContents()
{
    blDebug ( "BlSubForm::resizeRowsToContents", 0 );
    mui_list->resizeRowsToContents();
    blDebug ( "END BlSubForm::resizeRowsToContents", 0 );
}


///
/**
\param i
**/
void BlSubForm::resizeColumnToContents ( int i )
{
    blDebug ( "BlSubForm::resizeColumnToContents", 0 );
    mui_list->resizeColumnToContents ( i );
    blDebug ( "END BlSubForm::resizeColumnToContents", 0 );
}


///
/**
\param i
**/
void BlSubForm::resizeRowToContents ( int i )
{
    blDebug ( "BlSubForm::resizeRowToContents", 0 );
    mui_list->resizeRowToContents ( i );
    blDebug ( "END BlSubForm::resizeRowToContents", 0 );
}


///
/**
**/
void BlSubForm::hideConfig()
{
    blDebug ( "BlSubForm::hideConfig", 0 );
    mui_configurador->hide();
    blDebug ( "END BlSubForm::hideConfig", 0 );
}


///
/**
**/
void BlSubForm::showConfig()
{
    blDebug ( "BlSubForm::showConfig", 0 );
    mui_configurador->show();
    blDebug ( "END BlSubForm::showConfig", 0 );
}


///
/**
\param nom
**/
void BlSubForm::setDbTableName ( QString nom )
{
    blDebug ( "BlSubForm::setDbTableName", 0 );
    m_tablename = nom;
    m_fileconfig = nom;
    blDebug ( "END BlSubForm::setDbTableName", 0 );
}


///
/**
\return
**/
QString BlSubForm::tableName()
{
    blDebug ( "BlSubForm::tableName", 0 );
    blDebug ( "END BlSubForm::tableName", 0 );
    return m_tablename;
}


///
/**
\param nom
**/
void BlSubForm::setFileConfig ( QString nom )
{
    blDebug ( "BlSubForm::setFileConfig", 0 );
    m_fileconfig = nom;
    blDebug ( "END BlSubForm::setFileConfig", 0 );
}


///
/**
\param nom
**/
void BlSubForm::setDbFieldId ( QString nom )
{
    blDebug ( "BlSubForm::setDbFieldId", 0 );
    m_campoid = nom;
    blDebug ( "END BlSubForm::setDbFieldId", 0 );
}


///
/**
**/
QString BlSubForm::dbFieldId() {
    blDebug ( "BlSubForm::dbFieldId", 0 );
    blDebug ( "END BlSubForm::dbFieldId", 0 );
    return m_campoid;
}


///
/**
**/
void BlSubForm::clear()
{
    blDebug ( "BlSubForm::clear", 0 );
    mui_list->clear();
    blDebug ( "END BlSubForm::clear", 0 );
}


///
/**
\param i
**/
void BlSubForm::setRowCount ( int i )
{
    blDebug ( "BlSubForm::setRowCount", 0 );
    mui_list->setRowCount ( i );
    blDebug ( "END BlSubForm::setRowCount", 0 );
}


///
/**
\param i
**/
void BlSubForm::setColumnCount ( int i )
{
    blDebug ( "BlSubForm::setColumnCount", 0 );
    mui_list->setColumnCount ( i );
    blDebug ( "END BlSubForm::setColumnCount", 0 );
}


///
/**
**/
void BlSubForm::creaMenu ( QMenu * )
{
    blDebug ( "BlSubForm:: CreaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
**/
void BlSubForm::procesaMenu ( QAction * )
{
    blDebug ( "BlSubForm:: procesaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
\param pijama
**/
void BlSubForm::setListadoPijama ( bool pijama )
{
    blDebug ( "BlSubForm::setListadoPijama", 0 );
    if ( pijama ) {
        mui_list->setAlternatingRowColors ( TRUE );
    } else {
        mui_list->setAlternatingRowColors ( FALSE );
    } // end if
    blDebug ( "END BlSubForm::setListadoPijama", 0 );
}


///
/**
\return
**/
bool BlSubForm::listadoPijama()
{
    blDebug ( "BlSubForm::listadoPijama", 0 );
    return mui_list->alternatingRowColors();
    blDebug ( "END BlSubForm::listadoPijama", 0 );
}



/// Habilita o inhabilita el ordenado de columnas mediante el pulsar sobre ellas.
/**
\param sorting
**/
void BlSubForm::setSortingEnabled ( bool sorting )
{
    blDebug ( "BlSubForm::setSortingEnabled", 0 );
    mui_list->setSortingEnabled ( sorting );
    m_sorting = sorting;
    blDebug ( "END BlSubForm::setSortingEnabled", 0 );
}


///
/**
\return
**/
bool BlSubForm::sortingEnabled()
{
    blDebug ( "BlSubForm::sortingEnabled", 0 );
    blDebug ( "END BlSubForm::sortingEnabled", 0 );
    return m_sorting;
}


/// Establece si el subformulario se ordena mediante un campo orden en la base de datos.
/**
\param sorting
**/
void BlSubForm::setOrdenEnabled ( bool sorting )
{
    blDebug ( "BlSubForm::setOrdenEnabled", 0 );
    m_orden = sorting;
    blDebug ( "END BlSubForm::setOrdenEnabled", 0 );
}


///
/**
\return
**/
bool BlSubForm::ordenEnabled()
{
    blDebug ( "BlSubForm::ordenEnabled", 0 );
    return m_orden;
    blDebug ( "END BlSubForm::ordenEnabled", 0 );
}


///
/**
\param item
**/
void BlSubForm::on_mui_list_itemDoubleClicked ( QTableWidgetItem *item )
{
    blDebug ( "BlSubForm::on_mui_list_itemDoubleClicked", 0 );
    emit itemDoubleClicked ( item );
    blDebug ( "END BlSubForm::on_mui_list_itemDoubleClicked", 0 );
}

///
/**
\param item
**/
void BlSubForm::on_mui_list_itemClicked ( QTableWidgetItem *item )
{
    blDebug ( "BlSubForm::on_mui_list_itemClicked", 0 );
    emit itemClicked ( item );
    blDebug ( "END BlSubForm::on_mui_list_itemClicked", 0 );
}


///
/**
\param row
\param col
**/
void BlSubForm::on_mui_list_cellDoubleClicked ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_cellDoubleClicked", 0 );
    emit cellDoubleClicked ( row, col );
    blDebug ( "END BlSubForm::on_mui_list_cellDoubleClicked", 0 );
}


///
/**
\param head
\return
**/
bool BlSubForm::existsHeader ( const QString &head )
{
    blDebug ( "BlSubForm::existsHeader", 0 );
    BlSubFormHeader *linea;
    for ( int i = 0; i < m_lcabecera.size(); ++i ) {
        linea = m_lcabecera.at ( i );
        if ( linea->nomcampo() == head ) {
            blDebug ( "END BlSubForm::existsHeader", 0 );
            return TRUE;
        } // end if
    } // end for
    blDebug ( "END BlSubForm::existsHeader", 0 );
    return FALSE;
}

///
/**
\param head
\return
**/
BlSubFormHeader * BlSubForm::header ( const QString &head )
{
    blDebug ( "BlSubForm::existsHeader", 0 );
    BlSubFormHeader *linea = NULL;
    for ( int i = 0; i < m_lcabecera.size(); ++i ) {
        linea = m_lcabecera.at ( i );
        if ( linea->nomcampo() == head ) {
            blDebug ( "END BlSubForm::existsHeader", 0 );
            return linea;
        } // end if
    } // end for
    blDebug ( "END BlSubForm::existsHeader", 0 );
    return NULL;
}




const int BlSubFormHeader::numericPrecision() {
    blDebug ( "BlSubFormHeader::numericPrecision", 0 );
    return m_numericPrecision;
    blDebug ( "END BlSubFormHeader::numericPrecision", 0 );
}


void BlSubFormHeader::setNumericPrecision(int pres) {
    blDebug ( "BlSubFormHeader::setNumericPrecision", 0 );
    m_numericPrecision = pres;
    blDebug ( "END BlSubFormHeader::setNumericPrecision", 0 );
}



/// Se encarga de crear un nuevo registro (una fila entera) y de inicializarla para que
/// tenga todos los elementos necesarios (columnas).
/// Nota: Esta funcion es de uso interno, no debe ser usada.
/**
\return
**/
BlDbSubFormRecord *BlSubForm::newDbSubFormRecord()
{
    blDebug ( "BlSubForm::newDbSubFormRecord", 0 );
    BlDbSubFormRecord *rec = new BlDbSubFormRecord ( mainCompany() );
    rec->setDbTableName ( m_tablename );
    rec->setDbFieldId ( m_campoid );

    BlSubFormHeader *linea;
    for ( int i = 0; i < m_lcabecera.size(); ++i ) {
        linea = m_lcabecera.at ( i );
        rec->addDbField ( linea->nomcampo(), linea->dbFieldType(), linea->restricciones(), linea->nompresentacion() );
    } // end for

    BlDbSubFormField *camp;
    for ( int i = 0; i < rec->lista() ->size(); ++i ) {
        camp = ( BlDbSubFormField * ) rec->lista() ->at ( i );
        BlSubFormHeader *head = m_lcabecera.at ( i );
        Qt::ItemFlags flags = 0;
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if ( ! ( head->options() & BlSubFormHeader::DbNoWrite ) )
            flags |= Qt::ItemIsEditable;
        if ( head->dbFieldType() == BlDbField::DbBoolean ) {
            flags |= Qt::ItemIsUserCheckable;
        } // end if

        camp->setFlags ( flags );

        /// Tratamos el tema de la alineacion dependiendo del tipo.
        if ( head->dbFieldType() == BlDbField::DbInt || head->dbFieldType() == BlDbField::DbNumeric || head->dbFieldType() == BlDbField::DbDate ) {
            camp->setTextAlignment ( Qt::AlignRight | Qt::AlignVCenter );
        } else {
            camp->setTextAlignment ( Qt::AlignLeft | Qt::AlignVCenter );
        } // end if
    } // end for
    blDebug ( "END BlSubForm::newDbSubFormRecord", 0 );
    return rec;
}


/// Este metodo crea el registro final cuando se trata de subformularios con la opcion
/// de insertar nuevos registros en el subformulario.
/**
\return
**/
void BlSubForm::nuevoRegistro()
{
    blDebug ( "BlSubForm::nuevoRegistro", 0 );
    if ( !m_insercion ) {
        blDebug ( "END BlSubForm::nuevoRegistro", 0, "No se permiten inserciones" );
        return;
    } // end if

    /// Desactivamos la ordenacion automatica.
    mui_list->setSortingEnabled ( FALSE );

    BlDbSubFormRecord *rec = newDbSubFormRecord();

    m_lista.append ( rec );

    mui_list->insertRow ( m_lista.size() - 1 );
    BlDbSubFormField *camp;
    for ( int i = 0; i < rec->lista() ->size(); ++i ) {
        camp = ( BlDbSubFormField * ) rec->lista() ->at ( i );
        mui_list->setItem ( m_lista.size() - 1, i, camp );
    } // end for

    /// Nos posicionamos en una celda del nuevo registro ya que si no un bug no nos permite usar currentRow()
    mui_list->setCurrentCell ( m_lista.size() - 1, 2 );

    /// Activamos la ordenacion automatica.
    mui_list->setSortingEnabled ( m_sorting );
    blDebug ( "END BlSubForm::nuevoRegistro", 0 );
}


/// Pinta las cabeceras en la tabla.
/**
**/
void BlSubForm::pintaCabeceras()
{
    blDebug ( "BlSubForm::", 0 );
    QStringList headers;
    BlSubFormHeader *linea;
    for ( int i = 0; i < m_lcabecera.size(); ++i ) {
        linea = m_lcabecera.at ( i );
        headers << linea->nompresentacion();
        if ( linea->options() & BlSubFormHeader::DbHideView ) {
            mui_list->hideColumn ( i );
        } else {
            mui_list->showColumn ( i );
        } // end if
    } // end for
    mui_list->setHorizontalHeaderLabels ( headers );
    mui_list->horizontalHeader() ->setResizeMode ( 0, QHeaderView::Interactive );
    blDebug ( "END BlSubForm::pintaCabeceras", 0 );
}


/// Se situa en una celda especifica del subformulario.
/**
\param row
\param col
\param back Recorrer las celdas en orden inverso (por defecto, falso)
**/
void BlSubForm::situarse ( unsigned int row, unsigned int col, bool back )
{
    blDebug ( "BlSubForm::situarse", 0, QString::number ( row ) + " " + QString::number ( col ) );

    unsigned int nrow = row;
    unsigned int ncol = col;
    BlSubFormHeader *linea = m_lcabecera.at ( ncol );
    if ( !linea ) {
	  blDebug ( "END BlSubForm::situarse", 0, QString::number ( nrow ) + " " + QString::number ( ncol ) );
        return;
    } // end if
    bool invalido = TRUE;

    /// Mientras no se encuentre un candidato y haya candidatos vamos recorriendo celdas inversamente.
    while ( back && invalido && nrow >= 0 ) {

	  /// Si estamos al principio de la linea cogemos el principio de la linea anterior
	  if ( ncol == 0 ) {
		if ( nrow > 0 ) nrow--;
		else break;

		ncol = ( unsigned int ) m_lcabecera.count() - 1;
	  }
	  else {
		ncol--;
	  } // end if

	  linea = m_lcabecera.at ( ncol );
	  invalido = FALSE;
	  if ( ( linea->options() & BlSubFormHeader::DbHideView )
	  ||   ( linea->options() & BlSubFormHeader::DbNoWrite ) )
		invalido = TRUE;
     } // end while

    /// Mientras no se encuentre un candidato y haya candidatos vamos recorriendo celdas.
    while ( !back && invalido && nrow < row + 2 ) {
	  ncol++;
	  /// Si estamos al final de la linea cogemos el principio de la linea siguiente
	  if ( ncol == ( unsigned int ) m_lcabecera.count() ) {
		ncol = 0;
		nrow++;
	  } // end if
	  linea = m_lcabecera.at ( ncol );
	  invalido = FALSE;
	  if ( ( linea->options() & BlSubFormHeader::DbHideView )
	  ||   ( linea->options() & BlSubFormHeader::DbNoWrite ) )
		 invalido = TRUE;
    } // end while

    if (!invalido) {
       mui_list->setCurrentCell ( nrow, ncol );
    } else {
       blDebug("No hay más elementos editables en el subformulario", 2);
    } // end if
    blDebug ( "END BlSubForm::situarse", 0, QString::number ( nrow ) + " " + QString::number ( ncol ) );
}


/// Se situa en una celda especifica del subformulario.
/** Se incluye la celda incluye la actual */
/**
\param row
\param col
\return
**/
void BlSubForm::situarse1 ( unsigned int row, unsigned int col )
{
    blDebug ( "BlSubForm::situarse1", 0, QString::number ( row ) + " " + QString::number ( col ) );
    unsigned int nrow = row;
    unsigned int ncol = col;
    BlSubFormHeader *linea = m_lcabecera.at ( ncol );
    if ( !linea ) {
        return;
    } // end if
    bool invalido = TRUE;
    while ( invalido ) {
        if ( ncol == ( unsigned int ) m_lcabecera.count() ) {
            ncol = 0;
            nrow++;
        } // end if
        linea = m_lcabecera.at ( ncol );
        invalido = FALSE;
        if ( linea->options() & BlSubFormHeader::DbHideView )
            invalido = TRUE;
        if ( linea->options() & BlSubFormHeader::DbNoWrite )
            invalido = TRUE;
        if ( invalido )
            ncol++;
    } // end while
    mui_list->setCurrentCell ( nrow, ncol );
    blDebug ( "END BlSubForm::situarse1", 0 );
}


/// Cuando tenemos un registro que no se tiene que cargar (pq es nuevo o algo
/// asi) de la base de datos, con la funcion pintar lo dejamos en un estado que
/// se podria considerar presentable para poder operar con el subformulario.
/**
**/
void BlSubForm::pintar()
{
    blDebug ( "BlSubForm::pintar", 0 );
    m_procesacambios = FALSE;
    mui_list->setColumnCount ( m_lcabecera.count() );
    pintaCabeceras();
    if ( m_primero ) {
        cargaconfig();	
	/// Preparamos el menu de subformulario
	preparaMenu();
    } // end if
    nuevoRegistro();
    m_procesacambios = TRUE;
    blDebug ( "END BlSubForm::pintar", 0 );
}


/// Carga una tabla a partir del recordset que se le ha pasado.
/**
\return
**/
int BlSubForm::inicializar()
{
    blDebug ( "BlSubForm::inicializar", 0 );
    m_procesacambios = FALSE;
    mui_query->setPlainText ( "" );
    BlDbSubFormRecord *rec;

    int filpag = mui_filaspagina->text().toInt();
    if ( filpag <= 0 )
        filpag = 500;

    int pagact = mui_paginaact->text().toInt();
    if ( pagact <= 0 )
        pagact = 1;

    /// Vaciamos la tabla para que no contenga registros.
    mui_list->clear();
    mui_list->setRowCount ( 0 );

    /// Vaciamos el recordset para que no contenga registros.
    while ( m_lista.count() ) {
        rec = m_lista.takeFirst();
        if ( rec )
            delete rec;
    } // end while

    /// Ponemos los datos sobre el query.
    mui_numfilas->setText ( "0" );
    int numpag = 0;
    mui_numpaginas->setText ( QString::number ( numpag ) );
    /// Inicializamos las columnas y pintamos las cabeceras.
    mui_list->setColumnCount ( m_lcabecera.count() );
    pintaCabeceras();
    if ( m_primero ) {
        cargaconfig();
	/// Preparamos el menu de subformulario
	preparaMenu();
    } // end if

    nuevoRegistro();
    /// Ordenamos la tabla.
    mui_list->ordenar();
    /// configuramos que registros son visibles y que registros no lo son.
    on_mui_confcol_clicked();
    m_procesacambios = TRUE;
    blDebug ( "END BlSubForm::inicializar", 0 );
    return 0;
}


///
/**
\param campo
**/
void BlSubForm::setColumnToRowSpan ( QString campo )
{
    blDebug ( "BlSubForm::setColumnToRowSpan", 0 );
    m_columnaParaRowSpan = campo;
    blDebug ( "END BlSubForm::setColumnToRowSpan", 0 );
}


///
/**
\param color
**/
void BlSubForm::setColorFondo1 ( QString color )
{
    blDebug ( "BlSubForm::setColorFondo1", 0 );
    m_colorfondo1 = QColor ( color );
    blDebug ( "END BlSubForm::setColorFondo1", 0 );
}


///
/**
\param color
**/
void BlSubForm::setColorFondo2 ( QString color )
{
    blDebug ( "BlSubForm::setColorFondo2", 0 );
    m_colorfondo2 = QColor ( color );
    blDebug ( "END BlSubForm::setColorFondo1", 0 );
}


///
/**
\param twidget
\param filainicial
\param totalfilas
\param colorfondo
**/
void BlSubForm::ponItemColorFondo ( QTableWidget *twidget, int filainicial, int totalfilas, QColor colorfondo )
{
    BlDbSubFormRecord * reg3;
    /// Si hay que poner color al fondo de las filas se pone el que toque.
    if ( listadoPijama() == FALSE ) {
        /// Recorre todos los items de las filas afectadas.
        for ( int k = filainicial; k < filainicial + totalfilas; ++k ) {
            reg3 = m_lista.at ( k );
            for ( int r = 0; r < reg3->lista() ->size(); ++r ) {
                /// Pone el color
                twidget->item ( k, r ) ->setBackground ( QBrush ( colorfondo ) );
            } // end for
        } // end for
    } // end if
}


/// Carga una tabla a partir del recordset que se le ha pasado.
/** Este m&eacute;todo genera, a partir del recordset pasado como par&aacute;metro
    el listado y lo muestra. */
/**
\param cur
**/
void BlSubForm::cargar ( BlDbRecordSet *cur )
{
    blDebug ( "BlSubForm::cargar", 0, objectName() );
    m_procesacambios = FALSE;


    BlDbSubFormRecord *reg;
    BlDbSubFormRecord *reg2;
    BlDbSubFormField *camp;
    BlDbSubFormField *camp2;
    QColor colorfondo = m_colorfondo1;
    bool coloraponerfondo = FALSE;

    /// Preparamos la barra de progreso
    BlProgressBar *barra = new BlProgressBar;
//    barra.setText ( _( "Cargando SubFormulario" ) );
    if ( cur->numregistros() > 100 ) {
        barra->setValue ( 0 );
        barra->show();
        barra->setText ( _ ( "Cargando SubFormulario " ) + m_tablename );
    } // end if

    /// Desactivamos el sorting debido a un error en las Qt4.
    mui_list->setSortingEnabled ( FALSE );

    /// Reseteamos el "rowSpan" de la tabla antes de borrar las filas.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        reg = m_lista.at ( i );
        for ( int j = 0; j < reg->lista() ->size(); ++j ) {
            BlSubFormHeader *head = m_lcabecera.at ( j );
            if ( head->nomcampo() == m_columnaParaRowSpan ) {
                mui_list->setSpan ( i, j, 1, 1 );
            } // end if
        } // end for
    } // end for

    /// Vaciamos la tabla para que no contenga registros.
    mui_list->clear();
    mui_list->setRowCount ( 0 );

    /// Vaciamos el recordset para que no contenga registros.
    BlDbSubFormRecord *rec;
    while ( m_lista.count() ) {
        rec = m_lista.takeFirst();
        if ( rec )
            delete rec;
    } // end while

    /// Inicializamos las columnas y pintamos las cabeceras.
    mui_list->setColumnCount ( m_lcabecera.count() );
    pintaCabeceras();

    /// Si hay un problema con el cursor, se sale antes de generar Segmentation Fault.
    if ( cur == NULL ) return;

    /// Ponemos la consulta a la vista para que pueda ser editada.
    mui_query->setPlainText ( cur->pristineQuery() );

    /// Tratamos con la paginacion.
    int filpag = mui_filaspagina->text().toInt();
    if ( filpag <= 0 ) {
        filpag = 500;
    } // end if

    int pagact = mui_paginaact->text().toInt();
    if ( pagact <= 0 ) {
        pagact = 1;
    } // end if

    /// Ponemos los datos sobre la consulta.
    mui_numfilas->setText ( QString::number ( cur->numregistros() ) );
    int numpag = cur->numregistros() / filpag + 1;
    mui_numpaginas->setText ( QString::number ( numpag ) );

    /// Si el numero de elementos es igual al numero de filas por pagina abilitamos la pagina siguiente
    if ( filpag == cur->numregistros() ) {
        mui_pagsiguiente->setEnabled ( TRUE );
    } else {
        mui_pagsiguiente->setDisabled ( TRUE );
    } // end if

    /// Si la pagina actual es 1 desabilitamos la pagina anterior
    if ( pagact ==  1 ) {
        mui_paganterior->setDisabled ( TRUE );
    } else {
        mui_paganterior->setEnabled ( TRUE );
    } // end if

    /// Recorremos el recordset y ponemos los registros en un orden determinado.
    int porcentajecarga = 0;
    while ( !cur->eof() && m_lista.count() < filpag ) {
        BlDbSubFormRecord * rec = newDbSubFormRecord();
        rec->DBload ( cur );
        m_lista.append ( rec );
        cur->nextRecord();
        porcentajecarga++;
    } // end while

    /// Inicializamos la tabla con las filas necesarias.
    mui_list->setRowCount ( m_lista.count() );
    barra->setRange ( 0, m_lista.size() );
    for ( int i = 0; i < m_lista.size(); ++i ) {
        reg = m_lista.at ( i );
        QRegExp patronFecha ( "^.*00:00:00.*$" ); /// Para emparejar los valores fechas.
        QFont bold;
        bold.setBold ( true );
        for ( int j = 0; j < reg->lista() ->size(); ++j ) {
           /// Poner en negrita el texto de la cabecera de las columnas editables
           if ( i == 0 && ! ( m_lcabecera [ j ] -> options() & BlSubFormHeader::DbNoWrite ) ) {
              mui_list->horizontalHeaderItem ( j ) -> setFont( bold );
           } // end if
            camp = ( BlDbSubFormField * ) reg->lista() ->at ( j );
            /// Si es una fecha lo truncamos a 10 caracteres para presentar solo la fecha.
            if ( patronFecha.exactMatch ( camp->valorcampo() ) ) {
                camp->set
                ( camp->valorcampo().left ( 10 ) );
            } // end if
            /// Rellena la tabla con los datos.
            mui_list->setItem ( i, j, camp );
        } // end for
        barra->setValue ( i );
    } // end for

    /// Establece el "rowSpan" de la tabla.
    QString textoCeldaAnterior;
    QString textoCeldaActual;

    /// Recorre las filas.
    m_filaInicialRowSpan = -1;

    /// Pone el 'rowSpan' a las filas que son iguales.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        reg = m_lista.at ( i );
        for ( int j = 0; j < reg->lista() ->size(); ++j ) {
            BlSubFormHeader *head = m_lcabecera.at ( j );
            if ( head->nomcampo() == m_columnaParaRowSpan ) {
                camp = ( BlDbSubFormField * ) reg->lista() ->at ( j );
                textoCeldaActual = camp->valorcampo();
                /// Mira lo que hay en la fila anterior si existe.
                if ( i > 0 ) {
                    reg2 = m_lista.at ( i - 1 );
                    camp2 = ( BlDbSubFormField * ) reg2->lista() ->at ( j );
                    textoCeldaAnterior = camp2->valorcampo();
                    if ( textoCeldaActual == textoCeldaAnterior ) {
                        /// activamos el indice de celdas iguales
                        if ( m_filaInicialRowSpan == -1 ) {
                            m_filaInicialRowSpan = i - 1;
                        } // end if
                        /// hay un registro despu&eacute;s. No, dibuja 'rowSpan'.
                        if ( i == ( m_lista.size() - 1 ) ) {
                            ponItemColorFondo ( mui_list, m_filaInicialRowSpan, i - m_filaInicialRowSpan + 1, colorfondo );
                            mui_list->setSpan ( m_filaInicialRowSpan, j, i - m_filaInicialRowSpan + 1, 1 );

                            if ( coloraponerfondo == FALSE ) {
                                colorfondo = m_colorfondo2;
                                coloraponerfondo = TRUE;
                            } else {
                                colorfondo = m_colorfondo1;
                                coloraponerfondo = FALSE;
                            } // end if

                        } // end if
                    } else {
                        /// Comprobamos si queda algo pendiente de hacer 'rowSpan'.
                        if ( m_filaInicialRowSpan != -1 ) {
                            /// 'rowSpan' desde inicio iguales hasta fila anterior.
                            ponItemColorFondo ( mui_list, m_filaInicialRowSpan, i - m_filaInicialRowSpan, colorfondo );
                            mui_list->setSpan ( m_filaInicialRowSpan, j, i - m_filaInicialRowSpan, 1 );

                            if ( coloraponerfondo == FALSE ) {
                                colorfondo = m_colorfondo2;
                                coloraponerfondo = TRUE;
                            } else {
                                colorfondo = m_colorfondo1;
                                coloraponerfondo = FALSE;
                            } // end if
                        } else {
                            /// El registro s&oacute;lo tiene una fila.
                            ponItemColorFondo ( mui_list, i - 1, 1, colorfondo );
                            if ( coloraponerfondo == FALSE ) {
                                colorfondo = m_colorfondo2;
                                coloraponerfondo = TRUE;
                            } else {
                                colorfondo = m_colorfondo1;
                                coloraponerfondo = FALSE;
                            } // end if
                        } // end if
                        m_filaInicialRowSpan = -1;
                    } // end if
                } // end if
            } // end if
        } // end for
    } // end for

    /// Si est&aacute; definido no aplicamos ninguna ordenaci&oacute;n.
    if ( !m_ordenporquery ) {
        if ( m_orden ) {
            /// Si estamos con campos de ordenacion ordenamos tras la carga el listado
            for ( int i = 0; i < m_lcabecera.size(); ++i ) {
                if ( m_lcabecera.at ( i ) ->nomcampo() == "orden" + m_tablename )
                    mui_list->sortItems ( i );
            } // end for
        } else {
            /// Si no estamos con campos de ordenaci&oacute;n ordenamos por lo que toca.
            /// Ordenamos la tabla.
            mui_list->ordenar();
        } // end if
    } // end if
    /// Generamos el registro de insercion.
    nuevoRegistro();

    ///  Cargamos la configuracion previamente almacenada.
    /// Configuramos que registros son visibles y que registros no lo son
    if ( m_primero ) {
        cargaconfig();
	/// Preparamos el menu de subformulario
	preparaMenu();
    } else {
        on_mui_confcol_clicked();
    } // end if

    /// Reactivamos el sorting
    mui_list->setSortingEnabled ( m_sorting );
    /// Borramos la barra de progreso
    delete barra;

    m_procesacambios = TRUE;

    blDebug ( "END BlSubForm::cargar", 0 );
}


///
/**
\param ordenactivado
**/
void BlSubForm::setOrdenPorQuery ( bool ordenactivado )
{
    blDebug ( "BlSubForm::setOrdenPorQuery", 0 );
    m_ordenporquery = ordenactivado;
    blDebug ( "END BlSubForm::setOrdenPorQuery", 0 );
}


///
/**
\param query La consulta SQL a cargar en el subformulario.
**/
/*
void BlSubForm::cargar ( QString query )
{
    blDebug ( "BlSubForm::cargar", 0 );
    /// Si el query no existe no hacemos nada.
    if ( query == "" ) {
        blDebug ( "BlSubForm::cargar", 0, "Query inexistente" );
        return;
    } // end if

    try {
        m_query = query;
        /// Tratramos con la paginacion.
        int limit = mui_filaspagina->text().toInt();
        if ( limit <= 0 ) {
            limit = 500;
        } // end if

        int pagact = mui_paginaact->text().toInt();
        if ( pagact <= 0 ) {
            pagact = 1;
        } // end if
        int offset = limit * ( pagact - 1 );

        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query, "", limit, offset );

        if (!cur) throw -1;

        cargar ( cur );
        delete cur;
    } catch ( ... ) {
        blDebug ( "BlSubForm::cargar", 2, "Error en la carga de datos" );
    } // end try
    blDebug ( "END BlSubForm::cargar", 0 );
}
*/


void BlSubForm::cargar ( QString query )
{
    blDebug ( "BlSubForm::cargar", 0 );
    /// Si el query no existe no hacemos nada.
    if ( query == "" ) {
        blDebug ( "BlSubForm::cargar", 0, "Query inexistente" );
        return;
    } // end if

    try {

        /// Guardar los valores necesarios para poder mantener la posici&oaucte;n en la lista tras realizar un cambio o borrado
        int active_col = -1; /// Columna activa antes de ejecutar la recarga
        int active_row = -1; /// Fila activa antes de ejecutar la recarga
        QString active_row_id; /// Identificador que había en dicha fila
        QString prev_active_row_id; /// Identificador que había en la fila anterior a la activa
        QString next_active_row_id; /// Identificador que había en la fila posterior a la activa

        /// Si la tabla no est&aacute; vac&iacute;a y tiene alguna fila activa, guardar los datos necesarios
        if ( rowCount() > 0 && currentRow() > -1 )
        {
            active_row = currentRow();
            active_col = currentColumn();

            /// Tomar el identificador de la fila anterior (si es que la hay)
            if ( active_row > 0 ) {
                prev_active_row_id = dbValue(dbFieldId(), active_row - 1);
            } // end if

            /// Tomar el identificador de la fila actual
            active_row_id = dbValue(dbFieldId());

            /// Tomar el identificador de la fila siguiente (si es que la hay)
            if ( active_row < rowCount() - 1 ) {
                next_active_row_id = dbValue(dbFieldId(), active_row + 1);
            } // end if
        } // end if
        /// Fin de Guardar los valores necesarios para poder mantener la posici&oaucte;n en la lista tras realizar un cambio o borrado

        m_query = query;
        /// Tratramos con la paginacion.
        int limit = mui_filaspagina->text().toInt();
        if ( limit <= 0 ) {
            limit = 500;
        } // end if

        int pagact = mui_paginaact->text().toInt();
        if ( pagact <= 0 ) {
            pagact = 1;
        } // end if
        int offset = limit * ( pagact - 1 );

        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query, "", limit, offset );

        if (!cur) throw -1;

        cargar ( cur );
        delete cur;

        /// Restaurar la posición anterior a la carga si es posible
        /// La condición "active_row <= rowCount()" sirve cuando un filtrado reduce la cantidad de filas
        if ( rowCount() > 0 && active_row > -1 && active_row <= rowCount() ) {

           int fila_futura = -1;

           /// Si la fila activa no era la &uacute;ltima,
           if ( !next_active_row_id.isEmpty() && active_row < rowCount() ) {

              /// y sigue siendo la misma (no ha sido borrada)...
              if ( ( !active_row_id.isEmpty() && dbValue ( dbFieldId(), active_row ) == active_row_id )

              /// o ha sido borrada pero hay otra despu&eacute;s que ha ocupado su lugar...
                 || ( !next_active_row_id.isEmpty() && dbValue ( dbFieldId(), active_row ) == next_active_row_id ) ) {

                 /// ... usar el mismo n&uacute;mero de fila
                 fila_futura = active_row;
              } // end if
          } // end if

           /// Si la fila era la &uacute;ltima, seguir en la &uacute;ltima, pase lo que pase
           else
           {
               fila_futura = rowCount() - 1;
           } // end if

           if ( fila_futura > -1 ) {
               mui_list->setCurrentCell ( fila_futura, active_col ) ;
               mui_list->scrollToItem ( mui_list->currentItem(), QAbstractItemView::PositionAtCenter ) ;
           } // end if

        } // end if
        /// Fin de Restaurar la posición anterior a la carga si es posible

    } catch ( ... ) {
        blDebug ( "BlSubForm::cargar", 2, "Error en la carga de datos" );
    } // end try
    blDebug ( "END BlSubForm::cargar", 0 );
}



/// Devuelve la linea que se esta tratando actualmente.
/**
\return
**/
BlDbSubFormRecord *BlSubForm::lineaact()
{
    blDebug ( "BlSubForm::lineaact", 0 );
    blDebug ( "END BlSubForm::lineaact", 0 );
    return lineaat ( mui_list->currentRow() );
}


/// Devuelve la linea especificada o NULL si esta no existe.
/**
\param row
\return
**/
BlDbSubFormRecord *BlSubForm::lineaat ( int row )
{
    blDebug ( "BlSubForm::lineaat()", 0, QString::number ( row ) );
    BlDbSubFormRecord *rec = NULL;
    try {
        /// Si la lista no tiene suficientes elementos devolvemos NULL
        if ( mui_list->rowCount() < row || row < 0 ) {
            throw - 1;
        } // end if

        /// Seleccionamos el campo especificado y lo devolvemos.
        BlDbSubFormField *camp = ( BlDbSubFormField* ) mui_list->item ( row, 0 );
        if ( !camp ) {
            throw - 1;
        } // end if
        rec = ( BlDbSubFormRecord * ) camp->pare();

    } catch ( ... ) {
        blDebug ( "BlSubForm::lineaat linea inexistente", 2, QString::number ( row ) );
        rec = NULL;
    }
    blDebug ( "END BlSubForm::lineaat()", 0 );
    return rec;
}


/// Devuelve TRUE si el registro ha sido completamente rellenado.
/**
\param row
\return
**/
bool BlSubForm::campoCompleto ( int row )
{
    blDebug ( "BlSubForm::campoCompleto", 0 );
    bool resultat = false;
    bool *pResultat = &resultat;
    if ( g_plugins->lanza ( "BlSubForm_campoCompleto", this, ( void** ) &pResultat ) ) {
        blDebug ( "END BlSubForm::campoCompleto", 0 , "plugin retorna " + *pResultat );
        return resultat;
    } else {
        BlDbSubFormField *camp;
        BlSubFormHeader *header;
        /// Sacamos celda a celda toda la fila
        for ( int i = 0; i < mui_list->columnCount(); i++ ) {
            camp = ( BlDbSubFormField * ) mui_list->item ( row, i );

            /// Si el dato no es valido se sale
            if ( !camp ) return FALSE;

            header = m_lcabecera.at ( i );
            if ( camp->restrictcampo() & BlDbField::DbNotNull
                    && camp->text() == ""
                    && ! ( header->options() & BlSubFormHeader::DbHideView )
                    && camp->dbFieldType() != BlDbField::DbBoolean ) {
                blDebug ( "END BlSubForm::campoCompleto", 0, "El campo no es completo." );
                return FALSE;
            } // end if
            if ( camp->restrictcampo() & BlDbField::DbRequired
                    && camp->text() == "" ) {
                blDebug ( "END BlSubForm::campoCompleto", 0, "El campo no es completo." );
                return FALSE;
            } // end if

        } // end for
        blDebug ( "END BlSubForm::campoCompleto", 0 );
    }
    return TRUE;
}


/// M&eacute;todo que se dispara cuando se va a reposicionar en el formulario.
/// Se encarga de resituar el cursor al lugar que se haya indicado.
/// Con el fin de evitar problemas de reposicionado le activamos un semaforo y bloqueamos signals.
/**
\param row
\param col
**/
void BlSubForm::on_mui_list_cellRePosition ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_cellReposition", 0, "Row: " + QString::number ( row ) + " col: " + QString::number ( col ) );

    /// Implementacion del semaforo
    static bool semaforo = FALSE;
    if ( semaforo )
        return;
    semaforo = TRUE;

    /// Bloqueamos la emision de signals
    mui_list->blockSignals(TRUE);


    bool creado = FALSE;

    int key = mui_list->m_teclasalida;

    /// Miramos si hemos completado la linea y si es asi creamos una linea nueva
    if ( m_insercion && row == mui_list->rowCount() - 1 && campoCompleto ( row ) ) {
        nuevoRegistro();
        creado = TRUE;
    } // end if

    switch ( key ) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if ( !m_insercion ) {
            /// Se ha hecho un enter sobre una tabla sin insercion con lo que lanzamos un doble click para que sea
            /// La accion simulada.
            QTableWidgetItem * item = mui_list->currentItem();
            /// Desbloqueamos la emision de signals
            mui_list->blockSignals(FALSE);
            /// Emitimos las signals de dobleclick
            emit itemDoubleClicked ( item );
            emit cellDoubleClicked ( row, col );
        } else {
            situarse ( row, col );
        } // end if
        break;
    case Qt::Key_Backtab:
	    situarse ( row, col, true );
	break;
    case Qt::Key_Tab:
	    situarse ( row, col );
	break;
    case Qt::Key_Down:
        situarse ( row, col );
        situarse1 ( row, col );
        if ( creado ) {
            mui_list->setCurrentCell ( row + 1, col );
        } // end if
        break;
    } // end switch

    semaforo = FALSE;

    /// Desbloqueamos la emision de signals
    mui_list->blockSignals(FALSE);
    blDebug ( "END BlSubForm::on_mui_list_cellRePosition", 0 );
}


/// M&eacute;todo que se dispara cuando se termina de editar un campo del Subformulario.
/**
\param row
\param col
**/
void BlSubForm::on_mui_list_cellChanged ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_cellChanged", 0, "Row: " + QString::number ( row ) + " col: " + QString::number ( col ) );

    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        blDebug ( "END BlSubForm::on_mui_list_cellChanged", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
        return;
    } // end if

    BlDbSubFormField *camp = ( BlDbSubFormField * ) item ( row, col );
    camp->refresh();

    /// Si el campo no ha sido cambiado se sale.
    if ( ! camp->cambiado() ) {
        blDebug ( "END BlSubForm::on_mui_list_cellChanged", 0, QString::number ( row ) + " " + QString::number ( col ) + camp->valorcampo() + "Sin cambios" );
        return;
    } // end if

    /// En el caso de tener una fecha directamente la tratamos ya que esta claro cual es su tratamiento.
    if (camp->dbFieldType() == BlDbField::DbDate) {
        camp->set ( blNormalizeDate ( camp->valorcampo() ).toString ( "dd/MM/yyyy" ) );
    } // end if


    if ( m_procesacambios ) {
        m_procesacambios = FALSE;
        m_prevRow = row;
        m_prevCol = col;
        editFinished ( row, col, rec, camp );
        emit editFinish ( row, col );
        m_procesacambios = TRUE;
    } // end if
    blDebug ( "END BlSubForm::on_mui_list_cellChanged", 0 );
}


/// M&eacute;todo para agregar cabeceras al listado
/** Cada columna de un subformulario tiene una instancia de la clase BlSubFormHeader. Este m&eacute;todo
    se encarga de crear e inicializar cada una de dichas clases. */
/**
\param nom
\param typ
\param res
\param opt
\param nomp
\return
**/
int BlSubForm::addSubFormHeader ( QString nom, BlDbField::DbType typ, int res, int opt, QString nomp )
{
    blDebug ( "BlSubForm::addSubFormHeader", 0,  nom );

    BlSubFormHeader *camp = new BlSubFormHeader ( nom, typ, res, opt, nomp );
    m_lcabecera.append ( camp );
    mui_listcolumnas->insertRow ( mui_listcolumnas->rowCount() );

    BlTableWidgetItem *it = new BlTableWidgetItem ( nomp );
    it->setFlags ( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );

    if ( opt & BlSubFormHeader::DbHideView ) {
        mui_list->hideColumn ( mui_listcolumnas->rowCount() - 1 );
        it->setCheckState ( Qt::Unchecked );
    } else {
        it->setCheckState ( Qt::Checked );
    } // end if

    if ( opt & BlSubFormHeader::DbDisableView ) {
        it->setFlags ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
    } // end if




    /// Comprobamos la precision del campo (si este es del tipo numerico) y la guardamos.
    /// Lo hacemos aqui y no en el constructor de BlDbField porque precisamos del nombre de la tabla para
    /// poder hacer la consulta sobre la precisión del campo y ese dato solo esta en BlDbRecord.
    /// Buscamos los decimales que tiene el campo y establecemos el numero de decimales a ese valor.

    /// NOTA: En caso de que no haya sido posible calcular la precisión esta se establece por defecto como 2.

    if (typ == BlDbField::DbNumeric ) {
      if ( tableName() != "" && mainCompany() != NULL ) {
          QString query2 = "SELECT numeric_scale FROM information_schema.columns WHERE table_name = '"+tableName()+"' and column_name='"+nom+"';";
          BlDbRecordSet *cur = mainCompany() ->loadQuery ( query2 );
          if ( !cur->eof() ) {
              camp->setNumericPrecision(cur->valor("numeric_scale").toInt());
          } // end if
          delete cur;
      } else {
              camp->setNumericPrecision(2);
      } // end if
    } // end if





    mui_listcolumnas->setItem ( mui_listcolumnas->rowCount() - 1, 0, it );
    it = new BlTableWidgetItem ( nom );
    mui_listcolumnas->setItem ( mui_listcolumnas->rowCount() - 1, 1, it );
    it = new BlTableWidgetItem ( "" );
    mui_listcolumnas->setItem ( mui_listcolumnas->rowCount() - 1, 2, it );
    /// Escribe el order de las columnas.
    it = new BlTableWidgetItem ( QString::number ( mui_listcolumnas->rowCount() - 1 ) );
    mui_listcolumnas->setItem ( mui_listcolumnas->rowCount() - 1, 3, it );

    blDebug ( "END BlSubForm::addSubFormHeader", 0, nom );
    return 0;
}


/** Establece el valor de toda la columna.
*/
/**
\param campo
\param valor
**/
void BlSubForm::setColumnValue ( QString campo, QString valor )
{
    blDebug ( "BlSubForm::setColumnValue", 0, campo + " -- " + valor );
    BlDbSubFormRecord *rec;

    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        rec = lineaat ( i );
        if ( rec ) {
            rec->setDbValue ( campo, valor );
        } // end if
    } // end for
    blDebug ( "END BlSubForm::setColumnValue", 0 );
}


///
/**
\param campo
\return
**/
BlFixed BlSubForm::sumarCampo ( QString campo )
{
    blDebug ( "BlSubForm::sumarCampo", 0 );
    BlFixed total;
    BlDbSubFormRecord *rec;
    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        rec =  lineaat ( i );
        if ( rec ) {
            BlFixed subtotal = BlFixed ( rec->dbValue ( campo ) );
            total = total + subtotal;
        } // end if
    } // end for
    blDebug ( "END BlSubForm::sumarCampo", 0 );
    return total;
}


///
/**
\param campo
\param row
**/
QString BlSubForm::dbValue ( const QString &campo, int row )
{
    blDebug ( "BlSubForm::dbValue", 0 );
    try {
        BlDbSubFormRecord *rec;
        if ( row == -1 )
            rec = lineaact();
        else
            rec = lineaat ( row );
        if ( rec == NULL )
            throw - 1;
        blDebug ( "END BlSubForm::dbValue", 0 );
        return rec->dbValue ( campo );
    } catch ( ... ) {
        blMsgInfo ( _ ("Fila inexistente" ));
        throw - 1;
    }
}


///
/**
\param campo Nombre de la columna que debe cambiarse.
\param row   Fila correspondiente a la casilla
\param valor Valor que tomara la casilla
**/
void BlSubForm::setDbValue ( const QString &campo, int row, const QString &valor )
{
    blDebug ( "BlSubForm::setDbValue", 0 );
    try {
        BlDbSubFormRecord *rec;
        if ( row == -1 )
            rec = lineaact();
        else
            rec = lineaat ( row );
        if ( rec == NULL )
            throw - 1;
        rec->setDbValue ( campo, valor );
        blDebug ( "END BlSubForm::setDbValue", 0 );
    } catch ( ... ) {
        blMsgInfo ( _ ("Fila inexistente" ) );
        throw - 1;
    }
}


///
/**
\return
**/
int BlSubForm::guardar()
{
    blDebug ( "BlSubForm::guardar", 0 );
    try {
        BlDbSubFormRecord *rec;
        /// Borramos los elementos marcados para ser borrados.
        while ( !m_listaborrar.isEmpty() ) {
            rec = m_listaborrar.takeFirst();
            if ( rec ) {
                rec->borrar();
            } // end if
        } // end while

        /// Asegura que siempre la ultima linea se valide antes de guardar.
        /// Esto evita que se pueda perder informacion.
        if ( campoCompleto ( mui_list->rowCount() - 1 ) ) {
            nuevoRegistro();
        } // end if

        /// Si no hay elementos que guardar salimos.
        if ( mui_list->rowCount() == 0 || ( ( mui_list->rowCount() == 1 ) && m_insercion ) ) {
            return 0;
        } // end if

        /// Hacemos el guardado
        for ( int j = 0; j < mui_list->rowCount() - 1; ++j ) {
            rec = lineaat ( j );
            if ( rec ) {
                /// Si hay ordenacion de campos ahora la establecemos
                if ( m_orden ) {
                    rec->setDbValue ( "orden" + m_tablename, QString::number ( j ) );
                } // end if
                rec->refresh();
                rec->guardar();
            } // end if
        } // end for

        /// Si no hay modo insercion hacemos el guardado de la ultima linea.
        if ( !m_insercion ) {
            rec = lineaat ( mui_list->rowCount() - 1 );
            if ( m_orden )
                rec->setDbValue ( "orden" + m_tablename, QString::number ( mui_list->rowCount() - 1 ) );
            rec->refresh();
            rec->guardar();
        } // end if

        /// Liberamos memoria
        while ( !m_listaborrar.isEmpty() ) {
            rec = m_listaborrar.takeFirst();
            if ( rec ) {
                delete rec;
            } // end if
        } // end while

        blDebug ( "END BlSubForm::guardar", 0 );
        return 0;
    } catch ( int e ) {
        if ( e == 100 ) {
            throw - 1;
        } // end if
    } catch ( ... ) {
        blMsgError ( _ ("Error inesperado en el guardado. [BlSubForm::guardar]" ) );
        throw - 1;
    } // end try
    return -1;
}


int BlSubForm::borrarLineaActual() {
      blDebug ( "BlSubForm::borrarLineaActual", 0 );
      blDebug ( "BlSubForm::borrarLineaActual", 0 );
      return 	borrar(currentRow());
}

///
/**
\return
**/
int BlSubForm::borrar()
{
    blDebug ( "BlSubForm::borrar", 0 );
    BlDbSubFormRecord *rec;
    int i = 0;
    int error = 0;

    /// Si la lista esta vacia no podemos borrar.
    if ( m_lista.count() <= 0 ) {
        blDebug ( "END BlSubForm::borrar", 0 );
        return error;
    } // end if

    try {
        for ( rec = m_lista.at ( i++ ); i < m_lista.count(); rec = m_lista.at ( i++ ) ) {
            error = rec->borrar();
            if ( error )
                return -1;
        } // end for
        if ( !m_insercion ) {
            rec = m_lista.at ( m_lista.count() - 1 );
            error = rec->borrar();
        } // end if
        blDebug ( "END BlSubForm::borrar", 0 );
        return error;
    } catch ( ... ) {
        blMsgError ( _ ("Error al borrar. [BlSubForm::borrar]" ));
        return -1;
    } // end try
}


///
/**
\param row
\return
**/
int BlSubForm::borrar ( int row )
{
    blDebug ( "BlSubForm::borrar", 0 );
    try {
        BlDbSubFormRecord *rec, *rac;
        BlDbSubFormField *camp;

        rac = new BlDbSubFormRecord ( mainCompany() );

        /// Cogemos el elemento correspondiente, partimos de mui_list, tb podriamos usar lineaat
        rec = lineaat ( row );
        if ( !rec )
            return -1;

        /// Agregamos el elemento a la lista de borrados
        m_listaborrar.append ( rac );
        m_lista.takeAt ( m_lista.indexOf ( rec ) );

        rac->setDbTableName ( rec->tableName() );
        rac->setDbFieldId ( rec->fieldId() );
        rac->setNuevo ( FALSE );

        /// Sacamos celda a celda toda la fila
        for ( int i = 0; i < mui_list->columnCount(); i++ ) {
            camp = ( BlDbSubFormField * ) mui_list->item ( row, i );
            BlDbSubFormField *it = new BlDbSubFormField ( rac, mainCompany(), camp->nomcampo(), camp->dbFieldType(), camp->restrictcampo(), camp->nompresentacion() );
            rac->lista() ->append ( it );
            it->set ( camp->valorcampo() );
        } // end for

        /// Nos aseguramos que ningun campo de la fila a borrar este en modo edicion.
        /// Evitamos que falle el programa.
        cerrarEditor();

        mui_list->removeRow ( row );
        delete rec;

        /// Comprueba que no haya ninguna linea en el subformulario y crea una en blanco.
        if ( m_insercion == TRUE && rowCount() == 0 ) {
            nuevoRegistro();
        } // end if

        /// Terminamos
        blDebug ( "END BlSubForm::borrar", 0 );
        return 0;

    } catch ( ... ) {
        blMsgInfo ( _ ("Error al intentar borrar" ) );
        blDebug ( "BlSubForm::borrar error al borrar", 3 );
        throw - 1;
    } // end try
}


/// Metodo para ser derivado.
/**
\return
**/
int BlSubForm::cerrarEditor()
{
    blDebug ( "END BlSubForm::cerrarEditor", 0 );
    return 0;
}


/// Sacamos cual es el archivo en el que guardar/cargar configuraciones
const QString BlSubForm::nameFileConfig() 
{
   QString directorio = g_confpr->valor(CONF_DIR_USER);
   if (g_confpr->valor(CONF_GLOBAL_CONFIG_USER) == "TRUE") {
      directorio = g_confpr->valor(CONF_DIR_CONFIG);
   } // end if

   QString empresa = mainCompany()->dbName();
   if (g_confpr->valor(CONF_GLOBAL_CONFIG_COMPANY) == "TRUE") {
      empresa  = "";
   } // end if

  QString nombre = directorio + m_fileconfig + "_" + empresa + "_" + QString::number ( m_modo ) + "_tablecfn.cfn" ;
  return nombre;
}




/// Guardamos el archivo de configuracion.
/**
**/
void BlSubForm::guardaconfig()
{
    blDebug ( "BlSubForm::guardaconfig", 0 );

    /// Si el subformulario no esta inicializado no hacemos el guardado.
    if ( ! mainCompany() )
        return;

    QString aux = "";
    QFile file ( nameFileConfig() );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << mui_list->colorden() << "\n";
        stream << mui_list->tipoorden() << "\n";
        stream << mui_filaspagina->text() << "\n";

        /// Guarda la visibilidad de los elementos consultando la tabla de configuracion.
        for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i ) {
            for ( int j = 0; j < mui_listcolumnas->rowCount(); ++j ) {
                if ( mui_listcolumnas->item ( j, 3 )->text().toInt() == i ) {
                    if ( mui_listcolumnas->item ( j, 0 ) ->checkState() == Qt::Checked ) {
                        stream << "1" << "\n";
                    } else {
                        stream << "0" << "\n";
                    } // end if
                } // end if
            } // end for
        } // end for

        /// Guarda la configuracion de mui_list.
        stream << QString ( mui_list->horizontalHeader()->saveState().toBase64() ) << "\n";
        
	/// 12/Junio/2009:
        /// TODO: Se comentan las lineas que guardan el estado vertical porque da fallos en
        /// algunas circunstancias y no tiene en cuenta el registro (BD) concreto al que afecta la
        /// configuracion.
        
        //stream << QString ( mui_list->verticalHeader()->saveState().toBase64() ) << "\n";

        /// Guarda la configuracion de mui_listcolumnas.
        stream << QString ( mui_listcolumnas->horizontalHeader()->saveState().toBase64() ) << "\n";
        //stream << QString ( mui_listcolumnas->verticalHeader()->saveState().toBase64() ) << "\n";

        /// Guarda el orden de la lista de elementos en mui_listcolumnas.
        for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i ) {
            stream << mui_listcolumnas->item ( i, 3 )->text() << "\n";
        } // end for

	/// Guardamos el estado del menu de subformulario.
	if (mui_menusubform->isHidden()) {
	  stream << "0";
	} else {
	  stream << "1";
	} // end if


        file.close();
    } // end if
    blDebug ( "END BlSubForm::guardaconfig", 0 );
}


///
/**
**/
void BlSubForm::cargaconfig()
{
    blDebug ( "BlSubForm::cargaconfig", 0 );
    QFile file ( nameFileConfig() );
    QString line;
    int error = 1;
    if ( file.open ( QIODevice::ReadOnly ) ) {

        error = 0;
        QTextStream stream ( &file );
        /// Establecemos la columna de ordenaci&oacute;n
        QString linea = stream.readLine();
        mui_list->setcolorden ( linea.toInt() );

        /// Establecemos el tipo de ordenaci&oacute;n
        linea = stream.readLine();
        mui_list->settipoorden ( linea.toInt() );

        /// Establecemos el n&uacute;mero de filas por p&aacute;gina
        linea = stream.readLine();
        if ( linea.toInt() > 0 ) {
            mui_filaspagina->setValue ( linea.toInt() );
        } // end if

        /// Leemos la visibilidad de las columnas. Se hace antes de ordenarlas.
        for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i ) {
            linea = stream.readLine();
            if ( linea == "1" ) {
                mui_listcolumnas->item ( i, 0 ) ->setCheckState ( Qt::Checked );
            } else if ( linea == "0" ) {
                mui_listcolumnas->item ( i, 0 ) ->setCheckState ( Qt::Unchecked );
            } else {
                error = 1;
            } // end if
        } // end for

        /// Restaura el estado de mui_list.
        linea = stream.readLine();
        mui_list->horizontalHeader()->restoreState ( QByteArray::fromBase64 ( linea.toAscii() ) );
        //linea = stream.readLine();
        //mui_list->verticalHeader()->restoreState ( QByteArray::fromBase64 ( linea.toAscii() ) );

        /// Restaura el estado de mui_listcolumnas.
        linea = stream.readLine();
        mui_listcolumnas->horizontalHeader()->restoreState ( QByteArray::fromBase64 ( linea.toAscii() ) );
        //linea = stream.readLine();
        //mui_listcolumnas->verticalHeader()->restoreState ( QByteArray::fromBase64 ( linea.toAscii() ) );

        /// Restaura el orden de mui_listcolumnas.
        for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i ) {
            linea = stream.readLine();
            /// Busca en la 4a columna la fila que tenga el valor igual a 'linea.toInt()'. Que fila es?
            /// ese sera el valor de rowOld. i = rowNew.
            for ( int j = 0; j < mui_listcolumnas->rowCount(); ++j ) {
                if ( mui_listcolumnas->item ( j, 3 )->text().toInt() == linea.toInt() ) {
                    mui_listcolumnas->moveRow ( j, i );
                } // end if
            } // end for
        } // end for


	/// Guardamos el estado del menu de subformulario.
	linea = stream.readLine();
	if (linea == "1") {
	  mui_menusubform->setVisible(TRUE);
	} else {
	  mui_menusubform->setVisible(FALSE);
	} // end if

        file.close();
        on_mui_confcol_clicked();
    } // end if

    /// Si se ha producido alg&uacute;n error en la carga hacemos un maquetado autom&aacute;tico.
    if ( error )
        mui_list->resizeColumnsToContents();

    m_primero = FALSE;
    blDebug ( "END BlSubForm::cargaconfig", 0 );
}


///
/**
**/
void BlSubForm::on_mui_confcol_clicked()
{
    blDebug ( "BlSubForm::on_mui_confcol_clicked", 0 );
    for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i ) {
        if ( mui_listcolumnas->item ( i, 0 ) ->checkState() == Qt::Checked ) {
            mui_list->showColumn ( mui_listcolumnas->item ( i, 3 )->text().toInt() );
        } else {
            /// Coge el valor de la columna de 'order' para ocultarla.
            mui_list->hideColumn ( mui_listcolumnas->item ( i, 3 )->text().toInt() );
        } // end if
    } // end for
    blDebug ( "END BlSubForm::on_mui_confcol_clicked", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_pressedSlash ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_pressedSlash", 0 );
    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        blDebug ( "END BlSubForm::on_mui_list_pressedSlash", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
        return;
    } // end if

    BlDbSubFormField *camp = ( BlDbSubFormField * ) item ( row, col );
    camp->refresh();

    if ( m_procesacambios ) {
        m_procesacambios = FALSE;
        m_prevRow = row;
        m_prevCol = col;
        pressedSlash ( row, col, rec, camp );
        emit pushSlash ( row, col );
        m_procesacambios = TRUE;
        /// Invocamos la finalizacion de edicion para que todos los campos se actualicen.
        on_mui_list_cellChanged ( row, col );
    } // end if
    blDebug ( "END BlSubForm::on_mui_list_pressedSlash", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_pressedAsterisk ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_pressedAsterisk", 0 );
    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        blDebug ( "END BlSubForm::on_mui_list_pressedAsterisk", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
        return;
    } // end if

    BlDbSubFormField *camp = ( BlDbSubFormField * ) item ( row, col );
    camp->refresh();

    if ( m_procesacambios ) {
        m_procesacambios = FALSE;
        m_prevRow = row;
        m_prevCol = col;
        pressedAsterisk ( row, col, rec, camp );
        emit pushAsterisk ( row, col );
        m_procesacambios = TRUE;
        /// Invocamos la finalizacion de edicion para que todos los campos se actualicen.
        on_mui_list_cellChanged ( row, col );
    } // end if
    blDebug ( "END BlSubForm::on_mui_list_pressedAsterisk", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_pressedPlus ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_pressedPlus", 0 );
    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        blDebug ( "END BlSubForm::on_mui_list_pressedPlus", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
        return;
    } // end if

    BlDbSubFormField *camp = ( BlDbSubFormField * ) item ( row, col );
    camp->refresh();

    if ( m_procesacambios ) {
        m_procesacambios = FALSE;
        m_prevRow = row;
        m_prevCol = col;
        pressedPlus ( row, col, rec, camp );
        emit pushPlus ( row, col );
        m_procesacambios = TRUE;
        /// Invocamos la finalizacion de edicion para que todos los campos se actualicen.
        on_mui_list_cellChanged ( row, col );
    } // end if
    blDebug ( "END BlSubForm::on_mui_list_pressedPlus", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_pressedMinus ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_pressedMinus", 0 );
    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        blDebug ( "END BlSubForm::on_mui_list_pressedMinus", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
        return;
    } // end if

    BlDbSubFormField *camp = ( BlDbSubFormField * ) item ( row, col );
    camp->refresh();

    if ( m_procesacambios ) {
        m_procesacambios = FALSE;
        m_prevRow = row;
        m_prevCol = col;
        pressedMinus ( row, col, rec, camp );
        emit pushMinus ( row, col );
        m_procesacambios = TRUE;
        /// Invocamos la finalizacion de edicion para que todos los campos se actualicen.
        on_mui_list_cellChanged ( row, col );
    } // end if
    blDebug ( "END BlSubForm::on_mui_list_pressedMinus", 0 );
}


///
/**
\return
**/
QString BlSubForm::imprimir()
{
    blDebug ( "BlSubForm::imprimir", 0 );
    BlProgressBar barra;
    barra.show();
    barra.setRange ( 0, mui_listcolumnas->rowCount() + mui_list->columnCount() - 2 );
    barra.setText ( _ ( "Imprimiendo " ) +  m_tablename );
    barra.setValue ( 0 );
    QLocale::setDefault ( QLocale ( QLocale::Spanish, QLocale::Spain ) );
    QLocale spanish;

    QString fitxersortidarml = "<tr>\n";
    for ( int h = 0; h < mui_listcolumnas->rowCount(); ++h ) {
        if ( mui_listcolumnas->item ( h, 0 ) ->checkState() == Qt::Checked ) {
            fitxersortidarml += "    <td>" +  blXMLEncode( mui_listcolumnas->item ( h, 0 ) ->text() ) + "</td>\n";
        } // end if
        barra.setValue ( barra.value() + 1 );
    } // end for
    fitxersortidarml += "</tr>\n";
    
    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        fitxersortidarml += "<tr>\n";
        for ( int j = 0; j < mui_listcolumnas->rowCount(); ++j ) {
            if ( mui_listcolumnas->item ( j, 0 )->checkState() == Qt::Checked ) {
                BlDbSubFormField *valor = ( BlDbSubFormField * ) mui_list->item ( i, mui_listcolumnas->item ( j, 3)->text().toInt() );
                fitxersortidarml += formatFieldTableStory ( valor, spanish );
            } // end if
        } // end for
        fitxersortidarml += "</tr>\n";
        barra.setValue ( barra.value() + 1 );
    } // end for
    blDebug ( "END BlSubForm::imprimir", 0 );
    return fitxersortidarml;
}


/// Genera las etiquetas RML necesarias para poder mostrar un dato con el estilo adecuado a su tipo
/// a la hora de imprimirlo como una celda del "BlockTable" ubicado en el "story" del informe "listado"
/**
  \param value
  \param spanish
\return
**/
QString BlSubForm::formatFieldTableStory(BlDbSubFormField *value, QLocale spanish)
{
    blDebug ( "BlSubForm::formatFieldTableStory", 0 );

    QString field = "    ";

    /// Valor num&eacute;rico o fecha
    if ( ( value->dbFieldType() == BlDbField::DbInt )
      || ( value->dbFieldType() == BlDbField::DbNumeric )
      || ( value->dbFieldType() == BlDbField::DbDate ) )
    {
        field += "<td><para style=\"number\">";

        /// Si es un n&uacute;mero con decimales, tenerlo en cuenta
        if ( value->dbFieldType() & BlDbField::DbNumeric )
        {
            int prec = value->text().section(",", 1).count();
            field += spanish.toString ( value->text().toDouble(), 'f', prec );
        }
        else
            field += blXMLEncode ( value->text() );
    }

    /// Valor booleano
    else if ( value->dbFieldType() == BlDbField::DbBoolean )
    {
        /// Mostrar un "puntazo" centrado si el valor es verdadero
        if ( value->checkState() == Qt::Checked )
        {
           field += "<td><para style=\"checked\" vAlign=\"middle\">";
           field += blXMLEncode ( "&#9679;" );
        }
        else
           field += "<td><para>";
    }

    /// Texto
    else
    {
        field += "<td><para style=\"text\">";
        field += blXMLEncode ( value->text() );
    } // end if

    /// Cerrar p&aacute;rrafo y campo de la tabla para todos los casos
    field += "</para></td>\n";

    blDebug ( "END BlSubForm::formatFieldTableStory", 0 );
    return field;
}


///
/**
\return
**/
void BlSubForm::on_mui_confquery_clicked()
{
    blDebug ( "BlSubForm::on_mui_confquery_clicked ", 0 );
    if ( mainCompany() == NULL ) {
        blMsgInfo ( _ ("no se ha inicializado bien la clase" ) );
        return;
    } // end if
    mui_paginaact->setValue ( 1 );
    cargar ( mui_query->toPlainText() );
//  cargar(m_query);
    blDebug ( "END BlSubForm::on_mui_confquery_clicked ", 0 );
}


///
/**
\return
**/
void BlSubForm::confquery()
{
    blDebug ( "BlSubForm::confquery", 0 );
    if ( mainCompany() == NULL ) {
        blMsgInfo ( _ ("no se ha inicializado bien la clase" ) );
        return;
    } // end if

    cargar ( m_query );

    blDebug ( "END BlSubForm::confquery ", 0 );
}


/// Disparador que se activa al haber pulsado ctrl+Arriba en la tabla
/// Hace el intercambio con la fila inmediatamente superior.
/**
\param row
\param col
\return
**/
void BlSubForm::on_mui_list_ctrlSubir ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_ctrlSubir", 0, " (" + QString::number ( row ) + "," + QString::number ( col ) + ")" );
    if ( ! m_orden )
        return;
    if ( row >= mui_list->rowCount() - 1 || row == 0 )
        return;


    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled ( FALSE );
    mui_list->setCurrentCell ( 0, 0 );

    m_procesacambios = FALSE;

    for ( int i = 0; i < mui_list->columnCount(); ++i ) {
        QTableWidgetItem *it = mui_list->takeItem ( row, i );
        QTableWidgetItem *ot = mui_list->takeItem ( row - 1, i );
        mui_list->setItem ( row, i, ot );
        mui_list->setItem ( row - 1, i, it );
    } // end for
    mui_list->setCurrentCell ( row - 1, col );
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled ( m_sorting );

    m_procesacambios = TRUE;
    blDebug ( "END BlSubForm::on_mui_list_ctrlSubir", 0 );
}


/// Disparador que se activa al haber pulsado ctrl+Abajo en la tabla
/// Hace el intercambio con la fila inmediatamente inferior.
/**
\param row
\param col
\return
**/
void BlSubForm::on_mui_list_ctrlBajar ( int row, int col )
{
    blDebug ( "BlSubForm::on_mui_list_ctrlBajar", 0 );
    if ( ! m_orden )
        return;
    if ( row >= mui_list->rowCount() - 2 )
        return;
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled ( FALSE );
    m_procesacambios = FALSE;

    if ( row != 0 )
        mui_list->setCurrentCell ( 0, 0 );
    else
        mui_list->setCurrentCell ( mui_list->rowCount() - 1, 0 );

    for ( int i = 0; i < mui_list->columnCount(); ++i ) {
        QTableWidgetItem *it = mui_list->takeItem ( row, i );
        mui_list->setItem ( row, i, mui_list->takeItem ( row + 1, i ) );
        mui_list->setItem ( row + 1, i, it );
    } // end for
    mui_list->setCurrentCell ( row + 1, col );
    /// Desactivamos el sorting debido a un error en las Qt4
    mui_list->setSortingEnabled ( m_sorting );
    m_procesacambios = TRUE;

    blDebug ( "END BlSubForm::on_mui_list_ctrlBajar", 0 );
}


///
/**
**/
void BlSubForm::on_mui_pagsiguiente_clicked()
{
    blDebug ( "BlSubForm::on_mui_pagsiguiente_clicked", 0 );
    int pag = mui_paginaact->text().toInt();
    pag++;
    mui_paginaact->setValue ( pag );
    confquery();
    blDebug ( "END BlSubForm::on_mui_pagsiguiente_clicked", 0 );
}

///
/**
**/
void BlSubForm::on_mui_paganterior_clicked()
{
    blDebug ( "BlSubForm::on_mui_paganterior_clicked", 0 );
    int pag = mui_paginaact->text().toInt();
    if ( pag > 1 )
        pag--;
    mui_paginaact->setValue ( pag );
    confquery();
    blDebug ( "END BlSubForm::on_mui_paganterior_clicked", 0 );
}


///
/**
\param titular
**/
void BlSubForm::imprimirPDF ( const QString &titular )
{
    blDebug ( "BlSubForm::imprimir", 0 );

    /// Los listados siempre usan la misma plantilla para imprimir listado.
    QString archivo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "listado.rml";
//    QString archivo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + titular + ".rml";
    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + "listado.rml";
//    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + titular + ".rml";
    QString archivologo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WIN32

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    int result1 = system ( archivo.toAscii() );
    if (result1 == -1) {
	blMsgError(_("Errr al copiar listado.rml [ BlSubForm->imprimirPDF() ]"));
    } // end if
    
    /// Copiamos el logo.
#ifdef WIN32

    archivologo = "copy " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    int result2 = system ( archivologo.toAscii() );
    if (result2 == -1) {
	blMsgError(_("Errr al copiar listado.rml [ BlSubForm->imprimirPDF() ]"));
    } // end if

    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );

    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;

    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += imprimir();
    fitxersortidatxt += "</blockTable>";

    /// Reemplazamos en la impresion el parametro [story] por el resultado de la impresion
    buff.replace ( "[story]", fitxersortidatxt );
    /// La plantilla de listados admite unicamente el parametro titular que es lo que cambia
    /// en todos los listados
    buff.replace ( "[titulo]", titular );

    if ( file.open ( QIODevice::WriteOnly ) ) {

        QTextStream stream ( &file );
        stream << buff;

        file.close();
    } // end if

    blCreateAndLoadPDF ( "listado" );
    blDebug ( "END BlSubForm::imprimir", 0 );
}

void BlSubForm::preparaMenu() {
    blDebug ( "BlSubForm::preparaMenu", 0 );

    /// Disparamos los plugins.
    g_plugins->lanza ( "BlSubForm_preparaMenu", this );

    QHBoxLayout *m_hboxLayout1 = mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( mui_menusubform );
        m_hboxLayout1->setSpacing (0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    
    if ( m_delete ) {
      QToolButton *sel = new QToolButton ( mui_menusubform );
      sel->setStatusTip ( "Generar Q19" );
      sel->setToolTip ( "Borrar Linea" );
      sel->setMinimumSize ( QSize ( 18, 18 ) );
      sel->setIcon ( QIcon ( ":/Images/delete-one-line.png" ) );
      sel->setIconSize ( QSize ( 18, 18 ) );    
      m_hboxLayout1->addWidget ( sel );
      connect (sel, SIGNAL(released()), this, SLOT(borrarLineaActual()));
    } // end if
    
    /// Ajuste de la columna
    QToolButton *sel1 = new QToolButton ( mui_menusubform );
    sel1->setStatusTip ( "Ajustar Columnas" );
    sel1->setToolTip ( "Ajustar Columnas" );
    sel1->setMinimumSize ( QSize ( 18, 18 ) );
    sel1->setIcon ( QIcon ( ":/Images/adjust-all-rows.png" ) );
    sel1->setIconSize ( QSize ( 18, 18 ) );    
    m_hboxLayout1->addWidget ( sel1 );    
    connect (sel1, SIGNAL(released()), this, SLOT(resizeColumnsToContents()));    

    /// Ajuste de la altura
    QToolButton *sel2 = new QToolButton ( mui_menusubform );
    sel2->setStatusTip ( "Ajustar Filas" );
    sel2->setToolTip ( "Ajustar Filas" );
    sel2->setMinimumSize ( QSize ( 18, 18 ) );
    sel2->setIcon ( QIcon ( ":/Images/adjust-one-line.png" ) );
    sel2->setIconSize ( QSize ( 18, 18 ) );    
    m_hboxLayout1->addWidget ( sel2 );    
    connect (sel2, SIGNAL(released()), this, SLOT(resizeRowsToContents()));   
    
    /// Ver configuracion del subformulario
    QToolButton *sel3 = new QToolButton ( mui_menusubform );
    sel3->setStatusTip ( "Ver/Ocultar configurador de subformulario" );
    sel3->setToolTip ( "Ver/Ocultar configurador de subformulario" );
    sel3->setMinimumSize ( QSize ( 18, 18 ) );
    sel3->setIcon ( QIcon ( ":/Images/toggleconfig.png" ) );
    sel3->setIconSize ( QSize ( 18, 18 ) );    
    m_hboxLayout1->addWidget ( sel3 );    
    connect (sel3, SIGNAL(released()), this, SLOT(toogleConfig()));  
    
    /// Ocultar Menu
    QToolButton *sel4 = new QToolButton ( mui_menusubform );
    sel4->setStatusTip ( "Ver/Ocultar menu de subformulario" );
    sel4->setToolTip ( "Ver/Ocultar menu de subformulario" );
    sel4->setMinimumSize ( QSize ( 18, 18 ) );
    sel4->setIcon ( QIcon ( ":/Images/togglemenu.png" ) );
    sel4->setIconSize ( QSize ( 18, 18 ) );    
    m_hboxLayout1->addWidget ( sel4 );    
    connect (sel4, SIGNAL(released()), this, SLOT(toogleMenuConfig())); 
    
    blDebug ( "END BlSubForm::preparaMenu", 0 );
}



///
/**
\return
**/
void BlSubForm::contextMenuEvent ( QContextMenuEvent * )
{
    blDebug ( "BlSubForm::contextMenuEvent", 0 );
    QAction *del = NULL;
    int row = currentRow();
    if ( row < 0 ) {
	blDebug ( "END BlSubForm::contextMenuEvent", 0 );
        return;
    } // end if
    int col = currentColumn();
    if ( col < 0 ) {
	blDebug ( "END BlSubForm::contextMenuEvent", 0 );
        return;
    } // end if
    QMenu *popup = new QMenu ( this );

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu ( popup );

    /// Lanzamos la propagacion del menu a traves de las clases derivadas.
    creaMenu ( popup );



    if ( m_delete ) {
        popup->addSeparator();
        del = popup->addAction ( _ ( "Borrar registro" ) );
	del->setIcon ( QIcon ( ":/Images/delete-one-line.png" ) );
    } // end if
    popup->addSeparator();

    QAction *ajustc = popup->addAction ( _ ( "Ajustar columa" ) );
    ajustc->setIcon ( QIcon ( ":/Images/adjust-one-row.png" ) );
    QAction *ajustac = popup->addAction ( _ ( "Ajustar altura" ) );
    ajustac->setIcon ( QIcon ( ":/Images/adjust-one-line.png" ) );
    
    QAction *ajust = popup->addAction ( _ ( "Ajustar columnas" ) );
    ajust->setIcon ( QIcon ( ":/Images/adjust-all-rows.png" ) );
    QAction *ajusta = popup->addAction ( _ ( "Ajustar alturas" ) );
    ajusta->setIcon ( QIcon ( ":/Images/adjust-all-lines.png" ) );

    popup->addSeparator();
    QAction *menuconfig = popup->addAction ( _ ( "Ver/Ocultar menu de subformulario" ) );
    menuconfig->setIcon ( QIcon ( ":/Images/togglemenu.png" ) );
    QAction *verconfig = popup->addAction ( _ ( "Ver/Ocultar configurador de subformulario" ) );
    verconfig->setIcon ( QIcon ( ":/Images/toggleconfig.png" ) );
    
    QAction *opcion = popup->exec ( QCursor::pos() );

    /// Si no hay ninguna opcion pulsada se sale sin hacer nada
    if ( !opcion ) {
	return;
	blDebug ( "END BlSubForm::contextMenuEvent", 0 );
    } // end if
    if ( opcion == del )
        borrar ( row );
    if ( opcion == ajust )
        resizeColumnsToContents();
    if ( opcion == ajusta )
        resizeRowsToContents();
    if ( opcion == ajustc )
        resizeColumnToContents ( col );
    if ( opcion == ajustac )
        resizeRowToContents ( row );
    if ( opcion == verconfig )
        toogleConfig();
    if ( opcion == menuconfig )
        toogleMenuConfig();
    
    emit trataMenu ( opcion );

    /// Activamos las herederas.
    procesaMenu ( opcion );

    delete popup;
    blDebug ( "END BlSubForm::contextMenuEvent", 0 );
}

///
/**
**/
void BlSubForm::toogleConfig()
{
    blDebug ( "BlSubForm::toogleConfig", 0 );
    mui_configurador->setVisible ( mui_configurador->isHidden() );
    emit toogledConfig ( mui_configurador->isVisible() );
    blDebug ( "END BlSubForm::toogleConfig", 0 );
}

///
/**
**/
void BlSubForm::toogleMenuConfig()
{
    blDebug ( "BlSubForm::toogleMenuConfig", 0 );
    mui_menusubform->setVisible ( mui_menusubform->isHidden() );
    emit toogledMenuConfig ( mui_menusubform->isVisible() );
    blDebug ( "END BlSubForm::toogleMenuConfig", 0 );
}

///
/**
**/
void BlSubForm::on_mui_botonCerrar_clicked()
{
    blDebug ( "BlSubForm::on_mui_botonCerrar_clicked", 0 );
    toogleConfig();
    blDebug ( "END BlSubForm::on_mui_botonCerrar_clicked", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_itemChanged ( QTableWidgetItem * )
{
    blDebug ( "BlSubForm::on_mui_list_itemChanged", 0 );
    blDebug ( "END BlSubForm::on_mui_list_itemChanged", 0 );
}


///
/**
\param b
**/
void BlSubForm::setInsert ( bool b )
{
    blDebug ( "BlSubForm::setInsert", 0 );
    m_insercion = b;
    blDebug ( "END BlSubForm::setInsert", 0 );
}


///
/**
**/
bool BlSubForm::isInsert() {
    blDebug ( "BlSubForm::isInsert", 0 );
    blDebug ( "END BlSubForm::isInsert", 0 );
    return m_insercion;
}


/// Establece si el subformulario debe considerar y tratar los cambios de celdas
/**
\param proc
**/
void BlSubForm::setProcesarCambios ( bool proc )
{
    blDebug ( "BlSubForm::setProcesarCambios", 0 );
    m_procesacambios = proc;
    blDebug ( "END BlSubForm::setProcesarCambios", 0 );
}


/// Indica si el subformulario esta considerando el tratamiento de celdas modificadas
/**
\return
**/
bool BlSubForm::procesaCambios()
{
    blDebug ( "BlSubForm::procesaCambios", 0 );
    blDebug ( "END BlSubForm::procesaCambios", 0 );
    return m_procesacambios;
}


/// Devuelve el nombre del campo en la base de datos sabiendo que columna representa ese dato.
/**
\return
**/
QString BlSubForm::dbFieldNameByColumnId ( int columna )
{
    BlSubFormHeader * linea;
    linea = m_lcabecera.at ( columna );
    return linea->nomcampo();
}


/// Devuelve el nombre visual de la columna indicada
/**
  No usamos directamente la columna actual, ya que en ocasiones puede no ser correcta.
   \return Nombre visual de la columna
**/
QString BlSubForm::dbFieldViewNameByColumnId(int columna)
{
   blDebug("BlSubForm::dbFieldViewNameByColumnId", 0);

   blDebug("END BlSubForm::dbFieldViewNameByColumnId", 0);
   return m_lcabecera.at(columna)->nompresentacion();
}


/// Devuelve el tipo de la columna indicada
/**
    No usamos directamente la columna actual, ya que en ocasiones puede no ser correcta.
   \return Tipo de datos del campo
**/
BlDbField::DbType BlSubForm::dbFieldTypeByColumnId(int columna)
{
   blDebug("BlSubForm::dbFieldTypeByColumnId", 0);

   blDebug("END BlSubForm::dbFieldTypeByColumnId", 0);
   return m_lcabecera.at(columna)->dbFieldType();
}


/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::editFinished ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    blDebug ( "BlSubForm::editFinished", 0 );
    m_registrolinea = rec;
    m_campoactual = camp;

    /// Disparamos los plugins.
    g_plugins->lanza ( "BlSubForm_editFinished", this );


    blDebug ( "END BlSubForm::editFinished", 0 );
}

/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::pressedAsterisk ( int, int, BlDbSubFormRecord *, BlDbSubFormField * )
{
    blDebug ( "BlSubForm::pressedAsterisk", 0 );
    blDebug ( "END BlSubForm::pressedAsterisk", 0 );
}


/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::pressedPlus ( int, int, BlDbSubFormRecord *, BlDbSubFormField * )
{
    blDebug ( "BlSubForm::pressedPlus", 0 );
    blDebug ( "END BlSubForm::pressedPlus", 0 );
}


/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::pressedMinus ( int, int, BlDbSubFormRecord *, BlDbSubFormField * )
{
    blDebug ( "BlSubForm::pressedMinus", 0 );
    blDebug ( "END BlSubForm::pressedMinus", 0 );
}

/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::pressedSlash ( int, int, BlDbSubFormRecord *, BlDbSubFormField * )
{
    blDebug ( "BlSubForm::pressedSlash", 0 );
    blDebug ( "END BlSubForm::pressedSlash", 0 );
}

/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
QString BlSubForm::fileConfig (  )
{
    blDebug ( "BlSubForm::fileConfig", 0 );
    blDebug ( "END BlSubForm::fileConfig", 0 );
    return m_fileconfig;
}

///
/**
**/
void BlSubForm::setModoConsulta()
{
    blDebug ( "BlSubForm::setModoConsulta", 0 );
    m_modo = BL_SELECT_MODE;
    blDebug ( "END BlSubForm::setModoConsulta", 0 );

}


///
/**
**/
void BlSubForm::setModoEdicion()
{
    blDebug ( "BlSubForm::setModoEdicion", 0 );
    m_modo = BL_EDIT_MODE;
    blDebug ( "END BlSubForm::setModoEdicion", 0 );
}


///
/**
\return
**/
bool BlSubForm::modoEdicion()
{
    blDebug ( "BlSubForm::modoEdicion", 0 );
    blDebug ( "END BlSubForm::modoEdicion", 0 );
    return m_modo == BL_EDIT_MODE;
}


///
/**
\return
**/
bool BlSubForm::modoConsulta()
{
    blDebug ( "BlSubForm::modoConsulta", 0 );
    blDebug ( "END BlSubForm::modoConsulta", 0 );
    return m_modo == BL_SELECT_MODE;
}


///
/**
\return
**/
edmode BlSubForm::mode()
{
    blDebug ( "BlSubForm::mode", 0 );
    blDebug ( "END BlSubForm::mode", 0 );
    return m_modo;
}


/// Hace la exportacion del campo a XML
QString BlSubForm::exportXML() {
    blDebug ( "BlSubForm::exportXML", 0 );
    QString val;
    int error;

    val = "<BLSUBFORM>\n";
    val += "\t<QUERY>" + blXMLEncode(m_query) + "</QUERY>\n";
    val += "\t<TABLENAME>" + blXMLEncode(m_tablename) + "</TABLENAME>\n";
    val += "\t<MODO>"+ blXMLEncode(QString::number(m_modo))+"</MODO>\n";
    val += "\t<FILECONFIG>"+ blXMLEncode(m_fileconfig)+"</FILECONFIG>\n";
    val += "\t<CAMPOID>"+ blXMLEncode(m_campoid)+"</CAMPOID>\n";
    val += "\t<INSERCION>" + blXMLEncode(QString(m_insercion ? "TRUE" : "FALSE")) + "</INSERCION>\n";
    val += "\t<PRIMERO>" + blXMLEncode(QString(m_primero ? "TRUE" : "FALSE")) + "</PRIMERO>\n";
    val += "\t<DELETE>" + blXMLEncode(QString(m_delete ? "TRUE" : "FALSE")) + "</DELETE>\n";
    val += "\t<SORTING>" + blXMLEncode(QString(m_sorting ? "TRUE" : "FALSE")) + "</SORTING>\n";
    val += "\t<ORDEN>" + blXMLEncode(QString(m_orden ? "TRUE" : "FALSE")) + "</ORDEN>\n";
    val += "\t<ORDENPORQUERY>" + blXMLEncode(QString(m_ordenporquery ? "TRUE" : "FALSE")) + "</ORDENPORQUERY>\n";
    val += "\t<PROCESACAMBIOS>" + blXMLEncode(QString(m_procesacambios ? "TRUE" : "FALSE")) + "</PROCESACAMBIOS>\n";
    val += "\t<TEXTOCELDAPARAROWSPAN>" + blXMLEncode(m_textoceldaParaRowSpan) + "</TEXTOCELDAPARAROWSPAN>\n";
    val += "\t<COLUMNAPARAROWSPAN>" + blXMLEncode(m_columnaParaRowSpan) + "</COLUMNAPARAROWSPAN>\n";
    val += "\t<FILAINICIALROWSPAN>" + QString::number(m_filaInicialRowSpan) + "</FILAINICIALROWSPAN>\n";
    val += "\t<CABECERA>\n";
    BlSubFormHeader *linea;
    for ( int i = 0; i < m_lcabecera.size(); ++i ) {
        linea = m_lcabecera.at ( i );
        val += "\t\t" + linea->exportXML().replace("\t<","\t\t\t<").replace("\n<","\n\t\t<");
    } // end for
    val += "\t</CABECERA>\n";
    val += "\t<LISTA>\n";
    BlDbSubFormRecord *linea1;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        linea1 = m_lista.at ( i );
        val += "\t\t" + linea1->exportXML().replace("\t<","\t\t\t<").replace("\n<","\n\t\t<");
    } // end for
    val += "\t</LISTA>\n";
    val += "\t<LISTABORRAR>\n";
    for ( int i = 0; i < m_listaborrar.size(); ++i ) {
        linea1 = m_lista.at ( i );
        val += "\t\t" + linea1->exportXML().replace("\t<","\t\t\t<").replace("\n<","\n\t\t<");
    } // end for
    val += "\t</LISTABORRAR>\n";
    val += "</BLSUBFORM>\n";
    return val;
    blDebug ( "END BlSubForm::exportXML", 0 );
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
///
/**
\param parent
**/
BlSubFormDelegate::BlSubFormDelegate ( QObject *parent = 0 ) : QItemDelegate ( parent ), BlMainCompanyPointer()
{
    blDebug ( "BlSubFormDelegate::BlSubFormDelegate", 0 );
    m_subform = ( BlSubForm * ) parent;
    installEventFilter ( this );
    g_plugins->lanza ( "BlSubFormDelegate_BlSubFormDelegate_Post", ( void * ) this );
    blDebug ( "END BlSubFormDelegate::BlSubFormDelegate", 0 );
}


///
/**
**/
BlSubFormDelegate::~BlSubFormDelegate()
{
    blDebug ( "BlSubFormDelegate::~BlSubFormDelegate", 0 );
    blDebug ( "END BlSubFormDelegate::~BlSubFormDelegate", 0 );
}


///
/**
\param parent
\param option
\param index
**/
QWidget *BlSubFormDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    blDebug ( "BlSubFormDelegate::createEditor", 0, "CurrentColumn: " + QString::number ( index.column() ) + "CurrentRow" + QString::number ( index.row() )  );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    g_nomcampo = linea->nomcampo();
    g_editor = parent;

    if ( g_plugins->lanza ( "BlSubFormDelegate_createEditor", ( void * ) this ) ) {
        blDebug ( "END BlSubFormDelegate::createEditor", 0, "Salida por Plugins" );
        return ( QWidget * ) g_plugParams;
    } // end if

   if (linea->dbFieldType() == BlDbField::DbVarChar) {
        BlTextEditDelegate * editor = new BlTextEditDelegate ( parent );
        editor->setObjectName ( "BlTextEditDelegate" );
        blDebug ( "END BlSubFormDelegate::createEditor", 0, "QTextEdit" );
        return editor;
   } else if ( linea->dbFieldType() == BlDbField::DbNumeric ) {
        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -100000000 );
        editor->setMaximum ( 100000000 );
        editor->setDecimals(linea->numericPrecision());
        blDebug ( "END BfSubFormDelegate::createEditor", 0, "QSPinBox" );
        return editor;
    } else if ( linea->dbFieldType() == BlDbField::DbInt ) {
        QSpinBox * editor = new QSpinBox ( parent );
        editor->setMinimum ( -100000000 );
        editor->setMaximum ( 100000000 );
        blDebug ( "END BfSubFormDelegate::createEditor", 0, "QSPinBox" );
        return editor;
    }// end if



    blDebug ( "END BlSubFormDelegate::createEditor", 0, "Default Editor" );
    return QItemDelegate::createEditor ( parent, option, index );
}


///
/**
\param editor
\param model
\param index
\return
**/
void BlSubFormDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    blDebug ( "BlSubFormDelegate::setModelData", 0, "CurrentColumn: " + QString::number ( index.column() ) + "CurrentRow: " + QString::number ( index.row() ) );

    /// Si la fila o columna pasadas son inv&aacute;lidas salimos.
    if ( index.column() < 0 || index.row() < 0 ) {
        blDebug ( "END BlSubFormDelegate::setModelData", 0, "Fila o columna invalida" );
        return;
    } // end if
    g_index = index;
    g_model = model;
    g_editor = editor;
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    g_nomcampo = linea->nomcampo();

    if ( g_plugins->lanza ( "BlSubFormDelegate_setModelData", ( void * ) this ) ) {
        blDebug ( "END BlSubFormDelegate::setModelData", 0, "Salida por plugins" );
        return;
    } // end if


   if (linea->dbFieldType() == BlDbField::DbVarChar) {
        BlTextEditDelegate * textedit = qobject_cast<BlTextEditDelegate *> ( editor );
        model->setData ( index, textedit->toPlainText() );
   } else if ( linea->dbFieldType() == BlDbField::DbNumeric ) {
        BlDoubleSpinBox * spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        model->setData ( index, value );
    } else if ( linea->dbFieldType() == BlDbField::DbInt ) {
        QSpinBox * spinBox = static_cast<QSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = QString::number(spinBox->value());
        model->setData ( index, value );
    } else {
        QItemDelegate::setModelData ( editor, model, index );
    } // end if

    blDebug ( "END BlSubFormDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void BlSubFormDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    blDebug ( "BlSubFormDelegate::setEditorData", 0, "CurrentColumn: " + QString::number ( index.column() ) +  "CurrentRow: " + QString::number ( index.row() )  );


    g_index = index;
    g_editor = editor;
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    g_nomcampo = linea->nomcampo();

    if ( g_plugins->lanza ( "BlSubFormDelegate_setEditorData", ( void * ) this ) ) {
        blDebug ( "END BlSubFormDelegate::setEditorData", 0, "Salida por plugins" );
        return;
    } // end if

   if (linea->dbFieldType() == BlDbField::DbVarChar) {
        QString data = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlTextEditDelegate *textedit = qobject_cast<BlTextEditDelegate*> ( editor );
        textedit->setText ( data );
   } else if ( linea->dbFieldType() == BlDbField::DbNumeric ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDoubleSpinBox *spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();
    } else if ( linea->dbFieldType() == BlDbField::DbInt ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        QSpinBox *spinBox = static_cast<QSpinBox*> ( editor );
        spinBox->setValue ( value.toInt() );
        spinBox->selectAll();
    } else {
        QItemDelegate::setEditorData ( editor, index );
    }// end if



    blDebug ( "END BlSubFormDelegate::setEditorData", 0 );
}


///
/**
\param obj
\param event
\return
**/
bool BlSubFormDelegate::eventFilter ( QObject *obj, QEvent *event )
{
    /// Si es un release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyPress ) {
        blDebug ( "BlSubFormDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        blDebug ( "BlSubFormDelegate::key = : ", 0, QString::number ( key ) );
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// En caso de pulsacion de un retorno de carro o similar procesamos por nuestra cuenta.
        /// Si hemos pulsado ademas el Shift se lo pasamos al editor de texto para que haga un salto de linea
        /// Si no hemos pulsado el Shift se lo pasamos al BlTableWidget para que haga un saldo de celda.
        switch ( key ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if ( obj->objectName() == "BlTextEditDelegate" ) {
               if (mod & Qt::ShiftModifier) {
                   obj->event ( event );
               } else {
                   QApplication::sendEvent ( m_subform->mui_list, event );
               } // end if
                return TRUE;
            } // end if

	  case Qt::Key_Backtab:
        case Qt::Key_Tab:
            return TRUE;
        } // end switch
    } else if ( event->type() == QEvent::KeyRelease ) {
        blDebug ( "BlSubFormDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        blDebug ( "BlSubFormDelegate::key = : ", 0, QString::number ( key ) );
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// En caso de pulsacion de un retorno de carro o similar procesamos por nuestra cuenta.
        /// Si hemos pulsado ademas el Shift se lo pasamos al editor de texto para que haga un salto de linea
        /// Si no hemos pulsado el Shift se lo pasamos al BlTableWidget para que haga un saldo de celda.
        switch ( key ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if ( obj->objectName() == "BlTextEditDelegate" ) {
               if (mod & Qt::ShiftModifier) {
                   obj->event ( event );
               } else {
		   ((QWidget*)obj)->clearFocus();
                   QApplication::sendEvent ( m_subform->mui_list, event );
               } // end if
                return TRUE;
            } // end if

	  case Qt::Key_Backtab:
        case Qt::Key_Tab:
 	    ((QWidget *)obj)->clearFocus();
            QApplication::sendEvent ( m_subform->mui_list, event );
            return TRUE;

        } // end switch
    } // end if

    return QItemDelegate::eventFilter ( obj, event );
}


///
/**
\param editor
\return
**/
int BlSubFormDelegate::cerrarEditor ( QWidget *editor )
{
    blDebug ( "BlSubFormDelegate::cerrarEditor", 0 );
    emit closeEditor ( editor, QAbstractItemDelegate::NoHint );
    blDebug ( "END BlSubFormDelegate::cerrarEditor", 0 );
    return 0;
}


