<?php
#define TIPO_CTA 0
#define TIPO_FECHA 1
#define TIPO_TEXTO 2
#define TIPO_NUMERO 3
#define TIPO_PREDEFINIDO 4

$indvariablestexto=0;
$indvariablesnumero=0;
$indvariablesfecha=0;
$indvariablescta=0;
$indvariablesapunte=0;
$indvariablespredefinidas=0;

$variablespredefinidas=array();
$variablesapunte=array();
$variablescta=array();
$variablesfecha=array();
$variablesnumero=array();
$variablestexto=array();

$variablespredefinidas[$indvariablespredefinidas][1]=$fechaasiento;
$variablespredefinidas[$indvariablespredefinidas++][0]="fechaasiento";
$variablespredefinidas[$indvariablespredefinidas][1]="01/01/2004";
$variablespredefinidas[$indvariablespredefinidas++][0]="fechaactual";

function presentavarsfecha () {
	global $indvariablesfecha;
	global $variablesfecha;	
	for ($i=0 ; $i<$indvariablesfecha; $i++) {
		echo $variablesfecha[$i][2]."<input type=\"text\" name=\"".$variablesfecha[$i][0]."\" SIZE=\"20\">".$variablesfecha[$i][1]."[Calendario]<BR>";
	}// end foreach
}// end presentavarsnumero

function presentavarstexto () {
	global $indvariablestexto;
	global $variablestexto;	
	for ($i=0 ; $i<$indvariablestexto; $i++) {
		echo $variablestexto[$i][2]."<input type=\"text\" name=\"".$variablestexto[$i][0]."\" SIZE=\"35\">".$variablestexto[$i][1]."<BR>";
	}// end foreach
}// end presentavarsnumero

function presentavarsnumero () {
	global $indvariablesnumero;
	global $variablesnumero;	
	for ($i=0 ; $i<$indvariablesnumero; $i++) {
		echo $variablesnumero[$i][2]."<input type=\"text\" name=\"".$variablesnumero[$i][0]."\" SIZE=\"12\">".$variablesnumero[$i][1]."<BR>";
	}// end foreach
}// end presentavarsnumero

function presentavarscuenta () {
	global $indvariablescta;
	global $variablescta;	
	for ($i=0 ; $i<$indvariablescta; $i++) {
		echo $variablescta[$i][2]."<input type=\"text\" name=\"".$variablescta[$i][0]."\" SIZE=\"15\">\n";
		echo buscarcuenta("form1.".$variablescta[$i][0])." <BR>\n";
	}// end foreach
}// end presentavarscuenta


function presentavars() {
	presentavarscuenta();
	presentavarsfecha();
	presentavarstexto();
	presentavarsnumero();
}// end presentavars

function metevar($nomvar, $descvar, $tipo) {	
	global $indvariablestexto;
	global $indvariablesnumero;
	global $indvariablesfecha;
	global $indvariablescta;
	global $indvariablesapunte;
	global $indvariablespredefinidas;
	
	global $variablespredefinidas;
	global $variablesapunte;
	global $variablescta;
	global $variablesfecha;
	global $variablesnumero;
	global $variablestexto;
	

            // Buscamos si es una variable predefinida, y en caso de serlo obviamos una inserción de ésta.
            $j=0;
            while($j<$indvariablespredefinidas && $variablespredefinidas[$j][0] != $nomvar) {
                $j++;
            }// end while
            // Buscamos si es una variable de apunte, y en caso afirmativo obviamos una insercion de esta
            $d=0;
            while ($d<$indvariablesapunte && $variablesapunte[$d][0] != $nomvar) {
                $d++;
            }// end while

            if ($j == $indvariablespredefinidas && $d== $indvariablesapunte) {
//                printf("%d,%d, sub:%s\n",$indvariablespredefinidas, $j,$subcadena);
//                printf("nom:%s\n",$nomvar);
                switch($tipo) {
                case TIPO_CTA:
                    for($j=0;$j<$indvariablescta && $variablescta[$j][0] != $nomvar;$j++)
                        ;
                    if ($j == $indvariablescta) {
                        // Se trata de una inserción
                        $variablescta[$indvariablescta][0]=$nomvar;
                        $variablescta[$indvariablescta][2]=$descvar;
                        $indvariablescta++;
                    } else {
                        // Se trata de una modificacion
                        if ($variablescta[$j][2] == "")
                            $variablescta[$j][2] = $descvar;
                    }// end if
                    break;
                case TIPO_FECHA:
                    for($j=0;$j<$indvariablesfecha && $variablesfecha[$j][0] != $nomvar;$j++)
                        ;
                    if ($j == $indvariablesfecha) {
                        $variablesfecha[$indvariablesfecha][0]=$nomvar;
                        $variablesfecha[$indvariablesfecha][2]=$descvar;
                        $indvariablesfecha++;
                    } else {
                        if ($variablesfecha[$j][2] == "")
                            $variablesfecha[$j][2] = $descvar;
                    }// end if
                    break;
                case TIPO_NUMERO:
                    for($j=0;$j<$indvariablesnumero && $variablesnumero[$j][0] != $nomvar;$j++)
                        ;
                    if ($j == $indvariablesnumero) {
                        $variablesnumero[$indvariablesnumero][0]=$nomvar;
                        $variablesnumero[$indvariablesnumero][2]=$descvar;
                        $indvariablesnumero++;
                    } else {
                        if ($variablesnumero[$j][2] == "")
                            $variablesnumero[$j][2] = $descvar;
                    }// end if
                    break;
                case TIPO_TEXTO:
                    for($j=0;$j<$indvariablestexto && $variablestexto[$j][0] != $nomvar;$j++)
                        ;
                    if ($j == $indvariablestexto) {
                        $variablestexto[$indvariablestexto][0]=$nomvar;
                        $variablestexto[$indvariablestexto][2] = $descvar;
                        $indvariablestexto++;
                    } else {
                        if ($variablestexto[$j][2] == "")
                            $variablestexto[$j][2] = $descvar;
                    }// end if
                    break;
                }// end switch
            }// end if
}// end metevar


// Esta funcion busca variables en una cadena de texto y la coloca en los arrays de variables que forman el asiento 
// inteligente.
function recogevariables($texto, $tipo) {
    $cadena=$texto;

 // Hacemos la recolección de variables.
	while($pos=ereg('(\$[0-9A-Za-z, ]*\$)',$cadena,$variables) ) {   
		$nomvar="";
		$descvar=""; 
		$coment="";
		$var = $variables[1];
		$cadena=substr($cadena,$pos+strpos($cadena,$variables[1]));
		ereg('\$([0-9A-Za-z ]*)[,\$]',$var,$pentera);
		ereg(',([0-9A-za-z ]*)\$',$var,$coment);
		$nomvar=$pentera[1];
		$descvar=$coment[1];
		// Se omiten los $ pq puede haber problemas al hacer la suspensión.
		metevar($nomvar, $descvar, $tipo);
	}// end while
}// end recogevariables

/***************************************************************
 * Esta función recoge los valores de los cuadros de texto y   *
 * y rellena las tablas de variables con los valores recogidos.*
 ***************************************************************/
function recogevalores() {
	global $indvariablestexto;
	global $indvariablesnumero;
	global $indvariablesfecha;
	global $indvariablescta;
	global $indvariablesapunte;
	global $indvariablespredefinidas;
	
	global $variablespredefinidas;
	global $variablesapunte;
	global $variablescta;
	global $variablesfecha;
	global $variablesnumero;
	global $variablestexto;
	
	global $HTTP_POST_VARS;
	
    echo "<HR>";
    echo "<H3>RECOGEVALORES</H3>";
    $i=0;
    for ($i=0;$i<$indvariablestexto;$i++) {
        $variablestexto[$i][1]=$HTTP_POST_VARS[$variablestexto[$i][0]];
        printf("%s=%s\n",$variablestexto[$i][0],$variablestexto[$i][1]);
    }// end for
    
    for ($i=0;$i<$indvariablesnumero;$i++) {
        $variablesnumero[$i][1]=$HTTP_POST_VARS[$variablesnumero[$i][0]];
        printf("%s=%s\n",$variablesnumero[$i][0],$variablesnumero[$i][1]);
    }// end for
    
    for ($i=0;$i<$indvariablesfecha;$i++) {
        $variablesfecha[$i][1]=$HTTP_POST_VARS[$variablesfecha[$i][0]];
        printf("%s=%s\n",$variablesfecha[$i][0],$variablesfecha[$i][1]);
    }// end for
    
    for ($i=0;$i<$indvariablescta;$i++) {
        $variablescta[$i][1]=$HTTP_POST_VARS[$variablescta[$i][0]];
        printf("%s=%s\n",$variablescta[$i][0],$variablescta[$i][1]);
    }// end for
    echo "<HR>";
}// end recogevalores


