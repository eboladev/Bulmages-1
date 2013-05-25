/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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
#ifndef IMPQWRITER_H
#define IMPQWRITER_H

#include <QtCore/QString>
#include <QtWidgets/QMessageBox>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>

#include "bfcompany.h"
#include "blfunctions.h"
#include "cobroslist.h"


class Q19Writer : public QObject
{

  Q_OBJECT 
   
private:
    QString m_nif;
    QString m_nombreOrdenante;

    BfCompany *m_empresa;
    QString nifOrdenante(void);
    QString nifPresentador(void);

    QString nombreEmpresa(void);
    QString nombrePresentador(void);
    QString nombreOrdenante(void);
    
    QRegExp regex;

	///
	/**
  		longitud >0 alineacio dreta i <0 alineacio esquerra
	**/
	QString comprova(QString text, int longitud, QString nom, QChar farciment=' ' ) ;
    QString import(BlDbRecordSet *cur, QString nomcamp, int longitud) ;
    QString import(BlFixed f, int longitud) ;

    
protected:


	///
	/**
	\param out stream de sortida, que ja ha de saber traduir la codificacio' a q19
	\param sufijo:   la norma 19 el descriu aixi
	   CLIENTE ORDENANTE:
	se identifica por un co'digo de dos partes: Nu'mero de Identificacio'n Fiscal (
	N.I.F.) y Sufijo (Nu'mero de tres cifras que identifica los diferentes tipos de
	facturacio'n del cliente y/o los diferentes centros emisores de soportes o
	ficheros de facturacio'n).
	El NIF debera' ser el mismo y el Sufijo distinto cuando la facturacio'n sea
	repartida entre varias Entidades Receptoras, asignando un Sufijo distinto a
	cada Entidad.
	\param curbanco cursor sobre el registre de banc que rebr'a els ingressos.
	\return
	**/
	
	void cabeceraPresentador ( QTextStream &out, QString sufijo , BlDbRecordSet *curbanco );
	
	void cabeceraOrdenante ( QTextStream &out, QString sufijo , BlDbRecordSet *curbanco, QDate fechaCargo);
	
	int cobroQ19 ( QTextStream &out, QString sufijo,   BlDbRecordSet *curcobro );
	void totalOrdenante ( QTextStream &out,  QString sufijo , BlDbRecordSet *curbanco,
	                                        	BlFixed importes, int rebuts, int registros ) ;                         
	
	
	void totalPresentador (  QTextStream &out,  QString sufijo , BlDbRecordSet *curbanco,
	                                        	BlFixed importes, int rebuts, int registros , int ordenants);
	
	QWidget *parentWidget( void );
public:	
    Q19Writer ( BfCompany *emp );
    ~Q19Writer();

 /* genera el(s) fitxer(s) de remeses banc'aries
 se li passa el nom de fitxer a generar. Si hi ha me's d'un banc on ingressar
 els rebuts, genera un fitxer per cada banc, i els posa de nom filenane amb
 el nom del banc afegit abans de l'extensio' si n'hi ha.
 curcobro ha der ser un cursor amb les dades pels rebuts, un registre 
  per rebut. Ha de tenir les columnes
"bancocliente"  nu'm compte que paga
"cantcobro"    import
"comentcobro"  es posa al concepte (0-630 caracters)
"fechavenccobro"  data de c'arrec (ha de ser date sense hora, no timestamp)
"idbanco" id del banc (compte) que cobra a la taula banco (es fa servir pel sufijo)
"idcliente" es fa servir per la referencia del client 
"idcobro"  es fa servir pel codi de refer'encia interna de rebut i de devolucio'
"nomcliente"  qui paga
"refcobro"  es posa al principi del concepte
i no passa res si hi ha mes columnes
els registres han de venir ordenats per 
idbanco,fechavenccobro,entitat de bancocliente,oficina de bancocliente,idcliente
per exemple, serviria
SELECT * FROM cobro NATURAL LEFT JOIN cliente WHERE idcobro IN (...) ORDER BY cobro.idbanco,fechavenccobro,substring(cliente.bancocliente from 1 for 8),cliente.idcliente

Si li passes idsGenerats afegeix els ids dels registres correctament generats al
fitxer q19. Per'o si hi ha alguna excepcio interna per dades incorrectes, treu
un quadre de dialeg i esborra el idsGenerats, tant els ids que havia posat com
els que hi pogue's haver d'abans.

*/
    void genera(BlDbRecordSet *curcobro, QString filename, QStringList* idsGenerats=0);
};


#endif
