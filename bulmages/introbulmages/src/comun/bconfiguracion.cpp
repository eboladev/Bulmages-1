/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
 ***************************************************************************/
#include "bconfiguracion.h"
#include <qmessagebox.h>
#include <errno.h>
#include <qtable.h>

#include "bnuevaempresa.h"

#ifdef WIN32
#include <process.h>
#endif

#ifndef WIN32
#include <map>
#include <set>

typedef std::map<QString,QString> campos_usuario;
std::map<QString,campos_usuario> coleccion_usuarios;
std::set<QString> usuarios_borrados;
#endif


BConfiguracion::BConfiguracion(BSelector * ref, QWidget * parent, const char * name, WFlags f=0) : UIconfiguracion(parent,name,f) {
   PunteroAlSelector=ref;
   comboBoxFuente->insertStringList( (new QFontDatabase)->families() );
   if (PunteroAlSelector->tipo == "BulmaGés") {
      cargarFichaBulmages();
   }// end if
   cargarFichaUsuarios();
   tablaconfiguracion();
}// end BConfiguracion

BConfiguracion::~BConfiguracion() {
#ifndef WIN32
coleccion_usuarios.clear();
usuarios_borrados.clear();
#endif
}

void BConfiguracion::cerrar() {
  //Cargo el nuevo Idioma
  string archivo;
  switch (comboBoxIdioma->currentItem()) {
      case 0: archivo = "bulmages_ca.qm"; break;
      case 1: archivo = "bulmages_es.qm"; break;
      case 2: archivo = "bulmages_fr.qm"; break;
      case 3: archivo = "bulmages_en.qm"; break;
  }
  
  // Guardamos la configuración.
   for (int i =0;i<500;i++) {
      if (confpr->nombre(i) != "") {
         confpr->setValor(i,m_tconfiguracion->text(i,1).ascii());
      }// end if
   }// end for   

   confpr->saveconfig();
  
//  traductor->load(archivo.c_str(),confpr->valor(CONF_DIR_TRADUCCION).c_str());
  //Cargo la nueva fuente
  //Cierro la ventana de Configuración
  close();
}


// Esta función rellena la tabla de configuración del programa.
// Para luego poder guardar los parametros que haga falta.
void BConfiguracion::tablaconfiguracion() {
   m_tconfiguracion->setLeftMargin(0);
   m_tconfiguracion->hideColumn(2);
   m_tconfiguracion->setNumRows(500);
   QString cad;
   for (int i =0;i<500;i++) {
      cad= cad.setNum(i);
      m_tconfiguracion->setText(i,0,confpr->nombre(i));
      m_tconfiguracion->setText(i,1,confpr->valor(i));
      m_tconfiguracion->setText(i,2,cad);
      if (confpr->nombre(i) == "") m_tconfiguracion->hideRow(i);
   }// end for   
   m_tconfiguracion->adjustColumn(0);
   m_tconfiguracion->adjustColumn(1);
}// end tablaconfiguracion




/**********************************************************************************************/
/* BLOQUE UTILIDADES                                                                          */
/**********************************************************************************************/
void BConfiguracion::cargarFichaBulmages() {
//Carga idioma y fuente de la aplicacion 
  if (confpr->valor(CONF_TRADUCCION)=="ca") comboBoxIdioma->setCurrentItem(0);
  if (confpr->valor(CONF_TRADUCCION)=="es") comboBoxIdioma->setCurrentItem(1);
  if (confpr->valor(CONF_TRADUCCION)=="fr") comboBoxIdioma->setCurrentItem(2);
  if (confpr->valor(CONF_TRADUCCION)=="en") comboBoxIdioma->setCurrentItem(3);
  
  lineEditA_1->setText(PunteroAlSelector->nombreempresa->text());
  
  int i=0;
  while ( comboBoxFuente->text(i) != confpr->valor(CONF_FONTFAMILY_BULMAGES).c_str() &&  i < comboBoxFuente->count()) ++i;
  comboBoxFuente->setCurrentItem(i);
  spinBoxFuente->setValue(atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).c_str()));
}


void BConfiguracion::FontChanged(const QString & fuente) {
   muestra->setFont(QFont(fuente,spinBoxFuente->value()));
}

void BConfiguracion::FontSizeChanged(int tamano) {
   muestra->setFont(QFont(comboBoxFuente->currentText(),tamano));
}

void BConfiguracion::BotonA_10aceptar() {
//Salvar en la base de datos las preferencias del usuario: Idioma, Fuente, ...
  string codigoPais;
  switch (comboBoxIdioma->currentItem()) {
      case 0: codigoPais = "ca"; break;
      case 1: codigoPais = "es"; break;
      case 2: codigoPais = "fr"; break;
      case 3: codigoPais = "en"; break;
  }// end switch
  confpr->setValor(CONF_TRADUCCION,codigoPais);
  confpr->setValor(CONF_FONTFAMILY_BULMAGES,comboBoxFuente->currentText().ascii());
  confpr->setValor(CONF_FONTSIZE_BULMAGES,spinBoxFuente->text().ascii());
}


void BConfiguracion::BotonA_11rechazar() {
//poner el comboBoxFuente y el comboBoxIdioma a sus valores anteriores.
   if (PunteroAlSelector->tipo == "BulmaGés") {
      cargarFichaBulmages();
   }// end if
}// end BotonA_11rechazar


// ---------------------------------------------------------------------------
// Esta función se ejecuta cuando se lanza el cambio de nombre de la empresa.|
// ---------------------------------------------------------------------------
void BConfiguracion::BotonA_6nombreEmpresa() {
if (lineEditA_1->isReadOnly() ) { //Activa el line edit para que pueda ser editado.
    lineEditA_1->setReadOnly(false);
    lineEditA_1->setPaletteBackgroundColor(QColor(255,255,255));
  } else { //Y guarda el nuevo nombre de empresa.
    lineEditA_1->setReadOnly(true);  
    lineEditA_1->setPaletteBackgroundColor(QColor(255,255,0));
    
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(confpr->valor(CONF_METABASE).c_str());
    DBconn->begin();
    QString query;
    query.sprintf("UPDATE empresa SET nombre='%s' WHERE nombre='%s'",lineEditA_1->text().ascii(),PunteroAlSelector->nombreempresa->text().ascii());
    DBconn->ejecuta(query);
    DBconn->commit();
    delete DBconn;
    PunteroAlSelector->nombreempresa->setText(lineEditA_1->text());
  }// end if
}// end BotonA_6nombreEmpresa


/*********************************************************************************************************/
/* Aqui abrimos el cuador de dialogo que nos permite crear una empresa nueva basada en la                */
/* BASE DE DATOS bgplangcont                                                                             */
/*********************************************************************************************************/
void BConfiguracion::nuevaEmpresa() {
   BNuevaEmpresa *n= new BNuevaEmpresa(this,"Creador",true);
   n->exec();
   delete n;
}//Fin nuevaEmpresa



/*********************************************************************************************************/
/* Aqui creamos una nueva empresa que es una copia exacta de otra empresa que ya existe.                 */
/*********************************************************************************************************/
void BConfiguracion::BotonA_61clonarEmpresa(){
    QString dbEmpresa; 
//    (new BVisorEmpresas(&dbEmpresa, this,"Clonador",true))->exec();
    if (dbEmpresa!=NULL) {
        QMessageBox::information( this, "Debug", "Clonando la Base de Datos: " +dbEmpresa +"\n\nProceso no disponible...\n", QMessageBox::Ok);
    }
}

/*********************************************************************************************************/
/* Aqui borramos una empresa entera. No nos permite borrar la base de datos bgplangcont ni la base       */
/* de datos de la empresa que tengamos abierta en este momento.                                          */
/*********************************************************************************************************/
void BConfiguracion::borrarEmpresa() {
   QString dbEmpresa; 
   QString nombreEmpresa;
   QString idEmpresa;
   QString ejercicio;
   
   // Avisamos de que estamos en esta opcion
   fprintf(stderr,"borrarEmpresa\n");
   
   // Siempre se borra la empresa actual.
   dbEmpresa = PunteroAlSelector->empresabd;
   // (new BVisorEmpresas(& dbEmpresa, this,"Eliminador",true))->exec();
   if (dbEmpresa!="") {
      if (dbEmpresa=="bgplangcont") {
         QMessageBox::information( this, tr("Atención"), tr("Esta Base de Datos no puede ser eliminada.\n\r Es la plantilla para generar nuevas empresas."), QMessageBox::Ok);
      } else if (dbEmpresa != PunteroAlSelector->empresabd) {
         QMessageBox::warning( this, tr("Atención"), tr("No Está permitido eliminar la base \n\r de datos actualmente abierta."), QMessageBox::Ok,0);
      }else {
         fprintf(stderr,"VAmos a borrar\n");
               //Despues de evaluar algunos detalles, procedemos a eliminar la base de datos.
                  postgresiface2 *DBconn = new postgresiface2();
                  DBconn->inicializa( confpr->valor(CONF_METABASE).c_str() );
                  DBconn->begin();
                  QString query;
                  query.sprintf("SELECT idempresa, nombre, ano from empresa where nombredb='%s'",dbEmpresa.ascii());
                  cursor2 * recordSet = DBconn->cargacursor(query,"recordSet");
                  DBconn->commit();
                  idEmpresa=recordSet->valor("idempresa");
                  nombreEmpresa=recordSet->valor("nombre");
                  ejercicio=recordSet->valor("ano");
                  if (QMessageBox::Yes == QMessageBox::critical( this, tr("Peligro"), tr("Se perdera toda la información de la empresa seleccionada. \n\r. ESTAS SEGURO DE QUE DESEAS ELIMINAR LA EMPRESA?\n\r   "+ nombreEmpresa + " - Ejercicio: " +ejercicio) , QMessageBox::Yes,QMessageBox::No)) {
                     //Borramos...
                     DBconn->begin();
                     query.sprintf("DELETE FROM usuario_empresa WHERE idempresa='%s'",idEmpresa.ascii());
                     DBconn->ejecuta(query);
                     DBconn->commit();
                     DBconn->begin();
                     query.sprintf("DELETE FROM  empresa WHERE nombredb='%s'",dbEmpresa.ascii());
                     DBconn->ejecuta(query);
                     DBconn->commit();
                     DBconn->ejecuta("DROP DATABASE " + dbEmpresa);
                  }
                  delete recordSet;
                  delete DBconn;
      }   
   }
   close();
   PunteroAlSelector->seleccionaempresa_clicked();
}//Fin borrarEmpresa

/*********************************************************************************************************/
/* Aqui creamos un nuevo ejercicio para la empresa que tengamos abierta en el momento de llamar          */
/* a este proceso.                                                                                       */
/*********************************************************************************************************/
// Esta función es la que se encarga de cerrar al empresa
// Y de crear un nuevo ejercicio para dicha empresa

/*  
void empresa::nuevoejercicio() {
//Para crear un nuevo ejercicio se tiene que hacer desde el selector.

  char *args[4];
  int pid;
  int error;
   QString query;
    postgresiface2 *metabase;
    metabase = new postgresiface2();
    metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
    metabase->begin();
    query.sprintf("SELECT * FROM empresa WHERE nombredb='%s'",nombredb);
    fprintf(stderr,"%s\n",query.ascii());
    cursor2 *cur = metabase->cargacursor(query,"empresa");
    if (!cur->eof()) {
       QString nnombre = cur->valor("nombre");
       QString nnombredb = cur->valor("nombredb");
       QString nanodb = cur->valor("ano");
       nnombredb = nnombredb.left(nnombredb.length()-4);
       bool ok;
       int inanodb = nanodb.toInt(&ok,10);
       inanodb++;
       QString nanodb2;
       nanodb2.setNum(inanodb,10);
       nnombredb += nanodb2;
       query.sprintf("INSERT INTO EMPRESA ( nombre, nombredb, ano) VALUES ('%s','%s',%d)", nnombre.ascii(), nnombredb.ascii(), inanodb);
       fprintf(stderr,"%s\n",query.ascii());
       metabase->ejecuta(query);

       query.sprintf("SELECT last_value AS idempresa FROM empresa_idempresa_seq");
       cursor2 *cur2=metabase->cargacursor(query,"idempresa");
              
       query.sprintf("SELECT * FROM usuario_empresa WHERE idempresa = %s", cur->valor("idempresa").ascii());
       cursor2 *cur1=metabase->cargacursor(query,"us_emp");
       while (!cur1->eof()) {
          query.sprintf("INSERT INTO usuario_empresa (idusuario, idempresa, permisos) VALUES (%s,%s,%s)", cur1->valor("idusuario").ascii(), cur2->valor("idempresa").ascii(), cur1->valor("permisos").ascii());
          metabase->ejecuta(query);
          cur1->siguienteregistro();
       }// end whiel
       delete cur1;
       delete cur2;

       // Guardamos la Empresa
       args[0]=nombredb;
       args[1]=nombredb;
       args[2]="/tmp/bulmacop.pgdump";
       args[3]=NULL;
#ifndef WIN32
       if ((pid=fork()) < 0) {
         perror ("Fork failed");
         exit(errno);
       }// end if
       if (!pid) {
          string argumentos = confpr->valor(CONF_EJECUTABLES) + "guardaemp";
          error = execvp(argumentos.c_str(),args);
       }// end if
       if (pid) {
          waitpid (pid, NULL, 0);
       }// end if

       // Cargamos la empresa guardada
       args[0]=(char *)nnombredb.ascii();
       args[1]=(char *)nnombredb.ascii();
       args[2]="/tmp/bulmacop.pgdump";
       args[3]=NULL;
  //     defaultDB->close();
       if ((pid=fork()) < 0) {
         perror ("Fork failed");
         exit(errno);
       }// end if
       if (!pid) {
          string argumentos = confpr->valor(CONF_EJECUTABLES) + "cargaemp";
          error = execvp(argumentos.c_str(),args);
       }// end if
       if (pid) {
          waitpid (pid, NULL, 0);
       }// end if
#endif
       strcpy(nombredb,nnombredb.ascii());
    }// end if
    delete cur;
    metabase->commit();
    delete metabase;
    inicializa();
    inicializa1(pWorkspace);
    abreempresa();



// Borramos los asientos anteriores
   char query1[500];
   int valor;
   int resultado;
   valor = QMessageBox::warning( 0, theApp->translate("empresa","Borrar Asiento",""), theApp->translate("empresa","Se procedera a borrar el asiento.",""), QMessageBox::Yes, QMessageBox::No);
   if (valor ==  QMessageBox::Yes) {
       conexionbase2->begin();
       sprintf(query1,"DELETE FROM apunte where idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
       resultado = conexionbase2->ejecuta(query1);
       sprintf(query1,"DELETE FROM borrador where idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
       resultado += conexionbase2->ejecuta(query1);
       sprintf(query1,"DELETE FROM asiento WHERE idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
       resultado += conexionbase2->ejecuta(query1);
       if (resultado != 0) {
           conexionbase2->rollback();
       } else {
           conexionbase2->commit();
       }// end if
       Ordenarasientos();
//       introapunts1->repinta();
   }// end if

}// end nuevoejercicio

*/

void BConfiguracion::nuevoEjercicio() {
   QString query;
   if (0 == QMessageBox::warning( this, tr("Nuevo Ejercicio"), tr("¿Desea continuar usando la misma base de datos? - Haga clic en SI \n\n(Para iniciar el nuevo ejercico en otra base de datos - Haga clic en Otra)") , "&SI","&Otra"))  { 
      int x;
      QString ejer;
      postgresiface2 *DBconn = new postgresiface2();
      DBconn->inicializa(PunteroAlSelector->empresabd);
      DBconn->begin();
      query.sprintf("SELECT MAX(ejercicio) AS ejercicio FROM ejercicios WHERE periodo=0");
      cursor2 *curA = DBconn->cargacursor(query,"curA");
      if (!curA->eof()) ejer = curA->valor("ejercicio");
      ejer.setNum(ejer.toInt() + 1);
      if (ejer.toInt()<2000) ejer="2003";
      for (x=0; x<=12; x++) {
            query.sprintf("INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES('%s', '%d', 'f')",ejer.ascii(),x);
            DBconn->ejecuta(query);
      }
      DBconn->commit();
      QMessageBox::information( this, tr("Nuevo Ejercicio"), "El ejercicio ("+ ejer + ") ha sido creado con exito" , "&Aceptar");
   } else {  
      postgresiface2 *metabase = new postgresiface2();
      metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
      metabase->begin();
      query.sprintf("SELECT * FROM empresa WHERE nombredb='%s'",PunteroAlSelector->empresabd.ascii());
      cursor2 *cur = metabase->cargacursor(query,"empresa");
      metabase->commit();
      if (!cur->eof()) {
         QString campoIdEmpresa = cur->valor("idempresa");
         QString nuevo_nombre = cur->valor("nombre");
         QString nuevo_anodb = QString::number(cur->valor("ano").toInt()+1);
         QString nombredb = cur->valor("nombredb");
         QString nuevo_nombredb = nombredb.left(nombredb.length()-4)+nuevo_anodb;
         if ( (nombredb!="bgplangcont") && (QMessageBox::Yes == QMessageBox::information( this, tr("Nuevo Ejercicio"), tr("Este proceso generará un nuevo ejercicio. \n\r.  Empresa:   "+ nuevo_nombre + "\n\r.  Ejercicio: "+nuevo_anodb) , QMessageBox::Yes,QMessageBox::No)) ) { 
            //Comprovamos que no exista ya la base de datos que queremos crear.
            metabase->begin();
            query.sprintf("SELECT * FROM empresa WHERE nombredb='%s'",nuevo_nombredb.ascii());
            cursor2 *cur = metabase->cargacursor(query,"empresa");
            metabase->commit();
            if (cur->eof()) {//No existe aun el ejercicio que pretendemos crear, entonces a por el !!
               // Creamos la base de datos...
               query.sprintf("CREATE DATABASE %s WITH TEMPLATE=%s",nuevo_nombredb.ascii(),nombredb.ascii());
               if ( ! metabase->ejecuta(query) ) {
                     //Creamos la entrada en la metabd
                     metabase->begin();
                     query.sprintf("INSERT INTO empresa ( nombre, nombredb, ano, ejant) VALUES ('%s','%s',%d,'%s')", nuevo_nombre.ascii(), nuevo_nombredb.ascii(), nuevo_anodb.toInt(),nombredb.ascii());
                     metabase->ejecuta(query);
            
                     query.sprintf("SELECT last_value AS idempresa FROM empresa_idempresa_seq");
                     cursor2 *cur2=metabase->cargacursor(query,"idempresa");
                  
                     query.sprintf("SELECT * FROM usuario_empresa WHERE idempresa = %s", campoIdEmpresa.ascii());
                     cursor2 *cur1=metabase->cargacursor(query,"us_emp");
                     while (!cur1->eof()) {
                        query.sprintf("INSERT INTO usuario_empresa (idusuario, idempresa, permisos) VALUES (%s,%s,%s)", cur1->valor("idusuario").ascii(), cur2->valor("idempresa").ascii(), cur1->valor("permisos").ascii());
                        metabase->ejecuta(query);
                        cur1->siguienteregistro();
                     }// end while
                     metabase->commit();
                     delete cur1;
                     delete cur2; 
                     delete cur;
                     delete metabase;
               
                     // Borramos los asientos que hemos heredado del ejercicio anterior.
                     int resultado;
                     postgresiface2 *DBconn = new postgresiface2();
                     DBconn->inicializa(nuevo_nombredb.ascii());
                     DBconn->begin();
                     //query.sprintf("DELETE FROM apunte WHERE idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
                     query.sprintf("DELETE FROM apunte *");
                     resultado = DBconn->ejecuta(query);
                     //query.sprintf("DELETE FROM borrador WHERE idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
                     query.sprintf("DELETE FROM borrador *");
                     resultado += DBconn->ejecuta(query);
                     //query.sprintf("DELETE FROM asiento WHERE idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
                     query.sprintf("DELETE FROM asiento *");
                     resultado += DBconn->ejecuta(query);
                     if (resultado != 0) {
                        DBconn->rollback();
                     } else {
                        DBconn->commit();
                  }// end if
                  delete DBconn;
               } else {
                  QMessageBox::information( this, tr("Nuevo Ejercicio"), tr("No se ha podido crear un nuevo ejercicio. \n\r Posiblemente hay alguna conexión abierta  \n\r.  Empresa: ")+nombredb , QMessageBox::Ok); 
               }// end if
            } else {
               //Si el ejercicio ya ha sido creado con anterioridad se lo decimos al usuario.
               QMessageBox::information( this, tr("Nuevo Ejercicio"), tr("No se puede crear un nuevo ejercicio. \n\r El ejercicio que pretende crear ya existe.") , QMessageBox::Ok,0); 
            }//end if
         }//end if QMessageBox
      }// end if  
   }// end if
}//Fin nuevoEjercicio


/*********************************************************************************************************/
/* Creamos una copia de seguridad de una base de datos                                                   */
/*********************************************************************************************************/
void BConfiguracion::BotonA_3salvarEmpresa() {
  char *args[5];
  int pid;
  QString dbEmpresa; 
  QString PGserver;
  PGserver = "-h ";
  PGserver += confpr->valor(CONF_SERVIDOR).c_str();
  dbEmpresa = PunteroAlSelector->empresabd;

//  (new BVisorEmpresas(& dbEmpresa, this,"Backup",true))->exec();
  if (dbEmpresa!="") {
      QString fn = QFileDialog::getSaveFileName(0, "Empresas (*.pgdump)", 0,"Guardar Empresa","Elige el nombre de empresa con el que guardar");
      if (!fn.isEmpty()) {
         if (fn.right(7)!= ".pgdump") fn = fn +".pgdump";
         fprintf(stderr,"Vamos a guardar la empresa en el fichero %s\n",fn.ascii());
         args[0]=(char *) "guardaemp";
         args[1]=(char *) PGserver.ascii(); //Necesario para conectar a un host remoto.
         args[2]=(char *) dbEmpresa.ascii();
         args[3]=(char *) fn.ascii();
         args[4]=NULL;

#ifndef WIN32
         if ((pid=fork()) == -1) {
           perror ("Fork failed");
           exit(errno);
         }// end if
         if (!pid) {
            string argumentos = confpr->valor(CONF_EJECUTABLES) + "guardaemp";
            exit(execvp(argumentos.c_str(),args));
         }// end if
         if (pid) waitpid (pid, NULL, 0);
#endif
     }
  }
}