// Esta funcion recoge variables de un asiento inteligente determinado.
function recogevarsasiento($idasiento) {
	global $empresabd;
	$SQLQuery = "SELECT * FROM binteligente WHERE idainteligente=$idasiento";
	$result = pg_exec($empresabd,$SQLQuery);
	$filas = pg_NumRows($result);
	while ($filas--) {
		recogevariables(pg_result($result,$filas,"codcuenta"), TIPO_CTA);
		recogevariables(pg_result($result,$filas,"conceptocontable"), TIPO_TEXTO);
		recogevariables(pg_result($result,$filas,"fecha"), TIPO_FECHA);
		recogevariables(pg_result($result,$filas,"debe"), TIPO_NUMERO);
		recogevariables(pg_result($result,$filas,"haber"), TIPO_NUMERO);
		recogevariables(pg_result($result,$filas,"comentario"), TIPO_TEXTO);
		recogevariables(pg_result($result,$filas,"contrapartida"), TIPO_CTA);
		recogevariables(pg_result($result,$filas,"descripcion"), TIPO_TEXTO);
	}// end while
}// end recogevarsasiento


// Esta función aplica a un texto todas las variables del asiento inteligente.
// DE MOMENTO NO SE USA, ES SOLO UNA PRUEBA PARA VER SI ES VERAZ O NO.
function aplicavarasiento($idasiento) {
	global $empresabd;
	
	$SQLQuery = "SELECT * FROM binteligente WHERE idainteligente=$idasiento";
	$result = pg_exec($empresabd,$SQLQuery);
	$filas = pg_NumRows($result);
	while ($filas--) {
		echo "<HR>";
		echo "<BR>codigocuenta: ".pg_result($result,$filas,"codcuenta");
		$codigocuenta = aplicavariables(pg_result($result,$filas,"codcuenta"));
		echo "<BR>conceptocontable: ".pg_result($result,$filas,"conceptocontable");
		$conceptocontable = aplicavariables(pg_result($result,$filas,"conceptocontable"));
		echo "<BR>fecha: ".pg_result($result,$filas,"fecha");
		$fecha = aplicavariables(pg_result($result,$filas,"fecha"));
		echo "<BR>debe: ".pg_result($result,$filas,"debe");
		$debe = aplicavariables(pg_result($result,$filas,"debe"));
		echo "<BR>haber: ".pg_result($result,$filas,"haber");
		$haber = aplicavariables(pg_result($result,$filas,"haber"));
		echo "<BR>comentario: ".pg_result($result,$filas,"comentario");
		$comentario = aplicavariables(pg_result($result,$filas,"comentario"));
		echo "<BR>contrapartida: ".pg_result($result,$filas,"contrapartida");
		$contrapartida = aplicavariables(pg_result($result,$filas,"contrapartida"));
		echo "<BR>descripcion: ".pg_result($result,$filas,"descripcion");
		$descripcion = aplicavariables(pg_result($result,$filas,"descripcion"));
		echo "<BR>$codigocuenta";
		echo " -- $conceptocontable--";
		echo " $fecha-- $debe-- $haber-- $comentario-- $contrapartida-- $descripcion";
	}// end while
}// end aplicavarasiento

