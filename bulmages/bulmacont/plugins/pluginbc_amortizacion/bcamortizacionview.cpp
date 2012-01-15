 /***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QPushButton>
#include <QRadioButton>
#include <QMenu>

#include "bcamortizacionview.h"
#include "bccompany.h"
#include "blfunctions.h"
#include "bcasientointeligenteview.h"
#include "bcasientoview.h"


/// Constructor de la clase
/**
\param emp
\param parent
**/
BcAmortizacionView::BcAmortizacionView ( BcCompany *emp, QWidget *parent )
        : BcForm ( emp, parent )
{
    BL_FUNC_DEBUG

    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_idamortizacion = "";
    m_idctaactivo = "";
    m_idctaamortizacion = "";

    /// Nueva inicializacion de amortizaciones
    mui_listcuotas->setMainCompany ( emp );
    ctaactivo->setMainCompany ( emp );
    /// Arreglamos la cuenta
    ctaactivo->setMainCompany(mainCompany());
    ctaactivo->setLabel ( _ ( "Cuenta activa:" ) );
    ctaactivo->setTableName ( "cuenta" );
    ctaactivo->setFieldId("idcuenta");
    ctaactivo->m_valores["descripcion"] = "";
    ctaactivo->m_valores["codigo"] = "";
    ctaactivo->hideLabel();

    ctaamortizacion->setMainCompany ( emp );
    /// Arreglamos la cuenta
    ctaamortizacion->setMainCompany(mainCompany());
    ctaamortizacion->setLabel ( _ ( "Cuenta de amortizacion:" ) );
    ctaamortizacion->setTableName ( "cuenta" );
    ctaamortizacion->setFieldId("idcuenta");
    ctaamortizacion->m_valores["descripcion"] = "";
    ctaamortizacion->m_valores["codigo"] = "";
    ctaamortizacion->hideLabel();

    /// Inicializamos la clase para la base de datos.
    setTitleName ( _ ( "Amortizacion" ) );
    setDbTableName ( "amortizacion" );
    setDbFieldId ( "idamortizacion" );
    addDbField ( "nomamortizacion", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre de la amortizacion" ) );
    addDbField ( "descamortizacion", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion de la amortizacion" ) );
    addDbField ( "fechacompra", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de la compra" ) );
    addDbField ( "fecha1cuota", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha 1a cuota" ) );
    addDbField ( "valorcompra", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Valor de la compra" ) );
    addDbField ( "periodicidad", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Periodicidad" ) );
    addDbField ( "numcuotas", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero de cuotas" ) );
    addDbField ( "metodo", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Metodo" ) );
    addDbField ( "nifproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "NIF del proveedor" ) );
    addDbField ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del proveedor" ) );
    addDbField ( "dirproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion del proveedor" ) );
    addDbField ( "telproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono del proveedor" ) );
    addDbField ( "agrupacion", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Agrupacion" ) );
    addDbField ( "idamortizacion", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id amortizacion" ) );
    addDbField ( "idcuentaactivo", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Identificador" ) );
    addDbField ( "idcuentaamortizacion", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id cuenta amortizacion" ) );

    /// Inicializamos el listado.
    mui_listcuotas->setDbTableName ( "linamortizacion" );
    mui_listcuotas->setDbFieldId ( "idlinamortizacion" );
    mui_listcuotas->addSubFormHeader ( "ejercicio", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Ejercicio" ) );
    mui_listcuotas->addSubFormHeader ( "fechaprevista", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _ ( "Fecha prevista" ) );
    mui_listcuotas->addSubFormHeader ( "cantidad", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidad" ) );
    mui_listcuotas->addSubFormHeader ( "idasiento", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite , _ ( "Id asiento" ) );
    mui_listcuotas->addSubFormHeader ( "idlinamortizacion", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite , _ ( "Id lineas de amortizacion" ) );
    mui_listcuotas->addSubFormHeader ( "idamortizacion", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite , _ ( "Id amortizacion" ) );
    mui_listcuotas->setInsert ( FALSE );
    mui_listcuotas->setOrdenEnabled ( FALSE );

    /// Fin de nuevas amortizaciones
    insertWindow ( windowTitle(), this );

    /// Llamamos a los scripts
    blScript(this);
    
}


///
/**
**/
BcAmortizacionView::~BcAmortizacionView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
}


///
/**
\return
**/
int BcAmortizacionView::remove()
{
    BL_FUNC_DEBUG
    if ( m_idamortizacion != "" ) {
        QString query = "DELETE FROM linamortizacion WHERE idamortizacion = " + m_idamortizacion;
        mainCompany() ->runQuery ( query );
        query = "DELETE FROM amortizacion WHERE idamortizacion = " + m_idamortizacion;
        mainCompany() ->runQuery ( query );
        close();
    } // end if
    
    return 0;
}


///
/**
\return
**/
int BcAmortizacionView::save()
{
    BL_FUNC_DEBUG
    try {
        /// Guardamos los datos del formulario
        setDbValue ( "nomamortizacion", nomamortizacion->text() );
        setDbValue ( "idcuentaactivo",  ctaactivo->id() );
        setDbValue ( "valorcompra", valorcompra->text() );
        setDbValue ( "numcuotas", numcuotas->text() );
        setDbValue ( "fechacompra", fechacompra->text() );
        setDbValue ( "fecha1cuota", fecha1cuota->text() );
        setDbValue ( "idcuentaamortizacion", ctaamortizacion->id() );
        setDbValue ( "agrupacion", agrupacion->text() );

	QString metodo;
	if (metodolineal->isChecked()) {
		metodo = "1";
	} else if (metodoincremental->isChecked()) {
		metodo = "2";
	} else if (metododecremental->isChecked()) {
		metodo = "3";
	} else if (metodoporcentual->isChecked()) {
		metodo = "4";
	} // end if

	setDbValue ( "metodo", metodo);

	int periodo;
        if ( periodicidad->currentText() == _ ( "Anual" ) ) {
		periodo = 12;
        } else if ( periodicidad->currentText() == _ ( "Mensual" ) ) {
		periodo = 1;
        } else if ( periodicidad->currentText() == _ ( "Semestral" ) ) {
		periodo = 6;
        } else if ( periodicidad->currentText() == _ ( "Trimestral" ) ) {
		periodo = 3;
        } // end if
        setDbValue ( "periodicidad", QString::number(periodo) );

        QString id = "";
        BlDbRecord::dbSave ( id );

        /// Guardamos las lineas de amortizacion.
        mui_listcuotas->setColumnValue ( "idamortizacion", id );
        mui_listcuotas->save();
        load ( id );
        
        return 0;
    } catch ( ... ) {
        blMsgInfo ( "Error en el guardado" );
        return -1;
    } // end try
}


/// Esta funci&oacute;n inicializa la clase, y adem&aacute;s hace la presentaci&oacuet;n
/// de una determinada amortizaci&oacute;n.
/**
\param idamortizacion
\param
**/
int BcAmortizacionView::load ( QString idamortizacion )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString("idamortizacion = '%1'").arg(idamortizacion) );
    try {
        m_idamortizacion = idamortizacion;
        BlDbRecord::load ( m_idamortizacion );

        mui_listcuotas->setInsert ( TRUE );

        /// se ha cargado de la base de datos.
        nomamortizacion->setText ( dbValue ( "nomamortizacion" ) );
        valorcompra->setText ( dbValue ( "valorcompra" ) );
        numcuotas->setText ( dbValue ( "numcuotas" ) );
        fechacompra->setText ( dbValue ( "fechacompra" ) );
        fecha1cuota->setText ( dbValue ( "fecha1cuota" ) );
        ctaactivo->setId ( dbValue ( "idcuentaactivo" ) );
        m_idctaactivo = dbValue ( "idcuentaactivo" );
        ctaamortizacion->setId ( dbValue ( "idcuentaamortizacion" ) );
        m_idctaamortizacion = dbValue ( "idcuentaamortizacion" );
        agrupacion->setText ( dbValue ( "agrupacion" ) );

	QString metodo = dbValue ( "metodo" );
	if (metodo == "1") { /// Lineal
		metodolineal->setChecked(true);
	} else if (metodo == "2") { /// Incremental
		metodoincremental->setChecked(true);
	} else if (metodo == "3") { /// Decremental
		metododecremental->setChecked(true);
	} else if (metodo == "4") { /// Porcentual
		metodoporcentual->setChecked(true);
	} // end if

	int periodo;
	if (dbValue ( "periodicidad" ) == "12") {
		periodo = periodicidad->findText(_("Anual"));
	} else if (dbValue ( "periodicidad" ) == "6") {
		periodo = periodicidad->findText(_("Semestral"));
	} else if (dbValue ( "periodicidad" ) == "3") {
		periodo = periodicidad->findText(_("Trimestral"));
	} else if (dbValue ( "periodicidad" ) == "1") {
		periodo = periodicidad->findText(_("Mensual"));
	} // end if

	periodicidad->setCurrentIndex(periodo);

        QString query = "SELECT *, fechaprevista <= now() AS ant FROM linamortizacion LEFT JOIN asiento ON linamortizacion.idasiento = asiento.idasiento WHERE idamortizacion = " + m_idamortizacion + " ORDER BY fechaprevista";
        mui_listcuotas->load ( query );

        /// Calculamos lo que ya llevamos amortizado y lo presentamos en la pantalla.
        query = "SELECT sum(cantidad) AS amortizado FROM linamortizacion WHERE idasiento IS NOT NULL AND idamortizacion = " + m_idamortizacion;
        BlDbRecordSet *curs = mainCompany() ->loadQuery ( query );
        if ( !curs->eof() ) {
            amortizado->setText ( curs->value( "amortizado" ) );
        } // end if
        delete curs;

        /// Calculamos lo que nos falta por amortizar y lo presentamos en la pantalla.
        query = "SELECT sum(cantidad) AS pdte FROM linamortizacion WHERE idasiento IS NULL AND idamortizacion = " + m_idamortizacion;
        curs = mainCompany() ->loadQuery ( query, "pdte" );
        if ( !curs->eof() ) {
            pendiente->setText ( curs->value( "pdte" ) );
        } // end if
        delete curs;

        /// Deshabilitamos el bot&oacute;n de calcular, porque la amortizaci&oacute;n ya
        /// est&aacute; hecha.
        mui_btcalcular->setDisabled ( TRUE );
        dialogChanges_readValues();
        mainCompany() ->insertWindow ( windowTitle() + dbValue ( "idamortizacion" ), this );

        
        return 0;
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error en la carga de la amortizacion." ) );
        return -1;
    } // end try
}


///
/**
**/
void BcAmortizacionView::on_mui_btcalcular_clicked()
{
    BL_FUNC_DEBUG
    /// Para hacer el c&iacute;rculo de los plazos de cada amortizaci&oacute;n
    /// Hay que obtener diversos datos.
    QDate f1cuota = blNormalizeDate ( fecha1cuota->text() );
    int ncuotas = numcuotas->text().toInt();
    QString periodicidadtxt = periodicidad->currentText();
    double valcuota;
    QString valcuotastr;

    mui_listcuotas->inicializar();
    mui_listcuotas->pintar();
    /// Si usamos el m&eacute;todo lineal:
    if ( metodolineal->isChecked() ) {
        valcuota = valorcompra->text().toDouble() / ncuotas;
        valcuotastr.sprintf ( "%10.2f", valcuota );
        for ( int i = 0; i < ncuotas; i++ ) {
            mui_listcuotas->setInsert ( TRUE );
            mui_listcuotas->newRecord();
            mui_listcuotas->setInsert ( FALSE );
            BlDbSubFormRecord *rec = mui_listcuotas->lista() ->last();
            rec->setDbValue ( "fechaprevista", f1cuota.toString ( "dd/MM/yyyy" ) );
            rec->setDbValue ( "cantidad", valcuotastr );

            /// Dependiendo de la periodicidad actualizamos la fecha.
            if ( periodicidadtxt == _ ( "Anual" ) ) {
                f1cuota = f1cuota.addYears ( 1 );
            } else if ( periodicidadtxt == _ ( "Mensual" ) ) {
                f1cuota = f1cuota.addMonths ( 1 );
            } else if ( periodicidadtxt == _ ( "Semestral" ) ) {
                f1cuota = f1cuota.addMonths ( 6 );
            } else if ( periodicidadtxt == _ ( "Trimestral" ) ) {
                f1cuota = f1cuota.addMonths ( 3 );
            } // end if
        } // end for
    } else if ( metodoincremental->isChecked() ) {
        /// El m&eacute;todo de amortizaci&oacute;n es el incremental.
        double total = 0;
        for ( int j = 1; j <= ncuotas; j++ ) {
            total = total + j;
        } // end for

        for ( int i = 1; i <= ncuotas; i++ ) {
            valcuota = valorcompra->text().toDouble() * i / total;
            valcuotastr.sprintf ( "%10.2f", valcuota );
            mui_listcuotas->setInsert ( TRUE );
            mui_listcuotas->newRecord();
            mui_listcuotas->setInsert ( FALSE );
            BlDbSubFormRecord *rec = mui_listcuotas->lista() ->last();
            rec->setDbValue ( "fechaprevista", f1cuota.toString ( "dd/MM/yyyy" ) );
            rec->setDbValue ( "cantidad", valcuotastr );
            /// Dependiendo de la periodicidad actualizamos la fecha.
            if ( periodicidadtxt == _ ( "Anual" ) ) {
                f1cuota = f1cuota.addYears ( 1 );
            } else if ( periodicidadtxt == _ ( "Mensual" ) ) {
                f1cuota = f1cuota.addMonths ( 1 );
            } else if ( periodicidadtxt == _ ( "Semestral" ) ) {
                f1cuota = f1cuota.addMonths ( 6 );
            } else if ( periodicidadtxt == _ ( "Trimestral" ) ) {
                f1cuota = f1cuota.addMonths ( 3 );
            } // end if
        } // end for

    } else if ( metododecremental->isChecked() ) {
        /// El m&eacute;todo de amortizaci&oacute;n es el incremental.
        double total = 0;
        for ( int j = 1; j <= ncuotas; j++ ) {
            total = total + j;
        } // end for
        for ( int i = 0; i < ncuotas; i++ ) {
            valcuota = valorcompra->text().toDouble() * ( ncuotas - i ) / total;
            valcuotastr.sprintf ( "%10.2f", valcuota );
            mui_listcuotas->setInsert ( TRUE );
            mui_listcuotas->newRecord();
            mui_listcuotas->setInsert ( FALSE );
            BlDbSubFormRecord *rec = mui_listcuotas->lista() ->last();
            rec->setDbValue ( "fechaprevista", f1cuota.toString ( "dd/MM/yyyy" ) );
            rec->setDbValue ( "cantidad", valcuotastr );

            /// Dependiendo de la periodicidad actualizamos la fecha.
            if ( periodicidadtxt == _ ( "Anual" ) ) {
                f1cuota = f1cuota.addYears ( 1 );
            } else if ( periodicidadtxt == _ ( "Mensual" ) ) {
                f1cuota = f1cuota.addMonths ( 1 );
            } else if ( periodicidadtxt == _ ( "Semestral" ) ) {
                f1cuota = f1cuota.addMonths ( 6 );
            } else if ( periodicidadtxt == _ ( "Trimestral" ) ) {
                f1cuota = f1cuota.addMonths ( 3 );
            } // end if
        } // end for
    } else if ( metodoporcentual->isChecked() ) {
        /// El m&eacute;todo de amortizaci&oacute;n es el incremental.
        double porcent = ( double ) 1 / ( double ) ncuotas;
        fprintf ( stderr, "El coeficiente es: %10.2f\n", porcent );
        double total = 0;
        for ( int i = 0; i < ncuotas; i++ ) {
            if ( i < ( ncuotas - 1 ) ) {
                valcuota = ( valorcompra->text().toDouble() - total ) * porcent;
                total += valcuota;
                fprintf ( stderr, "cuota: %10.2f -- total: %10.2f\n", valcuota, total );
            } else {
                valcuota = valorcompra->text().toDouble() - total;
            } // end if
            valcuotastr.sprintf ( "%10.2f", valcuota );
            mui_listcuotas->setInsert ( TRUE );
            mui_listcuotas->newRecord();
            mui_listcuotas->setInsert ( FALSE );
            BlDbSubFormRecord *rec = mui_listcuotas->lista() ->last();
            rec->setDbValue ( "fechaprevista", f1cuota.toString ( "dd/MM/yyyy" ) );
            rec->setDbValue ( "cantidad", valcuotastr );
            /// Dependiendo de la periodicidad actualizamos la fecha.
            if ( periodicidadtxt == _ ( "Anual" ) ) {
                f1cuota = f1cuota.addYears ( 1 );
            } else if ( periodicidadtxt == _ ( "Mensual" ) ) {
                f1cuota = f1cuota.addMonths ( 1 );
            } else if ( periodicidadtxt == _ ( "Semestral" ) ) {
                f1cuota = f1cuota.addMonths ( 6 );
            } else if ( periodicidadtxt == _ ( "Trimestral" ) ) {
                f1cuota = f1cuota.addMonths ( 3 );
            } // end if
        } // end for
    } // end if
    mui_listcuotas->setInsert ( TRUE );
}


/****************************************************************
               AMORTIZACIONSUBFORM
*****************************************************************/


///
/**
\param parent
**/
BcAmortizacionSubForm::BcAmortizacionSubForm ( QWidget *parent ) : BcSubForm ( parent )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
BcAmortizacionSubForm::~BcAmortizacionSubForm()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void BcAmortizacionSubForm::createMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    QAction *opt1 = menu->addAction ( _ ( "Generar asiento" ) );
    menu->addSeparator();
    QAction *opt2 = menu->addAction ( _ ( "Ver asiento" ) );
    menu->addSeparator();
    QAction *opt3 = menu->addAction ( _ ( "Desvincular asiento" ) );
    QAction *opt4 = menu->addAction ( _ ( "Borrar asiento" ) );

    if ( dbValue ( "idasiento" ) == "" ) {
        opt1->setEnabled ( TRUE );
        opt2->setEnabled ( FALSE );
        opt3->setEnabled ( FALSE );
        opt4->setEnabled ( FALSE );
    } else {
        opt1->setEnabled ( FALSE );
        opt2->setEnabled ( TRUE );
        opt3->setEnabled ( TRUE );
        opt4->setEnabled ( TRUE );
    } // end if
    
}


///
/**
\param opcion
\return
**/
void BcAmortizacionSubForm::execMenuAction ( QAction *opcion )
{
    BL_FUNC_DEBUG

    /// Si no se ha seleccionado ninguna accion salimos.
    if ( ! opcion )
        return;


    if ( opcion->text() == _ ( "Borrar cuota" ) ) {
        QString idlinamortizacion = dbValue ( "idlinamortizacion" );
        QString query = "DELETE FROM linamortizacion WHERE idlinamortizacion = " + idlinamortizacion;
        if ( idlinamortizacion != "" ) {
            mainCompany() ->begin();
            mainCompany() ->runQuery ( query );
            mainCompany() ->commit();
        } // end if
        on_mui_confquery_clicked();
    } // end if
    if ( opcion->text() == _ ( "Ver asiento" ) || opcion->text() == _ ( "Borrar asiento" ) ) {
        /// Si se va a mostrar el asiento, o se va a borrar.
        /*
                ((BcCompany *)mainCompany())->intapuntsempresa()->show();
                QString idasiento = dbValue("idasiento");
                ((BcCompany *)mainCompany())->intapuntsempresa()->muestraAsiento(idasiento.toInt());
        */
        boton_asiento();
    } // end if
    if ( opcion->text() == _ ( "Desvincular asiento" ) || opcion->text() == _ ( "Borrar asiento" ) ) {
        /// Si se va a desvincular el asiento o se va a borrar.
        QString idasiento = dbValue ( "idasiento" );
        QString idlinamortizacion = dbValue ( "idlinamortizacion" );
        QString query = "UPDATE linamortizacion SET idasiento = NULL WHERE idlinamortizacion = " + idlinamortizacion;
        mainCompany() ->runQuery ( query );
    } // end if
    if ( opcion->text() == _ ( "Borrar asiento" ) ) {
        /// Si se va a borrar el asiento.

        int resur = g_plugins->run ( "SNewBcAsientoView", (BcCompany *) mainCompany() );
        if ( ! resur) {
            blMsgInfo(_("No se pudo crear instancia de asientos"));
            return;
        } // end if
        BcAsientoView *asiento = (BcAsientoView *) g_plugParams;

        asiento  ->on_mui_borrar_clicked();
    } // end if
    if ( opcion->text() == _ ( "Generar asiento" ) ) {
        /// Se va a generar el asiento.
        QString fecha = dbValue ( "fechaprevista" );
        //            fprintf(stderr, "Fecha: %s\n", fecha.toAscii().constData());
        QString cant = dbValue ( "cantidad" );
        //            fprintf(stderr, "Cuota: %s\n", cant.toAscii().constData());

        QString cuenta, cuentaamort;
        QString query = "SELECT idcuentaactivo, idcuentaamortizacion FROM amortizacion WHERE idamortizacion=" + dbValue ( "idamortizacion" );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( ! cur->eof() ) {
            query = "SELECT codigo from cuenta where idcuenta=" + cur->value( "idcuentaactivo" );
            BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( query );
            if ( ! cur1->eof() ) {
                cuenta = cur1->value( "codigo" );
            } // end if
            delete cur1;

            query = "SELECT codigo from cuenta where idcuenta=" + cur->value( "idcuentaamortizacion" );
            cur1 = mainCompany() ->loadQuery ( query );
            if ( ! cur1->eof() ) {
                cuentaamort = cur1->value( "codigo" );
            } // end if
            delete cur1;

        } // end if
        delete cur;

        BcAsientoInteligenteView *nueva = new BcAsientoInteligenteView ( ( ( BcCompany * ) mainCompany() ), 0 );
        nueva->inicializa ( 0 );

        nueva->muestraPlantilla ( "amortizacion" );
        nueva->setValores ( "$cuenta$", cuentaamort );
        nueva->setValores ( "$cuentabien$", cuenta );
        nueva->setValores ( "$fechaasiento$", fecha );
        nueva->setValores ( "$cuota$", cant.replace ( ',', '.' ) );
        /// Ponemos la fecha del asiento para evitar escribir.
        nueva->setFechaAsiento ( fecha );
        /// Ponemos los asientos plantilla en modo exclusivo, para poder recuperar
        /// el control en cuanto se haya hecho la inserci&oacute;n del asiento.
        nueva->setModo ( 1 );
        mainCompany() ->pWorkspace() ->addSubWindow ( nueva );
        nueva->show();
        nueva->on_mui_aceptar_clicked();

        /// Cogemos los datos del asiento recien creado.
        int resur = g_plugins->run ( "SNewBcAsientoView", (BcCompany *) mainCompany() );
        if ( ! resur) {
            blMsgInfo(_("No se pudo crear instancia de asientos"));
            return;
        } // end if
        BcAsientoView *asiento = (BcAsientoView *) g_plugParams;


        int numasiento1 = asiento ->idasiento().toInt();
        QString ordenasiento;
        QString SQLQuery = "SELECT * FROM asiento where idasiento = " + QString::number ( numasiento1 );
        mainCompany() ->begin();
        cur = mainCompany() ->loadQuery ( SQLQuery );
        mainCompany() ->commit();
        if ( !cur->eof() ) {
            ordenasiento = cur->value( "ordenasiento" );
        } // end if
        delete cur;
        /// Debemos guardar la modificaci&oacute;n en la l&iacute;nea de amortizaci&oacute;n.
        QString idlinamortizacion = dbValue ( "idlinamortizacion" );
        SQLQuery = "UPDATE linamortizacion set idasiento = " + QString::number ( numasiento1 ) + " WHERE idlinamortizacion = " + idlinamortizacion;
        mainCompany() ->runQuery ( SQLQuery );
    } // end if
//    on_mui_confquery_clicked();
}

