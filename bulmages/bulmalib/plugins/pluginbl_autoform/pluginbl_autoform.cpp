/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
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

#include <stdio.h>
#include <fstream>
#include <iostream>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QProcess>
#include <QTextStream>
#include <QDomDocument>
#include <QDomNode>


#include "pluginbl_autoform.h"
#include "blworkspace.h"
#include "autoform.h"
#include "blcombobox.h"
#include "blsearchwidget.h"

// Esta funcion solo se va a utilizar aqui asi que aqui se queda.
BlAutoFormList * genBlAutoFormList (const QString & );
BlAutoForm * genBlAutoForm( const QString &);


QMainWindow *g_pluginbl_autoform = NULL;
BlMainCompany *g_emp = NULL;


int entryPoint ( BlMainWindow *bges )
{
    BL_FUNC_DEBUG

    g_pluginbl_autoform = bges;
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_autoform", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    return 0;
}



int BfCompany_createMainWindows_Post ( BfCompany *cmp )
{
    BL_FUNC_DEBUG
    g_emp = cmp;
    
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( CONFIG_DIR_CONFIG + QString("autoform_") + cmp ->dbName() + "_spec.spc" );
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
    QDomNodeList autoformlists = docElem.elementsByTagName ( "AUTOFORMLIST" );
    for ( int i = 0; i < autoformlists.count(); i++ ) {
        QDomNode autoform = autoformlists.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString interfacefile = e1.firstChildElement ( "UI_INTERFACE" ).toElement().text();
	    QString title = e1.firstChildElement ( "TITLE" ).toElement().text();
	    QString objname = e1.firstChildElement ( "OBJECTNAME" ).toElement().text();
	    QString fileicon = e1.firstChildElement ( "ICON" ).toElement().text();
	    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
	    QString tableid = e1.firstChildElement ( "TABLEID" ).toElement().text();
	    
	    
	    /// Creamos el AutoForm, lo configuramos y lo presentamos.
	    BlAutoFormList *formulario = new BlAutoFormList(cmp, 0, 0, BL_EDIT_MODE, interfacefile);
	    formulario->setWindowTitle(title);
	    
	    formulario->setObjectName(objname);
	    formulario->setWindowIcon(QIcon(fileicon));
	    formulario->label->setPixmap(QPixmap(fileicon));
	    
	    formulario->mui_list->setDbTableName(tablename);
	    formulario->mui_list->setDbFieldId(tableid);

	    QDomNodeList nodos = e1.elementsByTagName ( "CAMPO" );
	    for ( int j = 0; j < nodos.count(); j++ ) {
		QDomNode ventana = nodos.item ( j );
		QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
		if ( !e1.isNull() ) { /// the node was really an element.
		    BlDbField::DbType type = BlDbField::DbVarChar;
		    QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
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

		    formulario->mui_list->addSubFormHeader ( nomheader, type, ( BlDbField::DbRestrict ) restricciones,opciones, nompheader );
		    
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
			combo->setAllowNull ( TRUE );
			combo->setId ( "" );
		    } // end if

		    if (tipocampo == "BLSEARCHWIDGET") {
			QString nombre= "mui_"+nomheader;
			BlSearchWidget * search = formulario->findChild<BlSearchWidget *>(nombre);
			QString label = e1.firstChildElement ( "LABEL" ).toElement().text();
			QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
			QString fieldid = e1.firstChildElement ( "FIELDID" ).toElement().text();
			
			
		    	// Para no liarla parda, ponemos provincias y asi no habrá pedatas de momento.
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

	    formulario->launch();
	    cmp ->m_pWorkspace->addSubWindow ( formulario );
	    formulario->show();
	} // end if
    } // end for

    return 0;
}


int BcCompany_createMainWindows_Post ( BcCompany *cmp )
{
    g_emp = cmp;
    blMsgInfo("En la creación de ventanas");
    return 0;
}





/// Vamos a implementar las busquedas en BlSearchWidget que responderan si hay una ventana
/// Asociada al tema


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
  
  
  BL_FUNC_DEBUG

    BlAutoFormList *formulario = NULL;
    
    formulario  = genBlAutoFormList(busq->tableName()) ;
    
    if (formulario) {
		/// Creamos el dialogo que contendra el BlAutoForm
		QDialog *diag = new QDialog ( 0 );
		diag->setModal ( true );
		diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
		blCenterOnScreen ( diag );
		
				formulario->launch();
		
		busq->connect ( formulario, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

		/// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
		/// para que sea redimensionable y aparezca el titulo de la ventana.
		QHBoxLayout *layout = new QHBoxLayout;
		layout->addWidget ( formulario );
		layout->setMargin ( 0 );
		layout->setSpacing ( 0 );
		diag->setLayout ( layout );
		diag->setWindowTitle ( formulario->windowTitle() );

		diag->exec();
		if ( formulario->mui_list->dbValue(formulario->mui_list->dbFieldId()) != "" ) {
		    busq->setId ( formulario->mui_list->dbValue(formulario->mui_list->dbFieldId()) );
		} // end if
		delete diag;


		return 1;
		
    } // end if
    
    return 0;

}




int BlSubForm_pressedAsterisk ( BlSubForm *sub )
{
    BL_FUNC_DEBUG


    /// Buscamos BLSUBFORMSEARCHEDIT que se correspondan con el campo sobre el que hemos pulsado
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( CONFIG_DIR_CONFIG + QString("autoform_") + g_emp ->dbName() + "_spec.spc" );
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
 
    QDomNodeList autoformlists = docElem.elementsByTagName ( "SUBFORMSEARCHEDIT" );
    for ( int i = 0; i < autoformlists.count(); i++ ) {
        QDomNode autoform = autoformlists.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString nomcampo = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
	    QString blautoformlist = e1.firstChildElement ( "BLAUTOFORMLIST" ).toElement().text();
	    QString query = e1.firstChildElement ( "QUERY" ).toElement().text();
	    QString vinculos = e1.firstChildElement ( "VINCULOS" ).toElement().text();
	    if (nomcampo == sub->m_campoactual->fieldName()) {

			BlAutoFormList *formulario = NULL;
			
			formulario  = genBlAutoFormList(blautoformlist) ;
			
			if (formulario) {
			  

				    /// Creamos el dialogo que contendra el BlAutoForm
				    QDialog *diag = new QDialog ( 0 );
				    diag->setModal ( true );
				    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
				    blCenterOnScreen ( diag );
				    
				    formulario->launch();
				    
				    diag->connect ( formulario, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

				    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
				    /// para que sea redimensionable y aparezca el titulo de la ventana.
				    QHBoxLayout *layout = new QHBoxLayout;
				    layout->addWidget ( formulario );
				    layout->setMargin ( 0 );
				    layout->setSpacing ( 0 );
				    diag->setLayout ( layout );
				    diag->setWindowTitle ( formulario->windowTitle() );

				    diag->exec();
				    if ( formulario->mui_list->dbValue(formulario->mui_list->dbFieldId()) != "" ) {

					/// Lo primero que hacemos es tratar el query y hacer las sustituciones
					int pos =  0;

					/// Buscamos parametros en el query y los ponemos.
					QRegExp rx ( "\\[(\\w*)\\]" );
					QString tmp;
					while ( ( pos = rx.indexIn ( query, pos ) ) != -1 ) {
					    if ( formulario->mui_list->existsHeader ( rx.cap ( 1 ) ) ) {    
						query.replace ( pos, rx.matchedLength(), formulario->mui_list->dbValue ( rx.cap ( 1 ) ) );
						pos = 0;
					    } else {
						pos += rx.matchedLength();
					    }
					} // end while
					

					BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
					if ( !cur->eof() ) {
					    /// Tratamos la vinculacion.
					    
					    
					    QStringList listavinculos = vinculos.split(",");
					    for (int j1 = 0; j1 < listavinculos.size(); ++j1) {
						  
						  int posicion = listavinculos.at(j1).indexOf("=");
						  QString campo = listavinculos.at(j1).left(posicion);
						  QString valor = listavinculos.at(j1).right(listavinculos.at(j1).size() - posicion-1);
						  
						  /// Lo primero que hacemos es tratar el query y hacer las sustituciones
						  int pos =  0;

						  /// Buscamos parametros en el query y los ponemos.
						  QRegExp rx ( "\\[(\\w*)\\]" );
						  QString tmp;
						  while ( ( pos = rx.indexIn ( valor, pos ) ) != -1 ) {
						      // Comprobamos la existencia del campo referenciado. Si este no existe entonces no hacemos nada.
						      if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {    
							  valor.replace ( pos, rx.matchedLength(), cur->value ( rx.cap ( 1 ) ) );
							  pos = 0;
						      } else {
							  pos += rx.matchedLength();
						      }
						  } // end while
						  

						  sub->m_registrolinea->setDbValue(campo, valor);
					    } // end for

					} // end if
					
					delete cur;
					
					
					
					
				    } // end if
				    delete diag;


				    return 1;
				    
			} // end if
	      
	    } // end if
	} // end if
    } // end for


    return 0;
}



/// Creamos un listado basandonos en la tabla.


BlAutoFormList * genBlAutoFormList (const QString &tname )
{
  
  
  BL_FUNC_DEBUG

    BlAutoFormList *formulario = NULL;
    
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( CONFIG_DIR_CONFIG + QString("autoform_") + g_emp ->dbName() + "_spec.spc" );
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
 
    QDomNodeList autoformlists = docElem.elementsByTagName ( "AUTOFORMLIST" );
    for ( int i = 0; i < autoformlists.count(); i++ ) {
        QDomNode autoform = autoformlists.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString interfacefile = e1.firstChildElement ( "UI_INTERFACE" ).toElement().text();
	    QString title = e1.firstChildElement ( "TITLE" ).toElement().text();
	    QString objname = e1.firstChildElement ( "OBJECTNAME" ).toElement().text();
	    QString fileicon = e1.firstChildElement ( "ICON" ).toElement().text();
	    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
	    QString tableid = e1.firstChildElement ( "TABLEID" ).toElement().text();
	    

	    if (tablename == tname) {
	      

	      
		/// Creamos el AutoForm, lo configuramos y lo presentamos.
		formulario = new BlAutoFormList(g_emp, 0, 0, BL_SELECT_MODE, interfacefile);
		formulario->setWindowTitle(title);
		
		formulario->setObjectName(objname);
		formulario->setWindowIcon(QIcon(fileicon));
		formulario->label->setPixmap(QPixmap(fileicon));
		
		formulario->mui_list->setDbTableName(tablename);
		formulario->mui_list->setDbFieldId(tableid);

		QDomNodeList nodos = e1.elementsByTagName ( "CAMPO" );
		for ( int j = 0; j < nodos.count(); j++ ) {
		    QDomNode ventana = nodos.item ( j );
		    QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
		    if ( !e1.isNull() ) { /// the node was really an element.
			BlDbField::DbType type = BlDbField::DbVarChar;
			QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
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

			formulario->mui_list->addSubFormHeader ( nomheader, type, ( BlDbField::DbRestrict ) restricciones,BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, nompheader );
			
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
			    combo->setAllowNull ( TRUE );
			    combo->setId ( "" );
			} // end if

			if (tipocampo == "BLSEARCHWIDGET") {
			    QString nombre= "mui_"+nomheader;
			    BlSearchWidget * search = formulario->findChild<BlSearchWidget *>(nombre);
			    QString label = e1.firstChildElement ( "LABEL" ).toElement().text();
			    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
			    QString fieldid = e1.firstChildElement ( "FIELDID" ).toElement().text();
			    
			    
			    // Para no liarla parda, ponemos provincias y asi no habrá pedatas de momento.
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
		
	    }// end if
	    
	} // end if
    } // end for

    return formulario;

}



/// --------------------------------------------------------------
/// --------- Implemento la edicion de alumnos -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de alumno como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///


/**
\param parent
**/
SubForm_AutoForm::SubForm_AutoForm ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_AutoForm::~SubForm_AutoForm()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_AutoForm::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BlSubForm *sub = ( BlSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "nombrealumno" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( QIcon ( ":/ImgGestionAula/icons/alumno.png" ), _ ( "Nuevo alumno" ) );
        QString idalumno = sub->dbValue ( "idalumno" );
        if ( idalumno != "" ) menu->addAction ( QIcon ( ":/ImgGestionAula/icons/alumno.png" ), _ ( "Editar alumno" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( QIcon ( ":/ImgGestionAula/icons/alumno-list.png" ), _ ( "Seleccionar alumno" ) );
        } // end if
    } // end if
    
}


///
/**
\param action
**/
void SubForm_AutoForm::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BlSubForm *sub = ( BlSubForm * ) parent();
    if ( action->text() == _ ( "Editar alumno" ) ) {
        QString idalumno = sub->dbValue ( "idalumno" );
        if ( idalumno != "" )
            editarAlumno ( idalumno );
    } else if ( action->text() == _ ( "Seleccionar alumno" ) ) {
        seleccionarAlumno ( sub );
    } else if ( action->text() == _ ( "Nuevo alumno" )  ) {
        nuevoAlumno();
    } // end if

    
}


///
/**
**/
void SubForm_AutoForm::editarAlumno ( QString idalumno )
{
    BL_FUNC_DEBUG

    
}



///
/**
**/
void SubForm_AutoForm::nuevoAlumno( )
{
    BL_FUNC_DEBUG
    QString tablename = sender()->objectName().right(sender()->objectName().size() -4 );


    BlSubForm * sub= (BlSubForm *) parent();
    

    /// Buscamos BLSUBFORMSEARCHEDIT que se correspondan con el campo sobre el que hemos pulsado
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( CONFIG_DIR_CONFIG + QString("autoform_") + g_emp ->dbName() + "_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        blMsgInfo("Error al abrir el archivo de autoformularios");
        return;
    } // end if
    if ( !doc.setContent ( &file ) ) {
        file.close();
	blMsgInfo("Error al parsear el archivo de autoformularios");
        return;
    } // end if
    file.close();
    
    /// Iteramos para cada AutoForm y lo creamos haciendo todo lo necesario para que este funcione.
    QDomElement docElem = doc.documentElement();
 
    QDomNodeList autoformlists = docElem.elementsByTagName ( "SUBFORMSEARCHEDIT" );
    for ( int i = 0; i < autoformlists.count(); i++ ) {
        QDomNode autoform = autoformlists.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString nomcampo = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
	    QString blautoformlist = e1.firstChildElement ( "BLAUTOFORMLIST" ).toElement().text();
	    QString query = e1.firstChildElement ( "QUERY" ).toElement().text();
	    QString vinculos = e1.firstChildElement ( "VINCULOS" ).toElement().text();
	    if (tablename == blautoformlist) {

			BlAutoForm *formulario = NULL;
			
			formulario  = genBlAutoForm(blautoformlist) ;
			
			if (formulario) {
				    formulario->setAttribute ( Qt::WA_DeleteOnClose, FALSE );
				    
				    formulario->mainCompany() ->m_pWorkspace->addSubWindow ( formulario );
				    formulario->setEnabled(false);
				    formulario->hide();
				    formulario->show();
				    formulario->setEnabled ( true );
				    
				    while ( !formulario->isHidden() )
					g_theApp->processEvents();

				    if ( formulario->dbValue(formulario-> fieldId()) != "" ) {

					/// Lo primero que hacemos es tratar el query y hacer las sustituciones
					int pos =  0;

					/// Buscamos parametros en el query y los ponemos.
					QRegExp rx ( "\\[(\\w*)\\]" );
					QString tmp;
					while ( ( pos = rx.indexIn ( query, pos ) ) != -1 ) {
					    if ( formulario->exists ( rx.cap ( 1 ) ) ) {    
						query.replace ( pos, rx.matchedLength(), formulario->dbValue ( rx.cap ( 1 ) ) );
						pos = 0;
					    } else {
						pos += rx.matchedLength();
					    }
					} // end while

					BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
					if ( !cur->eof() ) {
					    /// Tratamos la vinculacion.
					    
					    
					    QStringList listavinculos = vinculos.split(",");
					    for (int j1 = 0; j1 < listavinculos.size(); ++j1) {
						  
						  int posicion = listavinculos.at(j1).indexOf("=");
						  QString campo = listavinculos.at(j1).left(posicion);
						  QString valor = listavinculos.at(j1).right(listavinculos.at(j1).size() - posicion-1);
						  
						  /// Lo primero que hacemos es tratar el query y hacer las sustituciones
						  int pos =  0;

						  /// Buscamos parametros en el query y los ponemos.
						  QRegExp rx ( "\\[(\\w*)\\]" );
						  QString tmp;
						  while ( ( pos = rx.indexIn ( valor, pos ) ) != -1 ) {
						      // Comprobamos la existencia del campo referenciado. Si este no existe entonces no hacemos nada.
						      if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {    
							  valor.replace ( pos, rx.matchedLength(), cur->value ( rx.cap ( 1 ) ) );
							  pos = 0;
						      } else {
							  pos += rx.matchedLength();
						      }
						  } // end while
						  

						  sub->lineaact()->setDbValue(campo, valor);
					    } // end for

					} // end if
					
					delete cur;
				    } // end if
				    delete formulario;

				    
			} // end if
	      
	    } // end if
	} // end if
    } // end for


    



    
    
}


///
/**
**/
void SubForm_AutoForm::seleccionarAlumno ( BlSubForm *sub )
{
    BL_FUNC_DEBUG

    QString tablename = sender()->objectName().right(sender()->objectName().size() -5 );

    if (!sub) sub= (BlSubForm *) parent();
    

    /// Buscamos BLSUBFORMSEARCHEDIT que se correspondan con el campo sobre el que hemos pulsado
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( CONFIG_DIR_CONFIG + QString("autoform_") + g_emp ->dbName() + "_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        blMsgInfo("Error al abrir el archivo de autoformularios");
        return;
    } // end if
    if ( !doc.setContent ( &file ) ) {
        file.close();
	blMsgInfo("Error al parsear el archivo de autoformularios");
        return;
    } // end if
    file.close();
    
    /// Iteramos para cada AutoForm y lo creamos haciendo todo lo necesario para que este funcione.
    QDomElement docElem = doc.documentElement();
 
    QDomNodeList autoformlists = docElem.elementsByTagName ( "SUBFORMSEARCHEDIT" );
    for ( int i = 0; i < autoformlists.count(); i++ ) {
        QDomNode autoform = autoformlists.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString nomcampo = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
	    QString blautoformlist = e1.firstChildElement ( "BLAUTOFORMLIST" ).toElement().text();
	    QString query = e1.firstChildElement ( "QUERY" ).toElement().text();
	    QString vinculos = e1.firstChildElement ( "VINCULOS" ).toElement().text();
	    if (tablename == blautoformlist) {

			BlAutoFormList *formulario = NULL;
			
			formulario  = genBlAutoFormList(blautoformlist) ;
			
			if (formulario) {
			  

				    /// Creamos el dialogo que contendra el BlAutoForm
				    QDialog *diag = new QDialog ( 0 );
				    diag->setModal ( true );
				    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
				    blCenterOnScreen ( diag );
				    
				    formulario->launch();
				    
				    diag->connect ( formulario, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

				    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
				    /// para que sea redimensionable y aparezca el titulo de la ventana.
				    QHBoxLayout *layout = new QHBoxLayout;
				    layout->addWidget ( formulario );
				    layout->setMargin ( 0 );
				    layout->setSpacing ( 0 );
				    diag->setLayout ( layout );
				    diag->setWindowTitle ( formulario->windowTitle() );

				    diag->exec();
				    if ( formulario->mui_list->dbValue(formulario->mui_list->dbFieldId()) != "" ) {
// 					busq->setId ( formulario->mui_list->dbValue(formulario->mui_list->dbFieldId()) );
					

					/// Lo primero que hacemos es tratar el query y hacer las sustituciones
					int pos =  0;

					/// Buscamos parametros en el query y los ponemos.
					QRegExp rx ( "\\[(\\w*)\\]" );
					QString tmp;
					while ( ( pos = rx.indexIn ( query, pos ) ) != -1 ) {
					    if ( formulario->mui_list->existsHeader ( rx.cap ( 1 ) ) ) {    
						query.replace ( pos, rx.matchedLength(), formulario->mui_list->dbValue ( rx.cap ( 1 ) ) );
						pos = 0;
					    } else {
						pos += rx.matchedLength();
					    }
					} // end while

					BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
					if ( !cur->eof() ) {
					    /// Tratamos la vinculacion.
					    
					    
					    QStringList listavinculos = vinculos.split(",");
					    for (int j1 = 0; j1 < listavinculos.size(); ++j1) {
						  
						  int posicion = listavinculos.at(j1).indexOf("=");
						  QString campo = listavinculos.at(j1).left(posicion);
						  QString valor = listavinculos.at(j1).right(listavinculos.at(j1).size() - posicion-1);
						  
						  /// Lo primero que hacemos es tratar el query y hacer las sustituciones
						  int pos =  0;

						  /// Buscamos parametros en el query y los ponemos.
						  QRegExp rx ( "\\[(\\w*)\\]" );
						  QString tmp;
						  while ( ( pos = rx.indexIn ( valor, pos ) ) != -1 ) {
						      // Comprobamos la existencia del campo referenciado. Si este no existe entonces no hacemos nada.
						      if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {    
							  valor.replace ( pos, rx.matchedLength(), cur->value ( rx.cap ( 1 ) ) );
							  pos = 0;
						      } else {
							  pos += rx.matchedLength();
						      }
						  } // end while
						  

						  sub->lineaact()->setDbValue(campo, valor);
					    } // end for

					} // end if
					
					delete cur;
				    } // end if
				    delete diag;

				    
			} // end if
	      
	    } // end if
	} // end if
    } // end for

}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    SubForm_AutoForm *subformods = new SubForm_AutoForm ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}



/// Miramos de poner los iconos del menu de subformularios
///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    BL_FUNC_DEBUG

    /// Buscamos BLSUBFORMSEARCHEDIT que se correspondan con el campo sobre el que hemos pulsado
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( CONFIG_DIR_CONFIG + QString("autoform_") + g_emp ->dbName() + "_spec.spc" );
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
 
    QDomNodeList autoformlists = docElem.elementsByTagName ( "SUBFORMSEARCHEDIT" );
    for ( int i = 0; i < autoformlists.count(); i++ ) {
        QDomNode autoform = autoformlists.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	    QString nomcampo = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
	    QString blautoformlist = e1.firstChildElement ( "BLAUTOFORMLIST" ).toElement().text();
	    QString query = e1.firstChildElement ( "QUERY" ).toElement().text();
	    QString vinculos = e1.firstChildElement ( "VINCULOS" ).toElement().text();

	    BlSubFormHeader *header = sub->header ( nomcampo );
	    if ( header ) {
		SubForm_AutoForm *subformods = new SubForm_AutoForm ( sub );
		
		QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
		if ( !m_hboxLayout1 ) {
		    m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
		    m_hboxLayout1->setSpacing (0 );
		    m_hboxLayout1->setMargin ( 0 );
		    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
		} // end if
		
		if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
		  
		  /// Buscamos el AutoForm que contenga la tabla
		  QDomNodeList autoforms = docElem.elementsByTagName ( "AUTOFORM" );
		  for ( int i = 0; i < autoforms.count(); i++ ) {
		      QDomNode autoform = autoforms.item ( i );
		      QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
		      if ( !e1.isNull() ) { /// the node was really an element.
			  QString fileicon = e1.firstChildElement ( "ICON" ).toElement().text();
			  QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
			  QString title = e1.firstChildElement ( "TITLE" ).toElement().text();
			  if (tablename == blautoformlist) {
			      QToolButton *sel = new QToolButton ( sub->mui_menusubform );
			      sel->setObjectName("new_" + tablename);
			      sel->setStatusTip ( _("Nuevo ") + title );
			      sel->setToolTip ( _("Nuevo ") +   title );
			      sel->setMinimumSize ( QSize ( 18, 18 ) );
			      sel->setIcon ( QIcon ( fileicon ) );
			      sel->setIconSize ( QSize ( 18, 18 ) );    
			      m_hboxLayout1->addWidget ( sel );
			      sel->connect (sel, SIGNAL(released()), subformods, SLOT(nuevoAlumno()));
			  } // end if
		      } // end if
		  } // end for


		  /// Buscamos el AutoForm que contenga la tabla
		  QDomNodeList autoformls = docElem.elementsByTagName ( "AUTOFORMLIST" );
		  for ( int i = 0; i < autoformls.count(); i++ ) {
		      QDomNode autoform = autoformls.item ( i );
		      QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
		      if ( !e1.isNull() ) { /// the node was really an element.
			  QString fileicon = e1.firstChildElement ( "ICON" ).toElement().text();
			  QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
			  QString title = e1.firstChildElement ( "TITLE" ).toElement().text();
			  if (tablename == blautoformlist) {
			      QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
			      sel1->setObjectName("list_" + tablename);
			      sel1->setStatusTip ( _("Seleccionar ") + title );
			      sel1->setToolTip ( _("Seleccionar ") + title );
			      sel1->setMinimumSize ( QSize ( 18, 18 ) );
			      sel1->setIcon ( QIcon ( fileicon ) );
			      sel1->setIconSize ( QSize ( 18, 18 ) );
			      m_hboxLayout1->addWidget ( sel1 );
			      sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(seleccionarAlumno()));
			  } // end if
		      } // end if
		  } // end for
		  
		} // end if
	    } // end if
	} // end if
    } // end for

    return 0;
}

