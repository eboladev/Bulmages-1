/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include <stdlib.h>

#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QColor>
#include <QFont>
#include <QStringList>
#include <QLayout>
#include <QLabel>

#include "blmainwindow.h"
#include "bcbulmacont.h"
#include "bcasientoview.h"
#include "bldatesearch.h"
#include "bcasientointeligenteview.h"
#include "bcasientosubform.h"
#include "bccompany.h"


/// Constructor de la clase, inicializa los componentes
/** \param emp Empresa que llama al objeto.
    \param parent Widget padre de este.
    \return
*/
BcAsientoView::BcAsientoView ( BcCompany *emp, QWidget *parent, int )
        : BcAsientoList ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );

    /// Disparamos los plugins.
    int res = g_plugins->run ( "BcAsientoView_BcAsientoView", this );
    if ( res != 0 )
        return;

    eventos_mui_ordenAsiento *eventosOrdenAsiento = new eventos_mui_ordenAsiento ( this );
    mui_ordenAsiento->installEventFilter ( eventosOrdenAsiento );
    
    /// Conectamos el siganl returnPressed de mui_fecha
    connect ( mui_fecha, SIGNAL ( returnPressed() ), this, SLOT ( on_mui_fecha_enterPressed() ) );

    mui_list->setMainCompany ( emp );
    setListLinAsiento1 ( mui_list );

    /// Ocultamos los detalles del asiento.
    mui_detalles->toggle();
    /// Hacemos la carga del listado de asientos.
    cargaAsientos();
    /// Desplazamos hasta el ultimo asiento.
    botonFin();
    mainCompany() ->insertWindow ( windowTitle(), this );
    dialogChanges_readValues();

    /// Conectamos los botones del menu con las acciones de esta ventana.
    connect (((BcBulmaCont *)g_main)->actionSiguiente, SIGNAL(triggered()), this, SLOT(botonSiguiente()));
    connect (((BcBulmaCont *)g_main)->actionAnterior, SIGNAL(triggered()), this, SLOT(botonAnterior()));
    connect (((BcBulmaCont *)g_main)->actionInicio, SIGNAL(triggered()), this, SLOT(botonInicio()));
    connect (((BcBulmaCont *)g_main)->actionFin, SIGNAL(triggered()), this, SLOT(botonFin()));

    
}


/// Saca la ventana del workSpace.
/**
**/
BcAsientoView::~BcAsientoView()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void BcAsientoView::calculaPintaTotales()
{
    BL_FUNC_DEBUG
    BlFixed tdebe = mui_list->sumarCampo ( "debe" );
    BlFixed thaber = mui_list->sumarCampo ( "haber" );
    m_totaldebe->setText ( tdebe.toQString() );
    m_totalhaber->setText ( thaber.toQString() );
    BlFixed desc = tdebe - thaber;
    mui_descuadre->setText ( desc.toQString() );

    /// Si hay descuadre no se permite cerrar el asiento. Y si el asiento esta abierto y cuadrado se permite el cierre del asiento.
    if ( desc != 0 ) {
        mui_cerrarAsiento->setEnabled ( FALSE );
    } else if ( !mui_abrirAsiento->isEnabled() ) {
        mui_cerrarAsiento->setEnabled ( TRUE );
    } // end ir
    
}


///
/**
**/
void BcAsientoView::trataEstadoBcAsientoForm()
{
    BL_FUNC_DEBUG
    if ( estadoBcAsientoForm() == AsientoCerrado ) {
        modoAsientoCerrado();
    } else {
        modoAsientoAbierto();
    }// end if
    
}


/// Pone la pantalla en el modo de asiento abierto.
/** Activa los botones de cierre y pone los elementos como est&aacute;n configurados. */
/**
**/
void BcAsientoView::modoAsientoAbierto()
{
    BL_FUNC_DEBUG
    mui_descuadre->setEnabled ( TRUE );
    mui_abrirAsiento->setEnabled ( FALSE );
    mui_cerrarAsiento->setEnabled ( TRUE );
    mui_inteligente->setEnabled ( TRUE );
    mui_list->setInsert ( TRUE );

    //mui_list->mui_list->setBackgroundColor(Qt::lightGray);
    /// Los apuntes deben ser editables.
    for ( int fila = 0; fila < mui_list->rowCount(); fila++ ) {
        for ( int columna = 0; columna < mui_list->columnCount(); columna++ ) {
            mui_list->item ( fila, columna ) ->setFlags ( Qt::ItemIsEnabled | Qt::ItemIsEditable );
            mui_list->item ( fila, columna ) ->setBackgroundColor ( Qt::lightGray );
        } // end for
    } // end for

    
}


/// Pone la pantalla en el modo de asiento cerrado.
/** Activa los botones de apertura y pone los elementos como est&aacute;n configurados. */
/**
**/
void BcAsientoView::modoAsientoCerrado()
{
    BL_FUNC_DEBUG
    mui_abrirAsiento->setEnabled ( TRUE );
    mui_cerrarAsiento->setEnabled ( FALSE );
    mui_inteligente->setEnabled ( TRUE );
    mui_list->setInsert ( FALSE );

    /// Los apuntes deben dejar de ser editables (aunque no se graben sus posibles
    /// modificaciones por estar en modo CERRADO).
    for ( int fila = 0; fila < mui_list->rowCount(); fila++ ) {
        for ( int columna = 0; columna < mui_list->columnCount(); columna++ ) {
            mui_list->item ( fila, columna ) ->setFlags ( Qt::ItemIsEnabled );
            mui_list->item ( fila, columna ) ->setBackgroundColor ( Qt::darkGray );
        } // end for
    } // end for

    
}


/// Esta funci&oacute;n se activa cuando se pulsa sobre el bot&oacute;n nuevo asiento del
/// formulario.
/**
**/
void BcAsientoView::on_mui_nuevoAsiento_clicked()
{
    BL_FUNC_DEBUG
    
    if ( dialogChanges_isChanged() ) {
        switch ( QMessageBox::warning ( this, "BulmaCont",
                                        _("Asiento cambiado.\n") +
                                        _("Desea guardar los cambios \n\n "),
                                        QMessageBox::Save | QMessageBox::Discard
                                        | QMessageBox::Cancel,
                                        QMessageBox::Save ) ) {
        case QMessageBox::Save: // The user clicked the Retry again button or pressed Enter
            save();
            break;
        case QMessageBox::Discard: // The user clicked the Quit or pressed Escape
            break;
        case QMessageBox::Cancel:
	default:
            return;
        } // end switch
    } // end if

    
    mui_fecha->setText ( QDate::currentDate().toString ( "dd/MM/yyyy" ) );
    iniciar_asiento_nuevo();
    
}


/// Esta funci&oacute;n se encarga de hacer las inicializaciones en un asiento nuevo.
/**
\param nuevoordenasiento
\return
**/
void BcAsientoView::iniciar_asiento_nuevo ( QString nuevoordenasiento )
{
    BL_FUNC_DEBUG
    try {
        /// TRATAMIENTO DE BASE DE DATOS.
        unsigned int idasiento;
        QString ordenasiento, query;
        QString fecha = mui_fecha->text();
        BlDbRecordSet *cur;
        mainCompany() ->begin();
        if ( nuevoordenasiento == "" ) {
            QString query = "SELECT COALESCE(MAX(ordenasiento) + 1, 1) AS orden FROM asiento WHERE EXTRACT(YEAR FROM fecha) = '" + fecha.left ( 10 ).right ( 4 ) + "'";
            cur = mainCompany() ->loadQuery ( query );
            ordenasiento = cur->value( "orden" );
	    delete cur;
        } else {
            ordenasiento = nuevoordenasiento;
        } // end if

	/// Revisamos la existencia del ejercicio
	query = "SELECT * FROM ejercicios where ejercicio = extract('year' FROM '"+ mainCompany() ->sanearCadena ( fecha ) +"'::date)";
	cur = mainCompany() ->loadQuery ( query );
        if ( cur->eof() ) {
	    blMsgInfo("Debe crear el ejercicio antes de poder introducir asientos con esta fecha. Vaya a Herramientas->Bloqueo de Fechas para crearlo");
	    mainCompany() ->rollback();
	    delete cur;
	    return;
	} // end if
        delete cur;

        /// Creamos el asiento en la base de datos.
        query = "INSERT INTO asiento ( fecha, ordenasiento) VALUES ('" + mainCompany() ->sanearCadena ( fecha ) + "', " + ordenasiento + ")";
        mainCompany() ->runQuery ( query );


	/// TODO: el codigo que sigue no es correcto porque puede haber otras inserciones antes de capturar el Id.
	/// Hay que hacerlo usando 'currval'. Leer documentacion postgresql.
        query = "SELECT MAX(idasiento) AS id FROM asiento";
        cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() )
            idasiento = cur->value( "id" ).toInt();
        delete cur;


        mainCompany() ->commit();
        /// FIN TRATAMIENTO DE BASE DE DATOS.
        cargaAsientos();

	dialogChanges_readValues();
        muestraAsiento ( idasiento );

        abrir();

        
        return;
    } catch ( ... ) {
        blMsgInfo ( _("El asiento no pudo crearse") );
        mainCompany() ->rollback();
    } // end try
}


/** SLOT que se lanza cuando se ha terminado de editar la fecha.
    Si el asiento esta abierto cambia la fecha del asiento.
    Si el asiento esta cerrado crea un asiento nuevo llamando a \ref iniciar_asiento_nuevo()
*/
/**
\return
**/
void BcAsientoView::on_mui_fecha_enterPressed()
{
    BL_FUNC_DEBUG
    /// Usamos un semaforo para prevenir de entradas concurrentes.
    static bool semaforo = FALSE;
    if ( semaforo ) return;
    semaforo = TRUE;
    /// Cambiar la fecha del asiento.
    if ( estadoBcAsientoForm() != BcAsientoForm::AsientoCerrado && estadoBcAsientoForm() != BcAsientoForm::AsientoVacio ) {
        setDbValue ( "fecha", mui_fecha->text() );
        save();
    } else {
        iniciar_asiento_nuevo();
    } // end if
    semaforo = FALSE;
    
}


/// Se ha pulsado sobre el bot&oacute;n de duplicar asiento.
/** Inicializa el di&aacute;logo de duplicaci&oacute;n de asientos y lo presenta.
    Cuando se ha terminado carga el cursor de presentaci&oacute;n y repinta el asiento
    para que actualize los cambios. */
/**
**/
void BcAsientoView::on_mui_duplicar_clicked()
{
    BL_FUNC_DEBUG

    
}


/// Se ha pulsado sobre el bot&oacute;n de generar asientos inteligentes.
/** Se inicializa la clase \ref BcAsientoInteligenteView y se muestra ese di&aacute;logo
    para que se opere con los asientos plantilla. */
/**
**/
void BcAsientoView::on_mui_inteligente_clicked()
{
    BL_FUNC_DEBUG
    int numasiento;
    if ( estadoBcAsientoForm() != BcAsientoForm::AsientoCerrado ) {
        /// El asiento esta abierto y por tanto se muestra como abierto.
        modoAsientoAbierto();
        numasiento = idAsiento().toInt();
    } else {
        numasiento = 0;
    } // end if
    BcAsientoInteligenteView *nueva = new BcAsientoInteligenteView ( mainCompany(), 0 );
    nueva->inicializa ( numasiento );
    nueva->show();
    
}


/// Se ha pulsado sobre el bot&oacute;n de cargar asiento con lo
/// que debemos comprobar que el numero introducido es correcto
/// y hacer las gestiones oportunas para mostrar el asiento en
/// pantalla o crearlo si hace falta.
/**
**/
void BcAsientoView::boton_cargarasiento()
{
    BL_FUNC_DEBUG
    QString idas = "";
    QString query = "SELECT idasiento FROM asiento WHERE ordenasiento = " + mui_ordenAsiento->text() + " ORDER BY ordenasiento DESC";
    BlDbRecordSet *curs = mainCompany() ->loadQuery ( query );
    if ( !curs->eof() ) {
        idas = curs->value( "idasiento" );
        load ( idas );
    } else {
        /// Si el asiento no existe se da la posibilidad de crear uno nuevo.
        if ( QMessageBox::question ( this,
                                     _ ( "Asiento inexistente." ),
                                     _ ( "Desea crear un nuevo asiento en esa posicion?" ),
                                     _ ( "&Si" ), _ ( "&No" ), 0, 0, 1 ) == 0 ) {
            iniciar_asiento_nuevo ( mui_ordenAsiento->text() );
        } // end if
        pintar();
    } // end if
    delete curs;
    
}


///
/**
\param v
**/
void BcAsientoView::muestraAsiento ( QString v )
{
    BL_FUNC_DEBUG
    situarAsiento ( v );
    load ( v );
    
}


/// Prepara para guardar.
/**
**/
int BcAsientoView::save()
{
    BL_FUNC_DEBUG
    setDbValue ( "fecha", mui_fecha->text() );
    setDbValue ( "ordenasiento", mui_ordenAsiento->text() );
    setDbValue ( "comentariosasiento", mui_comentariosAsiento->toPlainText() );
    setDbValue ( "clase", QString::number ( mui_claseAsiento->currentIndex() ) );
    
    /// Miramos el campo fecha y lo completamos.
    for (int i =0; i < mui_list->rowCount() ; i++) {
	QString fecha = mui_list->dbValue("fecha",i);
	if (fecha == "") {
	    mui_list->setDbValue("fecha", i, mui_fecha->text());
	} // end if
    } // end for
    
    return BcAsientoForm::save();
    
}


/// Post Guardado se hace una carga ya que la base de datos podria haber modificado cosas.
/**
**/
int BcAsientoView::afterSave()
{
    BL_FUNC_DEBUG
    
    return 0;
}


///
/**
**/
void BcAsientoView::on_mui_borrar_clicked()
{
    bool atendido = TRUE; // asumimos que habra que atender al dialogo de confirmacion de borrado
    on_mui_borrar_clicked ( atendido );
}


///
/**
\param atendido
**/
void BcAsientoView::on_mui_borrar_clicked ( bool atendido )
{
    BL_FUNC_DEBUG
    QString asientoSiguiente = idAsientoSiguiente();
    QString asientoAnterior = idAsientoAnterior();
    int resultadoBorrar;
    resultadoBorrar = BcAsientoForm::remove ( atendido );

    /// Comprueba si se ha cancelado el borrado.
    if ( resultadoBorrar == 2 ) return;

    cargaAsientos();
    if ( asientoSiguiente != "" )
        muestraAsiento ( asientoSiguiente );
    else {
        if ( asientoAnterior != "" )
            muestraAsiento ( asientoAnterior );
        else {
            vaciar();
            pintar();
        } // end if
    } // end if
}


///
/**
**/
void BcAsientoView::on_mui_list_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaPintaTotales();
}


/// =========================================================================================
///                       LISTASIENTOS
/// =========================================================================================


///
/**
\param emp
\param parent
**/
BcAsientoList::BcAsientoList ( BcCompany *company, QWidget *parent ) : BcAsientoForm ( company, parent )
{
    BL_FUNC_DEBUG
    m_cursorAsientos = NULL;
}


///
/**
**/
BcAsientoList::~BcAsientoList()
{
    BL_FUNC_DEBUG
    if ( m_cursorAsientos != NULL ) {
        delete m_cursorAsientos;
    } // end if
}


