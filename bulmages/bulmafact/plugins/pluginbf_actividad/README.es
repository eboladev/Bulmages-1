#######
#Plugins
#######
#README.es
#20080501

Nombre: Actividades

Biblioteca: libpluginbf_actividad.so

Descripción: <H1>Actividades</H1>
<P>Gestionar las actividades llevadas a cabo por la asociacion.</P>
<P>Este plugin se enmarca dentro del control de asociaciones de padres y alumnos y sirve para gestionar las distintas actividades que lleva a cabo la asociación. Por ejemplo excursiones o actividades extraescolares. Las actividades tienen una serie de datos importantes (profesores, fechas, horarios, precios) y tienen asociados una lista de alumnos asistentes a la misma.</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/logopeq.png"/>
<HR>

ParmBD:

ArchivoSQLpatch:

ArchivoSQLdeins: 

Dependencias: libpluginbf_profesor.so; libpluginbf_alumno.so; libpluginbl_template.so; libpluginbl_report.so; libpluginbl_template2ods.so; libpluginbl_report2ods.so

Incompatibles:

Reports: actividad.rml; inf_calendario_semanal_actividades.rml; inf_calendario_actividades.rml; impers_actividad_inscripcion.rml; inf_material_mas_prestado.rml; impers_actividad_cartel.rml; impers_actividad_materialpendiente.rml; inf_prestamos_no_devueltos.rml; impers_actividad_faltas_de_asistencia.rml; impers_actividad_plantilla_notas.rml; impers_actividad_faltas.rml; impers_actividad_prestamo.rml; blform_ActividadView.qs; blform_ConvocatoriaView.qs; blform_TipoActividadView.qs


Categorias: Asociaciones de padres de alumnos; Experimentales;

Orden: 220

Comentarios:
