#######
#Plugins
#######
#README.es
#20090501

Nombre: Mini-Contabilidad

Biblioteca: libpluginbf_minicontabilidad.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/balance_sheet.svg" WIDTH="100"/><BR>Minicontabilidad</H1>
<PARA>Este plugin permite realizar una contabilidad sencilla. </PARA>
<H2><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/account_plan.svg" WIDTH="48"/>Partidas Contables</H2>
<PARA>La opción del menu <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/account_plan.svg" WIDTH="22"/><b>Economia/Partidas Contables</b> permite gestionar las diferentes partidas de imputación de la contabilidad. Estas partidas estan gestionadas en forma de arbol haciendo que una partida este compuesta por otras partidas. Cada partida lleva asociado un codigo que la identifica
de forma única. </PARA>
<H2><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/balance_sheet.svg" WIDTH="48"/>Apuntes Contables</H2>
<PARA>Presenta la opcion de introducir ingresos y gastos por partidas para generar un balance contable sencillo. Para poder introducir anotaciones contables es preciso haber introducido previamente la partida. En la tabla asociada los campos a rellenar son <B>partida</B><B>Ingreso</B><B>Gasto</B> indicando de esta forma todas las transacciones monetarias.</PARA>
<H2><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/account.svg" WIDTH="48"/>Previsiones</H2>
<PARA>Esta opción, muy similar a la anotacion de apuntes contables permite la consolidación de previsiones o de partidas presupuestarias. Su función es informativa para la redacción de presupuestación contable.</PARA>
<PARA>Una vez lanzada la opción <B>Economia/Previsiones</B> accederemos a un listado de los presupuestos realizados. A través de esta ventana podremos crear, modificar y eliminar los presupuestos que queramos.</PARA>
<PARA>Al pulsar el botón nuevo tendremos la posibilidad de crear un nuevo presupuesto. Rellenando los datos:</PARA>
<OL>
<LI> Descripción de presupuesto</LI>
<LI> Fecha de creación del presupuesto</LI>
<LI> Subformulario de la prevision (Partida - Concepto - Saldo</LI>
</OL>
<BR>
<PARA>Tambien permite contrastar con los presupuestos realizados con la realidad contable imputada.</PARA>
<B>Nota:</B> Este sistema contable es válido para pequeñas entidades y de forma informal. Para empresas medianas es preferible usar un software específico de contabilidad como puede ser <B>BulmaCont</B>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: PluginBf_MiniContabilidad

ArchivoSQLpatch: pluginbf_minicontabilidad.sql

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Orden: 210

Reports: presupuestocontable.pys; presupuestocontable.rml; blform_PresupuestoContableView.qs; inf_ventas.ui; inf_informeeconomico.rml; inf_balance.rml; inf_diario.rml

Categorias: Asociaciones de padres de alumnos; Experimentales;

Comentarios:
