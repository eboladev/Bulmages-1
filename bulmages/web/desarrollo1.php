<html>
<head>
<title>Untitled Document</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="estilos.css" type="text/css">
</head>

<body bgcolor="#FFFFFF" text="#000000" leftmargin="0" topmargin="0"><CENTER>
<?php
	include ("cabecera.inc");
?>
	<table border="0" cellspacing="0" cellpadding="0" height="88%" bordercolor="#333333" width="750">
  <tr>
      <td width="164" bgcolor="#FFFFFF" valign="top" bordercolor="#003366" height="544"> 
<?php
	include("menu.inc");
?>   
<!-- -------------------------------- -->
<!-- -------------------------------- -->
      
      </td>
      <td width="652" valign="top" align="left" bgcolor="#FFFFFF" height="544"> 
         <table width="100%" border="0" cellspacing="0" cellpadding="0" bgcolor="#000033">
		
            <tr> 
               <td bgcolor="#FFFFFF" valign="top" align="center" height="445"><BR>
                     <p><font face="Verdana, Arial, Helvetica, sans-serif"><font color="#0000FF" size="+2">Desarrollo</font></font></p>
                  <table width="96%" border="0" cellspacing="0" cellpadding="0">
                     <tr>
                        <td>
                                    
                              <p align="justify"><font face="Verdana, Arial, Helvetica, sans-serif"><b><font size="-3">Tabla cuenta</font></b></font></p>
                           </td>
                     </tr>
                  </table>
                     <p><BR>
                     Es la tabla principal de una contabilidad.</p>
                     <table width="100%" border="1" cellspacing="0" cellpadding="0">
                        <tr>
                           <td width="33%">idcuenta</td>
                           <td width="21%">autonumerico (K)</td>
                           <td width="46%">El identificador de la cuenta</td>
                        </tr>
                        <tr>
                           <td width="33%">codigo</td>
                           <td width="21%">char [12]</td>
                           <td width="46%">El c&oacute;digo de la cuenta.</td>
                        </tr>
                        <tr>
                           <td width="33%">descripcion</td>
                           <td width="21%">varchar [50]</td>
                           <td width="46%">Descripcion de la cuenta</td>
                        </tr>
                        <tr>
                           <td width="33%">imputacion</td>
                           <td width="21%">boolean</td>
                           <td width="46%">Determina si es nodo hijo o no. (Determina si la cuenta puede tener apuntes, borradores o no)</td>
                        </tr>
                        <tr>
                           <td width="33%">padre</td>
                           <td width="21%">numerico (FK)</td>
                           <td width="46%">Sirve para establecer la jerarqu&iacute;a en arbol de las cuentas. </td>
                        </tr>
                        <tr>
                           <td width="33%">bloqueada</td>
                           <td width="21%">boolean</td>
                           <td width="46%">determina si la cuenta est&aacute; bloqueada y se pueden insertar apuntes en ella o no (s&oacute;lo para las cuentas de &uacute;ltimo nivel)</td>
                        </tr>
                        <tr>
                           <td width="33%">idrupo</td>
                           <td width="21%">numerico (FK)</td>
                           <td width="46%">El grupo al que puede pertenecer la cuenta. </td>
                        </tr>
                        <tr>
                           <td width="33%">msg</td>
                           <td width="21%">varchar [100]</td>
                           <td width="46%">Mensaje para la cuenta.</td>
                        </tr>
                     </table>
                     <p>&nbsp;</p>
                     <p><a href="desarrollo.php">&lt;&lt;Anterior</a>----<a href="desarrollo2.php">Siguiente &gt;&gt; </a>                 						
						
                  </p>
                  </td>
               <td bgcolor="#FFFFFF" valign="middle" align="center" height="445"><img src="img/shim.gif" width="1" height="1"></td>
            </tr>
         </table>
    </td>
  </tr>
</table>
</CENTER>
</body>
</html>
