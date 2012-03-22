#######
#Plugins
#######
#README.es
#20080501

Nombre: Informes personalizados

Biblioteca: libpluginbl_report.so

Descripción: <H1>Informes personalizados</H1>
<P>Presenta un nuevo menu Informes que permite lanzar los informes personalizados
de que se disponga. Dichos informes son plantillas RML ubicadas en el directorio openreports.</P>
<P>Los archivos deben empezar por inf_ y tener la extension .rml para aparecer en el menu</P>
<P>Consulte la página http://www.iglues.org/wiki_users/doku.php?id=bulmages:rml para mas informacion sobre el formato RML</P>
<P>Tambien genera informes del tipo ESC/POS y los saca por la ticketera. Es ese caso los informes son archivos <b>inf_*.txt</b></P>
<BR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/logopeq.png"/>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Categorias: Utilidades genericas;

Orden: 940

Comentarios: Util para realizar informes de forma rápida sin tener que modificar el codigo fuente ni hacer un plugin específico para cada uno de ellos.
