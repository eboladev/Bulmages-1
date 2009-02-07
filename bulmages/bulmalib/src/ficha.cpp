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

#include "ficha.h"
#include "fichacfg.h"
#include "qcombobox2.h"
#include "plugins.h"
#include "qradiobutton2.h"
#include "busquedafecha.h"

#include <QMenu>
#include <QToolButton>
#include <QTextEdit>
#include <QCheckBox>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomNode>
#include <QInputDialog>
#include <QUiLoader>
///
/**
\param parent
\param f
\param modo
**/
Ficha::Ficha ( QWidget *parent, Qt::WFlags f, edmode modo ) : BLWidget ( parent, f ), DBRecord ( NULL ), dialogChanges ( this )
{
    _depura ( "Ficha::Ficha", 0 );

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "Ficha_Ficha", this );
    if ( res != 0 ) {
        return;
    } // end if

    m_title = windowTitle();
    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( on_customContextMenuRequested ( const QPoint & ) ) );
    m_modo = modo;
    dialogChanges_cargaInicial();
    _depura ( "END Ficha::Ficha", 0 );
}


///
/**
\param emp
\param parent
\param f
\param modo
**/
Ficha::Ficha ( EmpresaBase *emp, QWidget *parent, Qt::WFlags f, edmode modo ) : BLWidget ( emp, parent, f ), DBRecord ( emp ), dialogChanges ( this )
{
    _depura ( "Ficha::Ficha", 0 );

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "Ficha_Ficha", this );
    if ( res != 0 ) {
        return;
    } // end if

    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( on_customContextMenuRequested ( const QPoint & ) ) );
    m_modo = modo;
    dialogChanges_cargaInicial();
    _depura ( "END Ficha::Ficha", 0 );
}


///
/**
**/
Ficha::~Ficha()
{
    _depura ( "Ficha::~Ficha", 0, this->windowTitle() );
    g_plugins->lanza ( "Ficha_DesFicha", this );
    sacaWindow();
    _depura ( "END Ficha::~Ficha", 0 );
}



///
/**
**/
void Ficha::cargaSpecs()
{
    _depura ( "Ficha::cargaSpecs", 0 );
//    QFile file(confpr->valor(CONF_DIR_USER) + m_fileconfig + "_" + empresaBase()->nameDB() + "_specs.spc");
    QFile file ( CONFIG_DIR_CONFIG + objectName() + "_" + empresaBase() ->nameDB() + "_spec.spc" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) ) {
        _depura ( "END Ficha::cargaSpecs", 0, "Fichero no se puede abrir" );
        return;
    } // end if
    if ( !doc.setContent ( &file ) ) {
        file.close();
        _depura ( "END Ficha::cargaSpecs", 0, "XML Invalido" );
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
            DBCampo::dbtype type = DBCampo::DBvarchar;
            QString nomheader = e1.firstChildElement ( "NOMCAMPO" ).toElement().text();
            if ( exists ( nomheader ) ) return;
            QString nompheader = e1.firstChildElement ( "NOMPCAMPO" ).toElement().text();
            QString typeheader = e1.firstChildElement ( "DBTYPECAMPO" ).toElement().text();
            if ( typeheader == "DBVARCHAR" ) {
                type = DBCampo::DBvarchar;
            } else if ( typeheader == "DBINT" ) {
                type = DBCampo::DBint;
            } else if ( typeheader == "DBNUMERIC" ) {
                type = DBCampo::DBnumeric;
            } else if ( typeheader == "DBBOOLEAN" ) {
                type = DBCampo::DBboolean;
            } else if ( typeheader == "DBDATE" ) {
                type = DBCampo::DBdate;
            } // end if

            int restricciones = ( int ) DBCampo::DBNothing;
            QDomElement restrict = e1.firstChildElement ( "RESTRICTIONSCAMPO" );
            while ( !restrict.isNull() ) {
                QString trestrict = restrict.text();
                if ( trestrict == "DBNOTHING" ) {
                    restricciones |= DBCampo::DBvarchar;
                } else if ( trestrict == "DBNOTNULL" ) {
                    restricciones |= DBCampo::DBNotNull;
                } else if ( trestrict == "DBPRIMARYKEY" ) {
                    restricciones |= DBCampo::DBPrimaryKey;
                } else if ( trestrict == "DBNOSAVE" ) {
                    restricciones |= DBCampo::DBNoSave;
                } else if ( trestrict == "DBAUTO" ) {
                    restricciones |= DBCampo::DBAuto;
                } else if ( trestrict == "DBAUTO" ) {
                    restricciones |= DBCampo::DBAuto;
                } else if ( trestrict == "DBDUPPRIMARYKEY" ) {
                    restricciones |= DBCampo::DBDupPrimaryKey;
                } else if ( trestrict == "DBREQUIRED" ) {
                    restricciones |= DBCampo::DBRequired;
                } else if ( trestrict == "DBNOLOAD" ) {
                    restricciones |= DBCampo::DBNoLoad;
                } // end if
                restrict = restrict.nextSiblingElement ( "RESTRICTIONSCAMPO" );
            } // end while

            addDBCampo ( nomheader, type, ( DBCampo::dbrestrict ) restricciones, nompheader );
            generaCampo ( nomheader, nompheader, typeheader );
        } // end if
    } // end for

    _depura ( "END Ficha::cargaSpecs", 0 );
}