/*********************************************************************************************************/
/* Restauramos una copia de seguridad de una base de datos                                               */
/*********************************************************************************************************/
// Para cargar la empresa debe estar sin ningun usuario dentro de ella.
void BConfiguracion::BotonA_4restaurarEmpresa(){
  char *args[5];
  int pid;
  int error;
  QString dbEmpresa; 
  QString PGserver;
  PGserver = "-h ";
  PGserver += confpr->valor(CONF_SERVIDOR).c_str();
  dbEmpresa = PunteroAlSelector->empresabd;

//  (new BVisorEmpresas(& dbEmpresa, this,"Restore",true))->exec();
  if (!dbEmpresa.isEmpty()) {
     QString fn = QFileDialog::getOpenFileName(0, theApp->translate("empresa","Empresas (*.pgdump)",""), 0,theApp->translate("empresa","Cargar Empresa",""),theApp->translate("emrpesa","Elige el fichero a cargar.",""));
      if (!fn.isEmpty()) {
         args[0]=(char *) "cargaemp";
         args[1]=(char *) PGserver.ascii(); //Necesario para conectar a un host remoto.
         args[2]=(char *) dbEmpresa.ascii();
         args[3]=(char *) fn.ascii();
         args[4]=NULL;

#ifndef WIN32
         if ((pid=fork())== -1) {
           perror ("Fork failed");
           exit(errno);
         }// end if
         if (!pid) {
            QString argumentos = confpr->valor(CONF_EJECUTABLES).c_str();
	    argumentos  +=  "cargaemp";
            fprintf(stderr,"Ejecutamos el cargaemp\n");
            error = execvp(argumentos.ascii(),args);
            fprintf(stderr,"Fin del cargaemp\n");
            exit(error);
         }// end if
         if (pid) waitpid (pid, NULL, 0);
#endif
      }// end if
    }
  }

/**********************************************************************************************/
/* BLOQUE USUARIOS                                                                            */
/**********************************************************************************************/

void BConfiguracion::cargarFichaUsuarios() {

#ifndef WIN32

//Carga los Usuarios de la Base de Datos
   campos_usuario datos_usuario;   
   QString query;
   postgresiface2 * conexionDB;
   conexionDB = new postgresiface2();
   conexionDB->inicializa(confpr->valor(CONF_METABASE).c_str());
   cursor2 *recordSet, *privilegios=NULL;
   QString acceso;
   QListViewItem *it=NULL;
   conexionDB->begin();
   recordSet=conexionDB->cargacursor("SELECT * from usuario","recordSet");
   conexionDB->commit();
   coleccion_usuarios.clear();
   usuarios_borrados.clear();
   listView1->clear();
//   listView1->hideColumn(1);
   listView1->setColumnWidth(0,155);
//   (listView2->header())->setResizeEnabled (false);
   listView2->setColumnWidth(0,60);
   listView2->setColumnWidth(1,330);
//   listView2->hideColumn(2);
   listView2->setSorting(2,true);
   while (!recordSet->eof()) {
         it =new QListViewItem(listView1);
         datos_usuario.insert(make_pair("idusuario",recordSet->valor("idusuario")));
         datos_usuario.insert(make_pair("login",recordSet->valor("login")));
         datos_usuario.insert(make_pair("password",recordSet->valor("password")));
         datos_usuario.insert(make_pair("nombre",recordSet->valor("nombre")));
         datos_usuario.insert(make_pair("apellido1",recordSet->valor("apellido1")));	 
         datos_usuario.insert(make_pair("apellido2",recordSet->valor("apellido2")));
         datos_usuario.insert(make_pair("coment","Id. Usuario: " + recordSet->valor("idusuario")));
         query.sprintf("SELECT * from usuario_empresa WHERE idempresa IN (SELECT idempresa FROM empresa WHERE nombredb='%s') AND idusuario=%i",PunteroAlSelector->empresabd.ascii(),recordSet->valor("idusuario").toInt());
         conexionDB->begin();
         privilegios=conexionDB->cargacursor(query,"privilegios");
         conexionDB->commit();
         if ( !privilegios->eof() ) {
             privilegios->valor("permisos") == "1" ? acceso="S" : acceso="V";
             datos_usuario.insert(make_pair("prv1000",acceso));
         } else {
             datos_usuario.insert(make_pair("prv1000","N"));
         }
         coleccion_usuarios.insert(make_pair(recordSet->valor("login"),datos_usuario));
         datos_usuario.clear();
         it->setText(0,recordSet->valor("login"));
         it->setText(1,recordSet->valor("idusuario"));
         recordSet->siguienteregistro();
   }// end while
   listView1->setCurrentItem(it);
   if (privilegios) delete privilegios;
   delete recordSet;
   delete conexionDB;
#endif
}


