<!DOCTYPE HTML>
<html>
<head>
<style>
</style>
</head>
<body>


<?php
	$start = $_POST["start"];
	$stop = $_POST["stop"];
	$update = $_POST["update"];
	$delete = $_POST["delete"];
	$sensors = $_POST["sensors"];
	$script = $_POST["script"];

	$status = "";
	if(!empty($start)) {
		$status="Sampling...";
		$val=shell_exec("sudo sh start.sh $sensors 2>&1");
	}
	if(!empty($stop)){
		$status="Inactive...";
		$val=shell_exec("sudo sh kill.sh 2>&1");
	}

	if(!empty($update)){
		$val=shell_exec("sudo sh update.sh 2>&1");
	}
	if(!empty($delete)){
		$val=shell_exec("sudo sh delete.sh 2>&1");
	}

	echo "Status of sampling: $status";

?>
<h1>Commands: </h1>
<form method="post" action="sensor.php">
	Enter amount of sensors to measure from:<br>
	<input type="input" name="sensors">
	<button name="start" value="start" type="submit">Start Sampling</button>
	<br><br>
	<button name="stop" value="stop" type="submit">Stop Sampling</button>
	<br><br>
	<button name="update" value="update" type="submit">Update Database</button>
	<br><br>
	<button name="delete" value="delete" type="submit">Empty Database</button>
	<br><br>
	<textarea name="script" value="script" type="submit" rows="10" cols="30">Enter GNUPlot Script</textarea>
	<br><br>
</form>


<?php
        echo "<h2>Last input: </h2>";
        echo "Start:  $start <br>";
	echo "Stop:  $stop <br>";
	echo "Update:  $update <br>";
	echo "Delete:  $delete <br>";
	echo "Sensors:  $sensors<br>";
	echo "Command: $cmd<br>";
	echo "Script:  $val<br>";
	echo "GNUScript: $script<br>";
	echo "<br><br>";
?>


<a href="http://165.227.145.19:3000/d/tHwdperMz/final-dahsboard?orgId=1">WEBSITE TO DISPLAY VALUES!</a>

</body>
</html>
