/***************************************************************************
*   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include <QtWidgets/QMenu>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QCheckBox>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtWidgets/QInputDialog>


#include "autoform.h"
#include "blcombobox.h"
#include "bfsubform.h"
#include "blsearchwidget.h"
#include "bldatesearch.h"
#include "bluiloader.h"


BlAutoForm * BlAutoFormList::createAutoForm() {
  BL_FUNC_DEBUG

    BlAutoForm *formulario = NULL;
    
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( g_confpr->value(CONF_DIR_USER) + "mautoform_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        blMsgInfo("Error al abrir el archivo de autoformularios");
        return 0;
    } // end if
    if ( !doc.setContent ( &file ) ) {
        file.close();
	blMsgInfo("Error al parsear el archivo de autoformularios");
        return 0;
    } // end if
    file.close();
    
    /// Iteramos para cada AutoForm y lo creamos haciendo todo lo necesario para que este funcione.
    QDomElement docElem = doc.documentElement();
    
    QDomNodeList nodos = docElem.elementsByTagName ( "AUTOFORM" );
    for ( int i = 0; i < nodos.count(); i++ ) {
        QDomNode autoform = nodos.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString interfacefile = e1.firstChildElement ( "UI_INTERFACE" ).toElement().text();
	    QString title = e1.firstChildElement ( "TITLE" ).toElement().text();
	    QString objname = e1.firstChildElement ( "OBJECTNAME" ).toElement().text();
	    QString objdesc = e1.firstChildElement ( "OBJECTDESCRIPTION" ).toElement().text();
	    QString fileicon = e1.firstChildElement ( "ICON" ).toElement().text();
	    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
	    QString tableid = e1.firstChildElement ( "TABLEID" ).toElement().text();
	    
	    if ( tablename == mui_list->tableName()) {

		/// Creamos el AutoForm, lo configuramos y lo presentamos.
		formulario = new BlAutoForm(mainCompany(), 0, 0, BL_EDIT_MODE, interfacefile, objdesc);
		formulario->setWindowTitle(title);
		formulario->setTitleName ( title );
		
		formulario->setObjectName(objname);
		formulario->setWindowIcon(QIcon(fileicon));
		formulario->label->setPixmap(QPixmap(fileicon));
		

		formulario->setDbTableName ( tablename );
		formulario->setDbFieldId ( tableid );

		QDomNodeList nodos = e1.elementsByTagName ( "CAMPO" );
		for ( int j = 0; j < nodos.count(); j++ ) {
		    QDomNode ventana = nodos.item ( j );
		    QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
		    if ( !e1.isNull() ) { /// the node was really an element.
			BlDbField::DbType type = BlDbField::DbVarChar;
			QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
			if ( formulario->exists ( nomheader ) ) return 0;
			QString nompheader = e1.firstChildElement ( "NOMPCAMPO" ).toElement().text();
			QString typeheader = e1.firstChildElement ( "DBTYPECAMPO" ).toElement().text();
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
			QDomElement restrict = e1.firstChildElement ( "RESTRICTIONSCAMPO" );
			while ( !restrict.isNull() ) {
			    QString trestrict = restrict.text();
			    if ( trestrict == "DBNOTHING" ) {
				restricciones |= BlDbField::DbNothing;
			    } else if ( trestrict == "DBNOTNULL" ) {
				restricciones |= BlDbField::DbNotNull;
			    } else if ( trestrict == "DBPRIMARYKEY" ) {
				restricciones |= BlDbField::DbPrimaryKey;
			    } else if ( trestrict == "DBNOSAVE" ) {
				restricciones |= BlDbField::DbNoSave;
			    } else if ( trestrict == "DBAUTO" ) {
				restricciones |= BlDbField::DbAuto;
			    } else if ( trestrict == "DBAUTO" ) {
				restricciones |= BlDbField::DbAuto;
			    } else if ( trestrict == "DBDUPPRIMARYKEY" ) {
				restricciones |= BlDbField::DbDupPrimaryKey;
			    } else if ( trestrict == "DBREQUIRED" ) {
				restricciones |= BlDbField::DbRequired;
			    } else if ( trestrict == "DBNOLOAD" ) {
				restricciones |= BlDbField::DbNoLoad;
			    } // end if
			    restrict = restrict.nextSiblingElement ( "RESTRICTIONSCAMPO" );
			} // end while

			formulario->addDbField ( nomheader, type, ( BlDbField::DbRestrict ) restricciones, nompheader );
			
			
			QString tipocampo = e1.firstChildElement ( "TIPOCAMPO" ).toElement().text();
			if (tipocampo == "BLCOMBOBOX") {
			    QString nombre= "mui_"+nomheader;
			    BlComboBox * combo = formulario->findChild<BlComboBox *>(nombre);
			    QString query = e1.firstChildElement ( "QUERY" ).toElement().text();
			    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
			    QString fieldid = e1.firstChildElement ( "FIELDID" ).toElement().text();
			    
			    
			    // Para no liarla parda, ponemos provincias y asi no habrá pedatas de momento.
			    combo->setQuery ( query );
			    combo->setTableName ( tablename );
			    combo->setFieldId ( fieldid );
			    QDomNodeList nodos = e1.elementsByTagName ( "VALOR" );
			    for ( int j = 0; j < nodos.count(); j++ ) {
				QDomNode ventana = nodos.item ( j );
				QString va = ventana.toElement().text(); /// try to convert the node to an element.
				combo->m_valores[va] = "";
			    } // end for
			    combo->setAllowNull ( true );
			    combo->setId ( "" );
			} // end if

			if (tipocampo == "BLSEARCHWIDGET") {
			    QString nombre= "mui_"+nomheader;
			    BlSearchWidget * search = formulario->findChild<BlSearchWidget *>(nombre);
			    QString label = e1.firstChildElement ( "LABEL" ).toElement().text();
			    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
			    QString fieldid = e1.firstChildElement ( "FIELDID" ).toElement().text();
			    
			    search->setLabel ( label );
			    search->setTableName ( tablename );
			    search->setFieldId ( fieldid );
			    QDomNodeList nodos = e1.elementsByTagName ( "VALOR" );
			    for ( int j = 0; j < nodos.count(); j++ ) {
				QDomNode ventana = nodos.item ( j );
				QString va = ventana.toElement().text(); /// try to convert the node to an element.
				search->m_valores[va] = "";
			    } // end for
			    search->setId ( "" );
			} // end if

		    } // end if
		} // end for
		
		
		/// Cargamos los subformularios
		QDomNodeList nodoss = e1.elementsByTagName ( "SUBFORM" );
		for ( int j = 0; j < nodoss.count(); j++ ) {
		    QDomNode ventana = nodoss.item ( j );
		    QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
		    if ( !e1.isNull() ) { /// the node was really an element.
			BlDbField::DbType type = BlDbField::DbVarChar;
			QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
			if ( formulario->exists ( nomheader ) ) return 0;
			QString nompheader = e1.firstChildElement ( "NOMPCAMPO" ).toElement().text();
			BlSubForm * subform = formulario->findChild<BlSubForm *>("mui_" + nomheader);
			QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
			QString fieldid = e1.firstChildElement ( "FIELDID" ).toElement().text();
			QString fileconfig = e1.firstChildElement ( "FILECONFIG" ).toElement().text();
			
			QString allowinsert = e1.firstChildElement ( "ALLOWINSERT" ).toElement().text();
			QString allowdelete = e1.firstChildElement ( "ALLOWDELETE" ).toElement().text();
			QString setsorting = e1.firstChildElement ( "SETSORTING" ).toElement().text();
			///
			
			subform->setDbTableName ( tablename );
			subform->setDbFieldId ( fieldid );
			subform->setFileConfig ( fileconfig );
			
			/// Hacemos el tratamiento de los headers
			    QDomNodeList nodos = e1.elementsByTagName ( "HEADER" );
			    for ( int j = 0; j < nodos.count(); j++ ) {
				QDomNode ventana = nodos.item ( j );
				QDomElement header = ventana.toElement();
				
				BlDbField::DbType type = BlDbField::DbVarChar;
				QString nomheader = header.firstChildElement ( "NOMCAMPO" ).toElement().text();
				QString nompheader = header.firstChildElement ( "NOMPCAMPO" ).toElement().text();
				QString typeheader = header.firstChildElement ( "DBTYPECAMPO" ).toElement().text();
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
				QDomElement restrict = header.firstChildElement ( "RESTRICTIONSCAMPO" );
				while ( !restrict.isNull() ) {
				    QString trestrict = restrict.text();
				    if ( trestrict == "DBNOTHING" ) {
					restricciones |= BlDbField::DbNothing;
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
				    restrict = restrict.nextSiblingElement ( "RESTRICTIONSCAMPO" );
				} // end while
				int opciones = ( int ) BlSubFormHeader::DbNone;
				QDomElement opci = header.firstChildElement ( "OPTIONSHEADER" );
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
				subform->addSubFormHeader ( nomheader, type, ( BlDbField::DbRestrict ) restricciones, opciones , nompheader );
				

			    } // end for			
			subform->setInsert ( allowinsert.toLower() == "true" );
			subform->setDelete ( allowdelete.toLower() == "true" );
			subform->setSortingEnabled ( setsorting.toLower() == "true" );
			subform->inicializar();
			
		    } // end if
		} // end for
		
		formulario->launch();
	    } // end if
	} // end if
    } // end for

    return formulario;

}


///
/**
\param parent
\param f
\param modo
**/
BlAutoForm::BlAutoForm ( BlMainCompany *emp, QWidget *parent, Qt::WindowFlags f, edmode modo, const QString &interfacefile, const QString &objdesc ) : BlForm ( emp, parent, f, modo )
{
    BL_FUNC_DEBUG
    
    setupUi(this);
    
    
    
     BlUiLoader loader(emp);
     QFile file(interfacefile);
     file.open(QFile::ReadOnly);
     QWidget *myWidget = loader.load(&file, 0);
     
    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout(m_general);

    layout->addWidget ( myWidget );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    layout->addWidget(myWidget);

    file.close();
   
    setAttribute ( Qt::WA_DeleteOnClose );
    blCenterOnScreen ( this );
    
    m_objdesc = objdesc;
}    

