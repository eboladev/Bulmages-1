/***************************************************************************
                          bulmages01.cpp  -  description
                             -------------------
    begin                : Wed Nov 27 17:16:28 CET 2002
    copyright            : (C) 2002 by Tomeu Borras Riera
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
#ifndef WIN32
#include <unistd.h>
#endif

// Qt includes
#include <qaccel.h>


// Hacemos algunas pruebas con una docwindow. Para el corrector de contabilidad
#include <qdockwindow.h>
#include "correctorwidget.h"


// application specific includes
#include "bulmages01.h"
#include "configuracion.h"
#include "aboutview.h"
#include "actualizacionesview.h"
//#include "usuariosview.h"

// Inclusiones para el log
#include "log.h"

//Inclusión de las imagenes.
#include "images/filenew.xpm"
#include "images/fileopen.xpm"
#include "images/filesave.xpm"
#include "images/plancuentas.xpm"
#include "images/configuracion.xpm"
#include "images/lapiz.xpm"
#include "images/cuentas.xpm"
#include "images/mayor.xpm"
#include "images/diario.xpm"
#include "images/bulmages.xpm"
#include "images/asiento.xpm"
#include "images/forward.xpm"
#include "images/back.xpm"
#include "images/finish.xpm"
#include "images/start.xpm"
#include "images/save.xpm"
#include "images/print.xpm"
#include "images/reload.xpm"
#include "images/ccoste.xpm"
#include "images/canales.xpm"
#include "images/usuario.xpm"
#include "images/ainteligente.xpm"
#include "images/balance.xpm"
#include "images/balancetree.xpm"
#include "images/filter.xpm"
#include "images/help.xpm"
#include "images/estadisticas.xpm"

Bulmages01::Bulmages01(QWidget * parent, const char * name, WFlags f, QString * DB, QString * User, QString * Passwd,QString *ejercicioMetaDB) 
: QMainWindow(parent,name,f) {
  //Si no existe un ejercicio en la tabla "ejercicios" entonces usamos el ejercicio de la MetaDB
  DBName=*DB;
  empresaactual.inicializa(DB,User,Passwd);
  if (empresaactual.ejercicioactual() =="") empresaactual.setejactual(*ejercicioMetaDB);
  setCaption(tr("BulmaGés ") + empresaactual.ejercicioactual());  
  initView();
  initActions();
  initMenuBar();
  initToolBar();
  initStatusBar();
  showView();
  viewToolBar->setOn(true);
  viewStatusBar->setOn(true);
//  viewCorrector->setOn(true);
  
  // Le indicamos al sistema de log cual va a ser la empresa.
  fprintf(stderr,"HEMOS INICIALIZADO \n");
  
  // OJO QUE AQUI ESTA FALLANDO ESTO.
  ctllog->setempresa(&empresaactual);
  ctllog->add(LOG_SEG | LOG_TRA, 1,"BmgCtt001" , "El usuario a entrado en bulmages01"); //: --"+empresaactual.nomuserempresa()+"-- ha entrado en la empresa(DB): --"+empresaactual.nombreDB);
  fprintf(stderr,"HEMOS INICIALIZADO POR COMPLETO\n");

}// end Bulmages01


Bulmages01::~Bulmages01() {
  delete editCut;
  delete editCopy;
  delete editUndo;
  delete editPaste;
  delete viewToolBar;
  delete viewStatusBar;
  delete viewFullScreen;
  delete viewCorrector;
  delete windowNewWindow;
  delete windowCascade;
  delete windowTile;
  delete windowAction;
  delete helpAboutApp;
  delete planCuentas;
  delete grupos;
  delete listadoCuentas;
  delete introducirApuntes;
  delete introducirApuntes1;
  delete asientosInteligentes;
  delete listadoApuntes;
  delete propiedadesEmpresa;
  //delete guardarEmpresa;
  //delete cargarEmpresa;
  //delete nuevaEmpresa;
  delete cierraEmpresa;
  delete regularizaEmpresa;
  delete abreEmpresa;
  //delete borrarEmpresa;
  //delete cambiarEmpresa;
  delete libroMayor;
  delete libroDiario;  
  //delete perdygan;
  //delete balan;
  delete regiva;
  delete centrosCoste;
  delete canalesMenu;
  delete asientos;
  delete forwardaction;
  delete backaction;
  delete finishaction;
  delete startaction;
  delete saveaction;
  delete printaction;
  delete reloadaction;
  //delete usuarioaction;  
  //delete view_back;
  //delete pWorkspace; //En algunos casos provoca Segmentation Fault.
  delete ayuda;
  delete amortiz; 
  //En el constructor asignamos un puntero a empresa { ctllog->setempresa(&empresaactual); }
  //Al destruir la clase empresa no podemos seguir apuntando a un objeto que ya no existe.
  //es necesario eliminar la referencia a este objeto, por ejemplo aquí:
  //justo antes logeo la salida del usuario (René)
  
  ctllog->add(LOG_SEG | LOG_TRA, 1,"BmgDtt017","El usuario sale de bulmages01." ); 
  ctllog->setempresa(NULL);
}


void Bulmages01::initActions() {
  QPixmap openIcon, saveIcon, newIcon, planCuentasIcon, configuracionIcon, lapizIcon, mayorIcon, diarioIcon, asientoIcon;
  QPixmap forwardIcon, backIcon, finishIcon, startIcon;
  QPixmap printIcon, reloadIcon;
  QPixmap ccosteIcon, canalesIcon, usuarioIcon;
  QPixmap ainteligenteIcon, ayudaIcon;
  QPixmap balanceIcon, balancetreeIcon;
  QPixmap filterIcon;
  QPixmap estadisticasIcon;
  
  newIcon = QPixmap(filenew);
  openIcon = QPixmap(fileopen);
  saveIcon = QPixmap(filesave);
  planCuentasIcon = QPixmap(cuentas);
  configuracionIcon = QPixmap(configuracion);
  lapizIcon = QPixmap(lapiz);
  mayorIcon = QPixmap(mayor);
  diarioIcon = QPixmap(diario);
  asientoIcon = QPixmap(asiento);
  forwardIcon = QPixmap(forward);
  backIcon = QPixmap(back);
  finishIcon = QPixmap(finish);
  startIcon = QPixmap(start);
  saveIcon = QPixmap(save);
  printIcon = QPixmap(print);
  reloadIcon = QPixmap(reload);
  ccosteIcon = QPixmap(ccoste);
  canalesIcon = QPixmap(canales);
  usuarioIcon = QPixmap(usuario);
  ainteligenteIcon = QPixmap(ainteligente);
  balanceIcon = QPixmap(balance);
  balancetreeIcon = QPixmap(balancetree);
  ayudaIcon = QPixmap(help);
  filterIcon=QPixmap(filter);
  estadisticasIcon = QPixmap(estadisticas);
  
  
  // Ponemos el logo de la aplicacion
  setIcon(QPixmap(bulmages));
  
  editCut = new QAction(tr("Cut"), tr("Cu&t"), QAccel::stringToKey(tr("Ctrl+X")), this);
  editCut->setStatusTip(tr("Cuts the selected section and puts it to the clipboard"));
  editCut->setWhatsThis(tr("Cut\n\nCuts the selected section and puts it to the clipboard"));
  connect(editCut, SIGNAL(activated()), this, SLOT(slotEditCut()));

  editCopy = new QAction(tr("Copy"), tr("&Copy"), QAccel::stringToKey(tr("Ctrl+C")), this);
  editCopy->setStatusTip(tr("Copies the selected section to the clipboard"));
  editCopy->setWhatsThis(tr("Copy\n\nCopies the selected section to the clipboard"));
  connect(editCopy, SIGNAL(activated()), this, SLOT(slotEditCopy()));

  editUndo = new QAction(tr("Undo"), tr("&Undo"), QAccel::stringToKey(tr("Ctrl+Z")), this);
  editUndo->setStatusTip(tr("Reverts the last editing action"));
  editUndo->setWhatsThis(tr("Undo\n\nReverts the last editing action"));
  connect(editUndo, SIGNAL(activated()), this, SLOT(slotEditUndo()));

  editPaste = new QAction(tr("Paste"), tr("&Paste"), QAccel::stringToKey(tr("Ctrl+V")), this);
  editPaste->setStatusTip(tr("Pastes the clipboard contents to actual position"));
  editPaste->setWhatsThis(tr("Paste\n\nPastes the clipboard contents to actual position"));
  connect(editPaste, SIGNAL(activated()), this, SLOT(slotEditPaste()));

  viewToolBar = new QAction(tr("Toolbar"), tr("Tool&bar"), 0, this, 0, true);
  viewToolBar->setStatusTip(tr("Enables/disables the toolbar"));
  viewToolBar->setWhatsThis(tr("Toolbar\n\nEnables/disables the toolbar"));
  connect(viewToolBar, SIGNAL(toggled(bool)), this, SLOT(slotViewToolBar(bool)));

  viewStatusBar = new QAction(tr("Statusbar"), tr("&Statusbar"), 0, this, 0, true);
  viewStatusBar->setStatusTip(tr("Enables/disables the statusbar"));
  viewStatusBar->setWhatsThis(tr("Statusbar\n\nEnables/disables the statusbar"));
  connect(viewStatusBar, SIGNAL(toggled(bool)), this, SLOT(slotViewStatusBar(bool)));

  viewFullScreen = new QAction(tr("FullScreen"), tr("&FullScreen"), 0, this, 0, true);
  viewFullScreen->setStatusTip(tr("Enables/disables the full screen"));
  viewFullScreen->setWhatsThis(tr("FullScreen\n\nEnables/disables the full screen mode"));
  connect(viewFullScreen, SIGNAL(toggled(bool)), this, SLOT(slotViewFullScreen(bool)));

  viewCorrector = new QAction(tr("Corrector"), tr("&Corrector"), 0, this, 0, true);
  viewCorrector->setStatusTip(tr("Muestra/Oculta el corrector"));
  viewCorrector->setWhatsThis(tr("Corrector\n\nMuestra/oculta el corrector"));
  connect(viewCorrector, SIGNAL(toggled(bool)), this, SLOT(slotViewCorrector(bool)));
  
  windowNewWindow = new QAction(tr("New Window"), tr("&New Window"), 0, this);
  windowNewWindow->setStatusTip(tr("Opens a new view for the current document"));
  windowNewWindow->setWhatsThis(tr("New Window\n\nOpens a new view for the current document"));
  connect(windowNewWindow, SIGNAL(activated()), this, SLOT(slotWindowNewWindow()));

  windowCascade = new QAction(tr("Cascade"), tr("&Cascade"), 0, this);
  windowCascade->setStatusTip(tr("Cascades all windows"));
  windowCascade->setWhatsThis(tr("Cascade\n\nCascades all windows"));
  connect(windowCascade, SIGNAL(activated()), pWorkspace, SLOT(cascade()));

  windowTile = new QAction(tr("Tile"), tr("&Tile"), 0, this);
  windowTile->setStatusTip(tr("Tiles all windows"));
  windowTile->setWhatsThis(tr("Tile\n\nTiles all windows"));
  connect(windowTile, SIGNAL(activated()), pWorkspace, SLOT(tile()));

  windowAction = new QActionGroup(this, 0, false);
  windowAction->insert(windowNewWindow);
  windowAction->insert(windowCascade);
  windowAction->insert(windowTile);

  helpAboutApp = new QAction(tr("Acerca de "), tr("&Acerca de ..."), 0, this);
  helpAboutApp->setStatusTip(tr("Sobre la aplicacion"));
  helpAboutApp->setWhatsThis(tr("Acerca de ...\n\nInformacion sobre los autores de la aplicacion"));
  connect(helpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));

  planCuentas = new QAction(tr("Plan Contable"), planCuentasIcon, tr("&Plan Contable"), 0, this);
  planCuentas->setStatusTip(tr("Muestra el plan contable"));
  planCuentas->setWhatsThis(tr("Muestra el plan contable"));
  connect(planCuentas, SIGNAL(activated()), this, SLOT(slotPlanCuentas()));

  grupos = new QAction(tr("Grupos Contables"),  tr("&Grupos Contables"), 0, this);
  grupos->setStatusTip(tr("Muestra los grupos contables"));
  grupos->setWhatsThis(tr("Muestra los grupos contables"));
  connect(grupos, SIGNAL(activated()), this, SLOT(slotGruposContables()));

  listadoCuentas = new QAction(tr("Listado de Cuentas"), tr("&Listado de Cuentas"), 0, this);
  listadoCuentas->setStatusTip(tr("Muestra los listados de las cuentas"));
  listadoCuentas->setWhatsThis(tr("Muestra los listados de las cuentas"));
  connect(listadoCuentas, SIGNAL(activated()), this, SLOT(slotListadoCuentas()));

  introducirApuntes = new QAction(tr("Introducción de Apuntes"),lapizIcon, tr("&Introducción de Apuntes"), 0, this);
  introducirApuntes->setStatusTip(tr("Permite introducir Apuntes Contables"));
  introducirApuntes->setWhatsThis(tr("Permite introducir Apuntes Contables"));
  connect(introducirApuntes, SIGNAL(activated()), this, SLOT(slotIntroducirApuntes()));

  introducirApuntes1 = new QAction(tr("Introducción de Apuntes"),lapizIcon, tr("&Introducción de Apuntes"), 0, this);
  introducirApuntes1->setStatusTip(tr("Permite introducir Apuntes Contables"));
  introducirApuntes1->setWhatsThis(tr("Permite introducir Apuntes Contables"));
  connect(introducirApuntes1, SIGNAL(activated()), this, SLOT(slotIntroducirApuntes1()));

  asientosInteligentes = new QAction(tr("Asientos Inteligentes"),ainteligenteIcon,tr("&Asientos Inteligentes"), 0, this);
  asientosInteligentes->setStatusTip(tr("Permite introducir Apuntes Contables Mediante Plantillas"));
  asientosInteligentes->setWhatsThis(tr("Permite introducir Apuntes Contables Mediante Plantillas"));
  connect(asientosInteligentes, SIGNAL(activated()), this, SLOT(slotAsientosInteligentes()));
    
  listadoApuntes = new QAction(tr("Listado de Apuntes"), tr("&Listado de Apuntes"), 0, this);
  listadoApuntes->setStatusTip(tr("Listar Apuntes Contables"));
  listadoApuntes->setWhatsThis(tr("Listar Apuntes Contables"));
  connect(listadoApuntes, SIGNAL(activated()), this, SLOT(slotListarApuntes()));

  propiedadesEmpresa = new QAction(tr("Propiedades Empresa"),configuracionIcon, tr("&Propiedades Empresa"), 0,this);
  propiedadesEmpresa->setStatusTip(tr("Propiedades de la Empresa"));
  propiedadesEmpresa->setWhatsThis(tr("Propiedades de la Empresa"));
  connect(propiedadesEmpresa, SIGNAL(activated()), this, SLOT(slotPropiedadesEmpresa()));
/*
  guardarEmpresa = new QAction(tr("Copia Seguridad Empresa"), tr("&Guardar Empresa"), 0, this);
  guardarEmpresa->setStatusTip(tr("Hacer Copia de Seguridad de la Empresa Actual"));
  guardarEmpresa->setWhatsThis(tr("Hacer Copia de Seguridad de la Empresa Actual"));
  connect(guardarEmpresa, SIGNAL(activated()), this, SLOT(slotGuardarEmpresa()));
  
  cargarEmpresa = new QAction(tr("Restaurar Empresa"), tr("&Cargar Empresa"), 0, this);
  cargarEmpresa->setStatusTip(tr("Restaurar de Seguridad de la Empresa Actual"));
  cargarEmpresa->setWhatsThis(tr("Restaurar de Seguridad de la Empresa Actual"));
  connect(cargarEmpresa, SIGNAL(activated()), this, SLOT(slotCargarEmpresa()));

  nuevaEmpresa = new QAction(tr("Nueva Empresa"), tr("&Nueva Empresa"), 0, this);
  nuevaEmpresa->setStatusTip(tr("Crear una Empresal"));
  nuevaEmpresa->setWhatsThis(tr("Crear una empresa nueva"));
  connect(nuevaEmpresa, SIGNAL(activated()), this, SLOT(slotNuevaEmpresa()));
  
  borrarEmpresa = new QAction(tr("Borrar Empresa"), tr("&Borrar Empresa"), 0, this);
  borrarEmpresa->setStatusTip(tr("Borrar una Empresal"));
  borrarEmpresa->setWhatsThis(tr("Borrar una empresa"));
  connect(borrarEmpresa, SIGNAL(activated()), this, SLOT(slotBorrarEmpresa()));
  
  cambiarEmpresa = new QAction(tr("Cambiar Empresa"), tr("&Cambiar Empresa"), 0, this);
  cambiarEmpresa->setStatusTip(tr("Cambiar de Empresa o de ejercicio"));
  cambiarEmpresa->setWhatsThis(tr("Cambiar de empresa"));
  connect(cambiarEmpresa, SIGNAL(activated()), this, SLOT(slotCambiarEmpresa()));
    
  NEjercicio = new QAction(tr("Nuevo Ejercicio"), tr("&Nuevo Ejercicio"), 0, this);
  NEjercicio->setStatusTip(tr("Nuevo Ejercicio"));
  NEjercicio->setWhatsThis(tr("Nuevo Ejercicio"));
  connect(NEjercicio, SIGNAL(activated()), this, SLOT(slotNEjercicio()));

*/
  cierraEmpresa = new QAction(tr("Asiento de Cierre"), tr("Asiento de &Cierre"), 0, this);
  cierraEmpresa->setStatusTip(tr("Crear el asiento de cierre"));
  cierraEmpresa->setWhatsThis(tr("Crear el asiento de cierre"));
  connect(cierraEmpresa, SIGNAL(activated()), this, SLOT(slotcierraEmpresa()));

  regularizaEmpresa = new QAction(tr("Asiento de Regularización"), tr("Asiento de &Regularización"), 0, this);
  regularizaEmpresa->setStatusTip(tr("Regularizar Empresa"));
  regularizaEmpresa->setWhatsThis(tr("Regularizar Empresa"));
  connect(regularizaEmpresa, SIGNAL(activated()), this, SLOT(slotregularizaEmpresa()));
  
  abreEmpresa = new QAction(tr("Asiento de Apertura"), tr("Asiento de &Apertura"), 0, this);
  abreEmpresa->setStatusTip(tr("Abrir el asiento de apertura"));
  abreEmpresa->setWhatsThis(tr("Abrir el asiento de apertura"));
  connect(abreEmpresa, SIGNAL(activated()), this, SLOT(slotabreEmpresa()));
    
  libroMayor = new QAction(tr("Libro Mayor"),mayorIcon, tr("&Libro Mayor"), 0, this);
  libroMayor->setStatusTip(tr("Ver el libro Mayor"));
  libroMayor->setWhatsThis(tr("Ver el libro Mayor"));
  connect(libroMayor, SIGNAL(activated()), this, SLOT(slotLibroMayor()));

  libroDiario = new QAction(tr("Libro Diario"),diarioIcon, tr("&Libro Diario"), 0, this);
  libroDiario->setStatusTip(tr("Ver el libro Diario"));
  libroDiario->setWhatsThis(tr("Ver el libro Diario"));
  connect(libroDiario, SIGNAL(activated()), this, SLOT(slotLibroDiario()));

  librobalance = new QAction(tr("Balance"),balanceIcon, tr("&Balance"), 0, this);
  librobalance->setStatusTip(tr("Ver balance"));
  librobalance->setWhatsThis(tr("Ver balance"));
  connect(librobalance, SIGNAL(activated()), this, SLOT(slotBalance()));

  librobalancetree = new QAction(tr("Balance Jerarquico"),balancetreeIcon, tr("&Balance Jerarquico"), 0, this);
  librobalancetree->setStatusTip(tr("Ver balance Jerarquico"));
  librobalancetree->setWhatsThis(tr("Ver balance jerarquico"));
  connect(librobalancetree, SIGNAL(activated()), this, SLOT(slotBalanceTree()));