void Ficha::generaCampo ( const QString &objname, const QString &textname, const QString &type )
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
        BusquedaFecha * bus = new BusquedaFecha ( frame );
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
void Ficha::setModoConsulta()
{
    _depura ( "Ficha::setModoConsulta", 0 );
    m_modo = SelectMode;
    _depura ( "END Ficha::setModoConsulta", 0 );
}


///
/**
**/
void Ficha::setModoEdicion()
{
    _depura ( "Ficha::setModoEdicion", 0 );
    m_modo = EditMode;
    _depura ( "END Ficha::setModoEdicion", 0 );
}


///
/**
\return
**/
bool Ficha::modoEdicion()
{
    _depura ( "Ficha::modoEdicion", 0 );
    _depura ( "END Ficha::modoEdicion", 0 );
    return m_modo == EditMode;
}


///
/**
\return
**/
bool Ficha::modoConsulta()
{
    _depura ( "Ficha::modoConsulta", 0 );
    _depura ( "END Ficha::modoConsulta", 0 );
    return m_modo == SelectMode;
}


///
/**
**/
void Ficha::on_mui_cancelar_clicked()
{
    _depura ( "Ficha::on_mui_cancelar_clicked", 0 );
    close();
    _depura ( "END Ficha::on_mui_cancelar_clicked", 0 );
}


///
/**
**/
void Ficha::on_mui_guardar_clicked()
{
    _depura ( "Ficha::on_mui_guardar_clicked", 0 );
    guardar();
    _depura ( "END Ficha::on_mui_guardar_clicked", 0 );
}


