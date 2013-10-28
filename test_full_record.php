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
    $rows = $dwca->get_records(0, 50);

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

  print "# ------------ # \n";
  var_dump($rows[0]->data['core']['id']);
  var_dump($rows[0]->data['http://rs.gbif.org/terms/1.0/Image']);

  print "# ------------ # \n";
  var_dump($rows[1]->data['core']['id']);
  var_dump($rows[1]->data['http://rs.gbif.org/terms/1.0/Image']);

  print "# ------------ # \n";
  var_dump($rows[2]->data['core']['id']);
  var_dump($rows[2]->data['http://rs.gbif.org/terms/1.0/Image']);

  test_result('Test full records', count($rows) === 3, "Only ".count($rows)." founded!" );
}


$dir = 'examples';
$dh = opendir($dir);

$functions = Array('test_full_records');
#$functions = Array('test_get_records', 'test_list_extensions');

#test_full_records('examples/eol.zip');
#test_full_records('examples/plic-4-images.zip');
test_full_records('examples/plic-3-registers-8-images.zip');

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
