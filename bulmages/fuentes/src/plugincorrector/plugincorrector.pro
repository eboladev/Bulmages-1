# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./fuentes/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmacont

TEMPLATE = lib

CONFIG = qt \
         plugin \
         warn_on
	 	 
TARGET = ../../../installbulmages/plugins/corrector

VERSION = 1.0.0

DESTDIR = .

LIBS += -lqt-mt  \
        -lpq \
	-rdynamic

INCLUDEPATH = ../../../bulmalib \
              ../contabilidad \
	      ../comun \
              /usr/include/qt \
              ../../../bulmalib/.ui

unix{
UI_DIR = ../.ui
MOC_DIR = ../.moc
OBJECTS_DIR = ../.obj
  bulmages_install_script.path = .
  bulmages_install_script.extra = echo; echo "**************************"; echo Para Instalar BULMAGES ejecute el script de instalación; echo "installbulmages"; echo que encontrara en la carpeta installbulmages.; echo Gracias.; echo "(El equipo de Bulmages)"; echo "**************************"; echo;
  exists(/usr/include/postgresql/libpq-fe.h){
    DEFINES += DISTRO_DEBIAN
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

FORMS = correctorwdt.ui 

IDLS = correctorwdt.ui 

SOURCES = plugincorrector.cpp \
	  correctorwidget.cpp

HEADERS = plugincorrector.h \
          correctorwidget.h

