#######
#Plugins
#######
#README.es
#20080501

Nombre: Sincronización con BulmaCont

Biblioteca: 

Descripción: <H1>Sincronización con BulmaCont</H1>
<P>Permite la sincronización del TPV con Bulmacont</P>
<HR>

ParmBD: PluginBt_SincroBulmaCont

ArchivoSQLpatch: pluginbt_sincrobulmacont.sql

ArchivoSQLdeins: pluginbt_sincrobulmacont-rm.sql

Dependencias:

Incompatibles:

Orden:

Comentarios: Precisa de tener instalado dblink.
No se instala propiamente como un plugin ya que no lleva librerías asociadas.


Prerequisitos:

Debe instalarse DBlink que esta en el paquete postgresql-contrib (distros Debian o Ubuntu)

para poner DBlink en la base de datos seleccionada debe hacerse:

	$ psql database < dblink.sql

Instalacion del plugin
----------------------

Debe ejecutarse el script de funcionamiento en la base de datos seleccionada.

Ejemplo:
	$ psql bulmafact < revf-pluginsyncbulmacont.sql

Una vez instalado el plugin debe configurarse el parametro de la facturacion DataBaseContabilidad y especificar en dicho parametro la base de datos de contabilidad que se debe actualizar automaticamente.

Una vez hechos estos pasos todas las operaciones que realicemos con faturacion serán traspasadas a contabilidad.


Notas
-----

En este plugin se proporcionan dos scripts adicionales.

   - El primero quita toda la funcionalidad de sincronizacion de una base de datos de facturacion.

   - El segundo hace un sincronizado de TODOS los datos (util para bases de datos ya formadas).


Mediante estos scripts se puede hacer una sincronizacion manual en lugar de una sincronizacion automatica.


Las opciones de sincronizacion son ''standar'' si quiere opciones de sincronización especiales modifique los scripts
para que se ajusten a sus necesidades.