# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./bulmalib
# Destiono es una biblioteca:  bulmalib

FORMS += logpassbase.ui \
         abreempresadlg.ui \
         msgerrorbase.ui \
	 busquedafechabase.ui
IDLS += logpassbase.ui \
        abreempresadlg.ui 
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
	   pgimportfiles.h 
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
	   pgimportfiles.cpp
TEMPLATE = lib
LANGUAGE = C++
CONFIG += release \
qt\
warn_on \
staticlib
INCLUDEPATH += ../src /usr/include/qt
LIBS += -lqt-mt \
-lpq \
-rdynamic
VERSION = 0.1
TARGET = bulmalib
OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
unix{
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
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
