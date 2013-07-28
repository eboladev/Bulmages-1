#######
#Plugins
#######
#README.es
#20080501

Nombre: Cartera de Cobros

Biblioteca: libpluginbf_carteracobros.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/collect_list.svg" WIDTH="100"/><BR>Cartera de Cobros</H1>
<PARA>Permite gestion avanzada de cobros a clientes, con faccionado de facturas y control de vencimientos.</PARA>
<HR>
<PARA>Una vez instalado el plugin, en la ficha de cada cliente aparecerá la pestaña de vencimientos que permite indicar las fracciones (porcentajes) de cobro para cada factura que se emita a dicho cliente asi como los dias que deben transcurrir hasta que sea efectivo el vencimiento.</PARA>
<BR>
<PARA>Al crear una factura directamente se generarán los distintos cobros de la misma de forma automática utilizando los vencimientos que hayamos indicando para el cliente de la factura. Dichas previsiones
podran cambiarse manualmente.</PARA>
<BR>
<PARA>También incorpora una gestion de todos los cobros a realizar, permitiendo el remesado bancario y control de estado de cada uno de los
cobros</PARA>
<PARA>Es un buen sustituto, algo más elaborado que la gestion de cobros de factura. Útil para aquellas empresas que pretenden
llevar una gestión de cobros y ofrezcan facilidades de pago a sus clientes.</PARA><BR>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD:

ArchivoSQLpatch: revf-pluginbf_carteracobros.sql

ArchivoSQLdeins: 

Dependencias:

Incompatibles: libpluginbf_clientecobro.so

Orden: 450

Reports: impers_vencimientoc.rml

Categorias: Facturacion avanzada;Experimentales;Distribución;Distribución Alimentaria;

Comentarios:



