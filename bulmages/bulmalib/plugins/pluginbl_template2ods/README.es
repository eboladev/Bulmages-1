#######
#Plugins
#######
#README.es
20080502

Nombre: Informes personalizados de ficha en ODS

Biblioteca: libpluginbl_template2ods.so

DescripciÃ³n: <H1>Informes personalizados ODS</H1>
<P>Permite la generacion de informes personalizados relacionados con
una ficha generica. Los informes realizados mediante plantillas PYS (Python) generan
documentos en hoja de calculo.</P>
<P>Una vez instalado el plugin al pulsar con el botÃ³n derecho sobre cualquier zona en blanco de la pantalla
aparecera una nueva opcion de menu <B>Informes Personales ODS</P> que desplegara los posibles informes personales
que haya para dicha ficha</P>
<P>Las plantillas PYS son, bÃ¡sicamente, archivos en python que antes de su ejecucion son interpretados por
BulmaGes sustituyendo los tags especiales por sus correspondientes valores</P>
<P>Para ligar una plantilla con una ficha el programa buscara todos aquellos archivos cuyo nombre empiece por impers_ y contenga el
nombre de la tabla asociada a dicha ficha</P>
<P>El plugin utiliza el parÃmetro de configuraciÃn <B>CONF_ODS</B> para abrir los documentos generados.</P>
<P>Visite <A HREF="http://www.iglues.org/wiki1/doku.php?id=bulmages:rml" TARGET="_blank">http://www.iglues.org/wiki1/doku.php?id=bulmages:rml</A> para encontrar informacion mas detallada</P>
<BR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/logopeq.png"/>
<HR>

ParmBD:

ArchivoSQLpatch:

ArchivoSQLdeins:

Dependencias:

Incompatibles:

Orden: 60

Categorias: Utilidades genericas; Colmados; Ferreter¨ªas; Droguer¨ªas;Facturacion generica;Talleres;Distribuci¨®n; Supermercados;Distribuci¨®n Alimentaria;Servicios Generales;Alquileres;Restauraci¨®n; Parques Infantiles; Tallas y Colores;


Comentarios: Su funcionamiento se basa en que haya un documento RML cuyo nombre
contenga el nombre de la tabla a la que representa la ficha. De este modo el
informe aparecerÃ¡ como opcion al pulsar sobre la opcion informes personalizados
en el menu contextual de la ficha.

(c) 2007 AsociaciÃ³n IGLUES. Software Bajo Licencia G.P.L.

