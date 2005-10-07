<?php include ("inc/funciones.inc") ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<HTML>
  <head>
    <title>Sincronización de clientes.</title>
  </head>
<BODY>
<table><TR><TD valign="top">
<?php include("inc/menu.inc") ?>
</TD><TD valign="top">
<!-- BEGIN Cuerpo de la página -->

<?php



// Leemos los valores de sincronización.
$valssincr = new paramsincr();

/* Conexión, de la base de datos correspondiente de BulmaGés */
//$dbconta= $valssincr->connectbulmages();
$dbconta = $empresabd;
$bdgalopin = $valssincr->connectgalopin();


echo "<H1>Galopin => BulmaGés</H1>";
/* Realizar una consulta MYSQL */
$consulta  = "SELECT * FROM clientes";
$resultado = mysql_query($consulta) or die("La consulta fall&oacute;: " . mysql_error());

/* Impresion de resultados en HTML */
echo "<table border='1'>\n";
while ($linea = mysql_fetch_array($resultado, MYSQL_ASSOC)) {
   $nif = limpia($linea["nif"]);
   $SQLQuery = "SELECT * FROM cuenta WHERE cifent_cuenta IS NOT NULL AND cifent_cuenta <> ''";
   $result = pg_exec($dbconta,$SQLQuery);
   $filas = pg_numrows($result);
   $encontrado = 0;
   while ($filas--) {
		$nifent = limpia(pg_result($result,$filas,"cifent_cuenta"));
		if ($nif == $nifent) {
			$encontrado = 1;
		}// end if
   }// end while 
   if ($encontrado == 0) {
      echo "\t<tr><td><H2>$nif</H2>\n";       
      echo "No existe el nif en BulmaGés, por tanto hay que insertarlo<BR>";
	  //presentaarray($linea);
//	  nuevacuenta($dbconta, $linea );
	  ncuenta("");	
	  echo "\t</td></tr>\n";
   }// end if
}// end while
echo "</table>\n";


echo "<H1> BulmaGés => Galopin </H1>";
echo "<table border=\"1\">";
$SQLQuery = "SELECT * FROM cuenta WHERE cifent_cuenta IS NOT NULL AND cifent_cuenta <> ''";
$result = pg_exec($dbconta,$SQLQuery);
$filas = pg_numrows($result);
while ($filas--) {
	$nifent = limpia(pg_result($result,$filas,"cifent_cuenta"));
	/* Realizar una consulta MYSQL */
	$consulta  = "SELECT * FROM clientes";
	$resultado = mysql_query($consulta) or die("La consulta fall&oacute;: " . mysql_error());
	$encontrado = 0;
	while ($linea = mysql_fetch_array($resultado, MYSQL_ASSOC)) {
	   $nif = limpia($linea["nif"]);
	   if ($nif == $nifent) {
			$encontrado = 1;
	   }// end if
	}// end while
    if ($encontrado == 0) {
		echo "<tr><td>";
		echo "<H2>$nifent</H2>";
        echo "No existe el nif en Galopin, por tanto hay que insertarlo<BR>";
		echo "</td></tr>";
		// presentaarray($linea);
		// nuevacuenta($dbconta, $linea );	
    }// end if
}// end while
echo "</table>";

/* Liberar conjunto de resultados */
mysql_free_result($resultado);

/* Cerrar la conexion */
mysql_close($bdgalopin);

?>

<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
