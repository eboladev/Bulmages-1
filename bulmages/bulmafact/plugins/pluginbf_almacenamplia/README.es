#######
#Plugins
#######
#README.es
#20080501

Nombre: Ampliación de Almacenes

Biblioteca: libpluginbf_almacenamplia.so

Descripción: <H1>Ampliacion de Almacenes</H1>
<P>Crea la ficha de almacenes y amplia la información que en ella se presenta.
Sirve para aquellas gestiones que requieran de un uso mejorado y ampliable de los almacenes.</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias: libpluginbf_almacen.so

Incompatibles: 

Categorias: Facturacion avanzada; Experimentales;Distribución;Distribución Alimentaria;Alquileres;

Orden: 860

Comentarios: Imprescindible para empresas con múltiples almacenes.

Instalacion
===========

Configure la directiva CONF_PLUGINS_BULMAFACT para que contenga la llamada a este plugin. Dicha directiva
puede ser a modo global en el archivo /etc/bulmages.conf o personalizada con los archivos /etc/bulmafact_<<database>>.conf

