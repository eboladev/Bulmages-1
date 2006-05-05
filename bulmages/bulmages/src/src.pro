# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./introbulmages/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmages

QT += qt3support xml

LANGUAGE = C++

INCLUDEPATH += ../../bulmalib  \
	       ../../bulmalib/.ui \
	       ../../bulmalib/.moc \
	       /usr/include/qt4 \
	       /usr/include/qt4/Qt \
	       /usr/lib/qt4/include/Qt \
	       /usr/lib/qt4/include/QtXml

TARGET = ../../installbulmages/bulmages 

CONFIG += release \
          warn_on \
          qt4 \
          thread 

TEMPLATE = app 

#comun/selectorbase.ui.target = comun/selectorbase.ui 

FORMS += comun/importContaplusBase.ui \
	 comun/selectorbase.ui \
	comun/configuracion.ui\
	comun/nuevaempresadlg.ui \
	comun/nuevafactbase.ui \
	comun/importexportbulmafactbase.ui

#IDLS += comun/selectorbase.ui
#        comun/uiconfiguracion.ui \
#        comun/nuevaempresadlg.ui \
#        comun/nuevafactbase.ui \
#        comun/importContaplusBase.ui \
#	comun/importexportbulmafactbase.ui

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




#uiselector.ui.commands = $$IDL_COMPILER 
#uiconfiguracion.ui.commands = $$IDL_COMPILER 
#nuevafactbase.ui.commands = $$IDL_COMPILER 
#nuevaempresadlg.ui.commands = $$IDL_COMPILER 
#importContaplusBase.ui.commands = $$IDL_COMPILER 


#comun/importContaplusBase.ui.target = comun/importContaplusBase.ui 
#comun/nuevafactbase.ui.target = comun/nuevafactbase.ui 
#comun/nuevaempresadlg.ui.target = comun/nuevaempresadlg.ui 
#comun/uiconfiguracion.ui.target = comun/uiconfiguracion.ui 


#comun/importContaplusBase.ui.target = comun/importContaplusBase.ui
#comun/nuevafactbase.ui.target = comun/nuevafactbase.ui
#comun/nuevaempresadlg.ui.target = comun/nuevaempresadlg.ui
#comun/uiconfiguracion.ui.target = comun/uiconfiguracion.ui
#comun/uiselector.ui.target = comun/uiselector.ui

TRANSLATIONS += ibulmages_es.ts \
                ibulmages_ca.ts \
                ibulmages_en.ts \
                ibulmages_fr.ts \
                ibulmages_de.ts 

unix{


	LIBS += -lpq \
	        -lbulmalib

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
  DEFINES += WINDOWS

 UI_DIR = .ui

  MOC_DIR = .moc

  OBJECTS_DIR = .obj

  LIBS += ../../bulmalib/release/libbulmalib.a \
 	C:\Qt\4.1.0\lib\libQtXml4.a \
	C:\Qt\4.1.0\lib\libQtXmld4.a 

  INCLUDEPATH += ../../bulmalib/libpq \
			../../bulmalib \
			../../bulmalib/.ui \
                 ../contabilidad \
		 comun \
 		 C:\Qt\4.1.0\include\QtXml

TARGET = ..\..\..\installbulmages\bulmages 
  INCLUDEPATH += "C:\Archivos de programa\PostgreSQL\8.1\include"
  LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"

}

#The following line was inserted by qt3to4
#QT +=  
#The following line was inserted by qt3to4
# CONFIG += uic3
