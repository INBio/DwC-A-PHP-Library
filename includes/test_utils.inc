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


/** 
 * Format of the test array
 *

  $tests = Array(
    Array( $function, $file, $result), 
  );

*/
function run_tests( $dir, $tests){

  foreach($tests as $test){
    $func = $test[0];
    $file = $test[1];
    $result = $test[2];
    print "<#> Running $func in $dir with $file especting $result \n\n";
    call_user_func($func, $dir.'/'.$file);
  }
}

?>
