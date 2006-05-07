# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./bulmalib
# Destiono es una biblioteca:  bulmalib

QT += qt3support

FORMS += logpassbase.ui \
          abreempresadlg.ui \
          msgerrorbase.ui \
 	  busquedafechabase.ui \
	  subform3base.ui

HEADERS += postgresiface2.h \
           configuracion.h \
           numerocuenta.h \
           funcaux.h \
           logpass.h \
           abreempresaview.h \
           calendario.h \
           qmcdatenav.h \
           dialogchanges.h \
           fixed.h \
           msgerror.h \
	   busquedafecha.h \
	   qtable1.h \
	   qtable2.h \
	   pgimportfiles.h  \
	   plugins.h \
	   busquedaperiodo.h \
	   dbrecord.h \
	   subform.h \
	   subform3.h
	   
SOURCES += postgresiface2.cpp \
           configuracion.cpp \
           numerocuenta.cpp \
           funcaux.cpp \
           logpass.cpp \
           abreempresaview.cpp \
           calendario.cpp \
           qmcdatenav.cpp \
           dialogchanges.cpp \
           fixed.cpp \
           msgerror.cpp \
	   busquedafecha.cpp \
	   qtable1.cpp \
	   qtable2.cpp \
	   pgimportfiles.cpp \
	   plugins.cpp \
	   busquedaperiodo.cpp \
	   dbrecord.cpp \
	   subform.cpp \
	   subform3.cpp

TRANSLATIONS += bulmalib_es.ts \
                bulmalib_ca.ts \
                bulmalib_en.ts \
                bulmalib_fr.ts \
                bulmalib_de.ts 

TEMPLATE = lib

LANGUAGE = C++

INCLUDEPATH += ../src \
	/usr/include/qt4 \
	/usr/include/qt4/Qt \
	/usr/lib/qt4/include/Qt \
	/usr/include/Qt \
	/usr/include/QtXml
	

VERSION = 0.5.9

TARGET = bulmalib

OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc

unix{
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  LIBS += -lpq

LIBS += -rdynamic
CONFIG += release

TARGET = bulmalib
DESTDIR = ../installbulmages

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
  INCLUDEPATH += "C:\Archivos de programa\PostgreSQL\8.1\include"
  LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"
  DEFINES += WINDOWS
  DEFINES += WIN32

  CONFIG += release \
		dll

  VERSION += 0.5.9
}

#The following line was inserted by qt3to4
QT += xml  
#The following line was inserted by qt3to4
CONFIG += uic
