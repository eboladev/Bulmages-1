/***************************************************************************
                          ivaview.cpp  -  description
                             -------------------
    begin                : Tue Jan 28 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera & Josep Burci�
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
  * \brief definiciones de las tablas en la base de datos involucradas.
  *
CREATE TABLE registroiva (
    idregistroiva serial PRIMARY KEY,
    contrapartida integer REFERENCES cuenta(idcuenta),
    baseimp double precision,
    iva double precision,
    ffactura date,
    factura character varying(70),
    idborrador integer,
    incregistro boolean,
    regularizacion boolean,
    plan349 boolean,
    numorden character varying(50),
    cif character varying(25),
    idfpago integer REFERENCES fpago(idfpago)
);
 
-- La tabla prevcobro es prevision de cobros / pagos para facturas.
-- Esta tabla proporciona las formas de pago de cada factura que se pasa a un cliente o que recibimos de un proveedor.
 
-- tipoprevcobro indica si es un cobro (true) o un pago (false).
-- fpagoprevcobro es un identificador a la otra tabla de Formas de Pago.
-- idcuenta La cuenta sobre la que se har�el cobro / pago.
-- idasiento Asiento de cobro (Si est�hecho).
-- cantidadprevistaprevcobro Cantidad Prevista del cobro.
-- cantidadprevcobro         Cantidad Cobrada.
-- fprevistaprevcobro        Fecha en que se prevee el cobro / pago.
-- fcobroprevcobro           Fecha en que se ha realizado el cobro / pago.
-- idregistroiva             Registro de IVA con el que se corresponde, o factura con la que se corresponde. 
 
CREATE TABLE prevcobro (
    idprevcobro serial PRIMARY KEY,
    fprevistaprevcobro date,
    fcobroprevcobro date,
    fpagoprevcobro integer,
    idcuenta integer REFERENCES cuenta(idcuenta),
    idasiento integer REFERENCES asiento(idasiento),
    cantidadprevistaprevcobro double,
    cantidadprevcobro         double,
    idregistroiva NOT NULL REFERENCES registroiva(idregistroiva),
    tipoprevcobro boolean
);
 
 
-- La tabla fpago lleva las formas de pago existentes
-- idfpago Identificador de la forma de pago
-- nomfpago Nombre de la forma de pago
-- nplazosfpago Numero de plazos de la forma de pago
-- plazoprimerpagofpago El plazo hasta el primer cobro/pago.
-- tipoplazoprimerpagofpago Tipo del plazo de la forma de pago
			0 - dias
			1 - semanas
			2 - meses
			3 - a�s
-- plazoentrerecibofpago numero de plazos en los siguientes recibos
-- tipoplazoentrerecibofpago (Igual que tipoplazoprimerpagofpago)
 
CREATE TABLE fpago (
    idfpago serial PRIMARY KEY,
    nomfpago character varying(50),
    nplazosfpago integer,
    plazoprimerpagofpago integer,
    tipoplazoprimerpagofpago integer,
    plazoentrerecibofpago integer,
    tipoplazoentrerecibofpago integer
);
 
*/

#include "ivaview.h"
#include "listcuentasview1.h"
#include "empresa.h"
#include "calendario.h"

#include <qregexp.h>
#include <qradiobutton.h>

#define IVA_SOPORTADO    1
#define IVA_REPERCUTIDO  2

#define COL_PREV_IDPREVCOBRO 0
#define COL_PREV_FPREVISTAPREVCOBRO 1
#define COL_PREV_FCOBROPREVCOBRO     2
#define COL_PREV_IDCUENTA  3
#define COL_PREV_CODCUENTA 4
#define COL_PREV_NOMCUENTA 5
#define COL_PREV_CANTIDADPREVISTAPREVCOBRO 6
#define COL_PREV_CANTIDADPREVCOBRO 7
#define COL_PREV_TIPOCOBRO 8
#define COL_PREV_IDREGISTROIVA 9
#define COL_PREV_DOCPREVCOBRO 10
#define COL_PREV_IDASIENTO 11
#define COL_PREV_ORDENASIENTO 12

#define COL_IVA_IDIVA 0
#define COL_IVA_IDTIPOIVA 1
#define COL_IVA_NOMBRETIPOIVA 2
#define COL_IVA_CTAIVA 3
#define COL_IVA_PORCENTAJETIPOIVA 4
#define COL_IVA_IDREGISTROIVA 5
#define COL_IVA_BASEIVA 6
#define COL_IVA_IVAIVA 7


#include "fixed.h"

//class fixed;

/**
  * \brief constructor de la clase
  * Inicializa la clase cargando el puntero de empresaactual y de conexionbase para que apunten a las clases amigas apropiadas.
  * Carga el numdigitos de la empresa.
  * Inicializa todos los componentes, principalmente inicializa las tablas, pone los titulos de las columnas y hace las que
  * no deben verse invisibles.
  * Tambi� carga el cursor de las formas de pago.
  */
