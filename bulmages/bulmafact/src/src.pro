# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./bulmafact/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmafact


QT += qt3support


LANGUAGE = C++ 

INCLUDEPATH += ../../bulmalib  \
	       ../../bulmalib/.ui \
	       ../../bulmalib/.moc \
	       /usr/include/qt4 \
	       /usr/include/qt4/Qt \
	       /usr/include/QtXml \
               /usr/lib/qt4/include/Qt \
               /usr/lib/qt4/include/QtXml \
	       ..
	       
	       
DEFINES = REPORTS 

TARGET = ../../installbulmages/bulmafact 

CONFIG += release 

LIBS += -rdynamic

TEMPLATE = app 


RESOURCES = ../../bulmages-recursos-graficos.qrc


# Estos formularios hay que generarlos de forma manual porque, o se usa FORMS o FORMS3.
# Mientras se hace la migracion se tiene que hacer: uic about.ui -o ./.ui/about.h
# etc.
#
FORMS += about.ui \
 	 pagobase.ui \
         cobrobase.ui \
	 listprovincias.ui

# uic3 los convierte de forma automatica.
#
FORMS3 += bulmafactbase.ui \
          providerslistbase.ui \
          provedit_base.ui \
          clientslistbase.ui \
          articleslistbase.ui \
          divisionbase.ui \
          budgetbase.ui \
          budgetslistbase.ui \
          articleeditbase.ui \
          clientdelivnoteslistbase.ui \
          albaranclientebase.ui \
          fpagobase.ui \
          familiasdlg.ui \
          tiposarticulodlg.ui \
          facturaslistbase.ui \
          facturabase.ui \
          pedidosclientelistbase.ui \
          pedidoclientebase.ui \
          busquedaclientebase.ui \
          busquedaarticulobase.ui \
          busquedaproveedorbase.ui \
          albaranproveedorbase.ui \
          albaranesproveedorbase.ui \
          busquedafamiliabase.ui \
          busquedatipoarticulobase.ui \
          cobroslistbase.ui \
          facturasplistbase.ui \
          facturapbase.ui \
 	  trabajadorbase.ui \
	  pagoslistbase.ui \
	  pedidosproveedorlistbase.ui \
	  pedidoproveedorbase.ui \
	  listseriefacturabase.ui \
	  inventariosbase.ui \
	  inventariobase.ui \
	  clientebase.ui

HEADERS += bulmafact.h \
           company.h \
           providerslist.h \
           provedit.h \
           splashscreen.h \
           clientslist.h \
           articulolist.h \
           division.h \
           presupuestoview.h \
           presupuestolist.h \
           articuloview.h \
           EventHandler.h \
           clientdelivnoteslist.h \
           albaranclienteview.h \
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
	   busquedaseriefactura.h \
	   descalbaranprov.h \
	   listdescalbaranprov.h \
	   listdescalbaranprovview.h \
	   descfacturaprov.h \
	   listdescfacturaprov.h \
	   listdescfacturaprovview.h \
	   aboutview.h \
	   busquedaprovincia.h \
	   listprovinciasview.h \
	   inventariosview.h \
	   inventarioview.h \
	   inventario.h \
	   controlstock.h \
	   listcontrolstock.h \
	   listcontrolstockview.h \
	   cliente.h \
	   clienteview.h

	   
SOURCES += bulmafact.cpp \
           main.cpp \
           company.cpp \
           providerslist.cpp \
           provedit.cpp \
           splashscreen.cpp \
           clientslist.cpp \
           articulolist.cpp \
           division.cpp \
           presupuestoview.cpp \
           presupuestolist.cpp \
           articuloview.cpp \
           EventHandler.cpp \
           clientdelivnoteslist.cpp \
           albaranclienteview.cpp \
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
	   busquedaseriefactura.cpp \
	   descalbaranprov.cpp \
	   listdescalbaranprov.cpp \
	   listdescalbaranprovview.cpp \
	   descfacturaprov.cpp \
	   listdescfacturaprov.cpp \
	   listdescfacturaprovview.cpp \
	   aboutview.cpp \
	   busquedaprovincia.cpp \
	   listprovinciasview.cpp \
	   inventariosview.cpp \
	   inventarioview.cpp \
	   inventario.cpp \
	   controlstock.cpp \
	   listcontrolstock.cpp \
	   listcontrolstockview.cpp \
	   cliente.cpp \
	   clienteview.cpp

TRANSLATIONS += bulmafact_es.ts \
                bulmafact_ca.ts \
                bulmafact_en.ts \
                bulmafact_fr.ts \
                bulmafact_de.ts 

unix{

  TARGETDEPS += ../../bulmalib/libbulmalib.a
  LIBS += ../../bulmalib/libbulmalib.a \
	  -lpq

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

windows{
  DEFINES += WINDOWS
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
#  LIBS += ../../bulmalib/release/libbulmalib.a \


  LIBS += ../../bulmalib/release/bulmalib0.dll \
 	C:\Qt\4.1.0\lib\libQtXml4.a \
	C:\Qt\4.1.0\lib\libQtXmld4.a

  INCLUDEPATH += ../../bulmalib/libpq \
                 ../src \
		 .. \
 		 C:\Qt\4.1.0\include\QtXml


  INCLUDEPATH += "C:\Archivos de programa\PostgreSQL\8.1\include"
  LIBS += "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"

TARGET = ../../../installbulmages/bulmafact 
}

#The following line was inserted by qt3to4
QT +=  
#The following line was inserted by qt3to4
CONFIG += uic3