void BlAutoForm::launch() {
   BL_FUNC_DEBUG
        insertWindow ( windowTitle(), this, false );
        pintar();
        dialogChanges_readValues();
        blScript(this);
}

BlAutoForm::~BlAutoForm() {
    BL_FUNC_DEBUG
}



int BlAutoForm::load ( QString id, bool paint) {
  BL_FUNC_DEBUG
  BlForm::load(id, paint);
  
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( g_confpr->value(CONF_DIR_USER) + "mautoform_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        blMsgInfo("Error al abrir el archivo de autoformularios");
        return 0;
    } // end if
    if ( !doc.setContent ( &file ) ) {
        file.close();
	blMsgInfo("Error al parsear el archivo de autoformularios");
        return 0;
    } // end if
    file.close();
    
    /// Iteramos para cada AutoForm y lo creamos haciendo todo lo necesario para que este funcione.
    QDomElement docElem = doc.documentElement();
    
    QDomNodeList nodos = docElem.elementsByTagName ( "AUTOFORM" );
    for ( int i = 0; i < nodos.count(); i++ ) {
        QDomNode autoform = nodos.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
	    
	    if ( tablename == tableName()) {
		/// Cargamos los subformularios
		QDomNodeList nodoss = e1.elementsByTagName ( "SUBFORM" );
		for ( int j = 0; j < nodoss.count(); j++ ) {
		    QDomNode ventana = nodoss.item ( j );
		    QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
		    if ( !e1.isNull() ) { /// the node was really an element.
			BlDbField::DbType type = BlDbField::DbVarChar;
			QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
			BlSubForm * subform = findChild<BlSubForm *>("mui_" + nomheader);
			if (subform) {
			   /// Ya tenemos el subformulario presente. Y ahora podemos hacer lo que nos plazca.
			   QString query = e1.firstChildElement ( "QUERY" ).toElement().text();
			   /// Sustituimos las variables encontradas.
			   substrVars(query,0);
			   subform->load(query);
			} // end if
		    } // end if
		} // end for

	    } // end if
	} // end if
    } // end for
  return 0;
}


