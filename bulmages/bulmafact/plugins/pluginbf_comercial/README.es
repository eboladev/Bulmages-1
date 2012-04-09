#######
#Plugins
#######
#README.es
#20080501

Nombre: Comerciales

Biblioteca: libpluginbf_comercial.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/comercial.png" WIDTH="100"/><BR>Comerciales</H1>
<P>Permite llevar la gestión de los comerciales de la empresa.</P>
<P>Primero permite establecer las <B>Zonas Comerciales</B> en las que se delimita el área de actuación</P>
<P>En la ficha de cliente se podrá especificar a que <B>Zona Comercial</B> pertenece y que forma de visita se establece.</P>
<P>En la ficha de <B>Trabajadores</B> Podremos establecer cuales son comerciales y cuales no</P>
<P> De esta forma podremos establecer un planing de visitas y de rutas a realizar.</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: PluginBf_Comercial

ArchivoSQLpatch: pluginbf_comercial.sql

ArchivoSQLdeins: pluginbf_comercial-rm.sql

Dependencias: libpluginbf_cliente.so; libpluginbf_trabajador.so

Incompatibles:

Reports: rutascomerciales.rml

Categorias: Facturacion generica;Experimentales;

Orden: 700

Comentarios: Imprescindible para empresas con comerciales.


