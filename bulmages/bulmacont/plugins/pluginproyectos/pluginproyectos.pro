# Archivo generado por el gestor qmake de kdevelop. 
# -------------------------------------------------


TEMPLATE = lib

CONFIG += release \
          plugin \
	  assistant

LIBS +=	 -rdynamic \
	 -lbulmalib

VERSION = 1.0.0

QMAKE_LIBDIR += ../../../installbulmages

unix{
    TARGET = ../../../installbulmages/plugins/pluginproyectos

    INCLUDEPATH += ../../../bulmalib/src \
		  .. \
		  ../../src/.ui \
		  /usr/include/qt4 \
		  /usr/include/qt4/Qt \
		  ../../src \
		  ../../../bulmalib/src/.ui \
		  /usr/include/qt4 \
		  /usr/include/qt4/Qt \
		  /usr/include/qt4/QtXml \
		  /usr/include/qt4/QtGui \
		  /usr/include/Qt \
		  /usr/include/QtXml \
		  /usr/lib/qt4/include/Qt \
		  /usr/lib/qt4/include/QtXml \
		  /usr/include/qt4/QtAssistant \
		  .
		  

    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj

    bulmages_install_script.path = .
    bulmages_install_script.extra = echo; echo "**************************"; echo Para instalar BULMAGES ejecute el script de instalacion; echo "installbulmages"; echo que encontrara en la carpeta installbulmages.; echo Gracias.; echo "(El equipo de Bulmages)"; echo "**************************"; echo;
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

windows{
    DEFINES += WINDOWS
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    LIBS += ../../../bulmalib/src/release/libbulmalib.a \
	    C:\Qt\4.1.0\lib\libQtXml4.a \
	    C:\Qt\4.1.0\lib\libQtXmld4.a 

    INCLUDEPATH += .. \
		   ..\..\src\.ui \
		   ..\..\src \
		   ..\.. \
		   ..\..\.. \
		   ..\..\..\bulmalib\src \
		   ..\..\..\bulmalib\src\.ui \
		   C:\Qt\4.1.0\include\QtXml \
		   "C:\Archivos de programa\PostgreSQL\8.1\include"

    LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"
}

FORMS =   listproyectosbase.ui \
	  proyectobase.ui

SOURCES = pluginproyectos.cpp \
	  listproyectosview.cpp \
	  proyectoview.cpp

HEADERS = pluginproyectos.h \
	  tiptrab.h \
	  listproyectosview.h \
	  proyectoview.h
	  



