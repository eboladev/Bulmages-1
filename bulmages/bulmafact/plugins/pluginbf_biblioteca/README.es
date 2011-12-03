#######
#Plugins
#######
#README.es
#20080501

Nombre: Biblioteca (AutoForms)

Biblioteca: 

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/product.svg" WIDTH="100"/><BR>Gestion de biblioteca</H1>
<PARA>Este plugin realiza la gestion de una sencilla biblioteca. </PARA>
<HR>
<PARA>Permite catalogar los libros, gestion de autores, prestamos y devoluciones.</PARA>
<BR>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD:

ArchivoSQLpatch: pluginbf_biblioteca.sql

ArchivoSQLdeins: pluginbf_biblioteca-rm.sql

Dependencias: libpluginbl_autoform.so

Incompatibles:

Orden: 29

Reports: articulos.rml; articuloetiqueta.rml; familias.rml; blform_ArticuloView.qs; impers_articulo_estadisticas_comparativa.rml

Categorias: AutoForms;Biblioteca;Experimentales;

Comentarios: Este plugin sirve como ejemplo del uso de AutoForms.

AutoFormFile: autoform_pluginbf_biblioteca_spec.spc.es


