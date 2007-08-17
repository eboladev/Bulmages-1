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

#include "asiento1view.h"
#include "busquedafecha.h"
#include "duplicarasientoview.h"
#include "aplinteligentesview.h"
#include "listlinasiento1view.h"
#include "empresa.h"


/// Constructor de la clase, inicializa los componentes
/** \param emp Empresa que llama al objeto.
    \param parent Widget padre de este.
*/
Asiento1View::Asiento1View(Empresa *emp, QWidget *parent, int)
        : ListAsientos(emp, parent) {
    setupUi(this);
    _depura("Asiento1View::Asiento1View", 0);

        /// Disparamos los plugins.
        int res = g_plugins->lanza("Asiento1View_Asiento1View", this);
        if (res != 0)
            return;


    eventos_mui_ordenasiento *eventosOrdenAsiento = new eventos_mui_ordenasiento(this);
    mui_ordenasiento->installEventFilter(eventosOrdenAsiento);

    mui_list->setEmpresaBase(emp);
    setListLinAsiento1(mui_list);

    /// Ocultamos los detalles del asiento.
    mui_detalles->toggle();
    /// Hacemos la carga del listado de asientos.
    cargaasientos();
    /// Desplazamos hasta el último asiento.
    boton_fin();
    empresaBase()->meteWindow(windowTitle(), this);
    _depura("END Asiento1View::Asiento1View", 0);
}


/** Saca la ventana del workSpace.
*/
Asiento1View::~Asiento1View() {
    _depura("Asiento1View::~Asiento1View", 0);
    _depura("END Asiento1View::~Asiento1View", 0);
}


void Asiento1View::calculaypintatotales(QString idasiento) {
    _depura("Asiento1View::calculaypintatotales", 0);
    m_totaldebe->setText(totaldebe(idasiento).toQString());
    m_totalhaber->setText(totalhaber(idasiento).toQString());
    Fixed desc = totaldebe(idasiento) - totalhaber(idasiento);
    m_descuadre->setText(desc.toQString());
    _depura("END Asiento1View::calculaypintatotales", 0);
}


void Asiento1View::trataestadoAsiento1() {
    _depura("Asiento1View::trataestadoAsiento1", 0);
    if (estadoAsiento1() == ASCerrado) {
        asientocerradop();
    } else {
        asientoabiertop();
    }// end if
    _depura("END Asiento1View::trataestadoAsiento1", 0);
}


/// Pone la pantalla en el modo de asiento abierto.
/** Activa los botones de cierre y pone los elementos como est&aacute;n configurados. */
void Asiento1View::asientoabiertop() {
    _depura("Asiento1View::asientoabiertop", 0);
    m_descuadre->setEnabled(TRUE);
    mui_abrirasiento->setEnabled(FALSE);
    mui_cerrarasiento->setEnabled(TRUE);
    mui_inteligente->setEnabled(TRUE);
    mui_list->setinsercion(TRUE);

    /// Los apuntes deben ser editables.
    for (int fila = 0; fila < mui_list->rowCount(); fila++) {
        for (int columna = 0; columna < mui_list->columnCount(); columna++) {
            mui_list->item(fila, columna)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        } // end for
    } // end for

    _depura("END Asiento1View::asientoabiertop", 0);
}


/// Pone la pantalla en el modo de asiento cerrado.
/** Activa los botones de apertura y pone los elementos como est&aacute;n configurados. */
void Asiento1View::asientocerradop() {
    _depura("Asiento1View::asientocerradop", 0);
    mui_abrirasiento->setEnabled(TRUE);
    mui_cerrarasiento->setEnabled(FALSE);
    mui_inteligente->setEnabled(TRUE);
    mui_list->setinsercion(FALSE);

    /// Los apuntes deben dejar de ser editables (aunque no se graben sus posibles
    /// modificaciones por estar en modo CERRADO).
    for (int fila = 0; fila < mui_list->rowCount(); fila++) {
        for (int columna = 0; columna < mui_list->columnCount(); columna++) {
            mui_list->item(fila, columna)->setFlags(Qt::ItemIsEnabled);
        } // end for
    } // end for

    _depura("END Asiento1View::asientocerradop", 0);
}


/// Esta funci&oacute;n se activa cuando se pulsa sobre el bot&oacute;n nuevo asiento del
/// formulario.
void Asiento1View::on_mui_nuevoasiento_clicked() {
    _depura("Asiento1View::on_mui_nuevoasiento_clicked", 0);
    mui_fecha->setText(QDate::currentDate().toString("dd/MM/yyyy"));
    iniciar_asiento_nuevo();
    _depura("END Asiento1View::on_mui_nuevoasiento_clicked", 0);
}


/// Esta funci&oacute;n se encarga de hacer las inicializaciones en un asiento nuevo.
void Asiento1View::iniciar_asiento_nuevo(QString nuevoordenasiento) {
    _depura("Asiento1View::iniciar_asiento_nuevo", 0);
    try {
        /// TRATAMIENTO DE BASE DE DATOS.
        QString idasiento, ordenasiento, query;
        QString fecha = mui_fecha->text();
        cursor2 *cur;
        empresaBase()->begin();
        if (nuevoordenasiento == "") {
            QString query = "SELECT COALESCE(MAX(ordenasiento) + 1, 1) AS orden FROM asiento WHERE EXTRACT(YEAR FROM fecha) = '" + fecha.left(10).right(4) + "'";
            cur = empresaBase()->cargacursor(query);
            ordenasiento = cur->valor("orden");
            delete cur;
        } else {
            ordenasiento = nuevoordenasiento;
        } // end if


        //empresaBase()->commit();
        query = "INSERT INTO asiento (fecha, ordenasiento) VALUES ('" + empresaBase()->sanearCadena(fecha) + "', " + ordenasiento + ")";
        empresaBase()->ejecuta(query);

        /// Creamos el asiento en la base de datos.
        query = "SELECT MAX(idasiento) AS id FROM asiento";
        cur = empresaBase()->cargacursor(query);
        if (!cur->eof())
            idasiento = cur->valor("id");
        delete cur;

        empresaBase()->commit();
        /// FIN TRATAMIENTO DE BASE DE DATOS.

        cargaasientos();

        muestraasiento(idasiento.toInt());

        abrir();

        _depura("END Asiento1View::iniciar_asiento_nuevo", 0, idasiento);
        return;
    } catch (...) {
        mensajeInfo("Asiento no pudo crearse");
        empresaBase()->rollback();
    } // end try
}


/** SLOT que se lanza cuando se ha terminado de editar la fecha.
    Si el asiento esta abierto cambia la fecha del asiento.
    Si el asiento esta cerrado crea un asiento nuevo llamando a \ref iniciar_asiento_nuevo()
*/
void Asiento1View::on_mui_fecha_returnPressed() {
    _depura("Asiento1View::on_mui_fecha_returnPressed", 0);
    /// Usamos un semaforo para prevenir de entradas concurrentes.
    static bool semaforo = FALSE;
    if (semaforo) return;
    semaforo = TRUE;
    /// Cambiar la fecha del asiento.
    if (estadoAsiento1() != Asiento1::ASCerrado && estadoAsiento1() != Asiento1::ASVacio) {
        setDBvalue("fecha", mui_fecha->text());
        Asiento1::guardar();
    } else {
        iniciar_asiento_nuevo();
    } // end if
    semaforo = FALSE;
    _depura("END Asiento1View::on_mui_fecha_returnPressed", 0);
}


/// Se ha pulsado sobre el bot&oacute;n de duplicar asiento.
/** Inicializa el di&aacute;logo de duplicaci&oacute;n de asientos y lo presenta.
    Cuando se ha terminado carga el cursor de presentaci&oacute;n y repinta el asiento
    para que actualize los cambios. */
void Asiento1View::on_mui_duplicar_clicked() {
    _depura("Asiento1View::on_mui_duplicar_clicked", 0);
    DuplicarAsientoView *dupli = new DuplicarAsientoView(empresaBase(), 0);
    /// Establecemos los par&aacute;metros para el nuevo asiento a duplicar.
    dupli->inicializa(mui_ordenasiento->text(), mui_ordenasiento->text());
    dupli->exec();
    cargaasientos();
    boton_fin();
    delete dupli;
    _depura("END Asiento1View::on_mui_duplicar_clicked", 0);
}


/// Se ha pulsado sobre el bot&oacute;n de generar asientos inteligentes.
/** Se inicializa la clase \ref aplinteligentesview y se muestra ese di&aacute;logo
    para que se opere con los asientos plantilla. */
void Asiento1View::on_mui_inteligente_clicked() {
    _depura("Asiento1View::on_mui_inteligente_clicked", 0);
    int numasiento;
    if (estadoAsiento1() != Asiento1::ASCerrado) {
        /// El asiento esta abierto y por tanto se muestra como abierto.
        asientoabiertop();
        numasiento = idasiento().toInt();
    } else {
        numasiento = 0;
    } // end if
    aplinteligentesview *nueva = new aplinteligentesview(empresaBase(), 0);
    nueva->inicializa(numasiento);
    empresaBase()->pWorkspace()->addWindow(nueva);
    nueva->show();
    _depura("END Asiento1View::on_mui_inteligente_clicked", 0);
}


/// Se ha pulsado sobre el bot&oacute;n de cargar asiento con lo
/// que debemos comprobar que el numero introducido es correcto
/// y hacer las gestiones oportunas para mostrar el asiento en
/// pantalla o crearlo si hace falta.
void Asiento1View::boton_cargarasiento() {
    _depura("Asiento1View::boton_cargarasiento", 0);
    QString idas = "";
    QString query = "SELECT idasiento FROM asiento WHERE ordenasiento = " + mui_ordenasiento->text() + " ORDER BY ordenasiento DESC";
    cursor2 *curs = empresaBase()->cargacursor(query);
    if (!curs->eof()) {
        idas = curs->valor("idasiento");
        cargar(idas);
    } else {
        /// Si el asiento no existe se da la posibilidad de crear uno nuevo.
        if (QMessageBox::question(this,
                                  tr("Asiento inexistente."),
                                  tr("Desea crear un nuevo asiento en esa posicion?"),
                                  tr("&Si"), tr("&No"), 0, 0, 1) == 0) {
            iniciar_asiento_nuevo(mui_ordenasiento->text());
        } // end if
        pintar();
    } // end if
    delete curs;
    _depura("END Asiento1View::boton_cargarasiento", 0);
}


void Asiento1View::muestraasiento(QString v) {
    _depura("Asiento1View::muestraasiento ", 0);
    situarasiento(v);
    cargar(v);
    _depura("END Asiento1View::muestraasiento ", 0);
}


/// Prepara para guardar.
void Asiento1View::prepguardar() {
    _depura("Asiento1View::prepguardar", 0);
    setDBvalue("fecha", mui_fecha->text());
    setDBvalue("ordenasiento", mui_ordenasiento->text());
    setDBvalue("comentariosasiento", mui_comentariosAsiento->toPlainText());
    setDBvalue("clase", QString::number(mui_claseAsiento->currentIndex()));
    _depura("END Asiento1View::prepguardar", 0);
}

void Asiento1View::on_mui_borrar_clicked() {
    bool atendido = TRUE; // asumimos que habra que atender al dialogo de confirmacion de borrado
    on_mui_borrar_clicked(atendido);
}

void Asiento1View::on_mui_borrar_clicked(bool atendido) {
    _depura("Asiento1View::on_mui_borrar_clicked", 0);
    QString idasientosig = idasientosiguiente();
    QString idasientoant = idasientoanterior();
    int resultadoborrar;
    resultadoborrar = Asiento1::borrar(atendido);

    /// Comprueba si se ha cancelado el borrado.
    if (resultadoborrar == 2) return;

    cargaasientos();
    if (idasientosig != "")
        muestraasiento(idasientosig);
    else {
        if (idasientoant != "")
            muestraasiento(idasientoant);
        else {
            vaciar();
            pintar();
        } // end if
    } // end if
    _depura("END Asiento1View::on_mui_borrar_clicked", 0);
}


/// =========================================================================================
///                       LISTASIENTOS
/// =========================================================================================

ListAsientos::ListAsientos(Empresa *emp, QWidget *parent) : Asiento1(emp, parent) {
    _depura("ListAsientos::ListAsientos", 0);
    cursorasientos = NULL;
    _depura("END ListAsientos::ListAsientos", 0);

}


ListAsientos::~ListAsientos() {
    _depura("ListAsientos::~ListAsientos", 0);
    if (cursorasientos != NULL ) {
        delete cursorasientos;
    } // end if
    _depura("END ListAsientos::~ListAsientos", 0);
}


/// Prepara el cursor que sirve para recorrer los asientos uno a uno.
/** Carga el cursor que sirve para hacer todo
    el recorrido a trav&eacute;s de los asientos.
    numasiento: 0 indica el primer asiento.
               -1 indica el &uacute;ltimo asiento.
    otros indica el asiento o el inmediatamente m&aacute;s bajo.
    Esta funci&oacute;n no hace cambios en la presentaci&oacute;n,
    s&oacute;lo realiza una carga del cursor que sirve para recorrer los asientos. */
void ListAsientos::cargaasientos() {
    _depura("ListAsientos::cargaasientos", 0);
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

    if (cursorasientos != NULL) {
        delete cursorasientos;
    } // end if

    /// Indica si se tiene que agregar el AND o no en el 'select'.
    int pand = 0;
    if (saldototal != "") {
        cadwhere = " WHERE ";
        textsaldototal = " idasiento IN (SELECT idasiento FROM (SELECT idasiento, sum(debe) AS total from apunte GROUP BY idasiento) AS foo WHERE foo.total = " + saldototal + ")";
        pand = 1;
    } // end if
    if (cantapunt != "") {
        cadwhere = " WHERE ";
        if (pand)
            textcantapunt = " AND ";
        textcantapunt += " idasiento IN (SELECT idasiento FROM apunte where debe = " + cantapunt + " OR haber = " + cantapunt + ")";
        pand = 1;
    } // end if
    if (nombreasiento != "") {
        cadwhere = " WHERE ";
        if (pand)
            textnombreasiento = " AND ";
        textnombreasiento += " idasiento in (SELECT idasiento FROM apunte WHERE conceptocontable LIKE '%" + nombreasiento + "%' )";
        pand = 1;
    } // end if
    /// Los ejercicios los pondremos como filtraje de la introducci&oacute;n de asientos.
    if (ejercicio != "--") {
        if (pand)
            textejercicio = " AND EXTRACT(YEAR FROM fecha) = '" + ejercicio + "'";
        else
            textejercicio = " WHERE EXTRACT(YEAR FROM fecha) = '" + ejercicio + "'";
    } // end if
    /// Se ordenan los asientos por a&ntilde;o y por n&uacute;mero de orden.
    query = "SELECT * FROM asiento " + cadwhere + textsaldototal + textcantapunt + textnombreasiento + textejercicio + " ORDER BY EXTRACT (YEAR FROM fecha), ordenasiento";
    cursorasientos = empresaBase()->cargacursor(query);
    if (cursorasientos->eof()) {
        _depura("No existe ningun asiento para mostrar.", 0);
    } // end if
    _depura("End ListAsientos::cargaasientos\n", 0);
}


/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de inicio.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si
    existen se desplaza al primer registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que
    no salga basura. */
void ListAsientos::boton_inicio() {
    _depura("ListAsientos::boton_inicio", 0);
    if (cursorasientos->numregistros() != 0) {
        cursorasientos->primerregistro();
        cargar(cursorasientos->valor("idasiento"));
    } // end if
    _depura("END ListAsientos::boton_inicio", 0);
}


/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de fin.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si existen
    se desplaza al &uacute;ltimo registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura. */
void ListAsientos::boton_fin() {
    _depura("ListAsientos::boton_fin", 0);
    if (cursorasientos->numregistros() != 0) {
        cursorasientos->ultimoregistro();
        cargar(cursorasientos->valor("idasiento"));
    } // end if
    _depura("END ListAsientos::boton_fin", 0);
}

/// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de siguiente registro.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si existen
    se desplaza al siguiente registro y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que no
    salga basura. */
void ListAsientos::boton_siguiente() {
    _depura("ListAsientos::boton_siguiente", 0);
    ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
    if (cursorasientos->numregistros() == 0) {
        return;
    } // end if
    if (!cursorasientos->esultimoregistro()) {
        cursorasientos->siguienteregistro();
        cargar(cursorasientos->valor("idasiento"));
    }// end if
    _depura("END ListAsientos::boton_siguiente", 0);
}


/// Slot que responde a la pulsaci&oacute;n del botón de anterior registro.
/** Comprueba que existen registros en el cursor de asientos a mostrar y si
    existen se desplaza al registro anterior y lo muestra.
    En el caso de que no haya asiento a mostrar vacia la pantalla para que
    no salga basura. */
void ListAsientos::boton_anterior() {
    _depura("ListAsientos::boton_anterior", 0);
    ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
    if (cursorasientos->numregistros() == 0) {
        return;
    } // end if
    if (!cursorasientos->esprimerregistro()) {
        cursorasientos->registroanterior();
        cargar(cursorasientos->valor("idasiento"));
    } // end if
    _depura("END ListAsientos::boton_anterior", 0);
}


/** Situa el cursor de asientos en un asiento determinado.
    \param idasiento El identificador del asiento en el que debe situarse.
    Si no se encuentra el asiento buscado devuelve una excepcion -1
*/
void ListAsientos::situarasiento(QString idasiento) {
    _depura("ListAsientos::situarasiento ", 0, idasiento);
    try {
        if (cursorasientos == NULL)
            throw -1;
        cursorasientos->primerregistro();
        while (cursorasientos->valor("idasiento") != idasiento && !cursorasientos->esultimoregistro()) {
            cursorasientos->siguienteregistro();
        } // end while
    } catch (...) {
        mensajeInfo("Error al intentar situarse en el asiento");
        throw -1;
    } // end try
    _depura("END ListAsientos::situarasiento", 0, idasiento);
}


QString ListAsientos::idasientoanterior() {
    _depura("ListAsientos::idasientoanterior", 0);
    if (!cursorasientos->esprimerregistro()) {
        cursorasientos->registroanterior();
        QString id = cursorasientos->valor("idasiento");
        cursorasientos->siguienteregistro();
        _depura("END ListAsientos::idasientoanterior", 0);
        return id;
    } else {
        return "";
    } // end if
}


QString ListAsientos::idasientosiguiente() {
    _depura("ListAsientos::idasientosiguiente", 0);
    if (!cursorasientos->esultimoregistro()) {
        cursorasientos->siguienteregistro();
        QString id = cursorasientos->valor("idasiento");
        cursorasientos->registroanterior();
        _depura("END ListAsientos::idasientosiguiente", 0);
        return id;
    } else {
        return "";
    } // end if
}


bool eventos_mui_ordenasiento::eventFilter(QObject *obj, QEvent *event) {
    _depura("eventos_mui_ordenasiento::eventFilter", 0);

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)) {
            objeto->mui_ordenasiento_pulsadoIntro();
            return true;
        } // end if
    } // end if
    return QObject::eventFilter(obj, event);
}


/// =================================================================


void ListAsientos::boton_filtrar() {
    _depura("Funcion no implementada", 2);
}



void ListAsientos::muestraasiento(QString) {
    mensajeInfo("Funcion no implementada.");
}


bool ListAsientos::esprimerasiento() {
    return cursorasientos->esprimerregistro();
}


bool ListAsientos::esultimoasiento() {
    return cursorasientos->esultimoregistro();
}


void Asiento1View::pintafecha(QString val) {
    mui_fecha->setText(val);
}


void Asiento1View::pintaordenasiento(QString val) {
    mui_ordenasiento->setValue(val.toInt());
}


void Asiento1View::pintaclase(QString val) {
    mui_claseAsiento->setCurrentIndex(val.toInt());
}


void Asiento1View::pintacomentariosasiento(QString text) {
    mui_comentariosAsiento->setPlainText(text);
}


void Asiento1View::muestraasiento(int v) {
    muestraasiento(QString::number(v));
}


/// Desabilitamos el sacaWindow ya que esta ventana no debe ser sacada ante un close.
int Asiento1View::sacaWindow() {
    return 0;
}


void Asiento1View::setFecha(QString val) {
    mui_fecha->setText(val);
}


void Asiento1View::on_mui_abrirasiento_clicked() {
    abrir();
}


void Asiento1View::on_mui_cerrarasiento_clicked() {
    prepguardar();
    cerrar();
}


void Asiento1View::on_mui_guardarasiento_clicked() {
    prepguardar();
    Asiento1::guardar();
}




void Asiento1View::s_lineaValueChanged() {
    calculaypintatotales(idasiento());
}


/// Al pulsar return sobre el n&uacute;mero de asiento se procede como si fuese una
/// carga de dicho asiento.
void Asiento1View::mui_ordenasiento_pulsadoIntro() {
    boton_cargarasiento();
}


eventos_mui_ordenasiento::eventos_mui_ordenasiento(Asiento1View *ob) {
    objeto = ob;
}

