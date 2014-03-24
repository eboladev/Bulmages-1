/***************************************************************************
 *   Copyright (C) 2014 by Tomeu Borras Riera                              *
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

#include <fstream>

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include "blfile.h"
#include <QtCore/QTextStream>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QCalendarWidget>
#include "bfcompany.h"
#include "blconfiguration.h"
//#include "bldatesearch.h"
#include "blfunctions.h"
#include "sepa20022view.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
Sepa20022View::Sepa20022View ( FacturasList *fac, BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        m_facturasList = fac;
        idsFacturas();
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_idbanco->setMainCompany ( comp );
        mui_idbanco->setidbanco ( "0" );

        m_q19 = new Sepa20022Writer ( comp );

        setDbTableName ( "factura" );
        setDbFieldId ( "idfactura" );

        mui_fecha->setDate(QDate::currentDate());
        mui_fecha->calendarWidget()->setFirstDayOfWeek(Qt::Monday);

        insertWindow ( windowTitle(), this, false );
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el archivo" ) );
    } // end try
    
}


/** No precisa acciones adicionales en el destructor.
*/
Sepa20022View::~Sepa20022View()
{
    BL_FUNC_DEBUG
    /// ATENCION: Hacer esto es un error ya que puede machacar procesos dependientes del listado.
    // ((BfCompany *)mainCompany())->refreshCobrosCliente();
    delete m_q19;
    
}

 
 //aquest pobre ignorant no sap fer-ho de cap altra manera. No hi hauria d'haver
 // un widget a qt per editar un nom de fitxer? 
 void Sepa20022View::on_mui_buscararchivo_clicked()
 {
    QString fileName =  QFileDialog::getSaveFileName ( this, _ ( "Fichero q19 a generar para el banco " ),
                                                         "",
                                                         _ ( "*.q19;;*" ) );
     if (fileName.length()>0) {
      mui_archivo->setText( fileName );
     }
 }
 
 const QString unitats[4] = {"days","weeks","months","years"};

 void Sepa20022View::idsFacturas ( void )
 {
 	BlSubForm *sub = m_facturasList->mui_list;
 	int factures = 0;
 	ids="";
 	for ( int i = 0; i < sub->rowCount(); i++ )
 	{
 		BlDbSubFormRecord *rec = sub->lineaat ( i );
 		rec->refresh();
 		QString val = rec->dbValue ( "selector" );
 		if ( val == "true" )
 		{
 			ids = ids + ( ids.length() >0 ?",":"" ) +rec->dbValue ( "idfactura" );
 			factures++;
 		} // end if
 	} // end for
 	if ( factures==0 )
 	{
 		QMessageBox::critical ( parentWidget(), _ ( "Remesa vacia" ),_ ( "No hay facturas seleccionadas. Puede utilizar la columna selector para seleccionar los recibos a incluir en la remesa bancaria." ) );
                 close();
 	}
}

///
/**
**/
void Sepa20022View::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    try {


          if (mui_idbanco->idbanco().trimmed().length()==0) 
          {
             QMessageBox::critical ( parentWidget(), _ ( "Falta el  banco"
), _ ( "Por favor, escoja un banco donde cobrar los recibos a generar." ) );
          }
 		
          else
          {
              QString fechacargo;
              if ( mui_fechafac->isChecked() )
              {
                   fechacargo="ffactura ";
              }
              else
              {
                   fechacargo="to_date('"+mui_fecha->date().toString ( "yyyy-MM-dd" )
                              +"','yyyy-MM-dd') ";
              }
              fechacargo+="+ '"+QString::number ( mui_dias->value() ) +
                          " " + unitats[mui_unidad->currentIndex()]+"  '::interval ";

              QString concepte = "'"+mainCompany()->sanearCadenaUtf8(mui_concepto->text())+"'";
              concepte.replace("%serie","'||coalesce(codigoserie_factura,'')||'");
              concepte.replace("%num","'||coalesce(numfactura::varchar,'')||'");
              concepte.replace("%coment","'||coalesce(comentfactura,'')||'");
              concepte.replace("%contact","'||coalesce(contactfactura,'')||'");
              concepte.replace("%desc","'||coalesce(descfactura,'')||'");
              concepte.replace("%base","'||coalesce(bimpfactura::varchar,'')||'");
              concepte.replace("%imp","'||coalesce(impfactura::varchar,'')||'");
              concepte.replace("%total","'||coalesce(totalfactura::varchar,'')||'");
              concepte.replace("%ref","'||coalesce(reffactura,'')||'");
              concepte.replace("%id","'||coalesce(idfactura::varchar,'')||'");
              concepte.replace(QRegExp("%fechafac\\[([^]]+)\\]"),
                               "'||coalesce(to_char(ffactura,'\\1'),'')||'");
              concepte.replace(QRegExp("%fecharec\\[([^]]+)\\]"),
                               "'||coalesce(to_char("+fechacargo+",'\\1'),'')||'");


              // per omissio "FRA %serie%num %desc "
                         
              BlDbRecordSet *cur = mainCompany()->loadQuery (
                                   " select "
                                   "cliente.bancocliente,"
                                   "totalfactura as cantcobro, "
                                   + concepte + " as comentcobro,"
                                   "("+fechacargo+")::date as fechavenccobro, "
                                   + mui_idbanco->idbanco() +" as  idbanco,"
                                   "factura.idcliente,"
                                   "(idfactura % 100000)::varchar ||'F' as idcobro, "
                                   "cliente.nomcliente,"
                                   "reffactura as refcobro "
                                   "FROM factura LEFT JOIN cliente on  factura.idcliente =  cliente.idcliente "
                                   "WHERE idfactura IN ("+ids+") "
                                   "ORDER BY  fechavenccobro,substring(cliente.bancocliente from 1 for  8),cliente.idcliente" );

              m_q19->genera ( cur,mui_archivo->text() );
              delete cur;

              dialogChanges_readValues();
              close();
          }
    } catch ( ... ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al generar remesa bancaria.") );
	blMsgError(_("Error al generar remesa bancaria."));
    } // end try
    
}


void Sepa20022View::closeEvent ( QCloseEvent *e ) {
   //no hay nada que guardar
   BL_FUNC_DEBUG 
   
}


