#######
#Plugins
#######
#README.es
#20080501

Nombre: Informes personalizados

Biblioteca: libpluginbl_report.so



Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/oxygen/actions/48x48/application-pdf.png" WIDTH="48"/>Informes personalizados</H1>
<P>Presenta un nuevo menu Informes <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/oxygen/actions/48x48/application-pdf.png" WIDTH="16"/> que permite acceder a los informes personalizados instalados para la empresa. Dichos informes son plantillas RML ubicadas en el directorio openreports de la empresa.</P>
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

Categorias: Utilidades genericas; Colmados; Ferreterías; Droguerías;Facturacion generica;Talleres;Distribución; Supermercados;Distribución Alimentaria;Servicios Generales;Alquileres;Restauración; Parques Infantiles; Tallas y Colores;

Orden: 940

Comentarios: Util para realizar informes de forma rápida sin tener que modificar el codigo fuente ni hacer un plugin específico para cada uno de ellos.
