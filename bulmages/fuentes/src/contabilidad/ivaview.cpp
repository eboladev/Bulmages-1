/***************************************************************************
                          ivaview.cpp  -  description
                             -------------------
    begin                : Tue Jan 28 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera & Josep Burción
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
-- idcuenta La cuenta sobre la que se hará el cobro / pago.
-- idasiento Asiento de cobro (Si está hecho).
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
			0 - días
			1 - semanas
			2 - meses
			3 - años
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
#include "busquedafecha.h"
#include "busquedacuenta.h"


#include <qregexp.h>
#include <qradiobutton.h>

#define IVA_SOPORTADO    1
#define IVA_REPERCUTIDO  2

#define COL_IVA_IDIVA 0
#define COL_IVA_IDTIPOIVA 1
#define COL_IVA_NOMBRETIPOIVA 2
#define COL_IVA_CTAIVA 3
#define COL_IVA_PORCENTAJETIPOIVA 4
#define COL_IVA_IDREGISTROIVA 5
#define COL_IVA_BASEIVA 6
#define COL_IVA_IVAIVA 7

#include "fixed.h"
#include "listlinprevcobroview.h"
//class fixed;

/**
  * \brief constructor de la clase
  * Inicializa la clase cargando el puntero de empresaactual y de conexionbase para que apunten a las clases amigas apropiadas.
  * Carga el numdigitos de la empresa.
  * Inicializa todos los componentes, principalmente inicializa las tablas, pone los titulos de las columnas y hace las que
  * no deben verse invisibles.
  * También carga el cursor de las formas de pago.
  */
ivaview::ivaview(empresa *emp,QWidget *parent, const char *name ) : ivadlg(parent,name) {
    fprintf(stderr,"Inicialización de ivaview\n");
    empresaactual = emp;
    conexionbase = emp->bdempresa();
    contrapartida->setempresa(emp);
    m_listprevcobro->setcompany(emp);
    fprintf(stderr,"Aqui llegamos\n");
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

    // CAlculamos las formas de pago.
    m_cursorFPago = NULL;
    cargarComboFPago("NULL");
    fprintf(stderr,"Fin de la inicialización de ivaview\n");
}// end ivaview


/**
  * \brief Carga el combo de las formas de pago
  * 
  Esta función se encarga de cargar la tabla de formas de pago en el combo box correspondiente.
  Usa un cursor de clase (m_cursorFPago) que es construido al usar esta función y destruido en el destructor de clase.
  Esta función se llama con la inicialización de clase y cuando se quieren cargar datos.
*/
void ivaview::cargarComboFPago(QString idfpago) {
    if (m_cursorFPago != NULL)
        delete m_cursorFPago;
    m_cursorFPago = conexionbase->cargacursor("SELECT * FROM fpago");
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



/** \brief Este slot se ejeccuta cuando pusamos sobre el botón aceptar.
  * Si se trata de una modificacion modificamos.
  * Si se trata de una inserción insertamos.
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

            /// Si se trata de una modificación y hacemos el update.
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
            cursor2 *cur = conexionbase->cargacursor("SELECT MAX(idregistroiva) AS idregistroiva FROM registroiva");
            conexionbase->commit();
            if (!cur->eof() ) {
                idregistroiva = cur->valor("idregistroiva").toInt();
                m_listprevcobro->setidregistroiva(cur->valor("idregistroiva"));
                m_listprevcobro->s_setidregistroiva(cur->valor("idregistroiva"));
            }// end if
            delete cur;
        }// end if
    }// end if
    delete cursorcuenta;
    guardaiva();
    /// Guardamos todas las previsiones de pago
    m_listprevcobro->guardalistlinprevcobro();
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





/** \brief Se ha pulsado return sobre la contrapartida
  */
void ivaview::chContrapartida() {
    QString cad = contrapartida->text();
    if (cad != "") {
        QString SQLQuery = "SELECT * FROM cuenta WHERE codigo='"+contrapartida->text()+"'";
        cursor2 *cur = conexionbase->cargacursor(SQLQuery);
        if (!cur->eof()) {
            cif->setText(cur->valor("cifent_cuenta"));
        }// end if
        delete cur;
    }// end if
}// end return_codigofinal



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
    SQLQuery = "SELECT * FROM tipoiva LEFT JOIN lacosa ON tipoiva.idcuenta=lacosa.idcuenta LEFT JOIN (SELECT idcuenta, contrapartida, IVAdebe AS debe, IVAhaber AS haber FROM lacosa) AS base ON (base.debe*porcentajetipoiva/100)::NUMERIC(12,1)=lacosa.ivadebe::NUMERIC(12,1) AND (base.haber*porcentajetipoiva/100)::NUMERIC(12,2)=lacosa.ivahaber::NUMERIC(12,1) ORDER BY idtipoiva";

    cursor2 * cur=conexionbase->cargacursor(SQLQuery);
    m_listIva->setNumRows(cur->numregistros());
    int i=0;
    while (! cur->eof() ) {
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
  * Si hay varios servicios entonces tb deben pasarse en forma de lista. Aunque sólo se necesite uno.
  * \todo Los servicios no son todos los del grupo 6 y 7 son los 6x y 7x esto provoca que aparezca la ventana más veces de las deseadas.
  * En estos momentos sólo se utiliza para calcular el total de la base imponible.
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

    /// Vamos a tener en cuenta también las cuentas de iva
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

    // Atentos que aquí es donde se incorpora el parametro.
    SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO "+cuentas+" OR codigo SIMILAR TO "+cuentasIVA;
    cur=conexionbase->cargacursor(SQLQuery);
    conexionbase->commit();
    while (! cur->eof() ) {
        fprintf(stderr,"idborrador: %s contrapartida: %s cuenta: %s\n",cur->valor("idborrador").ascii(), cur->valor("contrapartida").ascii(), cur->valor("codigo").ascii());
        registro = atoi(cur->valor("idborrador").ascii());
        cur->siguienteregistro();
    }//end while
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
        chContrapartida();
        /// Comprobamos si es un cliente o un proveedor y segun sea actuamos en consecuencia.
        if (cur->valor("codigo").left(2) == "43") {
            m_factEmitida->setChecked(TRUE);
        } else {
            m_factSoportada->setChecked(TRUE);
        }// end if
        m_totalFactura->setText(cur->valor("totalfactura"));

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
    QString SQLQuery = "SELECT * FROM cuenta,tipoiva LEFT JOIN (SELECT * FROM iva WHERE idregistroiva="+idregistroiva+" ) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE cuenta.idcuenta=tipoiva.idcuenta ORDER BY tipoiva.idtipoiva";
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

    /// Hacemos la carga de las previsiones de cobro en caso de que se puedan hacer
    if (idregistroiva != "") {
        fprintf(stderr,"Hacemos la carga de las PREV COBRO\n");
        m_listprevcobro->setidregistroiva(idregistroiva);
        m_listprevcobro->s_setidregistroiva(idregistroiva);
        m_listprevcobro->chargeBudgetLines();
        m_listprevcobro->pintalistlinprevcobro();
    }// end if
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
                } else {
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
                if(num_fra.contains(patron) == 1) { // Si se cumple el patrón
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
    //    cargacobros();
}// end inicializa1

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

/** EN DESUSO
  */
void ivaview::currentIvaChanged(int, int) {
    calculaTotales();
}// end cambiadoIvaGrid


/**
  * \brief SLOT que se activa al pulsar sobre el botón de generar previsiones.
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
    QString tipocobro;

    // Vaciamos la lista de prevision para que no haga cosas raras
    QDate ffactura = normalizafecha(m_ffactura->text());
    QDate fpcobro = ffactura.addDays(plazoprimerpago);
    for (int i =0; i< numpagos; i++) {
        /// Hay que saber si es un cobro o un pago
        if (contrapartida->text().left(2) == "43") { // Si es un cliente es un cobro, si es un proveedor es un pago.
            tipocobro = "t";
        } else {
            tipocobro = "f";
        }// end if
        m_listprevcobro->nuevalinea(
            fpcobro.toString("dd/MM/yyyy"),
            fpcobro.toString("dd/MM/yyyy")
            ,"","","",
            totalplazo.toQString(),
            totalplazo.toQString(),
            tipocobro,
            "","",""
        );
        fpcobro = fpcobro.addDays(plazoentrerecibo);
    }// end for
    m_listprevcobro->pintalistlinprevcobro();
}// end boton_generarPrevisiones





