# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./fuentes/src/estadisticas
# Destiono es una biblioteca:  estadisticas.lib

LIBS += -lqt-mt 
INCLUDEPATH = ../../src/estadisticas \
              ../../src \
              /usr/include/qt 
TARGET = estadisticas.lib 
CONFIG += debug \
          warn_on \
          staticlib 
tolinechartsetupui.ui.commands = $$IDL_COMPILER $$IDL_OPTIONS $$tolinechartsetupui.ui.target 
tolinechartsetupui.ui.target = tolinechartsetupui.ui 
estadisticasdlg.ui.commands = $$IDL_COMPILER $$IDL_OPTIONS $$estadisticasdlg.ui.target 
estadisticasdlg.ui.target = estadisticasdlg.ui 
TEMPLATE = lib 
FORMS += tolinechartsetupui.ui 
IMAGES += print.xpm 
IDLS += estadisticasdlg.ui \
        tolinechartsetupui.ui 
HEADERS += tobarchart.h \
           topiechart.h \
           tolinechart.h 
SOURCES += tobarchart.cpp \
           tolinechart.cpp \
           topiechart.cpp 
win32{
  DEFINES += WIN32
}
