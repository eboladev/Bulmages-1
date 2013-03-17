#######
#Plugins
#######
#README.es
#20080501

Nombre: Inventarios

Biblioteca: libpluginbf_inventario.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/default-spa/mimetypes/product-family.svg" WIDTH="100"/><BR>Inventarios</H1>
<P>Creación de inventarios y control de stock. Inventarios en múltiples almacenes y con rehubicación.</P>
<P>Con este plugin se pueden realizar inventarios de forma periódica que modifican el stock de los artículos.</P>
<P>La forma de rellenar el inventario una vez creado es introducir los nuevos stocks en las casillas correspondientes</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: PluginBf_Inventario

ArchivoSQLpatch: pluginbf_inventario.sql

ArchivoSQLdeins: pluginbf_inventario-rm.sql

Dependencias: libpluginbf_articulo.so; libpluginbf_almacen.so

Incompatibles: libpluginbf_inventariosimple.so

Categorias: Colmados;Facturacion generica;Distribución;Distribución Alimentaria;Restauración;

Reports: inf_stockminimos.rml

Orden: 550

Comentarios: Imprescindible en los sistema de control de stock.
