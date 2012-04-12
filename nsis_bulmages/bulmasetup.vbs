Set WshShell = CreateObject("WScript.Shell")

If Wscript.Arguments.Count = 0 Then
	WshShell.Run "bulmasetup.bat", 0, true
Else
	WshShell.Run "bulmasetup.bat "&Wscript.Arguments(0), 0, true
End	If

Set WshShell = Nothing
