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

#include <QMenu>
#include <QToolButton>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomNode>
#include <QInputDialog>
#include <QUiLoader>


#include "blform.h"
#include "blformlist.h"
#include "blformconfig.h"
#include "blcombobox.h"
#include "blplugins.h"
#include "blradiobutton.h"
#include "bldatesearch.h"
#include "blperiodicitycombobox.h"
#include "blmainwindow.h"


///
/**
\param parent
\param f
\param modo
**/
BlForm::BlForm ( QWidget *parent, Qt::WFlags f, edmode modo ) : BlWidget ( parent, f ), BlDbRecord ( NULL ), BlDialogChanges ( this )
{
    BL_FUNC_DEBUG

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BlForm_BlForm", this );
    if ( res != 0 ) {
        return;
    } // end if

    m_title = windowTitle();
    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( on_customContextMenuRequested ( const QPoint & ) ) );
    m_modo = modo;
    dialogChanges_readValues();


    
    
}


/**  procedimiento de QtScript
Importa el objeto que se pasa como referencia al engine de script
Busca el archivo en el directorio de openreports que tenga el mismo nombre que la clase
Importa el script y lo lanza.

*/
void BlForm::blScript(QObject * obj) {
    BL_FUNC_DEBUG
    
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
}

///
/**
\param emp
\param parent
\param f
\param modo
**/
BlForm::BlForm ( BlMainCompany *emp, QWidget *parent, Qt::WFlags f, edmode modo ) : BlWidget ( emp, parent, f ), BlDbRecord ( emp ), BlDialogChanges ( this )
{
    BL_FUNC_DEBUG

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BlForm_BlForm", this );
    if ( res != 0 ) {
        return;
    } // end if

    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( on_customContextMenuRequested ( const QPoint & ) ) );
    m_modo = modo;
    dialogChanges_readValues();
    
    
    QString fileName = g_confpr->value( CONF_DIR_OPENREPORTS ) + "blform.qs";
    QFile scriptFile(fileName);
    if (scriptFile.open(QIODevice::ReadOnly)) {
	  // handle error
	  QTextStream stream(&scriptFile);
	  QString contents = stream.readAll();
	  QScriptValue objectValue = m_myEngine.newQObject(this);
	  m_myEngine.globalObject().setProperty("BlForm", objectValue);
	  m_myEngine.importExtension("qt.core");
	  m_myEngine.importExtension("qt.gui"); 
	  m_myEngine.evaluate(contents);
	  scriptFile.close();
	  if (m_myEngine.hasUncaughtException()) {
	    blMsgInfo(m_myEngine.uncaughtException().toString());
	  } // end if
    } // end if
    
    
}


///
/**
**/
BlForm::~BlForm()
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlForm::~BlForm", 0, this->windowTitle() );
    g_plugins->lanza ( "BlForm_DesBlForm", this );
    removeWindow();
    
}



///
/**
**/
void BlForm::loadSpecs()
{
    BL_FUNC_DEBUG
    
        /// Disparamos los plugins
    int res1 = g_plugins->lanza ( "BlForm_loadSpecs", this );
    if ( res1 != 0 ) {
        
        return;
    } // end if
    
    QFile file ( CONFIG_DIR_CONFIG + objectName() + "_" + mainCompany() ->dbName() + "_spec.spc" );
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
                    restricciones |= BlDbField::DbVarChar;
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
            generaCampo ( nomheader, nompheader, typeheader );
        } // end if
    } // end for

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BlForm_loadSpecs_Post", this );

    
}


void BlForm::generaCampo ( const QString &objname, const QString &textname, const QString &type )
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

    QLabel *textLabel2_9_26 = new QLabel ( frame );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( textname );

    if ( type == "DBDATE" ) {
        BlDateSearch * bus = new BlDateSearch ( frame );
        bus->setObjectName ( objname );
        hboxLayout160->addWidget ( bus );
    } else {
        QLineEdit *bus = new QLineEdit ( frame );
        bus->setObjectName ( objname );
        hboxLayout160->addWidget ( bus );
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
void BlForm::on_mui_eliminar_clicked()
{
    on_mui_borrar_clicked();
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
        /// \TODO Este removeWindow encubre un bug. Debe tratarse de otra forma el
        /// sacar las ventanas de listventanas.
//        removeWindow();
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
    } // end if

    /// De Forma rapida hacemos un tratamiento de los permisos
    setDbTableName ( tableName() );
    /// Tal vez no es el mejor sitio para hacer la carga de SPECS. Pero no hay llamada especifica
    /// De configuracion por lo que si no es este no es ninguno.
    loadSpecs();
    
}


///
/**
**/
void BlForm::on_customContextMenuRequested ( const QPoint & )
{
    BL_FUNC_DEBUG
    QMenu *popup = new QMenu ( this );

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu ( popup );

    /// Lanzamos la propagacion del menu a traves de las clases derivadas.
    creaMenu ( popup );
    QAction *avconfig = NULL;
    if (g_confpr->value(CONF_MODO_EXPERTO) == "TRUE") {
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
        procesaMenu ( opcion );
    } // end if

    delete popup;
    
}


///
/**
**/
void BlForm::creaMenu ( QMenu * )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlForm:: creaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
**/
void BlForm::procesaMenu ( QAction * )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlForm:: procesaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
\param nom
**/
void BlForm::setDbTableName ( QString nom )
{
    BL_FUNC_DEBUG
    BlDbRecord::setDbTableName ( nom );

    if ( mainCompany() != NULL ) {
	  /// Buscamos los permisos que tiene el usuario y desactivamos botones.
	  if ( !mainCompany() ->hasTablePrivilege ( nom, "INSERT" )
	    && !mainCompany() ->hasTablePrivilege ( nom, "UPDATE" ) ) {

            QToolButton * b = findChild<QToolButton *> ( "mui_guardar" );
		if ( b ) b->setDisabled ( TRUE );
		QPushButton * p = findChild<QPushButton *> ( "mui_aceptar" );
		if ( p ) p->hide();
		p = findChild<QPushButton *> ( "mui_cancelar" );
		if ( p ) p->setText("Cerrar");

	  } // end if
	  if ( !mainCompany() ->hasTablePrivilege ( nom, "DELETE" ) ) {
		QToolButton * b = findChild<QToolButton *> ( "mui_borrar" );
            if ( b ) b->setDisabled ( TRUE );
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
            l1->setText ( campo->valorcampo() );
		if ( readOnly ) l1->setReadOnly ( true );
        } // end if
        /// Buscamos los QPlainTextEdit con nombre coincidente.
        QPlainTextEdit *l2 = findChild<QPlainTextEdit *> ( "mui_" + campo->fieldName() );
        if ( l2 ) {
            l2->setPlainText( campo->valorcampo() );
		if ( readOnly ) l2->setReadOnly ( true );
	   } // end if
        /// Buscamos los QTextEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *> ( "mui_" + campo->fieldName() );
        if ( l3 ) {
            l3->setText ( campo->valorcampo() );
		if ( readOnly ) l3->setReadOnly ( true );
        } // end if
        /// Buscamos BlWidgets que coincidan con el campo supuestamente
        /// sirve para los campos personales.
        BlWidget *l4 = findChild<BlWidget *> ( "mui_" + campo->fieldName() );
        if ( l4 ) {
            l4->setFieldValue ( campo->valorcampo() );
		if ( readOnly ) l4->setDisabled( true );
        } // end if

        /// Buscamos BlComboBox que coincidan con el campo supuestamente
        /// sirve para los campos personales.
        BlPeriodicityComboBox *l5 = findChild<BlPeriodicityComboBox *> ( "mui_" + campo->fieldName() );
        if ( l5 ) {
            l5->setperiodo ( campo->valorcampo() );
        } else {
            BlComboBox *l6 = findChild<BlComboBox *> ( "mui_" + campo->fieldName() );
            if ( l6 ) {
                l6->setFieldValue ( campo->valorcampo() );
		    if ( readOnly ) l6->setDisabled( true );
            } else {
                /// Buscamos BlComboBox que coincidan con el campo supuestamente
                /// sirve para los campos personales.
                QComboBox *l7 = findChild<QComboBox *> ( "mui_" + campo->fieldName() );
                if ( l7 ) {
                    l7->setCurrentIndex ( l7->findText ( campo->valorcampo() ) );
                } // end if
            } // end if
        } // end if

        /// Buscamos un BlDoubleSpinBox con nombre coincidente.
        BlDoubleSpinBox *l8 = findChild<BlDoubleSpinBox *> ( "mui_" + campo->fieldName() );
        if ( l8 ) {
            l8->setValue ( campo->valorcampo().toDouble() );
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
            if ( campo->valorcampo() == "t" ) {
                l9->setCheckState( Qt::Checked );
            } else if ( campo->valorcampo() == "f" ) {
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
            if ( l10.at ( i ) ->fieldValue() == campo->valorcampo() ) {
                l10.at ( i ) ->setChecked ( TRUE );
            } else {
                l10.at ( i ) ->setChecked ( FALSE );
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
                    campo->set ( "TRUE" );
                    break;
                case Qt::Unchecked:
                    campo->set ( "FALSE" );
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
        if ( g_plugins->lanza ( "BlForm_cargar", this ) ) return 0;
        cargarPost ( id );


	/// Buscamos un titulo adecuado segun los valores que contenga la tabla.
	QString wtitle = m_title + " ";
	if (exists("num" + m_tablename)) {
	  wtitle = wtitle + "[" + dbValue ( "num" + m_tablename) + "]";
	} else	if ( exists ("ref" + m_tablename)) {
	  wtitle = wtitle + "["+ dbValue ("ref" + m_tablename) + "]";
	} else if ( exists ( "cif" + m_tablename)) {
	  wtitle = wtitle + "[" + dbValue( "cif" + m_tablename) + "]";
	} else if ( exists ( "nom" + m_tablename)) {
	  wtitle = wtitle + "[" +  dbValue( "nom" + m_tablename) + "]";
	} else if ( exists ( "cod" + m_tablename)) {
	  wtitle = wtitle + "[" + dbValue ( "cod" + m_tablename) + "]";
	} else {
	   wtitle = wtitle + dbValue ( m_campoid );
	} // end if

        setWindowTitle ( wtitle );
        /// Activamos documentos adicionales
        activateDocuments();

        if ( paint == TRUE ) {
            pintar();
        } // end if

        dialogChanges_readValues();
        insertWindow ( m_title + dbValue(m_campoid), this, TRUE, wtitle );
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
        if ( g_plugins->lanza ( "BlForm_guardar_Post", this ) ) return 0;

        afterSave();

	mainCompany() ->commit();

        /// Hacemos una carga para que se actualizen datos como la referencia.
        load ( id );

        /// Si la directiva CONF_REFRESH_LIST esta activada buscamos el listado referente y lo recargamos
        if (g_confpr->value(CONF_REFRESH_LIST) == "TRUE") {
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
        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "BlForm_borrar", this ) ) return 0;
        beforeDelete();
        int err;
        err =  BlDbRecord::remove();

        /// Si la directiva CONF_REFRESH_LIST esta activada buscamos el listado referente y lo recargamos
        if (g_confpr->value(CONF_REFRESH_LIST) == "TRUE") {
                /// Buscamos el listado que corresponde al widget.
                QList<BlFormList *> lista = g_main->findChildren<BlFormList *>();
                for (int i = 0; i < lista.size(); ++i) {
                    if (((BlFormList *)lista.at(i))->subForm()->tableName() == tableName())
                        lista.at(i)->presentar();
                } // end for
        } // end if

        
        return err;
    } catch ( ... ) {
        
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
          m_globalvars[varname ] = varvalue;
}

/// Permite que el programa introduzca variables de impresion propias sin tener 
/// Que introducir datos en el registro de base de datos de ficha.
void BlForm::clearVars() {
          m_globalvars.clear();
}



/// Sustituye valores en el texto pasado como variables por su valor.
/// tipoEscape puede ser 0 --> Sin parseo
///    1 --> ParseoXML
///    2 --> ParseoPython
void BlForm::substrVars ( QString &buff, int tipoEscape )
{

    int pos = 0;

    /// Tratamos la sustitucion de variables de m_variables
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

    pos =  0;

    /// Buscamos parametros en el query y los ponemos.
    QRegExp rx ( "\\[(\\w*)\\]" );
    QString tmp;
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

    QRegExp rx70 ( "<!--\\s*IFACE\\s*SRC\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx70.setMinimal ( TRUE );
    while ( ( pos = rx70.indexIn ( buff, pos ) ) != -1 ) {
        QString cadarchivo = rx70.cap ( 1 );

        substrConf ( cadarchivo );

        QFile fichero ( cadarchivo );
        if ( fichero.exists() ) {
            QUiLoader loader;
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
            int ret = diag->exec();
            if ( ret ) {

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
    rx54.setMinimal ( TRUE );
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
    rx19.setMinimal ( TRUE );
    while ( ( pos = rx19.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseIncludeFile ( rx19.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx19.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Existencia de Ficheros
    pos = 0;
    QRegExp rx9 ( "<!--\\s*EXISTS\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*EXISTS\\s*-->" );
    rx9.setMinimal ( TRUE );
    while ( ( pos = rx9.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseExists ( rx9.cap ( 1 ), rx9.cap ( 2 ) );
        buff.replace ( pos, rx9.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx4 ( "<!--\\s*IF\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*QUERY\\s*-->" );
    rx4.setMinimal ( TRUE );
    while ( ( pos = rx4.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ) );
        buff.replace ( pos, rx4.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's por tratar
    pos = 0;
    QRegExp rx1 ( "<!--\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*QUERY\\s*-->" );
    rx1.setMinimal ( TRUE );
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
	rx4.setMinimal ( TRUE );
	while ( ( pos = rx4.indexIn ( buff, pos ) ) != -1 ) {
	    QString ldetalle = parseIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ) );
	    buff.replace ( pos, rx4.matchedLength(), ldetalle );
	    pos = 0;
	} // end while
      
	pos = 0;
	QRegExp rx1 ( "<!--\\s*QUERY"+QString::number(aux)+"\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*QUERY"+QString::number(aux)+"\\s*-->" );
	rx1.setMinimal ( TRUE );
	while ( ( pos = rx1.indexIn ( buff, pos ) ) != -1 ) {
	    QString ldetalle = parseQuery ( rx1.cap ( 1 ), rx1.cap ( 2 ), tipoEscape );
	    buff.replace ( pos, rx1.matchedLength(), ldetalle );
	    pos = 0;
	} // end while
    }// end for

    /// Buscamos SubQuery's en condicional
    pos = 0;
    QRegExp rx14 ( "<!--\\s*IF\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*SUBQUERY\\s*-->" );
    rx14.setMinimal ( TRUE );
    while ( ( pos = rx14.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseIfQuery ( rx14.cap ( 1 ), rx14.cap ( 2 ) );
        buff.replace ( pos, rx14.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos SubQuery's por tratar
    pos = 0;
    QRegExp rx7 ( "<!--\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*SUBQUERY\\s*-->" );
    rx7.setMinimal ( TRUE );
    while ( ( pos = rx7.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = parseQuery ( rx7.cap ( 1 ), rx7.cap ( 2 ), tipoEscape );
        buff.replace ( pos, rx7.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx11 ( "<!--\\s*IF\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*ELSE\\s*-->(.*)<!--\\s*END\\s*IF\\s*-->" );
    rx11.setMinimal ( TRUE );
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

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {
                switch ( tipoEscape ) {
                case 1:
                    salidatemp.replace ( pos, rx.matchedLength(), blXMLEscape ( cur->value( rx.cap ( 1 ), -1, TRUE ) )  );
                    break;
                case 2:
                    salidatemp.replace ( pos, rx.matchedLength(), blPythonEscape ( cur->value( rx.cap ( 1 ), -1, TRUE ) )  );
                    break;
                default:
                    salidatemp.replace ( pos, rx.matchedLength(), cur->value( rx.cap ( 1 ), -1, TRUE ) );
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
    int res = g_plugins->lanza ( "BlForm_generateRML", this );
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