///
/**
**/
void Ficha::on_mui_aceptar_clicked()
{
    _depura ( "Ficha::on_mui_aceptar_clicked", 0 );
    try {
        if ( guardar() ) {
            throw - 1;
        } // end if
        close();
    } catch ( ... ) {
        _depura ( "Error al guardar la ficha", 2 );
    } // end try
    _depura ( "END Ficha::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
void Ficha::on_mui_imprimir_clicked()
{
    _depura ( "Ficha::on_mui_imprimir_clicked", 0 );
    imprimir();
    _depura ( "END Ficha::on_mui_imprimir_clicked", 0 );
}


///
/**
**/
void Ficha::on_mui_eliminar_clicked()
{
    on_mui_borrar_clicked();
}


///
/**
**/
void Ficha::on_mui_borrar_clicked()
{
    _depura ( "Ficha::on_mui_borrar_clicked", 0 );

    int val = QMessageBox::question ( this,
                                      _( "Borrar" ) + " " + windowTitle(),
                                      _( "Desea eliminar '" ) + " " + windowTitle() + "' ?",
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default );

    if ( val == QMessageBox::Yes ) {
        if ( !borrar() ) {
            dialogChanges_cargaInicial();
            _depura ( windowTitle() + " " + "borrado satisfactoriamente.", 10 );
            close();
        } else {
            mensajeInfo ( windowTitle() + _( "No se ha podido borrar" ) );
        }// end if
    } // end if
    _depura ( "END Ficha::on_mui_borrar_clicked", 0 );
}


///
/**
\param e
\return
**/
void Ficha::closeEvent ( QCloseEvent *e )
{
    _depura ( "Ficha::closeEvent", 0, windowTitle() );
    try {
        if ( dialogChanges_hayCambios() ) {
            int val = QMessageBox::warning ( this,
                                             _( "Guardar" ) + " " + windowTitle(),
                                             _( "Desea guardar los cambios?" ),
                                             _( "&Si" ), _( "&No" ), _( "&Cancelar" ), 0, 2 );
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
        mensajeInfo ( _( "No se pudo cerrar la ventana debido a un error" ) );
        e->ignore();
    } // end try
    _depura ( "END Ficha::closeEvent", 0 );
}


///
/**
\return
**/
int Ficha::sacaWindow()
{
    _depura ( "Ficha::sacaWindow", 0 );
    if ( empresaBase() != NULL ) {
        empresaBase() ->sacaWindow ( this );
    } // end if
    _depura ( "END Ficha::sacaWindow", 0 );
    return 0;
}


///
/**
\param nom
\param obj
\param compdup
**/
void Ficha::meteWindow ( QString nom, QObject *obj, bool compdup )
{
    _depura ( "Ficha::meteWindow", 0 );
    if ( empresaBase() != NULL ) {
        empresaBase() ->meteWindow ( nom, obj, compdup );
    } // end if

    /// De Forma rapida hacemos un tratamiento de los permisos
    setDBTableName ( tableName() );
    /// Tal vez no es el mejor sitio para hacer la carga de SPECS. Pero no hay llamada especifica
    /// De configuración por lo que si no es este no es ninguno.
    cargaSpecs();
    _depura ( "END Ficha::meteWindow", 0 );
}


///
/**
**/
void Ficha::on_customContextMenuRequested ( const QPoint & )
{
    _depura ( "Ficha::on_customContextMenuRequested", 0 );
    QMenu *popup = new QMenu ( this );

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu ( popup );

    /// Lanzamos la propagacion del menu a traves de las clases derivadas.
    creaMenu ( popup );


    QAction *avconfig = popup->addAction ( _( "Opciones avanzadas de ficha" ) );
    QAction *avprint = popup->addAction ( _( "Imprimir ficha" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );

    if ( opcion ) {
        if ( opcion == avconfig ) {
            new FichaCfg ( empresaBase(), this, 0 );
        } else if ( opcion == avprint ) {
            Ficha::imprimir();
        } // end if

        emit trataMenu ( opcion );

        /// Activamos las herederas.
        procesaMenu ( opcion );
    } // end if

    delete popup;
    _depura ( "END Ficha::on_customContextMenuRequested", 0 );
}


///
/**
**/
void Ficha::creaMenu ( QMenu * )
{
    _depura ( "SubForm3:: CreaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
**/
void Ficha::procesaMenu ( QAction * )
{
    _depura ( "SubForm3:: procesaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
\param nom
**/
void Ficha::setDBTableName ( QString nom )
{
    _depura ( "Ficha::setDBTableName", 0 );
    DBRecord::setDBTableName ( nom );

    if (empresaBase() != NULL) {
	if ( !empresaBase() ->has_table_privilege ( nom, "INSERT" ) ) {
		/// Buscamos los permisos que tiene el usuario y desactivamos botones.
		QToolButton * b = findChild<QToolButton *> ( "mui_guardar" );
		if ( b ) b->setDisabled ( TRUE );
		b = findChild<QToolButton *> ( "mui_borrar" );
		if ( b ) b->setDisabled ( TRUE );
	} // end if
    } // end if

    _depura ( "END Ficha::setDBTableName", 0 );
}


///
/**
\param nom
**/
void Ficha::setTitleName ( QString nom )
{
    _depura ( "Ficha::setTitleName" );
    m_title = nom;
    _depura ( "END Ficha::setTitleName" );
}


///
/**
**/
void Ficha::pintar()
{
    _depura ( "Ficha::pintar", 0 );
    DBCampo *campo;
    /// Recorremos todos los campos definidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        /// Buscamos los QLineEdit con nombre coincidente.
        QLineEdit *l = findChild<QLineEdit *> ( "mui_" + campo->nomcampo() );
        if ( l ) {
            l->setText ( campo->valorcampo() );
        } // end if
        /// Buscamos los QTextEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *> ( "mui_" + campo->nomcampo() );
        if ( l3 ) {
            l3->setText ( campo->valorcampo() );
        } // end if
        /// Buscamos BLWidgets que coincidan con el campo supuestamente
        /// sirve para los campos personales.
        BLWidget *l1 = findChild<BLWidget *> ( "mui_" + campo->nomcampo() );
        if ( l1 ) {
            l1->setValorCampo ( campo->valorcampo() );
        } // end if
        /// Buscamos QComboBox2 que coincidan con el campo supuestamente
        /// sirve para los campos personales.
        QComboBox2 *l2 = findChild<QComboBox2 *> ( "mui_" + campo->nomcampo() );
        if ( l2 ) {
            l2->setValorCampo ( campo->valorcampo() );
        } // end if
        /// Buscamos los QCheckBox con nombre coincidente.
        QCheckBox *l5 = findChild<QCheckBox *> ( "mui_" + campo->nomcampo() );
        if ( l5 ) {
            if ( campo->valorcampo() == "t" ) {
                l5->setChecked ( TRUE );
            } else {
                l5->setChecked ( FALSE );
            } // end if
        } // end if

        /// Buscamos los 'Radio Buttons' y los preparamos.
        QList<QRadioButton2 *> l6 = findChildren<QRadioButton2 *> ( QRegExp ( "mui_" + campo->nomcampo() + "_*" ) );
        for ( int i = 0; i < l6.size(); ++i ) {
            if ( l6.at ( i ) ->valorCampo() == campo->valorcampo() ) {
                l6.at ( i ) ->setChecked ( TRUE );
            } else {
                l6.at ( i ) ->setChecked ( FALSE );
            } // end if
        } // end for
    } // end for
    pintarPost();
    _depura ( "END Ficha::pintar", 0 );
}


/** Recoge de forma automatica los valores que tienen los QLineEdit y los pone en la
    estructura de DBRecord.
    Para que funcionen bien los QLineEdit deben tener como nombre el mismo campo que
    el correspondiente en la base de datos precedidos de mui_
*/
/**
**/
void Ficha::recogeValores()
{
    _depura ( "Ficha::recogeValores", 0 );
    DBCampo *campo;
    /// Recorremos todos los campos definidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        /// Buscamos un QLineEdit con nombre coincidente.
        QLineEdit *l = findChild<QLineEdit *> ( "mui_" + campo->nomcampo() );
        if ( l )
            campo->set ( l->text() );

        /// Buscamos un QTextEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *> ( "mui_" + campo->nomcampo() );
        if ( l3 )
            campo->set ( l3->toPlainText() );

        /// Buscamos BLWidgets que coincidan con el campo. Supuestamente sirve para los campos personales.
        BLWidget *l1 = findChild<BLWidget *> ( "mui_" + campo->nomcampo() );
        if ( l1 )
            campo->set ( l1->valorCampo() );

        /// Buscamos QComboBox2 que coincidan con el campo. Supuestamente sirve para los campos personales.
        QComboBox2 *l2 = findChild<QComboBox2 *> ( "mui_" + campo->nomcampo() );
        if ( l2 ) {
            campo->set ( l2->valorCampo() );
	} // end for

        /// Buscamos un QCheckBox con nombre coincidente.
        QCheckBox *l5 = findChild<QCheckBox *> ( "mui_" + campo->nomcampo() );
        if ( l5 ) {
            if ( l5->isChecked() ) {
                campo->set ( "TRUE" );
            } else {
                campo->set ( "FALSE" );
            } // end if
        } // end if

        /// Buscamos los 'Radio Buttons' y los preparamos.
        QList<QRadioButton2 *> l6 = findChildren<QRadioButton2 *> ( QRegExp ( "mui_" + campo->nomcampo() + "_*" ) );
        if ( l6.size() > 0 ) {
            int aux = 0;
            for ( int i = 0; i < l6.size(); ++i ) {
                if ( l6.at ( i ) ->isChecked() ) {
                    campo->set ( l6.at ( i ) ->valorCampo() );
                    aux = 1;
                } // end if
            } // end for
            if ( aux == 0 ) {
                campo->set ( "" );
            } // end if
        } // end if

    } // end for
    _depura ( "END Ficha::recogeValores", 0 );
}


///
/**
\param id
\return
**/
int Ficha::cargar ( QString id )
{
    _depura ( "Ficha::cargar", 0, id );
    try {
        if ( DBRecord::cargar ( id ) ) {
            throw - 1;
        } // end if
        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "Ficha_cargar", this ) ) return 0;
        cargarPost ( id );
        setWindowTitle ( m_title + " " + DBvalue ( m_campoid ) );
        /// Activamos documentos adicionales
        activaDocumentos();

        pintar();
        dialogChanges_cargaInicial();
        meteWindow ( windowTitle(), this );
    } catch ( ... ) {
        _depura ( "END Ficha::cargar", 0, "Error en la carga" );
        return -1;
    } // end try
    _depura ( "END Ficha::cargar", 0 );
    return 0;
}


/// Guarda los datos de la ficha en la base de datos.
/**
  Este metodo guarda los contenidos de la Ficha (siempre que esta haya sido inicializada).
  Luego llama a plugins y a guardarPost por si se quieren hacer acciones adicionales de guardado.
  Tras guardar todos los elementos hace una carga.
\return 0 Si no hay problemas. -1 Si ha habido problemas.
**/
int Ficha::guardar()
{
    _depura ( "Ficha::guardar", 0 );

    try {
        QString id;
        recogeValores();
        empresaBase() ->begin();
        DBsave ( id );
        setDBvalue ( m_campoid, id );
        empresaBase() ->commit();

        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "Ficha_guardar_Post", this ) ) return 0;

        guardarPost();

        /// Hacemos una carga para que se actualizen datos como la referencia.
        cargar ( id );


        _depura ( "END Ficha::guardar", 0 );
        return 0;
    } catch ( int valor ) {
        /// Valor del error diferente a -1 significa que ya se ha mostrado algun mensaje de
        /// error y no hay que mostrar otro.
        if ( valor == -1 ) {
            mensajeInfo ( "Error inesperado al guardar" );
        } // end if
        empresaBase() ->rollback();
        return -1;

    } catch ( ... ) {
        mensajeInfo ( "Error inesperado al guardar" );
        empresaBase() ->rollback();
        return -1;
    } // end try
}


int Ficha::borrarPre()
{
    _depura ( "Ficha::borrarPre", 0 );
    _depura ( "END Ficha::borrarPre", 0 );
    return 0;
}


/** Borra la ficha en la base de datos
    Este metodo puede ser invocado desde la clase ficha o desde la pantalla previa
*/
/**
\return
**/
int Ficha::borrar()
{
    _depura ( "Ficha::borrar", 0 );
    try {
        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "Ficha_borrar", this ) ) return 0;
        borrarPre();
        int err;
        err =  DBRecord::borrar();
        _depura ( "END Ficha::borrar", 0 );
        return err;
    } catch ( ... ) {
        _depura ( "END Ficha::borrar", 0, "Error en el guardado" );
        return -1;
    } // end try
}


