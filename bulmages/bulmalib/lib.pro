TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= release warn_on staticlib

INCLUDEPATH	+= ../src /usr/include/qt

HEADERS	+= postgresiface2.h \
	   configuracion.h

SOURCES	+= postgresiface2.cpp \
           configuracion.cpp


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
  exists(/usr/include/pgsql/libpq-fe.h){
    DEFINES += DISTRO_RED_HAT
  }
  exists(/usr/include/postgresql/pgsql/libpq-fe.h){
    DEFINES += DISTRO_GENTOO
  }
}
