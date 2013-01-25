<?php

class ArchiveFile{

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

  function get_fields_terminated_by(){
    return  $this->fields_terminated_by;
  }

  function get_fields_enclosed_by(){
    return  $this->fields_enclosed_by;
  }

  function get_lines_terminated_by(){
    return  $this->lines_terminated_by;
  }

  function get_encoding(){
    return  $this->encoding;
  }

  function get_row_type(){
    return  $this->row_type;
  }

  function get_ignoreheader_lines(){
    return  $this->ignoreheader_lines;
  }

  function get_date_format(){
    return  $this->date_format;
  }

  function get_location(){
    return  $this->location;
  }

  function set_fields_terminated_by($character){
    $this->fields_terminated_by   = $character  ;
  }

  function set_fields_enclosed_by($character){
    $this->fields_enclosed_by   = $character  ;
  }

  function set_lines_terminated_by($character){
    $this->lines_terminated_by   = $character  ;
  }

  function set_encoding($encoding){
    $this->encoding   = $encoding  ;
  }

  function set_row_type($row_type){
    $this->row_type   = $row_type  ;
  }

  function set_ignoreheader_lines($bool){
    $this->ignoreheader_lines   = $bool  ;
  }

  function set_date_format($format){
    $this->date_format   = $format  ;
  }

  function set_location($path){
    $this->location   = $path  ;
  }

  function set_fields($new_fields){
    $this->fields   = $new_fields  ;
  }

  function get_fields(){
    return $this->fields;
  }

  function set_defaults($new_defaults){
    $this->defaults   = $new_defaults  ;
  }

  function get_defaults(){
    return $this->defaults;
  }
}

?>
