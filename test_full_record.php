<?php 

include_once('includes/test_utils.inc');
include_once('dwca.inc');

ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL);

function test_get_records($file ){

  $core = FALSE;
  try{
    $dwca = new DwCA($file);
    $dwca->open();
    $rows = $dwca->get_records(50);

  }catch (Exception $ex){
    $rows = FALSE;
    print $ex;
  }

  test_result("Test get records from archive file [$file]", (count($rows) == 50));
}


function test_list_extensions($file){

  $extensions = FALSE;
  try{
    $dwca = new DwCA($file);
    $dwca->open();
    $extensions = $dwca->meta->extensions;

    print "\n\n\n";
    print "< === > [$file] < === >\n";
    foreach($extensions as $ext){
      print $ext->row_type."\n";
    }

  }catch (Exception $ex){
    $extensions = FALSE;
    print $ex;
  }

  test_result('Test get extensions', count($extensions) >= 0 );
}

function test_full_records($file){

  $rows = Array();


  try{
    $dwca = new DwCA($file);
    $dwca->open();

    // parsing occurrence.txt file looking for occurrences
    $iterator = new DWCAIterator($dwca->meta);
    
    $rows = $dwca->get_full_records($iterator, 10);

  }catch (Exception $ex){
    $rows = FALSE;
    print $ex;
  }

  test_result('Test full records', count($rows) === 3, "Only ".count($rows)." founded!" );
}

/** 
 * Format of the test array
 *

  $tests = Array(
    Array( $function, $file, $result), 
  );

*/

$tests = Array(
  Array('test_full_records', 'plic-3-registers-8-images.zip', 3),
  Array('test_list_extensions', 'plic-3-registers-8-images.zip', 1),
  Array('test_get_records', 'gbiffolder.zip', 50),
);

run_tests('examples', $tests);

?>
