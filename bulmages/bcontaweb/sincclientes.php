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
$dbconta=pg_connect("dbname=bulmages user=tborras");
// Establecemos el tipo de fecha para la base de datos.
$SQLQuery1 = "SET DateStyle TO 'SQL'";
$result1 = pg_exec($dbconta,$SQLQuery1);  
$SQLQuery1 = "SET DateStyle TO 'European'";
$result1 = pg_exec($dbconta,$SQLQuery1);  


$nomdbgalopin = "galopin";
$usergalopin = "root";
$passgalopin = "";
$server = "localhost";
$port = "";


/* Conexion, seleccion de base de datos GALOPIN */
$enlace = mysql_connect($server, $usergalopin, $passgalopin)
   or die("No pudo conectarse : " . mysql_error());
echo "Conexi&oacute;n exitosa";
mysql_select_db($nomdbgalopin) or die("No pudo seleccionarse la BD.");


/* Realizar una consulta SQL */
$consulta  = "SELECT * FROM clientes";
$resultado = mysql_query($consulta) or die("La consulta fall&oacute;: " . mysql_error());


/* Impresion de resultados en HTML */
echo "<table border='1'>\n";
while ($linea = mysql_fetch_array($resultado, MYSQL_ASSOC)) {
   echo "\t<tr>\n";
   echo "\t\t<td>".$linea["nif"]."<BR>\n"; 
   $nif = $linea["nif"];
   $nif= str_replace(".","",$nif);
   $nif= str_replace(" ","",$nif);
   $nif= str_replace("-","",$nif);
   $nif= strtoupper($nif);
   
/*   
   $SQLQuery = "SELECT * FROM cuenta WHERE cifent_cuenta = '$nif'";
   echo "<TD>$SQLQuery</TD>";
   $result = pg_exec($dbconta,$SQLQuery);
   $filas = pg_numrows($result);
   if ($filas == 0) {
   	echo "<TD>No existe la cuenta ni la entidad, por tanto hay que insertar la cuenta</TD>";
   } else {
   	echo "<TD>La cuenta es".pg_result($result,$filas,"cifent_cuenta")."</TD>";
   }// end if
*/
   $SQLQuery = "SELECT * FROM cuenta WHERE cifent_cuenta IS NOT NULL";
   $result = pg_exec($dbconta,$SQLQuery);
   $filas = pg_numrows($result);
   $encontrado = 0;
   while ($filas--) {
   	$nifent = pg_result($result,$filas,"cifent_cuenta");
	$nifent = str_replace(".","",$nifent);
	$nifent = str_replace(" ","",$nifent);
	$nifent = str_replace("-","",$nifent);
	$nifent = strtoupper($nifent);
	if ($nif == $nifent) {
		echo "Hay dos nifs iguales<BR>";
		$encontrado = 1;
	}// end if
   }// end while 
   if ($encontrado == 0) {
        echo "No existe el nif en la Contabilidad, por tanto hay que insertarlo<BR>";
	foreach ($linea as $nombre => $valor_col) {
	echo "\t\t$nombre = $valor_col<BR>\n";
	}// end foreach      
	nuevacuenta($dbconta, $linea );	
   }// end if

   echo "\t</td></tr>\n";
}// end while
echo "</table>\n";

/* Liberar conjunto de resultados */
mysql_free_result($resultado);

/* Cerrar la conexion */
mysql_close($enlace);

?>

<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
