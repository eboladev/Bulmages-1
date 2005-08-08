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

// application specific includes
#include "bulmages01.h"
#include "configuracion.h"
#include "aboutview.h"
#include "actualizacionesview.h"

// Inclusiones para el log
#include "log.h"

Bulmages01::Bulmages01(QWidget * parent, const char * name, WFlags f, QString * DB)
        : BulmaContBase(parent,name,f) {
    if (DB != NULL) {
        DBName=*DB;
    } else {
        DBName= "";
        while (DBName == "")
            DBName = empresaactual.searchCompany();
    }// end if
    setCaption(tr("BulmaGés:  ") + DBName +" --");
    initView();
    initActions();
    initMenuBar();
    initToolBar();
    initStatusBar();
    showView();

    ctllog->setempresa(&empresaactual);
    ctllog->add
    (LOG_SEG | LOG_TRA, 1,"BmgCtt001" , "El usuario ha entrado en bulmacont");
}// end Bulmages01


Bulmages01::~Bulmages01() {
    //En el constructor asignamos un puntero a empresa { ctllog->setempresa(&empresaactual); }
    //Al destruir la clase empresa no podemos seguir apuntando a un objeto que ya no existe.
    //es necesario eliminar la referencia a este objeto, por ejemplo aqu�
    //justo antes logeo la salida del usuario (Ren�
    ctllog->add
    (LOG_SEG | LOG_TRA, 1,"BmgDtt017","El usuario sale de bulmacont." );
    ctllog->setempresa(NULL);
}


void Bulmages01::initActions() {
}


void Bulmages01::initMenuBar() {
    Ayuda->insertItem(tr("Que es ??"), this, SLOT(whatsThis()), SHIFT+Key_F1);
}


// TOOLBAR
void Bulmages01::initToolBar() {
    QWhatsThis::whatsThisButton(fileToolbar);
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
    fondo.load(confpr->valor(CONF_BACKGROUND).ascii());
    pWorkspace->setBackgroundPixmap (fondo);
}// end initView


void Bulmages01::showView() {
    empresaactual.inicializa1(DBName, pWorkspace);
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
        empresaToolbar->hide();
        ventToolbar->hide();
        navegacionToolbar->hide();
        gestionToolbar->hide();
    } else {
        fileToolbar->show();
        empresaToolbar->show();
        ventToolbar->show();
        navegacionToolbar->show();
        gestionToolbar->show();
    }// end if
    statusBar()->message(tr("Listo."));
}// end slotViewToolBar

void Bulmages01::tile() {
	pWorkspace->tile();
}// end tile

void Bulmages01::cascade() {
	pWorkspace->cascade();
}// end tile


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
    statusBar()->message(tr("Introducci� de Apuntes"));
    empresaactual.muestraapuntes();
}// end slotIntroducirApuntes

void Bulmages01::slotIntroducirApuntes1()  {
    statusBar()->message(tr("Introducci� de Borrador"));
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


void Bulmages01::slotLibroMayor() {
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

void Bulmages01::slotPerdidas() {}// end slotPerdidas

void Bulmages01::slot347() {
    empresaactual.modelo347();
}// end slot347

void Bulmages01::slotBalan() {}// end slotBalan

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
    empresaactual.Ordenarasientos();
}// end slotOrdenarasientos



void Bulmages01::slotCobPag() {
    empresaactual.cobPag();
}// end cobPag


void Bulmages01::slotFiltro() {
    empresaactual.Filtro();
}// end slotFiltro

void Bulmages01::windowMenuAboutToShow()  {}// end windowMenuAboutToShow


void Bulmages01::slotWindowNewWindow() {}// end slotWindowNewWindow


void Bulmages01::slotAyuda() {
    actualizacionesview *act = new actualizacionesview(0,0);
    act->exec();
    delete act;
}// end slotAyuda

void Bulmages01::slotSCuentas() {
    empresaactual.reemplazacuentaenasientos();
}// end slotNEjercicio


void Bulmages01::slotRecSald() {
    // empresaactual.recalculasaldos();
}

void Bulmages01::slotCCosteDef() {
    empresaactual.centrocostedefecto();
}// end slotNEjercicio

void Bulmages01::slotCanalDef() {
    empresaactual.canaldefecto();
}// end slotNEjercicio


void Bulmages01::slotTiposIVA() {
    empresaactual.tiposIVA();
}


void Bulmages01::slotFPago() {
    empresaactual.fPago();
}



