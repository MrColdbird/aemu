<html>
<head>
<title>PRO Online Server Status</title>
</head>
<body style="background-color:white; color:black; font-family:sans-serif;">
<div align="center">
<?php
	// Unknown Product ID Logger (for easier manual name resolving)
	function logcode($productcode)
	{
		// Log Folder doesn't exist
		if(!file_exists("log"))
		{
			// Create Log Folder
			mkdir("log");
		}
		
		// Log Folder exists
		if(is_dir("log"))
		{
			// Path Puzzling
			$path = "log/" . $productcode;
			
			// Create Product ID File for "ls"
			if(!file_exists($path)) touch($path);
		}
	}
	
	// Game Product ID Resolver
	function gamename($productcode)
	{
		// Ace Combat Joint Assault
		if($productcode == "ULUS10511") return "Ace Combat X2";
		
		// Armored Core 3 Portable
		if($productcode == "NPUH10023") return "Armored Core 3";
		
		// Blood Bowl
		if($productcode == "ULUS10516") return "Blood Bowl";
		
		// Dissidia Final Fantasy
		if($productcode == "ULUS10437") return "Dissidia FF";
		
		// Dissidia Duodecim Final Fantasy
		if($productcode == "ULUS10566") return "Dissidia 012 FF";
		
		// Dungeon Siege Throne of Agony
		if($productcode == "ULUS10177") return "Dungeon Siege TOA";
		
		// Fat Princess Fistful of Cake
		if($productcode == "UCUS98740") return "Fat Princess";
		
		// Japanese God Eater
		if($productcode == "ULJS00237") return "God Eater";
		
		// God Eater Burst
		if($productcode == "ULUS10563") return "God Eater Burst";
		
		// K-ON! Houkago Live
		if($productcode == "ULJM05709") return "K-ON! Houkago Live";
		
		// Japanese Metal Gear Solid Peace Walker
		if($productcode == "NPJH50045") return "MGS Peace Walker";
		
		// Metal Gear Solid Portable Ops+
		if($productcode == "ULUS10290") return "MGS Portable Ops+";
		
		// Metal Gear Solid Portable Ops
		if($productcode == "ULUS10202") return "MGS Portable Ops";
		
		// Split Second
		if($productcode == "ULUS10513") return "Split Second";
		
		// SOCOM Fireteam Bravo
		if($productcode == "UCUS98615") return "SOCOM FTB";
		
		// SOCOM Fireteam Bravo 2
		if($productcode == "UCUS98645") return "SOCOM FTB2";
		
		// SOCOM Fireteam Bravo 3
		if($productcode == "UCUS98716") return "SOCOM FTB3";
		
		// Soul Calibur - Broken Destiny
		if($productcode == "ULUS10457") return "Soul Calibur";
		
		// Street Fighter Alpha 3
		if($productcode == "ULUS10062") return "SF Alpha 3";
		
		// Tekken 6
		if($productcode == "ULUS10466") return "Tekken 6";
		
		// Midnight Club LA Remix
		if($productcode == "ULUS10383") return "Midnight Club LA Remix";
		
		// Mod Nation Racers
		if($productcode == "UCUS98741") return "Mod Nation Racers";
		
		// Monster Hunter Portable 2nd G (MHFU)
		if($productcode == "ULUS10391") return "Monster Hunter 2nd G";
		
		// N+
		if($productcode == "ULUS10340") return "N+";
		
		// Japanese Monster Hunter Portable 3rd
		if($productcode == "ULJM05800") return "Monster Hunter 3";
		
		// MLB11 The Show
		if($productcode == "UCUS98758") return "MLB11 The Show";
		
		// Outrun 2006 Coast 2 Coast
		if($productcode == "ULUS10064") return "Outrun 2006 C2C";
		
		// Warship Gunner 2 Portable
		if($productcode == "ULJM05553") return "Warship Gunner 2";
		
		// Untold Legends 2 The Warriors Code
		if($productcode == "ULUS10086") return "Untold Legends 2";
		
		// Untold Legends Brotherhood of the Blade
		if($productcode == "ULUS10003") return "Untold Legends";
		
		// Pangya Fantasy Golf
		if($productcode == "ULUS10438") return "Pangya";
		
		// PES 2012
		if($productcode == "ULUS10586") return "PES 2012";
		
		// Virtua Tennis 3
		if($productcode == "ULUS10246") return "Virtua Tennis 3";
		
		// Worms Open Warfare
		if($productcode == "ULUS10065") return "Worms Open Warfare";
		
		// Worm Open Warfare 2
		if($productcode == "ULUS10260") return "Worms Open Warfare 2";
		
		// Worm Battle Islands
		if($productcode == "NPUH10045") return "Worms Battle Islands";
		
		// Log Unknown Product Code
		logcode($productcode);
		
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
		if($serveruc >= 0)
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
