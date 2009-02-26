#!/usr/bin/php -q
<?php
{
    // Version 1.11 2005-Nov-15
    // ------------
    // CONFIG Parms
    // ------------

    /// Parametros con la base de datos.
	$host = "192.168.0.36";
	$dbname = "bulmafact";
	$port   = "5432";
	$user   = "tborras";
	$password = "tborras";

    
    // If the application is having problems you can log to this file
    $parm_error_log = '/tmp/wakeup.log';
    
    // Set to 1 to turn on the log file
    $parm_debug_on = 1;	
    
    // This is where the Temporary WakeUp Call Files will be created
    $parm_temp_dir = '/tmp';
    
    // This is where the WakeUp Call Files will be moved to when finished
    $parm_call_dir = '/var/spool/asterisk/outgoing';

    // How many times to try the call
    $parm_maxretries = 3;
    
    // How long to keep the phone ringing
    $parm_waittime = 60;		// How long to keep the phone ringing
    
    // Number of seconds between retries
    $parm_retrytime = 60;
    
    // Caller ID of who the wakeup call is from Change this to the extension you want to display on the phone
    $parm_wakeupcallerid = '"WakeUp" <*62>';
    
    
    // Set to 1 to use the Channel
    // Set to 0 for Caller ID,  Caller IS is assumed just a number ### or "Name Name" <##>
    // The big difference is using caller ID will call ANY phone with that extension number
    // Where using Channel will only wake up the one specific phone
    $parm_chan_ext = 0;

    // ----------------------------------------------------
    // Which application to run when the call is connected.  
    $parm_application = 'MusicOnHold';
	$parm_data = '';

	// -- Use this for the ANNOY application
	//$parm_application = 'AGI';
	//$parm_data = 'wakeconfirm.agi';
    // ----------------------------------------------------

    
    //-------------------
    // END CONFIG PARMS
    //-------------------
    
    GLOBAL	$stdin, $stdout, $stdlog, $result, $parm_debug_on, $parm_test_mode;
    
    // These setting are on the WIKI pages http://www.voip-info.org
    ob_implicit_flush(false);
    set_time_limit(30);
    error_reporting(0);
    
    $stdin = fopen( 'php://stdin', 'r' );
    $stdout = fopen( 'php://stdout', 'w' );
    
    // You will see a whole bunch of this its for development or if you change anything and
    // want to write to a log file in the TMP directory
    if ($parm_debug_on)
    {
        $stdlog = fopen( $parm_error_log, 'w' );
        fputs( $stdlog, "---Start---\n" );
    }
    
    
    // ASTERISK * Sends in a bunch of variables, This accepts them and puts them in an array
    // http://www.voip-info.org/tiki-index.php?page=Asterisk%20AGI%20php
    while ( !feof($stdin) ) 
    {
        $temp = fgets( $stdin );
        
        if ($parm_debug_on)
            fputs( $stdlog, $temp );
        
        // Strip off any new-line characters
        $temp = str_replace( "\n", "", $temp );
        
        $s = explode( ":", $temp );
        $agivar[$s[0]] = trim( $s[1] );
        if ( ( $temp == "") || ($temp == "\n") )
        {
            break;
        }
    } 
    
    $almacen = $agivar[agi_callerid];
    
    
    // There are two ways to contact a phone, by its channel or by its local 
    // extension number.  This next session will extract both sides
    
    // split the Channel  SIP/11-3ef4  or Zap/4-1 into is components
    $channel = $agivar[agi_channel];
    if (preg_match('.^([a-zA-Z]+)/([0-9]+)([0-9a-zA-Z-]*).', $channel, $match) )
    {
        $sta = trim($match[2]);
        $chan = trim($match[1]);
    }
    
    
    
    // Go Split the Caller ID into its components
    $callerid = $agivar[agi_callerid];
    
    // First look for the Extension in <####> 
    if (preg_match('/<([ 0-9]+)>/', $callerid, $match) )
    {
        $cidn = trim($match[1]);
    }
    else	// Did not find the <##...> look for the first number in the string to use as CID
    {
        if (preg_match('/([0-9]+)/', $callerid, $match) )
        {
            $cidn = trim($match[1]);
        }
        else
            $cidn = -1;		// I'm saying an error no caller id # found
    }
    
    // Check if we have an outstanding Wakeup Call file
    if ( $parm_chan_ext )
        $dir_check = "$chan.$sta.call";
    else
        $dir_check = "ext.$cidn.call";
    
    if ($parm_debug_on)
        fputs( $stdlog, "Checking Directory [$parm_call_dir] Check=[$dir_check]\n" );
    
    // I started to think we could have many outstanding wakup calls, but then
    // it got very confusing on how to delete just one of them.  I wasn't about
    // to prompt each and every one.  So I went back to JUST ONE wakeup call
    // But this will get a list of all of them incase of problems
    $outc=0;
    $dir_handle = opendir( $parm_call_dir );
    while( $file = readdir($dir_handle ) )   {
        if ($parm_debug_on)
            fputs( $stdlog, "File=$file\n" );
        
        // Check if we have an outstanding wakup call
        if (strstr( $file, $dir_check ) )
            $out[$outc++] = $file;
    }
    closedir( $dir_handle );
    
    
    //=========================================================================
    // This is where we interact with the caller.  Answer the phone and so on
    //=========================================================================
    
    
    $rc = execute_agi( "ANSWER ");
    
    sleep(1);
    
            $rc = execute_agi( "STREAM FILE custom/bienvenido_control \"0123456789\" ");
            $rc[result] = 0;
            while ( !$rc[result] ) {
	        $rc = execute_agi( "GET DATA custom/porfavorcontrasenya 15000 4 ");
                
                if ( $rc[result] != -1 )      {
                    // They entered 4 digits,  check if its a valid time or they hung up
                    if ( strlen( $rc[result]) < 4 || $rc[result] < 0)  {
                        $rc[result] = 0;
                        $rc = execute_agi( "STREAM FILE custom/contrasenyaincorrecta \"\" ");
                    } // end if
		    
		    /// Si la contrasenya tiene longitud correcta comprobamos con la base de datos.
		    if ( strlen($rc[result])>= 4) {
                        // Save the time entered
                        $trabajador = $rc[result];
			if (comprueba_validacion($trabajador, $almacen) == FALSE) {
                           $rc = execute_agi( "STREAM FILE custom/contrasenyaincorrecta \"\" ");
                           $rc[result] = 0;
			} // end if
		    } // end if
		    
                } // end if
            } // end while
            
            $rc[result] = 0;

    if ($param_debug_on)
       fputs ($stdlog, "---DATABASE ---\n");
    $err = genera_validacion($trabajador, $almacen);


    if ( !$rc[result]) 
	$rc = execute_agi( "SAY DIGITS $trabajador \"\" ");
    if ( !$src[result])
        $rc = execute_agi( "STREAM FILE custom/diganombre \"\" ");

    $cad = $trabajador."_".$almacen."_".date("m-d-y_H:i:s");

    if ( !$rc[result])
	$rc = execute_agi( "RECORD FILE /tmp/password$cad wav \"0123456789\" 5000 ");
    if ( !$rc[result] )
        $rc = execute_agi( "STREAM FILE custom/gracias \"\" ");

    if ( !$rc[result] )
        $rc = execute_agi( "HANGUP");
    if ($parm_debug_on)
        fclose($stdlog);
	
    exit;
}