/*    
  perdygan = new QAction(tr("Perdidas y ganancias av"), tr("&Perdidas y ganancias av"), 0, this);
  perdygan->setStatusTip(tr("Ver cuenta de perdidas y ganancias"));
  perdygan->setWhatsThis(tr("Ver cuenta de perdidas y ganancias"));
  connect(perdygan, SIGNAL(activated()), this, SLOT(slotPerdidas()));
*/
  
  Modelo347 = new QAction(tr("Modelo 347 no oficial"), tr("&Modelo 347"), 0, this);
  Modelo347->setStatusTip(tr("Modelo 347 no oficial"));
  Modelo347->setWhatsThis(tr("Lista en pantalla los saldos acumulados superirores a ..."));
  connect(Modelo347, SIGNAL(activated()), this, SLOT(slot347()));

/*  
  balan = new QAction(tr("Balance abreviado"), tr("&Balance Abreviado"), 0, this);
  balan->setStatusTip(tr("Ver Balance de situacion abreviado"));
  balan->setWhatsThis(tr("Ver Balance de situacion abreviado"));
  connect(balan, SIGNAL(activated()), this, SLOT(slotBalan()));
*/
  regiva = new QAction(tr("Registro de IVA"), tr("&Registro de IVA"), 0, this);
  regiva->setStatusTip(tr("Ver Libro Registro de IVA"));
  regiva->setWhatsThis(tr("Ver Libro Registro de IVA"));
  connect(regiva, SIGNAL(activated()), this, SLOT(slotRegistroIva()));

  centrosCoste = new QAction(tr("Centros de Coste"),ccosteIcon, tr("&Centros de Coste"), 0, this);
  centrosCoste->setStatusTip(tr("Administracion de Centros de Coste"));
  centrosCoste->setWhatsThis(tr("Administracion de Centros de Coste"));
  connect(centrosCoste, SIGNAL(activated()), this, SLOT(slotCentrosCoste()));
  
  canalesMenu = new QAction(tr("Canales"),canalesIcon, tr("C&anales"), 0, this);
  canalesMenu->setStatusTip(tr("Administracion de Canales"));
  canalesMenu->setWhatsThis(tr("Administracion de Canales"));
  connect(canalesMenu, SIGNAL(activated()), this, SLOT(slotCanales()));
                                                                                                            
  asientos = new QAction(tr("Asientos"), asientoIcon, tr("&Asientos"), 0, this);
  asientos->setStatusTip(tr("Introduccion de asientos"));
  asientos->setWhatsThis(tr("Introduccion de ASientos"));
  connect(asientos, SIGNAL(activated()), this, SLOT(slotAsientos()));

  forwardaction = new QAction(tr("Siguiente"), forwardIcon, tr("&Siguiente"), 0, this);
  forwardaction->setStatusTip(tr("Siguiente"));
  forwardaction->setWhatsThis(tr("Siguiente"));
  connect(forwardaction, SIGNAL(activated()), this, SLOT(slotSiguiente()));

  backaction = new QAction(tr("Anterior"), backIcon, tr("&Anterior"), 0, this);
  backaction->setStatusTip(tr("Anterior"));
  backaction->setWhatsThis(tr("Anterior"));
  connect(backaction, SIGNAL(activated()), this, SLOT(slotAnterior()));

  finishaction = new QAction(tr("Ultimo"), finishIcon, tr("&Ultimo"), 0, this);
  finishaction->setStatusTip(tr("Ultimo"));
  finishaction->setWhatsThis(tr("Ultimo"));
  connect(finishaction, SIGNAL(activated()), this, SLOT(slotUltimo()));

  startaction = new QAction(tr("Primero"), startIcon, tr("&Primero"), 0, this);
  startaction->setStatusTip(tr("Primero"));
  startaction->setWhatsThis(tr("Primero"));
  connect(startaction, SIGNAL(activated()), this, SLOT(slotPrimero()));

  saveaction = new QAction(tr("Guardar"), saveIcon, tr("&Guardar"), 0, this);
  saveaction->setStatusTip(tr("Guardar"));
  saveaction->setWhatsThis(tr("Guardar"));
  connect(saveaction, SIGNAL(activated()), this, SLOT(slotGuardar()));

  printaction = new QAction(tr("Imprimir"), printIcon, tr("&Imprimir"), 0, this);
  printaction->setStatusTip(tr("Imprimir"));
  printaction->setWhatsThis(tr("Imprimir"));
  connect(printaction, SIGNAL(activated()), this, SLOT(slotImprimir()));

  reloadaction = new QAction(tr("Recargar"), reloadIcon, tr("&Recargar"), 0, this);
  reloadaction->setStatusTip(tr("Recargar"));
  reloadaction->setWhatsThis(tr("Recargar"));
  connect(reloadaction, SIGNAL(activated()), this, SLOT(slotReload()));