void BConfiguracion::listView1_currentChanged(QListViewItem *it) {
#ifndef WIN32
  campos_usuario datos_usuario;
  
  datos_usuario = coleccion_usuarios.find(it->text(0))->second;  
  listView2->findItem("prv1000",2)->setText( 0,datos_usuario.find("prv1000")->second );
  lineEdit1->setText( datos_usuario.find("login")->second );
  lineEdit2->setText( datos_usuario.find("password")->second );
  lineEdit3->setText( datos_usuario.find("nombre")->second );
  lineEdit4->setText( datos_usuario.find("apellido1")->second );
  lineEdit5->setText( datos_usuario.find("apellido2")->second );
  textEdit1->setText( datos_usuario.find("coment")->second );
#endif
}// end listView1_currentChanged

void BConfiguracion::users_info_changed() {
#ifndef WIN32
  campos_usuario datos_usuario;
  
  coleccion_usuarios.erase(listView1->currentItem()->text(0));  
  datos_usuario.insert(make_pair("login",lineEdit1->text()));
  datos_usuario.insert(make_pair("password",lineEdit2->text()));
  datos_usuario.insert(make_pair("nombre",lineEdit3->text()));
  datos_usuario.insert(make_pair("apellido1",lineEdit4->text()));	 
  datos_usuario.insert(make_pair("apellido2",lineEdit5->text()));
  datos_usuario.insert(make_pair("coment",textEdit1->text()));
  datos_usuario.insert(make_pair("idusuario",listView1->currentItem()->text(1)));
  datos_usuario.insert(make_pair("prv1000",listView2->findItem("prv1000",2)->text(0)));
  if ( (coleccion_usuarios.find(lineEdit1->text())) != coleccion_usuarios.end() ) {
      QMessageBox::warning( 0, "Nuevo Usuario", "Ya existe otro usuario con este login.", QMessageBox::Ok,0);
      lineEdit1->setText(listView1->currentItem()->text(0));
  } else {
      coleccion_usuarios.insert(make_pair(lineEdit1->text(),datos_usuario));
      listView1->currentItem()->setText(0,lineEdit1->text());
      
  }
#endif
}

//Creamos un usuario nuevo
void BConfiguracion::newUser() {
#ifndef WIN32
  campos_usuario datos_usuario;
   
  QString nombreInicial=tr("Nuevo Usuario");
  QListViewItem * it = new QListViewItem(listView1);
  it->setText(0,nombreInicial);
  it->setText(1,"-1");
  datos_usuario.insert(make_pair("login",nombreInicial));
  datos_usuario.insert(make_pair("password",""));
  datos_usuario.insert(make_pair("nombre",""));
  datos_usuario.insert(make_pair("apellido1",""));
  datos_usuario.insert(make_pair("apellido2",""));
  datos_usuario.insert(make_pair("coment",""));
  datos_usuario.insert(make_pair("idusuario","-1"));
  datos_usuario.insert(make_pair("prv1000","N"));
  coleccion_usuarios.insert(make_pair(nombreInicial,datos_usuario));
  listView1->setCurrentItem(it);
#endif
}

//Borramos un usuario
void BConfiguracion::deleteUser() {
#ifndef WIN32
coleccion_usuarios.erase(listView1->currentItem()->text(0));  
usuarios_borrados.insert(listView1->currentItem()->text(1));
listView1->takeItem(listView1->currentItem());
#endif
}

//Creamos un usuario nuevo con los mismos permisos que el usuario seleccionado.
void BConfiguracion::cloneUser() {
//Trabajo pendiente...
}

