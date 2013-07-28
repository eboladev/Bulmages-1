#######
#Plugins
#######
#README.es
#20080501

Nombre: Pedidos a Proveedor

Biblioteca: libpluginbf_proveedorpedido.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/supplier_purchase_order_new.svg" WIDTH="100"/><BR>Pedidos a proveedores</H1>
<P>Soporte para emisión de pedidos a proveedores.</P>
<P>Al cargar el plugin tendremos dos nuevas entradas de meńu <B>Listado de Pedidos a Proveedores <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/supplier_purchase_order_list.svg" WIDTH="22"/></B> desde la que accederemos al listado de pedidos emitidos y <B>Nuevo Pedido a Proveedor<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/supplier_purchase_order_new.svg" WIDTH="22"/></B> desde el que podremos emitir una nueva pedido.</P>
<P> Mediante el listado de pedidos podremos controlar todos los pedidos emitidas, editarlas y borrarlas. Para ello podemos utilizar la barra de botones superior del listado.</P>
<P> Es común en todos los listados de BulmaGés poder configurarlos presentando los campos que nos interese. Para ello tenemos el <B>menú del subformulario <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/48x48/toggleconfig.png" WIDTH="22"/></B> o la opción <B>Configurar Listado <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/document_properties.svg" WIDTH="22"/></B> que nos presenta en el lateral derecho del listado todos los campos mostrables por el mismo, opciones de paginación y la consulta de base de datos que genera dicho listado</P>
<P><B>Nota:</B>Puede haber opciones ocultas para usted que se activan mediante la pulsación del botón derecho del ratón en determinadas zonas. Estas opciones pueden verse de forma permanente como una barra de herramientas mediante la acción <B>Mostrar Menu SubFormulario <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/48x48/togglemenu.png" WIDTH="22"/></B> Recuerde que el botón derecho del ratón puede ser su mejor aliado en la búsqueda de determinadas acciones.</P>
<P>Al crear un nuevo pedido los datos necesarios a rellenar son el proveedor y el almacen. El resto de campos pueden
dejarse en blanco para ser rellenados posteriormente. Los campos que aparecen en la pantalla pueden variar según la configuración
de plugins que tengamos y que precisen de información adicional en los pedidos, dicha información puede ser
obligatoria o no dependiendo del plugin en cuestión</P>
<P>Una vez rellenados los datos podemos guardar la pedido o darle al boton <B>Aceptar</B> para almacenar los cambios</P>
<P>Todos las pedidos tienen una plantilla de impresión por defecto llamada <B>pedidoproveedor.rml</B> que define la forma en que se
imprime el pedido al pulsar sobre el botón imprimir. Además pueden existir informes adicionales que aparecen al pulsar
con el botón derecho del ratón en cualquier espacio vacio de la pantalla del pedido así como acciones ocultas no
accesibles de otro modo</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD:

ArchivoSQLpatch:

ArchivoSQLdeins: 

Dependencias: libpluginbf_proveedor.so

Incompatibles:

Reports: pedidoproveedor.rml; blform_PedidoProveedorView.qs

Categorias: Colmados; Ferreterías; Droguerías;Facturacion generica;Talleres;Distribución; Supermercados;Distribución Alimentaria;Servicios Generales;Alquileres;Restauración; Parques Infantiles; Tallas y Colores;

Orden: 630

Comentarios:
