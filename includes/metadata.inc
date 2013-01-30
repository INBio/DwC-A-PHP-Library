<?php

include_once('includes/archive.inc');

class EMLDocument {
  private $filename;
  private $dataset;
  private $additional_metadata;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class AdditionalMetadata {

  private $date_stamp;
  private $hierarchy_level;
  private $citation;
  private $bibliography = Array();
  private $physical = Array();
  private $resource_logo_url;
  private $collection;
  private $formation_period;
  private $specimen_preservation_method;
  private $living_time_period;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}


class Collection {
  private $parent_collection_identifier;
  private $collection_identifier;
  private $collection_name;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class Citation {
  private $identifier;
  private $citation;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class Physical {

  private $object_name;
  private $character_encoding;
  private $format;
  private $charset;
  private $distribution_url;
  private $name;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class DataFormat {
  private $format_name;
  private $format_value;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class Dataset {
  private $alternate_identifier = Array(); // String
  private $title; //String
  private $creator; // Agent
  private $metadata_provider; // Contact 
  private $associated_party; // Contact
  private $pub_date; // String
  private $language; // String
  private $abstract; // String
  private $keyword_set = Array(); //KeywordSet
  private $additional_info; // String
  private $intellectual_rights; // string
  private $distribution; //:s
  private $coverage; // Coverage
  private $purpose; // String
  private $contact; //Contact
  private $methods = Array(); //Method
  private $project; // Project

  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt: $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt: $dt");
    }
  }
}

// -------------------------------------

class Project {
  private $title;
  private $personnel = Array();
  private $funding;
  private $study_area_description;
  private $design_description;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}


class Method {
  private $method_step;
  private $sampling;
  private $quality_control;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class Sampling {
  private $study_extent;
  private $description;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class Coverage {}

class GeographicCoverage extends Coverage{
  private $geographic_description;
  private $bounding_box;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class TemporalCoverage extends Coverage{

  private $single;
  private $begin;
  private $end;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

}

class TaxonomicCoverage extends Coverage{
  private $general_taxonomic_coverage;
  private $taxon_rank_name;
  private $taxon_rank_value;
  private $common_name;

  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
  
}

class TimeKeyword {
  private $start;
  private $end;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class TaxonKeyword {

  private $taxon_rank_name;
  private $taxon_rank;
  private $common_name;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class KeywordSet {
  private $keyword = Array();
  private $thesaurus;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class Agent {
  private $given_name;
  private $last_name;
  private $organisation_name;
  private $position_name;
  private $address;
  private $phone;
  private $electronic_mail_address;
  private $online_url;
  private $role;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class Address {
  private $delivery_point;
  private $city;
  private $administrative_area;
  private $postal_code;
  private $country;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}

class Title {

  private $lang;
  private $text;
  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variable $dt");
    }
  }
}
?>
