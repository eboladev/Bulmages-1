/***************************************************************************
 *   Copyright: Xavi Drudis Ferran <xdrudis@tinet.cat>, (C) 2009
 *   (this is a new file built from the previous work of Tomeu Borras Riera, 
 *   Arturo Martin Llado, and the IGLUES organization in general           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
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

#include <QtWidgets/QWidget>
#include "sepa20022writer.h"
#include "blfile.h"

/// Necesarios para importacion de efactura
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include "blfixed.h"
#include "bfcompany.h"


///
/**
\param cob
\param parent
**/
Sepa20022Writer::Sepa20022Writer ( BfCompany *emp ) : QObject()
{

	BL_FUNC_DEBUG
	m_empresa = emp;
	regex=QRegExp ( "(.{1,40})\\s(.*)",Qt::CaseInsensitive,QRegExp::RegExp2 );
	
}


///
/**
**/
Sepa20022Writer::~Sepa20022Writer()
{
	BL_FUNC_DEBUG
	
}


QWidget *Sepa20022Writer::parentWidget( )
{
	return 0; // els dialegs son application modal
}

///
/**
\return
**/
void Sepa20022Writer::genera ( BlDbRecordSet  *curcobro, QString fileName , QStringList *idsGenerats)
{
      BL_FUNC_DEBUG
      QString refActual ( "cap rebut" );
      if (fileName.length()==0) {
          fileName = QFileDialog::getSaveFileName ( parentWidget(), _ ( "Fichero de remesa bancaria (Cuaderno 19)" ),
                       "",
                       _ ( "*.q19;;*" ) );
      }

      BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Nombre del fichero: '%1'")).arg(fileName) );

      if (fileName.length()>0) { // else ha apretat cancel?lar
       
	try
	{
		int cobraments=curcobro->numregistros();
		BlDbRecordSet  *curbanc;
		/*
		http://www.cam.es/1/empresas/servicios/pdf/c19.pdf
		"    Dentro de cada Cliente Ordenante, todos los registros individuales debera'n
		figurar en el soporte clasificados ascendentemente por el nu'mero de Entidad-
		Oficina de adeudo, Referencia y Co'digo de dato, terminando con un registro de
		<<Total Ordenante>>. Al final llevara' un registro de <<Total General>>.
		"
		Per'o cada idbanco (de fet cada entitat, per'o no filem tan prim) requereix un
		fitxer, perqu'e no portar'as a un banc els rebuts que vols cobrar per un altre.
		I la data de c'arrec va a la capc,alera d'ordenant, per tant hem d'ordenar primer
		per banc i data i despre's pel que demana als rebuts d'un ordenant. Farem tantes capc,aleres
		d'ordenant com dates encara que sempre sigui el mateix ordenant.

		*/
		bool bancUnic = ( curcobro->value( "idbanco",0 ) == curcobro->value( "idbanco",curcobro->numregistros()-1 ) );
		BlDebug::blDebug ( "bancUnic=",0,bancUnic?"si":"no" );
		QString idbanc ( "" );
		BlFile file;
		QTextStream out ( &file );
		/*
		   http://www.cam.es/1/empresas/servicios/pdf/c19.pdf
		   - Codigo ASCII ( en mayusculas) (caracter 165= enye).
		   - Registros de longitud fija (162 bytes).
		   - Formato MS-DOS secuencial tipo texto.
		   En canvi un fitxer de mostra generat amb un programa que do'na el banc
		   te' 162 car'acters + \x0a , que no e's un salt de li'nia MSDOS. Ni se' si
		   un fitxer de registres de longitud fixa necessita  salts de li'nia per a res.
		*/
		out.setCodec ( "Sepa20022" );
		QString sufijo;
		QDate fechaCargo = ( curcobro->eof() ?
		                     QDate()
		                     : QDate::fromString ( curcobro->value( "fechavenccobro" ),"dd/MM/yyyy" ) ) ;
		BlFixed impOrdenante ( 0,2 );
		BlFixed impPresentador ( 0,2 );
		QString resultats ( "" );
		int registrosOrdenante=0;
		int registrosPresentador=0;
		int cobramentsOrdenante=0;
		int cobramentsPresentador=0;
		int ordenants=0;
		int sensebanc=0;
		int sensevenc=0;
		while ( !curcobro->eof() )
		{
			if ( QDate::fromString ( curcobro->value( "fechavenccobro" ),"dd/MM/yyyy" ).isValid() )
			{
				if ( ( !curcobro->value( "idbanco" ).isNull() ) && ( curcobro->value( "idbanco" ).length() >0 ) )
				{

					if ( QDate::fromString ( curcobro->value( "fechavenccobro" ),"dd/MM/yyyy" ) != fechaCargo )
					{
						registrosPresentador++;
						registrosOrdenante++;
						totalOrdenante ( out, sufijo , curbanc, impOrdenante,
						                 cobramentsOrdenante, registrosOrdenante );
						ordenants++;
					}
					if ( curcobro->value( "idbanco" ) != idbanc )
					{
						// canvi de banc on cobrem els rebuts, canvi de fitxer
						idbanc=curcobro->value( "idbanco" );
						if ( file.handle() !=-1 )
						{
							registrosPresentador++;
							totalPresentador ( out, sufijo, curbanc, impPresentador, cobramentsPresentador, registrosPresentador , ordenants );
							resultats += _ ( "\n%3 : %1 recibos, %2 EUR. " ).arg ( cobramentsPresentador ).arg ( impPresentador.toQString() ).arg ( file.fileName() );
							file.close();
							delete curbanc;

						}
						curbanc = m_empresa->loadQuery ( "SELECT * FROM banco WHERE idbanco = $1",1,&idbanc );
                                                sufijo = curbanc->value("sufijobanco");
						if ( bancUnic )
						{
							file.setFileName ( fileName );
							BlDebug::blDebug ( "creare' ",0,fileName );
						}
						else
						{
							QString nomNou = fileName;
							QRegExp ext ( "(\\..*)$" );
							QString extensio ( "" );
							if ( ext.indexIn ( nomNou ) >=0 )
							{
								extensio = ext.cap ( 1 );
							}
							nomNou.replace ( ext,"" );
							nomNou+="_"+curbanc->value( "nombanco" ).replace ( QRegExp ( "[^a-zA-Z0-9_]" ),"-" )
							        +extensio;
							file.setFileName ( nomNou );
							BlDebug::blDebug ( "creare' el nom que m'he muntat: ",0,fileName );
						}
						if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
							return;
						cabeceraPresentador ( out, sufijo , curbanc );
						cobramentsPresentador=0;
						registrosPresentador=1;
						impPresentador=0;
						ordenants=0;
						fechaCargo=QDate::fromString ( curcobro->value( "fechavenccobro" ),"dd/MM/yyyy" ) ;
					}
					if ( ( QDate::fromString ( curcobro->value( "fechavenccobro" ),"dd/MM/yyyy" ) != fechaCargo ) || ( registrosPresentador==1 ) )
					{
						fechaCargo = QDate::fromString ( curcobro->value( "fechavenccobro" ),"dd/MM/yyyy" );
						registrosPresentador++;
						cabeceraOrdenante ( out, sufijo , curbanc, fechaCargo );
						cobramentsOrdenante=0;
						registrosOrdenante=1;
						impOrdenante=0;
					}

					refActual=curcobro->value( "refcobro" );
					int registres = cobroSepa20022 ( out, sufijo, curcobro );
                                        if (idsGenerats) 
                                        { 
                                           idsGenerats->append(curcobro->value("idcobro"));
                                        }
					registrosOrdenante+=registres;
					registrosPresentador+=registres;
					cobramentsOrdenante++;
					cobramentsPresentador++;
					impOrdenante=impOrdenante+BlFixed ( curcobro->value( "cantcobro" ) );
					impPresentador=impPresentador+BlFixed ( curcobro->value( "cantcobro" ) );
				}
				else
				{
					sensebanc++;
				}
			}
			else
			{
				sensevenc++;
			}
			curcobro->nextRecord();
		}
		if ( file.handle() !=-1 )
		{
			registrosPresentador++;
			registrosOrdenante++;
			totalOrdenante ( out, sufijo , curbanc, impOrdenante, cobramentsOrdenante, registrosOrdenante );
			ordenants++;
						
			registrosPresentador++;
                        totalPresentador ( out, sufijo, curbanc, impPresentador, cobramentsPresentador, registrosPresentador , ordenants );
			resultats += _ ( "\n%3 : %1 recibos, %2 EUR. " ).arg ( cobramentsPresentador ).arg ( impPresentador.toQString() ).arg ( file.fileName() );
			file.close();
			delete curbanc;

		}
		if ( ( sensevenc>0 ) || ( sensebanc>0 ) )
		{
			QMessageBox::warning ( parentWidget(),_ ( "Remesa parcialmente generada" ),
			                       _ ( "Excluidos %3 de %2 recibos por falta de fecha de vencimiento y otros %1 por falta de banco" ).arg ( sensebanc ).arg ( cobraments ).arg ( sensevenc ) );
		}
		QMessageBox::information ( parentWidget(),_ ( "Fichero(s) generado(s)" ),_ ( "Tiene los siguientes ficheros de recibos para los bancos:\n%1" ).arg ( resultats ) );
	}
	catch ( QString e )
	{
		BlDebug::blDebug ( "Error ",0,refActual+":"+e );
		QMessageBox::critical ( parentWidget(),_ ( "Remesa mal generada" ),_( "%2\n.El fichero de remesa bancaria generado no es aprovechable. Ha fallado la generacion en el recibo con referencia %1" ).arg ( refActual ).arg ( e ) );
                if (idsGenerats) 
                {
                     idsGenerats->clear();
                }
	}
      }
	
}

