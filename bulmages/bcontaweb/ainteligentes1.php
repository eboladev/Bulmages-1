<?php include ("inc/funciones.inc") ?>
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


// Esta funcion busca variables en una cadena de texto y la coloca en los arrays de variables que forman el asiento 
// inteligente.
function recogevariables($texto, $tipo) {
    $cadena=$texto;

    // Recogemos variables en el codigo de cuenta
    $posinicial = $cadena.find("$",0);
    if ($posinicial != -1) {
        $posfinal = $cadena.find("$",$posinicial+1);
        if ($posfinal != -1) {
            $subcadena = $cadena.mid($posinicial,$posfinal+1);
            $posaux = $subcadena.find(",",0);
            if ($posaux != -1) {
                $posaux1 = $subcadena.find("$",$posaux+1);
                $descvar = $subcadena.mid($posaux+1,$posaux1-$posaux-1);
                printf ("desc:%s<-->size %d\n",$descvar, $posaux1);
                $nomvar = $subcadena.mid(0,$posaux+1);
                // Si hay un comentario lo debemos borrar tras considerarlo para que no estorbe.
                $nomvar.replace ($nomvar.length()-1,$nomvar.length(),"$");
            } else {
                $nomvar = $subcadena;
                $descvar = "";
            }// end if
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
                printf("%d,%d, sub:%s\n",$indvariablespredefinidas, $j,$subcadena);
                printf("nom:%s\n",$nomvar);
                switch($tipo) {
                case TIPO_CTA:
                    for(j=0;j<indvariablescta && variablescta[j][0] != nomvar;j++)
                        ;
                    if (j == indvariablescta) {
                        // Se trata de una inserción
                        variablescta[indvariablescta][0]=nomvar;
                        variablescta[indvariablescta][2]=descvar;
                        indvariablescta++;
                    } else {
                        // Se trata de una modificacion
                        if (variablescta[j][2] == "")
                            variablescta[j][2] = descvar;
                    }// end if
                    break;
                case TIPO_FECHA:
                    for(j=0;j<indvariablesfecha && variablesfecha[j][0] != subcadena;j++)
                        ;
                    if (j == indvariablesfecha) {
                        variablesfecha[indvariablesfecha][0]=nomvar;
                        variablesfecha[indvariablesfecha][2]=descvar;
                        indvariablesfecha++;
                    } else {
                        if (variablesfecha[j][2] == "")
                            variablesfecha[j][2] = descvar;
                    }// end if
                    break;
                case TIPO_NUMERO:
                    for(j=0;j<indvariablesnumero && variablesnumero[j][0] != subcadena;j++)
                        ;
                    if (j == indvariablesnumero) {
                        variablesnumero[indvariablesnumero][0]=nomvar;
                        variablesnumero[indvariablesnumero][2]=descvar;
                        indvariablesnumero++;
                    } else {
                        if (variablesnumero[j][2] == "")
                            variablesnumero[j][2] = descvar;
                    }// end if
                    break;
                case TIPO_TEXTO:
                    for(j=0;j<indvariablestexto && variablestexto[j][0] != subcadena;j++)
                        ;
                    if (j == indvariablestexto) {
                        variablestexto[indvariablestexto][0]=nomvar;
                        variablestexto[indvariablestexto][2] = descvar;
                        indvariablestexto++;
                    } else {
                        if (variablestexto[j][2] == "")
                            variablestexto[j][2] = descvar;
                    }// end if
                    break;
                }// end switch
            }// end if
        }// end if
    }// end if
}// end recogevariables

/***************************************************************
 * Esta función recoge los valores de los cuadros de texto y   *
 * y rellena las tablas de variables con los valores recogidos.*
 ***************************************************************/
void recogevalores() {
    int i=0;
    for (i=0;i<indvariablestexto;i++) {
        variablestexto[i][1]=vartexto[i]->text();
        fprintf(stderr,"%s=%s\n",variablestexto[i][0].ascii(),variablestexto[i][1].ascii());
    }// end for
    for (i=0;i<indvariablesnumero;i++) {
        variablesnumero[i][1]=varnumero[i]->text();
        fprintf(stderr,"%s=%s\n",variablesnumero[i][0].ascii(),variablesnumero[i][1].ascii());
    }// end for
    for (i=0;i<indvariablesfecha;i++) {
        variablesfecha[i][1]=varfecha[i]->text();
        fprintf(stderr,"%s=%s\n",variablesfecha[i][0].ascii(),variablesfecha[i][1].ascii());
    }// end for
    for (i=0;i<indvariablescta;i++) {
        variablescta[i][1]=varcta[i]->text();
        fprintf(stderr,"%s=%s\n",variablescta[i][0].ascii(),variablescta[i][1].ascii());
    }// end for
}// end recogevalores


?>



<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<HTML>
  <head>
    <title>selectempresa.php</title>
  </head>
<BODY>
<table><TR><TD valign="top">
toma MENU
<?php include("inc/menu.inc")?>
</TD><TD>
<!-- BEGIN Cuerpo de la página -->
<H1>ASIENTOS INTELIGENTES</H1>

<?php echo idainteligente?>


<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