// Esta función aplica todas las variables a una cadena, consiguiente una sustitución incremental.
function aplicavariables($texto){
	global $indvariablestexto;
	global $indvariablesnumero;
	global $indvariablesfecha;
	global $indvariablescta;
	global $indvariablesapunte;
	global $indvariablespredefinidas;
	
	global $variablespredefinidas;
	global $variablesapunte;
	global $variablescta;
	global $variablesfecha;
	global $variablesnumero;
	global $variablestexto;

// Se hace un repaso que hace que se aplique una variable.
    $cadena=$texto;    
    // Lo primero vamos a quitar todos los comentarios.
    $cadena = ereg_replace('(\$[0-9A-Za-z ]*),[0-9A-Za-z ]*\$','\\1$',$cadena);

    echo "<HR>";
    echo "<H3>APLICAVARIABLES</H3>";
    echo "texto:".$texto."<BR>";
    echo "Cadena:".$cadena."<BR>";
    // Mientras haya variables hacemos sustitucines
    while ($pos=ereg('\$[0-9A-Za-z, ]*\$',$cadena) ) {   
	$i=0;
	for ($i=0;$i<$indvariablespredefinidas;$i++) {
		echo $variablespredefinidas[$i][0]."<BR>";
		echo $variablespredefinidas[$i][1]."<BR>";
		$cadena = ereg_replace('\$'.$variablespredefinidas[$i][0].'\$', $variablespredefinidas[$i][1], $cadena);
	}// end for
	for ($i=0;$i<$indvariablesapunte;$i++) {
		echo $variablesapunte[$i][0]."<BR>";
		echo $variablesapunte[$i][1]."<BR>";
		$cadena = ereg_replace('\$'.$variablesapunte[$i][0].'\$', $variablesapunte[$i][1], $cadena);
	}// end for
	for ($i=0;$i<$indvariablestexto;$i++) {
		echo $variablestexto[$i][0]."<BR>";
		echo $variablestexto[$i][1]."<BR>";
		$cadena = ereg_replace('\$'.$variablestexto[$i][0].'\$', $variablestexto[$i][1], $cadena);
	}// end for
	for ($i=0;$i<$indvariablesnumero;$i++) {
		echo $variablesnumero[$i][0]."<BR>";
		$cadena = ereg_replace('\$'.$variablesnumero[$i][0].'\$', $variablesnumero[$i][1], $cadena);
	}// end for
	for ($i=0;$i<$indvariablesfecha;$i++) {
		echo $variablesfecha[$i][0]."<BR>";
		$cadena = ereg_replace('\$'.$variablesfecha[$i][0].'\$', $variablesfecha[$i][1], $cadena);
	}// end for
	for ($i=0;$i<$indvariablescta;$i++) {
		echo "var:".$variablescta[$i][0]."<BR>";
		echo "valor:".$variablescta[$i][1]."<BR>";
		$cadena = ereg_replace('\$'.$variablescta[$i][0].'\$', $variablescta[$i][1], $cadena);
	}// end for   
    }// end while
    echo "<BR>";
    echo "Cadena:".$cadena;
    echo "<HR>";
    return $cadena;
}// end aplicavariable


// Esta función se encarga de controlar todo el tema.
// Se supone que todas las variables ya están iniciadas.
// Esta función crea un nuevo asiento, recoge el identificador. Luego llama a creaasiento con el 
// identificador que se encargará de crear todos los borradores necesarios y de borrar el asiento.
function crear($idainteligente, $fechaasiento) {
	global $empresabd;
	$anoasiento = substr($fechaasiento,-4,4);
	$SQLQuery = "SELECT MAX(ordenasiento) AS orden FROM asiento WHERE EXTRACT(YEAR FROM fecha)='$anoasiento'";
	$result = pg_exec($empresabd, $SQLQuery);
	$ordenasiento = pg_result($result,0,"orden");
	$ordenasiento++;	// Incrementamos el orden para que la inserción sea correcta.
	$SQLQuery = "SELECT MAX(idasiento) AS numasiento FROM asiento";
	$result = pg_exec($empresabd,$SQLQuery);
	$numasiento = pg_result($result,0,"numasiento");
	$numasiento++;		// También incrementamos el numasiento para que la inserción sea correcta.
	$SQLQuery = "INSERT INTO asiento (idasiento, ordenasiento, fecha, descripcion) VALUES ($numasiento, $ordenasiento, '$fechaasiento', '$fechaasiento')";
	$result = pg_exec($empresabd, $SQLQuery);
        creaasiento($idainteligente, $numasiento);
}// end boton_crear




// Esta función carga la variable de uso temporal $cifcuenta$
// con el valor del nif de la cuenta introducida.
// NOTA: Notese que el sistema normalmente carga el NIF de la contrapartida del asiento inteligente
// Y que este campo normalmente va en la parte de CIFCUENTA
function cifcuenta($idcuenta) {
    global $empresabd;
    global $variablesapunte;
    global $indvariablesapunte;
    
    $variablesapunte[$indvariablesapunte][0] ="$cifcuenta$";
    $SQLQeury = "SELECT * FROM cuenta WHERE idcuenta=$idcuenta";
    $result = pg_exec($empresabd, $SQLQuery);
    if (pg_numrows($result)) {
        $variablesapunte[$indvariablesapunte][1] = pg_result($result,0,"cifent_cuenta");
    } else {
        $variablesapunte[$indvariablesapunte][1] = "";
    }// end if
    $indvariablesapunte++;
}// end cifcuenta


