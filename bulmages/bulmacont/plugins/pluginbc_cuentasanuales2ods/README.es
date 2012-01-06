#######
#Plugins
#######
#README.es
20080502

Nombre: Cuentas Anuales 

Biblioteca: libpluginbc_cuentasanuales2ods.so

Descripción:<H1>Cuentas Anuales</H1>
<P>Permite obtener un balance de cuentas anuales para los planes
contables españoles del 2008: Cuentas Anuales, Abreviadas y de PYMES. Y para
los planes contables anteriores al 2008: Cuentas Anuales y Abreviadas de
Asociaciones sin lucro. </P>

ParmBD: -

ArchivoSQLpatch: -

ArchivoSQLdeins: -

Dependencias:

Incompatibles:

Categorias: Contabilidad general;

Orden: 70

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
