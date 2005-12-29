
<?php
// identifica si el navegador es Internet Explorer
$user_ag = $_SERVER['HTTP_USER_AGENT'];
if ( ereg( "MSIE", $user_ag ) ) {
    $IE = 1;
} else {
    $IE = 0;
}


$menu = array();

if (is_file('contents/menu.inc')) {
	include "contents/menu.inc";
};

foreach ($menu as $title => $modname) {
	if ($IE==0) {
	// Versión para navegador compatible
		echo "<a href=\"$PHP_SELF?go=" . urlencode($modname) . "\"><div>&nbsp;*&nbsp;" . htmlentities($title) . "</div></a>\n";
	} else {
	// versión para internet explorer
		echo "<a href=\"$PHP_SELF?go=" . urlencode($modname) . "\">&nbsp;*&nbsp;" . htmlentities($title) . "</a><BR>\n";
	}	
	
};

?>
