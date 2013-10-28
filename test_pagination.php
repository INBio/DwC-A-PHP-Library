<?php 

include_once('includes/test_utils.inc');
include_once('dwca.inc');

ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL);


function test_full_records($file){

  $rows = Array();

  try{
    $dwca = new DwCA($file);
    $dwca->open();

    // parsing occurrence.txt file looking for occurrences
    $iterator = new DWCAIterator($dwca->meta);
    
    $rows = $dwca->get_full_records($iterator, 10);
    $first_id = $rows[0]->data['core']['id'];
    print $rows[0]->data['core']['id']."\n";
    test_result('Test full records 1 iter', $first_id === "331985567");

    $rows = $dwca->get_full_records($iterator, 10);
    $first_id = $rows[0]->data['core']['id'];
    print $rows[0]->data['core']['id']."\n";
    test_result('Test full records 2 iter', $first_id === "277158154");

    $rows = $dwca->get_full_records($iterator, 10);
    $first_id = $rows[0]->data['core']['id'];
    print $rows[0]->data['core']['id']."\n";
    test_result('Test full records 3 iter', $first_id === "277221038");

  }catch (Exception $ex){
    $rows = FALSE;
    print $ex;
  }
}


$dir = 'examples';
$dh = opendir($dir);

$functions = Array('test_full_records');
#$functions = Array('test_get_records', 'test_list_extensions');

#test_full_records('examples/eol.zip');
#test_full_records('examples/plic-4-images.zip');
#test_full_records('examples/plic-3-registers-8-images.zip');
test_full_records('examples/gbiffolder.zip');

/*
  while (false !== ($file = readdir($dh))) {
    if (is_file($dir.'/'.$file) && $file != "." && $file != "..") {
      foreach ($functions as $func) {
        call_user_func($func, $dir.'/'.$file);
      }
    }
  }
*/

?>
