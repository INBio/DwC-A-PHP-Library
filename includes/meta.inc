<?php

class Field{

  //Attributes
  private $index = 0;
  private $term = '';
  private $default = '';
  private $vocabulary = '';

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

class Files{

  // list of the files (path or urls).
  private $location = Array();

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

class Core{

  // Atributes
  private $row_type;
  private $fields_terminated_by;
  private $lines_terminated_by;
  private $fields_enclosed_by;
  private $encoding;
  private $ignore_header_lines;
  private $date_format;

  // Elements
  private $files = new Array();
  private $id = 0; # indicates the index of the id
  // only for extensions
  private $core_id = 0; # indicates the index of the id that is linked to the core.
  private $fields = new Array();

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

class Extension{
  // Atributes
  private $row_type;
  private $fields_terminated_by;
  private $lines_terminated_by;
  private $fields_enclosed_by;
  private $encoding;
  private $ignore_header_lines;
  private $date_format;

  // Elements
  private $files = new Array();
  private $core_id = 0; # indicates the index of the id that is linked to the core.
  private $fields = new Array();

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


class Archive{


  // Metadata
  $metadata = '';

  // Files
  private $core = new Core();
  private $extensions = Array(); 



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

