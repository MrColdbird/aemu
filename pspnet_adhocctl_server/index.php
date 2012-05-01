<html>
<head>
<title>PRO Online Server Status</title>
</head>
<body style="background-color:white; color:black; font-family:sans-serif;">
<div align="center">
<?php
	// Game Product ID Resolver
	function gamename($productcode)
	{
		// God Eater Burst
		if($productcode == "ULUS10563") return "God Eater Burst";
		
		// Unknown Product Code
		return "Unknown Title";
	}
	
	// Log File Path
	$path = "/var/www/status/status.log";
	
	// Open Log File
	$fd = fopen($path, "r");
	
	// Opened Log File
	if($fd)
	{
		// Read Server Global User Count
		$serveruc = rtrim(fgets($fd));
		
		// Read Success
		if($serveruc)
		{
?>
<div style="padding:10px; margin:5px;">
	<b><?php echo($serveruc); ?> Users are currently playing on <a href="http://www.prometheus.uk.to">Prometheus Online</a>!</b>
</div>
<?php
			// Counter
			$counter = 0;
			
			// Read Game Information
			while(($gamedata = fgets($fd)))
			{
				// Split Line Data
				$gamedata_array = explode(":", rtrim($gamedata));
				
				// Valid Game Information
				if(count($gamedata_array) == 3)
				{
?>
<div style="padding:10px; background-color:<?php if($counter % 2 == 0) echo("#C8C8C8"); else echo("#E8E8E8"); ?>; border-style:solid; border-color:#A0A0A0; border-width:1px; margin:5px; float:left;">
	<div><img src="icons/<?php if(file_exists("icons/" . $gamedata_array[0] . ".PNG")) echo($gamedata_array[0]); else echo("UNKNOWN"); ?>.PNG" style="border-style:solid; border-color:#A0A0A0; border-width:2px;" /></div>
	<div style="font-size:0.8em;"><i><?php echo($gamedata_array[0]); ?></i></div>
	<div style="font-size:0.8em; color:#444444;"><b><?php echo(gamename($gamedata_array[0])); ?></b></div>
	<div style="font-size:0.8em; margin-top:10px;">Idle [<span style="color:red;"><?php echo($gamedata_array[1]); ?></span>] Active [<span style="color:green;"><?php echo($gamedata_array[2]); ?></span>]</div>
</div>
<?php
				}
				
				// Increase Counter
				$counter++;
			}
		}
		
		// Close Log File
		fclose($fd);
	}
?>
</div>
</body>
</html>
