/***************************************************************************
                          listcuentasview.cpp  -  description
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Tomeu Borrás Riera
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
#include "listcuentasview1.h"

#include <q3listview.h>
//Added by qt3to4:
#include <QPixmap>
#include <QKeyEvent>
#include <QEvent>

// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"

#include "funcaux.h"
#include "cuentaview.h"
#include "empresa.h"

#include <stdlib.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <q3table.h>
#include <qstring.h>
#include <qmap.h>


listcuentasview1::listcuentasview1(empresa *emp, QWidget *parent, const char *name, bool modal) : listcuentasdlg1(parent,name, modal), pgimportfiles(emp->bdempresa()) {
    empresaactual = emp;
    modo=0;
    conexionbase= emp->bdempresa();
    ccuenta=ListView1->addColumn("codigo cuenta",-1);
    cdesccuenta=ListView1->addColumn("nombre cuenta",-1);
    cdebe = ListView1->addColumn(tr("debe"), -1);
    chaber = ListView1->addColumn(tr("haber"), -1);
    cidcuenta = ListView1->addColumn("id cuenta",0);
    cbloqueada = ListView1->addColumn("bloqueada",0);
    cnodebe = ListView1->addColumn("nodebe",0);
    cnohaber = ListView1->addColumn("nohaber",0);
    cregularizacion = ListView1->addColumn("regularizacion",0);
    cimputacion = ListView1->addColumn("imputacion",0);
    cgrupo = ListView1->addColumn("grupo",0);
    ctipocuenta = ListView1->addColumn("tipo cuenta",0);
    tablacuentas->setNumCols(3);
    tablacuentas->horizontalHeader()->setLabel( 0, tr( "CODIGO" ) );
    tablacuentas->horizontalHeader()->setLabel( 1, tr( "NOMBRE" ) );

    tablacuentas->hideColumn(2);
    tablacuentas->setColumnWidth(1,400);
    tablacuentas->setColumnWidth(0,100);

    installEventFilter(this);
}// end listcuentasview




listcuentasview1::~listcuentasview1() {}// end ~listcuentasview


bool listcuentasview1::eventFilter( QObject *obj, QEvent *event ) {
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        int key = keyEvent->key();
        if (key == Qt::Key_Up) { // El enter
            ListView1->eventFilter(ListView1, event);
            Q3ListViewItem *it = ListView1->currentItem()->itemAbove();
            ListView1->setCurrentItem(it);
            ListView1->ensureItemVisible(it);
            return TRUE;
        }// end if
        if (key == Qt::Key_Down) { // El enter
            Q3ListViewItem *it = ListView1->currentItem()->itemBelow();
            ListView1->setCurrentItem(it);
            ListView1->ensureItemVisible(it);
            return TRUE;
        }// end if
    }// end if
    return listcuentasdlg1::eventFilter(obj, event);
}// end eventFilter


/**
 * Se encarga de hacer las inicializaciones de todo el formulario.
 * Se llama asi y no desde el constructor pq asi la podemos llamar desde dentro
 * de la misma clase, etc etc etc
 */