QString Sepa20022Writer::nifOrdenante ( void )
{
	if ( m_nif.isNull() )
	{
		BlDbRecordSet *cur = m_empresa ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
		m_nif = cur->value( "valor" );
		delete cur;
		if ( m_nif.isNull() )
		{
			throw _ ( "Falta NIF de su empresa" ) +"\n"+
			_ ( "No podemos generar remesas bancarias hasta que entre el NIF de su empresa en Propiedades de Empresa" );

		}
		m_nif.replace ( "\x0a"," " );
	}
	return m_nif;
}

QString Sepa20022Writer::nifPresentador ( void )
{
	return nifOrdenante();
}

QString Sepa20022Writer::nombreEmpresa ( void )
{
	if ( m_nombreOrdenante.isNull() )
	{
		BlDbRecordSet *cur = m_empresa ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
		m_nombreOrdenante = cur->value( "valor" );
		delete cur;
		if ( m_nombreOrdenante.isNull() )
		{
			throw _ ( "Falta el nombre de su empresa" ) +"\n"+tr ( "No podemos generar remesas bancarias hasta que entre el nombre de su empresa en Propiedades de Empresa" );
		}
		m_nombreOrdenante.replace ( "\x0a"," " );
	}
	return m_nombreOrdenante;
}
QString Sepa20022Writer::nombrePresentador ( void )
{
	return nombreEmpresa();
}

