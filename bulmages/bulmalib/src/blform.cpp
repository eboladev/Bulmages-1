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
#include "bluiloader.h"


#include "blform.h"
#include "blformlist.h"
#include "blformconfig.h"
#include "blcombobox.h"
#include "blplugins.h"
#include "blradiobutton.h"
#include "bldatesearch.h"
#include "blperiodicitycombobox.h"
#include "blmainwindow.h"
#include "blsearchwidget.h"
#include "blcombobox.h"

///
/**
\param parent
\param f
\param modo
**/
BlForm::BlForm ( QWidget *parent, Qt::WindowFlags f, edmode modo ) : BlWidget ( parent, f ), BlDbRecord ( NULL ), BlDialogChanges ( this )
{
    BL_FUNC_DEBUG

    /// Disparamos los plugins
    int res = g_plugins->run ( "BlForm_BlForm", this );
    if ( res != 0 ) {
        return;
    } // end if

    m_title = windowTitle();
    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( popMenu ( const QPoint & ) ) );
    m_modo = modo;
    m_firstLoad = true;
    dialogChanges_readValues();
}


/**  procedimiento de QtScript
Importa el objeto que se pasa como referencia al engine de script
Busca el archivo en el directorio de openreports que tenga el mismo nombre que la clase
Importa el script y lo lanza.

*/
void BlForm::blScript(QObject * obj) {
    BL_FUNC_DEBUG
    
    if (g_confpr->valueTrue(CONF_USE_QSCRIPT)) {
    
	QString fileName = g_confpr->value( CONF_DIR_OPENREPORTS ) + "blform_"+metaObject()->className()+".qs";
	QFile scriptFile1(fileName);
	if (scriptFile1.open(QIODevice::ReadOnly)) {
	      QTextStream stream(&scriptFile1);
	      QString contents = stream.readAll();
	      QScriptValue objectValue = m_myEngine.newQObject(this);
	      m_myEngine.globalObject().setProperty(metaObject()->className(), objectValue);
	      QScriptValue objectValue1 = m_myEngine.newQObject(mainCompany());
	      m_myEngine.globalObject().setProperty("MainCompany", objectValue1);
	      m_myEngine.importExtension("qt.core");
	      m_myEngine.importExtension("qt.gui"); 
	      m_myEngine.evaluate(contents);
	      scriptFile1.close();
	      if (m_myEngine.hasUncaughtException()) {
		blMsgInfo(m_myEngine.uncaughtException().toString());
	      } // end if
	} // end if
	
	
	fileName = g_confpr->value( CONF_DIR_OPENREPORTS ) + "blform"+".qs";
	scriptFile1.setFileName(fileName);
	if (scriptFile1.open(QIODevice::ReadOnly)) {
	      QTextStream stream(&scriptFile1);
	      QString contents = stream.readAll();
	      QScriptValue objectValue = m_myEngine.newQObject(this);
	      m_myEngine.globalObject().setProperty("BlForm", objectValue);
	      QScriptValue objectValue1 = m_myEngine.newQObject(mainCompany());
	      m_myEngine.globalObject().setProperty("MainCompany", objectValue1);
	      m_myEngine.importExtension("qt.core");
	      m_myEngine.importExtension("qt.gui"); 
	      m_myEngine.evaluate(contents);
	      scriptFile1.close();
	      if (m_myEngine.hasUncaughtException()) {
		blMsgInfo(m_myEngine.uncaughtException().toString());
	      } // end if
	} // end if
    
    } // end if
}

///
/**
\param emp
\param parent
\param f
\param modo
**/
BlForm::BlForm ( BlMainCompany *emp, QWidget *parent, Qt::WindowFlags f, edmode modo ) : BlWidget ( emp, parent, f ), BlDbRecord ( emp ), BlDialogChanges ( this )
{
    BL_FUNC_DEBUG
    
    /// Disparamos los plugins
    int res = g_plugins->run ( "BlForm_BlForm", this );
    if ( res != 0 ) {
        return;
    } // end if

    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( popMenu ( const QPoint & ) ) );
    m_modo = modo;
    m_firstLoad = true;
    dialogChanges_readValues();    
}


///
/**
**/
BlForm::~BlForm()
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlForm::~BlForm", 0, this->windowTitle() );
    g_plugins->run ( "BlForm_DesBlForm", this );
    removeWindow();
    
}


/// Junta todos los XML en uno solo dentro del .bulmages
int BlForm::mergeAllXML() {
   BL_FUNC_DEBUG
   QDir d(CONFIG_DIR_CONFIG);
   QString buff;
   d.setFilter( QDir::Files );
   QStringList filters;
   filters << objectName() + "_" + mainCompany()->dbName() + "_*.spc*";
   bool hayconfs = false;
   
   QList<QFileInfo> list = d.entryInfoList(filters);
   QList<QFileInfo>::iterator it;
   for (it = list.begin(); it != list.end(); ++it) {
	hayconfs = true;
        QFile fich(it->absoluteFilePath ());
        if (!fich.open ( QIODevice::ReadOnly )) {
              blMsgInfo("Error al abrir el archivo de autoformularios");
             return 0;
        } // end if
        buff += fich.readAll();
	fich.close();
   } // end for

   if (hayconfs) {
      QFile file1(g_confpr->value(CONF_DIR_USER) + objectName() + "_" + mainCompany()->dbName() + "_mautoform_spec.spc");
      if ( file1.open ( QIODevice::WriteOnly ) ) {
	    QTextStream stream ( &file1 );
	    stream <<  buff;
      } // end if
      file1.close();
   } // end if
   
   return 0;
}


///
/**
**/
void BlForm::loadSpecs()
{
    BL_FUNC_DEBUG
    
    if (m_firstLoad) {
        mergeAllXML();
	m_firstLoad = false;
    } else {
        return;
    } // end if
    
        /// Disparamos los plugins
    int res1 = g_plugins->run ( "BlForm_loadSpecs", this );
    if ( res1 != 0 ) {
        
        return;
    } // end if
    
    QFile file ( g_confpr->value(CONF_DIR_USER) + objectName() + "_" + mainCompany()->dbName() + "_mautoform_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        return;
    } // end if
    if ( !doc.setContent ( &file ) ) {
        file.close();
        return;
    } // end if
    file.close();

    QDomElement docElem = doc.documentElement();
    
    
    QDomNodeList tabnodos = docElem.elementsByTagName ( "TAB" );
    for ( int i = 0; i < tabnodos.count(); i++ ) {
        QDomNode ventana = tabnodos.item ( i );
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
	   QString name = e1.firstChildElement ( "NAME" ).toElement().text();
           QString interfacefile = e1.firstChildElement ( "UI_INTERFACE" ).toElement().text();
            QTabWidget *tab =  findChild<QTabWidget *>();
	    if (tab) {
	      BlUiLoader loader(mainCompany());
	      QFile file(interfacefile);
	      file.open(QFile::ReadOnly);
	      QWidget *myWidget = loader.load(&file, this);
	      tab->insertTab(-1,myWidget,name);
	    } else {
	      blMsgInfo("Se ha encontrado una especificacion TAB para una ficha que no posee QTabWidget");
	    } // end if
		QDomNodeList nodos = e1.elementsByTagName ( "CAMPO" );
		for ( int j = 0; j < nodos.count(); j++ ) {
		    QDomNode ventana = nodos.item ( j );
		    QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
		    if ( !e1.isNull() ) { /// the node was really an element.
			BlDbField::DbType type = BlDbField::DbVarChar;
			QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
			if ( exists ( nomheader ) ) return ;
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

			addDbField ( nomheader, type, ( BlDbField::DbRestrict ) restricciones, nompheader );
			
			
			QString tipocampo = e1.firstChildElement ( "TIPOCAMPO" ).toElement().text();
			if (tipocampo == "BLCOMBOBOX") {
			    QString nombre= "mui_"+nomheader;
			    BlComboBox * combo = findChild<BlComboBox *>(nombre);
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
			    BlSearchWidget * search = findChild<BlSearchWidget *>(nombre);
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
		
		
		/// Cargamos los subformularios
		QDomNodeList nodoss = e1.elementsByTagName ( "SUBFORM" );
		for ( int j = 0; j < nodoss.count(); j++ ) {
		    QDomNode ventana = nodoss.item ( j );
		    QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
		    if ( !e1.isNull() ) { /// the node was really an element.
			BlDbField::DbType type = BlDbField::DbVarChar;
			QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
			if ( exists ( nomheader ) ) return;
			QString nompheader = e1.firstChildElement ( "NOMPCAMPO" ).toElement().text();
			BlSubForm * subform = findChild<BlSubForm *>("mui_" + nomheader);
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
			subform->setInsert ( allowinsert == "true" );
			subform->setDelete ( allowdelete == "true" );
			subform->setSortingEnabled ( setsorting == "true" );
			subform->inicializar();
			
		    } // end if
		} // end for
	} // end if
    } // end for
    
    
    QDomElement principal = docElem.firstChildElement ( "FICHA" );
    /// Cogemos la coordenada X
    QString tablename = principal.firstChildElement ( "TABLENAME" ).toElement().text();
    QString campoid = principal.firstChildElement ( "CAMPOID" ).toElement().text();

    QDomNodeList nodos = docElem.elementsByTagName ( "CAMPO" );
    for ( int i = 0; i < nodos.count(); i++ ) {
        QDomNode ventana = nodos.item ( i );
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
            BlDbField::DbType type = BlDbField::DbVarChar;
            QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
            if ( exists ( nomheader ) ) return;
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

            addDbField ( nomheader, type, ( BlDbField::DbRestrict ) restricciones, nompheader );
	    generaCampo ( nomheader, nompheader, typeheader, e1 );
        } // end if
    } // end for

    /// Disparamos los plugins
    int res = g_plugins->run ( "BlForm_loadSpecs_Post", this );

    
}


void BlForm::generaCampo ( const QString &objname, const QString &textname, const QString &type, const QDomElement & e1 )
{

    /// Miramos si existe un menu Herramientas
    QFrame * frame = findChild<QFrame *> ( "m_frameplugin" );
    /// Si no hay un frame con lo que buscamos buscamos cualquier frame
    if ( !frame ) {
        frame = findChild<QFrame *> ( "m_frameplugin" );
    } // end if
    if ( !frame ) return;

    QVBoxLayout *vboxl = frame->findChild<QVBoxLayout *>();
    if ( !vboxl ) {
        vboxl = new QVBoxLayout ( frame );
        vboxl->setSpacing ( 0 );
        vboxl->setMargin ( 0 );
        vboxl->setObjectName ( QString::fromUtf8 ( "m_framevboxlayout" ) );
    } // end if

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout16" ) );

    
    	    
	    /// Si es un BlSearchWidget o un BlComboBox hacemos un tratamiento especial. Sino hacemos un tratamiento generico
	    QString tipocampo = e1.firstChildElement ( "TIPOCAMPO" ).toElement().text();
	    if (tipocampo == "BLSEARCHWIDGET" ) {
		  BlSearchWidget * search = new BlSearchWidget(frame);
		  search->setMainCompany(mainCompany());
		  search->setObjectName("mui_" + objname);
		  search->setMinimumHeight(100);
// 		  QString label = e1.firstChildElement ( "LABEL" ).toElement().text();
		  QString label = textname;
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
		  hboxLayout160->addWidget ( search );
	    } else if (tipocampo == "BLCOMBOBOX") {
		  QLabel *textLabel2_9_26 = new QLabel ( frame );
		  textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
		  hboxLayout160->addWidget ( textLabel2_9_26 );
		  textLabel2_9_26->setText ( textname );
		      
		  BlComboBox * combo = new BlComboBox(frame);
		  combo->setMainCompany(mainCompany());
		  combo->setObjectName("mui_" + objname);
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
		  hboxLayout160->addWidget ( combo );	      
	    } else {
    
		      QLabel *textLabel2_9_26 = new QLabel ( frame );
		      textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
		      hboxLayout160->addWidget ( textLabel2_9_26 );
		      textLabel2_9_26->setText ( textname );

		      if ( type == "DBDATE" ) {
			  BlDateSearch * bus = new BlDateSearch ( frame );
			  bus->setObjectName ( "mui_" + objname );
			  hboxLayout160->addWidget ( bus );
		      } else {
			  QLineEdit *bus = new QLineEdit ( frame );
			  bus->setObjectName ( "mui_" + objname );
			  hboxLayout160->addWidget ( bus );
		      } // end if
	    } // end if
    vboxl->addLayout ( hboxLayout160 );
}

///
/**
**/
void BlForm::setSelectMode()
{
    BL_FUNC_DEBUG
    m_modo = BL_SELECT_MODE;
    
}


///
/**
**/
void BlForm::setEditMode()
{
    BL_FUNC_DEBUG
    m_modo = BL_EDIT_MODE;
    
}


///
/**
\return
**/
bool BlForm::editMode()
{
    BL_FUNC_DEBUG
    
    return m_modo == BL_EDIT_MODE;
}


///
/**
\return
**/
bool BlForm::selectMode()
{
    BL_FUNC_DEBUG
    
    return m_modo == BL_SELECT_MODE;
}


///
/**
**/
void BlForm::on_mui_cancelar_clicked()
{
    BL_FUNC_DEBUG
    close();
    
}


///
/**
**/
void BlForm::on_mui_guardar_clicked()
{
    BL_FUNC_DEBUG
    save();
    
}


///
/**
**/
void BlForm::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    try {
        if ( save() ) {
            throw - 1;
        } // end if
        close();
    } catch ( ... ) {
        BlDebug::blDebug ( "Error al guardar la ficha", 2 );
    } // end try
    
}


///
/**
**/
void BlForm::on_mui_imprimir_clicked()
{
    BL_FUNC_DEBUG
    imprimir();
    
}


///
/**
**/
void BlForm::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG

    int val = QMessageBox::question ( this,
                                      _ ( "Borrar" ) + " " + windowTitle(),
                                      _ ( "Desea eliminar '" ) + " " + windowTitle() + "' ?",
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default );

    if ( val == QMessageBox::Yes ) {
        if ( !remove() ) {
            dialogChanges_readValues();
            BlDebug::blDebug ( windowTitle() + " " + "borrado satisfactoriamente.", 10 );
            close();
        } else {
            blMsgInfo ( windowTitle() + _ ( "No se ha podido borrar" ) );
        }// end if
    } // end if
    
}