/*
  usuarioaction = new QAction(tr("Usuarios"), usuarioIcon, tr("&Usuarios"), 0, this);
  usuarioaction->setStatusTip(tr("Usuarios"));
  usuarioaction->setWhatsThis(tr("Usuarios"));
  connect(usuarioaction, SIGNAL(activated()), this, SLOT(slotUsuarios()));
*/
  amortiz = new QAction(tr("Amortizaciones"), tr("&Amortizaciones"), 0, this);
  amortiz->setStatusTip(tr("Amortizaciones"));
  amortiz->setWhatsThis(tr("Amortizaciones"));
  connect(amortiz, SIGNAL(activated()), this, SLOT(slotAmortizaciones()));
  
  bloqaction = new QAction(tr("Bloquear Fechas"), tr("&Bloquear Fechas"), 0, this);
  bloqaction->setStatusTip(tr("Bloquear Fechas"));
  bloqaction->setWhatsThis(tr("Bloquear Fechas"));
  connect(bloqaction, SIGNAL(activated()), this, SLOT(slotBloqFechas()));
  
  ayuda = new QAction(tr("Ayuda"), ayudaIcon, tr("&Ayuda"), 0, this);
  ayuda->setStatusTip(tr("Ayuda"));
  ayuda->setWhatsThis(tr("Ayuda"));
  connect(ayuda, SIGNAL(activated()), this, SLOT(slotAyuda()));

  ayudai = new QAction(tr("Ayuda en Internet"),ayudaIcon, tr("&Ayuda en Internet"), 0, this);
  ayudai->setStatusTip(tr("Ayuda en la web del proyecto"));
  ayudai->setWhatsThis(tr("Ayuda en la web del proyecto"));
  connect(ayudai, SIGNAL(activated()), this, SLOT(slotAyudai()));


  Abrirasientos = new QAction(tr("Abrir Asientos"), tr("&Abrir Asientos"), 0, this);
  Abrirasientos->setStatusTip(tr("Abrir Asientos"));
  Abrirasientos->setWhatsThis(tr("Espacia los Asientos"));
  connect(Abrirasientos, SIGNAL(activated()), this, SLOT(slotAbrirasientos()));

  Ordenarasientos = new QAction(tr("Ordenar Asientos"), tr("&Ordenar Asientos"), 0, this);
  Ordenarasientos->setStatusTip(tr("Ordenar Asientos"));
  Ordenarasientos->setWhatsThis(tr("Reorganiza los Asientos"));
  connect(Ordenarasientos, SIGNAL(activated()), this, SLOT(slotOrdenarasientos()));

  Segcuentas = new QAction(tr("Seguimiento de Cuentas"),estadisticasIcon ,tr("&Seguimiento de Cuentas"), 0, this);
  Segcuentas->setStatusTip(tr("Seguimiento de Cuentas"));
  Segcuentas->setWhatsThis(tr("Seguimiento de Cuentas"));
  connect(Segcuentas, SIGNAL(activated()), this, SLOT(slotSegCuentas()));  

  BGrafico = new QAction(tr("Balance Gráfico"), tr("&Balance Gráfico"), 0, this);
  BGrafico->setStatusTip(tr("Balance Gráfico"));
  BGrafico->setWhatsThis(tr("Balance Gráfico"));
  connect(BGrafico, SIGNAL(activated()), this, SLOT(slotBalanceGrafico()));


  filtro = new QAction(tr("Filtro"),filterIcon, tr("&Filtro"), 0, this);
  filtro->setStatusTip(tr("Filtro"));
  filtro->setWhatsThis(tr("Filtro"));
  connect(filtro, SIGNAL(activated()), this, SLOT(slotFiltro()));

  MPatrimoniales = new QAction(tr("Masas Patrimoniales"), tr("&Masas Patrimoniales"), 0, this);
  MPatrimoniales->setStatusTip(tr("Masas Patrimoniales"));
  MPatrimoniales->setWhatsThis(tr("Masas Patrimoniales"));
  connect(MPatrimoniales, SIGNAL(activated()), this, SLOT(slotMPatrimoniales()));

  CompBalance = new QAction(tr("Balances"), tr("&Balances"), 0, this);
  CompBalance->setStatusTip(tr("Balances"));
  CompBalance->setWhatsThis(tr("Balances"));
  connect(CompBalance, SIGNAL(activated()), this, SLOT(slotCompBalance()));

  SCuentas = new QAction(tr("Sustituir Cuentas"), tr("&Sustituir Cuentas"), 0, this);
  SCuentas->setStatusTip(tr("Sustituir Cuentas"));
  SCuentas->setWhatsThis(tr("Sustituir Cuentas"));
  connect(SCuentas, SIGNAL(activated()), this, SLOT(slotSCuentas()));

  CanalDef = new QAction(tr("Canal por Defecto"), tr("&Canal por Defecto"), 0, this);
  CanalDef->setStatusTip(tr("Canal por Defecto"));
  CanalDef->setWhatsThis(tr("Canal por Defecto"));
  connect(CanalDef, SIGNAL(activated()), this, SLOT(slotCanalDef()));
  
  CCosteDef = new QAction(tr("Centro de Coste por Defecto"), tr("&Centro de Coste por Defecto"), 0, this);
  CCosteDef->setStatusTip(tr("Centro de Coste por Defecto"));
  CCosteDef->setWhatsThis(tr("Centro de Coste por Defecto"));
  connect(CCosteDef, SIGNAL(activated()), this, SLOT(slotCCosteDef()));
      
  SelectorAction = new QAction( this, "SelectorAction" );
  SelectorAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "tux1.png" ) ) );
  connect( SelectorAction, SIGNAL( activated() ), this, SLOT( mostrar_selector() ) );      

}


void Bulmages01::initMenuBar() {
  ///////////////////////////////////////////////////////////////////
  // menuBar entry editMenu
  pEditMenu=new QPopupMenu();
  editUndo->addTo(pEditMenu);
  pEditMenu->insertSeparator();
  editCut->addTo(pEditMenu);
  editCopy->addTo(pEditMenu);
  editPaste->addTo(pEditMenu);
  ///////////////////////////////////////////////////////////////////
  // menuBar entry viewMenu
  pViewMenu=new QPopupMenu();
  pViewMenu->setCheckable(true);
  viewToolBar->addTo(pViewMenu);
  viewStatusBar->addTo(pViewMenu);
  viewFullScreen->addTo(pViewMenu);
  viewCorrector->addTo(pViewMenu);
  ///////////////////////////////////////////////////////////////////
  // EDIT YOUR APPLICATION SPECIFIC MENUENTRIES HERE
  // El menu de cuenta
//  pCuentaMenu = new QPopupMenu();
//  pCuentaMenu->setCheckable(true);
//  planCuentas->addTo(pCuentaMenu);
  
  // El menu de apunte
  pApunteMenu = new QPopupMenu();
  pApunteMenu->setCheckable(true);
  planCuentas->addTo(pApunteMenu);
  introducirApuntes1->addTo(pApunteMenu);
  asientos->addTo(pApunteMenu);
  asientosInteligentes->addTo(pApunteMenu);
  libroMayor->addTo(pApunteMenu);
  libroDiario->addTo(pApunteMenu);
  librobalance->addTo(pApunteMenu);
  librobalancetree->addTo(pApunteMenu);
  centrosCoste->addTo(pApunteMenu);
  canalesMenu->addTo(pApunteMenu);

  // El menu asiento
  pAsientoMenu = new QPopupMenu();
  pAsientoMenu->setCheckable(true);
  Abrirasientos->addTo(pAsientoMenu);
  Ordenarasientos->addTo(pAsientoMenu);
  pAsientoMenu->insertSeparator();
  abreEmpresa->addTo(pAsientoMenu);
  regularizaEmpresa->addTo(pAsientoMenu);
  cierraEmpresa->addTo(pAsientoMenu);
  
  
  
  // El menu Listados
  pListMenu = new QPopupMenu();  
//  perdygan->addTo(pListMenu);
//  balan->addTo(pListMenu);
//  pListMenu->insertSeparator();
  regiva->addTo(pListMenu);
  CompBalance->addTo(pListMenu);
  MPatrimoniales->addTo(pListMenu);
  pListMenu->insertSeparator();
  Modelo347->addTo(pListMenu);

  // El menu de estadísticas
  pEstMenu = new QPopupMenu();
  Segcuentas->addTo(pEstMenu);
  BGrafico->addTo(pEstMenu);

  // El menu de Herramientas
  pHerramientasMenu = new QPopupMenu();
  //usuarioaction->addTo(pHerramientasMenu);
  amortiz->addTo(pHerramientasMenu);
  SCuentas->addTo(pHerramientasMenu);
  bloqaction->addTo(pHerramientasMenu);
  pHerramientasMenu->insertSeparator();
  CCosteDef->addTo(pHerramientasMenu);
  CanalDef->addTo(pHerramientasMenu);
   
  //El menu de empresa
  pEmpresaMenu = new QPopupMenu();
  pEmpresaMenu->setCheckable(true);
  propiedadesEmpresa->addTo(pEmpresaMenu);
  pEmpresaMenu->insertSeparator();
  menuEjercicios =new QPopupMenu();
  pEmpresaMenu->insertItem(tr("Ejercicios"),menuEjercicios);

  QActionGroup * anys= new QActionGroup(this);
  connect(anys, SIGNAL(selected(QAction *)), this, SLOT(setCurrentEjercicio(QAction *)));
  QAction * a;
  QString aux;
  postgresiface2 *DBconn = new postgresiface2();
  DBconn->inicializa( DBName.ascii() );
  DBconn->begin();
  QString query;
  query.sprintf("SELECT ejercicio FROM ejercicios WHERE periodo=0");
  cursor2 * curEjer = DBconn->cargacursor(query,"curEjer");
  DBconn->commit();
  while (!curEjer->eof()) {
      aux=curEjer->valor(0);
     // a = new QAction(aux,0,anys,aux);
//      a = new QAction(this,0);
//      a->setToggleAction(true);
//=======
      a = new QAction(aux,aux,0,this);
      connect(a, SIGNAL(activated()),this, SLOT(setCurrentEjercicio(a)));
      connect(a, SIGNAL(activated()),this, SLOT(setCurrentEjercicio()));
//      a->setToggleAction(true);
      a->addTo(menuEjercicios);
      curEjer->siguienteregistro();
  }
  delete curEjer;
  delete DBconn;
  //nuevaEmpresa->addTo(pEmpresaMenu);
  //borrarEmpresa->addTo(pEmpresaMenu);
  //cambiarEmpresa->addTo(pEmpresaMenu);
  pHerramientasMenu->insertSeparator();
  //guardarEmpresa->addTo(pEmpresaMenu);
  //cargarEmpresa->addTo(pEmpresaMenu);
  //NEjercicio->addTo(pEmpresaMenu);
  
  ///////////////////////////////////////////////////////////////////
  // menuBar entry windowMenu
//  pWindowMenu = new QPopupMenu(this);
//  pWindowMenu->setCheckable(true);
//  connect(pWindowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));
  ///////////////////////////////////////////////////////////////////
  // menuBar entry helpMenu
  pHelpMenu=new QPopupMenu();
  ayuda->addTo(pHelpMenu);
  ayudai->addTo(pHelpMenu);
  helpAboutApp->addTo(pHelpMenu);
  pHelpMenu->insertSeparator();
  pHelpMenu->insertItem(tr("Que es ??"), this, SLOT(whatsThis()), SHIFT+Key_F1);

  menuBar()->insertItem(tr("&Empresa"),pEmpresaMenu);
  menuBar()->insertItem(tr("&Asiento"),pAsientoMenu);
//  menuBar()->insertItem(tr("&Cuenta"), pCuentaMenu);
//  menuBar()->insertItem(tr("&Apunte"), pApunteMenu);
  menuBar()->insertItem(tr("&Listados"),pListMenu);
  menuBar()->insertItem(tr("&Estadistica"),pEstMenu);
  menuBar()->insertItem(tr("&Ventana"), pApunteMenu);
  menuBar()->insertItem(tr("&Herramientas"), pHerramientasMenu);
  
  menuBar()->insertItem(tr("&Ver"), pViewMenu);
  menuBar()->insertItem(tr("&Ayuda"), pHelpMenu);
//  menuBar()->insertItem(tr("&Salir"), pFileMenu);
}


// TOOLBAR
void Bulmages01::initToolBar() {
  fileToolbar = new QToolBar(this, "Empresa Operaciones");
  fileToolbar->setLabel("Ventanas");
  SelectorAction->addTo( fileToolbar );
  propiedadesEmpresa->addTo(fileToolbar);
  fileToolbar->addSeparator();
  //usuarioaction->addTo(fileToolbar);
  //fileToolbar->addSeparator();
  planCuentas->addTo(fileToolbar);
  fileToolbar->addSeparator();
  introducirApuntes1->addTo(fileToolbar);
  libroMayor->addTo(fileToolbar);
  libroDiario->addTo(fileToolbar);
  librobalance->addTo(fileToolbar);
  librobalancetree->addTo(fileToolbar);
  asientos->addTo(fileToolbar);
  centrosCoste->addTo(fileToolbar);
  canalesMenu->addTo(fileToolbar);
  asientosInteligentes->addTo(fileToolbar);
//  fileToolbar->addSeparator();

  fileToolbar->addSeparator();
  QWhatsThis::whatsThisButton(fileToolbar);
  ayuda->addTo(fileToolbar);

  navegacionToolbar = new QToolBar(this, "Opciones de navegacion");
  navegacionToolbar->setLabel("Navegacion");
  startaction->addTo(navegacionToolbar);
  backaction->addTo(navegacionToolbar);
  forwardaction->addTo(navegacionToolbar);
  finishaction->addTo(navegacionToolbar);
  navegacionToolbar->addSeparator();
  printaction->addTo(navegacionToolbar);
  saveaction->addTo(navegacionToolbar);
  navegacionToolbar->addSeparator();
  reloadaction->addTo(navegacionToolbar);
  navegacionToolbar->addSeparator();
  filtro->addTo(navegacionToolbar);
  
  
  
  // Vamos a probar con un docwindow.

  doc  = new  QDockWindow(this,"Corrector");
  doc->setGeometry(100,100,100,500);
  doc->setFixedExtentWidth(200);
  correctorwidget *corr = new correctorwidget(doc,"correctorw");
  doc->setWidget(corr);
//  doc->setCloseMode(QDockWindow::Always);
  doc->setResizeEnabled(TRUE);
  doc->setMovingEnabled(TRUE);
  moveDockWindow(doc,Left);
  doc->hide();
  corr->setEmpresa(&empresaactual);
  
}// end initToolBar


void Bulmages01::initStatusBar() {
  statusBar()->message(tr("Listo."));
}// end initSatusBar


void Bulmages01::initView() {
  QPixmap fondo;
  view_back = new QVBox( this);
  view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
  pWorkspace = new QWorkspace( view_back );
  setCentralWidget(view_back);
  fondo.load(confpr->valor(CONF_BACKGROUND).c_str());
//  pWorkspace->setBackgroundPixmap (QPixmap(logofx));
  pWorkspace->setBackgroundPixmap (fondo);
}// end initView


void Bulmages01::showView() {
  empresaactual.inicializa1(pWorkspace);
  showMaximized();
  empresaactual.maximiza();
}// end initView


bool Bulmages01::eventFilter(QObject* object, QEvent* event)  {
  return QWidget::eventFilter( object, event );    // standard event processing
}// end eventFilter


/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void Bulmages01::slotEditUndo()  {
  statusBar()->message(tr("Reverting last action..."));
  statusBar()->message(tr("Ready."));
}//end slotEditUndo

void Bulmages01::slotAmortizaciones() {
	empresaactual.amortizaciones();
}// end amortizaciones


void Bulmages01::slotBloqFechas() {
(new BbloqFecha(&DBName, this))->exec();
}

void Bulmages01::slotEditCut()  {
  statusBar()->message(tr("Cutting selection..."));
  statusBar()->message(tr("Ready."));
}// end slotEditCut


