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
    _depura ( "BlSubFormHeader::BlSubFormHeader", 0 );
    m_nomcampo = nom;
    m_tipo = typ;
    m_restricciones = res;
    m_options = opt;
    m_nompresentacion = nomp;
    _depura ( "END BlSubFormHeader::BlSubFormHeader", 0 );
}


///
/**
**/
BlSubFormHeader::~BlSubFormHeader()
{
    _depura ( "BlSubFormHeader::~BlSubFormHeader", 0 );
    _depura ( "END BlSubFormHeader::~BlSubFormHeader", 0 );
}


///
/**
\return
**/
unsigned int BlSubFormHeader::options()
{
    _depura ( "BlSubFormHeader::options", 0 );
    _depura ( "END BlSubFormHeader::options", 0 );
    return m_options;
}


///
/**
\return
**/
unsigned int BlSubFormHeader::restricciones()
{
    _depura ( "BlSubFormHeader::restricciones", 0 );
    _depura ( "END BlSubFormHeader::restricciones", 0 );
    return m_restricciones;
}


///
/**
\return
**/
BlDbField::DbType BlSubFormHeader::dbFieldType()
{
    _depura ( "BlSubFormHeader::dbFieldType", 0 );
    _depura ( "END BlSubFormHeader::dbFieldType", 0 );
    return m_tipo;

}


///
/**
\return
**/
QString BlSubFormHeader::nompresentacion()
{
    _depura ( "BlSubFormHeader::nompresentacion", 0 );
    _depura ( "END BlSubFormHeader::nompresentacion", 0 );
    return m_nompresentacion;
}


///
/**
\return
**/
int BlSubFormHeader::restrictcampo()
{
    _depura ( "BlSubFormHeader::restrictcampo", 0 );
    _depura ( "END BlSubFormHeader::restrictcampo", 0 );
    return m_restricciones;
}


///
/**
\return
**/
QString BlSubFormHeader::nomcampo()
{
    _depura ( "BlSubFormHeader::nomcampo", 0 );
    _depura ( "END BlSubFormHeader::nomcampo", 0 );
    return m_nomcampo;
}


/// BlSubForm, constructor de la clase base para subformularios.
/**
\param parent
**/
BlSubForm::BlSubForm ( QWidget *parent ) : BlWidget ( parent )
{
    _depura ( "BlSubForm::BlSubForm", 0 );
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
    _depura ( "END BlSubForm::BlSubForm", 0 );
}


void BlSubForm::columnMovedByUser ( int, int oldIndex, int newIndex )
{
    _depura ( "BlSubForm::columnMovedByUser", 0 );
    mui_listcolumnas->moveRow ( oldIndex, newIndex );
    _depura ( "END BlSubForm::columnMovedByUser", 0 );
}


/// Destructor de Clase que guarda la configuracion.
/**
**/
BlSubForm::~BlSubForm()
{
    _depura ( "BlSubForm::~BlSubForm", 0 );
    /// PAra destruir desactivamos el control de cambios.
    m_procesacambios = FALSE;
    guardaconfig();
    _depura ( "END BlSubForm::~BlSubForm", 0 );
}


///
/**
\param emp
**/
void BlSubForm::setMainCompany ( BlMainCompany *emp )
{
    _depura ( "BlMainCompanyPointer::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( emp );
    cargaSpecs();
    _depura ( "END BlMainCompanyPointer::setMainCompany", 0 );
}


///
/**
**/
void BlSubForm::cargaSpecs()
{
    _depura ( "BlSubForm::cargaSpecs", 0 );
//    QFile file(g_confpr->valor(CONF_DIR_USER) + m_fileconfig + "_" + mainCompany()->dbName() + "_specs.spc");
    QFile file ( CONFIG_DIR_CONFIG + m_fileconfig + "_" + mainCompany() ->dbName() + "_specs.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        _depura ( "END BlSubForm::cargaSpecs", 0, "No se pudo abrir archivo" );
        return;
    }
    if ( !doc.setContent ( &file ) ) {
        file.close();
        _depura ( "END BlSubForm::cargaSpecs", 0, "XML no valido" );
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
//                } else if ( trestrict == "DBAUTO" ) {
//                    restricciones |= BlDbField::DbAuto;
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

    _depura ( "END BlSubForm::cargaSpecs", 0 );
}


///
/**
\return
**/
QList<BlDbSubFormRecord *> *BlSubForm::lista()
{
    _depura ( "BlSubForm::lista", 0 );
    _depura ( "END BlSubForm::lista", 0 );
    return &m_lista;
}


///
/**
\return
**/
QList<BlSubFormHeader *> *BlSubForm::cabecera()
{
    _depura ( "BlSubForm::cabecera", 0 );
    _depura ( "END BlSubForm::cabecera", 0 );
    return &m_lcabecera;
}


///
/**
\param mode
**/
void BlSubForm::setHorizontalScrollMode ( QAbstractItemView::ScrollMode mode )
{
    _depura ( "BlSubForm::setHorizontalScrollMode", 0 );
    mui_list->setHorizontalScrollMode ( mode );
    _depura ( "END BlSubForm::setHorizontalScrollMode", 0 );
}


///
/**
\param f
**/
void BlSubForm::setDelete ( bool f )
{
    _depura ( "BlSubForm::setDelete", 0 );
    m_delete = f;
    _depura ( "END BlSubForm::setDelete", 0 );
}


///
/**
\param row
\param col
\return
**/
BlDbSubFormField *BlSubForm::item ( int row, int col )
{
    _depura ( "BlSubForm::item", 0 );
    _depura ( "END BlSubForm::item", 0 );
    return ( BlDbSubFormField * ) mui_list->item ( row, col );
}


///
/**
\param row
\param col
**/
void BlSubForm::setCurrentItem ( int row, int col )
{
    _depura ( "BlSubForm::setCurrentItem", 0 );
    mui_list->setCurrentItem ( item ( row, col ) );
    _depura ( "END BlSubForm::setCurrentItem", 0 );
}


///
/**
\return
**/
int BlSubForm::columnCount()
{
    _depura ( "BlSubForm::columnCount", 0 );
    _depura ( "END BlSubForm::columnCount", 0 );
    return mui_list->columnCount();
}


///
/**
\param i
**/
void BlSubForm::showColumn ( int i )
{
    _depura ( "BlSubForm::showColumn", 0 );
    mui_list->showColumn ( i );
    _depura ( "END BlSubForm::showColumn", 0 );
}


///
/**
\param i
\return
**/
int BlSubForm::columnWidth ( int i )
{
    _depura ( "BlSubForm::columnWidth", 0 );
    _depura ( "END BlSubForm::columnWidth", 0 );
    return mui_list->columnWidth ( i );
}


///
/**
\param i
\param j
**/
void BlSubForm::setColumnWidth ( int i, int j )
{
    _depura ( "BlSubForm::setColumnWidth", 0 );
    mui_list->setColumnWidth ( i, j );
    _depura ( "END BlSubForm::setColumnWidth", 0 );
}


///
/**
\param i
**/
void BlSubForm::hideColumn ( int i )
{
    _depura ( "BlSubForm::hideColumn", 0 );
    mui_list->hideColumn ( i );
    _depura ( "END BlSubForm::hideColumn", 0 );
}


///
/**
\return
**/
int BlSubForm::currentRow()
{
    _depura ( "BlSubForm::currentRow", 0 );
    _depura ( "END BlSubForm::currentRow", 0 );
    return mui_list->currentRow();
}


///
/**
\return
**/
int BlSubForm::rowCount()
{
    _depura ( "BlSubForm::rowCount", 0 );
    _depura ( "END BlSubForm::rowCount", 0 );
    return mui_list->rowCount();
}


///
/**
\return
**/
int BlSubForm::currentColumn()
{
    _depura ( "BlSubForm::currentColumn", 0 );
    _depura ( "END BlSubForm::currentColumn", 0 );
    return mui_list->currentColumn();
}


///
/**
\param modo
**/
void BlSubForm::setResizeMode ( QHeaderView::ResizeMode modo )
{
    _depura ( "BlSubForm::setResizeMode", 0 );
    mui_list->horizontalHeader() ->setResizeMode ( modo );
    _depura ( "END BlSubForm::setResizeMode", 0 );
}


///
/**
**/
void BlSubForm::resizeColumnsToContents()
{
    _depura ( "BlSubForm::resizeColumnsToContents", 0 );
    mui_list->resizeColumnsToContents();
    _depura ( "END BlSubForm::resizeColumnsToContents", 0 );
}


///
/**
**/
void BlSubForm::resizeRowsToContents()
{
    _depura ( "BlSubForm::resizeRowsToContents", 0 );
    mui_list->resizeRowsToContents();
    _depura ( "END BlSubForm::resizeRowsToContents", 0 );
}


///
/**
\param i
**/
void BlSubForm::resizeColumnToContents ( int i )
{
    _depura ( "BlSubForm::resizeColumnToContents", 0 );
    mui_list->resizeColumnToContents ( i );
    _depura ( "END BlSubForm::resizeColumnToContents", 0 );
}


///
/**
\param i
**/
void BlSubForm::resizeRowToContents ( int i )
{
    _depura ( "BlSubForm::resizeRowToContents", 0 );
    mui_list->resizeRowToContents ( i );
    _depura ( "END BlSubForm::resizeRowToContents", 0 );
}


///
/**
**/
void BlSubForm::hideConfig()
{
    _depura ( "BlSubForm::hideConfig", 0 );
    mui_configurador->hide();
    _depura ( "END BlSubForm::hideConfig", 0 );
}


///
/**
**/
void BlSubForm::showConfig()
{
    _depura ( "BlSubForm::showConfig", 0 );
    mui_configurador->show();
    _depura ( "END BlSubForm::showConfig", 0 );
}


///
/**
\param nom
**/
void BlSubForm::setDbTableName ( QString nom )
{
    _depura ( "BlSubForm::setDbTableName", 0 );
    m_tablename = nom;
    m_fileconfig = nom;
    _depura ( "END BlSubForm::setDbTableName", 0 );
}


///
/**
\return
**/
QString BlSubForm::tableName()
{
    _depura ( "BlSubForm::tableName", 0 );
    _depura ( "END BlSubForm::tableName", 0 );
    return m_tablename;
}


///
/**
\param nom
**/
void BlSubForm::setFileConfig ( QString nom )
{
    _depura ( "BlSubForm::setFileConfig", 0 );
    m_fileconfig = nom;
    _depura ( "END BlSubForm::setFileConfig", 0 );
}


///
/**
\param nom
**/
void BlSubForm::setDbFieldId ( QString nom )
{
    _depura ( "BlSubForm::setDbFieldId", 0 );
    m_campoid = nom;
    _depura ( "END BlSubForm::setDbFieldId", 0 );
}


///
/**
**/
void BlSubForm::clear()
{
    _depura ( "BlSubForm::clear", 0 );
    mui_list->clear();
    _depura ( "END BlSubForm::clear", 0 );
}


///
/**
\param i
**/
void BlSubForm::setRowCount ( int i )
{
    _depura ( "BlSubForm::setRowCount", 0 );
    mui_list->setRowCount ( i );
    _depura ( "END BlSubForm::setRowCount", 0 );
}


///
/**
\param i
**/
void BlSubForm::setColumnCount ( int i )
{
    _depura ( "BlSubForm::setColumnCount", 0 );
    mui_list->setColumnCount ( i );
    _depura ( "END BlSubForm::setColumnCount", 0 );
}


///
/**
**/
void BlSubForm::creaMenu ( QMenu * )
{
    _depura ( "BlSubForm:: CreaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
**/
void BlSubForm::procesaMenu ( QAction * )
{
    _depura ( "BlSubForm:: procesaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
\param pijama
**/
void BlSubForm::setListadoPijama ( bool pijama )
{
    _depura ( "BlSubForm::setListadoPijama", 0 );
    if ( pijama ) {
        mui_list->setAlternatingRowColors ( TRUE );
    } else {
        mui_list->setAlternatingRowColors ( FALSE );
    } // end if
    _depura ( "END BlSubForm::setListadoPijama", 0 );
}


///
/**
\return
**/
bool BlSubForm::listadoPijama()
{
    _depura ( "BlSubForm::listadoPijama", 0 );
    return mui_list->alternatingRowColors();
    _depura ( "END BlSubForm::listadoPijama", 0 );
}



/// Habilita o inhabilita el ordenado de columnas mediante el pulsar sobre ellas.
/**
\param sorting
**/
void BlSubForm::setSortingEnabled ( bool sorting )
{
    _depura ( "BlSubForm::setSortingEnabled", 0 );
    mui_list->setSortingEnabled ( sorting );
    m_sorting = sorting;
    _depura ( "END BlSubForm::setSortingEnabled", 0 );
}


///
/**
\return
**/
bool BlSubForm::sortingEnabled()
{
    _depura ( "BlSubForm::sortingEnabled", 0 );
    _depura ( "END BlSubForm::sortingEnabled", 0 );
    return m_sorting;
}


/// Establece si el subformulario se ordena mediante un campo orden en la base de datos.
/**
\param sorting
**/
void BlSubForm::setOrdenEnabled ( bool sorting )
{
    _depura ( "BlSubForm::setOrdenEnabled", 0 );
    m_orden = sorting;
    _depura ( "END BlSubForm::setOrdenEnabled", 0 );
}


///
/**
\return
**/
bool BlSubForm::ordenEnabled()
{
    _depura ( "BlSubForm::ordenEnabled", 0 );
    return m_orden;
    _depura ( "END BlSubForm::ordenEnabled", 0 );
}


///
/**
\param item
**/
void BlSubForm::on_mui_list_itemDoubleClicked ( QTableWidgetItem *item )
{
    _depura ( "BlSubForm::on_mui_list_itemDoubleClicked", 0 );
    emit itemDoubleClicked ( item );
    _depura ( "END BlSubForm::on_mui_list_itemDoubleClicked", 0 );
}

///
/**
\param item
**/
void BlSubForm::on_mui_list_itemClicked ( QTableWidgetItem *item )
{
    _depura ( "BlSubForm::on_mui_list_itemClicked", 0 );
    emit itemClicked ( item );
    _depura ( "END BlSubForm::on_mui_list_itemClicked", 0 );
}


///
/**
\param row
\param col
**/
void BlSubForm::on_mui_list_cellDoubleClicked ( int row, int col )
{
    _depura ( "BlSubForm::on_mui_list_cellDoubleClicked", 0 );
    emit cellDoubleClicked ( row, col );
    _depura ( "END BlSubForm::on_mui_list_cellDoubleClicked", 0 );
}


///
/**
\param head
\return
**/
bool BlSubForm::existsHeader ( const QString &head )
{
    _depura ( "BlSubForm::existsHeader", 0 );
    BlSubFormHeader *linea;
    for ( int i = 0; i < m_lcabecera.size(); ++i ) {
        linea = m_lcabecera.at ( i );
        if ( linea->nomcampo() == head ) {
            _depura ( "END BlSubForm::existsHeader", 0 );
            return TRUE;
        } // end if
    } // end for
    _depura ( "END BlSubForm::existsHeader", 0 );
    return FALSE;
}

///
/**
\param head
\return
**/
BlSubFormHeader * BlSubForm::header ( const QString &head )
{
    _depura ( "BlSubForm::existsHeader", 0 );
    BlSubFormHeader *linea = NULL;
    for ( int i = 0; i < m_lcabecera.size(); ++i ) {
        linea = m_lcabecera.at ( i );
        if ( linea->nomcampo() == head ) {
            _depura ( "END BlSubForm::existsHeader", 0 );
            return linea;
        } // end if
    } // end for
    _depura ( "END BlSubForm::existsHeader", 0 );
    return NULL;
}




/// Se encarga de crear un nuevo registro (una fila entera) y de inicializarla para que
/// tenga todos los elementos necesarios (columnas).
/// Nota: Esta funcion es de uso interno, no debe ser usada.
/**
\return
**/
BlDbSubFormRecord *BlSubForm::newDbSubFormRecord()
{
    _depura ( "BlSubForm::newDbSubFormRecord", 0 );
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
    _depura ( "END BlSubForm::newDbSubFormRecord", 0 );
    return rec;
}


/// Este metodo crea el registro final cuando se trata de subformularios con la opcion
/// de insertar nuevos registros en el subformulario.
/**
\return
**/
void BlSubForm::nuevoRegistro()
{
    _depura ( "BlSubForm::nuevoRegistro", 0 );
    if ( !m_insercion ) {
        _depura ( "END BlSubForm::nuevoRegistro", 0, "No se permiten inserciones" );
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
    _depura ( "END BlSubForm::nuevoRegistro", 0 );
}


/// Pinta las cabeceras en la tabla.
/**
**/
void BlSubForm::pintaCabeceras()
{
    _depura ( "BlSubForm::", 0 );
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
    _depura ( "END BlSubForm::pintaCabeceras", 0 );
}


/// Se situa en una celda especifica del subformulario.
/**
\param row
\param col
**/
void BlSubForm::situarse ( unsigned int row, unsigned int col )
{
    _depura ( "BlSubForm::situarse", 0, QString::number ( row ) + " " + QString::number ( col ) );

    unsigned int nrow = row;
    unsigned int ncol = col;
    BlSubFormHeader *linea = m_lcabecera.at ( ncol );
    if ( !linea ) {
        _depura ( "END BlSubForm::situarse", 0, QString::number ( nrow ) + " " + QString::number ( ncol ) );
        return;
    } // end if
    bool invalido = TRUE;

    /// Mientras no se encuentre un candidato y haya candidatos vamos recorriendo celdas.
    while ( invalido && nrow < row + 2 ) {
        ncol++;
        /// Si estamos al final de la linea cogemos el principio de la linea siguiente
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
    } // end while

    if (!invalido) {
       mui_list->setCurrentCell ( nrow, ncol );
    } else {
       _depura("No hay más elementos editables en el subformulario", 2);
    } // end if
    _depura ( "END BlSubForm::situarse", 0, QString::number ( nrow ) + " " + QString::number ( ncol ) );
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
    _depura ( "BlSubForm::situarse1", 0, QString::number ( row ) + " " + QString::number ( col ) );
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
    _depura ( "END BlSubForm::situarse1", 0 );
}


/// Cuando tenemos un registro que no se tiene que cargar (pq es nuevo o algo
/// asi) de la base de datos, con la funcion pintar lo dejamos en un estado que
/// se podria considerar presentable para poder operar con el subformulario.
/**
**/
void BlSubForm::pintar()
{
    _depura ( "BlSubForm::pintar", 0 );
    m_procesacambios = FALSE;
    mui_list->setColumnCount ( m_lcabecera.count() );
    pintaCabeceras();
    if ( m_primero ) {
        cargaconfig();
    } // end if
    nuevoRegistro();
    m_procesacambios = TRUE;
    _depura ( "END BlSubForm::pintar", 0 );
}


/// Carga una tabla a partir del recordset que se le ha pasado.
/**
\return
**/
int BlSubForm::inicializar()
{
    _depura ( "BlSubForm::inicializar", 0 );
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
    } // end if

    nuevoRegistro();
    /// Ordenamos la tabla.
    mui_list->ordenar();
    /// configuramos que registros son visibles y que registros no lo son.
    on_mui_confcol_released();
    m_procesacambios = TRUE;
    _depura ( "END BlSubForm::inicializar", 0 );
    return 0;
}


///
/**
\param campo
**/
void BlSubForm::setColumnToRowSpan ( QString campo )
{
    _depura ( "BlSubForm::setColumnToRowSpan", 0 );
    m_columnaParaRowSpan = campo;
    _depura ( "END BlSubForm::setColumnToRowSpan", 0 );
}


///
/**
\param color
**/
void BlSubForm::setColorFondo1 ( QString color )
{
    _depura ( "BlSubForm::setColorFondo1", 0 );
    m_colorfondo1 = QColor ( color );
    _depura ( "END BlSubForm::setColorFondo1", 0 );
}


///
/**
\param color
**/
void BlSubForm::setColorFondo2 ( QString color )
{
    _depura ( "BlSubForm::setColorFondo2", 0 );
    m_colorfondo2 = QColor ( color );
    _depura ( "END BlSubForm::setColorFondo1", 0 );
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
    _depura ( "BlSubForm::cargar", 0, objectName() );
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
        for ( int j = 0; j < reg->lista() ->size(); ++j ) {
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
    } else {
        on_mui_confcol_released();
    } // end if

    /// Reactivamos el sorting
    mui_list->setSortingEnabled ( m_sorting );
    /// Borramos la barra de progreso
    delete barra;

    m_procesacambios = TRUE;

    _depura ( "END BlSubForm::cargar", 0 );
}


///
/**
\param ordenactivado
**/
void BlSubForm::setOrdenPorQuery ( bool ordenactivado )
{
    _depura ( "BlSubForm::setOrdenPorQuery", 0 );
    m_ordenporquery = ordenactivado;
    _depura ( "END BlSubForm::setOrdenPorQuery", 0 );
}


///
/**
\param query La consulta SQL a cargar en el subformulario.
**/
void BlSubForm::cargar ( QString query )
{
    _depura ( "BlSubForm::cargar", 0 );
    /// Si el query no existe no hacemos nada.
    if ( query == "" ) {
        _depura ( "BlSubForm::cargar", 0, "Query inexistente" );
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
        _depura ( "BlSubForm::cargar", 2, "Error en la carga de datos" );
    } // end try
    _depura ( "END BlSubForm::cargar", 0 );
}


/// Devuelve la linea que se esta tratando actualmente.
/**
\return
**/
BlDbSubFormRecord *BlSubForm::lineaact()
{
    _depura ( "BlSubForm::lineaact", 0 );
    _depura ( "END BlSubForm::lineaact", 0 );
    return lineaat ( mui_list->currentRow() );
}


/// Devuelve la linea especificada o NULL si esta no existe.
/**
\param row
\return
**/
BlDbSubFormRecord *BlSubForm::lineaat ( int row )
{
    _depura ( "BlSubForm::lineaat()", 0, QString::number ( row ) );
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
        _depura ( "BlSubForm::lineaat linea inexistente", 2, QString::number ( row ) );
        rec = NULL;
    }
    _depura ( "END BlSubForm::lineaat()", 0 );
    return rec;
}


/// Devuelve TRUE si el registro ha sido completamente rellenado.
/**
\param row
\return
**/
bool BlSubForm::campoCompleto ( int row )
{
    _depura ( "BlSubForm::campoCompleto", 0 );
    bool resultat = false;
    bool *pResultat = &resultat;
    if ( g_plugins->lanza ( "BlSubForm_campoCompleto", this, ( void** ) &pResultat ) ) {
        _depura ( "END BlSubForm::campoCompleto", 0 , "plugin retorna " + *pResultat );
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
                _depura ( "END BlSubForm::campoCompleto", 0, "El campo no es completo." );
                return FALSE;
            } // end if
            if ( camp->restrictcampo() & BlDbField::DbRequired
                    && camp->text() == "" ) {
                _depura ( "END BlSubForm::campoCompleto", 0, "El campo no es completo." );
                return FALSE;
            } // end if

        } // end for
        _depura ( "END BlSubForm::campoCompleto", 0 );
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
    _depura ( "BlSubForm::on_mui_list_cellReposition", 0, "Row: " + QString::number ( row ) + " col: " + QString::number ( col ) );

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
    if ( row == mui_list->rowCount() - 1 && campoCompleto ( row ) ) {
        nuevoRegistro();
        creado = TRUE;
    } // end if

    switch ( key ) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Tab:
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
    _depura ( "END BlSubForm::on_mui_list_cellRePosition", 0 );
}


/// M&eacute;todo que se dispara cuando se termina de editar un campo del Subformulario.
/**
\param row
\param col
**/
void BlSubForm::on_mui_list_cellChanged ( int row, int col )
{
    _depura ( "BlSubForm::on_mui_list_cellChanged", 0, "Row: " + QString::number ( row ) + " col: " + QString::number ( col ) );

    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        _depura ( "END BlSubForm::on_mui_list_cellChanged", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
        return;
    } // end if

    BlDbSubFormField *camp = ( BlDbSubFormField * ) item ( row, col );
    camp->refresh();

    /// Si el campo no ha sido cambiado se sale.
    if ( ! camp->cambiado() ) {
        _depura ( "END BlSubForm::on_mui_list_cellChanged", 0, QString::number ( row ) + " " + QString::number ( col ) + camp->valorcampo() + "Sin cambios" );
        return;
    } // end if

    /// En el caso de tener una fecha directamente la tratamos ya que esta claro cual es su tratamiento.
    if (camp->dbFieldType() == BlDbField::DbDate) {
        camp->set ( normalizafecha ( camp->valorcampo() ).toString ( "dd/MM/yyyy" ) );
    } // end if


    if ( m_procesacambios ) {
        m_procesacambios = FALSE;
        m_prevRow = row;
        m_prevCol = col;
        editFinished ( row, col, rec, camp );
        emit editFinish ( row, col );
        m_procesacambios = TRUE;
    } // end if
    _depura ( "END BlSubForm::on_mui_list_cellChanged", 0 );
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
    _depura ( "BlSubForm::addSubFormHeader", 0,  nom );

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


    mui_listcolumnas->setItem ( mui_listcolumnas->rowCount() - 1, 0, it );
    it = new BlTableWidgetItem ( nom );
    mui_listcolumnas->setItem ( mui_listcolumnas->rowCount() - 1, 1, it );
    it = new BlTableWidgetItem ( "" );
    mui_listcolumnas->setItem ( mui_listcolumnas->rowCount() - 1, 2, it );
    /// Escribe el order de las columnas.
    it = new BlTableWidgetItem ( QString::number ( mui_listcolumnas->rowCount() - 1 ) );
    mui_listcolumnas->setItem ( mui_listcolumnas->rowCount() - 1, 3, it );

    _depura ( "END BlSubForm::addSubFormHeader", 0, nom );
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
    _depura ( "BlSubForm::setColumnValue", 0, campo + " -- " + valor );
    BlDbSubFormRecord *rec;

    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        rec = lineaat ( i );
        if ( rec ) {
            rec->setDbValue ( campo, valor );
        } // end if
    } // end for
    _depura ( "END BlSubForm::setColumnValue", 0 );
}


///
/**
\param campo
\return
**/
BlFixed BlSubForm::sumarCampo ( QString campo )
{
    _depura ( "BlSubForm::sumarCampo", 0 );
    BlFixed total;
    BlDbSubFormRecord *rec;
    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        rec =  lineaat ( i );
        if ( rec ) {
            BlFixed subtotal = BlFixed ( rec->dbValue ( campo ) );
            total = total + subtotal;
        } // end if
    } // end for
    _depura ( "END BlSubForm::sumarCampo", 0 );
    return total;
}


///
/**
\param campo
\param row
**/
QString BlSubForm::dbValue ( const QString &campo, int row )
{
    _depura ( "BlSubForm::dbValue", 0 );
    try {
        BlDbSubFormRecord *rec;
        if ( row == -1 )
            rec = lineaact();
        else
            rec = lineaat ( row );
        if ( rec == NULL )
            throw - 1;
        _depura ( "END BlSubForm::dbValue", 0 );
        return rec->dbValue ( campo );
    } catch ( ... ) {
        mensajeInfo ( _ ("Fila inexistente" ));
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
    _depura ( "BlSubForm::setDbValue", 0 );
    try {
        BlDbSubFormRecord *rec;
        if ( row == -1 )
            rec = lineaact();
        else
            rec = lineaat ( row );
        if ( rec == NULL )
            throw - 1;
        rec->setDbValue ( campo, valor );
        _depura ( "END BlSubForm::setDbValue", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ("Fila inexistente" ) );
        throw - 1;
    }
}


///
/**
\return
**/
int BlSubForm::guardar()
{
    _depura ( "BlSubForm::guardar", 0 );
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

        _depura ( "END BlSubForm::guardar", 0 );
        return 0;
    } catch ( int e ) {
        if ( e == 100 ) {
            throw - 1;
        } // end if
    } catch ( ... ) {
        mensajeError ( _ ("Error inesperado en el guardado. [BlSubForm::guardar]" ) );
        throw - 1;
    } // end try
    return -1;
}


///
/**
\return
**/
int BlSubForm::borrar()
{
    _depura ( "BlSubForm::borrar", 0 );
    BlDbSubFormRecord *rec;
    int i = 0;
    int error = 0;

    /// Si la lista esta vacia no podemos borrar.
    if ( m_lista.count() <= 0 ) {
        _depura ( "END BlSubForm::borrar", 0 );
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
        _depura ( "END BlSubForm::borrar", 0 );
        return error;
    } catch ( ... ) {
        mensajeError ( _ ("Error al borrar. [BlSubForm::borrar]" ));
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
    _depura ( "BlSubForm::borrar", 0 );
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
        _depura ( "END BlSubForm::borrar", 0 );
        return 0;

    } catch ( ... ) {
        mensajeInfo ( _ ("Error al intentar borrar" ) );
        _depura ( "BlSubForm::borrar error al borrar", 3 );
        throw - 1;
    } // end try
}


/// Metodo para ser derivado.
/**
\return
**/
int BlSubForm::cerrarEditor()
{
    _depura ( "END BlSubForm::cerrarEditor", 0 );
    return 0;
}


/// Guardamos el archivo de configuracion.
/**
**/
void BlSubForm::guardaconfig()
{
    _depura ( "BlSubForm::guardaconfig", 0 );

    /// Si el subformulario no esta inicializado no hacemos el guardado.
    if ( ! mainCompany() )
        return;

    QString aux = "";
    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + m_fileconfig + "_" + mainCompany()->dbName() + "_" + QString::number ( m_modo ) + "_tablecfn.cfn" );
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

        file.close();
    } // end if
    _depura ( "END BlSubForm::guardaconfig", 0 );
}


///
/**
**/
void BlSubForm::cargaconfig()
{
    _depura ( "BlSubForm::cargaconfig", 0 );
    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + m_fileconfig + "_" + mainCompany()->dbName() + "_" + QString::number ( m_modo ) + "_tablecfn.cfn" );
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

        file.close();
        on_mui_confcol_released();
    } // end if

    /// Si se ha producido alg&uacute;n error en la carga hacemos un maquetado autom&aacute;tico.
    if ( error )
        mui_list->resizeColumnsToContents();

    m_primero = FALSE;
    _depura ( "END BlSubForm::cargaconfig", 0 );
}


///
/**
**/
void BlSubForm::on_mui_confcol_released()
{
    _depura ( "BlSubForm::on_mui_confcol_released", 0 );
    for ( int i = 0; i < mui_listcolumnas->rowCount(); ++i ) {
        if ( mui_listcolumnas->item ( i, 0 ) ->checkState() == Qt::Checked ) {
            mui_list->showColumn ( mui_listcolumnas->item ( i, 3 )->text().toInt() );
        } else {
            /// Coge el valor de la columna de 'order' para ocultarla.
            mui_list->hideColumn ( mui_listcolumnas->item ( i, 3 )->text().toInt() );
        } // end if
    } // end for
    _depura ( "END BlSubForm::on_mui_confcol_released", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_pressedSlash ( int row, int col )
{
    _depura ( "BlSubForm::on_mui_list_pressedSlash", 0 );
    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        _depura ( "END BlSubForm::on_mui_list_pressedSlash", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
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
    _depura ( "END BlSubForm::on_mui_list_pressedSlash", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_pressedAsterisk ( int row, int col )
{
    _depura ( "BlSubForm::on_mui_list_pressedAsterisk", 0 );
    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        _depura ( "END BlSubForm::on_mui_list_pressedAsterisk", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
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
    _depura ( "END BlSubForm::on_mui_list_pressedAsterisk", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_pressedPlus ( int row, int col )
{
    _depura ( "BlSubForm::on_mui_list_pressedPlus", 0 );
    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        _depura ( "END BlSubForm::on_mui_list_pressedPlus", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
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
    _depura ( "END BlSubForm::on_mui_list_pressedPlus", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_pressedMinus ( int row, int col )
{
    _depura ( "BlSubForm::on_mui_list_pressedMinus", 0 );
    BlDbSubFormRecord *rec = lineaat ( row );
    if ( rec == NULL ) {
        _depura ( "END BlSubForm::on_mui_list_pressedMinus", 0, QString::number ( row ) + " " + QString::number ( col ) + "la linea no existe" );
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
    _depura ( "END BlSubForm::on_mui_list_pressedMinus", 0 );
}


///
/**
\return
**/
QString BlSubForm::imprimir()
{
    _depura ( "BlSubForm::imprimir", 0 );
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
            fitxersortidarml += "    <td>" +  mui_listcolumnas->item ( h, 0 ) ->text() + "</td>\n";
        } // end if
        barra.setValue ( barra.value() + 1 );
    } // end for
    fitxersortidarml += "</tr>\n";
    
    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        fitxersortidarml += "<tr>\n";
        for ( int j = 0; j < mui_listcolumnas->rowCount(); ++j ) {
            if ( mui_listcolumnas->item ( j, 0 )->checkState() == Qt::Checked ) {
		BlDbSubFormField *valor = ( BlDbSubFormField * ) mui_list->item ( i, mui_listcolumnas->item ( j, 3)->text().toInt() );
                if ( valor->dbFieldType() & BlDbField::DbNumeric )
                    fitxersortidarml += "    <td>" +  spanish.toString ( valor->text().toDouble(), 'f', 2 ) + "</td>\n";
                else
                    fitxersortidarml += "    <td>" +  valor->text() + "</td>\n";
            } // end if
        } // end for
        fitxersortidarml += "</tr>\n";
        barra.setValue ( barra.value() + 1 );
    } // end for
    _depura ( "END BlSubForm::imprimir", 0 );
    return fitxersortidarml;
}


///
/**
\return
**/
void BlSubForm::on_mui_confquery_released()
{
    _depura ( "BlSubForm::on_mui_confquery_released ", 0 );
    if ( mainCompany() == NULL ) {
        mensajeInfo ( _ ("no se ha inicializado bien la clase" ) );
        return;
    } // end if
    mui_paginaact->setValue ( 1 );
    cargar ( mui_query->toPlainText() );
//  cargar(m_query);
    _depura ( "END BlSubForm::on_mui_confquery_released ", 0 );
}


///
/**
\return
**/
void BlSubForm::confquery()
{
    _depura ( "BlSubForm::confquery", 0 );
    if ( mainCompany() == NULL ) {
        mensajeInfo ( _ ("no se ha inicializado bien la clase" ) );
        return;
    } // end if

    cargar ( m_query );

    _depura ( "END BlSubForm::confquery ", 0 );
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
    _depura ( "BlSubForm::on_mui_list_ctrlSubir", 0, " (" + QString::number ( row ) + "," + QString::number ( col ) + ")" );
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
    _depura ( "END BlSubForm::on_mui_list_ctrlSubir", 0 );
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
    _depura ( "BlSubForm::on_mui_list_ctrlBajar", 0 );
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

    _depura ( "END BlSubForm::on_mui_list_ctrlBajar", 0 );
}


///
/**
**/
void BlSubForm::on_mui_pagsiguiente_released()
{
    _depura ( "BlSubForm::on_mui_pagsiguiente_released", 0 );
    int pag = mui_paginaact->text().toInt();
    pag++;
    mui_paginaact->setValue ( pag );
    confquery();
    _depura ( "END BlSubForm::on_mui_pagsiguiente_released", 0 );
}

///
/**
**/
void BlSubForm::on_mui_paganterior_released()
{
    _depura ( "BlSubForm::on_mui_paganterior_released", 0 );
    int pag = mui_paginaact->text().toInt();
    if ( pag > 1 )
        pag--;
    mui_paginaact->setValue ( pag );
    confquery();
    _depura ( "END BlSubForm::on_mui_paganterior_released", 0 );
}


///
/**
\param titular
**/
void BlSubForm::imprimirPDF ( const QString &titular )
{
    _depura ( "BlSubForm::imprimir", 0 );

    /// Los listados siempre usan la misma plantilla para imprimir listado.
    QString archivo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "listado.rml";
//    QString archivo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + titular + ".rml";
    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + "listado.rml";
//    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + titular + ".rml";
    QString archivologo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii() );
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii() );

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

    invocaPDF ( "listado" );
    _depura ( "END BlSubForm::imprimir", 0 );
}

///
/**
\return
**/
void BlSubForm::contextMenuEvent ( QContextMenuEvent * )
{
    _depura ( "BlSubForm::contextMenuEvent", 0 );
    QAction *del = NULL;
    int row = currentRow();
    if ( row < 0 )
        return;
    int col = currentColumn();
    if ( row < 0 )
        return;

    QMenu *popup = new QMenu ( this );

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu ( popup );

    /// Lanzamos la propagacion del menu a traves de las clases derivadas.
    creaMenu ( popup );



    if ( m_delete ) {
        popup->addSeparator();
        del = popup->addAction ( _ ( "Borrar registro" ) );
    } // end if
    popup->addSeparator();

    QAction *ajustc = popup->addAction ( _ ( "Ajustar columa" ) );
    QAction *ajustac = popup->addAction ( _ ( "Ajustar altura" ) );

    QAction *ajust = popup->addAction ( _ ( "Ajustar columnas" ) );
    QAction *ajusta = popup->addAction ( _ ( "Ajustar alturas" ) );

    popup->addSeparator();
    QAction *verconfig = popup->addAction ( _ ( "Ver/Ocultar configurador de subformulario" ) );

    QAction *opcion = popup->exec ( QCursor::pos() );

    /// Si no hay ninguna opcion pulsada se sale sin hacer nada
    if ( !opcion ) return;
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

    emit trataMenu ( opcion );

    /// Activamos las herederas.
    procesaMenu ( opcion );

    delete popup;
    _depura ( "END BlSubForm::contextMenuEvent", 0 );
}

///
/**
**/
void BlSubForm::toogleConfig()
{
    _depura ( "BlSubForm::toogleConfig", 0 );
    mui_configurador->setVisible ( mui_configurador->isHidden() );
    emit toogledConfig ( mui_configurador->isVisible() );
    _depura ( "END BlSubForm::toogleConfig", 0 );
}


///
/**
**/
void BlSubForm::on_mui_botonCerrar_released()
{
    _depura ( "BlSubForm::on_mui_botonCerrar_released", 0 );
    toogleConfig();
    _depura ( "END BlSubForm::on_mui_botonCerrar_released", 0 );
}


///
/**
**/
void BlSubForm::on_mui_list_itemChanged ( QTableWidgetItem * )
{
    _depura ( "BlSubForm::on_mui_list_itemChanged", 0 );
    _depura ( "END BlSubForm::on_mui_list_itemChanged", 0 );
}


///
/**
\param b
**/
void BlSubForm::setInsert ( bool b )
{
    _depura ( "BlSubForm::setInsert", 0 );
    m_insercion = b;
    _depura ( "END BlSubForm::setInsert", 0 );
}


/// Establece si el subformulario debe considerar y tratar los cambios de celdas
/**
\param proc
**/
void BlSubForm::setProcesarCambios ( bool proc )
{
    _depura ( "BlSubForm::setProcesarCambios", 0 );
    m_procesacambios = proc;
    _depura ( "END BlSubForm::setProcesarCambios", 0 );
}


/// Indica si el subformulario esta considerando el tratamiento de celdas modificadas
/**
\return
**/
bool BlSubForm::procesaCambios()
{
    _depura ( "BlSubForm::procesaCambios", 0 );
    _depura ( "END BlSubForm::procesaCambios", 0 );
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


/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::editFinished ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    _depura ( "BlSubForm::editFinished", 0 );
    m_registrolinea = rec;
    m_campoactual = camp;

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BlSubForm_editFinished", this );


    _depura ( "END BlSubForm::editFinished", 0 );
}

/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::pressedAsterisk ( int, int, BlDbSubFormRecord *, BlDbSubFormField * )
{
    _depura ( "BlSubForm::pressedAsterisk", 0 );
    _depura ( "END BlSubForm::pressedAsterisk", 0 );
}


/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::pressedPlus ( int, int, BlDbSubFormRecord *, BlDbSubFormField * )
{
    _depura ( "BlSubForm::pressedPlus", 0 );
    _depura ( "END BlSubForm::pressedPlus", 0 );
}


/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::pressedMinus ( int, int, BlDbSubFormRecord *, BlDbSubFormField * )
{
    _depura ( "BlSubForm::pressedMinus", 0 );
    _depura ( "END BlSubForm::pressedMinus", 0 );
}

/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
void BlSubForm::pressedSlash ( int, int, BlDbSubFormRecord *, BlDbSubFormField * )
{
    _depura ( "BlSubForm::pressedSlash", 0 );
    _depura ( "END BlSubForm::pressedSlash", 0 );
}

/// Para ser derivado, permite a las clases derivadas y a esta el tratamiento de cambio de celda.
QString BlSubForm::fileConfig (  )
{
    _depura ( "BlSubForm::fileConfig", 0 );
    _depura ( "END BlSubForm::fileConfig", 0 );
    return m_fileconfig;
}

///
/**
**/
void BlSubForm::setModoConsulta()
{
    _depura ( "BlSubForm::setModoConsulta", 0 );
    m_modo = BL_SELECT_MODE;
    _depura ( "END BlSubForm::setModoConsulta", 0 );

}


///
/**
**/
void BlSubForm::setModoEdicion()
{
    _depura ( "BlSubForm::setModoEdicion", 0 );
    m_modo = BL_EDIT_MODE;
    _depura ( "END BlSubForm::setModoEdicion", 0 );
}


///
/**
\return
**/
bool BlSubForm::modoEdicion()
{
    _depura ( "BlSubForm::modoEdicion", 0 );
    _depura ( "END BlSubForm::modoEdicion", 0 );
    return m_modo == BL_EDIT_MODE;
}


///
/**
\return
**/
bool BlSubForm::modoConsulta()
{
    _depura ( "BlSubForm::modoConsulta", 0 );
    _depura ( "END BlSubForm::modoConsulta", 0 );
    return m_modo == BL_SELECT_MODE;
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
    _depura ( "BlSubFormDelegate::BlSubFormDelegate", 0 );
    m_subform = ( BlSubForm * ) parent;
    installEventFilter ( this );
    g_plugins->lanza ( "BlSubFormDelegate_BlSubFormDelegate_Post", ( void * ) this );
    _depura ( "END BlSubFormDelegate::BlSubFormDelegate", 0 );
}


///
/**
**/
BlSubFormDelegate::~BlSubFormDelegate()
{
    _depura ( "BlSubFormDelegate::~BlSubFormDelegate", 0 );
    _depura ( "END BlSubFormDelegate::~BlSubFormDelegate", 0 );
}


///
/**
\param parent
\param option
\param index
**/
QWidget *BlSubFormDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    _depura ( "BlSubFormDelegate::createEditor", 0, "CurrentColumn: " + QString::number ( index.column() ) + "CurrentRow" + QString::number ( index.row() )  );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    g_nomcampo = linea->nomcampo();
    g_editor = parent;

    if ( g_plugins->lanza ( "BlSubFormDelegate_createEditor", ( void * ) this ) ) {
        _depura ( "END BlSubFormDelegate::createEditor", 0, "Salida por Plugins" );
        return ( QWidget * ) g_plugParams;
    } // end if

   if (linea->dbFieldType() == BlDbField::DbVarChar) {
        BlTextEditDelegate * editor = new BlTextEditDelegate ( parent );
        editor->setObjectName ( "BlTextEditDelegate" );
        _depura ( "END BlSubFormDelegate::createEditor", 0, "QTextEdit" );
        return editor;
   } else if ( linea->dbFieldType() == BlDbField::DbNumeric ) {
        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -100000000 );
        editor->setMaximum ( 100000000 );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "QSPinBox" );
        return editor;
    } else if ( linea->dbFieldType() == BlDbField::DbInt ) {
        QSpinBox * editor = new QSpinBox ( parent );
        editor->setMinimum ( -100000000 );
        editor->setMaximum ( 100000000 );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "QSPinBox" );
        return editor;
    }// end if



    _depura ( "END BlSubFormDelegate::createEditor", 0, "Default Editor" );
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
    _depura ( "BlSubFormDelegate::setModelData", 0, "CurrentColumn: " + QString::number ( index.column() ) + "CurrentRow: " + QString::number ( index.row() ) );

    /// Si la fila o columna pasadas son inv&aacute;lidas salimos.
    if ( index.column() < 0 || index.row() < 0 ) {
        _depura ( "END BlSubFormDelegate::setModelData", 0, "Fila o columna invalida" );
        return;
    } // end if
    g_index = index;
    g_model = model;
    g_editor = editor;
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    g_nomcampo = linea->nomcampo();

    if ( g_plugins->lanza ( "BlSubFormDelegate_setModelData", ( void * ) this ) ) {
        _depura ( "END BlSubFormDelegate::setModelData", 0, "Salida por plugins" );
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

    _depura ( "END BlSubFormDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void BlSubFormDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    _depura ( "BlSubFormDelegate::setEditorData", 0, "CurrentColumn: " + QString::number ( index.column() ) +  "CurrentRow: " + QString::number ( index.row() )  );


    g_index = index;
    g_editor = editor;
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    g_nomcampo = linea->nomcampo();

    if ( g_plugins->lanza ( "BlSubFormDelegate_setEditorData", ( void * ) this ) ) {
        _depura ( "END BlSubFormDelegate::setEditorData", 0, "Salida por plugins" );
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



    _depura ( "END BlSubFormDelegate::setEditorData", 0 );
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
        _depura ( "BlSubFormDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "BlSubFormDelegate::key = : ", 0, QString::number ( key ) );
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

        case Qt::Key_Tab:
            return TRUE;
        } // end switch
    } else if ( event->type() == QEvent::KeyRelease ) {
        _depura ( "BlSubFormDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "BlSubFormDelegate::key = : ", 0, QString::number ( key ) );
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
    _depura ( "BlSubFormDelegate::cerrarEditor", 0 );
    emit closeEditor ( editor, QAbstractItemDelegate::NoHint );
    _depura ( "END BlSubFormDelegate::cerrarEditor", 0 );
    return 0;
}


