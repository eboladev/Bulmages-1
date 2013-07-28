#######
#Plugins
#######
#README.es
#20080501

Nombre: Bloqueo de fichas

Biblioteca: libpluginbl_formlock.so

Descripción: <H1>Bloqueos</H1>
<P>Detecta la apertura duplicada de fichas y avisa al usuario si la
ficha que esta intentando abrir ya esta siendo utilizada. El plugin permite ver la ficha aunque desabilita los botones de guardado, eliminación y de aceptar.</P>
<P>Este plugin es importante en entornos multi-usuario ya que sin el se puede cometer facilmente un cambio que sea borrado por otro usuario.</P>
<P>Este plugin mantiene una tabla de bloqueos en la que almacena la tabla de ficha, el identificador y el usuario que la ha abierto. Al destruir la ficha el plugin elimina la referencia correspondiente de la tabla de bloqueos.</P>
<BR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/logopeq.png"/>
<HR>

ParmBD: PluginBl_FormLock

ArchivoSQLpatch: pluginbl_formlock.sql

ArchivoSQLdeins: pluginbl_formlock-rm.sql

Dependencias:

Incompatibles:

Categorias: Utilidades genericas;Experimentales;

Orden: 800

Comentarios: 
