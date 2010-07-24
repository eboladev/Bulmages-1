#######
#Plugins
#######
#README.es
#20080501

Nombre: Impresion en Cocina

Biblioteca: 

Descripción: <H1>Impresion en Cocina</H1>
<P>Permite la impresion de pedidos generados en bulmatpv en la cocina</>
<P> Este componente modifica la base de datos para agregarle los campos necesarios para la impresion en cocina.</P>
<P> Por ahora no incorpora interfaz para el manejo de impresoras de cocina. Pero se puede lograr
facilmente creando un archivo /etc/bulmages/ArticuloBase_bulmafact_spec.spc</P>
<P> Y poner el contenido: 
<PRE>
<FICHA>
    <CAMPO>
        <NOMCAMPO>idprintercocina</NOMCAMPO>
        <NOMPCAMPO>Impresora de Cocina</NOMPCAMPO>
        <DBTYPECAMPO>DBINT</DBTYPECAMPO>
        <RESTRICTIONSCAMPO>DBNOTHING</RESTRICTIONSCAMPO>
    </CAMPO>
</FICHA>

</PRE>
</P>
<HR>

ParmBD: PluginBf_PrinterCocina

ArchivoSQLpatch: pluginbf_printercocina.sql

ArchivoSQLdeins: pluginbf_printercocina-rm.sql

Dependencias:

Incompatibles:

Categorias: Utilidades avanzadas; Experimentales; Restauracion;

Orden: 900

Comentarios: No se instala propiamente como un plugin ya que no lleva librerías asociadas.


Prerequisitos:
