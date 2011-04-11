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
// Aparte se encarga de inicializar una serie de variables nuevas
// obtenidas a partir de los parametros de inicio que aportan 
// vitaminas minerales y oligoelementos lacteo-fructosos.

class paramsincr {
   var $valores=array(); // Este array almacena los valores
   
   function paramsincr() {
   	$this->valores = $_POST;	
   }// end paramsincr
  
	function printparams() {
   		presentaarray($_POST);
	}// end printparams
  
   function config($valor) {
   	return $this->valores[$valor];
   }// end config
   
   // -----------------------------------------------------------------------------
   // Esta función nos conecta con la base de datos de galopin, si los parametros  |
   // Son correctos y existen. En caso contrario devuelve un error.
   // -----------------------------------------------------------------------------
   function connectgalopin() {
		// Conexión de la base de datos de galopin.
		// Aqui vamos a inicializar la base de datos de galopin
		// Y crearemos el valor "BD_GALOPIN" en el array para obtener
		// La conexión. (Puede que luego se aporten funciones nuevas).
		$nomdbgalopin = $this->valores["nomdbgalopin"];
		$usergalopin = $this->valores["usergalopin"];
		$passgalopin = $this->valores["passgalopin"];
		$servergalopin = $this->valores["servergalopin"];
		$portgalopin = $this->valores["portgalopin"];
		/* Conexion, seleccion de base de datos GALOPIN */
		$enlace = mysql_connect($servergalopin, $usergalopin, $passgalopin)
		or die("No pudo conectarse : " . mysql_error());
		mysql_select_db($nomdbgalopin) or die("No pudo seleccionarse la BD.");	
		
		// Devolvemos el puntero que es menester para la conexion con la base de datos.
	    return $enlace;
   }// endconnectgalopin
   
   function connectbulmages() {
   		// Conectamos con bulmages, a partir de
		// la base de datos que se ha seleccionado y esta en la variable global
		// idempresa.
		global $metabd;
		
		$idempresasinc=$this->valores["idempresasinc"];
		$query = "SELECT * FROM empresa WHERE idempresa=$idempresasinc";
		$result = pg_exec($metabd,$query);
		if (pg_numrows($result)) {
			$nomemp=pg_result($result,0,"nombredb");		
			$empresa1=pg_connect("dbname=$nomemp user=tborras");
			// Establecemos el tipo de fecha para la base de datos.
			$SQLQuery1 = "SET DateStyle TO 'SQL'";
			$result1 = pg_exec($empresa1,$SQLQuery1);  
			$SQLQuery1 = "SET DateStyle TO 'European'";
			$result1 = pg_exec($empresa1,$SQLQuery1);  
			return $empresa1;
		}// end if
	}// end connectbulmages
	
}// end class paramsincr

?>