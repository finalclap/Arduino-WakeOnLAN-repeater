<?php
/**
 * IP Change Logger
 * ----------------
 * This script write client's IP address into a log file.
 * Calling it from the Arduino (via HTTP), it is used to know your public IP address,
 * the one used by the Arduino and every other device plugged to the same router (LAN),
 * without using a dynamic DNS service like DynDNS or no-ip.
 * 
 * Date: 19/01/2012
 * PHP 5.2.10 or newer
 * © www.finalclap.com
 */
header('Content-Type: text/plain');

# Computer name is used to create different log files (one for every client as you want)
$machine = isset($_GET['machine']) ? $_GET['machine'] : 'default';
$machine_logfile = 'ipchange_'.$machine.'.log';
$ip = $_SERVER['REMOTE_ADDR'];

# Checking if we must log the IP address (we need to log only if the IP have changed)
$must_log = true;
if( file_exists($machine_logfile) ){
	$handle = fopen($machine_logfile, 'r');
	$last_line = '';
	while( ($line = fgets($handle)) !== false ){
		$line = trim($line);
		if( !empty($line) ){
			$last_line = $line;
		}
	}
	fclose($handle);
	
	# If the last recorded IP address is the current IP, the log file is up to date so we don't need to update it
	if( preg_match('#^([0-9\.]+)#', $last_line, $matches) ){
		$last_known_ip = $matches[1];
		if( $last_known_ip == $ip ){
			$must_log = false;
			die('Already logged, no changes.');
		}
	}
}

# Updating the log file
if( $must_log ){
	$row = $ip . "\t[" . date('d/m/Y H:i:s') . "]\t" . $machine . "\n";
	$handle = fopen($machine_logfile, 'a+');
	fputs($handle, $row);
	fclose($handle);
}
?>