///
/**
\param e
\return
**/
void BlForm::closeEvent ( QCloseEvent *e )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlForm::closeEvent", 0, windowTitle() );

    try {
	  /// Si el bot&oacute;n est&aacute; oculto es por no tener permiso para guardar
	  QPushButton * p = findChild<QPushButton *> ( "mui_aceptar" );

	  if ( ( !p || !p->isHidden() )
	  && ( dialogChanges_isChanged() ) ) {
            int val = QMessageBox::warning ( this,
                                             _ ( "Guardar" ) + " " + windowTitle(),
                                             _ ( "Desea guardar los cambios?" ),
                                             _ ( "&Si" ), _ ( "&No" ), _ ( "&Cancelar" ), 0, 2 );
            if ( val == 0 ) {
                save();
            } // end if
            if ( val == 2 ) {
                e->ignore();
                return;
            } // end if
        } // end if
        e->accept();
    } catch ( ... ) {
        blMsgInfo ( _ ( "No se pudo cerrar la ventana debido a un error" ) );
        e->ignore();
    } // end try
    
}


///
/**
\return
**/
int BlForm::removeWindow()
{
    BL_FUNC_DEBUG
    if ( mainCompany() != NULL ) {
        mainCompany() ->removeWindow ( this );
    } // end if
    
    return 0;
}


///
/**
\param nom
\param obj
\param compdup
**/
void BlForm::insertWindow ( QString nom, QObject *obj, bool compdup, QString titulo )
{
    BL_FUNC_DEBUG
    if ( mainCompany() != NULL ) {
        mainCompany() ->insertWindow ( nom, obj, compdup, titulo );
	/// De Forma rapida hacemos un tratamiento de los permisos
	setDbTableName ( tableName() );
	/// Tal vez no es el mejor sitio para hacer la carga de SPECS. Pero no hay llamada especifica
	/// De configuracion por lo que si no es este no es ninguno.
	loadSpecs();
    } // end if 
}


///
/**
**/
void BlForm::popMenu ( const QPoint & )
{
    BL_FUNC_DEBUG
    

    
    /// Creamos la instancia del menu.
    QMenu *popup = new QMenu ( this );
    
    /// Si estamos en modo experto. Lo primero que hacemos es encabezar el menu con el nombre del objeto para tenerlo bien ubicado.
    if (g_confpr->valueTrue(CONF_MODO_EXPERTO)) {
      QAction *nombreobjeto = popup->addAction( objectName() );
      nombreobjeto->setDisabled(true);
      QAction *claseobjeto = popup->addAction( metaObject()->className() );
      claseobjeto->setDisabled(true);
    } // end if
    
    /// Lanzamos el evento para que pueda ser capturado por terceros. Y pongan sus opciones.
    emit pintaMenu ( popup );

    /// Lanzamos la propagacion del menu a traves de las clases derivadas.
    createMenu ( popup );
    QAction *avconfig = NULL;
    if (g_confpr->valueTrue(CONF_MODO_EXPERTO)) {
      avconfig = popup->addAction ( _ ( "Opciones avanzadas de ficha" ) );
    } // end if

    QAction *avprint = popup->addAction ( _ ( "Imprimir ficha" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );

    if ( opcion ) {
        if ( opcion == avconfig ) {
            new BlFormConfig ( mainCompany(), this, 0 );
        } else if ( opcion == avprint ) {
            BlForm::imprimir();
        } // end if

        emit trataMenu ( opcion );

        /// Activamos las herederas.
        execMenuAction ( opcion );
    } // end if

    delete popup;
    
}


///
/**
**/
void BlForm::createMenu ( QMenu * )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlForm:: createMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
**/
void BlForm::execMenuAction ( QAction * )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlForm:: execMenuAction", 0, "funcion para ser sobreescrita" );
}


/// Set the table to use in the form and configure the standard buttons depending on the privileges on it.
/**
\param nomtabla
**/
void BlForm::setDbTableName ( QString nomtabla )
{
    BL_FUNC_DEBUG
    BlDbRecord::setDbTableName ( nomtabla );

    if ( mainCompany() != NULL ) {
       QAbstractButton * pbut = NULL; /// Puntero para buscar y manipular botones
       /// Buscamos los permisos que tiene el usuario y desactivamos botones.
       if ( !mainCompany() ->hasTablePrivilege ( nomtabla, "INSERT" )
         && !mainCompany() ->hasTablePrivilege ( nomtabla, "UPDATE" ) ) {
          pbut = findChild<QAbstractButton *> ( "mui_guardar" );
          if ( pbut ) pbut->setDisabled ( true );
          pbut = findChild<QAbstractButton *> ( "mui_aceptar" );
          if ( pbut ) pbut->hide();
          pbut = findChild<QAbstractButton *> ( "mui_cancelar" );
          if ( pbut ) pbut->setText("Cerrar");
       } // end if

       if ( !mainCompany() ->hasTablePrivilege ( nomtabla, "DELETE" ) ) {
          pbut = findChild<QAbstractButton *> ( "mui_borrar" );
          if ( pbut ) pbut->setDisabled ( true );
       } // end if
    } // end if
}


///
/**
\param nom
**/
void BlForm::setTitleName ( QString nom )
{
    BL_FUNC_DEBUG
    m_title = nom;
    
}


