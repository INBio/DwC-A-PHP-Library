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

test_open_dwca_file();
#$dwca->open();
#$rows = $dwca->get_core_info(0, 50);

?>
