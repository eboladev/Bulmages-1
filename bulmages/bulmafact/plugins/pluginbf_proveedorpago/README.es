#######
#Plugins
#######
#README.es
#20080501

Nombre: Pagos

Biblioteca: libpluginbf_proveedorpago.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/default-spa/mimetypes/pay.svg" WIDTH="100"/><BR>Pagos a proveedores</H1>
<P>Gestion de pagos a proveedores.</P>
<P>Mediante este plugin podremos gestionar de forma sencilla los pagos a proveedores.</P>
<P>Para ello dispondremos en los documentos de proveedor (pedidos, albaranes y facturas) de un botón para generar
pagos asociados a sus referencias.</P>
<P>También dispondremos de un listado de pagos para ver todos los pagos registrados y poder realizar pagos nuevos.<P>
<P>Cuando realizamos un pago se comprueba que todas las facturas con la misma referencia de ese pago esten casadas en cantidad
con todos los pagos que tienen la misma referencia. En cuyo caso se nos pedirá para procesar todos los pagos
de una determinada referencia</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD:

ArchivoSQLpatch:

ArchivoSQLdeins: 

Dependencias: libpluginbf_proveedor.so

Incompatibles: libpluginbf_carterapagos.so

Categorias: Colmados; Ferreterías; Droguerías;Facturacion generica;Talleres;Alquileres;

Reports: blform_PagoView.qs

Orden: 633

Comentarios:
