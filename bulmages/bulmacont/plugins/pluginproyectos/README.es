#######
#Plugins
#######
#README.es
#20080512

Nombre: Proyectos Contables

Biblioteca: libpluginproyectos.so

Descripción: <H1>Proyectos Contables</H1>
<P>Permite el control de proyectos a nivel presupuestario en la contabilidad.</P>
<HR>

ParmBD: DBRev-PresContables

ArchivoSQLpatch: rev-pluginpresupuestos.sql

ArchivoSQLdeins: -

Dependencias:

Incompatibles:

Categorias: Contabilidad General;Experimentales;

Orden: 90

Comentarios: Para instalar debe aplicar el parche ubicado en dbmodels a las bases de datos elegidas para funcionar con este plugin.

Configure la directiva CONF_PLUGINS_BULMACONT para que contenga la llamada a este plugin. Dicha directiva
puede ser a modo global en el archivo /etc/bulmages.conf o personalizada con los archivos /etc/bulmacont_<<database>>.conf


