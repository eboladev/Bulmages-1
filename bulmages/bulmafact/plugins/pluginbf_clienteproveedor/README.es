#######
#Plugins
#######
#README.es
#20080501

Nombre: Alta de proveedor conjunta con alta de cliente

Biblioteca: libpluginbf_clienteproveedor.so

Descripción: <H1>Crea un proveedor cada vez que se crea un cliente, asignandole el mismo CIF y utiliza los datos de este en el proveedor. Es frecuente tener clientes y proveedores juntos principalmente en operaciones de compra-venta.</H1>
<P>Crea un proveedor cada vez que se crea un cliente. Y utiliza los datos de este.</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias: libpluginbf_cliente.so; libpluginbf_proveedor.so

Incompatibles:

Categorias: Facturacion avanzada; Experimentales;

Orden: 900

Comentarios: Ideal para negocios en los que los clientes son a la vez cliente y proveedor.