BlAutoForm * genBlAutoForm( const QString &formname) {
  BL_FUNC_DEBUG

    BlAutoForm *formulario = NULL;
    
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( CONFIG_DIR_CONFIG + QString("autoform_") + g_emp ->dbName() + "_spec.spc" );
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
	    QString fileicon = e1.firstChildElement ( "ICON" ).toElement().text();
	    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
	    QString tableid = e1.firstChildElement ( "TABLEID" ).toElement().text();
	    
	    if ( tablename == formname) {

		/// Creamos el AutoForm, lo configuramos y lo presentamos.
		formulario = new BlAutoForm(g_emp, 0, 0, BL_EDIT_MODE, interfacefile);
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
			    combo->setAllowNull ( TRUE );
			    combo->setId ( "" );
			} // end if

			if (tipocampo == "BLSEARCHWIDGET") {
			    QString nombre= "mui_"+nomheader;
			    BlSearchWidget * search = formulario->findChild<BlSearchWidget *>(nombre);
			    QString label = e1.firstChildElement ( "LABEL" ).toElement().text();
			    QString tablename = e1.firstChildElement ( "TABLENAME" ).toElement().text();
			    QString fieldid = e1.firstChildElement ( "FIELDID" ).toElement().text();
			    
			    
			    // Para no liarla parda, ponemos provincias y asi no habrá pedatas de momento.
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
			subform->setInsert ( allowinsert == "TRUE" );
			subform->setDelete ( allowdelete == "TRUE" );
			subform->setSortingEnabled ( setsorting == "TRUE" );
			subform->inicializar();
			
		    } // end if
		} // end for
		
		formulario->launch();
	    } // end if
	} // end if
    } // end for

    return formulario;

}











