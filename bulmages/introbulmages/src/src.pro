# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./introbulmages/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmages

uiselector.ui.commands = $$IDL_COMPILER 
uiconfiguracion.ui.commands = $$IDL_COMPILER 
nuevafactbase.ui.commands = $$IDL_COMPILER 
nuevaempresadlg.ui.commands = $$IDL_COMPILER 
importContaplusBase.ui.commands = $$IDL_COMPILER 
TARGETDEPS += ../../bulmalib/libbulmalib.a 
LIBS += ../../bulmalib/libbulmalib.a \
        -lpq 
INCLUDEPATH += ../../bulmalib \
		../../bulmalib/.ui \
		../../bulmalib/.moc
TARGET = ../../installbulmages/bulmages 
CONFIG += release \
          warn_on \
          qt \
          thread 

comun/importContaplusBase.ui.target = comun/importContaplusBase.ui 
comun/nuevafactbase.ui.target = comun/nuevafactbase.ui 
comun/nuevaempresadlg.ui.target = comun/nuevaempresadlg.ui 
comun/uiconfiguracion.ui.target = comun/uiconfiguracion.ui 
comun/uiselector.ui.target = comun/uiselector.ui 

TEMPLATE = app 
FORMS += comun/uiselector.ui \
         comun/uiconfiguracion.ui \
         comun/nuevaempresadlg.ui \
         comun/nuevafactbase.ui \
         comun/importContaplusBase.ui \
	 comun/importexportbulmafactbase.ui

IDLS += comun/uiselector.ui \
        comun/uiconfiguracion.ui \
        comun/nuevaempresadlg.ui \
        comun/nuevafactbase.ui \
        comun/importContaplusBase.ui \
	comun/importexportbulmafactbase.ui

HEADERS += comun/bselector.h \
           comun/bconfiguracion.h \
           comun/bnuevaempresa.h \
           comun/nuevafact.h \
           comun/gongimportfiles.h \
           comun/importContaplus.h \
	   comun/importexportbulmafactview.h

SOURCES += main.cpp \
           comun/bselector.cpp \
           comun/bconfiguracion.cpp \
           comun/bnuevaempresa.cpp \
           comun/nuevafact.cpp \
           comun/gongimportfiles.cpp \
           comun/importContaplus.cpp \
	   comun/importexportbulmafactview.cpp

comun/importContaplusBase.ui.target = comun/importContaplusBase.ui
comun/nuevafactbase.ui.target = comun/nuevafactbase.ui
comun/nuevaempresadlg.ui.target = comun/nuevaempresadlg.ui
comun/uiconfiguracion.ui.target = comun/uiconfiguracion.ui
comun/uiselector.ui.target = comun/uiselector.ui

unix{
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  bulmages_install_script.path = .
  bulmages_install_script.extra = echo; echo "**************************"; echo Para Instalar BULMAGES ejecute el script de instalaci�; echo "installbulmages"; echo que encontrara en la carpeta installbulmages.; echo Gracias.; echo "(El equipo de Bulmages)"; echo "**************************"; echo;
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
  exists(/usr/include/no_se_que/pgsql/libpq-fe.h){
    DEFINES += DISTRO_NO_SE_QUE
  }
}

win32{
  DEFINES += WIN32
  INCLUDEPATH += lpqwin32/include
}
