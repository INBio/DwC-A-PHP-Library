<?php

class DwCAObject{

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
