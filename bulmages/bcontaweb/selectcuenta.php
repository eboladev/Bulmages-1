<?php include ("inc/funciones.inc") ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<!-- Este documento esta hecho para abrirse como un popup, presentar el listado de cuentas
y que se pueda hacer click sobre uno de ellos y que devuelva a la ventana que ha hecho 
la llamada el idcuenta o idscuenta que se han seleccionado.
-->
<html>
<head>
<title>Selector de Cuenta</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<script language="JavaScript">
function tornar(valor) {
	opener.document.<?php echo $campo?>.value=valor;
	window.close();	
}// end tornar
</script>
<body>
<?php 
echo $campo;
?>
<table width="100%" border="1">
<?php
	$SQLQueryMayor = "SELECT * FROM cuenta ORDER BY codigo DESC";
	$resmayor = pg_exec($empresabd, $SQLQueryMayor);
	$filasm = pg_NumRows($resmayor);
	while ($filasm--) {
		echo "<tr>";
		echo "<td><A href=\"javascript:tornar('".pg_result($resmayor,$filasm,"codigo")."')\">".pg_result($resmayor, $filasm, "codigo")."</A></TD>";
		echo "<td>".pg_result($resmayor, $filasm, "descripcion")."</TD>";
		echo "<td>".pg_result($resmayor, $filasm, "cifent_cuenta")."</TD>";
		echo "</tr>";
	}// end while
?>
</table>
</body>
</html>
