# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./fuentes/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmacont

QT += qt3support

#QMAKE_CLEAN = Makefile

LANGUAGE = C++

# TARGETDEPS += ../../../bulmalib/libbulmalib.a

RESOURCES += ../../../bulmages-recursos-graficos.qrc


INCLUDEPATH += ../../../bulmalib \
images \
/usr/include/qt4 \
/usr/include/qt4/Qt \
../../../bulmalib/.ui \
/usr/include/QtXml

CONFIG += release 

#warn_on \
#qt4
#rdynamic

LIBS += -rdynamic \
	-lbulmalib


TEMPLATE = app

TARGET = ../../../installbulmages/bulmacont

FORMS += bulmacontbase.ui

FORMS3 += uiselector.ui \
         about.ui \
         listcuentasdlg1.ui \
         cuentadlg.ui \
         diariodlg1.ui \
         filtrardiariodlg.ui \
         extractodlg1.ui \
         balance1dlg.ui \
         balancedlg.ui \
         int_apunts3.ui \
         asientodlg.ui \
         aplinteligentesdlg.ui \
         filtrarasientosdlg.ui \
         asientosdlg.ui \
#         listivadlg.ui \
         regivaprintdlg.ui \
#         ivadlg.ui \
         compbalancedlg.ui \
         mpatrimonialdlg.ui \
         mpatrimonialesdlg.ui \
         balanceprintdlg.ui \
         balancesprintdlg.ui \
         balancesdlg.ui \
         canaldlg.ui \
         ccostedlg.ui \
         ainteligentesdlg.ui \
         propiedemp.ui \
         filtrarextractosdlg.ui \
         extractoprintdlg.ui \
         actualizacionesdlg.ui \
         diarioprintdlg.ui \
         cambiactadlg.ui \
         duplicaasientodlg.ui \
         amortizaciondlg.ui \
         uimodelo347.ui \
         amortizacionesdlg.ui \
         uivisorempresas.ui \
         uibloqfecha.ui \
         selectccostedlg.ui \
         selectcanaldlg.ui \
         mod300dlg.ui \
#         cobropagodlg.ui \
         tipoivadlg.ui \
         fpagodlg.ui \
	 busquedacuentabase.ui \
	 asiento1dlg.ui

TRANSLATIONS += bulmages_es.ts \
                bulmages_ca.ts \
                bulmages_en.ts \
                bulmages_fr.ts \
                bulmages_de.ts 

IMAGES += images/kate.png \
          images/kfind.png \
          images/clock.png \
          images/hwinfo.png \
          images/kdmconfig.png \
          images/exit.png \
          images/tux.png \
          images/package_settings.png \
          images/password.png \
          images/ainteligente.xpm \
          images/asiento.xpm \
          images/back.png \
          images/back.xpm \
          images/balancetree.xpm \
          images/balance.xpm \
          images/bulmages.png \
          images/bulmages.xpm \
          images/cactivo.xpm \
          images/canales.xpm \
          images/cargaemp.xpm \
          images/casacion.xpm \
          images/ccoste.xpm \
          images/cingresos.xpm \
          images/cneto.xpm \
          images/configuracion.xpm \
          images/cpasivo.xpm \
          images/cuentas.xpm \
          images/diario.xpm \
          images/empresa.xpm \
          images/estadisticas.xpm \
          images/filenew.xpm \
          images/fileopen.xpm \
          images/filesave.png \
          images/filesave.xpm \
          images/filter.png \
          images/filter.xpm \
          images/find.png \
          images/find.xpm \
          images/finish.png \
          images/finish.xpm \
          images/forward.png \
          images/forward.xpm \
          images/guardaemp.xpm \
          images/help.png \
          images/help.xpm \
          images/igualant.xpm \
          images/iva.xpm \
          images/lapizp.xpm \
          images/lapiz.xpm \
          images/lo16-app-bulmages01.png \
          images/lo32-app-bulmages01.png \
          images/logofx.xpm \
          images/mayor.xpm \
          images/plancuentas.xpm \
          images/print.xpm \
          images/reload.png \
          images/reload.xpm \
          images/save.xpm \
          images/start.png \
          images/start.xpm \
          images/usuario.xpm \
          images/1leftarrow.png \
          images/1rightarrow.png \
          images/2leftarrow.png \
          images/2rightarrow.png \
          images/fileopen_4.png \
          images/identity.png \
          images/editdelete.png \
          images/1downarrow.png \
          images/folder_new.png \
          images/fileprint.png \
          images/compfile.png \
          images/window_new.png \
          images/edittrash.png \
          images/package.png \
          images/kcmkwm.png \
          images/package_editors.png \
          images/kmenuedit.png \
          images/xclipboard.png \
          images/button_ok.png \
          images/globus.png \
          images/kcmsystem.png \
          images/plan.png \
          images/pda_blue.png \
          images/tux1.png \
          images/filetypes.png \
          images/edit.png 

HEADERS += splashscreen.h \
           bulmages01.h \
           aboutview.h \
           empresa.h \
           listcuentasview1.h \
           cuentaview.h \
           diarioview1.h \
           qlineedit1.h \
           filtrardiarioview.h \
           extractoview1.h \
           balance1view.h \
           qlistviewitem1.h \
           balanceview.h \
#           intapunts3view.h \
           asientoview.h \
           aplinteligentesview.h \
           filtrarasientosview.h \
           asientosview.h \
#           listivaview.h \
           regivaprintview.h \
