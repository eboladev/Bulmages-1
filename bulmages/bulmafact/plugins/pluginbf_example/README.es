#######
#Plugins
#######
#README.es
#20080501

Nombre: Demostración del funcionamiento de los plugins

Biblioteca: libpluginbf_example.so

Descripción: <H1>Ejemplo</H1>
<P>Sirve para demostrar el funcionamiento de los plugins. Es, principalmente
un ejemplo de programacion.</P>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Categorias: Utilidades Avanzadas;

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


