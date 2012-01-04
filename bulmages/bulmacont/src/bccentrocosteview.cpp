/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "bccentrocosteview.h"
#include "bccompany.h"
#include "bccentrocosteseleccionarview.h"
#include "bcsubform.h"

#define COL_NOM_COSTE  0
#define COL_DESC_COSTE 1
#define COL_IDC_COSTE  2


/// Esta clase se encarga de presentar los centros de coste, la ventana, y
/// de controlar la inserci&oacute;n de nuevos centros de coste, borrarlos, etc.
/**
\param emp
\param parent
**/
BcCentroCosteView::BcCentroCosteView ( BcCompany  *emp, QWidget *parent )
        : BcForm ( emp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

    setTitleName ( _ ( "Centro de Coste" ) );
    /// Etablecemos cual va a ser la tabla para obtener los permisos
    setDbTableName ( "c_coste" );
    setDbFieldId ( "idc_coste" );
    addDbField ( "idc_coste", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id. Centro Coste" ) );
    addDbField ( "nombre", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Nombre" ) );
    addDbField ( "descripcion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Descripcion" ) );

    /// Inicializamos el subformulario de centros distribuidos
    mui_cdistribuidos->setMainCompany ( emp );
    mui_cdistribuidos->setDbTableName ( "c_costedist" );
    mui_cdistribuidos->setFileConfig ( "ccostedist" );
    mui_cdistribuidos->setDbFieldId ( "idc_costedist" );
    mui_cdistribuidos->addSubFormHeader ( "nomc_coste", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre Destinatario" ) );
    mui_cdistribuidos->addSubFormHeader ( "porcentc_costedist", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Porcentaje" ) );
    mui_cdistribuidos->addSubFormHeader ( "idc_costedist", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id" ) );
    mui_cdistribuidos->addSubFormHeader ( "idc_coste", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Destinatario" ) );
    mui_cdistribuidos->addSubFormHeader ( "iddestc_coste", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Origen" ) );
    mui_cdistribuidos->setInsert ( TRUE );
    mui_cdistribuidos->setOrdenEnabled ( FALSE );
    idc_coste = 0;


    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BcCentroCosteView_BcCentroCosteView", this );
    if ( res != 0 ) {
        return;
    } // end if


    mui_list->setColumnCount ( 3 );
    QStringList headers;
    headers << _ ( "Nombre" ) << _ ( "Descripcion" ) << _ ( "Id centro de coste" );
    mui_list->setColumnWidth ( 0, 200 );
    mui_list->setHeaderLabels ( headers );
    mui_list->setColumnHidden ( COL_IDC_COSTE, TRUE );
    dialogChanges_setExcludedObject ( mui_list );
    dialogChanges_readValues();
    insertWindow ( windowTitle(), this );
    repintar();
    
}


///
/**
**/
BcCentroCosteView::~BcCentroCosteView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


///
/**
**/
void BcCentroCosteView::repintar()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem *> Lista;
    int padre;
    int idc_coste1 = 0;
    BlDbRecordSet *cursoraux1, *cursoraux2;

    /// Vaciamos el &aacute;rbol.
    mui_list->clear();

    /// Cargamos las cuentas de primer nivel.
    cursoraux1 = mainCompany() ->loadQuery ( "SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste" );
    while ( !cursoraux1->eof() ) {
        padre = atoi ( cursoraux1->value( "padre" ).toAscii() );
        idc_coste1 = atoi ( cursoraux1->value( "idc_coste" ).toAscii() );
        it = new QTreeWidgetItem ( mui_list );
        Lista[idc_coste1] = it;
        it->setText ( COL_IDC_COSTE, cursoraux1->value( "idc_coste" ) );
        it->setText ( COL_DESC_COSTE, cursoraux1->value( "descripcion" ) );
        it->setText ( COL_NOM_COSTE, cursoraux1->value( "nombre" ) );
        mui_list->expandItem ( it );
        cursoraux1->nextRecord();
    } // end while
    delete cursoraux1;

    /// Cargamos las cuentas hijas
    cursoraux2 = mainCompany() ->loadQuery ( "SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste" );
    while ( !cursoraux2->eof() ) {
        padre = cursoraux2->value( "padre" ).toInt();
        idc_coste1 = cursoraux2->value( "idc_coste" ).toInt();
        if ( Lista[padre] ) {
            it = new QTreeWidgetItem ( Lista[padre] );
            Lista[idc_coste1] = it;
            it->setText ( COL_IDC_COSTE, cursoraux2->value( "idc_coste" ) );
            it->setText ( COL_DESC_COSTE, cursoraux2->value( "descripcion" ) );
            it->setText ( COL_NOM_COSTE, cursoraux2->value( "nombre" ) );
            mui_list->expandItem ( it );
        } else {
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error en la carga de centros de coste.") );
        } // end if
        cursoraux2->nextRecord();
    } // end while
    delete cursoraux2;

    /// Ya que se han producido cambios en los centros de coste
    /// Se inicializa el selector de centros de coste.
    BcCentroCosteSeleccionarView *scoste = mainCompany() ->getselccostes();
    scoste->cargacostes();
    
}


///
/**
\param it
**/
void BcCentroCosteView::on_mui_list_itemClicked ( QTreeWidgetItem *it, int )
{
    BL_FUNC_DEBUG
    int previdccoste = it->text ( COL_IDC_COSTE ).toInt();
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar centro de coste" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    _ ( "&Guardar" ), _ ( "&Cancelar" ), 0, 0, 1 ) == 0 ) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    idc_coste = previdccoste;
    mostrarPlantilla();
    
}


///
/**
**/
void BcCentroCosteView::mostrarPlantilla()
{
    BL_FUNC_DEBUG

    QString query;
    BcForm::load ( QString::number ( idc_coste ) );

    query = "SELECT * FROM c_costedist LEFT JOIN (SELECT idc_coste AS idcc, nombre AS nomc_coste FROM c_coste) AS t1 ON c_costedist.idc_Coste = t1.idcc WHERE iddestc_coste=" + QString::number ( idc_coste );
    mui_cdistribuidos->load ( query );


    dialogChanges_readValues();
    
}


///
/**
**/
int BcCentroCosteView::save()
{
    BL_FUNC_DEBUG
    BcForm::save();

    mui_cdistribuidos->setColumnValue ( "iddestc_coste", QString::number ( idc_coste ) );
    mui_cdistribuidos->save();

    dialogChanges_readValues();
    repintar();
    
    return 0;
}


///
/**
**/
void BcCentroCosteView::on_mui_crear_clicked()
{
    BL_FUNC_DEBUG
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar centro de coste" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok ) {
            on_mui_guardar_clicked();
        } // end if
    } // end if

    QString query;
    QTreeWidgetItem *it;
    it = mui_list->currentItem();
    if ( it ) {
        idc_coste = atoi ( it->text ( COL_IDC_COSTE ).toAscii() );
        query.sprintf ( "INSERT INTO c_coste (padre, nombre, descripcion) VALUES (%d, 'Nuevo centro de coste', 'Escriba su descripcion')", idc_coste );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
    } else {
        query.sprintf ( "INSERT INTO c_coste (nombre, descripcion) VALUES ('Nuevo centro de coste', 'Escriba su descripcion')" );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
    } // end if
    query.sprintf ( "SELECT MAX(idc_coste) AS id_coste FROM c_coste" );
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    idc_coste = atoi ( cur->value( "id_coste" ).toAscii() );
    delete cur;
    mainCompany() ->commit();
    repintar();
    
}


///
/**
**/
void BcCentroCosteView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
    try {
        switch ( QMessageBox::warning ( this,
                                        _ ( "Borrar centro de coste" ),
                                        _ ( "Se va a borrar el centro de coste.\nEsta operacion puede ocasionar perdida de datos." ),
                                        _ ( "&Borrar" ), _ ( "&Cancelar" ), 0, 0, 1 ) ) {
        case 0: /// Retry clicked or Enter pressed.
            QString query;
            query.sprintf ( "DELETE FROM c_coste WHERE idc_coste = %d", idc_coste );
            mainCompany() ->begin();
            mainCompany() ->runQuery ( query );
            mainCompany() ->commit();
            idc_coste = 0;
            repintar();
        } // end switch
    } catch ( ... ) {
        blMsgInfo ( _ ( "No se pudo borrar el Centro de Coste" ) );
        mainCompany() ->rollback();
        return;
    } // end try
    
}



