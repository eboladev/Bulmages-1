#######
#Plugins
#######
#README.es
#20080501

Nombre: Control de presencia por centralita Asterisk

Biblioteca: libpluginbf_asterisk.so

Descripción: <H1>Asterisk</H1>
<P>Presenta todas las validaciones realizadas en los usuarios y en los almacenes correspondientes. Modifica la ficha de almacenes y trabajadores para que presenten un cuadro de validaciones realizadas.</P>
<P>Requiere el pluginalmacen y el plugincuadrante, sin los cuales no funcionará.
También incluye en el código fuente ejemplos de scritps de validacion hecho en php para ser
puestos en marcha en la centralita con agi.</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: PluginBf_Asterisk

ArchivoSQLpatch: pluginbf_asterisk.sql

ArchivoSQLdeins: pluginbf_asterisk-rm.sql

Dependencias: libpluginbf_almacen.so; libpluginbf_cuadrante.so

Incompatibles:

Categorias: Utilidades avanzadas; Experimentales;

Orden: 900

Comentarios: 
