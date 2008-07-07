#######
#Plugins
#######
#README.es
#20080501

Nombre: Ampliación de Almacenes

Biblioteca: libpluginalmacen.so

Descripción: Crea la ficha de almacenes y amplia la información que en ella se presenta.
Sirve para aquellas gestiones que requieran de un uso mejorado y ampliable de los almacenes.

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Orden:

Comentarios: Imprescindible para empresas con múltiples almacenes.

Instalacion
===========

Configure la directiva CONF_PLUGINS_BULMAFACT para que contenga la llamada a este plugin. Dicha directiva
puede ser a modo global en el archivo /etc/bulmages.conf o personalizada con los archivos /etc/bulmafact_<<database>>.conf