ivaview::ivaview(empresa *emp,QWidget *parent, const char *name ) : ivadlg(parent,name) {
    empresaactual = emp;
    conexionbase = emp->bdempresa();
    idborrador=0;
    idregistroiva =0;
    m_listIva->setNumCols(8);
    m_listIva->horizontalHeader()->setLabel(COL_IVA_IDIVA,tr("COL_IVA_IDIVA") );
    m_listIva->horizontalHeader()->setLabel(COL_IVA_IDTIPOIVA,tr("COL_IVA_IDTIPOIVA") );
    m_listIva->horizontalHeader()->setLabel(COL_IVA_NOMBRETIPOIVA,tr("Tipo de IVA") );
    m_listIva->horizontalHeader()->setLabel(COL_IVA_CTAIVA,tr("Cuenta") );
    m_listIva->horizontalHeader()->setLabel(COL_IVA_BASEIVA,tr("Base Imponible") );
    m_listIva->horizontalHeader()->setLabel(COL_IVA_IDREGISTROIVA,tr("COL_IVA_IDREGISTROIVA") );
    m_listIva->horizontalHeader()->setLabel(COL_IVA_PORCENTAJETIPOIVA,tr("Porcentaje") );
    m_listIva->horizontalHeader()->setLabel(COL_IVA_IVAIVA,tr("IVA") );

    m_listIva->setNumRows(0);
    m_listIva->hideColumn(COL_IVA_IDIVA);
    m_listIva->hideColumn(COL_IVA_IDTIPOIVA);
    m_listIva->hideColumn(COL_IVA_IDREGISTROIVA);

    m_listPrevision->setNumCols(13);
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDPREVCOBRO, tr( "IDPREVCOBRO") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_FPREVISTAPREVCOBRO, tr( "FPREVISTAPREVCOBRO") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_FCOBROPREVCOBRO, tr( "FCOBROPREVCOBRO") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDCUENTA, tr( "IDCUENTA") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CODCUENTA, tr( "CODCUENTA") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_NOMCUENTA, tr( "NOMCUENTA") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CANTIDADPREVISTAPREVCOBRO, tr( "CANTIDADPREVISTAPREVCOBRO") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CANTIDADPREVCOBRO, tr( "CANTIDADPREVCOBRO") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDREGISTROIVA, tr( "IDREGISTROIVA") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_DOCPREVCOBRO, tr( "DOCPREVCOBRO") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_TIPOCOBRO, tr( "COBRO/PAGO") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDASIENTO, tr( "IDASIENTO") );
    m_listPrevision->horizontalHeader()->setLabel( COL_PREV_ORDENASIENTO, tr( "ORDENASIENTO") );
    
    m_listPrevision->setColumnWidth(COL_PREV_FPREVISTAPREVCOBRO,90);
    m_listPrevision->setColumnWidth(COL_PREV_FCOBROPREVCOBRO,90);
    m_listPrevision->setColumnWidth(COL_PREV_CODCUENTA,75);
    m_listPrevision->setColumnWidth(COL_PREV_CANTIDADPREVISTAPREVCOBRO,75);
    m_listPrevision->setColumnWidth(COL_PREV_CANTIDADPREVCOBRO,75);
    // Ocultamos las columnas que son de un tipo espec�ico.
    m_listPrevision->hideColumn(COL_PREV_IDCUENTA);
    m_listPrevision->hideColumn(COL_PREV_IDPREVCOBRO);
    m_listPrevision->hideColumn(COL_PREV_IDREGISTROIVA);
    m_listPrevision->hideColumn(COL_PREV_IDASIENTO);  
    m_listPrevision->hideColumn(COL_PREV_FCOBROPREVCOBRO);
    m_listPrevision->hideColumn(COL_PREV_CANTIDADPREVCOBRO);
    m_listPrevision->setNumRows(50);
    // CAlculamos las formas de pago.
    m_cursorFPago = NULL;
    cargarComboFPago("NULL");
}// end ivaview


/**
  * \brief Carga el combo de las formas de pago
  * 
  Esta funci� se encarga de cargar la tabla de formas de pago en el combo box correspondiente.
  Usa un cursor de clase (m_cursorFPago) que es construido al usar esta funcion y destruido en el destructor de clase.
  Esta funci� se llama con la inicializaci� de clase y cuando se quieren cargar datos.
*/
void ivaview::cargarComboFPago(QString idfpago) {
    conexionbase->begin();
    if (m_cursorFPago != NULL)
        delete m_cursorFPago;
    m_cursorFPago = conexionbase->cargacursor("SELECT * FROM fpago","unquery");
    conexionbase->commit();
    int i = 0;
    int i1 = 0;
    while (!m_cursorFPago->eof()) {
        i ++;
        if (m_cursorFPago->valor("idfpago") == idfpago) {
            i1 = i;
        }// end if
        m_fPago->insertItem(m_cursorFPago->valor("nomfpago"));
        m_cursorFPago->siguienteregistro();
    }// end while
    if (i1 != 0 ) {
        m_fPago->setCurrentItem(i1-1);
    }// end if
}// end cargarComboFPago

/**
  * \brief Destructor de clase.
  * Borra el cursor de las formas de pago para liberar la memoria reservada.
  */
ivaview::~ivaview() {
    delete m_cursorFPago;
}// end ~ivaview



/** \brief Este slot se ejeccuta cuando pusamos sobre el bot� aceptar.
  * Si se trata de una modificacion modificamos.
  * Si se trata de una insercion insertamos.
  */
void ivaview::accept() {
    QString query;
    QString idfactrectificada= "NULL";
    QString factura1= factura->text();
    /// Buscamos la factura rectificada si es que existe.
    if (m_numfactrectificada->text() != "") {
        query = "SELECT * FROM registroiva WHERE factura='"+m_numfactrectificada->text()+"' AND cif='"+cif->text()+"'";
        conexionbase->begin();
        cursor2 *cur = conexionbase->cargacursor(query,"elquery");
        conexionbase->commit();
        if (!cur->eof()) {
            idfactrectificada = cur->valor("idregistroiva");
        }// end if
    }// end if
    QString cif1 = cif->text();
    conexionbase->begin();
    cursor2 *cursorcuenta =conexionbase->cargacuenta(0,contrapartida->text());
    conexionbase->commit();

	    QString factemitida;
	    if (m_factEmitida->isChecked()) {
	    	factemitida="TRUE";
	    } else {
	    	factemitida="FALSE";
	    }// end if  
	          
    if (!cursorcuenta->eof()) {
        int idcuenta= atoi(cursorcuenta->valor("idcuenta").ascii());
        if (idregistroiva !=0) {

            /// Si se trata de una modificacion y hacemos el update.
            query.sprintf("UPDATE registroiva set factemitida=%s, idborrador=%d, contrapartida=%d, factura='%s', numorden='%s', cif='%s', ffactura='%s', rectificaaregistroiva = %s WHERE idregistroiva=%d",factemitida.ascii(), idborrador, idcuenta, factura1.ascii(), numorden->text().ascii(), cif1.ascii(), m_ffactura->text().ascii(), idfactrectificada.ascii() ,idregistroiva);
            conexionbase->begin();
            conexionbase->ejecuta(query);
            conexionbase->commit();
        } else {
            /// Se trata de una insercion y hacemos el insert.
            query="INSERT INTO registroiva (idborrador, baseimp, contrapartida, factura, numorden, cif, ffactura, rectificaaregistroiva, factemitida) VALUES ("+QString::number(idborrador)+","+m_baseImponible->text()+",  "+QString::number(idcuenta)+", '"+factura1+"', '"+numorden->text()+"', '"+cif1+"', '"+m_ffactura->text()+"',"+idfactrectificada+", "+factemitida+")";
            conexionbase->begin();
            conexionbase->ejecuta(query);
            /// Cargamos el identificador del registro de factura para poder mantener la clase.
            cursor2 *cur = conexionbase->cargacursor("SELECT MAX(idregistroiva) AS idregistroiva FROM registroiva", "elquery");
            conexionbase->commit();
            if (!cur->eof() )
                idregistroiva = cur->valor("idregistroiva").toInt();
            delete cur;
        }// end if
    }// end if
    delete cursorcuenta;
    /// Guardamos todas las previsiones de pago y asi nos curamos en salud
    guardaprevpago();
    guardaiva();
    done(1);
}// end accept


/** \brief Guardamos una unica linea de factura.
  * Para guardar una linea primero miramos si existe entrada en la base de datos correspondiente a dicha linea.
  * Si la entrada existe se hace un UPDATE y si no existe se hace un INSERT.
  */
void ivaview::guardaiva(int numrow) {
    QString idiva = m_listIva->text(numrow, COL_IVA_IDIVA);
    QString idregistroivastr;
    idregistroivastr.sprintf("%d",idregistroiva);
    if (idiva != "") {       // Ya un elemento, debemos hacer un UPDATE
        if (m_listIva->text(numrow, COL_IVA_IVAIVA) != "") { // hay que hacer un update
            QString SQLQuery = "UPDATE iva SET ";
            SQLQuery += " idtipoiva="+m_listIva->text(numrow, COL_IVA_IDTIPOIVA)+" ";
            SQLQuery += " , idregistroiva = "+idregistroivastr;
            SQLQuery += " , ivaiva = "+m_listIva->text(numrow, COL_IVA_IVAIVA);
            SQLQuery += " , baseiva = "+m_listIva->text(numrow, COL_IVA_BASEIVA);
            SQLQuery += " WHERE idiva="+idiva;
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
        } else { // Hay que hacer un DELETE
            conexionbase->begin();
            QString SQLQuery = "DELETE FROM iva WHERE idiva="+idiva;
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
        }// end if
    } else {
        if (m_listIva->text(numrow, COL_IVA_IVAIVA) != "") {
            // Hay que hacer un INSERT
            QString SQLQuery = "INSERT INTO iva (idtipoiva, idregistroiva, baseiva, ivaiva) VALUES (";
            SQLQuery += m_listIva->text(numrow, COL_IVA_IDTIPOIVA) + " , ";
            SQLQuery += idregistroivastr;
            SQLQuery += ","+m_listIva->text(numrow, COL_IVA_BASEIVA);	    
            SQLQuery += ","+m_listIva->text(numrow, COL_IVA_IVAIVA)  +") ";
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
        }// end if
    }// end if
}// end guardaiva

/** \brief Guardamos todo el registro de factura.
  */
void ivaview::guardaiva() {
    int i=0;
    while (i < m_listIva->numRows())
        guardaiva(i++);
}// end guardaiva


/** \brief Borramos el registro de factura.
 * Se ha pulsado sobre el boton de borrar en ivaview por lo que borraremos *
 * el registro actual.                                                     *
 */
void ivaview::boton_borrar() {
    QString query;
    if (idregistroiva != 0) {
        query.sprintf("DELETE FROM prevcobro WHERE idregistroiva=%d", idregistroiva);
        conexionbase->begin();
        conexionbase->ejecuta(query);
        conexionbase->commit();
        query.sprintf("DELETE FROM iva WHERE idregistroiva=%d", idregistroiva);
        conexionbase->begin();
        conexionbase->ejecuta(query);
        conexionbase->commit();
        query.sprintf("DELETE FROM registroiva WHERE idregistroiva=%d",idregistroiva);
        conexionbase->begin();
        conexionbase->ejecuta(query);
        conexionbase->commit();
    }// end if
    done(1);
}// end boton_borrar


/** \brief Calcula Totales y los presenta en el formulario.
  * Calculamos el resultado de las sumas de las tablas y presentamos los resultados en los campos correspondientes.
  * \todo aqui hay que cargarse este punto flotante como sea.
  * \todo Con esta forma de hacer las cosas hay un problema si los nmeros no llevan el formato de . y dos cifras decimales. (Hay que tenerlo muy en cuenta)
  */
void ivaview::calculaTotales() {
    Fixed base("0");
    Fixed iva("0");
    for (int i=0;i< m_listIva->numRows(); i++) {
        base = base + Fixed(m_listIva->text(i,COL_IVA_BASEIVA).ascii());
        iva = iva + Fixed(m_listIva->text(i,COL_IVA_IVAIVA).ascii());
    }// end for
    QString ivastr = iva.toQString();
    QString basestr = base.toQString();
    importeiva->setText(ivastr);
    m_baseImponible->setText(basestr);
}// end calculaTotales


/**
  * \brief SLOT que se ejecuta al pulsar sobre el boton de buscar una cuenta determinada
  * 
  * Crea una ventana \ref listcuentasview1 la pone en modo selecci�
  * La ejecuta en modo modal y espera a que termine
  * Pone en el campo contrapartida el valor devuelto por el selector de cuentas.
  * Simula la introducci� de la cuenta con la llamada \ref chContrapartida
  * Para que se actualize lo que se tenga que actualizar.
  */
void ivaview::boton_buscacuenta() {
    listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
    listcuentas->setModoLista();
    listcuentas->inicializa();
    listcuentas->exec();
    contrapartida->setText(listcuentas->codcuenta());
    chContrapartida();
    delete listcuentas;
}// end boton_buscacuenta

/**
  * \brief SLOT que se ejecuta al cambiar la cuenta
  */
void ivaview::cambiadacontrapartida() {
    QLineEdit *codigo = (QLineEdit *) sender();
    QString texto = contrapartida->text();
    if (texto == "+") {
        /// Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
        listcuentas->setModoLista();
        listcuentas->inicializa();
        listcuentas->exec();
        codigo->setText(listcuentas->codcuenta());
        delete listcuentas;
    }// end if
}// end cambiadacontrapartida



/** \brief Se ha pulsado return sobre la contrapartida
  */
void ivaview::chContrapartida() {
    QString cad = contrapartida->text();
    int numdigitos = empresaactual->numdigitosempresa();
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        conexionbase->begin();
        cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
        conexionbase->commit();
        int num = cursorcta->numregistros();
        if (num >0) {
            contrapartida->setText(cursorcta->valor("codigo"));
        }// end if
        QString SQLQuery = "SELECT * FROM cuenta WHERE codigo='"+contrapartida->text()+"'";
        conexionbase->begin();
        cursor2 *cur = conexionbase->cargacursor(SQLQuery,"buscacuenta");
        conexionbase->commit();
        if (!cur->eof()) {
            cif->setText(cur->valor("cifent_cuenta"));
            empfactura->setText(cur->valor("nombreent_cuenta"));
        }// end if
        delete cursorcta;
    }// end if
}// end return_codigofinal

/** \brief SLOT que se dispara cuando se ha hecho el bot� de bsqueda de una fecha.
  */
void ivaview::buscafecha(int idborrador) {
    QString SQLQuery;
    cursor2 * cur = conexionbase->cargacursor("SELECT fecha from borrador WHERE idborrador = "+QString::number(idborrador));
    if (!cur->eof()) {
        m_ffactura->setText(cur->valor("fecha").left(10));
    }// end if
    delete cur;
}

/** \brief busca la cuenta de IVA en el apunte que se ha seleccionado.
  * Esta funcion carga, dado un apunte y un asiento todas las cuentas de iva en el registro de iva 
  * que corresponden con la partida del asiento.
  * Los pasa en la tabla m_listIva. Tambien busca la fecha del asiento y la pone en m_ffactura
  */
int ivaview::buscaborradoriva(int idborrador) {
    fprintf(stderr,"BUSCABORRADORIVA\n");
    fprintf(stderr,"================\n");
    QString SQLQuery;
    SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT borrador.debe AS ivadebe, borrador.haber AS ivahaber, idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.fecha AS fecha  FROM borrador, cuenta WHERE borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador=%d)", idborrador);
    conexionbase->begin();
    conexionbase->ejecuta(SQLQuery);
    SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
    conexionbase->ejecuta(SQLQuery);
    conexionbase->commit();
    
    // Cargamos los registros que quedan pq seguro que son de IVA.
    // NOTA: Como con los cálculos de IVA a veces no funciona bien el redondeo a 2 decimales, vamos a tener siempre en cuenta 1 decimal para la comparacion.
    SQLQuery = "SELECT * FROM tipoiva LEFT JOIN lacosa ON tipoiva.idcuenta=lacosa.idcuenta LEFT JOIN (SELECT idcuenta, contrapartida, IVAdebe AS debe, IVAhaber AS haber FROM lacosa) AS base ON (base.debe*porcentajetipoiva/100)::NUMERIC(12,1)=lacosa.ivadebe::NUMERIC(12,1) AND (base.haber*porcentajetipoiva/100)::NUMERIC(12,2)=lacosa.ivahaber::NUMERIC(12,1) ORDER BY idtipoiva";
    
    cursor2 * cur=conexionbase->cargacursor(SQLQuery);
    m_listIva->setNumRows(cur->numregistros());
    int i=0;
    while (! cur->eof() ) {
        //fprintf(stderr,"BUSCABORRADORIVA: idborrador: %s contrapartida: %s cuenta: %s idtipoiva %s fecha %s\n",cur->valor("idborrador").ascii(), cur->valor("contrapartida").ascii(), cur->valor("codigo").ascii(), cur->valor("idtipoiva").ascii(), cur->valor("fecha").ascii());
	//fprintf(stderr,"BUSCABORRADORIVA: debe: %s pdebe: %s haber: %s phaber %s\n",cur->valor("debe").ascii(), cur->valor("pdebe").ascii(), cur->valor("haber").ascii(), cur->valor("phaber").ascii());
        m_listIva->setText(i,COL_IVA_IDIVA, cur->valor("idiva"));
        m_listIva->setText(i,COL_IVA_IDTIPOIVA, cur->valor("idtipoiva"));
        m_listIva->setText(i,COL_IVA_NOMBRETIPOIVA, cur->valor("nombretipoiva"));
        if (cur->valor("debe") != "0.00") {
            m_listIva->setText(i,COL_IVA_IVAIVA, cur->valor("ivadebe"));
	    m_listIva->setText(i,COL_IVA_BASEIVA, cur->valor("debe"));
        } else {
            m_listIva->setText(i,COL_IVA_IVAIVA, cur->valor("ivahaber"));
	    m_listIva->setText(i,COL_IVA_BASEIVA, cur->valor("haber"));
        }// end if
        m_listIva->setText(i,COL_IVA_IDREGISTROIVA, cur->valor("idregistroiva"));
        m_listIva->setText(i,COL_IVA_CTAIVA, cur->valor("codigo"));
        m_listIva->setText(i,COL_IVA_PORCENTAJETIPOIVA, cur->valor("porcentajetipoiva"));
        cur->siguienteregistro();
        i++;
    }// end while
    delete cur;
    conexionbase->begin();
    SQLQuery = "DROP TABLE lacosa";
    conexionbase->ejecuta(SQLQuery);
    conexionbase->commit();
    return 0;
}// end buscaborradoriva


/** \brief busca la cuenta del servicio
  *
  * Si hay varios servicios entonces tb deben pasarse en forma de lista. Aunque s�o se necesite uno.
  * \todo Los servicios no son todos los del grupo 6 y 7 son los 6x y 7x esto provoca que aparezca la ventana m� veces de las deseadas.
  * En estos momentos s�o se utiliza para calcular el total de la base imponible.
  */
int ivaview::buscaborradorservicio(int idborrador) {
    QString SQLQuery;
    int registro=0;
    SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.debe - borrador.haber AS baseimp FROM borrador, cuenta where borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador=%d)", idborrador);
    conexionbase->begin();
    conexionbase->ejecuta(SQLQuery);
    SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
    conexionbase->ejecuta(SQLQuery);
    
    /// Cogemos de la configuracion las cuentas que queremos que se apunten.
    /// Montamos los querys en base a la cadena cuentas.
    QString cuentas="";
    SQLQuery = "SELECT valor FROM configuracion WHERE nombre='CuentasIngresos'";
    cursor2 *cur=conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
    	cuentas += cur->valor("valor"); 
    }// end if
    delete cur;
    SQLQuery = "SELECT valor FROM configuracion WHERE nombre='CuentasGastos'";
    cur=conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
    	cuentas += ";"+cur->valor("valor"); 
    }// end if
    delete cur;
    cuentas.replace(';',"%|^");
    cuentas = "'^"+cuentas+"%'";
    
    /// Vamos a tener en cuenta tambien las cuentas de iva
    QString cuentasIVA="";
    SQLQuery = "SELECT valor FROM configuracion WHERE nombre='RegistroEmitida'";
    cur=conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
    	cuentasIVA += cur->valor("valor"); 
    }// end if
    delete cur;
    SQLQuery = "SELECT valor FROM configuracion WHERE nombre='RegistroSoportada'";
    cur=conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
    	cuentasIVA += ";"+cur->valor("valor"); 
    }// end if
    delete cur;
    cuentasIVA.replace(';',"%|^");
    cuentasIVA = "'^"+cuentasIVA+"%'";
    
    // Atentos que aqui es donde se incorpora el parametro.
    SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO "+cuentas+" OR codigo SIMILAR TO "+cuentasIVA;
    cur=conexionbase->cargacursor(SQLQuery);
    conexionbase->commit();
    while (! cur->eof() ) {
        fprintf(stderr,"idborrador: %s contrapartida: %s cuenta: %s\n",cur->valor("idborrador").ascii(), cur->valor("contrapartida").ascii(), cur->valor("codigo").ascii());
        registro = atoi(cur->valor("idborrador").ascii());
        cur->siguienteregistro();
    }// end while
    delete cur;

    /// Atentos que aqui es donde se calcula el total
    /// El cálculo se compara con el formato conocido de IVA (16, 7, 4: por tanto, formato de 2 dígitos con 0 decimales)
    SQLQuery = "SELECT abs(sum(baseimp)) AS subtotal FROM lacosa, (SELECT baseimp AS iva FROM lacosa WHERE codigo SIMILAR TO "+cuentasIVA+") AS iva WHERE codigo SIMILAR TO "+cuentas+" AND (iva.iva*100/baseimp)::NUMERIC(2,0) IN (SELECT porcentajetipoiva FROM tipoiva)";
    cur=conexionbase->cargacursor(SQLQuery);
    conexionbase->commit();
    if (! cur->eof() ) {
	m_baseImponible->setText(cur->valor("subtotal"));
    }// end while
    delete cur;    
    conexionbase->begin();
    SQLQuery = "DROP TABLE lacosa";
    conexionbase->ejecuta(SQLQuery);
    conexionbase->commit();
    return registro;
}// end if



/** \brief busca la cuenta del cliente o del proveedor en el apunte que se ha seleccionado
  *
  * Se basa en el supuesto de que si la cuenta no es el cliente entonces la contrapartida de la cuenta
  * Seguro que es el cliente.
  */
int ivaview::buscaborradorcliente(int idborrador) {
    QString SQLQuery;
    int registro=0;
    
    conexionbase->begin();
    SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.debe AS debe, borrador.haber AS haber, borrador.debe+borrador.haber AS totalfactura FROM borrador, cuenta where borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador=%d)", idborrador);
    conexionbase->ejecuta(SQLQuery);
    SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
    conexionbase->ejecuta(SQLQuery);
    conexionbase->commit();
    
    /// Cogemos de la configuracion las cuentas que queremos que se apunten.
    /// Montamos los querys en base a la cadena cuentas.
    /// Se consideran cuentas de Derechos y de Obligaciones a Clientes y Proveedores, respectivamente.
    /// Los campos sirven para encontrar la cuenta que corresponde a quien hace el pago de la factura.
    QString cuentas="";
    SQLQuery = "SELECT valor FROM configuracion WHERE nombre='CuentasDerechos'";
    cursor2 *cur1=conexionbase->cargacursor(SQLQuery);
    if (!cur1->eof()) {
    	cuentas += cur1->valor("valor"); 
    }// end if
    delete cur1;
    SQLQuery = "SELECT valor FROM configuracion WHERE nombre='CuentasObligaciones'";
    cur1=conexionbase->cargacursor(SQLQuery);
    if (!cur1->eof()) {
    	cuentas += ";"+cur1->valor("valor"); 
    }// end if
    delete cur1;
    cuentas.replace(';',"%|^");
    cuentas = "'^"+cuentas+"%'"; 
    
    /// Atentos que aqui es donde se incorpora el parametro.
    SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO "+cuentas;
    cursor2 * cur=conexionbase->cargacursor(SQLQuery, "buscaapunte");
    while (! cur->eof() ) {
        /// Ponemos la cuenta de Cliente y los valores adyacentes
        contrapartida->setText(cur->valor("codigo"));
	/// Comprobamos si es un cliente o un proveedor y segun sea actuamos en consecuencia.
	if (cur->valor("codigo").left(2) == "43") {
		m_factEmitida->setChecked(TRUE);
	} else {
		m_factSoportada->setChecked(TRUE);
	}// end if
	m_totalFactura->setText(cur->valor("totalfactura"));
        chContrapartida();
        registro = atoi(cur->valor("idborrador").ascii());
        cur->siguienteregistro();
    }// end while
    delete cur;
    SQLQuery = "DROP TABLE lacosa";
    conexionbase->begin();
    conexionbase->ejecuta(SQLQuery);
    conexionbase->commit();
    return registro;
}// end if


/** \brief Esta funcion se encarga de cargar la tabla de registro de IVA
  */
void ivaview::cargaiva(QString idregistroiva) {
    QString SQLQuery = "SELECT * FROM cuenta,tipoiva LEFT JOIN (SELECT * FROM iva WHERE idregistroiva = "+idregistroiva+" ) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE cuenta.idcuenta = tipoiva.idcuenta";
    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(SQLQuery, "Masquerys");
    conexionbase->commit();
    m_listIva->setNumRows(cur->numregistros());
    int i=0;
    while (! cur->eof()) {
        m_listIva->setText(i,COL_IVA_IDIVA, cur->valor("idiva"));
        m_listIva->setText(i,COL_IVA_IDTIPOIVA, cur->valor("idtipoiva"));
        m_listIva->setText(i,COL_IVA_NOMBRETIPOIVA, cur->valor("nombretipoiva"));
        m_listIva->setText(i,COL_IVA_IVAIVA, cur->valor("ivaiva"));
        m_listIva->setText(i,COL_IVA_BASEIVA, cur->valor("baseiva"));
        m_listIva->setText(i,COL_IVA_IDREGISTROIVA, cur->valor("idregistroiva"));
        m_listIva->setText(i,COL_IVA_CTAIVA, cur->valor("codigo"));
        m_listIva->setText(i,COL_IVA_PORCENTAJETIPOIVA, cur->valor("porcentajetipoiva"));
        cur->siguienteregistro();
        i++;
    }// end while
    delete cur;
    calculaTotales();
}// end cargaiva


/**
  * \brief Inicializa el registro de iva. Calculando los datos que son buscables (O encontrables a partir del asiento).
  * Esquema:
  * 1.- Comprobamos que no haya ya un registro con la factura
  * 2.- Si hay factura la ponemos, si no la hay sacamos los datos que podemos y los presentamos
  */
void ivaview::inicializa1(int idapunte1) {
    QString query, cadena;
    int numfact, numord;
    /// Busca si a este apunte le corresponde un borrador
    idborrador = buscaborradorcliente(idapunte1);
    /// En el caso de que no existiese una cuenta de servicio a la que asociar se
    /// Asocia directamente al apunte seleccionado.
    if (idborrador <= 0)
        idborrador = idapunte1;
    ///Busca entradas de IVA en la tabla registroiva
    query.sprintf ( "SELECT * FROM registroiva, cuenta WHERE idborrador=%d AND registroiva.contrapartida=cuenta.idcuenta",idborrador);
    cursor2 *cursoriva = conexionbase->cargacursor(query,"cursoriva");
    if (!cursoriva->eof()) {
        /// Se trata de un registro que ya ha sido introducido
        query.sprintf ("SELECT * from borrador, asiento, cuenta WHERE borrador.idborrador=%d AND borrador.idasiento=asiento.idasiento AND borrador.idcuenta=cuenta.idcuenta",idborrador);
        conexionbase->begin();
        cursor2 * cursorapunte = conexionbase->cargacursor(query,"cursorapunte");
        conexionbase->commit();
        if (!cursorapunte->eof()) {
            numasiento->setText(cursorapunte->valor("ordenasiento"));
        }// end if
        idregistroiva=atoi(cursoriva->valor("idregistroiva").ascii());
        contrapartida->setText(cursoriva->valor("codigo"));
        chContrapartida();
        empfactura->setText(cursoriva->valor("nombreent_cuenta"));
        factura->setText(cursoriva->valor("factura"));
        numorden->setText(cursoriva->valor("numorden"));
        cif->setText(cursoriva->valor("cif"));
        m_ffactura->setText(cursoriva->valor("ffactura"));
	
	/// Presentamos si es emitida o recibida.
	if(cursoriva->valor("factemitida") == "t") {
		m_factEmitida->setChecked(TRUE);
	} else {
		m_factSoportada->setChecked(TRUE);
	}// end if
        delete cursorapunte;
	
        /// Si la factura rectifica a otra la buscamos.
        if (cursoriva->valor("rectificaaregistroiva").toInt() >0) {
            QString query = "SELECT * FROM registroiva WHERE idregistroiva="+cursoriva->valor("rectificaaregistroiva");
            cursor2 *currect= conexionbase->cargacursor(query,"FRectificada");
            if (!currect->eof() ) {
                m_numfactrectificada->setText(currect->valor("factura"));
            }// end if
            delete currect;
        }// end if
        cargaiva(cursoriva->valor("idregistroiva"));
        m_baseImponible->setText(cursoriva->valor("baseimp"));
    } else {
        ///buscamos en todo el asiento las cuentas de IVA y lo reflejamos
        buscaborradoriva(idapunte1);
        calculaTotales();
        /// Buscamos la fecha que le corresponderá teoricamente a la factura.
        buscafecha(idapunte1);
	/// Buscamos la cuenta de servicio.
	buscaborradorservicio(idapunte1);
        //Proponemos un número de factura si se trata de una venta y proponemos un número de orden si se trata de una compra
        //Realmente esto se tendria que implementar con contadores en la base de datos.
        //Primero comprobamos si esta factura ya tiene un apunte de iva distinto y cojemos el mismo numero de factura
        query.sprintf("SELECT factura, numorden FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=(SELECT idasiento FROM borrador WHERE idborrador='%i'))",idborrador);
        cursor2 * recordset;
        conexionbase->begin();
        recordset = conexionbase->cargacursor(query,"recordset");
        conexionbase->commit();
        if (!recordset->eof()) {
            if (! numorden->isEnabled()) {  //Se trata de una Venta
                factura->setText(recordset->valor("factura"));
            } else { //Se trata de una compra
                factura->setText(recordset->valor("factura"));
                numorden->setText(recordset->valor("numorden"));
            }// end if
        }  else {   //La factura no existe, entonces proponemos el siguiente número de factura
            if (! numorden->isEnabled()) {  //Se trata de una Venta
                query.sprintf("SELECT MAX(to_number(factura,'99999')) AS factura FROM registroiva WHERE numorden=''");
                conexionbase->begin();
                recordset = conexionbase->cargacursor(query,"recordset");
                conexionbase->commit();
                if (!recordset->eof()) {
                    numfact = 1 + atoi(recordset->valor("factura").ascii());
                }
		else {
		    numfact= 1;
		}// end if
		cadena.sprintf("%i",numfact);
                factura->setText(cadena);		
            } else { //Se trata de una compra		
		/// Vemos si podemos extraer de la descripción del apunte el número de fra.
		query.sprintf("SELECT * FROM borrador WHERE idasiento IN (SELECT idasiento FROM borrador WHERE idborrador='%i') AND debe=0", idborrador);
		conexionbase->begin();
                recordset = conexionbase->cargacursor(query,"recordset");
                conexionbase->commit();
		QString num_fra = recordset->valor("conceptocontable");
		QRegExp patron("^S/Fra\\. [0-9]+$");
		if(num_fra.contains(patron) == 1){ // Si se cumple el patrón
		    num_fra = num_fra.remove("S/Fra. ");
		} else { // Si no saco el nº de fra. lo pongo a cero, por poner algo
		    num_fra.sprintf("0");
		}
		factura->setText(num_fra);
		/// Buscamos qué número de orden poner
                query.sprintf("SELECT MAX(to_number(numorden,'99999')) AS numorden FROM registroiva WHERE numorden<>''");
                conexionbase->begin();
                recordset = conexionbase->cargacursor(query,"recordset");
                conexionbase->commit();
                if (!recordset->eof()) {
                    numord = 1 + atoi(recordset->valor("numorden").ascii());
		} else {
		    numord = 1;
		}// end if
		cadena.sprintf("%i",numord);
                numorden->setText(cadena);
		
            }// end if
        }// end if
        delete recordset;//Fin proposicion numeros factura y orden.
    }// end if
    delete cursoriva;
    // Hacemos la carga de los cobros.
    cargacobros();
}// end inicializa1

/**
  * \brief Se encarga de cargar la rejilla de los cobros
  */
void ivaview::cargacobros () {
    // Cargamos las formas de pago correspondientes y las ponemos como toca.
    QString SQLQuery;
    SQLQuery.sprintf("SELECT * FROM prevcobro LEFT JOIN cuenta ON prevcobro.idcuenta = cuenta.idcuenta LEFT JOIN asiento ON asiento.idasiento = prevcobro.idasiento WHERE idregistroiva=%d", idregistroiva);
    conexionbase->begin();
    cursor2 *curprevcobros = conexionbase->cargacursor(SQLQuery, "queryprevcobros");
    conexionbase->commit();
    int i =0;
    while ( ! curprevcobros->eof()) {
        m_listPrevision->setText(i, COL_PREV_IDPREVCOBRO,curprevcobros->valor("idprevcobro"));
        m_listPrevision->setText(i, COL_PREV_FPREVISTAPREVCOBRO,curprevcobros->valor("fprevistaprevcobro"));
        m_listPrevision->setText(i, COL_PREV_FCOBROPREVCOBRO,curprevcobros->valor("fcobroprevcobro"));
        m_listPrevision->setText(i, COL_PREV_IDCUENTA,curprevcobros->valor("idcuenta"));
        m_listPrevision->setText(i, COL_PREV_CODCUENTA,curprevcobros->valor("codigo"));
        m_listPrevision->setText(i, COL_PREV_NOMCUENTA,curprevcobros->valor("descripcion"));
        m_listPrevision->setText(i, COL_PREV_CANTIDADPREVISTAPREVCOBRO,curprevcobros->valor("cantidadprevistaprevcobro"));
        m_listPrevision->setText(i, COL_PREV_CANTIDADPREVCOBRO,curprevcobros->valor("cantidadprevcobro"));
        m_listPrevision->setText(i, COL_PREV_DOCPREVCOBRO,curprevcobros->valor("docprevcobro"));
        m_listPrevision->setText(i, COL_PREV_IDREGISTROIVA,curprevcobros->valor("idregistroiva"));
        m_listPrevision->setText(i, COL_PREV_IDASIENTO,curprevcobros->valor("idasiento"));
        m_listPrevision->setText(i, COL_PREV_ORDENASIENTO,curprevcobros->valor("ordenasiento"));
	if (curprevcobros->valor("idasiento") != "") {
		m_listPrevision->setRowReadOnly(i, TRUE);
	}// end if
        if (curprevcobros->valor("tipoprevcobro") == "t") {
            m_listPrevision->setText(i, COL_PREV_TIPOCOBRO, "COBRO");
        } else {
            m_listPrevision->setText(i, COL_PREV_TIPOCOBRO, "PAGO");
        }// end if
        i++;
        curprevcobros->siguienteregistro();
    }// end while
    delete curprevcobros;
}// end cargacobros


/**
  * \brief Esta funci� guarda todas las previsiones.
  *
  * Se van guardando las previsiones linea por linea.
  */
void ivaview::guardaprevpago() {
    int i;
    i=0;
    while (m_listPrevision->text(i,COL_PREV_FPREVISTAPREVCOBRO) != "") {
        guardaprevpago(i++);
    }// end while
    cargacobros();
}// end guardaprevpago


/**
  * \brief Esta funci� guarda una linea de la tabla de previsiones
  *
  * La linea se guarda en la base de datos prevcobro, la tabla afectada es m_listPrevision
  * El parametro que se pasa es la columna de la tabla que queremos guardar en la base de datos.
  */
void ivaview::guardaprevpago (int numrow) {
    QString idprevpago = m_listPrevision->text(numrow, COL_PREV_IDPREVCOBRO);
    if (idprevpago != "") {       // Ya un elemento, debemos hacer un UPDATE
        QString SQLQuery = "UPDATE prevcobro SET ";
        SQLQuery += " idcuenta="+m_listPrevision->text(numrow, COL_PREV_IDCUENTA)+" ";
        SQLQuery += " , fprevistaprevcobro = '"+m_listPrevision->text(numrow, COL_PREV_FPREVISTAPREVCOBRO)+"' ";
        SQLQuery += " , fcobroprevcobro = '"+m_listPrevision->text(numrow, COL_PREV_FCOBROPREVCOBRO)+"' ";
        SQLQuery += " , cantidadprevistaprevcobro = "+m_listPrevision->text(numrow, COL_PREV_CANTIDADPREVISTAPREVCOBRO)+" ";
        SQLQuery += " , cantidadprevcobro = "+m_listPrevision->text(numrow, COL_PREV_CANTIDADPREVCOBRO)+" ";
        SQLQuery += " , docprevcobro = '"+m_listPrevision->text(numrow, COL_PREV_DOCPREVCOBRO)+"' ";
        if (m_listPrevision->text(numrow, COL_PREV_TIPOCOBRO) == "COBRO" ) {
            SQLQuery += " , tipoprevcobro = TRUE";
        } else {
            SQLQuery += " , tipoprevcobro = FALSE";
        }// end if
        SQLQuery += " WHERE idprevcobro="+idprevpago;
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
    } else {                    // Hay que hacer un INSERT
        QString SQLQuery = "INSERT INTO prevcobro (idcuenta, fprevistaprevcobro, fcobroprevcobro, cantidadprevistaprevcobro, cantidadprevcobro, docprevcobro, idregistroiva, tipoprevcobro) VALUES (";
        SQLQuery += conexionbase->sanearCadena(m_listPrevision->text(numrow, COL_PREV_IDCUENTA));
        SQLQuery += ", '"+conexionbase->sanearCadena(m_listPrevision->text(numrow, COL_PREV_FPREVISTAPREVCOBRO))+"'";
        SQLQuery += ", '"+conexionbase->sanearCadena(m_listPrevision->text(numrow, COL_PREV_FCOBROPREVCOBRO))+"'";
        SQLQuery += ", "+conexionbase->sanearCadena(m_listPrevision->text(numrow, COL_PREV_CANTIDADPREVISTAPREVCOBRO));
        SQLQuery += ", "+conexionbase->sanearCadena(m_listPrevision->text(numrow, COL_PREV_CANTIDADPREVCOBRO));
        SQLQuery += ", '"+conexionbase->sanearCadena(m_listPrevision->text(numrow, COL_PREV_DOCPREVCOBRO))+"'";
        SQLQuery += ", "+conexionbase->sanearCadena(QString::number(idregistroiva));
        if (m_listPrevision->text(numrow, COL_PREV_TIPOCOBRO) == "COBRO") {
            SQLQuery += ", TRUE";
        } else {
            SQLQuery += ", FALSE";
        }// end if
        SQLQuery += ") ";
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
    }// end if
}// end guardaprevpago


/**
  * \brief SLOT que captura el cambio de foco en la rejilla de Previsiones de cobros y pagos.
  * Detecta si se ha cambiado de fila y si es el caso se guarda el registro.
  * EN DESUSO
  */
void ivaview::tcambiaseleccion() {
    static int rowactual=0;
    fprintf(stderr,"tcambiaseleccion\n");
    if (rowactual != m_listPrevision->currentRow())
        guardaprevpago(rowactual);
    rowactual = m_listPrevision->currentRow();
}// end ivaview

/** EN DESUSO
  */
void ivaview::currentIvaChanged(int, int) {
    calculaTotales();
}// end cambiadoIvaGrid

/**
  * \brief SLOT que captura el cambio de foco en la rejilla de Previsiones de cobros y pagos.
  * Detecta si se ha cambiado de fila y si es el caso se guarda el registro.
  */
void ivaview::cambiadogrid(int row, int col) {
    QString fecha;
    switch (col) {
    case COL_PREV_FPREVISTAPREVCOBRO:
        fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
        m_listPrevision->setText(row, col, fecha);
        break;
    case COL_PREV_FCOBROPREVCOBRO:
        fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
        m_listPrevision->setText(row, col, fecha);
        break;
    case COL_PREV_CODCUENTA:
        cambiadasubcuenta(row);
        break;
    }// end switch
}// end apuntecambiadogrid

/** Se ha pulsado sobre el bot� de calcular la fecha.
  */
void ivaview::boton_fecha() {
    QList<QDate> a;
    m_ffactura->setText("");
    calendario *cal = new calendario(0,0);
    cal->exec();
    a = cal->dn->selectedDates();
    m_ffactura->setText(a.first()->toString("dd/MM/yyyy"));
    delete cal;
}// end boton_fecha


