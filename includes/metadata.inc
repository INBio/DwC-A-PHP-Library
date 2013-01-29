<?php

class EMLDocument{
  public $filename;
  public $dataset;
  public $additional_metadata;
}

class AdditionalMetadata{

  public $date_stamp;
  public $hierarchy_level;
  public $citation;
  public $bibliography = Array();
  public $physical = Array();
  public $resource_logo_url;
  public $collection;
  public $formation_period;
  public $specimen_observation_method;
  public $living_time_period;
}


class Collection{
  public $parent_collection_identifier;
  public $collection_identifier;
  public $collection_name;
}

class Citation{
  public $identifier;
  public $citation;
}

class Physical{

  public $object_name;
  public $character_encoding;
  public $format;
  public $charset;
  public $distribution_url;
  public $name;
}

class DataFormat{
  public $format_name;
  public $format_value;
}

class Dataset{
  public $alternate_identifier = Array(); // String
  public $titles = Array(); //Title
  public $creator; // Agent
  public $metadata_provider; // Contact 
  public $associated_party; // Contact
  public $pub_date; // String
  public $language; // String
  public $abstract; // String
  public $keyword_set = Array(); //KeywordSet
  public $aditional_info; // String
  public $intellectualRights; // String
  public $distribution; //:S
  public $coverage; // Coverage
  public $purpose; // String
  public $contact; //Contact
  public $methods = Array(); //Method
  public $project; // Project
}

// -------------------------------------

class Project{
  public $title;
  public $personnel = Array();
  public $funding;
  public $study_area_description;
  public $design_description;
}


class Method{
  public $method_step;
  public $sampling;
  public $quality_control;
}

class Sampling{
  public $study_extent;
  public $description;
}

class Coverage{}

class GeographicCoverage extends Coverage{
  public $geographic_description;
  public $bounding_box;
}

class TemporalCoverage extends Coverage{

  public $single;
  public $begin;
  public $end;

}

class TaxonomicCoverage extends Coverage{
  public $general_taxonomic_coverage;
  public $taxon_rank_name;
  public $taxon_rank_value;
  public $common_name;
  
}

class TimeKeyword{
  public $start;
  public $end;
}

class TaxonKeyword{

  private $taxon_rank_name;
  private $taxon_rank;
  private $common_name;
}

class KeywordSet{
  public $keyword = Array();
  public $thesaurus;
}

class Agent{
  public $given_name;
  public $last_name;
  public $organisation_name;
  public $position_name;
  public $address;
  public $phone;
  public $electronic_mail_address;
  public $online_url;
  public $role;
}

class Address{
  public $delivery_point;
  public $city;
  public $administrative_area;
  public $postal_code;
  public $country;
}

class Title{

  public $lang;
  public $text;
}
?>
