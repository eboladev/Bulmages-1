<?php
// Este fichero se lee al principio de cada fichero de la aplicacion
// por tanto contiene toda la información necesaria y hace
// todas las inicializaciones comunes a la aplicacion
// también 
$DEBUG=1;

$metabd=pg_connect("dbname=metabd user=tborras");
// Establecemos el tipo de fecha para la base de datos.
$SQLQuery1 = "SET DateStyle TO 'SQL'";
$result1 = pg_exec($metabd,$SQLQuery1);  
$SQLQuery1 = "SET DateStyle TO 'European'";
$result1 = pg_exec($metabd,$SQLQuery1);  


$autentificado = 0;
if ($logincookie != "") {
	$SQLQuery = "SELECT * FROM usuario WHERE login='$logincookie'";
	$usuario = pg_exec($metabd, $SQLQuery);
	$filas = pg_NumRows($usuario);
	if ($filas--) {
		$autentificado = 1;
		$idusuario = pg_result($usuario,$filas,"idusuario");
		$password =pg_result($usuario,$filas,"password");
		$nombre = pg_result($usuario, $filas, "nombre");
		$apellido1 = pg_result($usuario, $filas, "apellido1");
	}// end if
} else {
	if ($login != "") {
		$SQLQuery = "SELECT * FROM usuario WHERE login='$login'";
		$usuario = pg_exec($metabd, $SQLQuery);
		$filas = pg_NumRows($usuario);
		if ($filas--) {
			$password1=pg_result($usuario,$filas,"password");
			if ($password == $password1) {
				$autentificado = 1;
				$idusuario = pg_result($usuario,$filas,"idusuario");
				$nombre = pg_result($usuario, $filas, "nombre");
				$apellido1 = pg_result($usuario, $filas, "apellido1");
				setcookie("logincookie",$login);
			}// end if
		}// end if
	}// end if
}// end if

if ($autentificado == 0) {
	header("Location: index.php?error=1");
} else {
		$idempresaform= $HTTP_POST_VARS["idempresa"];
		if ($idempresaform != "") {
			$idempresa = $idempresaform;
		}// end if
		setcookie("idempresa",$idempresa);
		if ($idempresa != "") {
			$SQLQuery = "SELECT * FROM empresa WHERE idempresa=$idempresa";
			$empresa = pg_exec($metabd, $SQLQuery);
			$filas = pg_NumRows($empresa);
			if ($filas--) {
				$nomempresa = pg_result($empresa,$filas,"nombredb");
				$empresabd=pg_connect("dbname=$nomempresa user=tborras");
				// Establecemos el tipo de fecha para la base de datos.
				$SQLQuery1 = "SET DateStyle TO 'SQL'";
				$result1 = pg_exec($empresabd,$SQLQuery1);  
				$SQLQuery1 = "SET DateStyle TO 'European'";
				$result1 = pg_exec($empresabd,$SQLQuery1);  
				setcookie("idempresa",$idempresa);
			} else {
				if ($selempresa != 0) 
					header("Location:selectempresa.php");
			}// end if
		}// end if
}// end if

if ($DEBUG==1) {
	if ($idempresa!= "" && $selempresa !="0") {
		echo pg_result($empresa,0,"nombre");
	}// end if
	echo "<BR>";
	echo pg_result($usuario,0,"nombre");
}// end if



// Cargamos las configuraciones
include ("configuracion.inc");
$confpr = new configuracion();
$confpr->leeconffile("/etc/bulmages.conf");

?>
<?php 
// Esta función sirve para insertar una cuenta en la base de datos de bulmages
// que se pasa como parametro
function nuevacuenta($database, $array) {
   echo "Nueva cuenta:";
	$SQLQuery = "SELECT * FROM cuenta WHERE codigo LIKE '430%'";
	$result = pg_exec($database, $SQLQuery);
	$filas = pg_numRows($result);
	$valormax = 0;
	while ($filas--) {
	   $valor = pg_result($result, $filas, "codigo");
	   if ($valor > $valormax) {
	   	$valormax = $valor;
	   }// end if
	}// end while
	$valormax++;
	echo "La nueva cuenta puede ser:$valormax <BR>";
/*	
 idcuenta |   |                                            
                | imputacion | padre | bloqueada | idgrupo | msg |   debe   |  haber   | nodebe | nohaber
 | regularizacion | activo |      nombreent_cuenta      | cifent_cuenta |      | 
|                         coment_cuenta                          |   bancoent_cuenta   |    webent_cuenta    | tipocuenta
*/	
	$QueryInsercion = " INSERT INTO cuenta (codigo, descripcion, cifent_cuenta, nombreent_cuenta, dirent_cuenta , telent_cuenta, emailent_cuenta, coment_cuenta) VALUES ('$valormax', '".$array["nombre"]."' , '".$array["nif"]."' , '".$array["nombre"]."' , '".$array["direccion"]."' , '".$array["telefono"]."' , '".$array["email"]."' , '".$array["observaciones"]."')";
	echo "$QueryInsercion<BR>";
}// end nuevacuenta


// Esta funcion limpia una cadena de caracteres extraños que lo puedan confundir.
function limpia($nif) {
   $nif= str_replace(".","",$nif);
   $nif= str_replace(" ","",$nif);
   $nif= str_replace("-","",$nif);
   $nif= strtoupper($nif);
   return $nif;
}// end limpia


function presentaarray($array) {
	echo "<hr><pre>";
	foreach ($array as $nombre => $valor_col) {
		echo "\t\t$nombre = $valor_col<BR>\n";
	}// end foreach  
	echo "</pre><HR>";    
}

function buscarcuenta($cad) {
echo "<a href=\"#\" onclick=\"window.open('selectcuenta.php?campo=$cad','nueva',0)\"><IMG src=\"img/find.png\" width=\"22\" height=\"22\" border=\"0\"></a>";
}

function ncuenta($cad) {
echo "<a href=\"#\" onclick=\"window.open('nuevacuenta.php?campo=$cad','nueva',0)\"><IMG src=\"img/find.png\" width=\"22\" height=\"22\" border=\"0\"></a>";
}
?>