#           ivaview.h \
           compbalanceview.h \
           mpatrimonialview.h \
           mpatrimonialesview.h \
           balanceprintview.h \
           balancesprintview.h \
           balancesview.h \
           canalview.h \
           ccosteview.h \
           ainteligentesview.h \
           propiedadesempresa.h \
           filtrarextractosview.h \
           extractoprintview.h \
           libromayorprint.h \
           diarioprint.h \
           actualizacionesview.h \
           diarioprintview.h \
           cambiactaview.h \
           duplicarasientoview.h \
           amortizacionview.h \
           bmodelo347.h \
           amortizacionesview.h \
           log.h \
           selectccosteview.h \
           selectcanalview.h \
           modelosps.h \
           modelo300.h \
           importainteligente.h \
           importbalance.h \
#           cobropagoview.h \
           bbloqfecha.h \
           tipoivaview.h \
           fpagoview.h \
           arbol.h \
	   busquedacuenta.h \
#	   listlinprevcobro.h \
#	   listlinprevcobroview.h \
#	   linprevcobro.h \
	   asiento1.h \
	   asiento1view.h \
	   linasiento1.h \
	   listlinasiento1.h \
	   listlinasiento1view.h

SOURCES += main.cpp \
           splashscreen.cpp \
           bulmages01.cpp \
           aboutview.cpp \
           empresa.cpp \
           listcuentasview1.cpp \
           cuentaview.cpp \
           diarioview1.cpp \
           qlineedit1.cpp \
           filtrardiarioview.cpp \
           extractoview1.cpp \
           balance1view.cpp \
           qlistviewitem1.cpp \
           balanceview.cpp \
#           intapunts3view.cpp \
           asientoview.cpp \
           aplinteligentesview.cpp \
           filtrarasientosview.cpp \
           asientosview.cpp \
#           listivaview.cpp \
           regivaprintview.cpp \
#           ivaview.cpp \
           compbalanceview.cpp \
           mpatrimonialview.cpp \
           mpatrimonialesview.cpp \
           balanceprintview.cpp \
           balancesprintview.cpp \
           balancesview.cpp \
           canalview.cpp \
           ccosteview.cpp \
           ainteligentesview.cpp \
           propiedadesempresa.cpp \
           filtrarextractosview.cpp \
           extractoprintview.cpp \
           libromayorprint.cpp \
           diarioprint.cpp \
           actualizacionesview.cpp \
           diarioprintview.cpp \
           cambiactaview.cpp \
           duplicarasientoview.cpp \
           amortizacionview.cpp \
           bmodelo347.cpp \
           amortizacionesview.cpp \
           log.cpp \
           selectccosteview.cpp \
           selectcanalview.cpp \
           modelosps.cpp \
           modelo300.cpp \
           importainteligente.cpp \
           importbalance.cpp \
#           cobropagoview.cpp \
           bbloqfecha.cpp \
           tipoivaview.cpp \
           fpagoview.cpp \
           arbol.cpp \
	   busquedacuenta.cpp \
#	   listlinprevcobro.cpp \
#	   listlinprevcobroview.cpp \
#	   linprevcobro.cpp \
	   asiento1.cpp \
	   asiento1view.cpp \
	   linasiento1.cpp \
	   listlinasiento1.cpp \
	   listlinasiento1view.cpp
	 
	 



unix{
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj

	LIBS += -lpq

INCLUDEPATH += ../../bulmalib \
		../../bulmalib/.ui \
		../../bulmalib/.moc \
		comun \
		/usr/include/qt4 \
		/usr/include/qt4/Qt


  bulmages_install_script.path = .
  bulmages_install_script.extra = echo; echo "**************************"; echo Para Instalar BULMAGES ejecute el 	script de instalaci?; echo "installbulmages"; echo que encontrara en la carpeta installbulmages.; echo Gracias.; echo "(El equipo de Bulmages)"; echo "**************************"; echo;

  exists(/usr/include/postgresql/libpq-fe.h){
    DEFINES += DISTRO_DEBIAN
    DEFINES += QT_THREAD_SUPPORT
  }

  exists(/usr/include/postgresql/8.0/libpq-fe.h) {
    DEFINES += DISTRO_DEBIAN_8_0
    DEFINES += QT_THREAD_SUPPORT
  }
 
  exists(/usr/include/pgsql/libpq-fe.h){
    DEFINES += DISTRO_RED_HAT
  }

  exists(/usr/include/postgresql/pgsql/libpq-fe.h){
    DEFINES += DISTRO_GENTOO
    DEFINES += QT_THREAD_SUPPORT
  }

  exists(/usr/include/no_se_que/pgsql/libpq-fe.h){
    DEFINES += DISTRO_NO_SE_QUE
  }
}

win32{
  DEFINES += WIN32
  DEFINES += WINDOWS

 UI_DIR = .ui

  MOC_DIR = .moc

  OBJECTS_DIR = .obj

  LIBS += ../../../bulmalib/release/libbulmalib.a \
 	C:\Qt\4.1.0\lib\libQtXml4.a \
	C:\Qt\4.1.0\lib\libQtXmld4.a 

  INCLUDEPATH += ../../../bulmalib/libpq \
                 ../contabilidad \
		 .. \
 		 C:\Qt\4.1.0\include\QtXml


  INCLUDEPATH += "C:\Archivos de programa\PostgreSQL\8.1\include"
  LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"

TARGET = ../../../../installbulmages/bulmacont
}

#The following line was inserted by qt3to4
QT += xml  
#The following line was inserted by qt3to4
CONFIG += uic3

