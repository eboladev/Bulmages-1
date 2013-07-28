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

#include <QtWidgets/QMenu>

#include "blsearchwidget.h"
#include "blmaincompany.h"
#include "blplugins.h"


/** Inicializa la clase poniendo a NULL todos los valores para que no haya confusion
    en el hecho de que la clase aun no ha sido completamente inicializada.
*/
/**
\param parent
**/
BlSearchWidget::BlSearchWidget ( QWidget *parent )
        : BlWidget ( parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    m_textBusqueda->setText ( "" );
    mdb_id = "";
    m_campoid = "";

    /// Inicializamos los valores de vuelta a ""
    QMapIterator<QString, QString> i ( m_valores );
    while ( i.hasNext() ) {
        i.next();
        m_valores.insert ( i.key(), "" );
    } // end while

    m_semaforo = false;
    m_mask = "";
    

    /// Establecemos la delegacion del foco en el texto
    setFocusProxy(m_textBusqueda);
    
}


/** No requiere de acciones especiales en el destructor de clase.
*/
/**
**/
BlSearchWidget::~BlSearchWidget()
{
    BL_FUNC_DEBUG
    
}


/** Se encarga de presentar en el Widget los valores seleccionados.
*/
/** Esta funcion lanza un signal avisando de que se ha cambiado el id. Y debe tenerse en cuenta que el
    id puede estar vacio ya que tambien se puede haber borrado el que estaba puesto.
**/
void BlSearchWidget::pinta()
{
    BL_FUNC_DEBUG
    m_semaforo = true;
    QString cad = "";

    if (m_mask == "") {
      
	/// Iteramos y concatenamos"
	QMapIterator<QString, QString> i ( m_valores );
	if ( i.hasNext() ) {
	    i.next();
	    m_inputBusqueda->setText ( m_valores[i.key() ] );
	} // end if
	while ( i.hasNext() ) {
	    i.next();
	    cad = cad + " " + m_valores.value ( i.key() );
	}

    } else {
      
	cad = m_mask;
	/// Iteramos y reemplazamos
	QMapIterator<QString, QString> i ( m_valores );
	while ( i.hasNext() ) {
	    i.next();
	    cad.replace("["+i.key()+"]", m_valores.value(i.key()));
	}
      
      
    } // end if

    m_textBusqueda->setText ( cad );

    m_semaforo = false;
    emit ( valueChanged ( mdb_id ) );
    
}


/** Mediante este metodo indicamos al Widget que cargue un elemento determinado.
    Busca en la base de datos un elemento que coincida con el identificador pasado
    y si lo encuentra lo carga y lo presente.
    Dicho elemento se considera el elemento seleccionado.
    Al finalizar llama al metodo pinta() para que se refelejen visualmente los cambios.
*/
/**
\param val
**/
void BlSearchWidget::setId ( QString val, bool cargarvalores )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlSearchWidget::setId", 0, val );
    mdb_id = val;

    if ( m_tabla == "" || !cargarvalores) {
        return;
    } // end if

    if ( val == "" ) {
        m_inputBusqueda->setText ( "" );
        m_textBusqueda->setText ( "" );
        mdb_id = "";
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        while ( i.hasNext() ) {
            i.next();
            m_valores.insert ( i.key(), "" );
        } // end while
    } else {
        QString SQLQuery("");
	SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + m_campoid + "= $1";
        BlDbRecordSet *cur = mainCompany() ->load( SQLQuery, mdb_id );
        if ( !cur->eof() ) {
            /// Inicializamos los valores de vuelta a ""
            QMapIterator<QString, QString> i ( m_valores );
            while ( i.hasNext() ) {
                i.next();
                m_valores.insert ( i.key(), cur->value( i.key() ) );
            } // end while
        } // end if
        delete cur;
    } // end if
    pinta();
    
}


///
/**
\param val
**/
void BlSearchWidget::setFieldValue ( QString val )
{
    BL_FUNC_DEBUG
    setId ( val );
    
}

///
/**
\param val
**/
void BlSearchWidget::setText ( const QString &val )
{
    BL_FUNC_DEBUG
    m_inputBusqueda->setText(val);
    
}


/** Mediante este metodo le decimos al Widget cual es el CIF del elemento seleccionado.
    Busca en la base de datos cual el elemento cuyo CIF coincida con el que hemos seleccionado
    y si lo encuentra lo pinta y lo considera el elemento seleccionado.
    Al final llama al metodo pintar() para garantizar la correcta visualizacion del elemento en pantalla.
*/
/**
\param val
**/
void BlSearchWidget::setFieldValue ( QString campo, QString val )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlSearchWidget::setcifprofesor", 0, val );

    QString SQLQuery("");
    SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + campo + " = $1";
    BlDbRecordSet *cur = mainCompany() ->load ( SQLQuery, val );

    if ( !cur->eof() ) {
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        while ( i.hasNext() ) {
            i.next();
            m_valores.insert ( i.key(), cur->value( i.key() ) );
        } // end while
    } else {
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        while ( i.hasNext() ) {
            i.next();
            m_valores.insert ( i.key(), "" );
        } // end while
    } // end if
    delete cur;
    pinta();
    
}


/** Resetea el valor del 'id' y vacia la informacion del formulario.
*/
void BlSearchWidget::on_m_clearSearchWidget_clicked()
{
    BL_FUNC_DEBUG
    setId ( "" );
    
}


/** SLOT que responde a la accion de buscar.
    Abre el listado en modo Seleccionar y espera a que cerremos
    dicha ventana o que seleccionemos un elemento.
    Una vez determinado el elemento lo considera como el elemento seleccionado
    y lo muestra.
*/
/**
**/
void BlSearchWidget::on_m_buscarWidget_clicked()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins
    g_plugins->run ( "Busqueda_on_mui_buscar_clicked", this );
    
}


/** SLOT que responde al fin de la edicion del campos de texto.
    Se supone que hemos introducido un elemento identificable por codigo, por nombre o por CIF
    Asi que se encarga de buscarlo y encontrarlo.
*/
/**
**/
void BlSearchWidget::on_m_inputBusqueda_editingFinished()
{
    BL_FUNC_DEBUG
    pinta();
    g_plugins->run ( "Busqueda_on_m_inputBusqueda_editingFinished_Post", this );
    
}

/** SLOT que responde a la modificacion del campo de texto del Widget.
    A medida que vamos escribiendo en el campo de textos el sistema va buscando
    posibles elementos que cumplan las caracteristicas. Si encuentra solo uno lo
    considera como el elemento seleccionado.
*/
/**
\param val
\return
**/
void BlSearchWidget::on_m_inputBusqueda_textChanged ( const QString &val )
{
    BL_FUNC_DEBUG
    
    /// Si la cadena esta vacia entonces salimos sin hacer nada
    if  (val == "") {
        setId ( "" );
	
        return;
    } // end if
    
    if ( m_semaforo ) {
	
        return;
    } // end if


    if ( g_plugins->run ( "Busqueda_on_m_inputBusqueda_textChanged", this ) ) {
        return;
    } // end if


    QString cadwhere = "";
    /// Inicializamos los valores de vuelta a ""
    QMapIterator<QString, QString> i ( m_valores );
    QString cador = "";
    while ( i.hasNext() ) {
        i.next();
        cadwhere = cadwhere + cador + " " + i.key() + "::varchar = '" + mainCompany()->sanearCadenaUtf8(val) + "'";
        cador = " OR ";
    } // end while


    bool encontrado = false;
    QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + cadwhere;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_id = cur->value( m_campoid );
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        while ( i.hasNext() ) {
            i.next();
            m_valores[i.key() ] = cur->value( i.key() );
        } // end while
        encontrado = true;
    } // end if
    delete cur;



    if ( ! encontrado ) {
        cadwhere = "";
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        QString cador = "";
        while ( i.hasNext() ) {
            i.next();
            cadwhere = cadwhere + cador 
                        + " upper(" + i.key() + "::varchar ) LIKE  upper('%" 
                        + mainCompany()->sanearCadenaUtf8(val) + "%')";
            cador = " OR ";
        } // end while

        QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + cadwhere;
        cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_id = cur->value( m_campoid );
            /// Inicializamos los valores de vuelta a ""
            QMapIterator<QString, QString> i ( m_valores );
            while ( i.hasNext() ) {
                i.next();
                m_valores.insert ( i.key(), cur->value( i.key() ) );
            } // end while
            encontrado = true;
        } // end if
        delete cur;
    } // end if


    if ( !encontrado ) {
        m_textBusqueda->setText ( "" );
        mdb_id = "";
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        while ( i.hasNext() ) {
            i.next();
            m_valores[i.key() ] = "";
        } // end while
    } // end if

    if ( encontrado ) {
        QString cad = "";
        QMapIterator<QString, QString> i ( m_valores );
        while ( i.hasNext() ) {
            i.next();
            cad = cad + " " + i.value();
        } // end while
        m_textBusqueda->setText ( cad );
    } // end if
    

}


///
/**
\return
**/
QString BlSearchWidget::fieldValue ( QString campo )
{
    BL_FUNC_DEBUG
    
    return m_valores[campo];
}

///
/**
**/
QString BlSearchWidget::id()
{
    BL_FUNC_DEBUG
    
    return mdb_id;
}



///
/**
**/
void BlSearchWidget::setFieldId ( const QString &fieldId )
{
    BL_FUNC_DEBUG
    m_campoid= fieldId;
}

///
/**
\return
**/
QString BlSearchWidget::fieldValue()
{
    BL_FUNC_DEBUG
    
    return mdb_id;
}


void BlSearchWidget::setLabel ( QString label )
{
    mui_labelBusqueda->setText ( label  );
    mui_labelBusqueda->setBuddy ( m_buscarWidget );
}


QString BlSearchWidget::tableName()
{
    return m_tabla;
}

void BlSearchWidget::setTableName ( QString tableName )
{
    m_tabla = tableName;
    if (m_campoid == "") {
	m_campoid = "id" + m_tabla;
    } // end if
}

void BlSearchWidget::selectAll ( )
{
    m_inputBusqueda->selectAll ( );
}

void BlSearchWidget::setFocus ( )
{
    m_inputBusqueda->setFocus ( );
}

QString BlSearchWidget::text () {
    return m_inputBusqueda->text();
}

void BlSearchWidget::hideLabel(){
  mui_labelBusqueda->hide();
}

void BlSearchWidget::hideDescripcion(){
  m_textBusqueda->hide();
}

void BlSearchWidget::hideBotonBorrar(){
  m_clearSearchWidget->hide();
}

void BlSearchWidget::hideBotonBusqueda(){
  m_buscarWidget->hide();
}

QString BlSearchWidget::fieldId(){
  return m_campoid;
}

QString BlSearchWidget::mask(){
  return m_mask;
}


void BlSearchWidget::setMask(const QString & val){
  m_mask = val;
}



/// ===================================================================
/// Busqueda Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BlDbCompleterComboBox::BlDbCompleterComboBox ( QWidget *parent )
        : BlComboBox ( parent )
{
    BL_FUNC_DEBUG
    m_cursorcombo = NULL;
    setEditable ( true );
    /// Desconectamos el activated ya que en los subformularios no tiene que funcionar.
    disconnect ( this, SIGNAL ( activated ( int ) ), 0, 0 );
    connect ( this, SIGNAL ( editTextChanged ( const QString & ) ), this, SLOT ( s_editTextChanged ( const QString & ) ) );
    connect ( this, SIGNAL (customContextMenuRequested( const QPoint & )), this, SLOT ( popMenu(const QPoing &)));
    setContextMenuPolicy ( Qt::CustomContextMenu );
    
}


