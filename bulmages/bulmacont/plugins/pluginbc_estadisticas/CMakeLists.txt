# Archivo generado por el gestor qmake de kdevelop.
# -------------------------------------------------


	
SET(CONFIG qt )
         plugin \
         warn_on
	 
PROJECT(../../../installbulmages/plugins/estadisticas)

SET(VERSION 1.0.0)
SET(DESTDIR .)
LIBS += -lqt-mt \
        -lpq \
	-rdynamic

SET(INCLUDEPATH ../../../bulmalib )
	      ../../src \
	      ../../src/.ui \
	      /usr/include/qt \
              /usr/lib/qt4/include/Qt \
              ../../src \
	      ../images \
              ../formularios \
              ../../../bulmalib/.ui \
              ../.ui 

HEADERS += estadisticasview.h \
           resmensualview.h \
	   pluginestadisticas.h \
	   estadisticas/tobarchart.h \
           estadisticas/topiechart.h \ 
	   estadisticas/tolinechart.h

SOURCES += estadisticasview.cpp \
           resmensualview.cpp \
	   pluginestadisticas.cpp \
	   estadisticas/tobarchart.cpp \
           estadisticas/tolinechart.cpp \
           estadisticas/topiechart.cpp


SET(  estadisticas/tolinechartsetupui.ui.commands $$IDL_COMPILER $$IDL_OPTIONS $$tolinechartsetupui.ui.target)
SET(  estadisticas/tolinechartsetupui.ui.target tolinechartsetupui.ui)
SET(  estadisticasdlg.ui.commands $$IDL_COMPILER $$IDL_OPTIONS $$estadisticasdlg.ui.target)
SET(  estadisticasdlg.ui.target estadisticasdlg.ui)

SET(  estadisticasdlg.ui.target contabilidad/estadisticasdlg.ui)
SET(  estadisticasdlg.ui.commands $$IDL_COMPILER $$IDL_OPTIONS $$estadisticasdlg.ui.target)
SET(  resmensualdlg.ui.target contabilidad/resmensualdlg.ui)
SET(  resmensualdlg.ui.commands $$IDL_COMPILER $$IDL_OPTIONS $$resmensualdlg.ui.target)
  DEFINES += ESTADISTICAS
  IDLS += estadisticasdlg.ui \
  resmensualdlg.ui\
  estadisticas/tolinechartsetupui.ui

  FORMS += estadisticasdlg.ui \
  resmensualdlg.ui \
  estadisticas/tolinechartsetupui.ui

  IMAGES += estadisticas/print.xpm

unix{
SET(  UI_DIR .ui)
SET(  MOC_DIR .moc)
SET(  OBJECTS_DIR .obj)
SET(  bulmages_install_script.path .)
SET(  bulmages_install_script.extra echo; echo "**************************"; echo Para Instalar BULMAGES ejecute el script de instalación; echo "installbulmages"; echo que encontrara en la carpeta installbulmages.; echo Gracias.; echo "(El equipo de Bulmages)"; echo "**************************"; echo;)
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

ADD_LIBRARY(../../../installbulmages/plugins/estadisticas ${../../../installbulmages/plugins/estadisticas_sources})ADD_EXECUTABLE(../../../installbulmages/plugins/estadisticas ../../../installbulmages/plugins/estadisticas_sources)
