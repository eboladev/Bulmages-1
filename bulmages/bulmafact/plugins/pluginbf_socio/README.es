#######
#Plugins
#######
#README.es
#20080501

Nombre: Socios

Biblioteca: libpluginbf_socio.so

Descripción: <H1>Ejemplo</H1>
<P>Gestion de socios.</P>
<HR>

ParmBD: PluginBf_Socio

ArchivoSQLpatch: pluginbf_socio.sql

ArchivoSQLdeins: pluginbf_socio-rm.sql

Dependencias:

Incompatibles:

Categorias: Experimentales;

Orden: 90

Comentarios: 

Compilacion
===========

El plugin se compilará con el resto de bulmages y  se ubicara en bulmages/installbulmages/plugins con el nombre libplugin.so



Instalacion
===========

Configure la directiva CONF_PLUGINS_BULMAFACT para que contenga la llamada a este plugin. Dicha directiva
puede ser a modo global en el archivo /etc/bulmages.conf o personalizada con los archivos /etc/bulmafact_<<database>>.conf

Copie todos los archivos *.qm al directorio /usr/share/bulmages/traducciones