function genera_validacion($trab, $alm) {
    GLOBAL	$stdin, $stdout, $stdlog, $parm_debug_on, $host, $dbname, $port, $user, $password;
    fflush( $stdout );
    if ($parm_debug_on)
        fputs( $stdlog,"---- DATABASE ----". $trab ."--".$alm. "\n" );
    
	$conn = pg_connect("host=".$host." dbname=".$dbname." port=".$port." user=".$user." password=".$password);
	if (!$conn) {
	echo "An error occured.\n";
	exit;
	} else {
	pg_exec($conn, "SET datestyle TO 'European'");
	pg_exec($conn, "SET datestyle TO 'SQL'");
	pg_exec($conn, "SET client_encoding to 'UTF-8'");
	pg_exec($conn, "SELECT validacionasterisk('$trab','$alm')");
	}
	return 0;
}



function comprueba_validacion($trab, $alm) {

    GLOBAL	$stdin, $stdout, $stdlog, $parm_debug_on, $host, $dbname, $port, $user, $password;
    fflush( $stdout );
    if ($parm_debug_on)
        fputs( $stdlog,"---- DATABASE ----". $trab ."--".$alm. "\n" );
    
	$conn = pg_connect("host=".$host." dbname=".$dbname." port=".$port." user=".$user." password=".$password);
	if (!$conn) {
	echo "An error occured.\n";
	exit;
	} else {
	pg_exec($conn, "SET datestyle TO 'European'");
	pg_exec($conn, "SET datestyle TO 'SQL'");
	pg_exec($conn, "SET client_encoding to 'UTF-8'");

        $query = "SELECT * from cuadrante NATURAL LEFT JOIN horario NATURAL LEFT JOIN trabajador NATURAL LEFT JOIN ALMACEN where fechacuadrante  = now()::date AND passasterisktrabajador='".$trab."' AND extasteriskalmacen='".$alm."'  ";
	fputs( $stdlog, $query."\n");
        $result = pg_query($conn, $query );
        if (pg_num_rows($result) == 0) {
             return FALSE;
        } // end if
        } // end if
        return TRUE;
}



//--------------------------------------------------
// This function will send out the command and get 
//	the response back
//--------------------------------------------------
function execute_agi( $command )  {
    GLOBAL	$stdin, $stdout, $stdlog, $parm_debug_on;
    
    fputs( $stdout, $command . "\n" );
    fflush( $stdout );
    if ($parm_debug_on)
        fputs( $stdlog, $command . "\n" );
    
    $resp = fgets( $stdin, 4096 );
    
    if ($parm_debug_on)
        fputs( $stdlog, $resp );
    
    if ( preg_match("/^([0-9]{1,3}) (.*)/", $resp, $matches) )  {
        if (preg_match('/result=([-0-9a-zA-Z]*)(.*)/', $matches[2], $match))  {
            $arr['code'] = $matches[1];
            $arr['result'] = $match[1];
            if (isset($match[3]) && $match[3])
                $arr['data'] = $match[3];
            return $arr;
        }  else  {
            if ($parm_debug_on)
                fputs( $stdlog, "Couldn't figure out returned string, Returning code=$matches[1] result=0\n" );	
            $arr['code'] = $matches[1];
            $arr['result'] = 0;
            return $arr;
        }
   	}   else    {
        if ($parm_debug_on)
            fputs( $stdlog, "Could not process string, Returning -1\n" );
        $arr['code'] = -1;
        $arr['result'] = -1;
        return $arr;
        }
} 
?>
