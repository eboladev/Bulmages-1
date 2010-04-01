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
    _depura ( "BlForm::BlForm", 0 );

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BlForm_BlForm", this );
    if ( res != 0 ) {
        return;
    } // end if

    m_title = windowTitle();
    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( on_customContextMenuRequested ( const QPoint & ) ) );
    m_modo = modo;
    dialogChanges_cargaInicial();
    _depura ( "END BlForm::BlForm", 0 );
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
    _depura ( "BlForm::BlForm", 0 );

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BlForm_BlForm", this );
    if ( res != 0 ) {
        return;
    } // end if

    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( on_customContextMenuRequested ( const QPoint & ) ) );
    m_modo = modo;
    dialogChanges_cargaInicial();
    _depura ( "END BlForm::BlForm", 0 );
}


///
/**
**/
BlForm::~BlForm()
{
    _depura ( "BlForm::~BlForm", 0, this->windowTitle() );
    g_plugins->lanza ( "BlForm_DesBlForm", this );
    sacaWindow();
    _depura ( "END BlForm::~BlForm", 0 );
}



///
/**
**/
void BlForm::cargaSpecs()
{
    _depura ( "BlForm::cargaSpecs", 0 );
    QFile file ( CONFIG_DIR_CONFIG + objectName() + "_" + mainCompany() ->dbName() + "_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        _depura ( "END BlForm::cargaSpecs", 0, "Fichero no se puede abrir" );
        return;
    } // end if
    if ( !doc.setContent ( &file ) ) {
        file.close();
        _depura ( "END BlForm::cargaSpecs", 0, "XML Invalido" );
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

    _depura ( "END BlForm::cargaSpecs", 0 );
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
void BlForm::setModoConsulta()
{
    _depura ( "BlForm::setModoConsulta", 0 );
    m_modo = BL_SELECT_MODE;
    _depura ( "END BlForm::setModoConsulta", 0 );
}


///
/**
**/
void BlForm::setModoEdicion()
{
    _depura ( "BlForm::setModoEdicion", 0 );
    m_modo = BL_EDIT_MODE;
    _depura ( "END BlForm::setModoEdicion", 0 );
}


///
/**
\return
**/
bool BlForm::modoEdicion()
{
    _depura ( "BlForm::modoEdicion", 0 );
    _depura ( "END BlForm::modoEdicion", 0 );
    return m_modo == BL_EDIT_MODE;
}


///
/**
\return
**/
bool BlForm::modoConsulta()
{
    _depura ( "BlForm::modoConsulta", 0 );
    _depura ( "END BlForm::modoConsulta", 0 );
    return m_modo == BL_SELECT_MODE;
}


///
/**
**/
void BlForm::on_mui_cancelar_clicked()
{
    _depura ( "BlForm::on_mui_cancelar_clicked", 0 );
    close();
    _depura ( "END BlForm::on_mui_cancelar_clicked", 0 );
}


///
/**
**/
void BlForm::on_mui_guardar_clicked()
{
    _depura ( "BlForm::on_mui_guardar_clicked", 0 );
    guardar();
    _depura ( "END BlForm::on_mui_guardar_clicked", 0 );
}


///
/**
**/
void BlForm::on_mui_aceptar_clicked()
{
    _depura ( "BlForm::on_mui_aceptar_clicked", 0 );
    try {
        if ( guardar() ) {
            throw - 1;
        } // end if
        close();
    } catch ( ... ) {
        _depura ( "Error al guardar la ficha", 2 );
    } // end try
    _depura ( "END BlForm::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
void BlForm::on_mui_imprimir_clicked()
{
    _depura ( "BlForm::on_mui_imprimir_clicked", 0 );
    imprimir();
    _depura ( "END BlForm::on_mui_imprimir_clicked", 0 );
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
    _depura ( "BlForm::on_mui_borrar_clicked", 0 );

    int val = QMessageBox::question ( this,
                                      _ ( "Borrar" ) + " " + windowTitle(),
                                      _ ( "Desea eliminar '" ) + " " + windowTitle() + "' ?",
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default );

    if ( val == QMessageBox::Yes ) {
        if ( !borrar() ) {
            dialogChanges_cargaInicial();
            _depura ( windowTitle() + " " + "borrado satisfactoriamente.", 10 );
            close();
        } else {
            mensajeInfo ( windowTitle() + _ ( "No se ha podido borrar" ) );
        }// end if
    } // end if
    _depura ( "END BlForm::on_mui_borrar_clicked", 0 );
}


///
/**
\param e
\return
**/
void BlForm::closeEvent ( QCloseEvent *e )
{
    _depura ( "BlForm::closeEvent", 0, windowTitle() );

    try {
        if ( dialogChanges_hayCambios() ) {
            int val = QMessageBox::warning ( this,
                                             _ ( "Guardar" ) + " " + windowTitle(),
                                             _ ( "Desea guardar los cambios?" ),
                                             _ ( "&Si" ), _ ( "&No" ), _ ( "&Cancelar" ), 0, 2 );
            if ( val == 0 ) {
                guardar();
            } // end if
            if ( val == 2 ) {
                e->ignore();
                return;
            } // end if
        } // end if
        /// \TODO Este sacaWindow encubre un bug. Debe tratarse de otra forma el
        /// sacar las ventanas de listventanas.
//        sacaWindow();
    } catch ( ... ) {
        mensajeInfo ( _ ( "No se pudo cerrar la ventana debido a un error" ) );
        e->ignore();
    } // end try
    _depura ( "END BlForm::closeEvent", 0 );
}


///
/**
\return
**/
int BlForm::sacaWindow()
{
    _depura ( "BlForm::sacaWindow", 0 );
    if ( mainCompany() != NULL ) {
        mainCompany() ->sacaWindow ( this );
    } // end if
    _depura ( "END BlForm::sacaWindow", 0 );
    return 0;
}


///
/**
\param nom
\param obj
\param compdup
**/
void BlForm::meteWindow ( QString nom, QObject *obj, bool compdup, QString titulo )
{
    _depura ( "BlForm::meteWindow", 0 );
    if ( mainCompany() != NULL ) {
        mainCompany() ->meteWindow ( nom, obj, compdup, titulo );
    } // end if

    /// De Forma rapida hacemos un tratamiento de los permisos
    setDbTableName ( tableName() );
    /// Tal vez no es el mejor sitio para hacer la carga de SPECS. Pero no hay llamada especifica
    /// De configuracion por lo que si no es este no es ninguno.
    cargaSpecs();
    _depura ( "END BlForm::meteWindow", 0 );
}


///
/**
**/
void BlForm::on_customContextMenuRequested ( const QPoint & )
{
    _depura ( "BlForm::on_customContextMenuRequested", 0 );
    QMenu *popup = new QMenu ( this );

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu ( popup );

    /// Lanzamos la propagacion del menu a traves de las clases derivadas.
    creaMenu ( popup );


    QAction *avconfig = popup->addAction ( _ ( "Opciones avanzadas de ficha" ) );
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
    _depura ( "END BlForm::on_customContextMenuRequested", 0 );
}


///
/**
**/
void BlForm::creaMenu ( QMenu * )
{
    _depura ( "BlForm:: creaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
**/
void BlForm::procesaMenu ( QAction * )
{
    _depura ( "BlForm:: procesaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
\param nom
**/
void BlForm::setDbTableName ( QString nom )
{
    _depura ( "BlForm::setDbTableName", 0 );
    BlDbRecord::setDbTableName ( nom );

    if ( mainCompany() != NULL ) {
        if ( !mainCompany() ->hasTablePrivilege ( nom, "INSERT" ) ) {
            /// Buscamos los permisos que tiene el usuario y desactivamos botones.
            QToolButton * b = findChild<QToolButton *> ( "mui_guardar" );
            if ( b ) b->setDisabled ( TRUE );
            b = findChild<QToolButton *> ( "mui_borrar" );
            if ( b ) b->setDisabled ( TRUE );
        } // end if
    } // end if

    _depura ( "END BlForm::setDbTableName", 0 );
}


///
/**
\param nom
**/
void BlForm::setTitleName ( QString nom )
{
    _depura ( "BlForm::setTitleName" );
    m_title = nom;
    _depura ( "END BlForm::setTitleName" );
}


///
/**
**/
void BlForm::pintar()
{
    _depura ( "BlForm::pintar", 0 );
    BlDbField *campo;
    /// Recorremos todos los campos definidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        /// Buscamos los QLineEdit con nombre coincidente.
        QLineEdit *l1 = findChild<QLineEdit *> ( "mui_" + campo->nomcampo() );
        if ( l1 ) {
            l1->setText ( campo->valorcampo() );
        } // end if
        /// Buscamos los QPlainTextEdit con nombre coincidente.
        QPlainTextEdit *l2 = findChild<QPlainTextEdit *> ( "mui_" + campo->nomcampo() );
        if ( l2 ) {
            l2->setPlainText( campo->valorcampo() );
        } // end if
        /// Buscamos los QTextEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *> ( "mui_" + campo->nomcampo() );
        if ( l3 ) {
            l3->setText ( campo->valorcampo() );
        } // end if
        /// Buscamos BlWidgets que coincidan con el campo supuestamente
        /// sirve para los campos personales.
        BlWidget *l4 = findChild<BlWidget *> ( "mui_" + campo->nomcampo() );
        if ( l4 ) {
            l4->setFieldValue ( campo->valorcampo() );
        } // end if

        /// Buscamos BlComboBox que coincidan con el campo supuestamente
        /// sirve para los campos personales.
        BlPeriodicityComboBox *l5 = findChild<BlPeriodicityComboBox *> ( "mui_" + campo->nomcampo() );
        if ( l5 ) {
            l5->setperiodo ( campo->valorcampo() );
        } else {
            BlComboBox *l6 = findChild<BlComboBox *> ( "mui_" + campo->nomcampo() );
            if ( l6 ) {
                l6->setFieldValue ( campo->valorcampo() );
            } else {
                /// Buscamos BlComboBox que coincidan con el campo supuestamente
                /// sirve para los campos personales.
                QComboBox *l7 = findChild<QComboBox *> ( "mui_" + campo->nomcampo() );
                if ( l7 ) {
                    l7->setCurrentIndex ( l7->findText ( campo->valorcampo() ) );
                } // end if
            } // end if
        } // end if

        /// Buscamos un BlDoubleSpinBox con nombre coincidente.
        BlDoubleSpinBox *l8 = findChild<BlDoubleSpinBox *> ( "mui_" + campo->nomcampo() );
        if ( l8 )
            l8->setValue ( campo->valorcampo().toDouble() );

        /// Buscamos los QCheckBox con nombre coincidente.
        QCheckBox *l9 = findChild<QCheckBox *> ( "mui_" + campo->nomcampo() );
        if ( l9 ) {
            if ( campo->valorcampo() == "t" ) {
                l9->setCheckState( Qt::Checked );
            } else if ( campo->valorcampo() == "f" ) {
                l9->setCheckState( Qt::Unchecked );
            } else if ( l9->isTristate() ) {
                /// El estado indeterminado se aplica cuando el campo es triestado.
                l9->setCheckState( Qt::PartiallyChecked );
            } // end if
        } // end if

        /// Buscamos los 'Radio Buttons' y los preparamos.
        QList<BlRadioButton *> l10 = findChildren<BlRadioButton *> ( QRegExp ( "mui_" + campo->nomcampo() + "_*" ) );
        for ( int i = 0; i < l10.size(); ++i ) {
            if ( l10.at ( i ) ->fieldValue() == campo->valorcampo() ) {
                l10.at ( i ) ->setChecked ( TRUE );
            } else {
                l10.at ( i ) ->setChecked ( FALSE );
            } // end if
        } // end for
    } // end for
    pintarPost();
    _depura ( "END BlForm::pintar", 0 );
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
    _depura ( "BlForm::recogeValores", 0 );
    BlDbField *campo;
    /// Recorremos todos los campos definidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        /// Buscamos un QLineEdit con nombre coincidente.
        QLineEdit *l1 = findChild<QLineEdit *> ( "mui_" + campo->nomcampo() );
        if ( l1 )
            campo->set ( l1->text() );

        /// Buscamos un QPlainTextEdit con nombre coincidente.
        QPlainTextEdit *l2 = findChild<QPlainTextEdit *> ( "mui_" + campo->nomcampo() );
        if ( l2 )
            campo->set ( l2->toPlainText() );

        /// Buscamos un QTextEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *> ( "mui_" + campo->nomcampo() );
        if ( l3 )
            campo->set ( l3->toPlainText() );

        /// Buscamos BlWidgets que coincidan con el campo. Supuestamente sirve para los campos personales.
        BlWidget *l4 = findChild<BlWidget *> ( "mui_" + campo->nomcampo() );
        if ( l4 )
            campo->set ( l4->fieldValue() );

        /// Buscamos BlComboBox que coincidan con el campo. Supuestamente sirve para los campos personales.
        BlPeriodicityComboBox *l5 = findChild<BlPeriodicityComboBox *> ( "mui_" + campo->nomcampo() );
        if ( l5 ) {
            campo->set(l5->periodo() );
        } else {
            BlComboBox *l6 = findChild<BlComboBox *> ( "mui_" + campo->nomcampo() );
            if ( l6 ) {
                campo->set ( l6->fieldValue() );
            } else {
                /// Buscamos QComboBox que coincidan con el campo. Supuestamente sirve para los campos personales.
                QComboBox *l7 = findChild<QComboBox *> ( "mui_" + campo->nomcampo() );
                if ( l7 ) {
                    campo->set ( l7->currentText() );
                } // end if
            } // end if
        } // end if

        /// Buscamos un BlDoubleSpinBox con nombre coincidente.
        BlDoubleSpinBox *l8 = findChild<BlDoubleSpinBox *> ( "mui_" + campo->nomcampo() );
        if ( l8 )
            campo->set ( QString::number(l8->value()) );

        /// Buscamos un QCheckBox con nombre coincidente.
        QCheckBox *l9 = findChild<QCheckBox *> ( "mui_" + campo->nomcampo() );
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
        QList<BlRadioButton *> l10 = findChildren<BlRadioButton *> ( QRegExp ( "mui_" + campo->nomcampo() + "_*" ) );
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
    _depura ( "END BlForm::recogeValores", 0 );
}


///
/**
\param id
\return
**/
int BlForm::cargar ( QString id, bool paint )
{
    _depura ( "BlForm::cargar", 0, id );
    try {
        if ( BlDbRecord::cargar ( id ) ) {
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
        activaDocumentos();

        if ( paint == TRUE ) {
            pintar();
        } // end if

        dialogChanges_cargaInicial();
        meteWindow ( m_title + dbValue(m_campoid), this, TRUE, wtitle );
    } catch ( ... ) {
        _depura ( "END BlForm::cargar", 0, "Error en la carga" );
        return -1;
    } // end try
    _depura ( "END BlForm::cargar", 0 );
    return 0;
}


/// Guarda los datos de la ficha en la base de datos.
/**
  Este metodo guarda los contenidos de la Ficha (siempre que esta haya sido inicializada).
  Luego llama a plugins y a guardarPost por si se quieren hacer acciones adicionales de guardado.
  Tras guardar todos los elementos hace una carga.
\return 0 Si no hay problemas. -1 Si ha habido problemas.
**/
int BlForm::guardar()
{
    _depura ( "BlForm::guardar", 0 );

    try {
        QString id;
        recogeValores();
        mainCompany() ->begin();
        DBsave ( id );
        setDbValue ( m_campoid, id );
        mainCompany() ->commit();

        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "BlForm_guardar_Post", this ) ) return 0;

        guardarPost();

        /// Hacemos una carga para que se actualizen datos como la referencia.
        cargar ( id );

        /// Si la directiva CONF_REFRESH_LIST esta activada buscamos el listado referente y lo recargamos
        if (g_confpr->valor(CONF_REFRESH_LIST) == "TRUE") {
                /// Buscamos el listado que corresponde al widget.
                QList<BlFormList *> lista = g_main->findChildren<BlFormList *>();
                for (int i = 0; i < lista.size(); ++i) {
                    if (((BlFormList *)lista.at(i))->subForm()->tableName() == tableName())
                        lista.at(i)->presentar();
                } // end for
        } // end if

        _depura ( "END BlForm::guardar", 0 );
        return 0;
    } catch ( int valor ) {
        /// Valor del error diferente a -1 significa que ya se ha mostrado algun mensaje de
        /// error y no hay que mostrar otro.
        if ( valor == -1 ) {
            mensajeInfo ( "Error inesperado al guardar" );
        } // end if
        mainCompany() ->rollback();
        return -1;

    } catch ( ... ) {
        mensajeInfo ( "Error inesperado al guardar" );
        mainCompany() ->rollback();
        return -1;
    } // end try
}


int BlForm::borrarPre()
{
    _depura ( "BlForm::borrarPre", 0 );
    _depura ( "END BlForm::borrarPre", 0 );
    return 0;
}


/** Borra la ficha en la base de datos
    Este metodo puede ser invocado desde la clase ficha o desde la pantalla previa
*/
/**
\return
**/
int BlForm::borrar()
{
    _depura ( "BlForm::borrar", 0 );
    try {
        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "BlForm_borrar", this ) ) return 0;
        borrarPre();
        int err;
        err =  BlDbRecord::borrar();

        /// Si la directiva CONF_REFRESH_LIST esta activada buscamos el listado referente y lo recargamos
        if (g_confpr->valor(CONF_REFRESH_LIST) == "TRUE") {
                /// Buscamos el listado que corresponde al widget.
                QList<BlFormList *> lista = g_main->findChildren<BlFormList *>();
                for (int i = 0; i < lista.size(); ++i) {
                    if (((BlFormList *)lista.at(i))->subForm()->tableName() == tableName())
                        lista.at(i)->presentar();
                } // end for
        } // end if

        _depura ( "END BlForm::borrar", 0 );
        return err;
    } catch ( ... ) {
        _depura ( "END BlForm::borrar", 0, "Error en el guardado" );
        return -1;
    } // end try
}


///
/**
\return
**/
int BlForm::guardarPost()
{
    _depura ( "BlForm::guardarPost", 0 );
    _depura ( "END BlForm::guardarPost", 0 );
    return 0;
}


///
/**
\return
**/
int BlForm::cargarPost ( QString )
{
    _depura ( "BlForm::cargarPost", 0 );
    _depura ( "END BlForm::cargarPost", 0 );
    return 0;
}

///
/**
\return
**/
void BlForm::pintarPost()
{
    _depura ( "BlForm::pintarPost", 0 );
    _depura ( "END BlForm::pintarPost", 0 );
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


    substrConf ( buff );

    pos =  0;

    /// Buscamos parametros en el query y los ponemos.
    QRegExp rx ( "\\[(\\w*)\\]" );
    QString tmp;
    while ( ( pos = rx.indexIn ( buff, pos ) ) != -1 ) {
        if ( exists ( rx.cap ( 1 ) ) ) {
                        
            switch ( tipoEscape ) {
            case 1:
                buff.replace ( pos, rx.matchedLength(), xmlEscape ( dbValue ( rx.cap ( 1 ) ) ) );
                break;
            case 2:
                buff.replace ( pos, rx.matchedLength(), pythonEscape ( dbValue ( rx.cap ( 1 ) ) ) );
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


/// Busca strings del tipo [xxxx] entro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/// Tambien busca los parametros PARAM e IFACE para tambien tratarlos.
/// Devuelve 1 Si todo esta OK o 0 Si hay algun error
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
int BlForm::trataTags ( QString &buff, int tipoEscape )
{
    _depura ( "BlForm::trataTags", 0 );

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
                QList<QLineEdit *> l2 = iface->findChildren<QLineEdit *>();
                QListIterator<QLineEdit *> it2 ( l2 );
                while ( it2.hasNext() ) {
                    QLineEdit * item = it2.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->text();
                    addDbField ( nombre, BlDbField::DbVarChar, BlDbField::DbNoSave, nombre  );
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
				}
				if (fill) {
				actual = fill;
				} else {
				QVariant valor =  actual->property(nom.toUtf8().data());
				m_variables[camp] =valor.toString();
				if (valor.canConvert<QObject*>()) {
					actual = valor.value<QObject*>();
					} else {
					actual = NULL;
					}
				
				}
				} // while components nom prop
				
			} // while props
		}

            } // end if
            delete diag;
            // Si se ha pulsado cancelar entonce se sale del informe.
            if ( !ret ) return 0;
        } else {
            mensajeInfo ( "Archivo de Interficie no existe" );
        }// end if

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
        QString ldetalle = trataIncludeFile ( rx19.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx19.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx9 ( "<!--\\s*EXISTS\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*EXISTS\\s*-->" );
    rx9.setMinimal ( TRUE );
    while ( ( pos = rx9.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataExists ( rx9.cap ( 1 ), rx9.cap ( 2 ) );
        buff.replace ( pos, rx9.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx4 ( "<!--\\s*IF\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*QUERY\\s*-->" );
    rx4.setMinimal ( TRUE );
    while ( ( pos = rx4.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ) );
        buff.replace ( pos, rx4.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's por tratar
    pos = 0;
    QRegExp rx1 ( "<!--\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*QUERY\\s*-->" );
    rx1.setMinimal ( TRUE );
    while ( ( pos = rx1.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataQuery ( rx1.cap ( 1 ), rx1.cap ( 2 ), tipoEscape );
        buff.replace ( pos, rx1.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx14 ( "<!--\\s*IF\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*SUBQUERY\\s*-->" );
    rx14.setMinimal ( TRUE );
    while ( ( pos = rx14.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataIfQuery ( rx14.cap ( 1 ), rx14.cap ( 2 ) );
        buff.replace ( pos, rx14.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's por tratar
    pos = 0;
    QRegExp rx7 ( "<!--\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*SUBQUERY\\s*-->" );
    rx7.setMinimal ( TRUE );
    while ( ( pos = rx7.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataQuery ( rx7.cap ( 1 ), rx7.cap ( 2 ), tipoEscape );
        buff.replace ( pos, rx7.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Query's en condicional
    pos = 0;
    QRegExp rx11 ( "<!--\\s*IF\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*ELSE\\s*-->(.*)<!--\\s*END\\s*IF\\s*-->" );
    rx11.setMinimal ( TRUE );
    while ( ( pos = rx11.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataIf ( rx11.cap ( 1 ), rx11.cap ( 2 ), rx11.cap ( 3 ) );
        buff.replace ( pos, rx11.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    _depura ( "END BlForm::trataTags", 0 );
    return 1;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString BlForm::trataIfQuery ( const QString &query, const QString &datos )
{
    _depura ( "BlForm::trataIfQuery", 0 );
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
    _depura ( "END BlForm::trataIfQuery", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString BlForm::trataIf ( const QString &query, const QString &datos, const QString &datos1 )
{
    _depura ( "BlForm::trataIfQuery", 0 );
    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QString query2 = "SELECT (" + query1 + ") AS res";
    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query2 );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        if ( cur->valor ( "res" ) == "t" ) {
            result = datos;
        } else {
            result = datos1;
        } // end if
    } // end while
    delete cur;
    _depura ( "END BlForm::trataIf", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString BlForm::trataIncludeFile ( const QString &file, int tipoEscape )
{
    _depura ( "BlForm::trataIncludeFile", 0 );
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


    _depura ( "END BlForm::trataIncludeFile", 0 );
    return read;

}

/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString BlForm::trataQuery ( const QString &query, const QString &datos, int tipoEscape )
{
    _depura ( "BlForm::trataQuery", 0 );
    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1, tipoEscape );

    /// Cargamos el query y lo recorremos
    result = trataCursor ( mainCompany() ->loadQuery ( query1 ), datos, tipoEscape );
    _depura ( "END BlForm::trataQuery", 0 );
    return result;

}

QString BlForm::trataCursor ( BlDbRecordSet *cur, const QString &datos, int tipoEscape )
{
    _depura ( "BlForm::trataCursor", 0 );
    QString result = "";
    
    if ( !cur ) {
        _depura ( "END BlForm::trataCursor", 0 , "cur==NULL" );
        return "";
    };
    while ( !cur->eof() ) {
        QString salidatemp = datos;

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        //_depura("salidatemp =",0,salidatemp);
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            //_depura("substituïm ",0,rx.cap(1));
            if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {
                switch ( tipoEscape ) {
                case 1:
                    salidatemp.replace ( pos, rx.matchedLength(), xmlEscape ( cur->valor ( rx.cap ( 1 ), -1, TRUE ) )  );
                    break;
                case 2:
                    salidatemp.replace ( pos, rx.matchedLength(), pythonEscape ( cur->valor ( rx.cap ( 1 ), -1, TRUE ) )  );
                    break;
                default:
                    salidatemp.replace ( pos, rx.matchedLength(), cur->valor ( rx.cap ( 1 ), -1, TRUE ) );
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
    _depura ( "END BlForm::trataCursor", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString BlForm::trataExists ( const QString &query, const QString &datos )
{
    _depura ( "BlForm::trataExists", 0 );

    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QFile file ( query1 );
    if ( file.exists() )
        result = datos;
    _depura ( "END BlForm::trataExists", 0 );

    return result;
}


int BlForm::generaRML ( void )
{
    _depura ( "BlForm::generaRML", 0 );
    int err = BlDbRecord::generaRML();
    _depura ( "END BlForm::generaRML", 0 );
    return err;
}

///
/**
**/
int BlForm::generaRML ( const QString &arch )
{
    _depura ( "BlForm::generaRML", 0, arch );

    /// Vaciamos las variables de RML
    m_variables.clear();

    /// Ponemos en m_variables CONF_DBNAME y CONF_DBUSER
    m_variables["CONF_DBNAME"] = mainCompany()->dbName();
    m_variables["CONF_DBUSER"] = mainCompany()->currentUser();

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BlForm_generaRML", this );
    if ( res != 0 ) {
        return 1;
    } // end if

    res = BlDbRecord::generaRML ( arch );

    _depura ( "END BlForm::generaRML", 0 );
    return res;
}


///
/** Clase para ser derivada para activar documentos adicionales en las fichas.
**/
void BlForm::activaDocumentos ()
{
    _depura ( "BlForm::activaDocumentos", 0 );
    _depura ( "END BlForm::activaDocumentos", 0 );
}

///
/** Clase para ser derivada para desactivar documentos adicionales en las fichas.
**/
void BlForm::desactivaDocumentos ()
{
    _depura ( "BlForm::desactivaDocumentos", 0 );
    _depura ( "END BlForm::desactivaDocumentos", 0 );
}
