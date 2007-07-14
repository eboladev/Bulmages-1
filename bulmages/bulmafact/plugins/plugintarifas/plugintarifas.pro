# Archivo generado por el gestor qmake de kdevelop.
# Modificado por Tomeu Borras.
# -------------------------------------------------

TEMPLATE = lib

CONFIG += release \
          plugin \
          assistant

LIBS +=	 -rdynamic \
	 -lbulmalib

QMAKE_LIBDIR += ../../../installbulmages

VERSION = 1.0.0

unix{
    TARGET = ../../../installbulmages/plugins/tarifasbf

    INCLUDEPATH = ../../../bulmalib/src \
		  .. \
		  ../../src/.ui \
		  /usr/include/qt4 \
		  /usr/include/qt4/Qt \
		  /usr/include/qt4/QtXml \
		  ../../src \
		  ../../../bulmalib/src/.ui \
		  /usr/include/Qt \
		  /usr/include/QtXml

    UI_DIR = .ui

    MOC_DIR = .moc

    OBJECTS_DIR = .obj

    bulmages_install_script.path = .
    bulmages_install_script.extra = echo; echo "**************************"; echo Para Instalar BULMAGES ejecute el script de instalaci�; echo "installbulmages"; echo que encontrara en la carpeta installbulmages.; echo Gracias.; echo "(El equipo de Bulmages)"; echo "**************************"; echo;
    exists(/usr/include/postgresql/libpq-fe.h){
	DEFINES += DISTRO_DEBIAN
        DEFINES += QT_THREAD_SUPPORT
    }
    exists(/usr/include/postgresql/8.0/libpq-fe.h){
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

windows {
    DEFINES += WINDOWS
    UI_DIR=.ui
    MOC_DIR=.moc
    OBJECTS_DIR = .obj
    LIBS += ../../../bulmalib/src/release/libbulmalib.a \
	    C:\Qt\4.1.0\lib\libQtXml4.a \
	    C:\Qt\4.1.0\lib\libQtXmld4.a 

    INCLUDEPATH += .. \
    	           ..\.ui \
		   ..\..\src \
		   ..\.. \
		   ..\..\.. \
		   ..\..\..\bulmalib\src \
		   ..\..\..\bulmalib\src\.ui \
		   C:\Qt\4.1.0\include\QtXml \
		   "C:\Archivos de programa\PostgreSQL\8.1\include"

    LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"
}

FORMS = tarifalistbase.ui \
	tarifabase.ui

SOURCES = busquedatarifa.cpp \
	  plugintarifas.cpp \
	  listltarifaview.cpp \
	  tarifalistsubform.cpp \
	  tarifalistview.cpp \
	  tarifaview.cpp

HEADERS = busquedatarifa.h \
	  plugintarifas.h \
	  listltarifaview.h \
	  tarifalistsubform.h \
          tarifalistview.h \
	  tarifaview.h
