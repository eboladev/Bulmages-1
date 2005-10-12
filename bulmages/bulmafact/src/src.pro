# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./bulmafact/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmafact

LANGUAGE = C++ 
TARGETDEPS += ../../bulmalib/libbulmalib.a 
LIBS += ../../bulmalib/libbulmalib.a \
        -lpq 
INCLUDEPATH += ../../bulmalib  \
	       ../../bulmalib/.ui \
	       ../../bulmalib/.moc
DEFINES = REPORTS 
TARGET = ../../installbulmages/bulmafact 
CONFIG += release \
          warn_on \
          qt \
          thread 
TEMPLATE = app 
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
         facturabase.ui \
         pedidosclientelistbase.ui \
         pedidoclientebase.ui \
         busquedaclientebase.ui \
#         busquedafechabase.ui \
         busquedaarticulobase.ui \
         busquedaproveedorbase.ui \
         albaranproveedorbase.ui \
         albaranesproveedorbase.ui \
         busquedafamiliabase.ui \
         busquedatipoarticulobase.ui \
         cobrobase.ui \
         cobroslistbase.ui \
         facturasplistbase.ui \
         facturapbase.ui \
	 trabajadorbase.ui \
	 pagobase.ui \
	 pagoslistbase.ui \
	 pedidosproveedorlistbase.ui \
	 pedidoproveedorbase.ui \
	 listseriefacturabase.ui
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
           linalbarancliente.h \
           pedidosclientelist.h \
           pedidocliente.h \
           pedidoclienteview.h \
           linpedidocliente.h \
           listlinpedidocliente.h \
           listlinpedidoclienteview.h \
           busquedacliente.h \
           busquedaformapago.h \
#           busquedafecha.h \
           busquedaalmacen.h \
           busquedaarticulo.h \
           busquedaproveedor.h \
           albaranproveedor.h \
           albaranproveedorview.h \
           linalbaranproveedor.h \
           listlinalbaranproveedor.h \
           listlinalbaranproveedorview.h \
           albaranesproveedor.h \
           busquedafamilia.h \
           busquedatipoarticulo.h \
           cobro.h \
           cobroview.h \
           cobroslist.h \
           facturasplist.h \
           linfacturap.h \
           listlinfacturap.h \
           listlinfacturapview.h \
           facturap.h \
           facturapview.h \
           descpresupuesto.h \
           listdescpresupuesto.h \
           listdescpresupuestoview.h \
           descpedidocliente.h \
           listdescpedidocliente.h \
           listdescpedidoclienteview.h \
           descalbarancliente.h \
           listdescalbarancliente.h \
           listdescalbaranclienteview.h  \
	   descfactura.h \
	   listdescfactura.h \
	   listdescfacturaview.h \
	   informereferencia.h \
	   trabajador.h \
	   busquedatrabajador.h \
	   comparticulo.h \
	   listcomparticulo.h \
	   listcomparticuloview.h \
	   pago.h \
	   pagoview.h \
	   pagoslist.h \
	   pedidosproveedorlist.h \
	   pedidoproveedor.h \
	   pedidoproveedorview.h \
	   listlinpedidoproveedor.h\
	   listlinpedidoproveedorview.h \
	   linpedidoproveedor.h \
	   descpedidoproveedor.h \
	   listdescpedidoproveedor.h \
	   listdescpedidoproveedorview.h \
	   listseriefacturaview.h \
	   busquedaseriefactura.h
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
           linalbarancliente.cpp \
           pedidosclientelist.cpp \
           pedidocliente.cpp \
           pedidoclienteview.cpp \
           linpedidocliente.cpp \
           listlinpedidocliente.cpp \
           listlinpedidoclienteview.cpp \
           busquedacliente.cpp \
           busquedaformapago.cpp \
#           busquedafecha.cpp \
           busquedaalmacen.cpp \
           busquedaarticulo.cpp \
           busquedaproveedor.cpp \
           albaranproveedor.cpp \
           albaranproveedorview.cpp \
           linalbaranproveedor.cpp \
           listlinalbaranproveedor.cpp \
           listlinalbaranproveedorview.cpp \
           albaranesproveedor.cpp \
           busquedafamilia.cpp \
           busquedatipoarticulo.cpp \
           cobro.cpp \
           cobroview.cpp \
           cobroslist.cpp \
           facturasplist.cpp \
           linfacturap.cpp \
           listlinfacturap.cpp \
           listlinfacturapview.cpp \
           facturap.cpp \
           facturapview.cpp \
           descpresupuesto.cpp \
           listdescpresupuesto.cpp \
           listdescpresupuestoview.cpp \
           descpedidocliente.cpp \
           listdescpedidocliente.cpp \
           listdescpedidoclienteview.cpp \
           descalbarancliente.cpp \
           listdescalbarancliente.cpp \
           listdescalbaranclienteview.cpp \
	   descfactura.cpp \
	   listdescfactura.cpp \
	   listdescfacturaview.cpp \
	   informereferencia.cpp \
	   trabajador.cpp \
	   busquedatrabajador.cpp \
	   comparticulo.cpp \
	   listcomparticulo.cpp \
	   listcomparticuloview.cpp \
	   pago.cpp \
	   pagoview.cpp \
	   pagoslist.cpp \
	   pedidosproveedorlist.cpp \
	   pedidoproveedor.cpp \
	   pedidoproveedorview.cpp \
	   listlinpedidoproveedor.cpp\
	   listlinpedidoproveedorview.cpp \
	   linpedidoproveedor.cpp \
	   descpedidoproveedor.cpp \
	   listdescpedidoproveedor.cpp \
	   listdescpedidoproveedorview.cpp \
	   listseriefacturaview.cpp \
	   busquedaseriefactura.cpp

TRANSLATIONS += bulmafact_es.ts \
                bulmafact_ca.ts \
                bulmafact_en.ts \
                bulmafact_fr.ts \
                bulmafact_de.ts 

unix{
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  bulmages_install_script.path = .
  bulmages_install_script.extra = echo; echo "**************************"; echo "Para Instalar BULMAGES ejecute el script de instalacion";
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
