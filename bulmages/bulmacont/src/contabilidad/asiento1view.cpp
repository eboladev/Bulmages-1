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
    \param nam Nombre que recibe el widget o ventana. */
Asiento1View::Asiento1View(empresa *emp, QWidget *parent, int)
        : QWidget (parent), Asiento1(emp), ListAsientos(emp) {
    setupUi(this);
    _depura("Asiento1View::Asiento1View\n", 0);
    m_companyact = emp;

    mui_list->setcompany(m_companyact);
    setListLinAsiento1(mui_list);

    /// Ocultamos los detalles del asiento.
    mui_detalles->toggle();
    /// Hacemos la carga del listado de asientos.
    cargaasientos();
    /// Desplazamos hasta el último asiento.
    boton_fin();
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END Asiento1View::Asiento1View\n", 0);
}


/// Destruye los objetos creados y libera la memoria.
Asiento1View::~Asiento1View() {
    _depura("Asiento1View::~Asiento1View\n", 0);
    m_companyact->sacaWindow(this);
    _depura("END Asiento1View::~Asiento1View\n", 0);
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
    mui_iva->setEnabled(TRUE);
    mui_inteligente->setEnabled(TRUE);

    /// Los apuntes deben ser editables.
    for (int fila = 0; fila < mui_list->rowCount(); fila++) {
        for (int columna = 0; columna < mui_list->columnCount(); columna++) {
            mui_list->item(fila, columna)->setFont(QFont("Decorative", -1, -1, false));
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
    mui_iva->setEnabled(FALSE);
    mui_inteligente->setEnabled(TRUE);

    /// Los apuntes deben dejar de ser editables (aunque no se graben sus posibles
    /// modificaciones por estar en modo CERRADO).
    for (int fila = 0; fila < mui_list->rowCount(); fila++) {
        for (int columna = 0; columna < mui_list->columnCount(); columna++) {
            mui_list->item(fila, columna)->setFont(QFont("Courier", -1, -1, false));
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
void Asiento1View::iniciar_asiento_nuevo() {
    _depura("Asiento1View::iniciar_asiento_nuevo", 0);
    try {
        /// TRATAMIENTO DE BASE DE DATOS.
        QString idasiento = "";
        QString ordenasiento = "1";
        QString fecha = mui_fecha->text();
        QString query = "SELECT MAX(ordenasiento) + 1 AS orden FROM asiento WHERE EXTRACT(YEAR FROM fecha) = '" + fecha.right(4) + "'";
        cursor2 *cur = m_companyact->cargacursor(query);
        if (!cur->eof())
            ordenasiento = cur->valor("orden");
        delete cur;
        query = "INSERT INTO asiento (fecha, ordenasiento) VALUES ('" + m_companyact->sanearCadena(fecha) + "', " + ordenasiento + ")";
        m_companyact->ejecuta(query);
        query = "SELECT MAX(idasiento) AS id FROM asiento";
        cur = m_companyact->cargacursor(query);
        if (!cur->eof())
            idasiento = cur->valor("id");
        delete cur;
        /// FIN TRATAMIENTO DE BASE DE DATOS.
        cargaasientos();
        muestraasiento(idasiento.toInt());
        abreAsiento1();

        _depura("END Asiento1View::iniciar_asiento_nuevo", 0);
        return;
    } catch (...) {
        mensajeInfo("Asiento no pudo crearse");
    } // end try
}


void Asiento1View::on_mui_fecha_returnPressed() {
                                          _depura("Asiento1View::on_mui_fecha_returnPressed", 0);
                                          /// Cambiar la fecha del asiento.
                                          if (estadoAsiento1() != Asiento1::ASCerrado) {
                                              setDBvalue("fecha", mui_fecha->text());
                                              guardar();
                                          } else {
                                              iniciar_asiento_nuevo();
                                          } // end if
                                          _depura("END Asiento1View::on_mui_fecha_returnPressed", 0);
                                      }


                                      /// Se ha pulsado sobre el bot&oacute;n de duplicar asiento.
                                      /** Inicializa el di&aacute;logo de duplicaci&oacute;n de asientos y lo presenta.
                                          Cuando se ha terminado carga el cursor de presentaci&oacute;n y repinta el asiento
                                          para que actualize los cambios. */
                                      void Asiento1View::on_mui_duplicar_clicked() {
                                          _depura("Asiento1View::on_mui_duplicar_clicked", 0);
                                          duplicarasientoview *dupli = new duplicarasientoview(m_companyact, 0, "", true);
                                          /// Establecemos los par&aacute;metros para el nuevo asiento a duplicar.
                                          dupli->inicializa(m_ordenasiento->text(), m_ordenasiento->text());
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
                                          if (estadoasiento() != Asiento1::ASCerrado) {
                                              /// El asiento esta abierto y por tanto se muestra como abierto.
                                              asientoabiertop();
                                              numasiento = idasiento().toInt();
                                          } else {
                                              numasiento = 0;
                                          } // end if
                                          aplinteligentesview *nueva = new aplinteligentesview(m_companyact, 0);
                                          nueva->inicializa(numasiento);
                                          m_companyact->pWorkspace()->addWindow(nueva);
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
                                          QString query = "SELECT idasiento FROM asiento WHERE ordenasiento = " + m_ordenasiento->text() + " ORDER BY ordenasiento DESC";
                                          cursor2 *curs = m_companyact->cargacursor(query);
                                          if (!curs->eof()) {
                                              idas = curs->valor("idasiento");
                                              cargar(idas);
                                          } else {
                                              _depura("Asiento inexistente", 2);
                                              pintaAsiento1();
                                          } // end if
                                          delete curs;
                                          _depura("END Asiento1View::boton_cargarasiento", 0);
                                      }


                                      /// Prepara para guardar.
                                      void Asiento1View::prepguardar() {
                                          _depura("Asiento1View::prepguardar", 0);
                                          setDBvalue("fecha", mui_fecha->text());
                                          setDBvalue("ordenasiento", m_ordenasiento->text());
                                          setDBvalue("comentariosasiento", mui_comentariosAsiento->text());
                                          setDBvalue("clase", QString::number(mui_claseAsiento->currentIndex()));
                                          _depura("END Asiento1View::prepguardar", 0);
                                      }


                                      void Asiento1View::on_mui_borrar_clicked() {
                                          _depura("Asiento1View::on_mui_borrar_clicked", 0);
                                          QString idasiento = idasientosiguiente();
                                          borraAsiento1();
                                          cargaasientos();
                                          if (idasiento != "")
                                              muestraasiento(idasiento);
                                          else {
                                              vaciaAsiento1();
                                              pintaAsiento1();
                                          } // end if
                                          _depura("END Asiento1View::on_mui_borrar_clicked", 0);
                                      }


                                      ListAsientos::ListAsientos(empresa *emp) {
                                          _depura("ListAsientos::ListAsientos", 0);
                                          m_companyact = emp;
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
                                          QString cantapunt ="";
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
                                          cursorasientos = m_companyact->cargacursor(query);
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
                                              pintaasiento(cursorasientos->valor("idasiento"));
                                          } // end if
                                          _depura("END ListAsientos::boton_inicio", 0);
                                      }


                                      /// Slot que responde a la pulsaci&oacute;n del bot&oacute;n de fin.
                                      /** Comprueba que existen registros en el cursor de asientos a mostrar y si existen
                                          se desplaza al &uacute;ltimo registro y lo muestra.
                                          En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura. */
                                      void ListAsientos::boton_fin() {
                                          _depura("ListAsientos::boton_fin", 0);
                                          if (cursorasientos->numregistros() != 0 ) {
                                              cursorasientos->ultimoregistro();
                                              pintaasiento(cursorasientos->valor("idasiento"));
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
                                              pintaasiento(cursorasientos->valor("idasiento"));
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
                                              pintaasiento(cursorasientos->valor("idasiento"));
                                          } // end if
                                          _depura("END ListAsientos::boton_anterior", 0);
                                      }


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

