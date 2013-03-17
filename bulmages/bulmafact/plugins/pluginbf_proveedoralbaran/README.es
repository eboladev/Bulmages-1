#######
#Plugins
#######
#README.es
#20080501

Nombre: Albaranes de Proveedor

Biblioteca: libpluginbf_proveedoralbaran.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/supplier_delivery_note_new.svg" WIDTH="100"/><BR>Albaranres de proveedores</H1>
<P>Soporte para emisión de albaranres de proveedores.</P>
<P>Al cargar el plugin tendremos dos nuevas entradas de meńu <B>Listado de Albaranres de Proveedores <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/supplier_delivery_note_list.svg" WIDTH="22"/></B> desde la que accederemos al listado de albarans emitidos y <B>Nuevo Pedido a Proveedor<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/supplier_delivery_note_new.svg" WIDTH="22"/></B> desde el que podremos emitir una nueva albaran.</P>
<P> Mediante el listado de albarans podremos controlar todos los albarans emitidas, editarlas y borrarlas. Para ello podemos utilizar la barra de botones superior del listado.</P>
<P> Es común en todos los listados de BulmaGés poder configurarlos presentando los campos que nos interese. Para ello tenemos el <B>menú del subformulario <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/48x48/toggleconfig.png" WIDTH="22"/></B> o la opción <B>Configurar Listado <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/document_properties.svg" WIDTH="22"/></B> que nos presenta en el lateral derecho del listado todos los campos mostrables por el mismo, opciones de paginación y la consulta de base de datos que genera dicho listado</P>
<P><B>Nota:</B>Puede haber opciones ocultas para usted que se activan mediante la pulsación del botón derecho del ratón en determinadas zonas. Estas opciones pueden verse de forma permanente como una barra de herramientas mediante la acción <B>Mostrar Menu SubFormulario <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/48x48/togglemenu.png" WIDTH="22"/></B> Recuerde que el botón derecho del ratón puede ser su mejor aliado en la búsqueda de determinadas acciones.</P>
<P>Al crear un nuevo albaran los datos necesarios a rellenar son el proveedor y el almacen. El resto de campos pueden
dejarse en blanco para ser rellenados posteriormente. Los campos que aparecen en la pantalla pueden variar según la configuración
de plugins que tengamos y que precisen de información adicional en los albaranes, dicha información puede ser
obligatoria o no dependiendo del plugin en cuestión</P>
<P>Una vez rellenados los datos podemos guardar la albaran o darle al boton <B>Aceptar</B> para almacenar los cambios</P>
<P>Todos las albarans tienen una plantilla de impresión por defecto llamada <B>albaranproveedor.rml</B> que define la forma en que se
imprime el albaran del pulsar sobre el botón imprimir. Además pueden existir informes adicionales que aparecen al pulsar
con el botón derecho del ratón en cualquier espacio vacio de la pantalla del albaran así como acciones ocultas no
accesibles de otro modo</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD:

ArchivoSQLpatch:

ArchivoSQLdeins: 

Dependencias: libpluginbf_proveedor.so

Incompatibles:

Categorias: Colmados;Facturacion generica;Talleres;Distribución;Distribución Alimentaria;Servicios Generales;Restauración;

Reports: blform_AlbaranProveedorView.qs

Orden: 631

Comentarios:
