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
$dbconta= $valssincr->connectbulmages();
$bdgalopin = $valssincr->connectgalopin();


echo "<H1>Galopin => BulmaGés</H1>";
/* Realizar una consulta MYSQL */
$consulta  = "SELECT * FROM clientes, facturas WHERE clientes.codcliente=facturas.codcliente";
$resultado = mysql_query($consulta) or die("La consulta fall&oacute;: " . mysql_error());

/* Impresion de resultados en HTML */
echo "<table border='1'>\n";
while ($linea = mysql_fetch_array($resultado, MYSQL_ASSOC)) {
   $nif = limpia($linea["nif"]);
   $codfactura = limpia($linea["codfactura"]);
   echo "\t<tr><td><H2>$nif & $codfactura</H2>\n";  
   $consulta1= "SELECT sum(importe) AS bimp FROM factulinea WHERE codfactura=$codfactura";
   $resultado1 = mysql_query($consulta1) or die("La consulta fallo");
   $res = mysql_fetch_array($resultado1, MYSQL_ASSOC);
   $importe = $res["bimp"] * $linea["iva"] /100 + $res["bimp"];
   
   $SQLQuery = "SELECT * FROM cuenta, apunte WHERE cifent_cuenta IS NOT NULL AND cifent_cuenta <> '' AND apunte.idcuenta=cuenta.idcuenta";
   $result = pg_exec($dbconta,$SQLQuery);
   $filas = pg_numrows($result);
   $encontrado = 0;
   while ($filas--) {
		$nifent = limpia(pg_result($result,$filas,"cifent_cuenta"));
		echo "$nifent<BR>";
		// La primera comprobacion es que las cuentas son del mismo NIF.
		if ($nif == $nifent) {
			echo $nifent;
			echo "Posible candidado por coincidencia de cliente: ";
			echo pg_result($result,$filas,"idasiento");
			echo "<BR>";
			$debe = pg_result($result,$filas,"debe");
			$haber = pg_result($result,$filas,"haber");
			$total = $debe + $haber;
			// La siguiente comprobacion es que los importes son iguales (o casi)
			if (abs((float)$total-(float)$importe) < 1) {
				echo (float)$total -(float)$importe;
				echo "importe factura: $importe, total asiento: $total<BR>";
				echo "Se ha encontrado un posible candidado para esta factura [<A HREF=''>Adjuntar</A>] <BR>";
				// Puede haber muchas más comprobaciones:
				// Entrada en el registro de IVA
				// Entrada en tablas auxiliares
				// Fecha
				// etc etc etc, nosotros nos pararemos aqui de momento (esto es un ejemplo).
				$encontrado = 1;
			}// end if
		}// end if
   }// end while 
   if ($encontrado == 0) {
     
      echo "No se ha encontrado la factura <$codfactura> en BulmaGés con importe $importe, Seguramente haya que crearla.<BR>";
	  echo "[<A href=''>Crear</a>]";
	  //presentaarray($linea);
	  //nuevacuenta($dbconta, $linea );	

   }// end if
echo "\t</td></tr>\n";   
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
