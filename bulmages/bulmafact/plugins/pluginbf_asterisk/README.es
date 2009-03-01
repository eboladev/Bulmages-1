#######
#Plugins
#######
#README.es
#20080501

Nombre: Control de Presencia por Centralita Asterisk

Biblioteca: libpluginbf_asterisk.so

Descripción: <H1>Asterisk</H1>
<P>Presenta todas las validaciones realizadas en los usuarios y en los almacenes correspondientes. Modifica la ficha de almacenes y trabajadores para que presenten un cuadro de validaciones realizadas.</P>
<P>Requiere el pluginalmacen y el plugincuadrante, sin los cuales no funcionará.
También incluye en el código fuente ejemplos de scritps de validacion hecho en php para ser
puestos en marcha en la centralita con agi.</P>
<HR>

ParmBD: DBRev-ValAsterisk

ArchivoSQLpatch: revf-pluginasterisk.sql

ArchivoSQLdeins: 

Dependencias: libpluginalmacen.so libplugincuadrante.so

Incompatibles:

Categorias: Utilidades Avanzadas; Experimentales;

Orden: 900

Comentarios: 