QString Sepa20022Writer::nombreOrdenante ( void )
{
	return nombreEmpresa();
}

///
/**
  longitud >0 alineacio dreta i <0 alineacio esquerra
**/
QString  Sepa20022Writer::comprova ( QString text, int longitud, QString nom, QChar farciment )
{
	if ( text.size() > abs ( longitud ) )
	{
                m_nif=QString();
                m_nombreOrdenante=QString();
		throw        _ ( "Datos de recibos incorrectos" ) + "\n" +
		_ ( "%1 (%2) supera la longitud maxima (%3 caracteres)" )   .arg ( nom ).arg ( text ).arg ( abs ( longitud ) );
	}
	return QString ( "%1" ).arg ( text,longitud,farciment ).replace ( "\x0a"," " ) ;
}

///
/**
\param out stream de sortida, que ja ha de saber traduir la codificacio a sepa20022
\param sufijo:   la norma 19 el descriu aixi
   CLIENTE ORDENANTE:
se identifica por un codigo de dos partes: Numero de Identificacion Fiscal (
N.I.F.) y Sufijo (Numero de tres cifras que identifica los diferentes tipos de
facturacion del cliente y/o los diferentes centros emisores de soportes o
ficheros de facturacion).
El NIF debera ser el mismo y el Sufijo distinto cuando la facturacion sea
repartida entre varias Entidades Receptoras, asignando un Sufijo distinto a
cada Entidad.
\param curbanco cursor sobre el registre de banc que rebra els ingressos.
\return
**/
void Sepa20022Writer::cabeceraPresentador ( QTextStream &out, QString sufijo , BlDbRecordSet *curbanco )
{
	BL_FUNC_DEBUG

	/// CABECERA PRESENTADOR
	/// Generamos la cabecera presentador


	/// Registro en Euros. Longitud: 2
	out << ( "51" )  // pag. 25 diu 01, pag. 17 diu 51
	/// Registro de codigo de dato: 80. Longitud: 2
	<< ( "80" )
	/// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
	<< comprova ( nifPresentador(),9,_ ( "El CIF del presentador (propiedades de Empresa)" ),'0' )
	<< sufijo.rightJustified ( 3,'0',true )
	/// Fecha de emision del archivo
	<< ( QDate::currentDate().toString ( "ddMMyy" ) )
	/// Espacio libre Longitud: 6
	<< QString ( 6,' ' )
	/// Nombre del cliente Presentador Longitud: 40
	<< nombrePresentador().leftJustified ( 40,' ',true )
	/// Espacio libre Longitud: 20
	<< QString ( 20,' ' )
	/// Entidad Receptora del fichero Longitud: 4
	<< comprova ( curbanco->value( "codentidadbanco" ),-4,_ ( "Su entidad bancaria" ),'0' )
	/// Oficina Receptora del fichero Longitud: 4
	<< comprova ( curbanco->value( "codagenciabanco" ),-4,_ ( "Su oficina bancaria" ),'0' )
	/// Espacio libre Longitud: 12
	<<  QString ( 12, ' ' )
	/// Espacio libre Longitud: 40
	<< QString ( 40, ' ' )
	/// Espacio libre Longitud: 14
	<< QString ( 14, ' ' )
	/// Hi ha d'haver salts de linia o no?. Un fitxer d'exemple que tinc en porta.
	<< "\x0a";
	
}


///
/**
\param out
\param idcobro
\return
**/
void Sepa20022Writer::cabeceraOrdenante ( QTextStream &out, QString sufijo , BlDbRecordSet *curbanco, QDate fechaCargo )
{
	BL_FUNC_DEBUG

	/// GENERAMOS LA CABECERA ORDENANTE
	/// REGISTRO DEL ORDENANTE
	/// Registro en Euros. Longitud: 2
	out << ( "53" )  // pag. 18 diu 53 , pag. 25 diu 03
	/// Registro de codigo de dato: 80. Longitud: 2
	<< ( "80" )

	/// Codigo de ordenante (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
	<< comprova ( nifOrdenante(),9, _("El CIF del ordenante") ,'0' )
	<< sufijo.rightJustified ( 3,'0',true )

	/// Fecha de emision del archivo
	<< ( QDate::currentDate().toString ( "ddMMyy" ) )
	/// Fecha de cargo
	<< ( fechaCargo.toString ( "ddMMyy" ) )
	/// Nombre del cliente Ordenante Longitud: 40
	<< nombreOrdenante().leftJustified ( 40,' ',true )
	/// Entidad Receptora del fichero Longitud: 4
	<< comprova ( curbanco->value( "codentidadbanco" ),-4,_ ( "Su entidad bancaria" ),'0' )
	/// Oficina Receptora del fichero Longitud: 4
	<< comprova ( curbanco->value( "codagenciabanco" ),-4,_ ( "Su oficina bancaria" ),'0' )
	/// DC Receptora del fichero Longitud: 2
	<< comprova ( curbanco->value( "dcbanco" ) ,-2,
	              _ ( "Los digitos de control de su cuenta bancaria" ),'0' )
	/// Oficina Receptora del fichero Longitud: 10
	<< comprova ( curbanco->value( "numcuentabanco" ), -10,
	              _ ( "Su numero de cuenta bancaria" ) ,'0' )
	/// Espacio libre Longitud: 8
	<<  QString ( 8, ' ' )
	/// Procedimiento de realizacion del adeudo (01 o 02) Longitud: 2
	<< "01"
	/// Espacio libre Longitud: 10
	<< QString ( 10, ' ' )
	/// Espacio libre Longitud: 40
	<< QString ( 40, ' ' )
	/// Espacio libre Longitud: 14
	<< QString ( 14, ' ' )
	<< "\x0a";
}

QString Sepa20022Writer::import ( BlDbRecordSet *cur, QString nomcamp, int longitud )
{

	return import ( BlFixed ( cur->value( nomcamp ) ),longitud ) ;
}

QString Sepa20022Writer::import ( BlFixed f, int longitud )
{

	f.setPrecision ( 2 );
	QString res = QString::number ( f.value );
	if ( f.value < 0 )
	{
		throw _ ( "Datos incorrectos" ) +"\n" + _ ( "Importe negativo (%1) ! " ).arg ( f.toQString() );
	}
	if ( res.length() >longitud )
	{
		throw _ ( "Datos incorrectos" ) +"\n" + _ ( "Importe excesivo (%1) !. El limite son %2 digitos, incluyendo los 2 decimales " ).arg ( f.toQString() ).arg ( abs ( longitud ) );
	}
	return res.rightJustified ( longitud,'0',true );
}

///
/**
\param out
\param idcobro
\return
**/
int Sepa20022Writer::cobroSepa20022 ( QTextStream &out, QString sufijo,   BlDbRecordSet *curcobro )
{
	BL_FUNC_DEBUG
	QString bancocliente = curcobro->value( "bancocliente" ).remove ( QRegExp ( "[^0-9]" ) );
	if ( bancocliente.size() != 20 )
	{
		throw _ ( "Datos incorrectos" ) + "\n"+
		_ ( "Banco de Cliente '%1'  (para mi '%2') invalido en el recibo con id %3 (ref. %4), para cliente %5 (%6)" )
		.arg ( curcobro->value( "bancocliente" ) )
		.arg ( bancocliente )
		.arg ( curcobro->value( "idcobro" ) )
		.arg ( curcobro->value( "refcobro" ) )
		.arg ( curcobro->value( "nomcliente" ) )
		.arg ( curcobro->value( "idcliente" ) );
	}
	QString concepte = ( // "Ref:" + curcobro->value( "refcobro" )
	                     // +". "+
                             curcobro->value( "comentcobro" ) ).simplified();
	int i=0;
	for ( i=0; ( i<=5 ) && ( (i==0) || (concepte.length() >0) );i++ )   //num de registres d'aquest rebut depen de long de concepte
	{
		/// CABECERA INDIVIDUAL OBLIGATORIO
		/// Registro en Euros. Longitud: 2
		out <<  "56" //pag 18 diu 56, pag. 25 diu 06
		/// Registro de codigo de dato: 80. Longitud: 2
		<< ( 80+i )

		/// Codigo de ordenante (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
		<< comprova ( nifOrdenante(),9,_ ( "El CIF del ordenante" ),'0' )
		<< sufijo.rightJustified ( 3,'0',true )
		/// Codigo de referencia Longitud: 12
		<< curcobro->value( "idcliente" ).rightJustified ( 12, '0',true );

		if ( i==0 ) // registre individual obligatori
		{
			/// Nombre del titular de la domiciliacion: 40
			out << curcobro->value( "nomcliente" ).replace ( "\x0a"," " ).leftJustified ( 40, ' ', true )
			/// Entidad domiciliacion del fichero Longitud: 4
			/// Oficina domiciliacion del fichero Longitud: 4
			/// DC domiciliacion del fichero Longitud: 2
			/// Oficina domiciliacion del fichero Longitud: 10
			<< bancocliente

			/// Total Importe domiciliacion Longitud: 10
			<< import ( curcobro, "cantcobro" , 10 )

			/// Codigo para devoluciones: 6
			<< curcobro->value( "idcobro" ).leftJustified ( 6, ' ', true )
			/// Codigo para referencia interna
			<< curcobro->value( "idcobro" ).leftJustified ( 10, ' ', true )
			/// Primer campo de concepto Longitud: 40
			<< concepte.leftJustified ( 40, ' ',true )

			/// Espacio libre Longitud: 8
			<< QString ( 8, ' ' )
			<< "\x0a";
			concepte = concepte.mid ( 40 );
		}
		else   // registre opcional i-essim, per si no hi cap el concepte. Passem de 6e registre opcional.
		{
			// i==0 -> segon, tercer, quart camp de concepte, i==2 ->  cinque, sise, sete camp de concepte ...
			for ( int numCamp = ( ( i-1 ) *3 ) + 2; numCamp < i*3+2; numCamp++ )
			{
				BlDebug::blDebug ( "opcional inici ",0," numCamp="+QString::number ( numCamp )
				          +" concepte='"+concepte+"'" );
				bool finalLinia= ( ( concepte.length() >40 ) && ( numCamp % 2 ==0 ) );
				if ( finalLinia ) // cada camp es mitja linia, a final de linia partim per espai en blanc
				{
					//final de linia.
					finalLinia=regex.exactMatch ( concepte );
					if ( finalLinia )  {
						out << regex.cap ( 1 ).leftJustified ( 40,' ',true );
						concepte = regex.cap ( 2 );
					} // else hi ha mes de 40 caracters sense un espai, tractem com si fos principi de linia

				}
				BlDebug::blDebug ( "opcional mig ",0," numCamp="+QString::number ( numCamp )
				          +" concepte='"+concepte+"'" );

				if ( !finalLinia )  {
					out << concepte.leftJustified ( 40, ' ',true );
					concepte = concepte.mid ( 40 );
				} // end if
				BlDebug::blDebug ( "opcional fi ",0," numCamp="+QString::number ( numCamp )
				          +" concepte='"+concepte+"'" );

			} // fi for camps del registre
			/// Espacio libre Longitud: 14
			out << QString ( 14, ' ' )
			<< "\x0a";

		} // fi else registre opcional

	} //fi for i registres del rebut
	
	return i;
}


///
/**
\param out
\param importes
\param ordenantes
\param registros
\return
**/
void Sepa20022Writer::totalOrdenante ( QTextStream &out,  QString sufijo , BlDbRecordSet *curbanco,
                                 BlFixed importes, int rebuts, int registros )
{
	BL_FUNC_DEBUG
	/// CABECERA TOTAL ORDENANTE
	/// Registro en Euros. Longitud: 2
	out << "58" // pag. 28 diu 08, pag. 23 diu 58
	/// Registro de codigo de dato: 80. Longitud: 2
	<<"80"
	/// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
	<< comprova ( nifOrdenante(),9,tr ( "El CIF del ordenante" ),'0' )
	<< sufijo.rightJustified ( 3,'0',true )

	/// Espacio libre Longitud: 12
	/// Espacio libre Longitud: 40
	/// Espacio libre Longitud: 20

	<< QString ( 12+40+20, ' ' )

	/// Suma de Importes del Ordenante Longitud: 10
	<< import ( importes,10 )

	/// Espacio libre Longitud: 6
	<< QString ( 6, ' ' )

	/// Numero de domiciliaciones del ordenante : 10
	<< QString::number ( rebuts ).rightJustified ( 10, '0',true )

	/// Numero total registros del ordenante : 10
	<< QString::number ( registros ).rightJustified ( 10, '0',true )

	/// Espacio libre Longitud: 20
	/// Espacio libre Longitud: 18
	<< QString ( 20+18, ' ' )
	<<"\x0a";
	

}


void Sepa20022Writer::totalPresentador ( QTextStream &out,  QString sufijo , BlDbRecordSet *curbanco,
                                   BlFixed importes, int rebuts, int registros , int ordenants )
{
	BL_FUNC_DEBUG
	/// CABECERA TOTAL ORDENANTE
	/// Registro en Euros. Longitud: 2
	out << "59" //pag. 24 diu 59,pag. 29 diu 09
	/// Registro de codigo de dato: 80. Longitud: 2
	<<"80"
	/// Codigo de presentador (NIF + Sufijo alineado a la derecha y rellenado con ceros) Longitud: 12
	<< comprova ( nifPresentador(),9,_ ( "El CIF del presentador (propiedades de Empresa)" ),'0' )
	<< sufijo.rightJustified ( 3,'0',true )

	/// Espacio libre Longitud: 12
	/// Espacio libre Longitud: 40

	<< QString ( 12+40, ' ' )
	/// Num ordenants
	<< QString::number ( ordenants ).rightJustified ( 4,'0',true )
	/// Espacio libre Longitud: 16
	<< QString ( 16, ' ' )

	/// Suma de Importes del Ordenante Longitud: 10
	<< import ( importes,10 )

	/// Espacio libre Longitud: 6
	<< QString ( 6, ' ' )

	/// Numero de domiciliaciones del ordenante : 10
	<< QString::number ( rebuts ).rightJustified ( 10, '0',true )

	/// Numero total registros del ordenante : 10
	<< QString::number ( registros ).rightJustified ( 10, '0',true )

	/// Espacio libre Longitud: 20
	/// Espacio libre Longitud: 18
	<< QString ( 20+18, ' ' )
	<<"\x0a";
	

}