/** Libera la memoria reservada. */
/**
**/
BlDbCompleterComboBox::~BlDbCompleterComboBox()
{
    BL_FUNC_DEBUG
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BlDbCompleterComboBox::s_editTextChanged ( const QString &cod )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbCompleterComboBox::s_editTextChanged", 0, cod );
    static bool semaforo = false;
    if ( semaforo ) {
        return;
    } else {
        semaforo = true;
    } // end if
    m_entrada = cod;
    if ( !g_plugins->run ( "BlDbCompleterComboBox_textChanged", this ) ) {
        QString codigo = m_entrada;
        if ( codigo.size() >= g_confpr->value(CONF_NUMCHAR_RELOAD_FILTRO).toInt() ) {
            int pos = codigo.indexOf ( ".-" );
            // no se si es el autoComplete o què però em criden a
            // aquesta senyal quan omplo el combo, amb el primer valor
            // i si no m'aturo ara, recalcularia el combo amb nomes
            // aquest valor encara que l'usuari nomes hagi escrit
            // un prefix que permeti mes candidats
            if ( pos < 0 )  {
                QString cadwhere = "";
                /// Inicializamos los valores de vuelta a ""
                QMapIterator<QString, QString> i ( m_valores );
                QString cador = "";
                while ( i.hasNext() ) {
                    i.next();
                    cadwhere = cadwhere + cador + " upper( COALESCE(" + i.key() + ",'')::varchar ) LIKE  upper('%" + mainCompany()->sanearCadenaUtf8(cod) + "%')";
                    cador = " OR ";
                } // end while
                QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + cadwhere + " LIMIT 50";

                m_cursorcombo = mainCompany() ->loadQuery ( SQLQuery );
                clear();
                while ( !m_cursorcombo->eof() ) {
                    QMapIterator<QString, QString> i ( m_valores );
                    QString cad = "";
                    QString sep = ".- ";
                    QString cad1 = "";
                    while ( i.hasNext() ) {
                        i.next();
                        cad = cad + m_cursorcombo->value( i.key() ) + sep;
                        if ( sep == ".- " ) {
                            cad1 = i.key();
                            sep = " ";
                        } // end if
                    } // end while
                    addItem ( cad , QVariant ( m_cursorcombo->value( cad1 ) ) );
                    m_cursorcombo->nextRecord();
                } // end while
                delete m_cursorcombo;
            } // end if
        } // end if
    } // end if
    g_plugins->run ( "BlDbCompleterComboBox_textChanged_Post", this );
    setEditText ( cod );
    semaforo = false;
    
}

/// Retorna el codi associat a la unica entrada del combo que
/// hagi estat trobada a la BD a partir de l'entrada de l'usuari. Aixo
/// permet que abans de donar un error per codi d'article incorrecte
/// se li assigni l'unic article trobat per l'entrada (incompleta?) de l'usuari.
/// Retorna NULL si no se n'ha trobat cap o se n'ha trobat mes d'un.
QString BlDbCompleterComboBox::unicaEleccion ( void )
{
    BL_FUNC_DEBUG
    int num = 0;
    QString elec = NULL;
    for ( int i = 0; ( num < 2 ) && ( i < count() ); i++ ) {
        if ( itemData ( i ).isValid() ) {
            elec = itemData ( i ).toString();
            num++;
        } // end if
    } // end for
    
    return ( num == 1 ? elec : NULL );
}

/// Sii el combo nomes ha trobat un article a la BD per l'entrada de
/// l'usuari substitueix el text entrat per l'entrada del combo de l'article trobat.
QString BlDbCompleterComboBox::eligeUnico ( void )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbCompleterComboBox::eligeUnico", 0, "count = " + QString::number ( count() ) );

    QString elec = unicaEleccion();
    if ( !elec.isNull() ) {
        BlDebug::blDebug ( "elec=" + elec, 0 );
        setEditText ( elec );
    } // end if
    
    return elec;
}

/// quan deixa d'editar el camp substituim el que ha posat
/// per l'article que volia trobar si nomes hi ha un article candidat
void BlDbCompleterComboBox::focusOutEvent ( QFocusEvent * event )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbCompleterComboBox::focusOutEvent", 0, "count = " + QString::number ( count() ) );
    eligeUnico();
    BlComboBox::focusOutEvent ( event );
    
}

QString BlDbCompleterComboBox::entrada()
{
    BL_FUNC_DEBUG
    return m_entrada;
}


///
/**
**/
void BlDbCompleterComboBox::popMenu ( const QPoint & )
{
    BL_FUNC_DEBUG
    QMenu *popup = new QMenu ( this );

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu ( popup );

    QAction *avconfig = popup->addAction ( _ ( "Copiar " ) );
    QAction *avprint = popup->addAction ( _ ( "Pegar" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );

    if ( opcion ) {
        emit trataMenu ( opcion );
    } // end if

    delete popup;
}







///================================ BLDBEDITCOMBOBOX ================================



/// ===================================================================
/// Busqueda Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BlDbEditComboBox::BlDbEditComboBox ( QWidget *parent )
        : BlComboBox ( parent )
{
    BL_FUNC_DEBUG
    m_cursorcombo = NULL;
    setEditable ( true );
    /// Desconectamos el activated ya que en los subformularios no tiene que funcionar.
    disconnect ( this, SIGNAL ( activated ( int ) ), 0, 0 );
    connect ( this, SIGNAL (customContextMenuRequested( const QPoint & )), this, SLOT ( popMenu(const QPoing &)));
    setContextMenuPolicy ( Qt::CustomContextMenu );
}


/** Libera la memoria reservada. */
/**
**/
BlDbEditComboBox::~BlDbEditComboBox()
{
    BL_FUNC_DEBUG
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BlDbEditComboBox::setQuery ( const QString &query )
{
    BL_FUNC_DEBUG

    m_cursorcombo = mainCompany() ->loadQuery ( query );
    clear();
    while ( !m_cursorcombo->eof() ) {
	addItem ( m_cursorcombo->value(0) );
	m_cursorcombo->nextRecord();
    } // end while
    delete m_cursorcombo;
}


/// Retorna el codi associat a la unica entrada del combo que
/// hagi estat trobada a la BD a partir de l'entrada de l'usuari. Aixo
/// permet que abans de donar un error per codi d'article incorrecte
/// se li assigni l'unic article trobat per l'entrada (incompleta?) de l'usuari.
/// Retorna NULL si no se n'ha trobat cap o se n'ha trobat mes d'un.
QString BlDbEditComboBox::unicaEleccion ( void )
{
    BL_FUNC_DEBUG
    int num = 0;
    QString elec = NULL;
    for ( int i = 0; ( num < 2 ) && ( i < count() ); i++ ) {
        if ( itemData ( i ).isValid() ) {
            elec = itemData ( i ).toString();
            num++;
        } // end if
    } // end for
    
    return ( num == 1 ? elec : NULL );
}

/// Sii el combo nomes ha trobat un article a la BD per l'entrada de
/// l'usuari substitueix el text entrat per l'entrada del combo de l'article trobat.
QString BlDbEditComboBox::eligeUnico ( void )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbEditComboBox::eligeUnico", 0, "count = " + QString::number ( count() ) );

    QString elec = unicaEleccion();
    if ( !elec.isNull() ) {
        BlDebug::blDebug ( "elec=" + elec, 0 );
        setEditText ( elec );
    } // end if
    
    return elec;
}

/// quan deixa d'editar el camp substituim el que ha posat
/// per l'article que volia trobar si nomes hi ha un article candidat
void BlDbEditComboBox::focusOutEvent ( QFocusEvent * event )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbEditComboBox::focusOutEvent", 0, "count = " + QString::number ( count() ) );
    eligeUnico();
    BlComboBox::focusOutEvent ( event );
    
}

QString BlDbEditComboBox::entrada()
{
    BL_FUNC_DEBUG
    return m_entrada;
}


///
/**
**/
void BlDbEditComboBox::popMenu ( const QPoint & )
{
    BL_FUNC_DEBUG
    QMenu *popup = new QMenu ( this );

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu ( popup );

    QAction *avconfig = popup->addAction ( _ ( "Copiar " ) );
    QAction *avprint = popup->addAction ( _ ( "Pegar" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );

    if ( opcion ) {
        emit trataMenu ( opcion );
    } // end if

    delete popup;
}





