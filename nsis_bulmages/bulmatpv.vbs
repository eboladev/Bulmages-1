Set WshShell = CreateObject("WScript.Shell")

If Wscript.Arguments.Count = 0 Then
	WshShell.Run "bulmatpv.bat", 0, true
Else
	WshShell.Run "bulmatpv.bat "&Wscript.Arguments(0), 0, true
End	If

Set WshShell = Nothing