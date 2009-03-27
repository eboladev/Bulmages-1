/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include "busquedaarticulo.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blplugins.h"

/** Inicializa todos los componentes a null para que no haya posibles
    errores al introducir el puntero a company. */
/**
\param parent
**/
BusquedaArticulo::BusquedaArticulo ( QWidget *parent )
        : BlSearchWidget ( parent )
{
    _depura ( "BusquedaArticulo::BusquedaArticulo", 0 );

		/// Establecemos los parametros de busqueda del Cliente
		setLabel ( _( "Articulo:" ) );
		setTableName( "articulo" );
		m_valores["codigocompletoarticulo"] = "";
		m_valores["nomarticulo"] = "";

    _depura ( "END BusquedaArticulo::BusquedaArticulo", 0 );
}


/** El destructor no requiere de acciones especiales.
*/
/**
**/
BusquedaArticulo::~BusquedaArticulo()
{
    _depura ( "BusquedaArticulo::~BusquedaArticulo", 0 );
    _depura ( "END BusquedaArticulo::~BusquedaArticulo", 0 );
}


/** Con este metodo indicamos al Widget que articulo presentar como cargado.
    Lo busca en la base de datos y muestra el codigocompleto y el nombre.
*/
/**
\param val
**/
void BusquedaArticulo::setidarticulo ( QString val )
{
    _depura ( "BusquedaArticulo::setidarticulo", 0 );
    setId(val);
    _depura ( "END BusquedaArticulo::setidarticulo", 0 );
}




/** Con este metodo indicamos cual es el articulo que debe presentar el Widget
    como seleccionado, indicando cual es su codigocompleto ya que es un indice
    muy utilizado con los articulos.
*/
/**
\param val
**/
void BusquedaArticulo::setcodigocompletoarticulo ( QString val )
{
    _depura ( "BusquedaArticulo::setcodigocompletoarticulo", 0 );
    QString SQLQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo='" + val + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        setId( cur->valor ( "idarticulo" ) );
    } else {
        setId("");
    } // end if
    delete cur;
    _depura ( "END BusquedaArticulo::setcodigocompletoarticulo", 0 );
}



///
/**
\return
**/
QString BusquedaArticulo::codigocompletoarticulo()
{
    _depura ( "BusquedaArticulo::codigocompletoarticulo", 0 );
    _depura ( "END BusquedaArticulo::codigocompletoarticulo", 0 );
    return m_valores["codigocompletoarticulo"];
}


///
/**
\return
**/
QString BusquedaArticulo::idarticulo()
{
    return id();
}

///
/**
\return
**/
QString BusquedaArticulo::nomarticulo()
{
    _depura ( "BusquedaArticulo::nomarticulo", 0 );
    _depura ( "END BusquedaArticulo::nomarticulo", 0 );
    return m_valores["nomarticulo"];
}


/// ===================================================================
/// Busqueda Articulo Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaArticuloDelegate::BusquedaArticuloDelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BusquedaArticuloDelegate::BusquedaArticuloDelegate", 0 );
    m_cursorcombo = NULL;
    setEditable ( true );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    connect ( this, SIGNAL ( editTextChanged ( const QString & ) ), this, SLOT ( s_editTextChanged ( const QString & ) ) );
    _depura ( "END BusquedaArticuloDelegate::BusquedaArticuloDelegate", 0 );
}


