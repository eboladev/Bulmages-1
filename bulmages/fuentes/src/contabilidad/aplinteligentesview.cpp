/***************************************************************************
                          aplinteligentesview.cpp  -  description
                             -------------------
    begin                : Mon Feb 10 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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
#include "aplinteligentesview.h"
#include "images/find.xpm"
#include "calendario.h"

#define TIPO_CTA 0
#define TIPO_FECHA 1
#define TIPO_TEXTO 2
#define TIPO_NUMERO 3
#define TIPO_PREDEFINIDO 4


aplinteligentesview::aplinteligentesview(QWidget *parent, const char *name ) : aplinteligentesdlg(parent,name) {
    // iniciamos los contadores de variables para que no haya problemas.
    indvariablescta=0;
    indvariablesfecha=0;
    indvariablesnumero=0;
    indvariablestexto=0;
    indvariablespredefinidas=0;
    indvariablesapunte=0;
}// end aplinteligentesview


aplinteligentesview::~aplinteligentesview() {
    borrawidgets();
}// end aplinteligentesview


void aplinteligentesview::inicializa(postgresiface2 *con, int idasiento, intapunts3view *inta) {
    intapunts = inta;
    int i=0;
    numasiento = idasiento;
    conexionbase = con;
    inicializavariables();
    QString query;
    query.sprintf("SELECT * FROM ainteligente");
    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(query,"unsuperquery");
    conexionbase->commit();
    while (!cur->eof()) {
        fprintf(stderr,"%2.2s %s\n",cur->valor("idainteligente").latin1(), cur->valor("descripcion").latin1());
        comboainteligentes->insertItem(cur->valor("descripcion").latin1(),-1);
        listasientos[i++]= atoi(cur->valor("idainteligente").latin1());
        cur->siguienteregistro();
    }// end while
    delete cur;
    // Calculamos el número de dígitos que tiene una cuenta.
    conexionbase->begin();
    QString query1 = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query1,"codcuenta");
    numdigitos=cursoraux1->valor(2).length();
    conexionbase->commit();
    delete cursoraux1;
    cambiada_plantilla(0);
}// end inicializa


void aplinteligentesview::inicializavariables() {
    QString subcadena;
    QString buffer;
    QDate fecha;
    fecha = QDate::currentDate();
    buffer.sprintf("%d/%d/%d",fecha.day(),fecha.month(),fecha.year());
    subcadena=buffer;
    variablespredefinidas[indvariablespredefinidas][0]="$fechaactual$";
    variablespredefinidas[indvariablespredefinidas++][1]=subcadena;
    buffer.sprintf("SELECT * FROM asiento WHERE idasiento=%d",numasiento);
    fprintf(stderr,"%s\n",buffer.latin1());
    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(buffer,"cargaasiento");
    conexionbase->commit();
    if (!cur->eof()) {
        variablespredefinidas[indvariablespredefinidas][0]="$fechaasiento$";
        variablespredefinidas[indvariablespredefinidas++][1]=cur->valor("fecha");
    }// end if
    delete cur;
    inicializavariablesapunte(0);
}// end inicializavariables

// Existen variables de asientos inteligentes que cambian con la inserción
// de cada apunte.
void aplinteligentesview::inicializavariablesapunte(int idborrador) {
    fprintf(stderr,"aplicavariablesapunte %d\n",idborrador);
    variablesapunte[0][0] ="$cifcuenta$";
    QString query;
    query.sprintf("SELECT * FROM borrador WHERE idborrador=%d",idborrador);
    fprintf(stderr,"%s\n",query.latin1());
    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(query,"cursor");
    conexionbase->commit();
    if (!cur->eof()) {
        query.sprintf("SELECT * FROM cuenta WHERE idcuenta=%s",cur->valor("idcuenta").latin1());
        fprintf(stderr,"%s\n", query.latin1());
        conexionbase->begin();
        cursor2 *cur1 = conexionbase->cargacursor(query,"cursor2");
        conexionbase->commit();
        variablesapunte[0][0] ="$cifcuenta$";
        variablesapunte[0][1] = cur1->valor("cifent_cuenta");
        fprintf(stderr,"variable de APUNTE %s\n",cur1->valor("cifent_cuenta").latin1());
        delete cur1;
    }// end if
    delete cur;
    indvariablesapunte = 1;
    fprintf(stderr," Fin de aplicavariablesapunte\n");
}// end inicializavariablesapunte


// Esta función carga la variable de uso temporal $cifcuenta$
// con el valor del nif de la cuenta introducida.
// NOTA: Notese que el sistema normalmente carga el NIF de la contrapartida del asiento inteligente
// Y que este campo normalmente va en la parte de CIFCUENTA
void aplinteligentesview::cifcuenta(int idcuenta) {
    variablesapunte[0][0] ="$cifcuenta$";
    QString query;
    query.sprintf("SELECT * FROM cuenta WHERE idcuenta=%d", idcuenta);
    conexionbase->begin();
    cursor2 * cur = conexionbase->cargacursor(query,"cursor");
    conexionbase->commit();
    if (!cur->eof()) {
        variablesapunte[0][1] = cur->valor("cifent_cuenta");
    } else {
        variablesapunte[0][1] = "";
    }// end if
    delete cur;
    indvariablesapunte=1;
}// end cifcuenta

void aplinteligentesview::return_cta() {
    QLineEdit *cuenta;
    cuenta = (QLineEdit *) sender();
    fprintf(stderr,"Se ha pulsado return sobre una cuenta\n");
    fprintf(stderr,"Texto de la cuenta: %s\n",cuenta->text().latin1());

	 // Buscamos el label correspondiente para esta cuenta.
	 int i;
	 for (i=0; i<indvariablescta && varcta[i]!=cuenta;i++);
	 
    QString cad = cuenta->text();
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        conexionbase->begin();
        cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
        conexionbase->commit();
        int num = cursorcta->numregistros();
        if (num >0) {
            cuenta->setText(cursorcta->valor("codigo"));
				nomcta[i]->setText(cursorcta->valor("descripcion"));
            selectsiguiente(cuenta);
        }// end if
        delete cursorcta;
    }// end if
}// end return_cta


void aplinteligentesview::return_numero() {
    QLineEdit *numero;
    numero = (QLineEdit *) sender();
    fprintf (stderr,"Se ha pulsado return sobre el número: %s\n",numero->text().latin1());
    selectsiguiente(numero);
}// end return_numero


void aplinteligentesview::return_texto() {
    QLineEdit *texto;
    texto = (QLineEdit *) sender();
    fprintf (stderr,"Se ha pulsado return sobre el texto: %s\n",texto->text().latin1());
    selectsiguiente(texto);
}// end return_numero


void aplinteligentesview::boton_buscacuenta() {
    QToolButton *boton;
    QLineEdit *lineaeditada;
    lineaeditada = NULL;
    boton = (QToolButton *) sender();
    fprintf(stderr,"Se esta intentando buscar una cuenta\n");
    for(int i=0;i<indvariablescta;i++) {
        if (boton == boton_cta[i]) {
            lineaeditada = varcta[i];
        }// end if
    }// end for
    listcuentasview1 *listcuentas = new listcuentasview1();
    listcuentas->modo=1;
    listcuentas->inicializa(conexionbase);
    listcuentas->exec();
    if (lineaeditada != NULL)
        lineaeditada->setText(listcuentas->codcuenta);
    delete listcuentas;
}// end if


void aplinteligentesview::cambiada_plantilla(int num) {
    fprintf(stderr,"idasientointeligente %d\n",listasientos[num]);
    idainteligente = listasientos[num];
    mostrarplantilla();
}// end cambiada_plantilla


void aplinteligentesview::boton_crear() {
    // Se está insertando sobre un asiento abierto, con lo que debemos
    // Cerrar la ventana, ya que es un introduccion de asiento normal
    if (numasiento != 0) {
        recogevalores();
        creaasiento();
        intapunts->repinta();
        selectfirst();
        //	  done(1);
    } else {
        // Se está insertando de forma sistemática asientos inteligentes
        // Asi que debemos facilitar las cosas al máximo.
        intapunts->fechaasiento1->setText(fechaasiento->text());
        intapunts->return_fechaasiento();
        numasiento=atoi( intapunts->cursorasientos->valor("idasiento").latin1() );
        recogevalores();
        creaasiento();
        intapunts->boton_cerrarasiento();
        numasiento = 0;
        fechaasiento->selectAll();
        fechaasiento->setFocus();

    }// end if

}// end boton_crear


/*****************************************************************************
 * Esta funcion va a crear la plantilla necesaria para la creacion           *
 * de los asientos inteligentes. La idea es coger las variables de           *
 * la base de datos que son datos variables y presentarlos en el widget      *
 * Es el algoritmo mas bonito con el que he topado de momento en el          *
 * desarrollo de Bulmages y me hace mucha ilusion tenerlo listo pronto.      *
 *****************************************************************************/