///
/**
\return
**/
int Ficha::guardarPost()
{
    _depura ( "Ficha::guardarPost", 0 );
    _depura ( "END Ficha::guardarPost", 0 );
    return 0;
}


///
/**
\return
**/
int Ficha::cargarPost ( QString )
{
    _depura ( "Ficha::cargarPost", 0 );
    _depura ( "END Ficha::cargarPost", 0 );
    return 0;
}

///
/**
\return
**/
void Ficha::pintarPost()
{
    _depura ( "Ficha::pintarPost", 0 );
    _depura ( "END Ficha::pintarPost", 0 );
}




/// Sustituye valores en el texto pasado como variables por su valor.
/// tipoEscape puede ser 0 --> Sin parseo
///			 1 --> ParseoXML
///			 2 --> ParseoPython

void Ficha::substrVars ( QString &buff, int tipoEscape ) {

	int pos = 0;

    /// Tratamos la sustitucion de variables de m_variables
	QMapIterator<QString, QString> i(m_variables);
	while (i.hasNext()) {
		i.next();
        buff.replace ( "[" + i.key() + "]", i.value() );
	} // end while


    substrConf(buff);

    pos =  0;
    
    /// Buscamos parametros en el query y los ponemos.
    QRegExp rx ( "\\[(\\w*)\\]" );
    while ( ( pos = rx.indexIn ( buff, pos ) ) != -1 ) {
        if ( exists ( rx.cap ( 1 ) ) ) {
		switch (tipoEscape) {
			case 1:
	            	buff.replace ( pos, rx.matchedLength(), xmlEscape(DBvalue ( rx.cap ( 1 ) )) );
			break;
			case 2:
	            	buff.replace ( pos, rx.matchedLength(), pythonEscape(DBvalue ( rx.cap ( 1 ) )) );
			break;
			default:
	            	buff.replace ( pos, rx.matchedLength(), DBvalue ( rx.cap ( 1 ) ) );

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
int Ficha::trataTags ( QString &buff, int tipoEscape )
{
    _depura ( "Ficha::trataTags", 0 );

    ///Buscamos interfaces, los preguntamos y los ponemos.
    int pos = 0;

    QRegExp rx70 ( "<!--\\s*IFACE\\s*SRC\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx70.setMinimal ( TRUE );
    while ( ( pos = rx70.indexIn ( buff, pos ) ) != -1 ) {
		QString cadarchivo = rx70.cap(1);
	
            substrConf(cadarchivo); 

	    QFile fichero(cadarchivo);
	    if (fichero.exists()) {
		QUiLoader loader;
		fichero.open(QFile::ReadOnly);
		QWidget *iface = loader.load(&fichero, this);
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
		diag->setGeometry(0, 0, iface->width(), iface->height());
		QPushButton *button = iface->findChild<QPushButton *>("mui_aceptar");
		connect ( button, SIGNAL ( released ( ) ), diag, SLOT ( accept() ) );
		QPushButton *button1 = iface->findChild<QPushButton *>("mui_cancelar");
		connect ( button1, SIGNAL ( released ( ) ), diag, SLOT ( reject() ) );
		int ret = diag->exec();
		if (ret) {
			QList<QLineEdit *> l2 = iface->findChildren<QLineEdit *>();
			QListIterator<QLineEdit *> it2 ( l2 );
			while ( it2.hasNext() ) {
				QLineEdit * item = it2.next();
				QString nombre = item->objectName().right(item->objectName().size()-4);
				QString valor = item->text();
				addDBCampo ( nombre, DBCampo::DBvarchar, DBCampo::DBNoSave, nombre  );
				setDBvalue ( nombre, valor );
			} // end while
		} // end if
		delete diag;
		// Si se ha pulsado cancelar entonce se sale del informe.
		if (!ret) return 0;
	    } else {
		mensajeInfo("Archivo de Interficie no existe");
	    }// end if

        buff.replace ( pos, rx70.matchedLength(), "" );
        pos = 0;
    } // end while


	/// Buscamos establecimiento de variables y los ponemos en m_variables
	pos = 0;
	QRegExp rx54 ( "<!--\\s*SETVAR\\s*NAME\\s*=\\s*\"([^\"]*)\"\\s*VALUE\\s*=\\s*\"([^\"]*)\"\\s*-->" );
	rx54.setMinimal ( TRUE );
	while ( ( pos = rx54.indexIn ( buff, pos ) ) != -1) {
		QString valor = rx54.cap(2);
		substrVars(valor, tipoEscape);
		m_variables[rx54.cap (1)] = valor;
		buff.replace( pos, rx54.matchedLength(), "");
		pos = 0;
	} // end while

	/// Tratamos las variables establecidas.
	substrVars(buff);




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

    _depura ( "END Ficha::trataTags", 0 );
    return 1;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString Ficha::trataIfQuery ( const QString &query, const QString &datos )
{
    _depura ( "Ficha::trataIfQuery", 0 );
    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
	substrVars(query1);

    /// Cargamos el query y lo recorremos
    cursor2 *cur = empresaBase() ->cargacursor ( query1 );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        result = datos;
    } // end while
    delete cur;
    _depura ( "END Ficha::trataIfQuery", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString Ficha::trataIf ( const QString &query, const QString &datos, const QString &datos1 )
{
    _depura ( "Ficha::trataIfQuery", 0 );
    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
	substrVars(query1);

    QString query2 = "SELECT (" + query1 + ") AS res";
    /// Cargamos el query y lo recorremos
    cursor2 *cur = empresaBase() ->cargacursor ( query2 );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        if ( cur->valor ( "res" ) == "t" ) {
            result = datos;
        } else {
            result = datos1;
        } // end if
    } // end while
    delete cur;
    _depura ( "END Ficha::trataIf", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString Ficha::trataQuery ( const QString &query, const QString &datos, int tipoEscape )
{
    _depura ( "Ficha::trataQuery", 0 );
    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
	substrVars(query1);

    /// Cargamos el query y lo recorremos
    result = trataCursor( empresaBase() ->cargacursor ( query1 ), datos, tipoEscape);
    _depura ( "END Ficha::trataQuery", 0 );
    return result;

}

QString Ficha::trataCursor ( cursor2 *cur, const QString &datos, int tipoEscape )
{
    _depura ( "Ficha::trataCursor", 0 );
    QString result = "";

    if ( !cur )  
    {
        _depura ( "END Ficha::trataCursor", 0 , "cur==NULL");
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
		switch (tipoEscape) {
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
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura ( "END Ficha::trataCursor", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QString Ficha::trataExists ( const QString &query, const QString &datos )
{
    _depura ( "Ficha::trataExists", 0 );

    QString result = "";
    QString query1 = query;

    /// Buscamos parametros en el query y los ponemos.
	substrVars(query1);

    QFile file ( query1 );
    if ( file.exists() )
        result = datos;
    _depura ( "END Ficha::trataExists", 0 );

    return result;
}


int Ficha::generaRML ( void )
{
   DBRecord::generaRML();
}
///
/**
**/
int Ficha::generaRML ( const QString &arch )
{
    _depura ( "Ficha::generaRML", 0 );

	/// Vaciamos las variables de RML
	m_variables.clear();

	/// Ponemos en m_variables CONF_DBNAME y CONF_DBUSER
	m_variables["CONF_DBNAME"] = empresaBase()->nameDB();
	m_variables["CONF_DBUSER"] = empresaBase()->currentUser();

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "Ficha_generaRML", this );
    if ( res != 0 ) {
        return 1;
    } // end if

    res = DBRecord::generaRML( arch );

    _depura ( "END Ficha::generaRML", 0 );
    return res;
}


///
/** Clase para ser derivada para activar documentos adicionales en las fichas.
**/
void Ficha::activaDocumentos ()
{
    _depura ( "Ficha::activaDocumentos", 0 );
    _depura ( "END Ficha::activaDocumentos", 0 );
}


///
/** Clase para ser derivada para desactivar documentos adicionales en las fichas.
**/
void Ficha::desactivaDocumentos ()
{
    _depura ( "Ficha::desactivaDocumentos", 0 );
    _depura ( "END Ficha::desactivaDocumentos", 0 );
}