/** Libera la memoria reservada. */
/**
**/
BusquedaArticuloDelegate::~BusquedaArticuloDelegate()
{
    _depura ( "BusquedaArticuloDelegate::~BusquedaArticuloDelegate", 0 );
    _depura ( "END BusquedaArticuloDelegate::~BusquedaArticuloDelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BusquedaArticuloDelegate::s_editTextChanged ( const QString &cod )
{
	_depura ( "BusquedaArticuloDelegate::s_editTextChanged : "+cod, 0 );
	static bool semaforo = FALSE;



	if ( semaforo )
	{
		_depura ( "END BusquedaArticuloDelegate::s_editTextChanged (semafor ocupat)", 0 );
		return;
	}
	else
	{
		semaforo = TRUE;
	} // end if
	m_entrada = cod;
	if ( !g_plugins->lanza ( "BusquedaArticuloDelegate_textChanged", this ) )
	{
		QString codigo = m_entrada;
		if ( codigo.size() >= 3 )
		{
                    int pos = codigo.indexOf ( ".-" );
                    // no se si es el autoComplete o què però em criden a 
                    // aquesta senyal quan omplo el combo, amb el primer valor 
                    // i si no m'aturo ara, recalcularia el combo amb nomes 
                    // aquest valor encara que l'usuari nomes hagi escrit 
                    // un prefix que permeti mes candidats
                    if ( pos < 0 ) 
                    {
                        
			m_cursorcombo = mainCompany() ->loadQuery ( "SELECT codigocompletoarticulo, nomarticulo FROM articulo WHERE codigocompletoarticulo LIKE $1::text ORDER BY codigocompletoarticulo", 1, (QString[]){codigo + "%"},0,25 );
			clear();
			while ( !m_cursorcombo->eof() )
			{
				addItem ( m_cursorcombo->valor ( "codigocompletoarticulo" ) 
                                          + ".-" + m_cursorcombo->valor ( "nomarticulo" )
                                , QVariant(m_cursorcombo->valor ( "codigocompletoarticulo" ) ));
				m_cursorcombo->nextRecord();
			} // end while
			delete m_cursorcombo;

                }
		}
	}
	//    showPopup();
	g_plugins->lanza ( "BusquedaArticuloDelegate_textChanged_Post", this );
        setEditText ( cod );
   
	semaforo = FALSE;
	_depura ( "END BusquedaArticuloDelegate::s_editTextChanged", 0 );
}

/// Retorna el codi d'article associat a la unica entrada del combo que 
/// hagi estat trobada a la BD a partir de l'entrada de l'usuari. Això 
/// permet que abans de donar un error per codi d'article incorrecte 
/// se li assigni l'unic article trobat per l'entrada (incompleta?) de l'usuari.
/// Retorna NULL si no se n'ha trobat cap o se n'ha trobat mes d'un.
QString BusquedaArticuloDelegate::unicaEleccion(void) {
   int num = 0;
   QString elec = NULL;
   for(int i=0; (num<2)&&(i<count()); i++) {
       _depura("item "+QString::number(i)+". num= "+QString::number(num)
               +" itemText='"+itemText(i)+"' itemData="+itemData(i).toString(),0);
       if (itemData(i).isValid()) {
         _depura("aquest item es un article trobat, no entrada de l'usuari",0);
         elec= itemData(i).toString();
         num++;
       };
       _depura("END item "+QString::number(i)+". num= "+QString::number(num)
               +" itemText='"+itemText(i)+"' itemData="+itemData(i).toString(),0);
       
   }
   return (num==1?elec:NULL);
}

/// Sii el combo nomes ha trobat un article a la BD per l'entrada de 
/// l'usuari substitueix el text entrat per l'entrada del combo de l'article trobat.
QString BusquedaArticuloDelegate::eligeUnico(void) {
   _depura("BusquedaArticuloDelegate::eligeUnico. count=" 
           + QString::number(count()),0);
  
   QString elec = unicaEleccion();
   if (!elec.isNull()) 
   {  
      _depura("elec="+elec,0);
      setEditText(elec);
   } 
   _depura("END BusquedaArticuloDelegate::eligeUnico." ,0);
   return elec;
} 

/// quan deixa d'editar el camp substituim el que ha posat 
/// per l'article que volia trobar si nomes hi ha un article candidat
void BusquedaArticuloDelegate::focusOutEvent ( QFocusEvent * event ) {
   _depura("BusquedaArticuloDelegate::focusOutEvent. count=" 
           + QString::number(count()),0);
    eligeUnico();
   _depura("crido pare",0);
   BlComboBox::focusOutEvent(event);
   _depura("END BusquedaArticuloDelegate::focusOutEvent",0);
}

QString BusquedaArticuloDelegate::entrada() {
   return m_entrada;
}

