<?php
$menu = array();

if (is_file('contents/menu.inc')) {
	include "contents/menu.inc";
};

foreach ($menu as $title => $modname) {
	echo "<a href=\"$PHP_SELF?go=" . urlencode($modname) . "\"><div>&nbsp;*&nbsp;" . htmlentities($title) . "</div></a>\n";
};

?>
