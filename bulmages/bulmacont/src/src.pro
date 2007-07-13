# Archivo generado por el gestor qmake de kdevelop.
# -------------------------------------------------

exists(/usr/bin/distcc) {
    QMAKE_CXX = distcc g++
}

LANGUAGE = C++

QMALE_LIBDIR += ../../installbulmages

INCLUDEPATH += ../../bulmalib/src \
               images \
               /usr/include/qt4 \
               /usr/include/qt4/Qt \
               ../../bulmalib/src/.ui \
               /usr/include/QtXml

LIBPATH += ../../installbulmages

CONFIG += release warn_on assistant debug

LIBS += -rdynamic \
	-lbulmalib


TEMPLATE = app

TARGET = ../../installbulmages/bulmacont

FORMS += bulmacontbase.ui \
	 diariobase.ui \
	 extractobase.ui \
	 extractoprintbase.ui \
	 diarioprintbase.ui \
	 asientobase.ui \
	 asientolistbase.ui \
	 balancebase.ui \
	 balancetreebase.ui \
	 cuentabase.ui \
	 listcuentasbase.ui \
	 sobrebase.ui \
	 amortizacionesbase.ui \
	 amortizacionbase.ui \
	 busquedacuentabase.ui \
	 aplinteligentesbase.ui \
	 canualesbase.ui \
	 ccostebase.ui \
	 canalbase.ui \
	 fpagobase.ui \
	 tipoivabase.ui \
	 bbloqfechabase.ui \
	 duplicarasientobase.ui \
	 cambiactabase.ui \
	 selectcanalbase.ui \
	 selectccostebase.ui \
	 propiedadesempresabase.ui \
	 canualesprintbase.ui \
	 regivaprintbase.ui \
	 balanceprintbase.ui \
	 mpatrimonialesbase.ui \
	 mpatrimonialbase.ui \
	 listado347base.ui \
	 modelo300base.ui \
	 paisbase.ui

#FORMS3 +=  mod300dlg.ui 

TRANSLATIONS += bulmacont_es.ts \
                bulmacont_ca.ts \
                bulmacont_en.ts \
                bulmacont_fr.ts \
                bulmacont_de.ts \
		bulmacont_zz.ts

HEADERS += splashscreen.h \
           bulmacont.h \
           sobreview.h \
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
           asientosview.h \
           regivaprintview.h \
           mpatrimonialview.h \
           mpatrimonialesview.h \
           balanceprintview.h \
           canualesprintview.h \
           canualesview.h \
           canalview.h \
           ccosteview.h \
           propiedadesempresa.h \
           extractoprintview.h \
           libromayorprint.h \
           diarioprint.h \
           diarioprintview.h \
           cambiactaview.h \
           duplicarasientoview.h \
           amortizacionview.h \
           listado347.h \
           amortizacionesview.h \
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
	   busquedaccoste.h \
	   busquedacanal.h \
	   fichabc.h \
	   paisview.h

SOURCES += main.cpp \
           splashscreen.cpp \
           bulmacont.cpp \
           sobreview.cpp \
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
           asientosview.cpp \
           regivaprintview.cpp \
           mpatrimonialview.cpp \
           mpatrimonialesview.cpp \
           balanceprintview.cpp \
           canualesprintview.cpp \
           canualesview.cpp \
           canalview.cpp \
           ccosteview.cpp \
           propiedadesempresa.cpp \
           extractoprintview.cpp \
           libromayorprint.cpp \
           diarioprint.cpp \
           diarioprintview.cpp \
           cambiactaview.cpp \
           duplicarasientoview.cpp \
           amortizacionview.cpp \
           listado347.cpp \
           amortizacionesview.cpp \
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
	   busquedaccoste.cpp \
	   busquedacanal.cpp \
	   fichabc.cpp \
	   paisview.cpp

unix{
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    LIBS += -lpq

    INCLUDEPATH += ../../bulmalib/src \
  		   ../../bulmalib/src/.ui \
		   ../../bulmalib/src/.moc \
	   	   /usr/include/qt4 \
		   /usr/include/qt4/Qt

    bulmages_install_script.path = .
    bulmages_install_script.extra = echo; echo "**************************"; echo Para Instalar BULMAGES ejecute el script de instalacion; echo "installbulmages"; echo que encontrara en la carpeta installbulmages.; echo Gracias.; echo "(El equipo de Bulmages)"; echo "**************************"; echo;

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

    LIBS += ../../../bulmalib/src/release/libbulmalib0.a \
     	    C:\Qt\4.1.1\lib\libQtXml4.a \
#	    C:\Qt\4.1.1\lib\libQtXmld4.a 

    INCLUDEPATH += ../../bulmalib/src/libpq \
                   ../src \
		   .. \
 		   C:\Qt\4.1.1\include\QtXml


    INCLUDEPATH += "C:\Archivos de programa\PostgreSQL\8.1\include"
    LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"

    TARGET = ../../../installbulmages/bulmacont
}

QT += xml  

