<?php 

include_once('includes/test_utils.inc');
include_once('dwca.inc');

function test_open_dwca_file(){

  try{
    $dwca = new DwCA('examples/dwca-eol.zip');
  }catch (Exception $ex){
    $dwca = FALSE;
  }

  test_result('Open DwC-A file', ($dwca != FALSE));
}


function test_open(){

  $core = FALSE;
  try{
    $dwca = new DwCA('examples/dwca-benin.zip');
    $dwca->open();
    $core = $dwca->get_core();

  }catch (Exception $ex){
    $core = false;
  }

  test_result('DwCA::open ', ($core->get_location()  === 'occurrence.txt'));
}

function test_get_records(){

  $core = FALSE;
  try{
    $dwca = new DwCA('examples/dwca-eol.zip');
    $dwca->open();
    $core = $dwca->get_core();
    $rows = $dwca->get_records($core, 0, 50);

  }catch (Exception $ex){
    $rows = FALSE;
  }

  test_result('Test get records from archive file', (count($rows) == 50));
}


function test_get_extensions(){

  $extensions = FALSE;
  try{
    $dwca = new DwCA('examples/dwca-eol.zip');
    $dwca->open();
    $extensions = $dwca->get_extensions();

  }catch (Exception $ex){
    $extensions = FALSE;
  }

  test_result('Test get extensions', (count($extensions) == 4));
}

function test_get_metadata_dataset(){

  $eml = False;

  try{
    $dwca = new DwCA('examples/eml.zip');
    $dwca->open();
    $eml = $dwca->get_metadata();

  }catch (Exception $ex){
    $eml = FALSE;
    print $ex;
  }

  test_result('Test get_metadata: dataset', ($eml->dataset->metadata_provider->address->delivery_point === "Universitestparken 15" ));
}

function test_get_metadata_additional_metadata(){

  $eml = False;

  try{
    $dwca = new DwCA('examples/eml.zip');
    $dwca->open();
    $eml = $dwca->get_metadata();

  }catch (Exception $ex){
    $eml = FALSE;
    print $ex;
  }

  test_result('Test get_metadata: additional_metadata', ($eml->additional_metadata->collection->collection_name === "Mammals" ));}

test_open_dwca_file();
test_open();
test_get_records();
test_get_extensions();
test_get_metadata_dataset();
test_get_metadata_additional_metadata();

?>