///
/**
**/
void BlForm::pintar()
{
    BL_FUNC_DEBUG

    bool readOnly = !mainCompany() ->hasTablePrivilege ( tableName(), "UPDATE" );

    BlDbField *campo;
    /// Recorremos todos los campos definidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        /// Buscamos los QLineEdit con nombre coincidente.
        QLineEdit *l1 = findChild<QLineEdit *> ( "mui_" + campo->fieldName() );
        if ( l1 ) {
            l1->setText ( campo->fieldValue() );
		if ( readOnly ) l1->setReadOnly ( true );
        } // end if
        /// Buscamos los QPlainTextEdit con nombre coincidente.
        QPlainTextEdit *l2 = findChild<QPlainTextEdit *> ( "mui_" + campo->fieldName() );
        if ( l2 ) {
            l2->setPlainText( campo->fieldValue() );
		if ( readOnly ) l2->setReadOnly ( true );
	   } // end if
        /// Buscamos los QTextEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *> ( "mui_" + campo->fieldName() );
        if ( l3 ) {
            l3->setText ( campo->fieldValue() );
		if ( readOnly ) l3->setReadOnly ( true );
        } // end if
        /// Buscamos BlWidgets que coincidan con el campo supuestamente
        /// sirve para los campos personales.
        BlWidget *l4 = findChild<BlWidget *> ( "mui_" + campo->fieldName() );
        if ( l4 ) {
            l4->setFieldValue ( campo->fieldValue() );
		if ( readOnly ) l4->setDisabled( true );
        } // end if

        /// Buscamos BlComboBox que coincidan con el campo supuestamente
        /// sirve para los campos personales.
        BlPeriodicityComboBox *l5 = findChild<BlPeriodicityComboBox *> ( "mui_" + campo->fieldName() );
        if ( l5 ) {
            l5->setperiodo ( campo->fieldValue() );
        } else {
            BlComboBox *l6 = findChild<BlComboBox *> ( "mui_" + campo->fieldName() );
            if ( l6 ) {
                l6->setFieldValue ( campo->fieldValue() );
		    if ( readOnly ) l6->setDisabled( true );
            } else {
                /// Buscamos BlComboBox que coincidan con el campo supuestamente
                /// sirve para los campos personales.
                QComboBox *l7 = findChild<QComboBox *> ( "mui_" + campo->fieldName() );
                if ( l7 ) {
                    l7->setCurrentIndex ( l7->findText ( campo->fieldValue() ) );
                } // end if
            } // end if
        } // end if

        /// Buscamos un BlDoubleSpinBox con nombre coincidente.
        BlDoubleSpinBox *l8 = findChild<BlDoubleSpinBox *> ( "mui_" + campo->fieldName() );
        if ( l8 ) {
            l8->setValue ( campo->fieldValue().replace(",",".").toDouble() );
		if ( readOnly ) l8->setDisabled( true );
            /// Buscamos los decimales que tiene el campo y establecemos el numero de decimales a ese valor.
            QString query2 = "SELECT numeric_scale FROM information_schema.columns WHERE table_name = '"+tableName()+"' and column_name='"+campo->fieldName()+"';";
            /// Cargamos el query y lo recorremos
            BlDbRecordSet *cur = mainCompany() ->loadQuery ( query2 );
            if ( !cur->eof() ) {
                l8->setDecimals(cur->value("numeric_scale").toInt());
            } // end if
            delete cur;

        } // end if


        /// Buscamos los QCheckBox con nombre coincidente.
        QCheckBox *l9 = findChild<QCheckBox *> ( "mui_" + campo->fieldName() );
        if ( l9 ) {
            if ( campo->fieldValue() == "t" ) {
                l9->setCheckState( Qt::Checked );
            } else if ( campo->fieldValue() == "f" ) {
                l9->setCheckState( Qt::Unchecked );
            } else if ( l9->isTristate() ) {
                /// El estado indeterminado se aplica cuando el campo es triestado.
                l9->setCheckState( Qt::PartiallyChecked );
            } // end if
		if ( readOnly ) l9->setDisabled( true );
	   } // end if

        /// Buscamos los 'Radio Buttons' y los preparamos.
        QList<BlRadioButton *> l10 = findChildren<BlRadioButton *> ( QRegExp ( "mui_" + campo->fieldName() + "_*" ) );
        for ( int i = 0; i < l10.size(); ++i ) {
            if ( l10.at ( i ) ->fieldValue() == campo->fieldValue() ) {
                l10.at ( i ) ->setChecked ( true );
            } else {
                l10.at ( i ) ->setChecked ( false );
            } // end if
		if ( readOnly ) l10.at ( i ) ->setDisabled( true );
        } // end for
    } // end for
    pintarPost();
}


/** Recoge de forma automatica los valores que tienen los 'widgets' y los pone en la
    estructura de BlDbRecord.
    Para que funcionen bien los 'widgets' deben tener como nombre el mismo campo que
    el correspondiente en la base de datos precedidos de mui_
*/
/**
**/
void BlForm::recogeValores()
{
    BL_FUNC_DEBUG
    BlDbField *campo;
    /// Recorremos todos los campos definidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        /// Buscamos un QLineEdit con nombre coincidente.
        QLineEdit *l1 = findChild<QLineEdit *> ( "mui_" + campo->fieldName() );
        if ( l1 )
            campo->set ( l1->text() );

        /// Buscamos un QPlainTextEdit con nombre coincidente.
        QPlainTextEdit *l2 = findChild<QPlainTextEdit *> ( "mui_" + campo->fieldName() );
        if ( l2 )
            campo->set ( l2->toPlainText() );

        /// Buscamos un QTextEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *> ( "mui_" + campo->fieldName() );
        if ( l3 )
            campo->set ( l3->toPlainText() );

        /// Buscamos BlWidgets que coincidan con el campo. Supuestamente sirve para los campos personales.
        BlWidget *l4 = findChild<BlWidget *> ( "mui_" + campo->fieldName() );
        if ( l4 )
            campo->set ( l4->fieldValue() );

        /// Buscamos BlComboBox que coincidan con el campo. Supuestamente sirve para los campos personales.
        BlPeriodicityComboBox *l5 = findChild<BlPeriodicityComboBox *> ( "mui_" + campo->fieldName() );
        if ( l5 ) {
            campo->set(l5->periodo() );
        } else {
            BlComboBox *l6 = findChild<BlComboBox *> ( "mui_" + campo->fieldName() );
            if ( l6 ) {
                campo->set ( l6->fieldValue() );
            } else {
                /// Buscamos QComboBox que coincidan con el campo. Supuestamente sirve para los campos personales.
                QComboBox *l7 = findChild<QComboBox *> ( "mui_" + campo->fieldName() );
                if ( l7 ) {
                    campo->set ( l7->currentText() );
                } // end if
            } // end if
        } // end if

        /// Buscamos un BlDoubleSpinBox con nombre coincidente.
        BlDoubleSpinBox *l8 = findChild<BlDoubleSpinBox *> ( "mui_" + campo->fieldName() );
        if ( l8 )
            campo->set ( QString::number(l8->value()) );

        /// Buscamos un QCheckBox con nombre coincidente.
        QCheckBox *l9 = findChild<QCheckBox *> ( "mui_" + campo->fieldName() );
        if ( l9 ) {
            switch ( l9->checkState() ) {
                case Qt::Checked:
                    campo->set ( "true" );
                    break;
                case Qt::Unchecked:
                    campo->set ( "false" );
                    break;
                case Qt::PartiallyChecked:
                default:
                    /// Esta opci&oacute;n se usa si el campo SQL admite el valor nulo y por tanto tenemos un CheckBox triestado
                    campo->set ( "" );
                    break;
            } // end switch
        } // end if

        /// Buscamos los 'Radio Buttons' y los preparamos.
        QList<BlRadioButton *> l10 = findChildren<BlRadioButton *> ( QRegExp ( "mui_" + campo->fieldName() + "_*" ) );
        if ( l10.size() > 0 ) {
            int aux = 0;
            for ( int i = 0; i < l10.size(); ++i ) {
                if ( l10.at ( i ) ->isChecked() ) {
                    campo->set ( l10.at ( i ) ->fieldValue() );
                    aux = 1;
                } // end if
            } // end for
            if ( aux == 0 ) {
                campo->set ( "" );
            } // end if
        } // end if

    } // end for
    
}


