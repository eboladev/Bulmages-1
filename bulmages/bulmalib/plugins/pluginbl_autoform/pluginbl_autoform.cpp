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
//     QDomElement principal = docElem.firstChildElement ( "FICHA" );
//     /// Cogemos 
//     QString tablename = principal.firstChildElement ( "TABLENAME" ).toElement().text();
//     QString campoid = principal.firstChildElement ( "CAMPOID" ).toElement().text();

    
/*    QDomNodeList nodos = docElem.elementsByTagName ( "AUTOFORM" );
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
	    
	    /// Creamos el AutoForm, lo configuramos y lo presentamos.
	    BlAutoForm *formulario = new BlAutoForm(cmp, 0, 0, BL_EDIT_MODE, interfacefile);
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
	    formulario->launch();
	    cmp ->m_pWorkspace->addSubWindow ( formulario );
	    formulario->show();
	} // end if
    } // end for
    
*/

// =========================
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
// 	    formulario->setTitleName ( title );
	    
	    formulario->setObjectName(objname);
	    formulario->setWindowIcon(QIcon(fileicon));
	    formulario->label->setPixmap(QPixmap(fileicon));
	    
	    formulario->mui_list->setDbTableName(tablename);
	    formulario->mui_list->setDbFieldId(tableid);
	    
// ================ Tratamos los campos

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
// ================ Tratamos los campos

	    formulario->launch();
	    cmp ->m_pWorkspace->addSubWindow ( formulario );
	    formulario->show();
	} // end if
    } // end for
// ==========================
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
//     QDomElement principal = docElem.firstChildElement ( "FICHA" );
//     /// Cogemos 
//     QString tablename = principal.firstChildElement ( "TABLENAME" ).toElement().text();
//     QString campoid = principal.firstChildElement ( "CAMPOID" ).toElement().text();

    

// =========================
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
	    
	    if (tablename == busq->tableName()) {
	      
		/// Creamos el dialogo que contendra el BlAutoForm
		QDialog *diag = new QDialog ( 0 );
		diag->setModal ( true );
		diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
		blCenterOnScreen ( diag );
	      
	      
		/// Creamos el AutoForm, lo configuramos y lo presentamos.
		BlAutoFormList *formulario = new BlAutoFormList(g_emp, 0, 0, BL_SELECT_MODE, interfacefile);
		formulario->setWindowTitle(title);
		
		formulario->setObjectName(objname);
		formulario->setWindowIcon(QIcon(fileicon));
		formulario->label->setPixmap(QPixmap(fileicon));
		
		formulario->mui_list->setDbTableName(tablename);
		formulario->mui_list->setDbFieldId(tableid);
		
    // ================ Tratamos los campos

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
		
	    }// end if
	    
	} // end if
    } // end for

    return 0;

}