/** PostPintado el formulario.
*/
/**
\param idbudget
\return
**/
void BlAutoForm::pintarPost ( )
{
    BL_FUNC_DEBUG
    /// Escribimos como descripcion el nombre del cliente para que aparezca en el titulo y en el dockwidget
    QString buff = m_objdesc;
    parseTags(buff);
    setDescripcion( buff);
    
}


int BlAutoForm::afterSave () {
  BL_FUNC_DEBUG
  BlForm::afterSave();
  
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( g_confpr->value(CONF_DIR_USER) + "mautoform_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        blMsgInfo("Error al abrir el archivo de autoformularios");
        return 0;
    } // end if
    if ( !doc.setContent ( &file ) ) {
        file.close();
	blMsgInfo("Error al parsear el archivo de autoformularios");
        return 0;
    } // end if
    file.close();
    
    /// Iteramos para cada AutoForm y lo creamos haciendo todo lo necesario para que este funcione.
    QDomElement docElem = doc.documentElement();
    
    QDomNodeList nodos = docElem.elementsByTagName ( "AUTOFORM" );
    for ( int i = 0; i < nodos.count(); i++ ) {
        QDomNode autoform = nodos.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
	    
	    if ( tablename == tableName()) {

		
		/// Cargamos los subformularios
		QDomNodeList nodoss = e1.elementsByTagName ( "SUBFORM" );
		for ( int j = 0; j < nodoss.count(); j++ ) {
		    QDomNode ventana = nodoss.item ( j );
		    QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
		    if ( !e1.isNull() ) { /// the node was really an element.
			BlDbField::DbType type = BlDbField::DbVarChar;
			QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
			BlSubForm * subform = findChild<BlSubForm *>("mui_" + nomheader);
			if (subform) {
			   /// Ya tenemos el subformulario presente. Y ahora podemos hacer lo que nos plazca.
			   QString vinculos = e1.firstChildElement ( "VINCULOS" ).toElement().text();
			   /// Sustituimos las variables que informa el campo
			    QStringList listavinculos = vinculos.split(",");
			    for (int j1 = 0; j1 < listavinculos.size(); ++j1) {
				  
				  int posicion = listavinculos.at(j1).indexOf("=");
				  QString campo = listavinculos.at(j1).left(posicion);
				  QString valor = listavinculos.at(j1).right(listavinculos.at(j1).size() - posicion -1);
			    
				  subform->setColumnValue ( campo, dbValue ( valor ) );
			    } // end for

			    subform->save();
			} // end if
		    } // end if
		} // end for

	    } // end if
	} // end if
    } // end for
  return 0;
}


// ================ BLAUTOFORMLIST =====================

BlAutoFormList::BlAutoFormList ( BlMainCompany *comp, QWidget *parent, Qt::WindowFlags flag, edmode editmodo, const QString &interfacefile ) : BlFormList ( comp, parent, flag, editmodo ) {
    BL_FUNC_DEBUG
    setupUi ( this );

    mui_list->setMainCompany ( comp );

    setSubForm ( mui_list );
    
    
    
    
     BlUiLoader loader(comp);
     QFile file(interfacefile);
     file.open(QFile::ReadOnly);
     QWidget *myWidget = loader.load(&file, 0);

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout(m_busqueda);

    layout->addWidget ( myWidget );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    
    layout->addWidget(myWidget);


}

void BlAutoFormList::launch() {
    BL_FUNC_DEBUG
    if ( editMode() )
        mainCompany() ->insertWindow ( windowTitle(), this );
    hideBusqueda();
    
    
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( mui_list->tableName() );
    
    /// Cargamos los filtros guardados.
//     cargaFiltrosXML();
    
    presentar();
    
    /// Llamamos a los scripts
    blScript(this);
    
}



BlAutoFormList::~BlAutoFormList() {
    BL_FUNC_DEBUG
}


/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void BlAutoFormList::editar ( int )
{
    BL_FUNC_DEBUG
    try {
        QString campoid = mui_list->dbValue ( mui_list->dbFieldId() );
        if ( editMode() ) {
            BlAutoForm * bud = createAutoForm ();
            if ( bud->load ( campoid ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->show();
        } else {
	    close();
            emit ( selected ( campoid ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
}



/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase SocioView, y la presenta.
*/
void BlAutoFormList::crear()
{
    BL_FUNC_DEBUG
    BlAutoForm * bud = createAutoForm ();
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    bud->pintar();
    
}


/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase SocioView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void BlAutoFormList::remove()
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        if ( editMode() ) {
            BlAutoForm * cv = createAutoForm ();
            if ( cv->load ( mui_list->dbValue ( mui_list->dbFieldId() ) ))
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el cobro a cliente" ) );
    } // end try
    
}

