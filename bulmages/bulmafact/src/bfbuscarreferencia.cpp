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

#include "bfbuscarreferencia.h"
#include "bfcompany.h"
#include "bfinformereferencia.h"
#include "blform.h"
#include "blplugins.h"


/** Inicializa todos los componentes del Widget a NULL para que no haya
    confusion en determinar si un objeto ha sido creado o no.
*/
/**
\param parent
**/
BfBuscarReferencia::BfBuscarReferencia ( QWidget *parent )
        : BlWidget ( parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    connect ( mui_referencia, SIGNAL ( returnPressed() ), this, SLOT ( mui_rferencia_returnPressed() ) );

    /// Establecemos el focusProxy
    setFocusProxy (mui_referencia);
    
    
}


/** No requiere de acciones adicionales en la destruccion de la clase.
*/
/**
**/
BfBuscarReferencia::~BfBuscarReferencia()
{
    BL_FUNC_DEBUG
    
}


/** SLOT que responde a la peticion del informe por referencia.
    Crea una instancia de InformeReferencia, la inicializa y la lanza.
*/
/**
**/
void BfBuscarReferencia::on_mui_informe_released()
{
    BL_FUNC_DEBUG

    BlForm newfich ( mainCompany(), NULL );
    newfich.setTitleName ( _ ( "Informe" ) );
    newfich.addDbField ( "referencia", BlDbField::DbVarChar, BlDbField::DbPrimaryKey, _ ( "Referencia" ) );
    newfich.setDbValue ( "referencia", mui_referencia->text() );
    if ( newfich.generateRML ( "informereferencia.rml" ) ) {
        blCreateAndLoadPDF ( "informereferencia" );
    } // end if

    

}


/** SLOT que responde al fin de edicion de la casilla de referencia.
    Emite la se&ntilde;al valueChanged() para notificaciones.
*/
/**
**/
void BfBuscarReferencia::on_mui_referencia_editingFinished()
{
    BL_FUNC_DEBUG
    emit ( valueChanged ( mui_referencia->text() ) );
    
}


/** SLOT que responde a la pulsacion de RETURN sobre la casilla de referencia.
    Emite la se&ntilde;al returnPressed() para notificaciones.
*/
/**
**/
void BfBuscarReferencia::mui_rferencia_returnPressed()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins
    int res = g_plugins->run ( "BfBuscarReferencia_mui_rferencia_returnPressed", this );
    if ( res == 0 ) {
        emit ( returnPressed ( ) );
    } // end if
    
}


/** SLOT que responde a la pulsacion del boton abrirtodo.
    Busca en todos los documentos de la base de datos (presupuestos, pedidos, albaranes, facturas, cobros y pagos)
    Todos aquellos elementos que tengan la misma referencia que el objeto abierto.
    Instancia los objetos correspondientes y los lanza.
*/
/**
**/
void BfBuscarReferencia::on_mui_abrirtodo_released()
{
    BL_FUNC_DEBUG
    g_plugins->run ( "BfBuscarReferencia_on_mui_abrirtodo_clicked_Post", this );
}


/** Establece la referencia que va a mostrar el control
**/
/**
\param val
**/
void BfBuscarReferencia::setText ( QString val )
{
    BL_FUNC_DEBUG
    mui_referencia->setText ( val );
    
}


/** Establece la referencia que va a mostrar el control
**/
/**
\param val
**/
void BfBuscarReferencia::setFieldValue ( QString val )
{
    BL_FUNC_DEBUG
    mui_referencia->setText ( val );
    
}


/** Devuelve el texto que esta escrito en el control
**/
/**
\return
**/
QString BfBuscarReferencia::text()
{
    BL_FUNC_DEBUG
    
    return mui_referencia->text();
}


/** Devuelve el texto que esta escrito en el control
**/
/**
\return
**/
QString BfBuscarReferencia::fieldValue()
{
    BL_FUNC_DEBUG
    
    return mui_referencia->text();
}

