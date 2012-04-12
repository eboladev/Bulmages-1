;PLUGINS
SetOutPath "$INSTDIR\lib\bulmacont\plugins"

!include "NSIS_includes_comunes\pluginbc\include_install_pluginbc_cuenta.nsh"
!include "NSIS_includes_comunes\pluginbc\include_install_pluginbc_asiento.nsh"
!include "NSIS_includes_comunes\pluginbc\include_install_pluginbc_diario.nsh"
!include "NSIS_includes_comunes\pluginbc\include_install_pluginbc_mayor.nsh"
!include "NSIS_includes_comunes\pluginbc\include_install_pluginbc_balance.nsh"

!insertmacro InstallFilesFromBuild libpluginbc_amortizacion.dll  "\bulmacont\plugins\pluginbc_amortizacion\libpluginbc_amortizacion.dll"
!insertmacro InstallFilesFromBuild libpluginbc_asiento.dll  "\bulmacont\plugins\pluginbc_asiento\libpluginbc_asiento.dll"
!insertmacro InstallFilesFromBuild libpluginbc_balance.dll  "\bulmacont\plugins\pluginbc_balance\libpluginbc_balance.dll"
;!insertmacro InstallFilesFromBuild libpluginbc_balancearbol.dll  "\bulmacont\plugins\pluginbc_balancearbol\libpluginbc_balancearbol.dll"
!insertmacro InstallFilesFromBuild libpluginbc_bloqueofechas.dll  "\bulmacont\plugins\pluginbc_bloqueofechas\libpluginbc_bloqueofechas.dll"
!insertmacro InstallFilesFromBuild libpluginbc_ccaa2ods.dll  "\bulmacont\plugins\pluginbc_ccaa2ods\libpluginbc_ccaa2ods.dll"
!insertmacro InstallFilesFromBuild libpluginbc_corrector.dll  "\bulmacont\plugins\pluginbc_corrector\libpluginbc_corrector.dll"
!insertmacro InstallFilesFromBuild libpluginbc_cuenta.dll  "\bulmacont\plugins\pluginbc_cuenta\libpluginbc_cuenta.dll"
!insertmacro InstallFilesFromBuild libpluginbc_cuentaresumen.dll  "\bulmacont\plugins\pluginbc_cuentaresumen\libpluginbc_cuentaresumen.dll"
!insertmacro InstallFilesFromBuild libpluginbc_cuentasanuales.dll  "\bulmacont\plugins\pluginbc_cuentasanuales\libpluginbc_cuentasanuales.dll"
!insertmacro InstallFilesFromBuild libpluginbc_depuracion.dll  "\bulmacont\plugins\pluginbc_depuracion\libpluginbc_depuracion.dll"
!insertmacro InstallFilesFromBuild libpluginbc_diario.dll  "\bulmacont\plugins\pluginbc_diario\libpluginbc_diario.dll"
!insertmacro InstallFilesFromBuild libpluginbc_duplicarasiento.dll  "\bulmacont\plugins\pluginbc_duplicarasiento\libpluginbc_duplicarasiento.dll"
!insertmacro InstallFilesFromBuild libpluginbc_example.dll  "\bulmacont\plugins\pluginbc_example\libpluginbc_example.dll"
!insertmacro InstallFilesFromBuild libpluginbc_mayor.dll  "\bulmacont\plugins\pluginbc_mayor\libpluginbc_mayor.dll"
!insertmacro InstallFilesFromBuild libpluginbc_proyectos.dll  "\bulmacont\plugins\pluginbc_proyectos\libpluginbc_proyectos.dll"
;!insertmacro InstallFilesFromBuild libpluginbc_registroiva.dll  "\bulmacont\plugins\pluginbc_registroiva\libpluginbc_registroiva.dll"


; README
SetOutPath "$INSTDIR\share\bulmages\bulmacont-plugins"
!insertmacro InstallFilesFromBuild README.es.pluginbc_cuenta  "\bulmacont\plugins\pluginbc_cuenta\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_asiento  "\bulmacont\plugins\pluginbc_asiento\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_diario  "\bulmacont\plugins\pluginbc_diario\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_mayor  "\bulmacont\plugins\pluginbc_mayor\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_balance  "\bulmacont\plugins\pluginbc_balance\README.es"


!insertmacro InstallFilesFromBuild README.es.pluginbc_amortizacion  "\bulmacont\plugins\pluginbc_amortizacion\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_asiento  "\bulmacont\plugins\pluginbc_asiento\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_balance  "\bulmacont\plugins\pluginbc_balance\README.es"
;!insertmacro InstallFilesFromBuild README.es.pluginbc_balancearbol  "\bulmacont\plugins\pluginbc_balancearbol\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_bloqueofechas  "\bulmacont\plugins\pluginbc_bloqueofechas\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_ccaa2ods  "\bulmacont\plugins\pluginbc_ccaa2ods\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_corrector  "\bulmacont\plugins\pluginbc_corrector\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_cuenta  "\bulmacont\plugins\pluginbc_cuenta\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_cuentaresumen  "\bulmacont\plugins\pluginbc_cuentaresumen\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_cuentasanuales  "\bulmacont\plugins\pluginbc_cuentasanuales\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_depuracion  "\bulmacont\plugins\pluginbc_depuracion\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_diario  "\bulmacont\plugins\pluginbc_diario\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_duplicarasiento  "\bulmacont\plugins\pluginbc_duplicarasiento\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_example  "\bulmacont\plugins\pluginbc_example\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_mayor  "\bulmacont\plugins\pluginbc_mayor\README.es"
!insertmacro InstallFilesFromBuild README.es.pluginbc_proyectos  "\bulmacont\plugins\pluginbc_proyectos\README.es"
;!insertmacro InstallFilesFromBuild README.es.pluginbc_registroiva  "\bulmacont\plugins\pluginbc_registroiva\README.es"


;!include "NSIS_includes_comunes\pluginbf\include_install_pluginbf_feeds.nsh"

; No los incluyo porque ya lo hace la parte de bulmafact
;SetOutPath "$INSTDIR\program\pluginsbl"

;!include "NSIS_includes_comunes\pluginbl\include_install_pluginbl_report.nsh"
;!include "NSIS_includes_comunes\pluginbl\include_install_pluginbl_report2ods.nsh"
;!include "NSIS_includes_comunes\pluginbl\include_install_pluginbl_template.nsh"
;!include "NSIS_includes_comunes\pluginbl\include_install_pluginbl_template2ods.nsh"
;!include "NSIS_includes_comunes\pluginbl\include_install_pluginbl_clipboard.nsh"
;!include "NSIS_includes_comunes\pluginbl\include_install_pluginbl_register.nsh"
;!include "NSIS_includes_comunes\pluginbl\include_install_pluginbl_autoform.nsh"

