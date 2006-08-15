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

LIBPATH += ../../../installbulmages

CONFIG += release 

LIBS += -rdynamic \
	-lbulmalib


TEMPLATE = app

TARGET = ../../../installbulmages/bulmacont

FORMS += bulmacontbase.ui \
	 diariobase.ui \
	 extractobase.ui \
	 extractoprintbase.ui \
	 diarioprintbase.ui \
	 asientobase.ui \
	 asientolistbase.ui \
	 balancebase.ui \
	 balancetreebase.ui \
#	 ainteligentesbase.ui \
	 cuentabase.ui \
	 listcuentasbase.ui \
	 aboutbase.ui \
	 amortizacionesbase.ui \
	 amortizacionbase.ui \
	 busquedacuentabase.ui \
	 aplinteligentesbase.ui \
	 canualesbase.ui


FORMS3 += uiselector.ui \
#         about.ui \
#         listcuentasdlg1.ui \
#         cuentadlg.ui \
#         balance1dlg.ui \
         int_apunts3.ui \
         filtrarasientosdlg.ui \
         regivaprintdlg.ui \
         compbalancedlg.ui \
         mpatrimonialdlg.ui \
         mpatrimonialesdlg.ui \
         balanceprintdlg.ui \
         balancesprintdlg.ui \
#         balancesdlg.ui \
         canaldlg.ui \
         ccostedlg.ui \
         propiedemp.ui \
         actualizacionesdlg.ui \
         cambiactadlg.ui \
         duplicaasientodlg.ui \
#         amortizaciondlg.ui \
         uimodelo347.ui \
#         amortizacionesdlg.ui \
         uivisorempresas.ui \
         uibloqfecha.ui \
         selectccostedlg.ui \
         selectcanaldlg.ui \
         mod300dlg.ui \
#         cobropagodlg.ui \
         tipoivadlg.ui \
         fpagodlg.ui

TRANSLATIONS += bulmages_es.ts \
                bulmages_ca.ts \
                bulmages_en.ts \
                bulmages_fr.ts \
                bulmages_de.ts \
		bulmages_zz.ts

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
           diarioview.h \
           qlineedit1.h \
           extractoview1.h \
           balance1view.h \
           qlistviewitem1.h \
           balanceview.h \
           aplinteligentesview.h \
           filtrarasientosview.h \
           asientosview.h \
           regivaprintview.h \
           compbalanceview.h \
           mpatrimonialview.h \
           mpatrimonialesview.h \
           balanceprintview.h \
           balancesprintview.h \
           canualesview.h \
           canalview.h \
           ccosteview.h \
           propiedadesempresa.h \
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
           bbloqfecha.h \
           tipoivaview.h \
           fpagoview.h \
           arbol.h \
	   busquedacuenta.h \
	   asiento1.h \
	   asiento1view.h \
	   listlinasiento1view.h \
	   subform2bc.h \
	   diariosubform.h \
	   extractosubform.h \
	   asientolistsubform.h \
	   balancesubform.h \
	   busquedaccoste.h

SOURCES += main.cpp \
           splashscreen.cpp \
           bulmages01.cpp \
           aboutview.cpp \
           empresa.cpp \
           listcuentasview1.cpp \
           cuentaview.cpp \
           diarioview.cpp \
           qlineedit1.cpp \
           extractoview1.cpp \
           balance1view.cpp \
           qlistviewitem1.cpp \
           balanceview.cpp \
           aplinteligentesview.cpp \
           filtrarasientosview.cpp \
           asientosview.cpp \
           regivaprintview.cpp \
           compbalanceview.cpp \
           mpatrimonialview.cpp \
           mpatrimonialesview.cpp \
           balanceprintview.cpp \
           balancesprintview.cpp \
           canualesview.cpp \
           canalview.cpp \
           ccosteview.cpp \
           propiedadesempresa.cpp \
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
           bbloqfecha.cpp \
           tipoivaview.cpp \
           fpagoview.cpp \
           arbol.cpp \
	   busquedacuenta.cpp \
	   asiento1.cpp \
	   asiento1view.cpp \
	   listlinasiento1view.cpp \
	   subform2bc.cpp \
	   diariosubform.cpp \
	   extractosubform.cpp \
	   asientolistsubform.cpp \
	   balancesubform.cpp \
	   busquedaccoste.cpp
	 

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

  LIBS += ../../../bulmalib/release/libbulmalib0.a \
 	C:\Qt\4.1.1\lib\libQtXml4.a \
#	C:\Qt\4.1.1\lib\libQtXmld4.a 

  INCLUDEPATH += ../../../bulmalib/libpq \
                 ../contabilidad \
		 .. \
 		 C:\Qt\4.1.1\include\QtXml


  INCLUDEPATH += "C:\Archivos de programa\PostgreSQL\8.1\include"
  LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"

TARGET = ../../../../installbulmages/bulmacont
}

#The following line was inserted by qt3to4
QT += xml  
#The following line was inserted by qt3to4
CONFIG += uic3

