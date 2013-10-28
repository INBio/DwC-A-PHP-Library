<?php 

include_once('includes/test_utils.inc');
include_once('dwca.inc');

function test_open_dwca_file($file){

  try{
    $dwca = new DwCA($file);
  }catch (Exception $ex){
    $dwca = FALSE;
    print $ex;
  }

  test_result('Open DwC-A file', ($dwca != FALSE));
}

function test_open($file){

  $core = FALSE;
  try{
    $dwca = new DwCA($file);
    $dwca->open();
    $core = $dwca->core;

  }catch (Exception $ex){
    $core = false;
    print $ex;
  }

  test_result('DwCA::open ', ($core->location  === 'occurrence.txt'));
}

function test_get_records($file){

  $core = FALSE;
  try{
    $dwca = new DwCA($file);
    $dwca->open();
    $core = $dwca->core;
    $rows = $dwca->get_records($core, 0, 50);

  }catch (Exception $ex){
    $rows = FALSE;
    print $ex;
  }

  test_result('Test get records from archive file', (count($rows) == 50));
}

function test_get_extensions($file){

  $extensions = FALSE;
  try{
    $dwca = new DwCA($file);
    $dwca->open();
    $extensions = $dwca->extensions;

  }catch (Exception $ex){
    $extensions = FALSE;
    print $ex;
  }

  test_result('Test get extensions', (count($extensions) == 4));
}

function test_get_metadata_dataset($field){

  $eml = False;

  try{
    $dwca = new DwCA($file);
    $dwca->open();
    $eml = $dwca->get_metadata();

  }catch (Exception $ex){
    $eml = FALSE;
    print $ex;
  }

  test_result('Test get_metadata: dataset', ($eml->dataset->metadata_provider->address->delivery_point === "Universitestparken 15" ));
}

function test_get_metadata_additional_metadata($file){

  $eml = False;

  try{
    $dwca = new DwCA($file);
    $dwca->open();
    $eml = $dwca->get_metadata();

  }catch (Exception $ex){
    $eml = FALSE;
    print $ex;
  }

  test_result('Test get_metadata: additional_metadata', ($eml->additional_metadata->collection->collection_name === "Mammals" ));}

/** 
 * Format of the test array
 *

  $tests = Array(
    Array( $function, $file, $result), 
  );

*/

$tests = Array(
  Array('test_open_dwca_file', 'examples/dwca-eol.zip', FALSE),
  Array('test_open','examples/dwca-benin.zip' , 50),
  Array('test_get_records','examples/dwca-eol.zip' , 50),
  Array('test_get_extensions', 'examples/dwca-eol.zip', 4),
  Array('test_get_metadata_dataset','examples/eml.zip' , 'Universitestparken 15'),
  Array('test_get_metadata_additional_metadata','examples/eml.zip', 'Mammals'),
);

run_tests('examples', $tests);

?>
