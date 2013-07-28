#######
#Plugins
#######
#README.es
#20080501

Nombre: Informes personalizados en ODS

Biblioteca: libpluginbl_report2ods.so

Descripción: <H1>Informes personalizados en hoja de calculo</H1>
<P>Presenta un nuevo menu Informes que permite lanzar los informes personalizados
de que se disponga. Dichos informes son scripts Python con ooolib.</P>
<P>Los informes deben estar en formato PYS y deben llamarse inf_XXXX.pys para que aparezcan en el menu
<B>Informes personales ODS</B></P>
<P>El plugin utiliza el parametro de configuracion <B>CONF_ODS</B> para encontrar la ruta de la suite ofimatica que se encargaráde abrir el programa.
Asegurese que dicha ruta es correcta si nos informes no se abren, especialmente si es usuario de MS Windows</P>
<P>Existen varios ejemplos del formato pys en la seccion de informes de la instalación del programa.</P>
<BR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/logopeq.png"/>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Reports:

Categorias: Utilidades genericas; Colmados; Ferreterías; Droguerías;Facturacion generica;Talleres;Distribución; Supermercados;Distribución Alimentaria;Servicios Generales;Alquileres;Restauración; Parques Infantiles; Tallas y Colores;

Orden: 950

Comentarios: Util para realizar informes de forma rápida sin tener que modificar el codigo fuente ni hacer un plugin específico para cada uno de ellos.
