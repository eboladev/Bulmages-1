<?php
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
			$SQLQuery = "SELECT * FROM empresa WHERE idempresa='$idempresa'";
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
?>