void aplinteligentesview::mostrarplantilla() {
    QString query;
    int i;
    int j=0;
    int inc=30;
    QString cadena;
    QString subcadena;
    numainteligente = idainteligente;
    fprintf(stderr,"Mostrar plantilla %d\n",numainteligente);
    // Vamos a intentar borrar todos los datos antes de empezar.
    borrawidgets();
    fprintf(stderr,"Hemos borrado los datos anteriores\n");

    // Vamos a ver que tal se nos da el recoger variables de la plantilla apuntes
    query.sprintf("SELECT * FROM binteligente WHERE idainteligente=%d",numainteligente);
    conexionbase->begin();
    cursor2 *cur= conexionbase->cargacursor(query,"superquery");
    conexionbase->commit();
    while (!cur->eof()) {
        recogevariables(cur->valor("codcuenta"),TIPO_CTA);
        recogevariables(cur->valor("contrapartida"),TIPO_CTA);
        recogevariables(cur->valor("fecha"),TIPO_FECHA);
        recogevariables(cur->valor("debe"),TIPO_NUMERO);
        recogevariables(cur->valor("haber"), TIPO_NUMERO);
        recogevariables(cur->valor("conceptocontable"), TIPO_TEXTO);
        recogevariables(cur->valor("descripcion"), TIPO_TEXTO);
        cur->siguienteregistro();
    }// end while
    delete cur;

    // Vamos a ver que tal se nos da el recoger variables de la plantilla iva
    query.sprintf("SELECT * FROM ivainteligente WHERE idainteligente=%d",numainteligente);
    conexionbase->begin();
    cur= conexionbase->cargacursor(query,"superquery1");
    conexionbase->commit();
    while (!cur->eof()) {
        recogevariables(cur->valor("contrapartida"),TIPO_CTA);
        recogevariables(cur->valor("baseimp"),TIPO_NUMERO);
        recogevariables(cur->valor("iva"),TIPO_NUMERO);
        recogevariables(cur->valor("factura"),TIPO_TEXTO);
        recogevariables(cur->valor("incregistro"), TIPO_NUMERO);
        recogevariables(cur->valor("regularizacion"), TIPO_TEXTO);
        recogevariables(cur->valor("plan349"), TIPO_TEXTO);
        recogevariables(cur->valor("numorden"), TIPO_NUMERO);
        recogevariables(cur->valor("cif"), TIPO_TEXTO);
        cur->siguienteregistro();
    }// end while
    delete cur;

    fprintf(stderr,"Hemos recolectado las nuevas variables\n");
    for (i=0;i<indvariablescta;i++) {
        labelcta[i] = new QLabel(groupBox1,"");
        labelcta[i]->setGeometry( QRect( 5, inc+32*(j), 150, 25 ) );
        labelcta[i]->setText(variablescta[i][2]);
        labelcta[i]->show();
        varcta[i] = new QLineEdit(groupBox1,"");
        varcta[i]->setGeometry( QRect( 150, inc+32*(j), 120, 25 ) );
        connect( varcta[i], SIGNAL( returnPressed() ), this, SLOT( return_cta() ) );
        connect( varcta[i], SIGNAL( textChanged(const QString &)), this, SLOT(codigo_textChanged(const QString &)));
		  varcta[i]->show();
        boton_cta[i] = new QToolButton( groupBox1, "" );
        boton_cta[i]->setPixmap( QPixmap(find1));
        boton_cta[i]->setGeometry( QRect( 275, inc+32*(j), 25, 25 ) );
        connect( boton_cta[i], SIGNAL( clicked() ), this, SLOT( boton_buscacuenta() ) );
        boton_cta[i]->show();
		  nomcta[i] = new QLabel(groupBox1,"");
		  nomcta[i]->setGeometry( QRect (310, inc+32*(j++), 350, 25) );
		  nomcta[i]->show();
    }// end for

    for (i=0;i<indvariablesfecha;i++) {
        labelfecha[i] = new QLabel(groupBox1,"");
        labelfecha[i]->setGeometry( QRect( 5, inc+32*(j), 150, 25 ) );
        labelfecha[i]->setText(variablesfecha[i][2]);
        labelfecha[i]->show();
        varfecha[i] = new QLineEdit(groupBox1,"");
        varfecha[i]->setGeometry( QRect( 150, inc+32*(j++), 150, 25 ) );
        connect( varfecha[i], SIGNAL( returnPressed() ), this, SLOT( return_fecha() ) );
        connect( varfecha[i], SIGNAL( textChanged(const QString &)), this, SLOT(fecha_textChanged(const QString &)));
        varfecha[i]->show();
    }// end for

    for (i=0;i<indvariablesnumero;i++) {
        labelnumero[i] = new QLabel(groupBox1,"");
        labelnumero[i]->setGeometry( QRect( 5, inc+32*(j), 150, 25 ) );
        labelnumero[i]->setText(variablesnumero[i][2]);
        labelnumero[i]->show();
        varnumero[i] = new QLineEdit(groupBox1,"");
        varnumero[i]->setGeometry( QRect( 150, inc+32*(j++), 150, 25 ) );
        connect( varnumero[i], SIGNAL( returnPressed() ), this, SLOT( return_numero() ) );
        varnumero[i]->show();
    }// end for

    for (i=0;i<indvariablestexto;i++) {
        labeltexto[i] = new QLabel(groupBox1,"");
        labeltexto[i]->setGeometry( QRect( 5, inc+32*(j), 150, 25 ) );
        labeltexto[i]->setText(variablestexto[i][2]);
        labeltexto[i]->show();
        vartexto[i] = new QLineEdit(groupBox1,"");
        vartexto[i]->setGeometry( QRect( 150, inc+32*(j++), 350, 25 ) );
        connect( vartexto[i], SIGNAL( returnPressed() ), this, SLOT( return_texto() ) );
        vartexto[i]->show();
    }// end for
}// end mostrarplantilla



/***************************************************************
 * Esta función recoge los valores de los cuadros de texto y   *
 * y rellena las tablas de variables con los valores recogidos.*
 ***************************************************************/
void aplinteligentesview::recogevalores() {
    int i=0;
    for (i=0;i<indvariablestexto;i++) {
        variablestexto[i][1]=vartexto[i]->text();
        fprintf(stderr,"%s=%s\n",variablestexto[i][0].latin1(),variablestexto[i][1].latin1());
    }// end for
    for (i=0;i<indvariablesnumero;i++) {
        variablesnumero[i][1]=varnumero[i]->text();
        fprintf(stderr,"%s=%s\n",variablesnumero[i][0].latin1(),variablesnumero[i][1].latin1());
    }// end for
    for (i=0;i<indvariablesfecha;i++) {
        variablesfecha[i][1]=varfecha[i]->text();
        fprintf(stderr,"%s=%s\n",variablesfecha[i][0].latin1(),variablesfecha[i][1].latin1());
    }// end for
    for (i=0;i<indvariablescta;i++) {
        variablescta[i][1]=varcta[i]->text();
        fprintf(stderr,"%s=%s\n",variablescta[i][0].latin1(),variablescta[i][1].latin1());
    }// end for
}// end recogevalores



/**************************************************************
 * Esta funcion se encarga de crear el nuevo asiento a partir *
 * de los datos introducidos                                  *
 **************************************************************/
void aplinteligentesview::creaasiento() {
    QString codcuenta;
    QString contrapartida;
    QString debe;
    QString haber;
    QString fecha;
    QString conceptocontable;
    QString descripcion;
    int idcuenta=0;
    QString idcontrapartida;
    QString query;
    cursor2 *cur;
    cursor2 *cur1;

    // El primer paso para crear el asiento inteligente es crear una instancia en la tabla de asiento.
    query.sprintf("SELECT * FROM ainteligente WHERE idainteligente=%d",numainteligente);
    conexionbase->begin();
    cur = conexionbase->cargacursor(query,"asientoi");
    conexionbase->commit();
    delete cur;
    query.sprintf("SELECT * FROM binteligente WHERE idainteligente=%d",numainteligente);
    conexionbase->begin();
    cur= conexionbase->cargacursor(query,"superquery");
    conexionbase->commit();
    while (!cur->eof()) {
        codcuenta = aplicavariable(cur->valor("codcuenta"));
        query.sprintf("SELECT * FROM cuenta where codigo='%s'",codcuenta.latin1());
        conexionbase->begin();
        cur1 = conexionbase->cargacursor(query,"buscacodigo");
        conexionbase->commit();
        if (!cur1->eof()) {
            idcuenta = atoi(cur1->valor("idcuenta").latin1());
        }// end if
        delete cur1;
        contrapartida = aplicavariable(cur->valor("contrapartida"));
        query.sprintf("SELECT * FROM cuenta where codigo='%s'",contrapartida.latin1());
        conexionbase->begin();
        cur1 = conexionbase->cargacursor(query,"buscacodigo");
        conexionbase->commit();
        if (!cur1->eof()) {
            idcontrapartida = cur1->valor("idcuenta");
        } else {
            idcontrapartida = "NULL";
        }// end if
        delete cur1;
        debe = aplicavariable(cur->valor("debe"));
        haber = aplicavariable(cur->valor("haber"));
        fecha = aplicavariable(cur->valor("fecha"));
        conceptocontable = aplicavariable(cur->valor("conceptocontable"));
        descripcion = aplicavariable(cur->valor("descripcion"));
        query.sprintf("INSERT INTO borrador (idasiento, idcuenta, contrapartida, debe, haber, fecha, conceptocontable, descripcion) VALUES (%d, %d, %s, %s, %s, '%s', '%s', '%s')",numasiento,idcuenta,idcontrapartida.latin1(), debe.latin1(), haber.latin1(), fecha.latin1(), conceptocontable.latin1(), descripcion.latin1());
        conexionbase->begin();
        conexionbase->ejecuta(query);
        //    conexionbase->commit();

        // Miramos si existe una entrada en iva para este apunte y la creamos
        query.sprintf("SELECT * from ivainteligente WHERE idbinteligente=%s",cur->valor("idbinteligente").latin1());
        fprintf(stderr,"%s\n",query.latin1());
        conexionbase->begin();
        cursor2 *curiva = conexionbase->cargacursor(query,"ivas");
        conexionbase->commit();
        if (!curiva->eof()) {
            // Lo primero que hacemos es encontrar la entrada de borrador que corresponde
            query.sprintf( "SELECT max(idborrador) AS id FROM borrador");
            conexionbase->begin();
            cur1 = conexionbase->cargacursor(query,"borrador");
            conexionbase->commit();
            int idborrador = atoi(cur1->valor("id").latin1());
            delete cur1;

            //Inicializamos las variables de apunte
            //		inicializavariablesapunte(idborrador);


            // Luego buscamos la contrapartida
            contrapartida = aplicavariable(curiva->valor("contrapartida"));
            query.sprintf("SELECT * FROM cuenta where codigo='%s'",contrapartida.latin1());
            conexionbase->begin();
            cur1 = conexionbase->cargacursor(query,"buscacodigo");
            conexionbase->commit();
            if (!cur1->eof()) {
                idcontrapartida = cur1->valor("idcuenta");
            } else {
                idcontrapartida = "NULL";
            }// end if
            cifcuenta(atoi(idcontrapartida.latin1()));
            delete cur1;


            // Buscamos el iva
            QString iva = aplicavariable(curiva->valor("iva"));
            if (iva == "")
                iva = "NULL";
            // La factura
            QString factura = "'"+aplicavariable(curiva->valor("factura"))+"'";
            if (factura == "''")
                factura = "NULL";
            // La Base Imponible
            QString baseimp = aplicavariable(curiva->valor("baseimp"));
            if (baseimp == "")
                baseimp = "NULL";
            // El numero de orden
            QString numorden = "'"+aplicavariable(curiva->valor("numorden"))+"'";
            if (numorden == "''")
                numorden = "NULL";
            QString cif = "'"+aplicavariable(curiva->valor("cif"))+"'";
            if (cif == "''")
                cif = "NULL";


            query.sprintf("INSERT INTO registroiva (idborrador, contrapartida, iva, factura, baseimp, numorden, cif) VALUES (%d, %s, %s, %s, %s, %s, %s)",idborrador, idcontrapartida.latin1(), iva.latin1(), factura.latin1(), baseimp.latin1(), numorden.latin1(), cif.latin1());
            fprintf(stderr,"Registro de iva: %s",query.latin1());
            conexionbase->begin();
            conexionbase->ejecuta(query);
            conexionbase->commit();
        }// end if
        delete curiva;

        // Borramos las variables creadas para este apunte
        indvariablesapunte = 0;

        cur->siguienteregistro();
    }// end while
}// end creaasiento



void aplinteligentesview::recogevariables(QString texto, int tipo) {
    int posinicial,posfinal, posaux, posaux1;
    QString cadena=texto;
    QString subcadena;
    QString nomvar, descvar;
    int j;
    // Recogemos variables en el codigo de cuenta
    posinicial = cadena.find("$",0);
    if (posinicial != -1) {
        posfinal = cadena.find("$",posinicial+1);
        if (posfinal != -1) {
            subcadena = cadena.mid(posinicial,posfinal+1);
            posaux = subcadena.find(",",0);
            if (posaux != -1) {
                posaux1 = subcadena.find("$",posaux+1);
                descvar = subcadena.mid(posaux+1,posaux1-posaux-1);
                fprintf (stderr,"desc:%s<-->size %d\n",descvar.latin1(), posaux1);
                nomvar = subcadena.mid(0,posaux+1);
                // Si hay un comentario lo debemos borrar tras considerarlo para que no estorbe.
                nomvar.replace (nomvar.length()-1,nomvar.length(),"$");
            } else {
                nomvar = subcadena;
                descvar = "";
            }// end if
            // Buscamos si es una variable predefinida, y en caso de serlo obviamos una inserción de ésta.
            j=0;
            while(j<indvariablespredefinidas && variablespredefinidas[j][0] != nomvar) {
                j++;
            }// end while
            // Buscamos si es una variable de apunte, y en caso afirmativo obviamos una insercion de esta
            int d=0;
            while (d<indvariablesapunte && variablesapunte[d][0] != nomvar) {
                d++;
            }// end while

            if (j == indvariablespredefinidas && d== indvariablesapunte) {
                fprintf(stderr,"%d,%d, sub:%s\n",indvariablespredefinidas, j,subcadena.latin1());
                fprintf(stderr,"nom:%s\n",nomvar.latin1());
                switch(tipo) {
                case TIPO_CTA:
                    for(j=0;j<indvariablescta && variablescta[j][0] != nomvar;j++)
                        ;
                    if (j == indvariablescta) {
                        // Se trata de una inserción
                        variablescta[indvariablescta][0]=nomvar;
                        variablescta[indvariablescta][2]=descvar;
                        indvariablescta++;
                    } else {
                        // Se trata de una modificacion
                        if (variablescta[j][2] == "")
                            variablescta[j][2] = descvar;
                    }// end if
                    break;
                case TIPO_FECHA:
                    for(j=0;j<indvariablesfecha && variablesfecha[j][0] != subcadena;j++)
                        ;
                    if (j == indvariablesfecha) {
                        variablesfecha[indvariablesfecha][0]=nomvar;
                        variablesfecha[indvariablesfecha][2]=descvar;
                        indvariablesfecha++;
                    } else {
                        if (variablesfecha[j][2] == "")
                            variablesfecha[j][2] = descvar;
                    }// end if
                    break;
                case TIPO_NUMERO:
                    for(j=0;j<indvariablesnumero && variablesnumero[j][0] != subcadena;j++)
                        ;
                    if (j == indvariablesnumero) {
                        variablesnumero[indvariablesnumero][0]=nomvar;
                        variablesnumero[indvariablesnumero][2]=descvar;
                        indvariablesnumero++;
                    } else {
                        if (variablesnumero[j][2] == "")
                            variablesnumero[j][2] = descvar;
                    }// end if
                    break;
                case TIPO_TEXTO:
                    for(j=0;j<indvariablestexto && variablestexto[j][0] != subcadena;j++)
                        ;
                    if (j == indvariablestexto) {
                        variablestexto[indvariablestexto][0]=nomvar;
                        variablestexto[indvariablestexto][2] = descvar;
                        indvariablestexto++;
                    } else {
                        if (variablestexto[j][2] == "")
                            variablestexto[j][2] = descvar;
                    }// end if
                    break;
                }// end switch
            }// end if
        }// end if
    }// end if
}// end recogevariables


// Se hace un repaso que hace que se aplique una variable.
QString aplinteligentesview::aplicavariable(QString texto) {
    QString cadena=texto;
    fprintf(stderr,"Aplicavariable: texto: %s\n",texto.latin1());
    int posinicial, posfinal;
    int i;
    int fin = 0;
    // Vamos a quitar todos los comentarios que aparezcan.
    while (!fin) {
        fin = 1;
        posinicial = cadena.find(",",0);
        if (posinicial != -1) {
            posfinal = cadena.find("$",posinicial);
            if (posfinal != -1) {
                cadena.replace(posinicial,posfinal,"$");
                fin = 0;
            }// end if
        }// end if
    }// end while

    for (i=0;i<indvariablespredefinidas;i++) {
        posinicial = cadena.find(variablespredefinidas[i][0],0);
        while (posinicial != -1) {
            cadena.replace(posinicial,variablespredefinidas[i][0].length(),variablespredefinidas[i][1]);
            posinicial = cadena.find(variablespredefinidas[i][0],0);
        }// end if
    }// end for
    for (i=0;i<indvariablestexto;i++) {
        posinicial = cadena.find(variablestexto[i][0],0);
        while (posinicial != -1) {
            cadena.replace(posinicial,variablestexto[i][0].length(),variablestexto[i][1]);
            posinicial = cadena.find(variablestexto[i][0],0);
        }// end if
    }// end for
    for (i=0;i<indvariablesfecha;i++) {
        posinicial = cadena.find(variablesfecha[i][0],0);
        while (posinicial != -1) {
            cadena.replace(posinicial,variablesfecha[i][0].length(),variablesfecha[i][1]);
            posinicial = cadena.find(variablesfecha[i][0],0);
        }// end if
    }// end for
    for (i=0;i<indvariablesnumero;i++) {
        posinicial = cadena.find(variablesnumero[i][0],0);
        while (posinicial != -1) {
            cadena.replace(posinicial,variablesnumero[i][0].length(),variablesnumero[i][1]);
            posinicial = cadena.find(variablesnumero[i][0],0);
        }// end if
    }// end for
    for (i=0;i<indvariablescta;i++) {
        posinicial = cadena.find(variablescta[i][0],0);
        while (posinicial != -1) {
            cadena.replace(posinicial,variablescta[i][0].length(),variablescta[i][1]);
            posinicial = cadena.find(variablescta[i][0],0);
        }// end if
    }// end for

    // aplicamos las variables de apunte
    for (i=0;i<indvariablesapunte;i++) {
        posinicial = cadena.find(variablesapunte[i][0],0);
        while (posinicial != -1) {
            cadena.replace(posinicial,variablesapunte[i][0].length(),variablesapunte[i][1]);
            posinicial = cadena.find(variablesapunte[i][0],0);
        }// end if
    }// end for
    return (cadena);
}// end aplicavariable