/// Prepara el cursor que sirve para recorrer los asientos uno a uno.
/** Carga el cursor que sirve para hacer todo
    el recorrido a trav&eacute;s de los asientos.
    numasiento: 0 indica el primer asiento.
               -1 indica el &uacute;ltimo asiento.
    otros indica el asiento o el inmediatamente m&aacute;s bajo.
    Esta funci&oacute;n no hace cambios en la presentaci&oacute;n,
    s&oacute;lo realiza una carga del cursor que sirve para recorrer los asientos. */
/**
**/
void BcAsientoList::cargaAsientos()
{
    BL_FUNC_DEBUG
    QString cantapunt = "";
    QString saldototal = "";
    QString nombreasiento = "";
    QString query;
    QString cadwhere;
    QString textsaldototal = "";
    QString textcantapunt = "";
    QString textnombreasiento = "";
    QString textejercicio = "";
    QString ejercicio = "--";

    if ( m_cursorAsientos != NULL ) {
        delete m_cursorAsientos;
    } // end if

    /// Indica si se tiene que agregar el AND o no en el 'select'.
    int pand = 0;
    if ( saldototal != "" ) {
        cadwhere = " WHERE ";
        textsaldototal = " idasiento IN (SELECT idasiento FROM (SELECT idasiento, sum(debe) AS total from apunte GROUP BY idasiento) AS foo WHERE foo.total = " + saldototal + ")";
        pand = 1;
    } // end if
    if ( cantapunt != "" ) {
        cadwhere = " WHERE ";
        if ( pand )
            textcantapunt = " AND ";
        textcantapunt += " idasiento IN (SELECT idasiento FROM apunte where debe = " + cantapunt + " OR haber = " + cantapunt + ")";
        pand = 1;
    } // end if
    if ( nombreasiento != "" ) {
        cadwhere = " WHERE ";
        if ( pand )
            textnombreasiento = " AND ";
        textnombreasiento += " idasiento in (SELECT idasiento FROM apunte WHERE conceptocontable LIKE '%" + nombreasiento + "%' )";
        pand = 1;
    } // end if
    /// Los ejercicios los pondremos como filtraje de la introducci&oacute;n de asientos.
    if ( ejercicio != "--" ) {
        if ( pand )
            textejercicio = " AND EXTRACT(YEAR FROM fecha) = '" + ejercicio + "'";
        else
            textejercicio = " WHERE EXTRACT(YEAR FROM fecha) = '" + ejercicio + "'";
    } // end if
    /// Se ordenan los asientos por a&ntilde;o y por n&uacute;mero de orden.
    query = "SELECT * FROM asiento " + cadwhere + textsaldototal + textcantapunt + textnombreasiento + textejercicio + " ORDER BY EXTRACT (YEAR FROM fecha), ordenasiento";
    m_cursorAsientos = mainCompany() ->loadQuery ( query );
    if ( m_cursorAsientos->eof() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("No existe ningun asiento para mostrar") );
    } // end if
    
}

void BcAsientoList::on_mui_inicio_released() {
      BL_FUNC_DEBUG
      botonInicio();
}

/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de inicio.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si
    existen se desplaza al primer registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que
    no salga basura. */
/**
**/
void BcAsientoList::botonInicio()
{
    BL_FUNC_DEBUG
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
	if ( m_cursorAsientos->numregistros() != 0 ) {
	    m_cursorAsientos->firstRecord();
	    load ( m_cursorAsientos->value( "idasiento" ) );
	} // end if
    } // end if
    
}

void BcAsientoList::on_mui_fin_released() {
      BL_FUNC_DEBUG
      botonFin();
}

/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de fin.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si existen
    se desplaza al &uacute;ltimo registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura. */
/**
**/
void BcAsientoList::botonFin()
{
    BL_FUNC_DEBUG
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
	if ( m_cursorAsientos->numregistros() != 0 ) {
	    m_cursorAsientos->lastRecord();
	    load ( m_cursorAsientos->value( "idasiento" ) );
	} // end if
    } // end if
    
}



void BcAsientoList::on_mui_siguiente_released() {
      BL_FUNC_DEBUG
      botonSiguiente();
}

/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de siguiente registro.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si existen
    se desplaza al siguiente registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que no
    salga basura. */
/**
\return
**/
void BcAsientoList::botonSiguiente()
{
    BL_FUNC_DEBUG
    if (mainCompany()->pWorkspace()->activeWindow() == this) {
        ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
        if ( m_cursorAsientos->numregistros() == 0 ) {
	    return;
        } // end if

        if ( !m_cursorAsientos->isLastRecord() ) {
            m_cursorAsientos->nextRecord();
            load ( m_cursorAsientos->value( "idasiento" ) );
        }// end if
    } // end if
}


void BcAsientoList::on_mui_anterior_released() {
      BL_FUNC_DEBUG
      botonAnterior();
}

/// Slot que responde a la pulsaci&oacute;n del botÃ³n de anterior registro.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si
    existen se desplaza al registro anterior y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que
    no salga basura. */
/**
\return
**/
void BcAsientoList::botonAnterior()
{
    BL_FUNC_DEBUG
    
    if (mainCompany()->pWorkspace()->activeWindow() == this) {
        ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
        if ( m_cursorAsientos->numregistros() == 0 ) {
            return;
        } // end if

        if ( !m_cursorAsientos->isFirstRecord() ) {
            m_cursorAsientos->previousRecord();
            load ( m_cursorAsientos->value( "idasiento" ) );
        } // end if
    } // end if
}


/** Situa el cursor de asientos en un asiento determinado.
    \param idasiento El identificador del asiento en el que debe situarse.
    Si no se encuentra el asiento buscado devuelve una excepcion -1
*/
/**
\param idasiento
**/
void BcAsientoList::situarAsiento ( QString idasiento )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString("idasiento = '%1'").arg(idasiento) );
    try {
        if ( m_cursorAsientos == NULL )
            throw - 1;
        m_cursorAsientos->firstRecord();
        while ( m_cursorAsientos->value( "idasiento" ) != idasiento && !m_cursorAsientos->isLastRecord() ) {
            m_cursorAsientos->nextRecord();
        } // end while
    } catch ( ... ) {
        blMsgError ( "Error al intentar situarse en el asiento.\nEs posible que el asiento ya no exista en la base de datos.\nActualice el listado de asientos." );
        throw - 1;
    } // end try
    
}


///
/**
\return
**/
QString BcAsientoList::idAsientoAnterior()
{
    BL_FUNC_DEBUG
    if ( !m_cursorAsientos->isFirstRecord() ) {
        m_cursorAsientos->previousRecord();
        QString id = m_cursorAsientos->value( "idasiento" );
        m_cursorAsientos->nextRecord();
        
        return id;
    } else {
        
        return "";
    } // end if
}


///
/**
\return
**/
QString BcAsientoList::idAsientoSiguiente()
{
    BL_FUNC_DEBUG
    if ( !m_cursorAsientos->isLastRecord() ) {
        m_cursorAsientos->nextRecord();
        QString id = m_cursorAsientos->value( "idasiento" );
        m_cursorAsientos->previousRecord();
        
        return id;
    } else {
        
        return "";
    } // end if
}


///
/**
\param obj
\param event
\return
**/
bool eventos_mui_ordenAsiento::eventFilter ( QObject *obj, QEvent *event )
{
    BL_FUNC_DEBUG

    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *> ( event );
        if ( ( keyEvent->key() == Qt::Key_Enter ) || ( keyEvent->key() == Qt::Key_Return ) ) {
            objeto->mui_ordenAsiento_pulsadoIntro();
            return true;
        } // end if
    } // end if
    
    return QObject::eventFilter ( obj, event );
}


/// =================================================================


///
/**
**/
void BcAsientoList::botonFiltrar()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void BcAsientoList::muestraAsiento ( QString )
{
    blMsgInfo ( "Funcion no implementada." );
}


///
/**
\return
**/
bool BcAsientoList::esPrimerAsiento()
{
    BL_FUNC_DEBUG
    
    return m_cursorAsientos->isFirstRecord();
}


///
/**
\return
**/
bool BcAsientoList::esUltimoAsiento()
{
    BL_FUNC_DEBUG
    
    return m_cursorAsientos->isLastRecord();
}


///
/**
\param val
**/
void BcAsientoView::pintaFecha ( QString val )
{
    BL_FUNC_DEBUG
    mui_fecha->setText ( val );
}


///
/**
\param val
**/
void BcAsientoView::pintaOrdenAsiento ( QString val )
{
    BL_FUNC_DEBUG
    mui_ordenAsiento->setValue ( val.toInt() );
}


/// Pintamos la clase del asiento
/// 0 = Normal, 1 = Regularizacion, 2 = Cierre, 3 = Apertura
/**
\param val
**/
void BcAsientoView::pintaClase ( QString val )
{
    BL_FUNC_DEBUG
    mui_claseAsiento->setCurrentIndex ( val.toInt() );
    
}


///
/**
\param text
**/
void BcAsientoView::pintaComentariosAsiento ( QString text )
{
    BL_FUNC_DEBUG
    mui_comentariosAsiento->setPlainText ( text );
    
}


///
/**
\param v
**/
void BcAsientoView::muestraAsiento ( int v )
{
    BL_FUNC_DEBUG
    muestraAsiento ( QString::number ( v ) );
    
}


/// Desabilitamos el removeWindow ya que esta ventana no debe ser sacada ante un close.
/**
\return
**/
int BcAsientoView::removeWindow()
{
    BL_FUNC_DEBUG
    
    return 0;
}


///
/**
\param val
**/
void BcAsientoView::setFecha ( QString val )
{
    BL_FUNC_DEBUG
    mui_fecha->setText ( val );
}


///
/**
**/
void BcAsientoView::on_mui_abrirAsiento_clicked()
{
    BL_FUNC_DEBUG
    abrir();
}


/// SLOT de cierre de asiento.
/**
\return
**/
void BcAsientoView::on_mui_cerrarAsiento_clicked()
{
    BL_FUNC_DEBUG
    if ( BlFixed ( mui_descuadre->text() ) != 0 )  {
        blMsgInfo ( _("Asiento descuadrado, no se puede cerrar.") );
        return;
    } // end if
    cerrar();
}


///
/**
**/
void BcAsientoView::on_mui_guardarAsiento_clicked()
{
    BL_FUNC_DEBUG
    save();
    
}


/// Al pulsar return sobre el n&uacute;mero de asiento se procede como si fuese una
/// carga de dicho asiento.
/**
**/
void BcAsientoView::mui_ordenAsiento_pulsadoIntro()
{
    BL_FUNC_DEBUG
    boton_cargarasiento();
}


///
/**
\param ob
**/
eventos_mui_ordenAsiento::eventos_mui_ordenAsiento ( BcAsientoView *ob )
{
    BL_FUNC_DEBUG
    objeto = ob;
}

///
/**
\param finicial
\param ffinal
**/
void BcAsientoView::asientoRegularizacion ( QString finicial, QString ffinal )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Fecha inicial = '%1', fecha final = '%2'.")).arg(finicial).arg(ffinal) );
    try {

        /// Para poder generar un asiento de regularizacion debemos tener un asiento abierto.
        /// Sino, no merece la pena hacerlo.

        int idcuenta;
        int idcuenta1;
        BlFixed diferencia ( "0" );
        BlFixed totaldebe ( "0" ), totalhaber ( "0" );
        BlFixed totaldebe1 ( "0" ), totalhaber1 ( "0" );
        QString concepto = "Asiento de Regularizacion";
        QString fecha = ffinal;

        mainCompany() ->begin();

/*        QString querycomp = "SELECT * FROM asiento where clase = 1 and fecha <= '" + ffinal + "' AND fecha >= '" + ffinal + "'";                                                      
        BlDbRecordSet *curcomp = mainCompany() -> loadQuery(querycomp);                        
        while (!curcomp -> eof() ) {                                                       
                blMsgInfo("Vamos a borrar el asiento " + curcomp->value("ordenasiento"));
                muestraAsiento(curcomp->value("idasiento").toInt());                       
                BcAsientoForm::remove ( FALSE );                                                
                curcomp->nextRecord();                                              
        } // end if                                                                        
        delete curcomp;*/ 


        /// Creamos un asiento nuevo con la fecha final indicada.
        QString supquery = "INSERT INTO asiento (fecha, descripcion, comentariosasiento, clase) VALUES ('" + ffinal + "', 'Asiento de Regularizacion " + finicial + "--" + ffinal + "', 'Asiento de Regularizacion " + finicial + "--" + ffinal + "', 1)";
        mainCompany() ->runQuery ( supquery );
        supquery = "SELECT max(idasiento) as id FROM asiento";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( supquery );
        int idasiento = cur->value( "id" ).toInt();
        delete cur;


        /// El parametro esta en la configuracion de empresa.
        /// Buscamos la cuenta de regularizacion. Normalmente es la 129
        QString query = "SELECT * FROM cuenta WHERE codigo in (SELECT valor FROM configuracion WHERE nombre='CuentaRegularizacion')";
        cur = mainCompany() ->loadQuery ( query );
        if ( cur->eof() ) {
          blMsgInfo(_("Cuenta de Regularizacion incorrecta. Revise la configuracion"));
          throw - 1;
        } // end if
        idcuenta1 = cur->value( "idcuenta" ).toInt();
        delete cur;

        /// Hacemos el calculo de saldos hasta la fecha.
        query = "SELECT t1.codigo, t1.idcuenta, sum(t1.debe) AS sumdebe, sum(t1.haber) AS sumhaber, sum(t1.debe)-sum(t1.haber) AS saldito FROM ( SELECT cuenta.codigo AS codigo, cuenta.idcuenta AS idcuenta, apunte.debe AS debe, apunte.haber AS haber FROM (apunte LEFT JOIN cuenta ON apunte.idcuenta = cuenta.idcuenta) WHERE apunte.idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo LIKE '6%' OR codigo LIKE '7%') AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "'  ) AS t1 GROUP BY t1.idcuenta, t1.codigo ORDER BY saldito";
        cur = mainCompany() ->loadQuery ( query );
        if ( !cur ) throw - 1;
        int orden = 0;
        while ( !cur->eof() ) {
            orden++;
            idcuenta = cur->value( "idcuenta" ).toInt();
            diferencia = BlFixed ( cur->value( "sumdebe" ) ) - BlFixed ( cur->value( "sumhaber" ) );
            if ( diferencia > 0 ) {
                totalhaber = diferencia;
                totaldebe = 0;
            } else {
                totaldebe = -diferencia;
                totalhaber = 0;
            }// end if
            totaldebe1 = totaldebe1 + totaldebe;
            totalhaber1 = totalhaber1 + totalhaber;
            /// Insercion de Borrador
            /// El borrador no existe, por lo que hay que hacer un insert
            query = "INSERT INTO borrador (orden, conceptocontable, descripcion, fecha, idcuenta, debe, haber, idasiento) VALUES (";
            query +=  QString::number ( orden );
            query +=  ", '" + mainCompany() ->sanearCadena ( concepto ) + "'",
                      query +=  ", 'Asiento de Regularizacion " + finicial + "--" + ffinal + "'";
            query +=  ", '" + mainCompany() ->sanearCadena ( fecha ) + "'",
                      query +=  "," +   QString::number ( idcuenta );
            query +=  "," +   totaldebe.toQString().replace ( ",", "." );
            query +=  "," +   totalhaber.toQString().replace ( ",", "." );
            query +=  "," +   QString::number ( idasiento ) + ")";

            mainCompany() ->runQuery ( query );

            /// Fin de la insercion de Borrador
            cur->nextRecord();
        }// end while
        delete cur;


        if ( totaldebe1 > 0 ) {
            orden++;
            query = "INSERT INTO borrador (orden, conceptocontable, descripcion, fecha, idcuenta, debe, haber, idasiento) VALUES (";
            query += QString::number ( orden );
            query += ", '" + mainCompany() ->sanearCadena ( concepto ) + "'";
            query +=  ", 'Asiento de Regularizacion " + finicial + "--" + ffinal + "'";
            query += ", '" + mainCompany() ->sanearCadena ( fecha ) + "'";
            query += "," + QString::number ( idcuenta1 );
            query += ", 0";
            query += "," + totaldebe1.toQString().replace ( ",", "." );
            query += "," + QString::number ( idasiento ) + ")";
            mainCompany() ->runQuery ( query );
        } // end if


        if ( totalhaber1 > 0 ) {
            orden++;
            query = "INSERT INTO borrador (orden, conceptocontable, descripcion, fecha, idcuenta, debe, haber, idasiento) VALUES (";
            query += QString::number ( orden );
            query += ", '" + mainCompany() ->sanearCadena ( concepto ) + "'";
            query += ", 'Asiento de Regularizacion " + finicial + "--" + ffinal + "'";
            query += ", '" + mainCompany() ->sanearCadena ( fecha ) + "'";
            query += "," + QString::number ( idcuenta1 );
            query += "," + totalhaber1.toQString().replace ( ",", "." );
            query += ", 0";
            query += "," + QString::number ( idasiento ) + ")";
            mainCompany() ->runQuery ( query );
        }// end if

        mainCompany() ->commit();
        cargaAsientos();
        muestraAsiento ( idasiento );
    } catch ( ... ) {
        blMsgInfo ( "Error en los calculos" );
        mainCompany() ->rollback();
    } // end try
    
}



///
/**
**/
void BcAsientoView::asientoCierre ( QString finicial, QString ffinal )
{
    BL_FUNC_DEBUG
    try {
        mainCompany() ->begin();

        QString querycomp = "SELECT * FROM asiento WHERE clase = 2 AND fecha <= '" + ffinal + "' AND fecha >= '" + ffinal + "'";
        BlDbRecordSet *curcomp = mainCompany() -> loadQuery(querycomp);
        while (!curcomp -> eof() ) {
                blMsgInfo("Vamos a borrar el asiento " + curcomp->value("ordenasiento"));
                muestraAsiento(curcomp->value("idasiento").toInt());
                BcAsientoForm::remove ( FALSE );
                curcomp->nextRecord();
        } // end if
        delete curcomp;

        /// Creamos un asiento nuevo con la fecha final indicada.
        QString supquery = "INSERT INTO asiento (fecha, descripcion, comentariosasiento, clase) VALUES ('" + ffinal + "', 'Asiento de Cierre " + finicial + "--" + ffinal + "', 'Asiento de Cierre " + finicial + "--" + ffinal + "', 2)";
        mainCompany() ->runQuery ( supquery );
        supquery = "SELECT MAX(idasiento) AS id FROM asiento";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( supquery );
        int idasiento = cur->value( "id" ).toInt();
        delete cur;

        int idcuenta;
        QString snuevodebe, snuevohaber;
        /// Si no hay asiento lo calculamos.

        /// Hacemos el calculo de saldos hasta la fecha.
        QString query = "SELECT t1.codigo, t1.idcuenta, sum(t1.debe) AS sumdebe, sum(t1.haber) AS sumhaber, sum(t1.debe)-sum(t1.haber) AS saldito FROM ( SELECT cuenta.codigo AS codigo, cuenta.idcuenta AS idcuenta, apunte.debe AS debe, apunte.haber AS haber FROM (apunte LEFT JOIN cuenta ON apunte.idcuenta = cuenta.idcuenta) WHERE apunte.idcuenta NOT IN (SELECT idcuenta FROM cuenta WHERE codigo LIKE '6%' OR codigo LIKE '7%') AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "'  ) AS t1 GROUP BY t1.idcuenta, t1.codigo ORDER BY saldito";

//        QString query = "SELECT codigo, idcuenta, sum(debe) AS sumdebe, sum(haber) AS sumhaber, sum(debe)-sum(haber) AS saldito FROM apunte WHERE idcuenta NOT IN (SELECT idcuenta FROM cuenta WHERE codigo LIKE '6%' OR codigo LIKE '7%')  AND fecha <= '" + ffinal + "' AND fecha >= '" + finicial + "' GROUP BY idcuenta ORDER BY saldito";
        BlDbRecordSet *cursor = mainCompany() ->loadQuery ( query, "cursor" );

        int orden = 0;
        QString concepto = _("Asiento de cierre");
        QString fecha = ffinal;
        while ( ! cursor->eof() ) {
            orden++;
            idcuenta = cursor->value( "idcuenta" ).toInt();
            if ( cursor->value( "saldito" ).left ( 1 ) != "-" ) {
                snuevohaber = cursor->value( "saldito" );
                snuevodebe = "0";
            } else {
                snuevodebe = "ABS(" + cursor->value( "saldito" ) + ")";
                snuevohaber = "0";
            }// end if
            if ( cursor->value( "saldito" ) != "0.00" ) {
                /// Insercion de Borrador
                /// El borrador no existe, por lo que hay que hacer un insert
                query = "INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento) VALUES (",
                        query += QString::number ( orden );
                query += ", '" + mainCompany() ->sanearCadena ( concepto ) + "'";
                query += ", '" + mainCompany() ->sanearCadena ( fecha ) + "'";
                query += "," + QString::number ( idcuenta );
                query += "," + snuevodebe;
                query += "," + snuevohaber;
                query += "," + QString::number ( idasiento ) + ")";

                mainCompany() ->runQuery ( query );

            }// end if
            cursor->nextRecord();
        }// end while
        delete cursor;

        mainCompany() ->commit();
        cargaAsientos();
        muestraAsiento ( idasiento );
    } catch ( ... ) {
        blMsgInfo ( "Error en los calculos" );
        mainCompany() ->rollback();
    } // end try
}


///
/**
**/
void BcAsientoView::asientoApertura ( QString ffinal )
{
    try {
        mainCompany() ->begin();

        QString querycomp = "SELECT * FROM asiento where clase = 3 and fecha <= '" + ffinal + "' AND fecha >= '" + ffinal + "'";                                                      
        BlDbRecordSet *curcomp = mainCompany() -> loadQuery(querycomp);                        
        while (!curcomp -> eof() ) {                                                       
                blMsgInfo("Vamos a borrar el asiento " + curcomp->value("ordenasiento"));
                muestraAsiento(curcomp->value("idasiento").toInt());                       
                BcAsientoForm::remove ( FALSE );                                                
                curcomp->nextRecord();                                              
        } // end if                                                                        
        delete curcomp; 


        /// Creamos un asiento nuevo con la fecha final indicada.
        QString supquery = "INSERT INTO asiento (fecha, descripcion, comentariosasiento, clase) VALUES ('" + ffinal + "', 'Asiento de Apertura " + ffinal + "', 'Asiento de Apertura " + ffinal + "', 3)";
        mainCompany() ->runQuery ( supquery );
        supquery = "SELECT max(idasiento) as id FROM asiento";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( supquery );
        int idasiento = cur->value( "id" ).toInt();
        delete cur;

        /// Preparamos los datos.
        QString concepto = _("Asiento de apertura");
        QString fecha = ffinal;
        QString idasientocierre;

        /// Buscamos el asiento de cierre del ejercicio anterior a la fecha introducida.
//         QString SQLQuery = "SELECT * FROM asiento WHERE fecha < '" + fecha + "' AND idasiento < " + QString::number ( idasiento ) + " ORDER BY ordenasiento DESC";
	int anyo = fecha.right( 4 ).toInt () - 1;
	QString SQLQuery = "SELECT * FROM asiento WHERE descripcion similar to 'Asiento de Cierre%' AND EXTRACT(YEAR FROM fecha)=" + QString::number ( anyo ) + " ORDER BY ordenasiento DESC";
        cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur->eof() ) {
            idasientocierre = cur->value( "idasiento" );
        }// end if
        delete cur;

        int orden = 1;
        /// Seleccionamos todos sus registros de borrador.
        QString SQLQuery1 = "SELECT * FROM borrador WHERE idasiento=" + idasientocierre + " ORDER BY orden";
        cur = mainCompany() ->loadQuery ( SQLQuery1 );
        while ( !cur->eof() ) {
            QString idcuenta = cur->value( "idcuenta" );
            QString totaldebe = cur->value( "debe" );
            QString totalhaber = cur->value( "haber" );
            SQLQuery1  = "INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento) VALUES (",
                         SQLQuery1 += QString::number ( orden++ );
            SQLQuery1 += ", '" + mainCompany() ->sanearCadena ( concepto ) + "'";
            SQLQuery1 += ", '" + mainCompany() ->sanearCadena ( fecha ) + "'";
            SQLQuery1 += ","   + idcuenta;
            SQLQuery1 += ","   + totalhaber.replace ( ",", "." );
            SQLQuery1 += ","   + totaldebe.replace ( ",", "." );
            SQLQuery1 += ","   + QString::number ( idasiento ) + ")";

            mainCompany() ->runQuery ( SQLQuery1 );
            cur->nextRecord();
        }// end while
        delete cur;


        mainCompany() ->commit();
        cargaAsientos();
        muestraAsiento ( idasiento );
    } catch ( ... ) {
        blMsgInfo ( "Error en los calculos" );
        mainCompany() ->rollback();
    } // end try
}


///
/**
\param checked
**/
void BcAsientoView::on_mui_configurar_toggled ( bool checked )
{
    BL_FUNC_DEBUG
    if ( checked ) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    
}

///
/**
\param check
**/
void BcAsientoView::on_mui_list_toogledConfig ( bool check )
{
    BL_FUNC_DEBUG

    QToolButton *botonconfigurar = findChild<QToolButton *> ( "mui_configurar" );
    if ( botonconfigurar )
        botonconfigurar->setChecked ( check );
}