/**
  * \brief SLOT que se activa al pulsar sobre el bot� de generar previsiones.
  *
  1.- Vacia la lista de Prevision de Cobros
  2.- Calcula la fecha inicial a partir de la fecha de factura y la forma de pago.
  3.- Itera para cada plazo en la forma de pago calculando el nuevo plazo.
  
  // Falta usar las cuentas de servicio para saber si es cobro o pago.
  */
void ivaview::boton_generarPrevisiones() {
    QString snumpagos = m_cursorFPago->valor("nplazosfpago",m_fPago->currentItem());
    QString splazoprimerpago = m_cursorFPago->valor("plazoprimerpagofpago",m_fPago->currentItem());
    QString splazoentrerecibo = m_cursorFPago->valor("plazoentrerecibofpago",m_fPago->currentItem());
    Fixed totalfactura = Fixed(m_baseImponible->text()) + Fixed(importeiva->text());
    int plazoentrerecibo = splazoentrerecibo.toInt();
    int plazoprimerpago = splazoprimerpago.toInt();
    int numpagos = snumpagos.toInt();
    Fixed totalplazo = totalfactura / numpagos;

    // Vaciamos la lista de prevision para que no haga cosas raras
    m_listPrevision->setNumRows(0);
    m_listPrevision->setNumRows(50);
    QDate ffactura = normalizafecha(m_ffactura->text());
    QDate fpcobro = ffactura.addDays(plazoprimerpago);
    for (int i =0; i< numpagos; i++) {
        m_listPrevision->setText(i,COL_PREV_FPREVISTAPREVCOBRO,fpcobro.toString("dd/MM/yyyy"));
        m_listPrevision->setText(i,COL_PREV_FCOBROPREVCOBRO,fpcobro.toString("dd/MM/yyyy"));
        m_listPrevision->setText(i, COL_PREV_CANTIDADPREVCOBRO, totalplazo.toQString());
        m_listPrevision->setText(i, COL_PREV_CANTIDADPREVISTAPREVCOBRO, totalplazo.toQString());
	/// Hay que saber si es un cobro o un pago
	if (contrapartida->text().left(2) == "43") // Si es un cliente es un cobro, si es un proveedor es un pago.
	        m_listPrevision->setText(i, COL_PREV_TIPOCOBRO, "COBRO"); // Cobro
	else
	        m_listPrevision->setText(i, COL_PREV_TIPOCOBRO, "PAGO"); // Pago
        fpcobro = fpcobro.addDays(plazoentrerecibo);
    }// end for
}// end boton_generarPrevisiones


/**
  * \brief SLOT que captura la pulsaci� de determinadas telcas especiales para la aplicacion
  * Se emite cuando se han pulsado determinadas teclas en la edici� de la tabla de cobros/pagos.
  * Responde a la pulsaci� del '+' y del '*'
  */
void ivaview::pulsadomas(int row, int col, int caracter) {
    QString query, fecha;
    calendario *cal;
    int dia, mes, ano;
    QList<QDate> a;
    QString cadena;
    switch (caracter) {
    case '+':
        switch(col) {
        case COL_PREV_FPREVISTAPREVCOBRO:
            cal = new calendario(0,0);
            cal->exec();
            a = cal->dn->selectedDates();
            dia = a.first()->day();
            mes = a.first()->month();
            ano = a.first()->year();
            cadena.sprintf( "%2.2d/%2.2d/%d",dia, mes, ano);
            fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
            m_listPrevision->setText(row, COL_PREV_FPREVISTAPREVCOBRO, cadena);
            delete cal;
            break;
        case COL_PREV_FCOBROPREVCOBRO:
            cal = new calendario(0,0);
            cal->exec();
            a = cal->dn->selectedDates();
            dia = a.first()->day();
            mes = a.first()->month();
            ano = a.first()->year();
            cadena.sprintf( "%2.2d/%2.2d/%d",dia, mes, ano);
            fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
            m_listPrevision->setText(row, COL_PREV_FCOBROPREVCOBRO, cadena);
            delete cal;
            break;
        case COL_PREV_CODCUENTA:
            cambiadasubcuenta(row);
            break;
        }// end switch
    case '*':
        fprintf(stderr,"Se ha pulsado el *\n");
        //duplicar(col);
        break;
    case 4100:
        fprintf(stderr,"Se ha pulsado el enter\n");
        switch (col) {
        case COL_PREV_FPREVISTAPREVCOBRO:
            fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
            m_listPrevision->setText(row, col, fecha);
            m_listPrevision->setCurrentCell(row, COL_PREV_FCOBROPREVCOBRO);
            break;
        case COL_PREV_FCOBROPREVCOBRO:
            fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
            m_listPrevision->setText(row, col, fecha);
            m_listPrevision->setCurrentCell(row, COL_PREV_CODCUENTA);
            break;
        case COL_PREV_CODCUENTA:
            cambiadasubcuenta(row);
            m_listPrevision->setCurrentCell(row, COL_PREV_CANTIDADPREVISTAPREVCOBRO);
            break;
        default:
            m_listPrevision->setCurrentCell(row, col+1);
            break;
        }// end switch
        break;
    }// end switch
}// end pulsadomas


/**
  * \brief Esta funci� se llama cada vez que se sospecha que se ha cambiado la cuenta.
  *
  * Comprueba cual es la cadena que se ha metido como cuenta y le hace todos los tratamientos posibles
  * hasta determinar si es una cuenta del plan contable o no.
  */
void ivaview::cambiadasubcuenta(int row) {
    int numdigitos = empresaactual->numdigitosempresa();
    QString subcuenta = m_listPrevision->text(row,COL_PREV_CODCUENTA);
    if (subcuenta == "") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
        listcuentas->setModoLista();
        listcuentas->inicializa();
        listcuentas->exec();
        m_listPrevision->setText(row,COL_PREV_CODCUENTA,listcuentas->codcuenta());
        m_listPrevision->setText(row,COL_PREV_IDCUENTA,listcuentas->idcuenta());
        delete listcuentas;
    }// end if
    // Hacemos la recodificacin
    QString cad = m_listPrevision->text(row,COL_PREV_CODCUENTA);
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        conexionbase->begin();
        cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
        conexionbase->commit();
        int num = cursorcta->numregistros();
        if (num >0) {
            m_listPrevision->setText(row,COL_PREV_CODCUENTA,cursorcta->valor("codigo"));
            m_listPrevision->setText(row,COL_PREV_NOMCUENTA,cursorcta->valor("descripcion"));
            m_listPrevision->setText(row,COL_PREV_IDCUENTA,cursorcta->valor("idcuenta"));
        } else {
            QMessageBox::warning( 0, tr("No existe cuenta"), tr("No existe una cuenta con el codigo proporcionado, desea crear una?."), QMessageBox::Yes, QMessageBox::No);
        }// end if
        delete cursorcta;
    }// end if
}// end cambiadasubcuenta

