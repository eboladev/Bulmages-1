#######
#Plugins
#######
#README.es
#20080501

Nombre: Alquileres (AutoForms)

Biblioteca: libpluginbf_alquileres

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/pluginbf_alquileres/vivienda.png" WIDTH="48"/><BR>Gestion de alquileres</H1>
<PARA>Este plugin realiza la gestion de una sencilla de arrendamientos. </PARA>
<HR>
<PARA>Permite controlar contratos, recibos de luz y agua y facturar a los inquilinos.</PARA>
<BR>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD:

ArchivoSQLpatch: pluginbf_alquileres.sql

ArchivoSQLdeins: pluginbf_alquileres-rm.sql

Dependencias: libpluginbl_autoform.so; libpluginbl_template.so; libpluginbl_template2ods.so; libpluginbl_report.so; libpluginbl_report2ods.so;

Incompatibles:

Orden: 900

Reports: impers_contrato_alquiler.rml;impers_contrato_odt.pys;contrato_plantilla1.odt;contrato_plantilla2.odt;impers_contrato_odt2.pys

Categorias: AutoForms;Alquileres;Experimentales;

Comentarios: Este plugin sirve como ejemplo del uso de AutoForms.

AutoFormFile: autoform__pluginbf_alquileres_spec.spc.es; ContratoBase__pluginbf_alquileres_spec.spc.es;