///
/**
\param id
\return
**/
int BlForm::load ( QString id, bool paint )
{
    BL_FUNC_DEBUG
    try {
        if ( BlDbRecord::load ( id ) ) {
            throw - 1;
        } // end if
        /// Lanzamos los plugins.
        if ( g_plugins->run ( "BlForm_load", this ) ) return 0;
        cargarPost ( id );
	/// Cargamos los subformularios de specificaciones
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file ( CONFIG_DIR_CONFIG + objectName() + "_" + mainCompany() ->dbName() + "_spec.spc");
    QDomDocument doc ( "mydocument" );
    if ( file.open ( QIODevice::ReadOnly ) ) {


    if ( !doc.setContent ( &file ) ) {
        file.close();
	blMsgInfo("Error al parsear el archivo de especificaciones");
        return 0;
    } // end if
    file.close();
    
    /// Iteramos para cada AutoForm y lo creamos haciendo todo lo necesario para que este funcione.
    QDomElement docElem = doc.documentElement();
    
    QDomNodeList nodos = docElem.elementsByTagName ( "TAB" );
    for ( int i = 0; i < nodos.count(); i++ ) {
        QDomNode autoform = nodos.item ( i );
        QDomElement e1 = autoform.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.

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
    } // end form
    
    
    
        } // end if
	/// Buscamos un titulo adecuado segun los valores que contenga la tabla.
	QString wtitle = m_title + " ";
	if (exists("num" + tableName())) {
	  wtitle = wtitle + "[" + dbValue ( "num" + tableName()) + "]";
	} else	if ( exists ("ref" + tableName())) {
	  wtitle = wtitle + "["+ dbValue ("ref" + tableName()) + "]";
	} else if ( exists ( "cif" + tableName())) {
	  wtitle = wtitle + "[" + dbValue( "cif" + tableName()) + "]";
	} else if ( exists ( "nom" + tableName())) {
	  wtitle = wtitle + "[" +  dbValue( "nom" + tableName()) + "]";
	} else if ( exists ( "cod" + tableName())) {
	  wtitle = wtitle + "[" + dbValue ( "cod" + tableName()) + "]";
	} else {
	   wtitle = wtitle + dbValue ( m_campoid );
	} // end if

        setWindowTitle ( wtitle );
        /// Activamos documentos adicionales
        activateDocuments();
	
        if ( paint == true ) {
            pintar();
        } // end if
	
        dialogChanges_readValues();
        insertWindow ( m_title + dbValue(m_campoid), this, true, wtitle );
    } catch ( ... ) {
        
        return -1;
    } // end try
    
    return 0;
}


/// Guarda los datos de la ficha en la base de datos.
/**
  Este metodo guarda los contenidos de la Ficha (siempre que esta haya sido inicializada).
  Luego llama a plugins y a afterSave por si se quieren hacer acciones adicionales de guardado.
  Tras guardar todos los elementos hace una carga.
\return 0 Si no hay problemas. -1 Si ha habido problemas.
**/
int BlForm::save()
{
    BL_FUNC_DEBUG

    try {
        QString id;
        recogeValores();
        mainCompany() ->begin();

        beforeSave();

        dbSave ( id );
        setDbValue ( m_campoid, id );

        /// Lanzamos los plugins.
        if ( g_plugins->run ( "BlForm_guardar_Post", this ) ) return 0;

        afterSave();

	        /// Lanzamos los plugins.
        if ( g_plugins->run ( "BlForm_afterSave_Post", this ) ) return 0;
	
	
	mainCompany() ->commit();

        /// Hacemos una carga para que se actualizen datos como la referencia.
        load ( id );

        /// Si la directiva CONF_REFRESH_LIST esta activada buscamos el listado referente y lo recargamos
        if (g_confpr->valueTrue(CONF_REFRESH_LIST)) {
                /// Buscamos el listado que corresponde al widget.
                QList<BlFormList *> lista = g_main->findChildren<BlFormList *>();
                for (int i = 0; i < lista.size(); ++i) {
                    /// Puede darse el caso de que un listado no tenga asignado subform y haga petar el programa
                    if (((BlFormList *)lista.at(i))->subForm()) {
                       if (((BlFormList *)lista.at(i))->subForm()->tableName() == tableName())
                           lista.at(i)->presentar();
                    } else {
                       blMsgInfo("No se establecio SubForm para el formulario " + ((BlFormList *)lista.at(i))->objectName());
                    } // end if
                } // end for
        } // end if

        
        return 0;
    } catch ( int valor ) {
        /// Valor del error diferente a -1 significa que ya se ha mostrado algun mensaje de
        /// error y no hay que mostrar otro.
        if ( valor == -1 ) {
	  blMsgInfo (_ ( "Error inesperado al guardar" ));
        } // end if
        mainCompany() ->rollback();
        return -1;

    } catch ( ... ) {
        blMsgInfo (_ ( "Error inesperado al guardar" ));
        mainCompany() ->rollback();
        return -1;
    } // end try
}


int BlForm::beforeDelete()
{
    BL_FUNC_DEBUG
    
    return 0;
}


/** Borra la ficha en la base de datos
    Este metodo puede ser invocado desde la clase ficha o desde la pantalla previa
*/
/**
\return
**/
int BlForm::remove()
{
    BL_FUNC_DEBUG
    try {
        mainCompany()->begin();
        /// Lanzamos los plugins.
        if ( g_plugins->run ( "BlForm_borrar", this ) ) return 0;
        beforeDelete();
        int err;
        err =  BlDbRecord::remove();

        /// Si la directiva CONF_REFRESH_LIST esta activada buscamos el listado referente y lo recargamos
        if (g_confpr->valueTrue(CONF_REFRESH_LIST)) {
                /// Buscamos el listado que corresponde al widget.
                QList<BlFormList *> lista = g_main->findChildren<BlFormList *>();
                for (int i = 0; i < lista.size(); ++i) {
                    if (((BlFormList *)lista.at(i))->subForm()->tableName() == tableName())
                        lista.at(i)->presentar();
                } // end for
        } // end if
	mainCompany()->commit();
        
        return err;
    } catch ( ... ) {
        mainCompany()->rollback();
        return -1;
    } // end try
}


///
/**
\return
**/
int BlForm::beforeSave()
{
    BL_FUNC_DEBUG
    
    return 0;
}


///
/**
\return
**/
int BlForm::afterSave()
{
    BL_FUNC_DEBUG
    
    
    /// Guardamos datos de los subformularios de especificaciones
    
    /// Cargamos el XML de descripcion de autoforms y lo procesamos.
    QFile file (  CONFIG_DIR_CONFIG + objectName() + "_" + mainCompany() ->dbName() + "_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( file.open ( QIODevice::ReadOnly ) ) {
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
    } // end for
    
    } // end if

    
    return 0;
}


///
/**
\return
**/
int BlForm::cargarPost ( QString )
{
    BL_FUNC_DEBUG

    return 0;
}

///
/**
\return
**/
void BlForm::pintarPost()
{
    BL_FUNC_DEBUG
    
}



/// Permite que el programa introduzca variables de impresion propias sin tener 
/// Que introducir datos en el registro de base de datos de ficha.
void BlForm::setVar(const QString &varname, const QString &varvalue) {
      BL_FUNC_DEBUG
          m_globalvars[varname ] = varvalue;
}

/// Permite que el programa introduzca variables de impresion propias sin tener 
/// Que introducir datos en el registro de base de datos de ficha.
void BlForm::clearVars() {
      BL_FUNC_DEBUG
          m_globalvars.clear();
}



/// Sustituye valores en el texto pasado como variables por su valor.
/// tipoEscape puede ser 0 --> Sin parseo
///    1 --> ParseoXML
///    2 --> ParseoPython
void BlForm::substrVars ( QString &buff, int tipoEscape )
{
    BL_FUNC_DEBUG
    
    /// Tratamos la sustitucion de variables de m_variables
    int pos = 0;
    QMapIterator<QString, QString> i ( m_variables );
    while ( i.hasNext() ) {
        i.next();
        buff.replace ( "[" + i.key() + "]", i.value() );
    } // end while

    /// Tratamos la sustitucion de variables de m_variables
    QMapIterator<QString, QString> j ( m_globalvars );
    while ( j.hasNext() ) {
        j.next();
        buff.replace ( "[" + j.key() + "]", j.value() );
    } // end while

    substrConf ( buff );

    
    /// Buscamos parametros en el query y los ponemos de forma literal.
    pos =  0;
    QRegExp rx1 ( "\\[(\\w*),l\\]" );
    while ( ( pos = rx1.indexIn ( buff, pos ) ) != -1 ) {
        if ( exists ( rx1.cap ( 1 ) ) ) {
            buff.replace ( pos, rx1.matchedLength(), dbValue ( rx1.cap ( 1 ) ) );
            pos = 0;
        } else {
            pos += rx1.matchedLength();
        }
    } // end while
    
    
    /// Buscamos parametros en el query y los ponemos.
    pos =  0;
    QRegExp rx ( "\\[(\\w*)\\]" );
    while ( ( pos = rx.indexIn ( buff, pos ) ) != -1 ) {
        if ( exists ( rx.cap ( 1 ) ) ) {
                        
            switch ( tipoEscape ) {
            case 1:
                buff.replace ( pos, rx.matchedLength(), blXMLEscape ( dbValue ( rx.cap ( 1 ) ) ) );
                break;
            case 2:
                buff.replace ( pos, rx.matchedLength(), blPythonEscape ( dbValue ( rx.cap ( 1 ) ) ) );
                break;
            default:
                buff.replace ( pos, rx.matchedLength(), dbValue ( rx.cap ( 1 ) ) );
            } // end switch

            pos = 0;
        } else {
            pos += rx.matchedLength();
        }
    } // end while

    /// Buscamos elementos existentes en el formulario / subformulario y los sustituimos
    /// Buscamos parametros en el query y los ponemos.
    /// NOTE: Esta sustitucion solo tiene sentido hacerse una vez en todo el informe.
    pos = 0;
    QRegExp rx5 ( "\\[(\\w*)\\]" );
    while ( ( pos = rx5.indexIn ( buff, pos ) ) != -1 ) {
	   QString valor = "";
            BlComboBox *combo = findChild <BlComboBox *>(rx5.cap( 1 ));
	   if (combo) {
	     valor = combo->id();
	   } // end if
	   QComboBox *combo1 = findChild <QComboBox *>(rx5.cap( 1 ));
	   if (combo1) {
		valor = combo1->currentText();
	   } // end if
            
	  QPlainTextEdit *l5 = findChild<QPlainTextEdit *>(rx5.cap( 1 ));
	  if ( l5 ) {
	      valor = l5->toPlainText();
	  } // end if

	  QTextEdit * l1 = findChild<QTextEdit *>(rx5.cap( 1 ));
	  if ( l1 ) {
	      valor = l1->toPlainText();
	  } // end if

	  QLineEdit * l2 = findChild<QLineEdit *>(rx5.cap( 1 ));
	  if ( l2 ) {
	      valor = l2->text();
	  } // end if

	  QCheckBox  *l6 = findChild<QCheckBox *>(rx5.cap( 1 ));
	  if ( l6 ) {
	      if (l6->isChecked()) {
		  valor = "true";
	      } else {
		  valor = "false";
	      } // end if
	  } // end if

	  BlDateSearch * l7 = findChild<BlDateSearch *>(rx5.cap( 1 ));
	  if ( l7 ) {
		valor = l7->text();
	  } // end if
	  
	  BlSearchWidget  *l8 = findChild<BlSearchWidget *>(rx5.cap( 1 ));
	  if ( l8 ) {
		  valor = l8->id();
	  } // end if
	  
	  
	   if ( valor != "") {
	      switch ( tipoEscape ) {
	      case 1:
		  buff.replace ( pos, rx5.matchedLength(), blXMLEscape ( valor ) );
		  break;
	      case 2:
		  buff.replace ( pos, rx5.matchedLength(), blPythonEscape ( valor ) );
		  break;
	      default:
		  buff.replace ( pos, rx5.matchedLength(),  valor  );

	      } // end switch
	      pos = 0;
	   } else {
	         pos += rx5.matchedLength();
	   } // end if
	   

    } // end while
    
    
}


