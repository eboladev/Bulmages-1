<?php
// La clase configuración contiene los parametros que se leen
// De un archivo de configuración normal de bulmages
// Normalmente se leerá /etc/bulmages.conf y se almacenará
// En un array.

class configuracion {
   var $confpr; // el array de configuraciones.
   
   function configuracion() {
   }// end configuracion
   
   function leeconffile($file) {
      $fp = file($file);
      foreach($fp as $row) {
         if (! ereg("^#.",$row)) {
         	if (ereg("([^ ]+) ([^ ]+)",$row, $line)) {
			$this->confpr[$line[1]]=$line[2];
		}// end if
	 }// end if
      }// end foreach 
   }// end leeconffile
   
   function config($valor) {
      return $this->confpr[$valor];
   }// end config
}// end class configuracion


// La clase paramsinc recoge todos los valores de
// hTTP_POST_VARS que estan relacionados con la sincronización.
// En la página correspondiente se cogen todos los parametros y se hacen cosas guapas.

class paramsincr {
   var $valores; // Este array almacena los valores
   function paramsincr() {
   	$this->valores = $HTTP_POST_VARS;
	$this->valores += $HTTP_GET_VARS;
   }// end paramsincr
   function config($valor) {
   	return $this->valores[$valor];
   }// end config
}// end class paramsincr



?>