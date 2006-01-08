# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./bulmacont/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmacont

FORMS += adocumentalbase.ui 

SOURCES += pluginadocumental.cpp \
	  adocumental.cpp

HEADERS += pluginadocumental.h  \
	  adocumental.h



TEMPLATE = lib

CONFIG +=  plugin \
         warn_on
	 	 
TARGET = ../../../installbulmages/plugins/adocumental

VERSION = 1.0.0

DESTDIR = .

LIBS += -lpq \
	-rdynamic

INCLUDEPATH = ../../../bulmalib \
              ../contabilidad \
	      ../contabilidad/.ui \
	      ../comun \
              /usr/include/qt4 \
	      /usr/include/qt4/Qt \
	      /usr/include/QtXml \
              ../../../bulmalib/.ui \
	      .ui \
	      ../.ui 
	      

unix{
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


#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3

