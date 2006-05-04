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
               /usr/include/qt4/QtXml \
	       /usr/include/Qt \
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
 	 trabajadorbase.ui \
         fpagobase.ui \
	 pagoslistbase.ui \
         cobroslistbase.ui \
         articleslistbase.ui \
         articleeditbase.ui \
         providerslistbase.ui \
         proveditbase.ui \
         pedidosclientelistbase.ui \
         clientslistbase.ui \
         albaranclientelistbase.ui \
         facturaslistbase.ui \
	 pedidosproveedorlistbase.ui \
	 listprovincias.ui \
         facturasplistbase.ui \
         albaranesproveedorlistbase.ui \
	 pedidoproveedorbase.ui \
         albaranproveedorbase.ui \
         facturapbase.ui \
         pedidoclientebase.ui \
         albaranclientebase.ui \
         facturabase.ui \
         tiposarticulodlg.ui \
         busquedaclientebase.ui \
         busquedaarticulobase.ui \
         busquedaproveedorbase.ui \
	 busquedatipoarticulobase.ui \
         familiasdlg.ui \
         busquedafamiliabase.ui \
	 listseriefacturabase.ui \
	 inventariosbase.ui \
	 inventariobase.ui \
	 clientebase.ui \
         budgetslistbase.ui \
         bulmafactbase.ui \
	 listalmacenbase.ui \
	 listconfiguracionbase.ui \
         budgetbase.ui \
	 subform3base.ui


HEADERS += bulmafact.h \
           company.h \
           providerslist.h \
           provedit.h \
           splashscreen.h \
           clientslist.h \
           articulolist.h \
           presupuestoview.h \
           presupuestolist.h \
           articuloview.h \
           EventHandler.h \
           albaranclientelist.h \
           albaranclienteview.h \
           listventanas.h \
           fpagoview.h \
           familiasview.h \
           tiposarticuloview.h \
           presupuesto.h \
           listlinpresupuestoview.h \
           facturaslist.h \
           facturaview.h \
           factura.h \
           listlinfacturaview.h \
           albarancliente.h \
           listlinalbaranclienteview.h \
           pedidosclientelist.h \
           pedidocliente.h \
           pedidoclienteview.h \
           listlinpedidoclienteview.h \
           busquedacliente.h \
           busquedaformapago.h \
           busquedaalmacen.h \
           busquedaarticulo.h \
           busquedaproveedor.h \
           albaranproveedor.h \
           albaranproveedorview.h \
           listlinalbaranproveedorview.h \
           albaranesproveedor.h \
           busquedafamilia.h \
           busquedatipoarticulo.h \
           cobro.h \
           cobroview.h \
           cobroslist.h \
           facturasplist.h \
           listlinfacturapview.h \
           facturap.h \
           facturapview.h \
           listdescpresupuestoview.h \
           listdescpedidoclienteview.h \
           listdescalbaranclienteview.h  \
	   listdescfacturaview.h \
	   informereferencia.h \
	   trabajadorview.h \
	   busquedatrabajador.h \
	   comparticulo.h \
	   comparticulolist.h \
	   comparticulolistview.h \
	   pago.h \
	   pagoview.h \
	   pagoslist.h \
	   pedidosproveedorlist.h \
	   pedidoproveedor.h \
	   pedidoproveedorview.h \
	   listlinpedidoproveedorview.h \
	   listdescpedidoproveedorview.h \
	   listseriefacturaview.h \
	   busquedaseriefactura.h \
	   listdescalbaranprovview.h \
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
	   clienteview.h \
	   articulo.h \
	   subform.h \
	   listalmacenview.h \
	   listconfiguracionview.h \
	   subform2bf.h \
	   subform3.h

	   
SOURCES += bulmafact.cpp \
           main.cpp \
           company.cpp \
           providerslist.cpp \
           provedit.cpp \
           splashscreen.cpp \
           clientslist.cpp \
           articulolist.cpp \
           presupuestoview.cpp \
           presupuestolist.cpp \
           articuloview.cpp \
           EventHandler.cpp \
           albaranclientelist.cpp \
           albaranclienteview.cpp \
           listventanas.cpp \
           fpagoview.cpp \
           familiasview.cpp \
           tiposarticuloview.cpp \
           presupuesto.cpp \
           listlinpresupuestoview.cpp \
           facturaslist.cpp \
           facturaview.cpp \
           factura.cpp \
           listlinfacturaview.cpp \
           albarancliente.cpp \
           listlinalbaranclienteview.cpp \
           pedidosclientelist.cpp \
           pedidocliente.cpp \
           pedidoclienteview.cpp \
           listlinpedidoclienteview.cpp \
           busquedacliente.cpp \
           busquedaformapago.cpp \
           busquedaalmacen.cpp \
           busquedaarticulo.cpp \
           busquedaproveedor.cpp \
           albaranproveedor.cpp \
           albaranproveedorview.cpp \
           listlinalbaranproveedorview.cpp \
           albaranesproveedor.cpp \
           busquedafamilia.cpp \
           busquedatipoarticulo.cpp \
           cobro.cpp \
           cobroview.cpp \
           cobroslist.cpp \
           facturasplist.cpp \
           listlinfacturapview.cpp \
           facturap.cpp \
           facturapview.cpp \
           listdescpresupuestoview.cpp \
           listdescpedidoclienteview.cpp \
           listdescalbaranclienteview.cpp \
	   listdescfacturaview.cpp \
	   informereferencia.cpp \
	   trabajadorview.cpp \
	   busquedatrabajador.cpp \
	   comparticulo.cpp \
	   comparticulolist.cpp \
	   comparticulolistview.cpp \
	   pago.cpp \
	   pagoview.cpp \
	   pagoslist.cpp \
	   pedidosproveedorlist.cpp \
	   pedidoproveedor.cpp \
	   pedidoproveedorview.cpp \
	   listlinpedidoproveedorview.cpp \
	   listdescpedidoproveedorview.cpp \
	   listseriefacturaview.cpp \
	   busquedaseriefactura.cpp \
	   listdescalbaranprovview.cpp \
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
	   clienteview.cpp \
	   articulo.cpp \
	   subform.cpp \
	   listalmacenview.cpp \
	   listconfiguracionview.cpp \
	   subform2bf.cpp \
	   subform3.cpp

TRANSLATIONS += bulmafact_es.ts \
                bulmafact_ca.ts \
                bulmafact_en.ts \
                bulmafact_fr.ts \
                bulmafact_de.ts 

unix{

  TARGETDEPS += ../../installbulmages/libbulmalib.so
  LIBS += -lbulmalib \
	  -lpq

  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  bulmages_install_script.path = .
  bulmages_install_script.extra = echo; echo "**************************"; echo "Para Instalar BULMAGES ejecute el script de instalacion"
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

