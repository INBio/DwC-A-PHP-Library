<?php 

include_once('dwca.inc');

  function test_1(){

    $dwca = new DwCAFile();
    $dwca->open('examples/dwca-eol.zip');

    $rows = $dwca->get_core_info(0, 50);

    test_result('Test 1', (count($rows)== 50));
  }

test_1();

?>
