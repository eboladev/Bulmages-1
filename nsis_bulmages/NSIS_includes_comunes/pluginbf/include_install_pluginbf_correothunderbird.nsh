SetOutPath "$INSTDIR\program\pluginsbf"
!insertmacro InstallFilesFromBuild libpluginbf_correothunderbird.dll "\bulmafact\plugins\pluginbf_correothunderbird\libpluginbf_correothunderbird.dll"

CreateDirectory "$INSTDIR\icons"
SetOutPath "$INSTDIR\icons"
File icons\*