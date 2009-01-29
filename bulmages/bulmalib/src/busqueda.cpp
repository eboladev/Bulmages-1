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

#include "busqueda.h"
#include "empresabase.h"
#include "funcaux.h"
#include "plugins.h"

/** Inicializa la clase poniendo a NULL todos los valores para que no haya confusion
    en el hecho de que la clase aun no ha sido completamente inicializada.
*/
/**
\param parent
**/
Busqueda::Busqueda ( QWidget *parent )
        : BLWidget ( parent )
{
    _depura ( "Busqueda::Busqueda", 0 );
    setupUi ( this );
    /// Establecemos los Buddies:
    mui_labelBusqueda->setText ( tr( "Clien&te:" ) );
    mui_labelBusqueda->setBuddy ( mui_buscar );

    m_textBusqueda->setText ( "" );
    mdb_id = "";

	/// Inicializamos los valores de vuelta a ""
	QMapIterator<QString, QString> i(m_valores);
	while (i.hasNext()) {
		i.next();
		m_valores.insert(i.key(), "");
	}

    m_semaforo = FALSE;
    _depura ( "END Busqueda::Busqueda", 0 );
}


/** No requiere de acciones especiales en el destructor de clase.
*/
/**
**/
Busqueda::~Busqueda()
{
    _depura ( "Busqueda::~Busqueda", 0 );
    _depura ( "END Busqueda::~Busqueda", 0 );
}


/** Se encarga de presentar en el Widget los valores seleccionados.
*/
/** Esta funcion lanza un signal avisando de que se ha cambiado el id profesor. Y debe tenerse en cuenta que el
    idprofesor puede estar vacio ya que tambien se puede haber borrado el profesor que estaba puesto.
**/
void Busqueda::pinta()
{
    _depura ( "Busqueda::pinta", 0 );
    m_semaforo = TRUE;

	/// Inicializamos los valores de vuelta a ""
	QMapIterator<QString, QString> i(m_valores);
	if (i.hasNext()) {
		i.next();
		m_inputBusqueda->setText(m_valores[i.key()]);
	} // end if
	QString cad;
	while (i.hasNext()) {
		i.next();
		cad = cad +" "+ m_valores.value(i.key());
	}

    m_textBusqueda->setText ( cad );

    m_semaforo = FALSE;
    emit ( valueChanged ( mdb_id ) );
    _depura ( "END Busqueda::pinta", 0 );
}


/** Mediante este metodo indicamos al Widget que cargue un profesor determinado.
    Busca en la base de datos un profesor que coincida con el identificador pasado
    y si lo encuentra lo carga y lo presente.
    Dicho elemento se considera el elemento seleccionado.
    Al finalizar llama al metodo pinta() para que se refelejen visualmente los cambios.
*/
/**
\param val
**/
void Busqueda::setId ( QString val )
{
    _depura ( "Busqueda::setId", 0, val );
    mdb_id = val;

    if ( val == "" ) {
        m_inputBusqueda->setText ( "" );
        m_textBusqueda->setText ( "" );
        mdb_id = "";
			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			while (i.hasNext()) {
				i.next();
				m_valores.insert(i.key(), "");
			} // end while
    } else {
        QString SQLQuery = "SELECT * FROM " + m_tabla + " WHERE id" + m_tabla + "= '" + mdb_id + "'";
        cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur->eof() ) {

			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			while (i.hasNext()) {
				i.next();
				m_valores.insert(i.key(), cur->valor(i.key()));
			} // end while
        } // end if
        delete cur;
    } // end if
    pinta();
    _depura ( "END Busqueda::setId", 0 );
}


///
/**
\param val
**/
void Busqueda::setValorCampo ( QString val )
{
    _depura ( "Busqueda::setValorCampo", 0 );
    setId ( val );
    _depura ( "END Busqueda::setValorCampo", 0 );
}



/** Mediante este metodo le decimos al Widget cual es el CIF del profesor seleccionado.
    Busca en la base de datos cual el profesor cuyo CIF coincida con el que hemos seleccionado
    y si lo encuentra lo pinta y lo considera el elemento seleccionado.
    Al final llama al metodo pintar() para garantizar la correcta visualizacion del elemento en pantalla.
*/
/**
\param val
**/
void Busqueda::setValorCampo ( QString campo, QString val )
{
    _depura ( "Busqueda::setcifprofesor", 0, val );

    QString SQLQuery = "SELECT * FROM "+m_tabla+" WHERE "+campo+" = '" + val + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );

    if ( !cur->eof() ) {
			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			while (i.hasNext()) {
				i.next();
				m_valores.insert(i.key(), cur->valor(i.key()));
			} // end while
    } else {
			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			while (i.hasNext()) {
				i.next();
				m_valores.insert(i.key(), "");
			} // end while
    } // end if
    delete cur;
    pinta();
    _depura ( "END Busqueda::setcifprofesor", 0, val );
}


/** Resetea el valor del 'id_profesor' y vacia la informacion del formulario.
    Es util sobre todo el los filtros por profesor para anular el filtro.
*/
void Busqueda::on_mui_borrar_clicked()
{
    _depura ( "Busqueda::on_mui_borrar_clicked", 0 );
    setId ( "" );
    _depura ( "END Busqueda::on_mui_borrar_clicked", 0 );
}


/** SLOT que responde a la accion de buscar un profesor.
    Abre el listado de profesors en modo Seleccionar y espera a que cerremos
    dicha ventana o que seleccionemos un elemento.
    Una vez determinado el elemento lo considera como el elemento seleccionado
    y lo muestra.
*/
/**
**/
void Busqueda::on_mui_buscar_clicked()
{
    _depura ( "Busqueda::on_mui_buscar_clicked", 0 );
	mensajeInfo("funcion para ser derivada");

/*
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
    centrarEnPantalla ( diag );

    ClientsList *clients = new ClientsList ( ( Company * ) empresaBase(), diag, 0, ClientsList::SelectMode );
    connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( clients );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( clients->windowTitle() );

    diag->exec();
    if ( clients->idclient() != "" ) {
        setidprofesor ( clients->idclient() );
    } // end if
    delete diag;
*/
    _depura ( "END Busqueda::on_mui_buscar_clicked", 0 );
}


/** SLOT que responde al fin de la edicion del campos de texto.
    Se supone que hemos introducido un profesor identificable por codigo, por nombre o por CIF
    Asi que se encarga de buscarlo y encontrarlo.
*/
/**
**/
void Busqueda::on_m_inputBusqueda_editingFinished()
{
    _depura ( "Busqueda::on_m_inputBusqueda_editingFinished", 0 );
    pinta();
    g_plugins->lanza ( "Busqueda_on_m_inputBusqueda_editingFinished_Post", this );
    _depura ( "END Busqueda::on_m_inputBusqueda_editingFinished", 0 );
}

/** SLOT que responde a la modificacion del campo de texto del Widget.
    A medida que vamos escribiendo en el campo de textos el sistema va buscando
    posibles profesors que cumplan las caracteristicas. Si encuentra solo uno lo
    considera como el profesor seleccionado.
*/
/**
\param val
\return
**/
void Busqueda::on_m_inputBusqueda_textChanged ( const QString &val )
{
    _depura ( "Busqueda::on_m_inputBusqueda_textChanged", 0 );
    if ( m_semaforo ) {
        _depura ( "END Busqueda::on_m_inputBusqueda_textChanged", 0, "Semaforo parado" );
        return;
    } // end if

	QString cadwhere = "";
			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			QString cador = "";
			while (i.hasNext()) {
				i.next();
				cadwhere = cadwhere + cador + " "+i.key()+" = '"+val+"'";
				cador = " OR ";
			} // end while


    bool encontrado = FALSE;
    QString SQLQuery = "SELECT * FROM "+m_tabla+" WHERE "+ cadwhere;
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_id = cur->valor ( "id"+m_tabla );
			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			while (i.hasNext()) {
				i.next();
				m_valores[i.key()] == cur->valor(i.key());
			} // end while
        encontrado = TRUE;
    }
    delete cur;



    if ( ! encontrado ) {
		cadwhere = "";
			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			QString cador = "";
			while (i.hasNext()) {
				i.next();
				cadwhere = cadwhere + cador + " upper("+i.key()+") LIKE  upper('%"+val+"%')";
				cador = " OR ";
			} // end while

        QString SQLQuery = "SELECT * FROM profesor WHERE "+ cadwhere;
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_id = cur->valor ( "id"+m_tabla );
			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			while (i.hasNext()) {
				i.next();
				m_valores.insert(i.key(),cur->valor(i.key()));
			} // end while
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if


    if ( !encontrado ) {
        m_textBusqueda->setText ( "" );
        mdb_id = "";
			/// Inicializamos los valores de vuelta a ""
			QMapIterator<QString, QString> i(m_valores);
			while (i.hasNext()) {
				i.next();
				m_valores[i.key()] == "";
			} // end while
    } // end if

    if ( encontrado ) {
			QString cad = "";
			QMapIterator<QString, QString> i(m_valores);
			while (i.hasNext()) {
				i.next();
				cad = cad + " " + i.value();
			} // end while
        m_textBusqueda->setText ( cad );
    } // end if
    _depura ( "END Busqueda::on_m_cifprofesor_textChanged", 0 );

}


///
/**
\return
**/
QString Busqueda::valorCampo(QString campo)
{
    _depura ( "Busqueda::cifprofesor", 0 );
    _depura ( "END Busqueda::cifprofesor", 0 );
    return m_valores[campo];
}

///
/**
**/
QString Busqueda::id()
{
    _depura ( "Busqueda::idprofesor", 0 );
    _depura ( "END Busqueda::idprofesor", 0 );
    return mdb_id;
}


///
/**
\return
**/
QString Busqueda::valorCampo()
{
    _depura ( "Busqueda::valorCampo", 0 );
    _depura ( "END Busqueda::valorCampo", 0 );
    return mdb_id;
}


