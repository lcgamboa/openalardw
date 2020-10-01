<?PHP

$servername = "localhost";
$username = "usuario";
$password = "senha";
$dbname='alardwifi';

$key=$_POST["id"];
$cmd=$_POST["cmd"];


$sql="";
if('AL_DISP_ON' == $cmd){
  $code=hexdec($_POST["code"]);
  $sql="INSERT INTO `eventos` (`chave`, `evento`, `atributo`, `datahora`) VALUES (NULL, 'DON', '$code', NOW());";
}
else if('AL_DISP_OFF' == $cmd){
  $sql="INSERT INTO `eventos` (`chave`, `evento`, `atributo`, `datahora`) VALUES (NULL, 'DOF', NULL , NOW());";
}
else if('AL_WIFI_ON' == $cmd){
  $sql="INSERT INTO `eventos` (`chave`, `evento`, `atributo`, `datahora`) VALUES (NULL, 'AON', NULL , NOW());";
}
else if('AL_WIFI_OFF' == $cmd){
  $sql="INSERT INTO `eventos` (`chave`, `evento`, `atributo`, `datahora`) VALUES (NULL, 'AOF', NULL , NOW());";
}
else if('AL_CONFIG' == $cmd){
  $sql="INSERT INTO `eventos` (`chave`, `evento`, `atributo`, `datahora`) VALUES (NULL, 'CFG', NULL , NOW());";
}
else if('AL_KEEP_AL' == $cmd){
  $sql="INSERT INTO `eventos` (`chave`, `evento`, `atributo`, `datahora`) VALUES (NULL, 'KAL', NULL , NOW());";
}else if('AL_CADASTRO' == $cmd){
  $code=hexdec($_POST["code"]);
  $sql="INSERT INTO `eventos` (`chave`, `evento`, `atributo`, `datahora`) VALUES (NULL, 'CAD', '$code', NOW());";
}else if('AL_STATUS' == $cmd){
  $sql="SELECT valor FROM `status` WHERE estado = 'AL_ON';"; 
}


if(strlen($sql)> 0)
{
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }
  $result = $conn->query($sql);
  $conn->close();
  
    if ($result->num_rows > 0) {
      $row = $result->fetch_assoc();

      if($row["valor"] == "0"){
        echo "[Alarme:Off]";
      }
      else
      {
        echo "[Alarme:On]";
      }
    }  
}

?>
