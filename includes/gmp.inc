<?php
/**
Acording to the "GBIF Metadata Profile, Reference Guide":

  «Metadata, literally “data about data” are an essential component of a data management
  system, describing such aspects as the “who, what, where, when and how” pertaining to a
  resource. In the GBIF context, resources are datasets, loosely defined as collections of
  related data, the granularity of which is determined by the data custodian. Metadata can
  occur in several levels of completeness. In general, metadata should allow a prospective
  end user of data to:

    1. identify/discover its existence,
    2. learn how to access or acquire the data,
    3. understand its fitness-for-use, and
    4. learn how to transfer (obtain a copy of) the data.

  The GBIF Metadata Profile (GMP) was developed in order to standardise how resources get
  described at the dataset level in the GBIF Data Portal2.»

 */

/// GBIF Metadata Profile
class GMP{

  /*
    The elements are categorised as follows:
      Dataset (Resource)
      Project
      People and Organisations
      Keyword Set (General Keywords)
      Coverage
        Taxonomic Coverage
        Geographic Coverage
        Temporal Coverage
      Methods
      Intellectual Property Rights
      Additional Metadata + NCD (Natural Collections Descriptions Data) Related
   */

  private $dataset;
  private $project;
  private $people_and_organizations;
  private $keyword_set;
  private $taxonomic_coverage;
  private $geographic_coverage;
  private $temporal_coverage;
  private $methods;
  private $intellectual_property_rights;
  private $additional_metadata;


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

class Dataset {
  private $alternate_identifier = Array(); // String
  private $title; //String
  private $creator; // Agent
  private $metadata_provider; // Contact 
  private $associated_party; // Contact
  private $contact; // Contact
  private $pub_date; // String
  private $language; // String
  private $additional_info; // String
  private $url; // String
  private $abstract; // String

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

cass PeopleAndOrganizations{
  private $given_name;
  private $sur_name;
  private $organisation_name;
  private $position_name;
  private $role;
  private $phone;
  private $electronic_mail_address;
  private $delivery_point;
  private $postal_code;
  private $city;
  private $administrative_area;
  private $country;
  private $online_url;

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
  private $keyword_thesaurus;

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

class TaxonomicCoverage extends Coverage{
  private $general_taxonomic_coverage;
  private $taxonomic_classification;
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

class GeographicCoverage extends Coverage{
  private $geographic_description;
  private $west_bounding_coordinate;
  private $east_bounding_coordinate;
  private $north_bounding_coordinate;
  private $south_bounding_coordinate;

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


  private $begin_date;
  private $end_date;
  private $single_date_time;

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
  private $quality_control;
  private $sampling;
  private $study_extent;
  private $sampling_description;

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

class IntellectualPropertyRights{

  private $purpose;
  private $intellectual_rights;

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
  private $metadata_language;
  private $hierarchy_level;

  private $citation;
  private $bibliography = Array();
  private $physical = Array();
  private $resource_logo_url;
  private $parent_collection_identifier;
  private $collection_name;
  private $collection_identifier;
  private $formation_period;
  private $living_time_period;
  private $specimen_preservation_method;
  private $jgti_curatorial_unit;

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