/// Busca strings del tipo [xxxx] dentro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/// Tambien busca los parametros PARAM e IFACE para tambien tratarlos.
/// Devuelve 1 Si todo esta OK o 0 Si hay algun error
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
int BlForm::parseTags ( QString &buff, int tipoEscape )
{
    BL_FUNC_DEBUG

    ///Buscamos interfaces, los preguntamos y los ponemos.
    int pos = 0;
    
    buff.replace("@!--", "<!--");
    buff.replace("--@", "-->");
    
    QRegExp rx70 ( "<!--\\s*IFACE\\s*SRC\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx70.setMinimal ( true );
    while ( ( pos = rx70.indexIn ( buff, pos ) ) != -1 ) {
        QString cadarchivo = rx70.cap ( 1 );

        substrConf ( cadarchivo );

        QFile fichero ( cadarchivo );
        if ( fichero.exists() ) {
            BlUiLoader loader(mainCompany());
            fichero.open ( QFile::ReadOnly );
            QWidget *iface = loader.load ( &fichero, this );
            fichero.close();
            QDialog *diag = new QDialog ( 0 );
            diag->setModal ( true );

            /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
            /// para que sea redimensionable y aparezca el titulo de la ventana.
            QHBoxLayout *layout = new QHBoxLayout;
            layout->addWidget ( iface );
            layout->setMargin ( 5 );
            layout->setSpacing ( 5 );
            diag->setLayout ( layout );
            diag->setWindowTitle ( iface->windowTitle() );
            diag->setGeometry ( 0, 0, iface->width(), iface->height() );
            QPushButton *button = iface->findChild<QPushButton *> ( "mui_aceptar" );
            connect ( button, SIGNAL ( released ( ) ), diag, SLOT ( accept() ) );
            QPushButton *button1 = iface->findChild<QPushButton *> ( "mui_cancelar" );
            connect ( button1, SIGNAL ( released ( ) ), diag, SLOT ( reject() ) );
	    
	    /// Vamos a buscar parametros tipo PARAM BLSEARCHWIDGET para configurar el dialogo.
	    QRegExp rx700 ( "<!--\\s*PARAM\\s*NAME\\s*=\\s*\"([^\"]*)\"\\s*TYPE\\s*=\\s*\"([^\"]*)\"\\s*LABEL\\s*=\\s*\"([^\"]*)\"\\s*TABLE\\s*=\\s*\"([^\"]*)\"\\s*TABLEID\\s*=\\s*\"([^\"]*)\"\\s*VALUES\\s*=\\s*\"([^\"]*)\"\\s*-->" );
	    rx700.setMinimal ( true );
	    int pos1 = 0;
	    while ( ( pos1 = rx700.indexIn ( buff, pos1 ) ) != -1 ) {
		QString name = rx700.cap ( 1 );
		QString type = rx700.cap ( 2 );
		QString label = rx700.cap( 3 );
		QString tablename = rx700.cap ( 4 );
		QString fieldid = rx700.cap ( 5 );
		QString values = rx700.cap ( 6 );
			BlSearchWidget * search = iface->findChild<BlSearchWidget *>("mui_" + name);
			if (search) {
			    // Para no liarla parda, ponemos provincias y asi no habrá pedatas de momento.
			    search->setLabel ( label );
			    search->setTableName ( tablename );
			    search->setFieldId ( fieldid );
			    QStringList listvalues = values.split(",");
			    for ( int j = 0; j < listvalues.size(); ++j ) {
				search->m_valores[listvalues.at(j)] = "";
			    } // end for
			    search->setId ( "" );
			} // end if
			pos1 += rx700.matchedLength();
	      } // end while
	    
	    /// Vamos a buscar parametros tipo PARAM BLCOMBOBOX para configurar el dialogo.
	    QRegExp rx701 ( "<!--\\s*PARAM\\s*NAME\\s*=\\s*\"([^\"]*)\"\\s*TYPE\\s*=\\s*\"([^\"]*)\"\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*TABLEID\\s*=\\s*\"([^\"]*)\"\\s*VALUES\\s*=\\s*\"([^\"]*)\"\\s*-->" );
	    rx701.setMinimal ( true );
	    int pos2 = 0;
	    while ( ( pos2 = rx701.indexIn ( buff, pos2 ) ) != -1 ) {
		QString name = rx701.cap ( 1 );
		QString type = rx701.cap ( 2 );
		QString query = rx701.cap ( 3 );
		QString fieldid = rx701.cap ( 4 );
		QString values = rx701.cap ( 5 );
			BlComboBox * search = iface->findChild<BlComboBox *>("mui_" + name);
			if (search) {
			    search->setQuery ( query );
			    search->setFieldId ( fieldid );
			    QStringList listvalues = values.split(",");
			    for ( int j = 0; j < listvalues.size(); ++j ) {
				search->m_valores[listvalues.at(j)] = "";
			    } // end for
			    search->setId ( "" );
			} // end if
			pos2 += rx701.matchedLength();
	      } // end while
	    
            int ret = diag->exec();
            if ( ret ) {

        	/// Recorre los BlDateSearch.
                QList<BlDateSearch *> dl2 = iface->findChildren<BlDateSearch *>();
                QListIterator<BlDateSearch *> dit2 ( dl2 );
                while ( dit2.hasNext() ) {
                    BlDateSearch * item = dit2.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->text();
                    addDbField ( nombre, BlDbField::DbDate, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
                } // end while
	      
        	/// Recorre los QLineEdit.
                QList<QLineEdit *> l2 = iface->findChildren<QLineEdit *>();
                QListIterator<QLineEdit *> it2 ( l2 );
                while ( it2.hasNext() ) {
                    QLineEdit * item = it2.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->text();
                    addDbField ( nombre, BlDbField::DbVarChar, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
                } // end while
                
                /// Recorre los QCheckBox.
                QList<QCheckBox *> qcb = iface->findChildren<QCheckBox *>();
                QListIterator<QCheckBox *> itqcb ( qcb );

                while ( itqcb.hasNext() ) {
                    QCheckBox *item = itqcb.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = "false";
                    if ( item->isChecked() ) {
                        valor = "true";
                    } else {
                	valor = "false";
		    } // end if
                    addDbField ( nombre, BlDbField::DbBoolean, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
                } // end while

		/// Recorre los BlSearchWidget.
                QList<BlSearchWidget *> jqcb = iface->findChildren<BlSearchWidget *>();
                QListIterator<BlSearchWidget *> itjqcb ( jqcb );

                while ( itjqcb.hasNext() ) {
                    BlSearchWidget *item = itjqcb.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->id();
                    addDbField ( nombre, BlDbField::DbInt, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
		 } // end while
		 
		/// Recorre los BlComboBox.
                QList<BlComboBox *> jqcb1 = iface->findChildren<BlComboBox *>();
                QListIterator<BlComboBox *> itjqcb1 ( jqcb1 );

                while ( itjqcb1.hasNext() ) {
                    BlComboBox *item = itjqcb1.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->id();
                    addDbField ( nombre, BlDbField::DbInt, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
		 } // end while
		 
		 

		QVariant exportaRML = iface->property("exportaRML");
		if (exportaRML.isValid() ) {
			QStringList props = exportaRML.toStringList();
		
			QListIterator<QString> iProps(props);
			while (iProps.hasNext()) {
				QString camp = iProps.next();
				QStringList cami = camp.split(".");
				QObject *actual=iface;
				QListIterator<QString> iCami(cami);
				
				while(iCami.hasNext() && actual) {

				    QString nom = iCami.next();
				    QObject *fill = actual->findChild<QObject *>("mui_"+nom);

				    if (!fill) {
					fill = actual->findChild<QObject *>(nom);
				    } // end if

				    if (fill) {
					actual = fill;
				    } else {
				    
					QVariant valor = actual->property(nom.toUtf8().data());
					m_variables[camp] =valor.toString();
					
					if (valor.canConvert<QObject*>()) {
					    actual = valor.value<QObject*>();
					} else {
					    actual = NULL;
					} // end if
				
				    } // end if
				    
				} // end while
				
			} // end while
			
		} // end if

            } // end if
            
            delete diag;
            
            // Si se ha pulsado cancelar entonce se sale del informe.
            if ( !ret ) return 0;
            
        } else {
            blMsgInfo ( "Archivo de Interficie no existe" );
        } // end if

        buff.replace ( pos, rx70.matchedLength(), "" );
        pos = 0;
    } // end while


    /// Buscamos establecimiento de variables y los ponemos en m_variables
    pos = 0;
    QRegExp rx54 ( "<!--\\s*SETVAR\\s*NAME\\s*=\\s*\"([^\"]*)\"\\s*VALUE\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx54.setMinimal ( true );
    while ( ( pos = rx54.indexIn ( buff, pos ) ) != -1 ) {
        QString valor = rx54.cap ( 2 );
        substrVars ( valor, tipoEscape );
        m_variables[rx54.cap ( 1 ) ] = valor;
        buff.replace ( pos, rx54.matchedLength(), "" );
        pos = 0;
    } // end while

    /// Tratamos las variables establecidas.
    substrVars ( buff, tipoEscape );


    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx19 ( "<!--\\s*INCLUDE\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx19.setMinimal ( true );
    while ( ( pos = rx19.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseIncludeFile ( rx19.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx19.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Existencia de Ficheros
    pos = 0;
    QRegExp rx9 ( "<!--\\s*EXISTS\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*EXISTS\\s*-->" );
    rx9.setMinimal ( true );
    while ( ( pos = rx9.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseExists ( rx9.cap ( 1 ), rx9.cap ( 2 ) );
        buff.replace ( pos, rx9.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx4 ( "<!--\\s*IF\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*QUERY\\s*-->" );
    rx4.setMinimal ( true );
    while ( ( pos = rx4.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ) );
        buff.replace ( pos, rx4.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's por tratar
    pos = 0;
    QRegExp rx1 ( "<!--\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*QUERY\\s*-->" );
    rx1.setMinimal ( true );
    while ( ( pos = rx1.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseQuery ( rx1.cap ( 1 ), rx1.cap ( 2 ), tipoEscape );
        buff.replace ( pos, rx1.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's por tratar con numeracion
    for (int aux = 0;aux < 10; aux++) {
      
	/// Buscamos Query's en condicional
	pos = 0;
	QRegExp rx4 ( "<!--\\s*IF\\s*QUERY"+QString::number(aux)+"\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*QUERY"+QString::number(aux)+"\\s*-->" );
	rx4.setMinimal ( true );
	while ( ( pos = rx4.indexIn ( buff, pos ) ) != -1 ) {
	    QString ldetalle = parseIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ) );
	    buff.replace ( pos, rx4.matchedLength(), ldetalle );
	    pos = 0;
	} // end while
      
	pos = 0;
	QRegExp rx1 ( "<!--\\s*QUERY"+QString::number(aux)+"\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*QUERY"+QString::number(aux)+"\\s*-->" );
	rx1.setMinimal ( true );
	while ( ( pos = rx1.indexIn ( buff, pos ) ) != -1 ) {
	    QString ldetalle = parseQuery ( rx1.cap ( 1 ), rx1.cap ( 2 ), tipoEscape );
	    buff.replace ( pos, rx1.matchedLength(), ldetalle );
	    pos = 0;
	} // end while
    }// end for

    /// Buscamos SubQuery's en condicional
    pos = 0;
    QRegExp rx14 ( "<!--\\s*IF\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*SUBQUERY\\s*-->" );
    rx14.setMinimal ( true );
    while ( ( pos = rx14.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseIfQuery ( rx14.cap ( 1 ), rx14.cap ( 2 ) );
        buff.replace ( pos, rx14.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos SubQuery's por tratar
    pos = 0;
    QRegExp rx7 ( "<!--\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*SUBQUERY\\s*-->" );
    rx7.setMinimal ( true );
    while ( ( pos = rx7.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseQuery ( rx7.cap ( 1 ), rx7.cap ( 2 ), tipoEscape );
        buff.replace ( pos, rx7.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx11 ( "<!--\\s*IF\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*ELSE\\s*-->(.*)<!--\\s*END\\s*IF\\s*-->" );
    rx11.setMinimal ( true );
    while ( ( pos = rx11.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseIf ( rx11.cap ( 1 ), rx11.cap ( 2 ), rx11.cap ( 3 ) );
        buff.replace ( pos, rx11.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    
    return 1;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString BlForm::parseIfQuery ( const QString &query, const QString &datos )
{
    BL_FUNC_DEBUG
    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query1 );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        result = datos;
    } // end while
    delete cur;
    
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString BlForm::parseIf ( const QString &query, const QString &datos, const QString &datos1 )
{
    BL_FUNC_DEBUG
    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QString query2 = "SELECT (" + query1 + ") AS res";
    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query2 );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        if ( cur->value( "res" ) == "t" ) {
            result = datos;
        } else {
            result = datos1;
        } // end if
    } // end while
    delete cur;
    
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString BlForm::parseIncludeFile ( const QString &file, int tipoEscape )
{
    BL_FUNC_DEBUG
    QString read = "";
    QFile arch ( file );
    if ( arch.open ( QIODevice::ReadOnly ) ) {
        QTextStream in ( &arch );
        while ( !in.atEnd() ) {
            read += in.readLine();
        } // end while
        arch.close();
    } // end if
    /// Buscamos parametros en el query y los ponemos.
    substrVars ( read, tipoEscape );


    
    return read;

}

/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString BlForm::parseQuery ( const QString &query, const QString &datos, int tipoEscape )
{
    BL_FUNC_DEBUG
    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1, tipoEscape );

    /// Cargamos el query y lo recorremos
    result = parseRecordset ( mainCompany() ->loadQuery ( query1 ), datos, tipoEscape );
    
    return result;

}

QString BlForm::parseRecordset ( BlDbRecordSet *cur, const QString &datos, int tipoEscape )
{
    BL_FUNC_DEBUG
    QString result = "";
    
    if ( !cur ) {
        
        return "";
    };
    while ( !cur->eof() ) {
        QString salidatemp = datos;

        int pos =  0;
	
        /// Buscamos cadenas que deban ir en ASCII puro (127) por ejemplo en los tickets.
        QRegExp rx2 ( "\\[(\\w*),a\\]" );
        while ( ( pos = rx2.indexIn ( salidatemp, pos ) ) != -1 ) {
            if ( cur->numcampo ( rx2.cap ( 1 ) ) != -1 ) {
		    /// Esta salida normalmente es para una ticketera, con lo que no entran, para nada, caracteres especiales.
                    salidatemp.replace ( pos, rx2.matchedLength(), blStringToUsAscii (cur->value( rx2.cap ( 1 ), -1, true )) );
                pos = 0;
            } else {
                pos += rx2.matchedLength();
            }
        } // end while
	
	
        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        QRegExp rx1 ( "\\[(\\w*),l\\]" );
        pos =  0;
        while ( ( pos = rx1.indexIn ( salidatemp, pos ) ) != -1 ) {
            if ( cur->numcampo ( rx1.cap ( 1 ) ) != -1 ) {
                salidatemp.replace ( pos, rx1.matchedLength(), cur->value( rx1.cap ( 1 ), -1, true ) );
                pos = 0;
            } else {
                pos += rx1.matchedLength();
            }
        } // end while
        
        
	
        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        QRegExp rx ( "\\[(\\w*)\\]" );
        pos =  0;
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {
                switch ( tipoEscape ) {
                case 1:
                    salidatemp.replace ( pos, rx.matchedLength(), blXMLEscape ( cur->value( rx.cap ( 1 ), -1, true ) )  );
                    break;
                case 2:
                    salidatemp.replace ( pos, rx.matchedLength(), blPythonEscape ( cur->value( rx.cap ( 1 ), -1, true ) )  );
                    break;
                default:
                    salidatemp.replace ( pos, rx.matchedLength(), cur->value( rx.cap ( 1 ), -1, true ) );
                    break;
                } // emd switch
                pos = 0;
            } else {
                pos += rx.matchedLength();
            }
        } // end while

        result += salidatemp;
        cur->nextRecord();
    } // end while
    delete cur;
    
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString BlForm::parseExists ( const QString &query, const QString &datos )
{
    BL_FUNC_DEBUG

    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QFile file ( query1 );
    if ( file.exists() )
        result = datos;
    

    return result;
}


int BlForm::generateRML ( void )
{
    BL_FUNC_DEBUG
    int err = BlDbRecord::generateRML();
    
    return err;
}

///
/**
**/
int BlForm::generateRML ( const QString &arch )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlForm::generateRML", 0, arch );

    /// Vaciamos las variables de RML
    m_variables.clear();

    /// Ponemos en m_variables CONF_DBNAME y CONF_DBUSER
    m_variables["CONF_DBNAME"] = mainCompany()->dbName();
    m_variables["CONF_DBUSER"] = mainCompany()->currentUser();

    /// Ofrecer el uso de unas variables con la fecha y hora de impresi&oacute;n (por ejemplo, en el pie)
    m_variables["fecha_actual"] = QDate::currentDate().toString ( Qt::DefaultLocaleShortDate );
    m_variables["hora_actual"] = QTime::currentTime().toString ( "HH:mm" );

    /// Disparamos los plugins
    int res = g_plugins->run ( "BlForm_generateRML", this );
    if ( res != 0 ) {
        return 1;
    } // end if

    res = BlDbRecord::generateRML ( arch );

    
    return res;
}


///
/** Clase para ser derivada para activar documentos adicionales en las fichas.
**/
void BlForm::activateDocuments ()
{
    BL_FUNC_DEBUG
    
}

///
/** Clase para ser derivada para desactivar documentos adicionales en las fichas.
**/
void BlForm::deactivateDocuments ()
{
    BL_FUNC_DEBUG
    
}
