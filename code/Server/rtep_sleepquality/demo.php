<?php
$mysqli = new mysqli("localhost","username","password","databasename");
$mysqli->set_charset('utf8');
$query = 'SELECT * FROM information_list';
$result = $mysqli->query($query);
$arr = $result->fetch_all(MYSQLI_ASSOC);
$info = json_encode($arr);
echo $json = '{"success":true,"info":'.$info.'}';
