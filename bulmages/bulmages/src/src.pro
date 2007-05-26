# Archivo generado por el gestor qmake de kdevelop. 
# -------------------------------------------------

QT += xml

LANGUAGE = C++

INCLUDEPATH += ../../bulmalib  \
	       ../../bulmalib/.ui \
	       ../../bulmalib/.moc \
	       ../../bulmages/src/.ui \
	       /usr/include/qt4 \
	       /usr/include/qt4/Qt \
	       /usr/lib/qt4/include/Qt \
	       /usr/lib/qt4/include/QtXml

TARGET = ../../installbulmages/bulmages 

RESOURCES += ../../bulmages.qrc

CONFIG += release \
          thread 

TEMPLATE = app 

FORMS += comun/importContaplusBase.ui \
	 comun/selectorbase.ui \
 	 comun/configuracion.ui\
	 comun/nuevaempresadlg.ui \
	 comun/nuevafactbase.ui \
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

TRANSLATIONS += bulmages_es.ts \
                bulmages_ca.ts \
                bulmages_en.ts \
                bulmages_fr.ts \
                bulmages_de.ts \
                bulmages_zz.ts

unix{


	LIBS += -lpq \
	        -lbulmalib

QMAKE_LIBDIR += ../../installbulmages

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

