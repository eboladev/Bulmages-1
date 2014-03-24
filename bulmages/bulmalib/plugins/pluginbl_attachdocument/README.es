#######
#Plugins
#######
#README.es
#20080801

Nombre: Adjuntar documentos

Biblioteca: libpluginbl_attachdocument.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/48x48/attach_document.png" WIDTH="48"/><BR>Adjuntar documentos</H1>
<P>Permite adjuntar archivos a fichas del programa para para poder acceder a ellos fácilmente y tenerlos siempre visibles junto a las fichas. Sólo almacena una referencia a la ubicación del documento con lo que el almacenado y protección del mismo, copias de seguridad y mantenimiento deben realizarse de forma independiente al programa.</P>
<P>Una vez instalado el plugin en todas las fichas del programa aparecerá la opción <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/48x48/attach_document.png" WIDTH="16"/> en el menú y en el menú contextual de la ficha que nos permitirá agregar referéncias a archivos existentes, abrir los archivos referenciados y borrarlas. Al borrar una referencia no se altera el archivo referenciado, éste permanecerá en su ubicación.</P>
<P>Este plugin puede funcionar perfectamente con sistemas de archivos compartidos, unidades en red, o discos en la nube teniendo las referencias siempre disponibles en todos los equipos que utilizan el programa. Es conveniente en estos casos utilizar siempre una ubicación que tenga la misma ruta de acceso. Unidad compartida o punto de montaje identicos en todos los equipos.</P>
<P>El modulo no altera el archivo documental ni depende de la estructuración de éste. Simplementa proporciona una forma sencilla de referenciar documentos y acceder a ellos desde el programa.</P>
<h2>Instalación</h2>
<P>Este plugin no necesita de acciones adicionales tras su instalación. Si se realiza mediante bulmasetup. En 
una instalación manual debe tenerse en cuenta que hay que aplicar el parche correspondiente a la base de datos.</P>
<BR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/logopeq.png"/>
<HR>

ParmBD: PluginBl_AttachDocument

ArchivoSQLpatch: pluginbl_attachdocument.sql

ArchivoSQLdeins: pluginbl_attachdocument-rm.sql

Dependencias:

Incompatibles:

Categorias: Asociaciones de padres de alumnos;Utilidades genericas;Experimentales; Colmados; Ferreterías; Droguerías;Facturacion generica;Talleres;Distribución; Supermercados;Distribución Alimentaria;Servicios Generales;Alquileres;Restauración; Parques Infantiles; Tallas y Colores;

Orden: 90

Comentarios:

