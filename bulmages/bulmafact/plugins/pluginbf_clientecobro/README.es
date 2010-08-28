#######
#Plugins
#######
#README.es
#20080501

Nombre: Cobros

Biblioteca: libpluginbf_clientecobro.so

Descripción: <H1>Cobros</H1>
<P>Gestionar los cobros realizados a los clientes.</P>
<P>Este plugin realiza una gestión simple de los cobros a clientes. Permitiendo hacer previsiones de cobros futuros</P>
<P>Al realizar una factura nos permite generar el cobro para dicha factura mediante un botón en la ficha de factura. También nos permite hacer cobros de forma independiente a través de las opciones de menu "Nuevo Cobro"
</P>
<P> Por defecto los cobros efectuados quedan marcados como previsiones de cobro y se pueden procesar cuando el cobro es efectivo. Los cobros efectuados a través de una factura tienen la misma referencia que
la factura. El sistema detecta cuando una factura ha sido completamente pagada sumando las cantidades de los cobros y contrastandolos con el total de la factura. En dicho caso se preguntará si se desa
procesar la factura puesto que ya ha sido completamente cobrada.</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD:

ArchivoSQLpatch:

ArchivoSQLdeins: 

Dependencias: libpluginbf_cliente.so

Incompatibles: libpluginbf_carteracobros.so

Reports: recibo.rml

Categorias: Facturacion generica;

Orden: 624

Comentarios:
