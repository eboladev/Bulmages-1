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

// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"

#include "cuentaview.h"

listcuentasview1::listcuentasview1(QWidget *parent, const char *name, bool modal) : listcuentasdlg1(parent,name, modal) {
    modo=0;
    conexionbase= NULL;
}// end listcuentasview


listcuentasview1::~listcuentasview1() {}// end ~listcuentasview


/*************************************************************
 * Esta funcion se encarga de hacer las inicializaciones de todo el formulario.
 * Se llama asi y no desde el constructor pq asi la podemos llamar desde dentro
 * de la misma clase, etc etc etc
 *************************************************************/
int listcuentasview1::inicializa(postgresiface2 *conn ) {
    QListViewItem * it;
    QListViewItem *Lista[10000];
    int idcuenta;
    int padre;
    int idcuenta1;
    cursor2 *cursoraux1, *cursoraux2;

    ListView1->clear();
    if (conexionbase == NULL) {
        ccuenta=ListView1->addColumn("código cuenta",-1);
        cdesccuenta=ListView1->addColumn("nombre cuenta",-1);
        cidcuenta = ListView1->addColumn("id cuenta",0);
        cbloqueada = ListView1->addColumn("bloqueada",0);
        cnodebe = ListView1->addColumn("nodebe",0);
        cnohaber = ListView1->addColumn("nohaber",0);
        cregularizacion = ListView1->addColumn("regularizacion",0);
        cimputacion = ListView1->addColumn("imputacion",0);
        cgrupo = ListView1->addColumn("grupo",0);
        ctipocuenta = ListView1->addColumn("tipo cuenta",0);
    }// end if
    conexionbase = conn;
    conexionbase->begin();
    cursoraux1=conexionbase->cargacuentas(0);
	 conexionbase->commit();
    //   cursoraux1->ultimoregistro();

    while (!cursoraux1->eof()) {
        padre = atoi( cursoraux1->valor("padre").ascii());
        idcuenta1 = atoi( cursoraux1->valor("idcuenta").ascii());
        it =new QListViewItem(ListView1);
        Lista[idcuenta1]=it;
        it->setText(ccuenta, cursoraux1->valor("codigo"));
        it->setText(cdesccuenta,cursoraux1->valor("descripcion"));
        idcuenta = atoi(cursoraux1->valor("idcuenta").ascii());
        it->setText(cidcuenta,cursoraux1->valor("idcuenta"));
        it->setText(cbloqueada,cursoraux1->valor(5));
        it->setText(cnodebe,cursoraux1->valor(10));
        it->setText(cnohaber,cursoraux1->valor(11));
        it->setText(cregularizacion,cursoraux1->valor(12));
        it->setText(cimputacion,cursoraux1->valor(3));
        it->setText(cgrupo,cursoraux1->valor("idgrupo"));

        // Ponemos los iconos para que la cosa parezca mas guay.
        if (cursoraux1->valor("tipocuenta") == "1")
            it->setPixmap(ccuenta, QPixmap(cactivo));
        else if (cursoraux1->valor("tipocuenta") == "2")
            it->setPixmap(ccuenta, QPixmap(cpasivo));
        else if (cursoraux1->valor("tipocuenta") == "3")
            it->setPixmap(ccuenta, QPixmap(cneto));
        else if (cursoraux1->valor("tipocuenta") == "4")
            it->setPixmap(ccuenta, QPixmap(cingresos));
        else if (cursoraux1->valor("tipocuenta") == "5")
            it->setPixmap(ccuenta, QPixmap(cgastos));


        it->setText(ctipocuenta, cursoraux1->valor("tipocuenta"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    }// end while

	 conexionbase->begin();
    cursoraux2=conexionbase->cargacuentas(-2);
	 conexionbase->commit();
    //   cursoraux1->ultimoregistro();
    while (!cursoraux2->eof()) {
        padre = atoi(cursoraux2->valor(4).ascii());
        idcuenta1 = atoi(cursoraux2->valor(0).ascii());
		  fprintf(stderr,"Cuentas de subnivel:%d",padre);
        if (padre != 0) {
            it = new QListViewItem(Lista[padre]);
            Lista[idcuenta1]=it;

            it->setText(ccuenta,cursoraux2->valor("codigo"));
            it->setText(cdesccuenta, cursoraux2->valor("descripcion"));
            idcuenta = atoi(cursoraux2->valor("idcuenta").ascii());
            it->setText(cidcuenta,cursoraux2->valor("idcuenta"));
            it->setText(cbloqueada,cursoraux2->valor(5));
            it->setText(cnodebe,cursoraux2->valor(10));
            it->setText(cnohaber,cursoraux2->valor(11));
            it->setText(cregularizacion,cursoraux2->valor(12));
            it->setText(cimputacion,cursoraux2->valor(3));
            it->setText(cgrupo,cursoraux2->valor("idgrupo"));

            // Ponemos los iconos para que la cosa parezca mas guay.
            if (cursoraux2->valor("tipocuenta") == "1")
                it->setPixmap(ccuenta, QPixmap(cactivo));
            else if (cursoraux2->valor("tipocuenta") == "2")
                it->setPixmap(ccuenta, QPixmap(cpasivo));
            else if (cursoraux2->valor("tipocuenta") == "3")
                it->setPixmap(ccuenta, QPixmap(cneto));
            else if (cursoraux2->valor("tipocuenta") == "4")
                it->setPixmap(ccuenta, QPixmap(cingresos));
            else if (cursoraux2->valor("tipocuenta") == "5")
                it->setPixmap(ccuenta, QPixmap(cgastos));

            it->setOpen(true);
        }// end if
        cursoraux2->siguienteregistro();
    }// end while
    delete cursoraux2;
    delete cursoraux1;
    // Vamos a cargar el número de digitos de cuenta para poder hacer una introduccion de numeros de cuenta mas practica.
    QString query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
    conexionbase->begin();
    cursoraux1 = conexionbase->cargacursor(query,"codcuenta");
    conexionbase->commit();
    numdigitos=cursoraux1->valor(2).length();
    delete cursoraux1;
    fprintf(stderr,"las cuentas tienen %d digitos\n",numdigitos);
    inicializatabla();
    return(0);
}// end inicializa



void listcuentasview1::inicializatabla()  {
    //  tablacuentas->clear();
    tablacuentas->setNumCols(3);
    tablacuentas->horizontalHeader()->setLabel( 0, tr( "CODIGO" ) );
    tablacuentas->horizontalHeader()->setLabel( 1, tr( "NOMBRE" ) );

    tablacuentas->hideColumn(2);
    tablacuentas->setColumnWidth(1,400);
    tablacuentas->setColumnWidth(0,100);

    conexionbase->begin();
    cursor2 *cursoraux1=conexionbase->cargacuentas(-1);
    conexionbase->commit();
    tablacuentas->setNumRows(cursoraux1->numregistros());
    int i=0;
    while (!cursoraux1->eof()) {
        tablacuentas->setText(i,0,cursoraux1->valor("codigo"));
        tablacuentas->setText(i,1,cursoraux1->valor("descripcion"));
        tablacuentas->setText(i,2,cursoraux1->valor(0));


        // Ponemos los iconos para que la cosa parezca mas guay.
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

        QString codigo = cursoraux1->valor(1);
        if (codigo.length() != numdigitos) {
            tablacuentas->hideRow(i);
        }// end if
        cursoraux1->siguienteregistro ();
        i++;
    }// end while
    delete cursoraux1;
    tablacuentas->setReadOnly(TRUE);
}// end inicializatabla


void listcuentasview1::arbolcuentas(QListViewItem *itempadre, int padre ) {
    QListViewItem * it;
    cursor2 *cursoraux;
    QString cadena;
    QString cadena1;
    int num, idcuenta;
	 conexionbase->begin();
    cursoraux = conexionbase->cargacuentas(padre);
	 conexionbase->commit();
    num = cursoraux->numregistros();
    while (!cursoraux->eof()) {
        it =new QListViewItem(itempadre);
        it->setText(cdesccuenta,cursoraux->valor(2));
        idcuenta = atoi(cursoraux->valor(1).ascii());
        it->setText(ccuenta,cursoraux->valor(1));
        it->setText(cidcuenta,cursoraux->valor(0).ascii());
        it->setText(cbloqueada,cursoraux->valor(5));
        it->setText(cnodebe,cursoraux->valor(10));
        it->setText(cnohaber,cursoraux->valor(11));
        it->setText(cregularizacion,cursoraux->valor(12));
        it->setText(cimputacion,cursoraux->valor(3));
        cursoraux->siguienteregistro();
    }// end while
    delete  cursoraux;
}// end arbolcuentas



/***************************************************************
 * Este es el slot que se activa al hacer click sobre el
 * ListView del formulario. Lo que hace es actualizar los valores
 * de la derecha del formulario con los valores almacenados en el
 * item que se acaba de pulsar.
 ****************************************************************/
// OJO ahora no es de pulsacion, sino que es de cambio.
void listcuentasview1::listpulsada(QListViewItem *it) {
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


void listcuentasview1::descripcioncambiada(const QString &string1) {
    QListViewItem *it;
    it = ListView1->findItem(string1, cdesccuenta, Qt::BeginsWith);
    if (it != 0) {
        ListView1->setCurrentItem(it);
        ListView1->ensureItemVisible(it);
    }// end if
}// end descripcioncambiada


void listcuentasview1::codigocambiado(const QString &string1) {
    QListViewItem *it;
    // Con este trozo de código lo que hacemos es sustituir los puntos
    // por ceros ampliando hasta el número de dígitos requeridos.
    // De esta forma podemos hacer búsquedas del tipo 1.3 para buscar 1000003
   QString cod = extiendecodigo(string1, numdigitos);

    /*if (cod.length() < numdigitos) {
      string str7 (numdigitos-cod.length()+1,'0');
      int pos = cod.find(".",0);
      if (pos > 0) {
        cod.replace(pos,1,str7);
      }// end if
    }// end if
    */
    it = ListView1->findItem(cod, ccuenta, Qt::BeginsWith);
    if (it != 0) {
        ListView1->setCurrentItem(it);
        ListView1->ensureItemVisible(it);
    }// end if
}// end descripcioncambiada

/***************************************************************
 * Este es el slot que se activa al hacer doble click sobre el
 * ListView del formulario. Lo que hace es abrir la ventana de
 * detalle de cuenta para que se puedan modificar los parametros
 * y una vez terminado refresca el formulario para que aparezcan
 * los datos actualizados.
 ****************************************************************/
void listcuentasview1::listdblpulsada(QListViewItem *it) {
    listpulsada(it);
    codcuenta = it->text(ccuenta);
    idcuenta = it->text(cidcuenta);
    if (modo == 0) {
	 
        cuentaview *nuevae = new cuentaview(0,"",true);
        nuevae->inicializa(conexionbase);
        nuevae->cargacuenta(atoi(idcuenta.ascii()));
        nuevae->exec();
        inicializa(conexionbase);
        delete nuevae;
        // Para no perder el foco del elemento, al mismo tiempo que se
        // actualizan los cambios luego buscamos y enfocamos el item
        it = ListView1->findItem(idcuenta, cidcuenta, Qt::ExactMatch);
        ListView1->setCurrentItem(it);
        ListView1->ensureItemVisible(it);
    } else {
        close();
    }// end if
}// end listdblpulsada


/********************************************************
 * Esta funcion es el slot que se activa al pulsar sobre el
 * boton nueva cuenta.
 * Su función es crear una nueva cuenta desde la ventana del plan de cuentas
 * La inserción de la nueva se hace como cuenta hija de la cuenta actualmente
 * seleccionada por lo que se hace que la ventana que se habre tenga el campo
 * del padre de la cuenta rellenado.
 *********************************************************/
void listcuentasview1::nuevacuenta()  {

    string cadena;
    string codigo;
    int idgrupo=0;
    int idcuenta;
    QListViewItem *it;
    fprintf(stderr,"listcuentasview1::nuevacuenta\n");
    cuentaview *nuevae = new cuentaview(0,0,true);
    nuevae->inicializa(conexionbase);
    it = ListView1->currentItem();
    codigo = (string) it->text(ccuenta).ascii();
    cadena = (string) it->text(cgrupo).ascii();
    idgrupo = atoi (cadena.c_str());
    nuevae->nuevacuenta((char *) codigo.c_str(),idgrupo);
    fprintf(stderr,"Llamamos a cuentaview\n");
    nuevae->exec();
    fprintf(stderr,"Ya volvemos de cuentaview\n");

    inicializa(conexionbase);
    idcuenta = nuevae->idcuenta;
    cadena= Ttos(idcuenta);
    // Para no perder el foco del elemento, al mismo tiempo que se
    // actualizan los cambios luego buscamos y enfocamos el item
    it = ListView1->findItem(cadena.c_str(), cidcuenta, Qt::ExactMatch);
    ListView1->setCurrentItem(it);
    ListView1->ensureItemVisible(it);
    delete nuevae;
	 
}// end nuevacuenta


/********************************************************
 * Esta funcion es el slot que se activa al pulsar sobre el
 * boton nueva cuenta.
 * Su función es crear una nueva cuenta desde la ventana del plan de cuentas
 * La inserción de la nueva se hace como cuenta hija de la cuenta actualmente
 * seleccionada por lo que se hace que la ventana que se habre tenga el campo
 * del padre de la cuenta rellenado.
 *********************************************************/
void listcuentasview1::editarcuenta()  {

    QListViewItem *it;
    it = ListView1->currentItem();
    listpulsada(it);
    codcuenta = it->text(ccuenta);
    idcuenta = it->text(cidcuenta);
    cuentaview *nuevae = new cuentaview(0,"",true);
    nuevae->inicializa(conexionbase);
    nuevae->cargacuenta(atoi(idcuenta.ascii()));
    nuevae->exec();
    inicializa(conexionbase);
    // Para no perder el foco del elemento, al mismo tiempo que se
    // actualizan los cambios luego buscamos y enfocamos el item
    it = ListView1->findItem(idcuenta, cidcuenta, Qt::ExactMatch);
    ListView1->setCurrentItem(it);
    ListView1->ensureItemVisible(it);
    delete nuevae;
}// end editacuenta



/********************************************************
 * Esta funcion es el slot que se activa al pulsar sobre el
 * boton borrar cuenta.
 *********************************************************/
void listcuentasview1::borrarcuenta()  {
    QListViewItem *it;
    int valor = QMessageBox::warning( 0, "Borrar Cuenta", "Se procedera a borrar la cuenta.", QMessageBox::Yes, QMessageBox::No);
    if (valor ==  QMessageBox::Yes) {
        it = ListView1->currentItem();
        int idcuenta =atoi((char *) it->text(cidcuenta).ascii());
        conexionbase->begin();
        if (conexionbase->borrarcuenta(idcuenta) == 0) {
            QListViewItem *ot = it->itemAbove();
            if (ot)
                idcuenta =atoi((char *) ot->text(cidcuenta).ascii());
            inicializa(conexionbase);
            QString cadena;;
				cadena.sprintf("%d",idcuenta);
            it = ListView1->findItem(cadena, cidcuenta, Qt::ExactMatch);
            ListView1->setCurrentItem(it);
            ListView1->ensureItemVisible(it);
        } else {
            qWarning( "No se ha podido borrar la cuenta" );
        }// end if
        conexionbase->commit();
    }// end if
}// end borrarcuenta


void listcuentasview1::dbtabla(int row, int colummn, int button,const QPoint &mouse) {
    fprintf(stderr,"Se ha hecho doble click sobre la tabla\n");
    string idcuenta = tablacuentas->text(row,2).ascii();
    QListViewItem *it = ListView1->findItem(idcuenta.c_str(), cidcuenta, Qt::ExactMatch);
    ListView1->setCurrentItem(it);
    ListView1->ensureItemVisible(it);
    listdblpulsada(it);
    
    //PAra quitar el warning
    colummn=button=0;
    mouse.isNull();
}// end dbtabla

void listcuentasview1::return_codigo() {
                                 QListViewItem *it = ListView1->currentItem();
                                 if (it != 0) {
                                     listdblpulsada(it);
                                 }// end if
                             }// end return_codigo


                             void listcuentasview1::return_descripcion() {
                                                              QListViewItem *it = ListView1->currentItem();
                                                              if (it != 0) {
                                                                  listdblpulsada(it);
                                                              }// end if
                                                          }// end return_codigo
