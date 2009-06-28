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
    _depura ( "BlSearchWidget::BlSearchWidget", 0 );
    setupUi ( this );
    m_textBusqueda->setText ( "" );
    mdb_id = "";

    /// Inicializamos los valores de vuelta a ""
    QMapIterator<QString, QString> i ( m_valores );
    while ( i.hasNext() ) {
        i.next();
        m_valores.insert ( i.key(), "" );
    }

    m_semaforo = FALSE;
    _depura ( "END BlSearchWidget::BlSearchWidget", 0 );
}


/** No requiere de acciones especiales en el destructor de clase.
*/
/**
**/
BlSearchWidget::~BlSearchWidget()
{
    _depura ( "BlSearchWidget::~BlSearchWidget", 0 );
    _depura ( "END BlSearchWidget::~BlSearchWidget", 0 );
}


/** Se encarga de presentar en el Widget los valores seleccionados.
*/
/** Esta funcion lanza un signal avisando de que se ha cambiado el id. Y debe tenerse en cuenta que el
    id puede estar vacio ya que tambien se puede haber borrado el que estaba puesto.
**/
void BlSearchWidget::pinta()
{
    _depura ( "BlSearchWidget::pinta", 0 );
    m_semaforo = TRUE;

    /// Inicializamos los valores de vuelta a ""
    QMapIterator<QString, QString> i ( m_valores );
    if ( i.hasNext() ) {
        i.next();
        m_inputBusqueda->setText ( m_valores[i.key() ] );
    } // end if
    QString cad;
    while ( i.hasNext() ) {
        i.next();
        cad = cad + " " + m_valores.value ( i.key() );
    }

    m_textBusqueda->setText ( cad );

    m_semaforo = FALSE;
    emit ( valueChanged ( mdb_id ) );
    _depura ( "END BlSearchWidget::pinta", 0 );
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
void BlSearchWidget::setId ( QString val )
{
    _depura ( "BlSearchWidget::setId", 0, val );
    mdb_id = val;

    if ( m_tabla == "" ) {
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
        QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE id" + m_tabla + "= $1";
        BlDbRecordSet *cur = mainCompany() ->load( SQLQuery, mdb_id );
        if ( !cur->eof() ) {

            /// Inicializamos los valores de vuelta a ""
            QMapIterator<QString, QString> i ( m_valores );
            while ( i.hasNext() ) {
                i.next();
                m_valores.insert ( i.key(), cur->valor ( i.key() ) );
            } // end while
        } // end if
        delete cur;
    } // end if
    pinta();
    _depura ( "END BlSearchWidget::setId", 0 );
}


///
/**
\param val
**/
void BlSearchWidget::setFieldValue ( QString val )
{
    _depura ( "BlSearchWidget::setFieldValue", 0 );
    setId ( val );
    _depura ( "END BlSearchWidget::setFieldValue", 0 );
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
    _depura ( "BlSearchWidget::setcifprofesor", 0, val );

    QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + campo + " = $1";
    BlDbRecordSet *cur = mainCompany() ->load ( SQLQuery, val );

    if ( !cur->eof() ) {
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        while ( i.hasNext() ) {
            i.next();
            m_valores.insert ( i.key(), cur->valor ( i.key() ) );
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
    _depura ( "END BlSearchWidget::setcifprofesor", 0, val );
}


/** Resetea el valor del 'id' y vacia la informacion del formulario.
*/
void BlSearchWidget::on_mui_clearSearchWidget_released()
{
    _depura ( "BlSearchWidget::on_mui_clearSearchWidget_released", 0 );
    setId ( "" );
    _depura ( "END BlSearchWidget::on_mui_clearSearchWidget_released", 0 );
}


/** SLOT que responde a la accion de buscar.
    Abre el listado en modo Seleccionar y espera a que cerremos
    dicha ventana o que seleccionemos un elemento.
    Una vez determinado el elemento lo considera como el elemento seleccionado
    y lo muestra.
*/
/**
**/
void BlSearchWidget::on_mui_buscar_released()
{
    _depura ( "BlSearchWidget::on_mui_buscar_released", 0 );
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "Busqueda_on_mui_buscar_released", this );
    _depura ( "END BlSearchWidget::on_mui_buscar_released", 0 );
}


/** SLOT que responde al fin de la edicion del campos de texto.
    Se supone que hemos introducido un elemento identificable por codigo, por nombre o por CIF
    Asi que se encarga de buscarlo y encontrarlo.
*/
/**
**/
void BlSearchWidget::on_m_inputBusqueda_editingFinished()
{
    _depura ( "BlSearchWidget::on_m_inputBusqueda_editingFinished", 0 );
    pinta();
    g_plugins->lanza ( "Busqueda_on_m_inputBusqueda_editingFinished_Post", this );
    _depura ( "END BlSearchWidget::on_m_inputBusqueda_editingFinished", 0 );
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
    _depura ( "BlSearchWidget::on_m_inputBusqueda_textChanged", 0 );
    if ( m_semaforo ) {
        _depura ( "END BlSearchWidget::on_m_inputBusqueda_textChanged", 0, "Semaforo parado" );
        return;
    } // end if


    if ( g_plugins->lanza ( "Busqueda_on_m_inputBusqueda_textChanged", this ) ) {
        return;
    } // end if


    QString cadwhere = "";
    /// Inicializamos los valores de vuelta a ""
    QMapIterator<QString, QString> i ( m_valores );
    QString cador = "";
    while ( i.hasNext() ) {
        i.next();
        cadwhere = cadwhere + cador + " " + i.key() + " = '" + mainCompany()->sanearCadenaUtf8(val) + "'";
        cador = " OR ";
    } // end while


    bool encontrado = FALSE;
    QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + cadwhere;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_id = cur->valor ( "id" + m_tabla );
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        while ( i.hasNext() ) {
            i.next();
            m_valores[i.key() ] == cur->valor ( i.key() );
        } // end while
        encontrado = TRUE;
    }
    delete cur;



    if ( ! encontrado ) {
        cadwhere = "";
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( m_valores );
        QString cador = "";
        while ( i.hasNext() ) {
            i.next();
            cadwhere = cadwhere + cador 
                        + " upper(" + i.key() + ") LIKE  upper('%" 
                        + mainCompany()->sanearCadenaUtf8(val) + "%')";
            cador = " OR ";
        } // end while

        QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + cadwhere;
        cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_id = cur->valor ( "id" + m_tabla );
            /// Inicializamos los valores de vuelta a ""
            QMapIterator<QString, QString> i ( m_valores );
            while ( i.hasNext() ) {
                i.next();
                m_valores.insert ( i.key(), cur->valor ( i.key() ) );
            } // end while
            encontrado = TRUE;
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
            m_valores[i.key() ] == "";
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
    _depura ( "END BlSearchWidget::on_m_cifprofesor_textChanged", 0 );

}


///
/**
\return
**/
QString BlSearchWidget::fieldValue ( QString campo )
{
    _depura ( "BlSearchWidget::fieldValue", 0 );
    _depura ( "END BlSearchWidget::fieldValue", 0 );
    return m_valores[campo];
}

///
/**
**/
QString BlSearchWidget::id()
{
    _depura ( "BlSearchWidget::id", 0 );
    _depura ( "END BlSearchWidget::id", 0 );
    return mdb_id;
}


///
/**
\return
**/
QString BlSearchWidget::fieldValue()
{
    _depura ( "BlSearchWidget::fieldValue", 0 );
    _depura ( "END BlSearchWidget::fieldValue", 0 );
    return mdb_id;
}


void BlSearchWidget::setLabel ( QString label )
{
    mui_labelBusqueda->setText ( label  );
    mui_labelBusqueda->setBuddy ( mui_buscar );
}


QString BlSearchWidget::tableName()
{
    return m_tabla;
}

void BlSearchWidget::setTableName ( QString tableName )
{
    m_tabla = tableName;
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
    _depura ( "BlDbCompleterComboBox::BlDbCompleterComboBox", 0 );
    m_cursorcombo = NULL;
    setEditable ( true );
//    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    /// Desconectamos el activated ya que en los subformularios no tiene que funcionar.
    disconnect ( this, SIGNAL ( activated ( int ) ), 0, 0 );
    connect ( this, SIGNAL ( editTextChanged ( const QString & ) ), this, SLOT ( s_editTextChanged ( const QString & ) ) );
    _depura ( "END BlDbCompleterComboBox::BlDbCompleterComboBox", 0 );
}


/** Libera la memoria reservada. */
/**
**/
BlDbCompleterComboBox::~BlDbCompleterComboBox()
{
    _depura ( "BlDbCompleterComboBox::~BlDbCompleterComboBox", 0 );
    _depura ( "END BlDbCompleterComboBox::~BlDbCompleterComboBox", 0 );
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
    _depura ( "BlDbCompleterComboBox::s_editTextChanged", 0, cod );
    static bool semaforo = FALSE;
    if ( semaforo ) {
        _depura ( "END BlDbCompleterComboBox::s_editTextChanged (semafor ocupat)", 0 );
        return;
    } else {
        semaforo = TRUE;
    } // end if
    m_entrada = cod;
    if ( !g_plugins->lanza ( "BlDbCompleterComboBox_textChanged", this ) ) {
        QString codigo = m_entrada;
        if ( codigo.size() >= 3 ) {
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
                    cadwhere = cadwhere + cador + " upper(" + i.key() + ") LIKE  upper('%" + mainCompany()->sanearCadenaUtf8(cod) + "%')";
                    cador = " OR ";
                } // end while
                QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE " + cadwhere;
                m_cursorcombo = mainCompany() ->loadQuery ( SQLQuery );
                clear();
                while ( !m_cursorcombo->eof() ) {
                    QMapIterator<QString, QString> i ( m_valores );
                    QString cad = "";
                    QString sep = ".- ";
                    QString cad1 = "";
                    while ( i.hasNext() ) {
                        i.next();
                        cad = cad + m_cursorcombo->valor ( i.key() ) + sep;
                        if ( sep == ".- " ) {
                            cad1 = i.key();
                            sep = " ";
                        } // end if
                    } // end while
                    addItem ( cad , QVariant ( m_cursorcombo->valor ( cad1 ) ) );
                    m_cursorcombo->nextRecord();
                } // end while
                delete m_cursorcombo;
            } // end if
        } // end if
    } // end if
    g_plugins->lanza ( "BlDbCompleterComboBox_textChanged_Post", this );
    setEditText ( cod );
    semaforo = FALSE;
    _depura ( "END BlDbCompleterComboBox::s_editTextChanged", 0 );
}

/// Retorna el codi associat a la unica entrada del combo que
/// hagi estat trobada a la BD a partir de l'entrada de l'usuari. Aixo
/// permet que abans de donar un error per codi d'article incorrecte
/// se li assigni l'unic article trobat per l'entrada (incompleta?) de l'usuari.
/// Retorna NULL si no se n'ha trobat cap o se n'ha trobat mes d'un.
QString BlDbCompleterComboBox::unicaEleccion ( void )
{
    _depura ( "BlDbCompleterComboBox::unicaEleccion", 0 );
    int num = 0;
    QString elec = NULL;
    for ( int i = 0; ( num < 2 ) && ( i < count() ); i++ ) {
        if ( itemData ( i ).isValid() ) {
            elec = itemData ( i ).toString();
            num++;
        } // end if
    } // end for
    _depura ( "END BlDbCompleterComboBox::unicaEleccion" );
    return ( num == 1 ? elec : NULL );
}

/// Sii el combo nomes ha trobat un article a la BD per l'entrada de
/// l'usuari substitueix el text entrat per l'entrada del combo de l'article trobat.
QString BlDbCompleterComboBox::eligeUnico ( void )
{
    _depura ( "BlDbCompleterComboBox::eligeUnico", 0, "count = " + QString::number ( count() ) );

    QString elec = unicaEleccion();
    if ( !elec.isNull() ) {
        _depura ( "elec=" + elec, 0 );
        setEditText ( elec );
    } // end if
    _depura ( "END BlDbCompleterComboBox::eligeUnico" , 0 );
    return elec;
}

/// quan deixa d'editar el camp substituim el que ha posat
/// per l'article que volia trobar si nomes hi ha un article candidat
void BlDbCompleterComboBox::focusOutEvent ( QFocusEvent * event )
{
    _depura ( "BlDbCompleterComboBox::focusOutEvent", 0, "count = " + QString::number ( count() ) );
    eligeUnico();
    BlComboBox::focusOutEvent ( event );
    _depura ( "END BlDbCompleterComboBox::focusOutEvent", 0 );
}

QString BlDbCompleterComboBox::entrada()
{
    _depura ( "BlDbCompleterComboBox::entrada", 0 );
    _depura ( "END BlDbCompleterComboBox::entrada", 0 );
    return m_entrada;
}