void aplinteligentesview::borrawidgets() {
    int i;
    // Vamos a intentar borrar todos los datos antes de empezar.
    for (i=0;i<indvariablescta;i++) {
        delete varcta[i];
        delete boton_cta[i];
        delete labelcta[i];
		  delete nomcta[i];
    }// end for
    for (i=0;i<indvariablesfecha;i++) {
        delete varfecha[i];
        delete labelfecha[i];
    }// end for
    for (i=0;i<indvariablesnumero;i++) {
        delete varnumero[i];
        delete labelnumero[i];
    }// end for
    for (i=0;i<indvariablestexto;i++) {
        delete vartexto[i];
        delete labeltexto[i];
    }// end for
    indvariablescta = 0;
    indvariablesfecha = 0;
    indvariablesnumero = 0;
    indvariablestexto = 0;
}// end borrawidgets


void aplinteligentesview::selectfirst() {
    if (indvariablescta > 0) {
        varcta[0]->selectAll();
        varcta[0]->setFocus();
    } else if (indvariablesfecha > 0) {
        varfecha[0]->selectAll();
        varfecha[0]->setFocus();
    } else if (indvariablesnumero > 0) {
        varnumero[0]->selectAll();
        varnumero[0]->setFocus();
    } else if (indvariablestexto > 0) {
        vartexto[0]->selectAll();
        vartexto[0]->setFocus();
    }// end if
}// end selectfirst

// Esta funcion sirve para cambiar de un widget a otro dentro de la pantalla creada virtualmente.
// Si el widget es el último de la lista se hace un aceptar.
void aplinteligentesview::selectsiguiente(QLineEdit *edit) {
    int encontrado=0;
    int i=0;
    for (i=0; i<indvariablescta;i++) {
        if (encontrado == 1) {
            varcta[i]->selectAll();
            varcta[i]->setFocus();
            encontrado = 2;
        }// end if
        if (varcta[i] == edit) {
            encontrado = 1;
        }// end if
    }// end for
    for (i=0; i<indvariablesfecha;i++) {
        if (encontrado == 1) {
            varfecha[i]->selectAll();
            varfecha[i]->setFocus();
            encontrado = 2;
        }// end if
        if (varfecha[i] == edit) {
            encontrado = 1;
        }// end if
    }// end for
    for (i=0; i<indvariablesnumero;i++) {
        if (encontrado == 1) {
            varnumero[i]->selectAll();
            varnumero[i]->setFocus();
            encontrado = 2;
        }// end if
        if (varnumero[i] == edit) {
            encontrado = 1;
        }// end if
    }// end for
    for (i=0; i<indvariablestexto;i++) {
        if (encontrado == 1) {
            vartexto[i]->selectAll();
            vartexto[i]->setFocus();
            encontrado = 2;
        }// end if
        if (vartexto[i] == edit) {
            encontrado = 1;
        }// end if
    }// end for
    // En caso de que se haya encontrado el edit, y sea el último
    // Se hace un accept
    if (encontrado == 1) {
        boton_crear();
    }// end if
}// end selectsiguiente

void aplinteligentesview::return_fecha() {
    QLineEdit *fecha;
    fecha = (QLineEdit *) sender();
    fecha->setText(normalizafecha(fecha->text()).toString("dd/MM/yyyy"));
    if (fecha == fechaasiento) {
        selectfirst();
    } else {
        selectsiguiente(fecha);
    }
}// end return_fecha

void aplinteligentesview::fecha_textChanged( const QString & texto ) {
    QLineEdit *fecha;
    fecha = (QLineEdit *) sender();

    if (texto=="+") {
        QList<QDate> a;
        fecha->setText("");
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        fecha->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    }
    if (texto=="*")
        fecha->setText(QDate::currentDate().toString("dd/MM/yyyy") );
}//fin fechaasiento1_textChanged( const QString &texto )


void aplinteligentesview::codigo_textChanged(const QString &texto) {
    QLineEdit *codigo = (QLineEdit *) sender();
    if (texto == "+") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1();
        listcuentas->modo=1;
        listcuentas->inicializa(conexionbase);
        listcuentas->exec();
        codigo->setText(listcuentas->codcuenta);
        delete listcuentas;
    }// end if
}// end codigo_textChanged


