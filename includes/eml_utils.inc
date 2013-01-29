<?php
function process_address($xml){

  $address = new Address();

  $address->deliveryPoint = $xml->deliveryPoint;
  $address->city = $xml->city;
  $address->administrativeArea = $xml->administrativeArea;
  $address->postalCode = $xml->postalCode;
  $address->contry = $xml->country;

  return $address;
}

function process_agent($xml){

  $agent = new Agent();

  $agent->given_name = $xml->individualName->givenName;
  $agent->last_name = $xml->individualName->surName;
  $agent->organisation_name = $xml->organizationName;
  $agent->position_name = $xml->positionName;
  $agent->address = process_address( $xml->address);
  $agent->phone = $xml->phone;
  $agent->electronic_mail_address = $xml->electronicMailAddress;
  $agent->online_url = $xml->onlineUrl;
  $agent->role = $xml->role;

  return $agent;
}

function process_keyword_set($xml){

  $keyword_set = new  KeywordSet();

  $keyword_set->keyword = $xml->keyword;
  $keyword_set->thesaurus =  $xml->keywordThesaurus;

  return $keyword_set;
}

function process_geographic_coverage($xml){

  $coverage = new GeographicCoverage();

  $coverage->geographic_description = $xml->geographicCoverage->geographicDescription;
  $coverage->bounding_box = Array (
    $xml->boundingCoordinate->swestBoundingCoordinate,
    $xml->boundingCoordinate->eastBoundingCoordinate,
    $xml->boundingCoordinate->northBoundingCoordinate,
    $xml->boundingCoordinate->southBoundingCoordinate);

  return $coverage;
}

function process_temporal_coverage($xml){

  $coverage = new TemporalCoverage();

  $coverage->begin = $xml->rangeOfDates->beginDate->calendarDate;
  $coverage->end = $xml->rangeOfDates->endDate->calendarDate;
  $coverage->single = $xml->singleDateTime->calendarDate;

  return $coverage;
}

function process_taxonomic_coverage($xml){

  $coverage = new TaxonomicCoverage();

  $coverage->taxon_rank_name = $xml->taxonomicClassification->taxonRankName;
  $coverage->ranxon_rank_value = $xml->taxonomicClassification->taxonRankValue;
  $coverage->common_name = $xml->taxonomicClassification->commonName;

  return $coverage;
}


function process_coverage($xml){

  $coverage = Array();

  $coverage[] = process_geographic_coverage( $xml->geographicCoverage);
  $coverage[] = process_temporal_coverage($xml->temporalCoverage);
  $coverage[] = process_taxonomic_coverage( $xml->taxonomicCoverage);

  return $coverage;
}

function process_method($xml){

  $method = new Method();

  $method->method_step = $xml->methodStep->description->para;
  $method->sampling = $xml->sampling->studyExtent->description->para;
  $method->quality_control = $xml->qualityControl->description->para;

  return $method;

}

function process_project($xml){

  $project = new Project();

  $project->title = $xml->project->title;
  $project->personnel = process_agent($xml->project->personnel);
  $project->funding = $xml->project->funding->para;
  $project->study_area_description = $xml->project->studyAreaDescription->descriptor->descriptorValue;
  $project->design_description = $xml->project->designDescription->description;

  return $project;
}

function process_dataset_element($xml){

  $dataset = new Dataset();

  $dataset-> alternateIdentifier = $xml->alternateIdentifier;

  $dataset->title  = $xml->title;
  $dataset->creator = process_agent($xml->creator);
  $dataset->metadataProvider = process_agent($xml->metadataProvider);
  $dataset->associatedParty = process_agent($xml->associatedParty);
  $dataset->pubDate = $xml->pubDate;
  $dataset->language = $xml->language;
  $dataset->abstract = $xml->abstract->para;
  $dataset->keywordSet = process_keyword_set($xml->keywordSet);
  $dataset->additionalInfo->para = $xml->additionalInfo->para;
  $dataset->intellectualRights =$xml->intellectualRights-> para;
  $dataset->distribution = $xml->distribution->online->url;
  $dataset->coverage = process_coverage($xml->coverage);
  $dataset->purpose = $xml->purpose->para;
  $dataset->contact = process_agent($xml->contact);
  $dataset->methods = process_method($xml->methods); 
  $dataset->project = process_project($xml->project);

  return $dataset;
}



?>