// Esta función crea todos los registros implicados con los asientos inteligentes
// Recibe como parámetro el numero de asiento inteligente que se aplica 
// Y el numero de asiento sobre el que se aplica.
function creaasiento($numainteligente, $numasiento) {
	global $empresabd;
	global $indvariablesapunte;

    $SQLQuery = "SELECT * FROM binteligente WHERE idainteligente=$numainteligente";
    $result = pg_exec($empresabd,$SQLQuery);
    $filas = pg_numrows($result);
    while ($filas--) {  
    	echo "<BR>codigo cuenta:".pg_result($result,$filas,"codcuenta")."<BR>";
    	$codcuenta = aplicavariables(pg_result($result,$filas,"codcuenta"));
	$SQLQuery = "SELECT * FROM cuenta where codigo='$codcuenta'";
	$result1 = pg_exec($empresabd, $SQLQuery);
	$idcuenta = pg_result($result1,0,"idcuenta");
	echo "idcuenta:$idcuenta<BR>";
	
	$contrapartida = aplicavariables(pg_result($result,$filas,"contrapartida"));
 	$SQLQuery = "SELECT * FROM cuenta where codigo='$contrapartida'";
	$result1 = pg_exec($empresabd, $SQLQuery);
	if (pg_numrows($result1)) {
		$idcontrapartida = pg_result($result1,0,"idcuenta");
	} else {
		$idcontrapartida = "NULL";
	}// end if
	$debe = aplicavariables(pg_result($result, $filas, "debe"));
	$haber = aplicavariables(pg_result($result, $filas, "haber"));
	$fecha = aplicavariables(pg_result($result, $filas, "fecha"));
	echo pg_result($result, $filas, "fecha")."-----";
	echo "$fecha<BR>";
	$conceptocontable = aplicavariables(pg_result($result, $filas, "conceptocontable"));
	$descripcion = aplicavariables(pg_result($result, $filas, "descripcion"));
	
	$SQLQuery = "INSERT INTO borrador (idasiento, idcuenta, contrapartida, debe, haber, fecha, conceptocontable, descripcion) VALUES ($numasiento, $idcuenta, $idcontrapartida, $debe, $haber, '$fecha', '$conceptocontable', '$descripcion')";
	pg_exec($empresabd, $SQLQuery);
	echo $SQLQuery;
    
	
	// Miramos si existe una entrada en IVA para este apunte y la creamos
	$SQLQuery = "SELECT * from ivainteligente WHERE idbinteligente=".pg_result($result,$filas,"idbinteligente");
	$curiva = pg_exec($empresabd, $SQLQuery);
	if( pg_numrows($curiva) ) {
		$SQLQuery = "SELECT max(idborrador) AS id FROM borrador";
		$result3 = pg_exec($empresabd, $SQLQuery);
		$idborrador = pg_result($result3, 0, "id");
		// Buscamos la contrapartida
	    	$contrapartida = aplicavariables(pg_result($curiva,0,"contrapartida"));
		$SQLQuery = "SELECT * FROM cuenta where codigo='$contrapartida'";
		$result4 = pg_exec($empresabd, $SQLQuery);
		if (pg_numrows($result4)) {
			$idcontrapartida = pg_result($result4, 0, "idcuenta");
		} else {
			$idcontrapartida = "NULL";
		}// end if
		
		// Esta función no está implementada, pero me imagino que tiene que ver con las variables de apunte.
		// Lo que hace es cargar la variable de apunte $cifcuenta$ que proporciona el CIF de la cuenta usada en la contrapartida del apunte.
		cifcuenta($idcontrapartida);
		
		$iva = aplicavariables(pg_result($curiva, 0, "iva"));
		if ($iva == "") $iva="NULL";
		$factura = aplicavariables(pg_result($curiva, 0, "factura"));
		if ($factura == "") $factura = "NULL";
		$baseimp = aplicavariables(pg_result($curiva, 0, "baseimp"));
		if ($baseimp == "") $baseimp = "NULL";
		$numorden = aplicavariables(pg_result($curiva, 0, "numorden"));
		if ($numorden == "") $numorden = "NULL";
		$cif = aplicavariables(pg_result($curiva, 0, "cif"));
		if ($cif == "") $cif = "NULL";
		
		// Hacemos el query de inserción del registro de IVA.
		$SQLQuery = "INSERT INTO registroiva (idborrador, contrapartida, iva, factura, baseimp, numorden, cif) VALUES ($idborrador, $idcontrapartida, $iva, '$factura', $baseimp, '$numorden', '$cif')";
		pg_exec($empresabd, $SQLQuery);
		echo $SQLQuery;
	}// end if
	$indvariablesapunte=0;		// Reseteamos las variables de apunte pq con cada apunte deben recalcularse.
   }// end while
}// end creaasiento

?>