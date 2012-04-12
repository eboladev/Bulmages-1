
!include "NSIS_includes_comunes\bulmafact\es_ES\include_remove_bulmafact_es_ES.nsh"

!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_tutor_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_alumno_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_minicontabilidad_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_recibo_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_actividad_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_profesor_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_asociacion_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_inventariosimple_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_formasdepago_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_banco_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbf\es_ES\include_remove_pluginbf_correo_es_ES.nsh"


!include "NSIS_includes_comunes\bulmalib\es_ES\include_remove_bulmalib_es_ES.nsh"

!include "NSIS_includes_comunes\pluginbl\es_ES\include_remove_pluginbl_subform2ods_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbl\es_ES\include_remove_pluginbl_subform2pdf_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbl\es_ES\include_remove_pluginbl_template_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbl\es_ES\include_remove_pluginbl_attachdocument_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbl\es_ES\include_remove_pluginbl_logo_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbl\es_ES\include_remove_pluginbl_report_es_ES.nsh"
!include "NSIS_includes_comunes\pluginbl\es_ES\include_remove_pluginbl_report2ods_es_ES.nsh"


; Borra los directorios de traducciones si estan vacios.
StrCpy $0 "$INSTDIR\es_ES\LC_MESSAGES"
Call un.DeleteDirIfEmpty

StrCpy $0 "$INSTDIR\es_ES"
Call un.DeleteDirIfEmpty


