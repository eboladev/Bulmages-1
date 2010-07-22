/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef BTTICKET_H
#define BTTICKET_H

#include <QWidget>
#include <QCloseEvent>
#include <QLabel>

#include "blfunctions.h"
#include "blwidget.h"
#include "bldb.h"
#include "blfixed.h"

class BT_EXPORT BtTicket :  public BlWidget, public BlDbRecord
{
    Q_OBJECT

private:
    QList<BlDbRecord *> *m_listaLineas;
    BlDbRecord *m_lineaActual;
    QString m_nomTicketDefecto;
    QMap<QString,QString> m_variables;   /// Son elementos que han sido incrustados e inicializados por programas externos, generalmente el RML

public:
    virtual void pintar();

public:
    BtTicket ( BlMainCompany *emp = NULL, QWidget *parent = 0 );
    virtual ~BtTicket();
    QString nomTicketDefecto();
    BlDbRecord *agregarLinea();
    QList<BlDbRecord *> *listaLineas();
    /// Inserta o agrega cantidad de articulos al ticket
    /// El parametro nuevalinea indica que se inserte en nueva linea.
    BlDbRecord * insertarArticulo ( QString idArticulo, BlFixed cantidad = BlFixed ( "1" ), bool nuevaLinea = FALSE );
    void borrarArticulo ( BlDbRecord *linea, BlFixed cantidad = BlFixed ( "1" ) );
    void vaciarBtTicket();
    void subirPosArticulo ( BlDbRecord *linea, int filas = 1 );
    void bajarPosArticulo ( BlDbRecord *linea, int filas = 1 );
    void inicioPosBtTicket ( BlDbRecord * );
    void finPosBtTicket ( BlDbRecord * );
    BlDbRecord *lineaBtTicket ( int posicion );
    BlDbRecord *lineaActBtTicket();
    void setLineaActual ( BlDbRecord * );
    void setDescuentoGlobal ( BlFixed descuento );
    virtual void imprimir(bool save = TRUE);
    virtual void abrircajon();
    int guardar();
    int cargar ( QString );
    void borrarLinea ( BlDbRecord *linea );
    virtual QString exportXML();
    virtual bool syncXML(const QString &text, bool insertarSiempre = FALSE);

    virtual int generaRML ( const QString & );
    virtual int generaRML ( void );
    virtual int trataTags ( QByteArray &buff, int tipoEscape = 0 );
    virtual QByteArray trataIncludeFile ( const QString &file, int tipoEscape = 0 );
    virtual QByteArray trataIncludeImg ( const QString &file, int tipoEscape = 0 );
    virtual QByteArray trataSetCharacterPrintMode( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetCharacterSpacing( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetCharacterCodeTable( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetUnderlineMode( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetCharacterSize( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetSmoothing( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetDoubleStrike( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataTurnUpsideDown( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataTurn90CWRotation( const QString &param, const QString &param1, int tipoEscape = 0 );
    virtual QByteArray trataQuery ( const QString &query, const QByteArray &datos, int tipoEscape = 0 );
    virtual QByteArray trataTurnWhiteBlack( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetColor( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataHorizontalTab( int tipoEscape = 0 );
    virtual QByteArray trataSetHorizontalTabPos( const QString &param, const QString &param1, int tipoEscape = 0 );
    virtual QByteArray trataSetLeftMargin( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetPrintingAreaWidth( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetJustification( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetHAbsolutePos( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetHRelativePos( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetBarcodeFormat( const QString &param, const QString &param1, const QString &param2, const QString &param3, int tipoEscape = 0 );
    virtual QByteArray trataPrintBarCode( const QString &param, const QString &param1, const QString &param2, int tipoEscape = 0 );
    virtual QByteArray trataSetBarCodeWidth( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetBarCodeHeight( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSelectPageMode(int tipoEscape = 0 );
    virtual QByteArray trataSetPrintArea( const QString &param, const QString &param1, const QString &param2, const QString &param3, int tipoEscape = 0 );
    virtual QByteArray trataSetPrintDirection( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetVAbsolutePos( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetVRelativePos( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataCutPaper( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataCutPaperAndFeed( const QString &param,  const QString &param1, int tipoEscape = 0 );
    virtual QByteArray trataRightJustified( const QString &param, const QString &param1, const QString &param2, const QString &param3, int tipoEscape = 0 );
    virtual QByteArray trataLeftJustified( const QString &param, const QString &param1, const QString &param2, const QString &param3, int tipoEscape = 0 );

    QByteArray trataCursor ( BlDbRecordSet *cur, const QByteArray &datos, int tipoEscape = 0 );
    virtual QByteArray trataIfQuery ( const QString &query, const QByteArray &datos );
    virtual QByteArray trataIf ( const QString &query, const QByteArray &datos, const QByteArray &datos1 );
    virtual QByteArray trataExists ( const QString &query, const QByteArray &datos );
    void substrVars(QByteArray &buff, int tipoEscape = 0);


public slots:
    virtual void subir();
    virtual void bajar();
    virtual void agregarCantidad ( QString cantidad );
    virtual void ponerCantidad ( QString cantidad );
    virtual void ponerPrecio ( QString precio );
    virtual void insertarArticuloCodigo ( QString );
    virtual void insertarArticuloCodigoNL ( QString );
};

#endif