void Bulmages01::slotEditCopy()  {
  statusBar()->message(tr("Copying selection to clipboard..."));
  statusBar()->message(tr("Ready."));
}//end slotEditCopy


void Bulmages01::slotEditPaste()  {
  statusBar()->message(tr("Inserting clipboard contents..."));
  statusBar()->message(tr("Ready."));
}// end slotEditPaste


void Bulmages01::slotViewToolBar(bool toggle)  {
  statusBar()->message(tr("Toggle toolbar..."));
  if (toggle== false) {
    fileToolbar->hide();
    navegacionToolbar->hide();
  } else {
    fileToolbar->show();
    navegacionToolbar->show();
  }// end if
 statusBar()->message(tr("Listo."));
}// end slotViewToolBar


///////////////////////////////////////////////////////////////////
//turn Statusbar on or off                                       //
///////////////////////////////////////////////////////////////////
void Bulmages01::slotViewStatusBar(bool toggle)  {
  statusBar()->message(tr("Toggle statusbar..."));
  if (toggle == false) {
    statusBar()->hide();
  } else {
    statusBar()->show();
  }// end if
  statusBar()->message(tr("Ready."));
}// end slotViewStatusBar


///////////////////////////////////////////////////////////////////
//turn Statusbar on or off                                       //
///////////////////////////////////////////////////////////////////
void Bulmages01::slotViewFullScreen(bool toggle)  {
  statusBar()->message(tr("Toggle full screen mode..."));
  if (toggle == false) {
    showNormal();
  } else {
    showFullScreen();
  }// end if
  statusBar()->message(tr("Ready."));
}// end slotViewStatusBar



///////////////////////////////////////////////////////////////////
//turn Statusbar on or off                                       //
///////////////////////////////////////////////////////////////////
void Bulmages01::slotViewCorrector(bool toggle)  {
  statusBar()->message(tr("Ver Corrector..."));
  if (toggle == false) {
    doc->hide();
  } else {
    doc->show();
  }// end if
  statusBar()->message(tr("Ready."));
}// end slotViewStatusBar


void Bulmages01::slotHelpAbout()  {
   aboutview *about=new aboutview(0,0);
   about->exec();
   delete about;
}// end slotHelpAbout


void Bulmages01::slotStatusHelpMsg(const QString &text) {
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  statusBar()->message(text, 2000);
}// end slotStatusHelpMsg


void Bulmages01::slotPlanCuentas()  {
  statusBar()->message(tr("Plan de Cuentas"));
  empresaactual.muestracuentas();
}// end slotPlanCuentas


void Bulmages01::slotGruposContables()  {
  statusBar()->message(tr("Grupos Contables"));
}// end slotGruposContables


void Bulmages01::slotListadoCuentas()  {
  statusBar()->message(tr("Listado de Cuentas"));
}// end slotListadoCuentas


void Bulmages01::slotAsientosInteligentes()  {
  statusBar()->message(tr("Asientos Inteligentes"));
  empresaactual.ainteligentes();
}// end slotAsientosInteligentes


void Bulmages01::slotIntroducirApuntes()  {
  statusBar()->message(tr("Introducción de Apuntes"));
  empresaactual.muestraapuntes();
}// end slotIntroducirApuntes

void Bulmages01::slotIntroducirApuntes1()  {
  statusBar()->message(tr("Introducción de Borrador"));
  empresaactual.muestraapuntes1();
}// end slotIntroducirApuntes1

void Bulmages01::slotListarApuntes() {
      empresaactual.muestraasientos();
}// end slotListarApuntes

void Bulmages01::slotAsientos()  {
      empresaactual.muestraasientos();
}// end slotAsientos

void Bulmages01::slotCentrosCoste() {
      empresaactual.ccostes();
}

void Bulmages01::slotCanales() {
    empresaactual.canales();
}

void Bulmages01::slotMPatrimoniales() {
    empresaactual.mpatrimoniales();
}

void Bulmages01::slotPropiedadesEmpresa()  {
  statusBar()->message(tr("Propiedades Empresa"));
  empresaactual.propiedadempresa();
}// end slotPropiedadesEmpresa

void Bulmages01::setCurrentEjercicio(QAction *a)  {
fprintf(stderr,"Cambio de ejercicio\n");
setCaption(tr("BulmaGés ") + a->text());
empresaactual.setejactual(a->text());
empresaactual.cambioejercicio();
}// end setCurrentEjercicio

void Bulmages01::setCurrentEjercicio()  {
QAction *a= (QAction *)sender();
fprintf(stderr,"Cambio de ejercicio\n");
setCaption(tr("BulmaGés ") + a->text());
empresaactual.setejactual(a->text());
empresaactual.cambioejercicio();
}// end setCurrentEjercicio

void Bulmages01::slotLibroMayor()
{
  statusBar()->message(tr("Libro Mayor"));
  empresaactual.libromayor();
}

void Bulmages01::slotLibroDiario()  {
  statusBar()->message(tr("Libro Diario"));
  empresaactual.librodiario();
}// end slotLibroDiario


void Bulmages01::slotSiguiente() {
  empresaactual.boton_siguiente();
}// end slotSiguiente


void Bulmages01::slotCompBalance() {
  empresaactual.compbalance();
}// end slotCompBalance


void Bulmages01::slotAnterior() {
  empresaactual.boton_anterior();
}//end slotAnterior

void Bulmages01::slotUltimo() {
  empresaactual.boton_ultimo();
}// end slotUltimo

void Bulmages01::slotPrimero() {
  empresaactual.boton_primero();
}// end slotPrimero

void Bulmages01::slotPerdidas() {
}// end slotPerdidas

void Bulmages01::slot347() {
empresaactual.modelo347();
}// end slot347

void Bulmages01::slotBalan() {
}// end slotBalan

void Bulmages01::slotRegistroIva() {
   empresaactual.registroiva();
}// end slotRegistroIva


void Bulmages01::slotReload() {
   empresaactual.boton_reload();
}// end slotReload


void Bulmages01::slotBalance() {
   empresaactual.librobalance();
}// end slotBalance

void Bulmages01::slotBalanceTree() {
   empresaactual.librobalancetree();
}// end slotBalance

void Bulmages01::slotGuardar() {
  empresaactual.boton_guardar();
}// end slotGuardar


void Bulmages01::slotcierraEmpresa() {
  empresaactual.cierraempresa();
}// end slotGuardar


void Bulmages01::slotabreEmpresa() {
  empresaactual.abreempresa();
}// end slotabreEmpresa

void Bulmages01::slotregularizaEmpresa() {
  empresaactual.regularizaempresa();
}// end slotregularizaEmpresa


void Bulmages01::slotImprimir() {
  empresaactual.boton_imprimir();
}// end slotImprimir



void Bulmages01::slotAbrirasientos() {
  empresaactual.Abrirasientos();
}// end slotAbrirasientos

void Bulmages01::slotOrdenarasientos() {
  empresaactual.Ordenarasientos(empresaactual.ejercicioactual().toInt());
}// end slotOrdenarasientos


void Bulmages01::slotSegCuentas() {
  empresaactual.SegCuentas();
}// end slotOrdenarasientos

void Bulmages01::slotBalanceGrafico() {
  empresaactual.BalanceGrafico();
}// end slotOrdenarasientos

void Bulmages01::slotFiltro() {
   empresaactual.Filtro();
}// end slotFiltro


void Bulmages01::windowMenuAboutToShow()  {
/*
  pWindowMenu->clear();	
  windowNewWindow->addTo(pWindowMenu);
  windowCascade->addTo(pWindowMenu);
  windowTile->addTo(pWindowMenu);
  if ( pWorkspace->windowList().isEmpty() ) {
    windowAction->setEnabled(false);
  } else {
    windowAction->setEnabled(true);
  }// end if
  pWindowMenu->insertSeparator();
  QWidgetList windows = pWorkspace->windowList();
  for ( int i = 0; i < int(windows.count()); ++i )  {
    int id = pWindowMenu->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(), this, SLOT( windowMenuActivated( int ) ) );
    pWindowMenu->setItemParameter( id, i );
    pWindowMenu->setItemChecked( id, pWorkspace->activeWindow() == windows.at(i) );
  }// end for

*/
}// end windowMenuAboutToShow


void Bulmages01::slotWindowNewWindow() {
}// end slotWindowNewWindow


void Bulmages01::slotAyuda() {
   actualizacionesview *act = new actualizacionesview(0,0);
   act->exec();
   delete act;
}// end slotAyuda


void Bulmages01::slotAyudai() {
   char *args[]={"http://www.conetxia.com/bulmages/manualindex.php?version=0.4.3","http://www.conetxia.com/bulmages/manualindex.php?version=0.4.3",NULL};
   int pid;
   int error;
#ifndef WIN32
   if ((pid=fork()) < 0) {
       perror ("Fork failed");
       exit(errno);
   }// end if
   if (!pid) {
      error = execvp(confpr->valor(CONF_NAVEGADOR).c_str(),args);
   }// end if
#endif
}// end slotAyudai

void Bulmages01::slotSCuentas() {
   empresaactual.reemplazacuentaenasientos();
}// end slotNEjercicio


void Bulmages01::slotCCosteDef() {
   empresaactual.centrocostedefecto();
}// end slotNEjercicio

void Bulmages01::slotCanalDef() {
   empresaactual.canaldefecto();
}// end slotNEjercicio

void  Bulmages01::mostrar_selector() {
}

