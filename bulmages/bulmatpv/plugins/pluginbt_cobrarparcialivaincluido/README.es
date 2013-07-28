##################################
#PluginBt_CobrarParcialIVAincluido
##################################
#README.es
#20110509

Nombre: Cobrar parcial IVA Incluido

Biblioteca: libpluginbt_cobrarparcialivaincluido.so

Descripción: <H1>Cobrar parcial IVA incluido</H1>
<P>Divide el ticket para poder cobrarlo por partes.</P>
<B>Author:</B>Fco. Javier M. C. (fcojavmc@todo-redes.com)<BR>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias: libpluginbt_cobrarivaincluido.so

Incompatibles:

Categorias: Utilidades avanzadas; Restauración; Parques Infantiles;

Orden: 90

Comentarios: 

Compilacion
===========

El plugin se compilará con el resto de bulmages y  se ubicara en bulmages/installbulmages/plugins con el nombre libpluginbt_cobrarparcialivaincluido.so



Instalacion
===========

Configure la directiva CONF_PLUGINS_BULMATPV para que contenga la llamada a este plugin. Dicha directiva
puede ser a modo global en el archivo /etc/bulmages.conf o personalizada con los archivos /etc/bulmatpv_<<database>>.conf



