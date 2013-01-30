<?php

include_once('includes/archive.inc');

class ArchiveFile extends DwCAObject{

  private $fields_terminated_by;
  private $fields_enclosed_by;
  // this is actually ignored by the csvreader and any of \n, \r or \n\r is used
  private $lines_terminated_by;
  private $encoding;
  private $row_type;
  private $ignoreheader_lines;
  private $date_format;
  private $location;
  private $fields;
  private $defaults;

  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variables");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variables");
    }
  }
}

?>
