


<?php
$title = '';
$body = '';
if (is_file($section = "contents/" . $_GET['go'] . ".inc")) {
	include_once $section;
} else if (is_file(($project = "projects/" . $_GET['prj'] . "/index.inc"))) {
	include_once $project;
} else {
	if (! isset($_GET['go'])) {
		include_once "contents/inicio.inc";
	} else {
		$title = "Sección en construcción";
	};
};

?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>

<head>
  <title>IGLUES</title>
  <meta name="GENERATOR" content="Quanta Plus">
  <?php
  ($stylesheet = $_GET['css']) || $stylesheet = "iglues.css";
  echo "  <link rel=\"stylesheet\" type=\"text/css\" href=\"$stylesheet\">\n";
  ?>
</head>
<?php
//echo "<body onLoad=\"easyinit()\">\n";
?>
<div id="header">
<div id="section_title">
<?php
echo "<h1>$title</h1>";
?>
</div>
</div>
	<div id="navbar">
	<div id="navbar_title"><div style="position:relative; top:.8em;">&nbsp;<strong>Contenidos:</strong></div></div>
	<?php include "navbar.inc"; ?>
	</div>
	




	<div id="contents">
	
	<?php
//	include "navmenu.inc";
	echo "<div id=\"contents_margins\">\n";

	echo $body;
	?>
	<br>
	</div>
	
	</div>

</body>
</html>
