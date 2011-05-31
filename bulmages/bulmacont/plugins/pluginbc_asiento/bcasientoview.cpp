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
    blDebug ( "BcAsientoView::BcAsientoView", 0 );
    setupUi ( this );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BcAsientoView_BcAsientoView", this );
    if ( res != 0 )
        return;


    eventos_mui_ordenasiento *eventosOrdenAsiento = new eventos_mui_ordenasiento ( this );
    mui_ordenasiento->installEventFilter ( eventosOrdenAsiento );

    /// Conectamos el siganl returnPressed de mui_fecha
    connect ( mui_fecha, SIGNAL ( returnPressed() ), this, SLOT ( on_mui_fecha_enterPressed() ) );

    mui_list->setMainCompany ( emp );
    setListLinAsiento1 ( mui_list );

    /// Ocultamos los detalles del asiento.
    mui_detalles->toggle();
    /// Hacemos la carga del listado de asientos.
    cargaasientos();
    /// Desplazamos hasta el ultimo asiento.
    boton_fin();
    mainCompany() ->insertWindow ( windowTitle(), this );
    dialogChanges_readValues();

    /// Conectamos los botones del menu con las acciones de esta ventana.
    connect (((BcBulmaCont *)g_main)->actionSiguiente, SIGNAL(triggered()), this, SLOT(boton_siguiente()));
    connect (((BcBulmaCont *)g_main)->actionAnterior, SIGNAL(triggered()), this, SLOT(boton_anterior()));
    connect (((BcBulmaCont *)g_main)->actionInicio, SIGNAL(triggered()), this, SLOT(boton_inicio()));
    connect (((BcBulmaCont *)g_main)->actionFin, SIGNAL(triggered()), this, SLOT(boton_fin()));

    blDebug ( "END BcAsientoView::BcAsientoView", 0 );
}


/// Saca la ventana del workSpace.
/**
**/
BcAsientoView::~BcAsientoView()
{
    blDebug ( "BcAsientoView::~BcAsientoView", 0 );
    blDebug ( "END BcAsientoView::~BcAsientoView", 0 );
}


///
/**
**/
void BcAsientoView::calculaypintatotales()
{
    blDebug ( "BcAsientoView::calculaypintatotales", 0 );
    BlFixed tdebe = mui_list->sumarCampo ( "debe" );
    BlFixed thaber = mui_list->sumarCampo ( "haber" );
    m_totaldebe->setText ( tdebe.toQString() );
    m_totalhaber->setText ( thaber.toQString() );
    BlFixed desc = tdebe - thaber;
    m_descuadre->setText ( desc.toQString() );

    /// Si hay descuadre no se permite cerrar el asiento. Y si el asiento esta abierto y cuadrado se permite el cierre del asiento.
    if ( desc != 0 ) {
        mui_cerrarasiento->setEnabled ( FALSE );
    } else if ( !mui_abrirasiento->isEnabled() ) {
        mui_cerrarasiento->setEnabled ( TRUE );
    } // end ir
    blDebug ( "END BcAsientoView::calculaypintatotales", 0 );
}


///
/**
**/
void BcAsientoView::trataestadoBcAsientoForm()
{
    blDebug ( "BcAsientoView::trataestadoBcAsientoForm", 0 );
    if ( estadoBcAsientoForm() == ASCerrado ) {
        asientocerradop();
    } else {
        asientoabiertop();
    }// end if
    blDebug ( "END BcAsientoView::trataestadoBcAsientoForm", 0 );
}


/// Pone la pantalla en el modo de asiento abierto.
/** Activa los botones de cierre y pone los elementos como est&aacute;n configurados. */
/**
**/
void BcAsientoView::asientoabiertop()
{
    blDebug ( "BcAsientoView::asientoabiertop", 0 );
    m_descuadre->setEnabled ( TRUE );
    mui_abrirasiento->setEnabled ( FALSE );
    mui_cerrarasiento->setEnabled ( TRUE );
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

    blDebug ( "END BcAsientoView::asientoabiertop", 0 );
}


/// Pone la pantalla en el modo de asiento cerrado.
/** Activa los botones de apertura y pone los elementos como est&aacute;n configurados. */
/**
**/
void BcAsientoView::asientocerradop()
{
    blDebug ( "BcAsientoView::asientocerradop", 0 );
    mui_abrirasiento->setEnabled ( TRUE );
    mui_cerrarasiento->setEnabled ( FALSE );
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

    blDebug ( "END BcAsientoView::asientocerradop", 0 );
}


/// Esta funci&oacute;n se activa cuando se pulsa sobre el bot&oacute;n nuevo asiento del
/// formulario.
/**
**/
void BcAsientoView::on_mui_nuevoasiento_clicked()
{
    blDebug ( "BcAsientoView::on_mui_nuevoasiento_clicked", 0 );
    mui_fecha->setText ( QDate::currentDate().toString ( "dd/MM/yyyy" ) );
    iniciar_asiento_nuevo();
    blDebug ( "END BcAsientoView::on_mui_nuevoasiento_clicked", 0 );
}


/// Esta funci&oacute;n se encarga de hacer las inicializaciones en un asiento nuevo.
/**
\param nuevoordenasiento
\return
**/
void BcAsientoView::iniciar_asiento_nuevo ( QString nuevoordenasiento )
{
    blDebug ( "BcAsientoView::iniciar_asiento_nuevo", 0 );
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
        } else {
            ordenasiento = nuevoordenasiento;
        } // end if

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

        cargaasientos();

        muestraasiento ( idasiento );

        abrir();

        blDebug ( "END BcAsientoView::iniciar_asiento_nuevo", 0 );
        return;
    } catch ( ... ) {
        blMsgInfo ( "Asiento no pudo crearse" );
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
    blDebug ( "BcAsientoView::on_mui_fecha_enterPressed", 0 );
    /// Usamos un semaforo para prevenir de entradas concurrentes.
    static bool semaforo = FALSE;
    if ( semaforo ) return;
    semaforo = TRUE;
    /// Cambiar la fecha del asiento.
    if ( estadoBcAsientoForm() != BcAsientoForm::ASCerrado && estadoBcAsientoForm() != BcAsientoForm::ASVacio ) {
        setDbValue ( "fecha", mui_fecha->text() );
        BcAsientoForm::save();
    } else {
        iniciar_asiento_nuevo();
    } // end if
    semaforo = FALSE;
    blDebug ( "END BcAsientoView::on_mui_fecha_enterPressed", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n de duplicar asiento.
/** Inicializa el di&aacute;logo de duplicaci&oacute;n de asientos y lo presenta.
    Cuando se ha terminado carga el cursor de presentaci&oacute;n y repinta el asiento
    para que actualize los cambios. */
/**
**/
void BcAsientoView::on_mui_duplicar_clicked()
{
    blDebug ( "BcAsientoView::on_mui_duplicar_clicked", 0 );

    blDebug ( "END BcAsientoView::on_mui_duplicar_clicked", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n de generar asientos inteligentes.
/** Se inicializa la clase \ref BcAsientoInteligenteView y se muestra ese di&aacute;logo
    para que se opere con los asientos plantilla. */
/**
**/
void BcAsientoView::on_mui_inteligente_clicked()
{
    blDebug ( "BcAsientoView::on_mui_inteligente_clicked", 0 );
    int numasiento;
    if ( estadoBcAsientoForm() != BcAsientoForm::ASCerrado ) {
        /// El asiento esta abierto y por tanto se muestra como abierto.
        asientoabiertop();
        numasiento = idasiento().toInt();
    } else {
        numasiento = 0;
    } // end if
    BcAsientoInteligenteView *nueva = new BcAsientoInteligenteView ( mainCompany(), 0 );
    nueva->inicializa ( numasiento );
    nueva->show();
    blDebug ( "END BcAsientoView::on_mui_inteligente_clicked", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n de cargar asiento con lo
/// que debemos comprobar que el numero introducido es correcto
/// y hacer las gestiones oportunas para mostrar el asiento en
/// pantalla o crearlo si hace falta.
/**
**/
void BcAsientoView::boton_cargarasiento()
{
    blDebug ( "BcAsientoView::boton_cargarasiento", 0 );
    QString idas = "";
    QString query = "SELECT idasiento FROM asiento WHERE ordenasiento = " + mui_ordenasiento->text() + " ORDER BY ordenasiento DESC";
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
            iniciar_asiento_nuevo ( mui_ordenasiento->text() );
        } // end if
        pintar();
    } // end if
    delete curs;
    blDebug ( "END BcAsientoView::boton_cargarasiento", 0 );
}


///
/**
\param v
**/
void BcAsientoView::muestraasiento ( QString v )
{
    blDebug ( "BcAsientoView::muestraasiento ", 0 );
    situarasiento ( v );
    load ( v );
    blDebug ( "END BcAsientoView::muestraasiento ", 0 );
}


/// Prepara para guardar.
/**
**/
void BcAsientoView::prepareSave()
{
    blDebug ( "BcAsientoView::prepareSave", 0 );
    setDbValue ( "fecha", mui_fecha->text() );
    setDbValue ( "ordenasiento", mui_ordenasiento->text() );
    setDbValue ( "comentariosasiento", mui_comentariosAsiento->toPlainText() );
    setDbValue ( "clase", QString::number ( mui_claseAsiento->currentIndex() ) );
    blDebug ( "END BcAsientoView::prepareSave", 0 );
}


/// Post Guardado se hace una carga ya que la base de datos podria haber modificado cosas.
/**
**/
int BcAsientoView::afterSave()
{
    blDebug ( "BcAsientoView::afterSave", 0 );
    blDebug ( "BcAsientoView::afterSave", 0 );
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
    blDebug ( "BcAsientoView::on_mui_borrar_clicked", 0 );
    QString idasientosig = idasientosiguiente();
    QString idasientoant = idasientoanterior();
    int resultadoborrar;
    resultadoborrar = BcAsientoForm::remove ( atendido );

    /// Comprueba si se ha cancelado el borrado.
    if ( resultadoborrar == 2 ) return;

    cargaasientos();
    if ( idasientosig != "" )
        muestraasiento ( idasientosig );
    else {
        if ( idasientoant != "" )
            muestraasiento ( idasientoant );
        else {
            vaciar();
            pintar();
        } // end if
    } // end if
    blDebug ( "END BcAsientoView::on_mui_borrar_clicked", 0 );
}


///
/**
**/
void BcAsientoView::on_mui_list_editFinish ( int, int )
{
    blDebug ( "BcAsientoView::on_mui_list_editFinish", 0 );
    calculaypintatotales();
    blDebug ( "END BcAsientoView::on_mui_list_editFinish", 0 );
}


/// =========================================================================================
///                       LISTASIENTOS
/// =========================================================================================


///
/**
\param emp
\param parent
**/
BcAsientoList::BcAsientoList ( BcCompany *emp, QWidget *parent ) : BcAsientoForm ( emp, parent )
{
    blDebug ( "BcAsientoList::BcAsientoList", 0 );
    cursorasientos = NULL;
    blDebug ( "END BcAsientoList::BcAsientoList", 0 );

}


///
/**
**/
BcAsientoList::~BcAsientoList()
{
    blDebug ( "BcAsientoList::~BcAsientoList", 0 );
    if ( cursorasientos != NULL ) {
        delete cursorasientos;
    } // end if
    blDebug ( "END BcAsientoList::~BcAsientoList", 0 );
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
void BcAsientoList::cargaasientos()
{
    blDebug ( "BcAsientoList::cargaasientos", 0 );
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

    if ( cursorasientos != NULL ) {
        delete cursorasientos;
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
    cursorasientos = mainCompany() ->loadQuery ( query );
    if ( cursorasientos->eof() ) {
        blDebug ( "No existe ningun asiento para mostrar.", 0 );
    } // end if
    blDebug ( "END BcAsientoList::cargaasientos", 0 );
}


/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de inicio.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si
    existen se desplaza al primer registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que
    no salga basura. */
/**
**/
void BcAsientoList::boton_inicio()
{
    blDebug ( "BcAsientoList::boton_inicio", 0 );
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
    if ( cursorasientos->numregistros() != 0 ) {
        cursorasientos->firstRecord();
        load ( cursorasientos->value( "idasiento" ) );
    } // end if
    } // end if
    blDebug ( "END BcAsientoList::boton_inicio", 0 );
}


/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de fin.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si existen
    se desplaza al &uacute;ltimo registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura. */
/**
**/
void BcAsientoList::boton_fin()
{
    blDebug ( "BcAsientoList::boton_fin", 0 );
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
    if ( cursorasientos->numregistros() != 0 ) {
        cursorasientos->lastRecord();
        load ( cursorasientos->value( "idasiento" ) );
    } // end if
    } // end if
    blDebug ( "END BcAsientoList::boton_fin", 0 );
}

/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de siguiente registro.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si existen
    se desplaza al siguiente registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que no
    salga basura. */
/**
\return
**/
void BcAsientoList::boton_siguiente()
{
    blDebug ( "BcAsientoList::boton_siguiente", 0 );
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
    ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
    if ( cursorasientos->numregistros() == 0 ) {
        return;
    } // end if
    if ( !cursorasientos->isLastRecord() ) {
        cursorasientos->nextRecord();
        load ( cursorasientos->value( "idasiento" ) );
    }// end if
    } // end if
    blDebug ( "END BcAsientoList::boton_siguiente", 0 );
}


/// Slot que responde a la pulsaci&oacute;n del botÃ³n de anterior registro.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si
    existen se desplaza al registro anterior y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que
    no salga basura. */
/**
\return
**/
void BcAsientoList::boton_anterior()
{
    blDebug ( "BcAsientoList::boton_anterior", 0 );
    if(mainCompany()->pWorkspace()->activeWindow() == this) {
    ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
    if ( cursorasientos->numregistros() == 0 ) {
        return;
    } // end if
    if ( !cursorasientos->isFirstRecord() ) {
        cursorasientos->previousRecord();
        load ( cursorasientos->value( "idasiento" ) );
    } // end if
    } // end if
    blDebug ( "END BcAsientoList::boton_anterior", 0 );
}


/** Situa el cursor de asientos en un asiento determinado.
    \param idasiento El identificador del asiento en el que debe situarse.
    Si no se encuentra el asiento buscado devuelve una excepcion -1
*/
/**
\param idasiento
**/
void BcAsientoList::situarasiento ( QString idasiento )
{
    blDebug ( "BcAsientoList::situarasiento ", 0, idasiento );
    try {
        if ( cursorasientos == NULL )
            throw - 1;
        cursorasientos->firstRecord();
        while ( cursorasientos->value( "idasiento" ) != idasiento && !cursorasientos->isLastRecord() ) {
            cursorasientos->nextRecord();
        } // end while
    } catch ( ... ) {
        blMsgError ( "Error al intentar situarse en el asiento.\nEs posible que el asiento ya no exista en la base de datos.\nActualice el listado de asientos." );
        throw - 1;
    } // end try
    blDebug ( "END BcAsientoList::situarasiento", 0, idasiento );
}


///
/**
\return
**/
QString BcAsientoList::idasientoanterior()
{
    blDebug ( "BcAsientoList::idasientoanterior", 0 );
    if ( !cursorasientos->isFirstRecord() ) {
        cursorasientos->previousRecord();
        QString id = cursorasientos->value( "idasiento" );
        cursorasientos->nextRecord();
        blDebug ( "END BcAsientoList::idasientoanterior", 0 );
        return id;
    } else {
        blDebug ( "END BcAsientoList::idasientoanterior", 0 );
        return "";
    } // end if
}


///
/**
\return
**/
QString BcAsientoList::idasientosiguiente()
{
    blDebug ( "BcAsientoList::idasientosiguiente", 0 );
    if ( !cursorasientos->isLastRecord() ) {
        cursorasientos->nextRecord();
        QString id = cursorasientos->value( "idasiento" );
        cursorasientos->previousRecord();
        blDebug ( "END BcAsientoList::idasientosiguiente", 0 );
        return id;
    } else {
        blDebug ( "END BcAsientoList::idasientosiguiente", 0 );
        return "";
    } // end if
}


///
/**
\param obj
\param event
\return
**/
bool eventos_mui_ordenasiento::eventFilter ( QObject *obj, QEvent *event )
{
    blDebug ( "eventos_mui_ordenasiento::eventFilter", 0 );

    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *> ( event );
        if ( ( keyEvent->key() == Qt::Key_Enter ) || ( keyEvent->key() == Qt::Key_Return ) ) {
            objeto->mui_ordenasiento_pulsadoIntro();
            return true;
        } // end if
    } // end if
    blDebug ( "END eventos_mui_ordenasiento::eventFilter", 0 );
    return QObject::eventFilter ( obj, event );
}


/// =================================================================


///
/**
**/
void BcAsientoList::boton_filtrar()
{
    blDebug ( "Funcion no implementada", 2 );
}


///
/**
**/
void BcAsientoList::muestraasiento ( QString )
{
    blMsgInfo ( "Funcion no implementada." );
}


///
/**
\return
**/
bool BcAsientoList::esprimerasiento()
{
    blDebug ( "BcAsientoList::esprimerasiento", 0 );
    blDebug ( "END BcAsientoList::esprimerasiento", 0 );
    return cursorasientos->isFirstRecord();
}


///
/**
\return
**/
bool BcAsientoList::esultimoasiento()
{
    blDebug ( "BcAsientoList::esultimoasiento", 0 );
    blDebug ( "END BcAsientoList::esultimoasiento", 0 );
    return cursorasientos->isLastRecord();
}


///
/**
\param val
**/
void BcAsientoView::pintaFecha ( QString val )
{
    blDebug ( "BcAsientoView::pintaFecha", 0 );
    mui_fecha->setText ( val );
    blDebug ( "END BcAsientoView::pintaFecha", 0 );
}


///
/**
\param val
**/
void BcAsientoView::pintaordenasiento ( QString val )
{
    blDebug ( "BcAsientoView::pintaordenasiento", 0 );
    mui_ordenasiento->setValue ( val.toInt() );
    blDebug ( "END BcAsientoView::pintaordenasiento", 0 );
}


/// Pintamos la clase del asiento
/// 0 = Normal, 1 = Regularizacion, 2 = Cierre, 3 = Apertura
/**
\param val
**/
void BcAsientoView::pintaclase ( QString val )
{
    blDebug ( "BcAsientoView::pintaclase", 0 );
    mui_claseAsiento->setCurrentIndex ( val.toInt() );
    blDebug ( "END BcAsientoView::pintaclase", 0 );
}


///
/**
\param text
**/
void BcAsientoView::pintacomentariosasiento ( QString text )
{
    blDebug ( "BcAsientoView::pintacomentariosasiento", 0 );
    mui_comentariosAsiento->setPlainText ( text );
    blDebug ( "END BcAsientoView::pintacomentariosasiento", 0 );
}


///
/**
\param v
**/
void BcAsientoView::muestraasiento ( int v )
{
    blDebug ( "BcAsientoView::muestraasiento", 0 );
    muestraasiento ( QString::number ( v ) );
    blDebug ( "END BcAsientoView::muestraasiento", 0 );
}


/// Desabilitamos el removeWindow ya que esta ventana no debe ser sacada ante un close.
/**
\return
**/
int BcAsientoView::removeWindow()
{
    blDebug ( "BcAsientoView::removeWindow", 0 );
    blDebug ( "END BcAsientoView::removeWindow", 0 );
    return 0;
}


///
/**
\param val
**/
void BcAsientoView::setFecha ( QString val )
{
    blDebug ( "BcAsientoView::setFecha", 0 );
    mui_fecha->setText ( val );
    blDebug ( "END BcAsientoView::setFecha", 0 );
}


///
/**
**/
void BcAsientoView::on_mui_abrirasiento_clicked()
{
    blDebug ( "BcAsientoView::on_mui_abrirasiento_clicked", 0 );
    abrir();
    blDebug ( "END BcAsientoView::on_mui_abrirasiento_clicked", 0 );
}


/// SLOT de cierre de asiento.
/**
\return
**/
void BcAsientoView::on_mui_cerrarasiento_clicked()
{
    blDebug ( "BcAsientoView::on_mui_cerrarasiento_clicked", 0 );
    if ( BlFixed ( m_descuadre->text() ) != 0 )  {
        blMsgInfo ( "Asiento descuadrado, no se puede cerrar" );
        blDebug ( "END BcAsientoView::on_mui_cerrarasiento_clicked", 0, "Descuadrado" );
        return;
    } // end if
    prepareSave();
    cerrar();
    blDebug ( "END BcAsientoView::on_mui_cerrarasiento_clicked", 0 );
}


///
/**
**/
void BcAsientoView::on_mui_guardarasiento_clicked()
{
    blDebug ( "BcAsientoView::on_mui_guardarasiento_clicked", 0 );
    prepareSave();
    BcAsientoForm::save();
    blDebug ( "END BcAsientoView::on_mui_guardarasiento_clicked", 0 );
}


/// Al pulsar return sobre el n&uacute;mero de asiento se procede como si fuese una
/// carga de dicho asiento.
/**
**/
void BcAsientoView::mui_ordenasiento_pulsadoIntro()
{
    blDebug ( "BcAsientoView::mui_ordenasiento_pulsadoIntro", 0 );
    boton_cargarasiento();
    blDebug ( "END BcAsientoView::mui_ordenasiento_pulsadoIntro", 0 );
}


///
/**
\param ob
**/
eventos_mui_ordenasiento::eventos_mui_ordenasiento ( BcAsientoView *ob )
{
    blDebug ( "eventos_mui_ordenasiento::eventos_mui_ordenasiento", 0 );
    objeto = ob;
    blDebug ( "END eventos_mui_ordenasiento::eventos_mui_ordenasiento", 0 );
}

///
/**
\param finicial
\param ffinal
**/
void BcAsientoView::asiento_regularizacion ( QString finicial, QString ffinal )
{
    blDebug ( "BcAsientoView::regularizacion", 0, finicial + "--" + ffinal );
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
                muestraasiento(curcomp->value("idasiento").toInt());                       
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
        cargaasientos();
        muestraasiento ( idasiento );
    } catch ( ... ) {
        blMsgInfo ( "Error en los calculos" );
        mainCompany() ->rollback();
    } // end try
    blDebug ( "END BcAsientoView::regularizacion", 0 );
}



///
/**
**/
void BcAsientoView::asiento_cierre ( QString finicial, QString ffinal )
{
    blDebug ( "BcAsientoView::asiento_cierre", 0 );
    try {
        mainCompany() ->begin();

        QString querycomp = "SELECT * FROM asiento where clase = 2 and fecha <= '" + ffinal + "' AND fecha >= '" + ffinal + "'";                                                      
        BlDbRecordSet *curcomp = mainCompany() -> loadQuery(querycomp);                        
        while (!curcomp -> eof() ) {                                                       
                blMsgInfo("Vamos a borrar el asiento " + curcomp->value("ordenasiento"));
                muestraasiento(curcomp->value("idasiento").toInt());                       
                BcAsientoForm::remove ( FALSE );                                                
                curcomp->nextRecord();                                              
        } // end if                                                                        
        delete curcomp; 

        /// Creamos un asiento nuevo con la fecha final indicada.
        QString supquery = "INSERT INTO asiento (fecha, descripcion, comentariosasiento, clase) VALUES ('" + ffinal + "', 'Asiento de Cierre " + finicial + "--" + ffinal + "', 'Asiento de Cierre " + finicial + "--" + ffinal + "', 2)";
        mainCompany() ->runQuery ( supquery );
        supquery = "SELECT max(idasiento) as id FROM asiento";
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
        QString concepto = "Asiento de Cierre";
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
        cargaasientos();
        muestraasiento ( idasiento );
    } catch ( ... ) {
        blMsgInfo ( "Error en los calculos" );
        mainCompany() ->rollback();
    } // end try
}


///
/**
**/
void BcAsientoView::asiento_apertura ( QString ffinal )
{
    try {
        mainCompany() ->begin();

        QString querycomp = "SELECT * FROM asiento where clase = 3 and fecha <= '" + ffinal + "' AND fecha >= '" + ffinal + "'";                                                      
        BlDbRecordSet *curcomp = mainCompany() -> loadQuery(querycomp);                        
        while (!curcomp -> eof() ) {                                                       
                blMsgInfo("Vamos a borrar el asiento " + curcomp->value("ordenasiento"));
                muestraasiento(curcomp->value("idasiento").toInt());                       
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
        QString concepto = "Asiento de Apertura";
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
        cargaasientos();
        muestraasiento ( idasiento );
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
    blDebug ( "BcAsientoView::on_mui_configurar_toggled", 0 );
    if ( checked ) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    blDebug ( "END BcAsientoView::on_mui_configurar_toggled", 0 );
}

///
/**
\param check
**/
void BcAsientoView::on_mui_list_toogledConfig ( bool check )
{
    blDebug ( "BcAsientoView::on_mui_list_toogledConfig", 0 );

    QToolButton *botonconfigurar = findChild<QToolButton *> ( "mui_configurar" );
    if ( botonconfigurar )
        botonconfigurar->setChecked ( check );
    blDebug ( "END BcAsientoView::on_mui_list_toogledConfig", 0 );
}

