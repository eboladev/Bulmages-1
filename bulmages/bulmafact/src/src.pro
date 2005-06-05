# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./bulmafact/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmafact

FORMS += bulmafactbase.ui \
         providerslistbase.ui \
         provedit_base.ui \
         clientslistbase.ui \
         clienteditbase.ui \
         articleslistbase.ui \
         orderslistbase.ui \
         linorderslistbase.ui \
         delivnoteslistbase.ui \
         divisionbase.ui \
         budgetbase.ui \
         budgetslistbase.ui \
         articleeditbase.ui \
         clientdelivnoteslistbase.ui \
         albaranclientebase.ui \
         clientinvoiceslistbase.ui \
         fpagobase.ui \
         familiasdlg.ui \
         tiposarticulodlg.ui \
         facturaslistbase.ui \
         facturabase.ui 
TRANSLATIONS += bulmafact_es.ts \
                bulmafact_ca.ts \
                bulmafact_en.ts \
                bulmafact_fr.ts \
                bulmafact_de.ts 
HEADERS += bulmafact.h \
           company.h \
           providerslist.h \
           provedit.h \
           splashscreen.h \
           clientslist.h \
           clientedit.h \
           articleslist.h \
           orderslist.h \
           linorderslist.h \
           delivnoteslist.h \
           division.h \
           budget.h \
           budgetslist.h \
           articleedit.h \
           EventHandler.h \
           clientdelivnoteslist.h \
           albaranclienteview.h \
           clientinvoiceslist.h \
           listventanas.h \
           fpago.h \
           familiasview.h \
           tiposarticuloview.h \
           presupuesto.h \
           listlinpresupuesto.h \
           linpresupuesto.h \
           listlinpresupuestoview.h \
           facturaslist.h \
           facturaview.h \
           factura.h \
           listlinfactura.h \
           listlinfacturaview.h \
           linfactura.h \
           albarancliente.h \
	   listlinalbarancliente.h \
	   listlinalbaranclienteview.h \
	   linalbarancliente.h
SOURCES += bulmafact.cpp \
           main.cpp \
           company.cpp \
           providerslist.cpp \
           provedit.cpp \
           splashscreen.cpp \
           clientslist.cpp \
           clientedit.cpp \
           articleslist.cpp \
           orderslist.cpp \
           linorderslist.cpp \
           delivnoteslist.cpp \
           division.cpp \
           budget.cpp \
           budgetslist.cpp \
           articleedit.cpp \
           EventHandler.cpp \
           clientdelivnoteslist.cpp \
           albaranclienteview.cpp \
           clientinvoiceslist.cpp \
           listventanas.cpp \
           fpago.cpp \
           familiasview.cpp \
           tiposarticuloview.cpp \
           presupuesto.cpp \
           listlinpresupuesto.cpp \
           linpresupuesto.cpp \
           listlinpresupuestoview.cpp \
           facturaslist.cpp \
           facturaview.cpp \
           factura.cpp \
           listlinfactura.cpp \
           listlinfacturaview.cpp \
           linfactura.cpp \
           albarancliente.cpp \
	   listlinalbarancliente.cpp \
	   listlinalbaranclienteview.cpp \
	   linalbarancliente.cpp
TEMPLATE = app
LANGUAGE = C++
CONFIG += release \
warn_on \
thread \
qt
LIBS += ../../bulmalib/libbulmalib.a -lpq
INCLUDEPATH += ../../bulmalib
TARGET = ../../installbulmages/bulmafact
DEFINES += REPORTS
unix{
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  bulmages_install_script.path = .
  bulmages_install_script.extra = echo; echo "**************************"; echo "Para Instalar BULMAGES ejecute el script de instalacion";
  exists(/usr/include/postgresql/libpq-fe.h){
    DEFINES += DISTRO_DEBIAN
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