//Salvamos los usuarios en la base de datos
void BConfiguracion::BotonB_1Aplicar(){
#ifndef WIN32
  campos_usuario datos_usuario;
  std::map<QString,campos_usuario>::iterator bucle_usuarios;
  postgresiface2 *DBConn = new postgresiface2();
  DBConn->inicializa(confpr->valor(CONF_METABASE).c_str());
  QString query;
  DBConn->begin();
  //Introducioms los cambios en la base de datos
  bucle_usuarios=coleccion_usuarios.begin();
  while (bucle_usuarios != coleccion_usuarios.end()) {
      datos_usuario = bucle_usuarios->second;  
      if ( datos_usuario.find("idusuario")->second == "-1" ) {
        query.sprintf("INSERT INTO usuario(nombre,apellido1,apellido2,login,password) VALUES('%s','%s','%s','%s','%s')", (datos_usuario.find("nombre")->second).ascii(), (datos_usuario.find("apellido1")->second).ascii(), (datos_usuario.find("apellido2")->second).ascii(), (datos_usuario.find("login")->second).ascii(), (datos_usuario.find("password")->second).ascii());
      } else {
        query.sprintf("UPDATE usuario SET nombre='%s', apellido1='%s', apellido2='%s', login='%s', password='%s' WHERE idusuario=%s", (datos_usuario.find("nombre")->second).ascii(), (datos_usuario.find("apellido1")->second).ascii(), (datos_usuario.find("apellido2")->second).ascii(), (datos_usuario.find("login")->second).ascii(), (datos_usuario.find("password")->second).ascii(),(datos_usuario.find("idusuario")->second).ascii());
      }
      DBConn->ejecuta(query);
      ++bucle_usuarios;
  }
  //Borramos los usuarios que hemos eliminado
  std::set<QString>::iterator id_borrados;
  id_borrados=usuarios_borrados.begin();
  while ( id_borrados != usuarios_borrados.end() ) {
      query.sprintf("DELETE FROM usuario_empresa WHERE idusuario='%s'",id_borrados->ascii());
      DBConn->ejecuta(query);
      query.sprintf("DELETE FROM usuario WHERE idusuario='%s'",id_borrados->ascii());
      DBConn->ejecuta(query);
      ++id_borrados;
  }
  DBConn->commit();
  usuarios_borrados.clear();
  //Actualizamos la tabla usuario_empresa
  DBConn->begin();
  query.sprintf("SELECT login,idusuario,idempresa FROM usuario, empresa WHERE nombredb='%s'",PunteroAlSelector->empresabd.ascii());
  cursor2 * usuaris = DBConn->cargacursor(query,"usuaris"); 
  while (! usuaris->eof() ) {
      bucle_usuarios=coleccion_usuarios.begin();
      while ( bucle_usuarios != coleccion_usuarios.end() ) { 
          if ( usuaris->valor("login") != (bucle_usuarios->second).find("login")->second  )  ++bucle_usuarios;
          else break; 
      }
      if (bucle_usuarios != coleccion_usuarios.end()) {
          query.sprintf("DELETE FROM usuario_empresa WHERE idempresa IN (SELECT idempresa FROM empresa WHERE nombredb='%s') AND idusuario='%s'",PunteroAlSelector->empresabd.ascii(),usuaris->valor("idusuario").ascii());
          DBConn->ejecuta(query);
          if ( (bucle_usuarios->second).find("prv1000")->second  == "S") { //Lectura Escritura
              query.sprintf("INSERT INTO usuario_empresa (idusuario, idempresa, permisos) VALUES ('%s','%s','%s')", usuaris->valor("idusuario").ascii(), usuaris->valor("idempresa").ascii(),"1");
              DBConn->ejecuta(query);
          }
          if ( (bucle_usuarios->second).find("prv1000")->second == "V") { //Solo Lectura
              query.sprintf("INSERT INTO usuario_empresa (idusuario, idempresa, permisos) VALUES ('%s','%s','%s')", usuaris->valor("idusuario").ascii(), usuaris->valor("idempresa").ascii(),"2");
              DBConn->ejecuta(query);
         } 
      }
     usuaris->siguienteregistro();
  }
  DBConn->commit();
#endif
}

//Desacemos los cambios que hemos hecho (UNDO).
void BConfiguracion::BotonB_2Desacer(){
cargarFichaUsuarios();
}

void BConfiguracion::listView2_clickBotonDerecho(QListViewItem* item,const QPoint& coord ,int col){
   QPopupMenu *popup;
   popup = new QPopupMenu;
        popup->insertItem(tr("S - Acceso Permitido"),101);
        popup->insertItem(tr("N - Acceso Denegado"),102);
        popup->insertItem(tr("V - Acceso Restringido"),103);
        //popup->insertSeparator();
   col = popup->exec(coord);
   delete popup;
   item=listView2->firstChild();
   while (item != 0) {
       if ( item->isSelected() ) {
           switch(col) {
               case 101: item->setText(0,"S"); break;
               case 102: item->setText(0,"N"); break;
               case 103: item->setText(0,"V"); break;
          }
       }       
       item = item->itemBelow();
   }
users_info_changed();
}


//Por conveniencia (Bug QT??) 
void BConfiguracion::listiView2_clickMouse(int button,QListViewItem * item,const QPoint& pos,int col){
//   (listView2->header())->setResizeEnabled (false);
   button=1; //elimina warning
   pos.isNull(); //elimina warning
   item->text(col); //elimina warning
   listView2->setColumnWidth(0,60);
   listView2->setColumnWidth(1,330);
//   listView2->hideColumn(2);
   listView2->setSorting(2,true);
}
