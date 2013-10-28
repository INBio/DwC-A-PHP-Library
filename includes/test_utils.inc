<?php 

function test_result($test_name, $pass = TRUE, $reason = "Baaaaad programmer working" ){

  $result = '';

  if($pass === TRUE){
    $result  = "[PASS] $test_name \n\n";
  }else{
    $result  = "[FAIL] $test_name -> ($reason)\n\n";
  }

  print $result;
}


?>
