# Archivo generado por el gestor qmake de kdevelop.
# -------------------------------------------------

exists(/usr/bin/distcc) {
    QMAKE_CXX = distcc g++
}

QT +=  xml

TEMPLATE = lib

CONFIG += plugin \
          qt \
	  release \
	  assistant

TARGET = ../../../installbulmages/plugins/registroiva

# RESOURCES = ../../../bulmagesp.qrc

VERSION = 1.0.0

DESTDIR = .

QMAKE_LIBDIR += ../../../installbulmages

LIBS += -rdynamic \
        -lbulmalib

INCLUDEPATH = ../../../bulmalib/src \
              ../../src \
              ../../src/.ui \
              /usr/include/qt4 \
              /usr/include/qt4/Qt \
              /usr/include/QtXml \
              /usr/lib/qt4/include/QtXml \
              /usr/lib/qt4/include/Qt \
              ../../../bulmalib/src/.ui \
              .ui 

FORMS += registroivabase.ui \
	 listregistroivabase.ui \
	 cobropagobase.ui

HEADERS += registroiva.h \
	   iva.h \
	   listiva.h \
	   listivaview.h \
	   registroivaview.h \
	   pluginregistroiva.h \
	   linprevcobro.h \
	   listlinprevcobro.h \
	   listlinprevcobroview.h \
	   cobropagoview.h \
	   listregistroivaview.h
	   
SOURCES += registroiva.cpp \
	   iva.cpp \
	   listiva.cpp \
	   listivaview.cpp  \
	   registroivaview.cpp \
	   pluginregistroiva.cpp \
	   linprevcobro.cpp \
	   listlinprevcobro.cpp \
	   listlinprevcobroview.cpp \
	   cobropagoview.cpp \
	   listregistroivaview.cpp

unix{
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

