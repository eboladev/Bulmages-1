#######
#Plugins
#######
#README.es
#20080501

Nombre: Contratos con Clientes

Biblioteca: libpluginbf_contrato.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/contract.svg" WIDTH="100"/><BR>Contratos</H1>
<P>Permite la gestión de contratos con clientes. A partir de dichos contratos se permite una facturación automática periódica.</P>
<P>Una vez instalado el plugin en <B>BulmaFact</B> tendremos la nueva opción de menu <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/contract_list.svg" WIDTH="32"/>  <B>Contratos</B> que nos permitirá hacer todas las gestiones con contratos a nuestros clientes</P>
<P> Si pulsamos sobre esta nueva opción obtendremos el listado de contratos realizados con nuestros clientes. El manejo de esta ventana de listados es identico al del resto de ventanas de listados del programa. Además en la botonera superior aparecerá el botón <B>Facturar Periodos Pendientes</B> que nos permite generar todas las facturas pendientes por contrato</P>
<P>Desde la ventana de creación/edición de contratos debemos introducir, el cliente, las líneas de factura que se facturan, la fecha de inicialización y de finalización del contrato y la periodicidad de emisión de las facturas. Desde esta misma ventana también podemos generar todas las facturas pendientes relacionadas con este contrato en particular.</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: PluginBf_Contrato

ArchivoSQLpatch: pluginbf_contrato.sql

ArchivoSQLdeins: pluginbf_contrato-rm.sql

Dependencias: libpluginbf_clientefactura.so

Incompatibles:

Categorias: Facturacion avanzada;Distribución;Distribución Alimentaria;Servicios Generales;Alquileres;

Orden: 830

Comentarios: Muy útil en sistemas de facturación periódicos.
