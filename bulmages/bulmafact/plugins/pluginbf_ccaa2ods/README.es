#######
#Plugins
#######
#README.es
20090419

Nombre: Cuentas anuales 

Biblioteca: libpluginbf_ccaaods

Descripción: <H1>Cuentas anuales</H1>
<P>Permite obtener un balance de cuentas anuales para los planes contables españoles del 2007: Cuentas anuales, abreviadas y de pymes. Y para los planes contables anteriores al 2008: Cuentas anuales y abreviadas de asociaciones sin animo de lucro. </P>
<HR>

ParmBD: -

ArchivoSQLpatch: -

ArchivoSQLdeins: -

Dependencias:

Incompatibles:

Reports: inf_ccaa-pgc1990.pys; inf_ccaa-pgc2007_asl.pys; inf_ccaa-pgc2007_pymes.pys; inf_ccaa-pgc2007_abrev.pys; inf_ccaa-pgc2007_fac.pys; inf_ccaa-pgc2007.pys; fbalance.ui


Categorias: Contabilidad general;Experimentales;

Orden: 180

Comentarios: El formato de salida es ods. Esta basado en el plugincanualesods,
creado por Fco. Javier a partir de un código de Tomeu Borràs. A partir de esto,
Adeladia Clavaguera y Leopold Palomo crearon el ccaa2ods, que era las cuentas
anuales para las asociaciones sin lucro. Luego crearon reportods donde se
integró, canuales y ccaa2ods y se añadió el resto de cuentas anuales que existen
publicadas en el BOE. 

Entonces, viendo que reportods hacia lo mismo que los otros dos, se decide
substituir canuales por reportods integrando en un nuevo canuales todas las
cuentas anuales.

(c) 2008 Adelaida Clavaguera y Leopold Palomo.
(c) 2006 Fco. Javier M. C.   
(c) 2005 Tomeu Borras.