int listcuentasview1::inicializa( ) {
    Q3ListViewItem * it;
    QMap <int, Q3ListViewItem*> Lista1;
    int idcuenta;
    int padre;
    int idcuenta1;
    ListView1->clear();

    /// Cargamos y pintamos las cuentas hijas.
	/// OJO AQUI, Hay que crear la restricción pertinente en la base de datos para hacer el campo padre NOT NULL y además hay que borrar este update que hace perder eficiencia.
    conexionbase->ejecuta("UPDATE cuenta SET padre=0 WHERE padre IS NULL");
    cursor2 *ctas = conexionbase->cargacursor("SELECT * FROM cuenta ORDER BY padre");
    if (ctas->eof()) {
        fprintf(stderr,"El query está vacia\n");
    } else {
        fprintf(stderr,"El query tiene registros \n");
    }// end if
    while (!ctas->eof()) {
        fprintf(stderr,"Cuentas de subnivel:%d\n",padre);
        padre = ctas->valor("padre").toInt();
        idcuenta1 = ctas->valor("idcuenta").toInt();
        if (padre != 0) {
            it = new Q3ListViewItem(Lista1[padre]);
            Lista1[idcuenta1]=it;
        } else {
            it =new Q3ListViewItem(ListView1);
            Lista1[idcuenta1]=it;
        }// end if
        it->setText(ccuenta,ctas->valor("codigo"));
        it->setText(cdesccuenta, ctas->valor("descripcion"));
        idcuenta = atoi(ctas->valor("idcuenta").ascii());
        it->setText(cidcuenta,ctas->valor("idcuenta"));
        it->setText(cbloqueada,ctas->valor("bloqueada"));
        it->setText(cnodebe,ctas->valor("nodebe"));
        it->setText(cnohaber,ctas->valor("nohaber"));
        it->setText(cregularizacion,ctas->valor("regularizacion"));
        it->setText(cimputacion,ctas->valor("imputacion"));
        it->setText(cgrupo,ctas->valor("idgrupo"));
        it->setText(cdebe,ctas->valor("debe"));
        it->setText(chaber,ctas->valor("haber"));
        /// Ponemos los iconos.
        if (ctas->valor("tipocuenta") == "1")
            it->setPixmap(ccuenta, QPixmap(cactivo));
        else if (ctas->valor("tipocuenta") == "2")
            it->setPixmap(ccuenta, QPixmap(cpasivo));
        else if (ctas->valor("tipocuenta") == "3")
            it->setPixmap(ccuenta, QPixmap(cneto));
        else if (ctas->valor("tipocuenta") == "4")
            it->setPixmap(ccuenta, QPixmap(cingresos));
        else if (ctas->valor("tipocuenta") == "5")
            it->setPixmap(ccuenta, QPixmap(cgastos));
        it->setOpen(true);
        ctas->siguienteregistro();
    }// end while
    delete ctas;

    /// Cargamos el número de digitos de cuenta para poder hacer una introducción de números de cuenta más práctica.
    numdigitos = empresaactual->numdigitosempresa();

    inicializatabla();
    return(0);
}// end inicializa



/** Inicializa la tabla de cuentas, que es la segunda pestaña de la pantalla
  * Dicha tabla sólo muestra las cuentas hoja del plan contable, y deja los demás elementos
  * ocultos.
  */
void listcuentasview1::inicializatabla()  {
    QString query;
    query = "SELECT * FROM cuenta ORDER BY padre";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query);
    tablacuentas->setNumRows(cursoraux1->numregistros());
    int i=0;
    while (!cursoraux1->eof()) {
        tablacuentas->setText(i,0,cursoraux1->valor("codigo"));
        tablacuentas->setText(i,1,cursoraux1->valor("descripcion"));
        tablacuentas->setText(i,2,cursoraux1->valor("idcuenta"));


        /// Ponemos los iconos.
        if (cursoraux1->valor("tipocuenta") == "1")
            tablacuentas->setPixmap(i,0, QPixmap(cactivo));
        else if (cursoraux1->valor("tipocuenta") == "2")
            tablacuentas->setPixmap(i,0, QPixmap(cpasivo));
        else if (cursoraux1->valor("tipocuenta") == "3")
            tablacuentas->setPixmap(i,0, QPixmap(cneto));
        else if (cursoraux1->valor("tipocuenta") == "4")
            tablacuentas->setPixmap(i,0, QPixmap(cingresos));
        else if (cursoraux1->valor("tipocuenta") == "5")
            tablacuentas->setPixmap(i,0, QPixmap(cgastos));

        QString codigo = cursoraux1->valor("codigo");
        if ((unsigned int)codigo.length() != numdigitos) {
            tablacuentas->hideRow(i);
        }// end if
        cursoraux1->siguienteregistro ();
        i++;
    }// end while
    delete cursoraux1;
    tablacuentas->setReadOnly(TRUE);
}// end inicializatabla



/**
 * Este es el slot que se activa al hacer click sobre el
 * ListView del formulario. Lo que hace es actualizar los valores
 * de la derecha del formulario con los valores almacenados en el
 * item que se acaba de pulsar.
 */
void listcuentasview1::listpulsada(Q3ListViewItem *it) {
    QString idcuenta = it->text(cidcuenta);
    QString cad;
    int i;
    for (i=0;i< tablacuentas->numRows();i++) {
        cad= tablacuentas->text(i,2);
        if (cad == idcuenta) {
            fprintf(stderr,"Lo he encontrado\n");
            tablacuentas->setCurrentCell(i,2);
            tablacuentas->ensureCellVisible(i,2);
        }// end if
    }// end for
}// end listpulsada


/** La pantalla lleva implicito un buscador de cuentas, cuando cambia el contenido
  * del QLineEdit del buscador se lanza esta función que hace una bsqueda sobre el árbol
  * de cuentas
  */
void listcuentasview1::descripcioncambiada(const QString &string1) {
    Q3ListViewItem *it;
    QString cod = extiendecodigo(string1, (int) numdigitos);

    it = ListView1->findItem(cod, ccuenta, Q3ListView::BeginsWith);
    if (it != 0) {
        ListView1->setCurrentItem(it);
        ListView1->ensureItemVisible(it);
    } else {
        it = ListView1->findItem(string1, cdesccuenta, Q3ListView::BeginsWith);
        if (it != 0) {
            ListView1->setCurrentItem(it);
            ListView1->ensureItemVisible(it);
        }// end if
    }// end if
}// end descripcioncambiada


/**
  * Este es el slot que se activa al hacer doble click sobre el
  * ListView del formulario. Lo que hace es abrir la ventana de
  * detalle de cuenta para que se puedan modificar los parametros
  * y una vez terminado refresca el formulario para que aparezcan
  * los datos actualizados.
  */
void listcuentasview1::listdblpulsada(Q3ListViewItem *it) {
    listpulsada(it);
    mdb_codcuenta = it->text(ccuenta);
    mdb_idcuenta = it->text(cidcuenta);
    mdb_desccuenta = it->text(cdesccuenta);
    if (modo == 0) {

        cuentaview *nuevae = new cuentaview(empresaactual,0,"",true);
        nuevae->cargacuenta(atoi(idcuenta().ascii()));
        nuevae->exec();
        inicializa();
        delete nuevae;
        // Para no perder el foco del elemento, al mismo tiempo que se
        // actualizan los cambios luego buscamos y enfocamos el item
        it = ListView1->findItem(mdb_idcuenta, cidcuenta, Q3ListView::ExactMatch);
        ListView1->setCurrentItem(it);
        ListView1->ensureItemVisible(it);
    } else {
        close();
    }// end if
}// end listdblpulsada


/**
  * Esta funcion es el slot que se activa al pulsar sobre el
  * boton nueva cuenta.
  * Su función es crear una nueva cuenta desde la ventana del plan de cuentas
  * La inserción de la nueva se hace como cuenta hija de la cuenta actualmente
  * seleccionada por lo que se hace que la ventana que se habre tenga el campo
  * del padre de la cuenta rellenado.
  */
void listcuentasview1::nuevacuenta()  {
    QString cadena, codigo;
    int idcuenta, idgrupo=0;
    Q3ListViewItem *it;
    fprintf(stderr,"listcuentasview1::nuevacuenta\n");
    cuentaview *nuevae = new cuentaview(empresaactual,0,0,true);
    it = ListView1->currentItem();
    codigo = it->text(ccuenta);
    cadena = it->text(cgrupo);
    idgrupo = cadena.toInt();
    nuevae->nuevacuenta(codigo,idgrupo);
    fprintf(stderr,"Llamamos a cuentaview\n");
    nuevae->exec();
    fprintf(stderr,"Ya volvemos de cuentaview\n");
    inicializa();
    idcuenta = nuevae->idcuenta;
    cadena.setNum(idcuenta);
    // Para no perder el foco del elemento, al mismo tiempo que se
    // actualizan los cambios luego buscamos y enfocamos el item
    it = ListView1->findItem(cadena, cidcuenta, Q3ListView::ExactMatch);
    ListView1->setCurrentItem(it);
    ListView1->ensureItemVisible(it);
    delete nuevae;
}// end nuevacuenta


/**
 * Esta función es el slot que se activa al pulsar sobre el
 * boton nueva cuenta.
 * Su función es crear una nueva cuenta desde la ventana del plan de cuentas
 * La inserción de la nueva se hace como cuenta hija de la cuenta actualmente
 * seleccionada por lo que se hace que la ventana que se habre tenga el campo
 * del padre de la cuenta rellenado.
 */
void listcuentasview1::editarcuenta()  {

    Q3ListViewItem *it;
    it = ListView1->currentItem();
    listpulsada(it);
    mdb_codcuenta = it->text(ccuenta);
    mdb_idcuenta = it->text(cidcuenta);
    mdb_desccuenta = it->text(cdesccuenta);
    cuentaview *nuevae = new cuentaview(empresaactual,0,"",true);
    nuevae->cargacuenta(atoi(idcuenta().ascii()));
    nuevae->exec();
    inicializa();
    // Para no perder el foco del elemento, al mismo tiempo que se
    // actualizan los cambios luego buscamos y enfocamos el item
    it = ListView1->findItem(mdb_idcuenta, cidcuenta, Q3ListView::ExactMatch);
    ListView1->setCurrentItem(it);
    ListView1->ensureItemVisible(it);
    delete nuevae;
}// end editacuenta



/**
  * Esta funcion es el slot que se activa al pulsar sobre el
  * boton borrar cuenta.
  */
void listcuentasview1::borrarcuenta()  {
    Q3ListViewItem *it;
    int valor = QMessageBox::warning( 0, "Borrar Cuenta", "Se procedera a borrar la cuenta.", QMessageBox::Yes, QMessageBox::No);
    if (valor ==  QMessageBox::Yes) {
        it = ListView1->currentItem();
        int idcuenta =atoi((char *) it->text(cidcuenta).ascii());
        conexionbase->begin();
        if (conexionbase->borrarcuenta(idcuenta) == 0) {
            Q3ListViewItem *ot = it->itemAbove();
            if (ot)
                idcuenta =atoi((char *) ot->text(cidcuenta).ascii());
            inicializa();
            QString cadena;
            cadena.sprintf("%d",idcuenta);
            it = ListView1->findItem(cadena, cidcuenta, Q3ListView::ExactMatch);
            ListView1->setCurrentItem(it);
            ListView1->ensureItemVisible(it);
        } else {
            qWarning( "No se ha podido borrar la cuenta" );
        }// end if
        conexionbase->commit();
    }// end if
}// end borrarcuenta


/** \brief Se ha hecho una doble click sobre la tabla de cuentas
  *
  * Al hacer doble click sobre la tabla de cuentas, se encuentra el elemento análogo
  * en el arbol contable y se simula una doble pulsación sobre ese elemento.
  */
void listcuentasview1::dbtabla(int row, int , int ,const QPoint &) {
    string idcuenta = tablacuentas->text(row,2).ascii();
    Q3ListViewItem *it = ListView1->findItem(idcuenta.c_str(), cidcuenta, Q3ListView::ExactMatch);
    ListView1->setCurrentItem(it);
    ListView1->ensureItemVisible(it);
    listdblpulsada(it);
}// end dbtabla


/** \brief Cuando se pulsa el Return sobre la bsqueda de cuentas
  * 
  * Actua como si fuese una doble pulsación con el ratón sobre la tabla de cuentas.
  */
void listcuentasview1::eturn_descripcion() {
    Q3ListViewItem *it = ListView1->currentItem();
    if (it != 0) {
        listdblpulsada(it);
    }// end if
}// end return_codigo


/** \brief Responde a la pulsacion del boton de imprimir en la ventana de cuentas.
  * Crea un string de llamada a rtkview y lo lanza como llamada de sistema.
  * \todo La plantilla podria tener contenidos dinamicos mendiante marcas sustituibles por
  * un egrep, o un sedit que aun no estan realizados.
  * \todo Esta funcion deberia implementarse con una clase nueva de Qt que solicitase 
  * el rango de cuentas entre el que se quiere el listado.
  */
void listcuentasview1::s_PrintCuentas() {
    QString cadena;
    cadena = "rtkview --input-sql-driver QPSQL7 --input-sql-database ";
    cadena += conexionbase->nameDB()+" ";
    cadena += confpr->valor(CONF_DIR_REPORTS)+"cuentas.rtk &";
    fprintf(stderr,"%s\n",cadena.ascii());
    system (cadena.ascii());
}// end s_PrintCuentas


void listcuentasview1::s_exportar() {
    QFile filexml (Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Plan Contable (*.xml)", this, "select file", "Elija el Archivo"));
    if(filexml.open(QIODevice::WriteOnly)) {
        bulmages2XML(filexml, IMPORT_CUENTAS);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR ARCHIVO\n");
    }// end if
}

void listcuentasview1::s_importar() {
    QFile filexml (Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER),"Plan Contable (*.xml)", this, "select file", "Elija el Archivo"));
    if (filexml.open(QIODevice::ReadOnly)) {
        XML2Bulmages(filexml, IMPORT_CUENTAS);
        filexml.close();
        inicializa();
    } else {
        _depura("ERROR AL ABRIR ARCHIVO\n");
    }// end if
}


