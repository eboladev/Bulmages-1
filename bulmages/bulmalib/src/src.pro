# Archivo generado por el gestor qmake de kdevelop.
# -------------------------------------------------

#Probamos de usar distcc
#!isEmpty( DISTCC_HOSTS ) {
exists(/usr/bin/distcc){
    QMAKE_CXX = distcc g++
}
#}


FORMS += logpassbase.ui \
         abreempresabase.ui \
         msgerrorbase.ui \
         busquedafechabase.ui \
         subform3base.ui \
	 porcentajecargabase.ui

TRANSLATIONS += bulmalib_es.ts \
                bulmalib_ca.ts \
                bulmalib_en.ts \
                bulmalib_fr.ts \
                bulmalib_de.ts \
                bulmalib_zz.ts 

HEADERS += postgresiface2.h \
           configuracion.h \
           numerocuenta.h \
           funcaux.h \
           logpass.h \
           abreempresaview.h \
           dialogchanges.h \
           fixed.h \
           msgerror.h \
           busquedafecha.h \
           qtable2.h \
           pgimportfiles.h \
           plugins.h \
           busquedaperiodo.h \
           dbrecord.h \
           subform.h \
           subform3.h \
           listventanas.h \
           qworkspace2.h \
           qapplication2.h \
           ficha.h \
	   empresabase.h \
	   qtexteditdelegate.h \
           qdoublespinbox2.h \
	   blwidget.h \
	   listado.h \
           porcentajecarga.h \
	   busquedafecha2.h \
	   splashscreen.h
	   
	   
SOURCES += postgresiface2.cpp \
           configuracion.cpp \
           numerocuenta.cpp \
           funcaux.cpp \
           logpass.cpp \
           abreempresaview.cpp \
           dialogchanges.cpp \
           fixed.cpp \
           msgerror.cpp \
           busquedafecha.cpp \
           qtable2.cpp \
           pgimportfiles.cpp \
           plugins.cpp \
           busquedaperiodo.cpp \
           dbrecord.cpp \
           subform.cpp \
           subform3.cpp \
           listventanas.cpp \
           qworkspace2.cpp \
           qapplication2.cpp \
           ficha.cpp \
	   empresabase.cpp \
	   qtexteditdelegate.cpp \
           qdoublespinbox2.cpp \
	   blwidget.cpp \
	   listado.cpp \
           porcentajecarga.cpp \
	   busquedafecha2.cpp \
	   splashscreen.cpp
	   
	   
RESOURCES = ../../bulmages.qrc
TEMPLATE = lib
LANGUAGE = C++
INCLUDEPATH += . \
/usr/include/qt4 \
/usr/include/qt4/Qt \
/usr/lib/qt4/include/Qt \
/usr/include/Qt \
/usr/include/QtXml \
/usr/include/qt4/include/QtXml

VERSION = 0.9.3
TARGET = bulmalib
OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
QT += xml
CONFIG += uic

unix{
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    LIBS += -lpq
    LIBS += -rdynamic
    CONFIG += release debug
    #        CONFIG += qt debug
    TARGET = bulmalib
    DESTDIR = ../../installbulmages
    exists(/usr/include/postgresql/libpq-fe.h){
        DEFINES += DISTRO_DEBIAN
    }
    exists(/usr/include/postgresql/8.0/libpq-fe.h){
        DEFINES += DISTRO_DEBIAN_8_0
    }
    exists(/usr/include/pgsql/libpq-fe.h){
        DEFINES += DISTRO_RED_HAT
    }
    exists(/usr/include/postgresql/pgsql/libpq-fe.h){
        DEFINES += DISTRO_GENTOO
    }
}

windows{
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"
    INCLUDEPATH += "C:\Archivos de programa\PostgreSQL\8.1\include"
    DEFINES += WINDOWS
    DEFINES += WIN32
    CONFIG += release \
		   dll
    VERSION += 0.9.3
}

