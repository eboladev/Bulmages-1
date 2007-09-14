# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./bulmafact/src
# Destiono es una aplicaci??n: ../../installbulmages/bulmafact

LANGUAGE = C++
TARGET = ../../installbulmages/bulmafact
#CONFIG += qt debug
CONFIG += release warn_on assistant
LIBS += -rdynamic
TEMPLATE = app

QMAKE_LIBDIR += ../../installbulmages

#Si existe distcc hacemos compilacion distribuida
exists( /usr/bin/distcc ) {
   QMAKE_CXX = distcc g++
}
# Lo mismo pero para SuSe (compilado distcc desde la fuente)
exists( /usr/local/bin/distcc ) {
   QMAKE_CXX = distcc g++
}

QMAKE_CFLAGS_WARN_OFF += strict_aliasing
QMAKE_CXXFLAGS_WARN_OFF += strict_aliasing


# RESOURCES += ../../bulmages-recursos-graficos.qrc

FORMS += aboutbase.ui \
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
         facturasplistbase.ui \
         albaranesproveedorlistbase.ui \
         pedidoproveedorbase.ui \
         albaranproveedorbase.ui \
         facturapbase.ui \
         pedidoclientebase.ui \
         albaranclientebase.ui \
         facturabase.ui \
         tiposarticulobase.ui \
         busquedaclientebase.ui \
         busquedaarticulobase.ui \
         busquedaproveedorbase.ui \
         busquedatipoarticulobase.ui \
         familiasbase.ui \
         busquedafamiliabase.ui \
         listseriefacturabase.ui \
         clientebase.ui \
         presupuestoslistbase.ui \
         bulmafactbase.ui \
         listalmacenbase.ui \
         listconfiguracionbase.ui \
         presupuestoclientebase.ui  \
         busquedareferenciabase.ui \
	 listtipoivabase.ui \
	 listtasaivabase.ui \
	 bancobase.ui

TRANSLATIONS += bulmafact_es.ts \
                bulmafact_ca.ts \
                bulmafact_en.ts \
                bulmafact_fr.ts \
                bulmafact_de.ts \
                bulmafact_zz.ts 

HEADERS += bulmafact.h \
           company.h \
           providerslist.h \
           provedit.h \
           clientslist.h \
           articulolist.h \
           presupuestoview.h \
           presupuestolist.h \
           articuloview.h \
           albaranclientelist.h \
           albaranclienteview.h \
           fpagoview.h \
           familiasview.h \
           tiposarticuloview.h \
           presupuesto.h \
           listlinpresupuestoview.h \
           facturaslist.h \
           facturaview.h \
           listlinfacturaview.h \
           listlinalbaranclienteview.h \
           pedidosclientelist.h \
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
           cobroview.h \
           cobroslist.h \
           facturasplist.h \
           listlinfacturapview.h \
           facturap.h \
           facturapview.h \
           listdescpresupuestoview.h \
           listdescpedidoclienteview.h \
           listdescalbaranclienteview.h \
           listdescfacturaview.h \
           informereferencia.h \
           trabajadorview.h \
           busquedatrabajador.h \
           comparticulolistview.h \
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
           cliente.h \
           clienteview.h \
           articulo.h \
           listalmacenview.h \
           listconfiguracionview.h \
           subform2bf.h \
           busquedareferencia.h \
	   fichabf.h \
	   busquedaregimenfiscal.h \
	   listtipoivaview.h \
	   listtasaivaview.h \
	   busquedatipoiva.h \
	   bancoview.h \
	   busquedabanco.h

SOURCES += bulmafact.cpp \
           main.cpp \
           company.cpp \
           providerslist.cpp \
           provedit.cpp \
           clientslist.cpp \
           articulolist.cpp \
           presupuestoview.cpp \
           presupuestolist.cpp \
           articuloview.cpp \
           albaranclientelist.cpp \
           albaranclienteview.cpp \
           fpagoview.cpp \
           familiasview.cpp \
           tiposarticuloview.cpp \
           presupuesto.cpp \
           listlinpresupuestoview.cpp \
           facturaslist.cpp \
           facturaview.cpp \
           listlinfacturaview.cpp \
           listlinalbaranclienteview.cpp \
           pedidosclientelist.cpp \
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
           comparticulolistview.cpp \
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
           cliente.cpp \
           clienteview.cpp \
           articulo.cpp \
           listalmacenview.cpp \
           listconfiguracionview.cpp \
           subform2bf.cpp \
           busquedareferencia.cpp \
	   fichabf.cpp \
	   busquedaregimenfiscal.cpp \
	   listtipoivaview.cpp \
	   listtasaivaview.cpp \
	   busquedatipoiva.cpp \
	   bancoview.cpp \
	   busquedabanco.cpp


INCLUDEPATH += ../../bulmalib/src \
../../bulmalib/src/.ui \
../../bulmalib/src/.moc \
/usr/include/qt4 \
/usr/include/qt4/Qt \
/usr/include/qt4/QtXml \
/usr/include/qt4/QtGui \
/usr/include/Qt \
/usr/include/QtXml \
/usr/lib/qt4/include/Qt \
/usr/lib/qt4/include/QtXml \
..

unix {
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
}

windows {
  DEFINES += WINDOWS
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  LIBS += ../../bulmalib/src/release/bulmalib0.dll \
          "C:\Archivos de programa\PostgreSQL\8.1\bin\libpq.dll"
  INCLUDEPATH += "C:\Archivos de programa\PostgreSQL\8.1\include"
  TARGET = ../../../installbulmages/bulmafact
}
