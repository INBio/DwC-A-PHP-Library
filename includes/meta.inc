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
  private $row_type = 'http://rs.tdwg.org/dwc/xsd/simpledarwincore/SimpleDarwinRecord';
  private $fields_terminated_by = ',';
  private $lines_terminated_by  = '\n';
  private $fields_enclosed_by   = '"';
  private $encoding             = 'UTF-8';
  private $ignore_header_lines  = 0;
  private $date_format          = 'YYYY-MM-DD';

  // Elements
  private $files  = NULL;
  private $id     = NULL; # indicates the index of the id
  private $fields = NULL;

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
      throw new Exception("Undefined variables ".$dt);
    }
  }
}

class Extension{
  // Atributes
  private $row_type = 'http://rs.tdwg.org/dwc/xsd/simpledarwincore/SimpleDarwinRecord';
  private $fields_terminated_by = ',';
  private $lines_terminated_by  = '\n';
  private $fields_enclosed_by   = '"';
  private $encoding             = 'UTF-8';
  private $ignore_header_lines  = 0;
  private $date_format          = 'YYYY-MM-DD';

  // Elements
  private $files    = NULL;
  private $core_id  = NULL; # indicates the index of the id that is linked to the core.
  private $fields   = NULL;

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
  private $metadata = NULL;

  // Files
  private $core = NULL;
  private $extensions = NULL; 

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

