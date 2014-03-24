#######
#Plugins
#######
#README.es
20080501

Nombre: Listados y Formularios Automaticos

Biblioteca: libpluginbl_autoform.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/autoforms.png" WIDTH="48"/><BR>Listados y Formularios Automaticos (AutoForms)</H1>
<P>Permite crear pantallas y listados de forma automática sin necesidad de programar. Para ello utiliza el archivo de especificaciones autoform_database_specs.spc donde se describen los formularios y listados a crear.</P>
<P>Autoforms utiliza un archivo de especificacion de pantallas escrito en XML donde se describen dos tipos de formularios <FORM> que describe un formulario en correspondencia con una tabla de la base de datos y <FORMLIST> que describe un listado basado en una consulta sobre una tabla de la base de datos.</P>
<P>El sistema permite, de forma automática, enlazar los listados (que funcionan sobre una tabla) con la ficha correspondiente a esta tabla. Además facilita los sistemas de búsqueda y filtrado en los listados.</P>
<P>Se pueden describir los componentes especiales BlComboBox ,BlSearchWidget y demás que permiten gestionar las claves foraneas, tablas relacionadas y vistas de la forma eficaz</P>
<P>AutoForms está basado en las mismas clases de BulmaLib que los componentes programados. Con lo que todos los plugins de BulmaLib funcionaran perfectamente con los componentes de AutoForms</P>
<P>Para los formularios de autoforms se pueden crear todas las plantillas e informes necesarios con RML o Python extendiendo de forma sencilla la impresión de informes y documentos</P>
<P>Los módulos de autoforms pueden ser instalados con BulmaSetup convirtiéndose no sólo en componentes de personalización sino también en componentes de uso genérico. Estando disponibles algunos componentes realizados únicamente con AutoForms</P>
<P>Para mas información sobre el  modulo AutoForms, y sobre la creación de plantillas visitar <A HREF="http://bulmages.wordpress.com/2011/11/07/autoforms-en-bulmages/">http://bulmages.wordpress.com/2011/11/07/autoforms-en-bulmages/</A></P>
<BR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/logopeq.png"/>
<HR>

ParmBD: -

ArchivoSQLpatch: -

ArchivoSQLdeins: -

Dependencias:

Incompatibles:

Categorias: Experimentales;AutoForms;

Orden: 70

Comentarios: -

