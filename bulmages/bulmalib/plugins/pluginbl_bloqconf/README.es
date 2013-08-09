#######
#Plugins
#######
#README.es
20080501

Nombre: Bloquear configuraciones

Biblioteca: libpluginbl_bloqconf.so

Descripción: <H1>Bloquear configuraciones (Sólo Linux)</H1>
<P>Permite bloquear las configuraciones de presentacion de subformularios para que en cada carga del programa las configuraciones no se alteren.</P>
<P>Una vez instalado aparecen dos opciones nuevas en el menu Herramientas que permiten bloquear y desbloquear los archivos de configuracion</P>
<P>Este plugin cambia los permisos de los archivos del directorio que almacena las configuraciones (normalmente .bulmages) y los establece a modo de sólo lectura para protegerlos de posteriores escrituras.</P>
<h2>Instalación</h2>
<P>Este plugin no requiere de ningún tipo de atención tanto si es instalado automáticamente como con BulmaSetup. Debe tenerse en cuenta que éste sólo funciona con Linux</P>
<P>También debe tenerse en cuenta, si se utiliza el directorio de datos compartido, que todos los usuarios tengan permisos suficientes en los archivos de configuración o el módulo.</P>
<BR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/logopeq.png"/>
<HR>

ParmBD: -

ArchivoSQLpatch: -

ArchivoSQLdeins: -

Dependencias:

Incompatibles:

Categorias: Utilidades genericas;Experimentales;

Orden: 70

Comentarios: -

