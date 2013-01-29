<?php
function process_address($xml){

  $address = new Address();

  $address->delivery_point = (string)$xml->deliveryPoint;
  $address->city = (String) $xml->city;
  $address->administrative_area = (String) $xml->administrativeArea;
  $address->postal_code = (String) $xml->postalCode;
  $address->contry = (String) $xml->country;

  return $address;
}

function process_agent($xml){

  $agent = new Agent();

  $agent->given_name = (String) $xml->individualName->givenName;
  $agent->last_name = (String) $xml->individualName->surName;
  $agent->organisation_name = (String) $xml->organizationName;
  $agent->position_name = (String) $xml->positionName;
  $agent->address = process_address( $xml->address);
  $agent->phone = (String) $xml->phone;
  $agent->electronic_mail_address = (String) $xml->electronicMailAddress;
  $agent->online_url = (String) $xml->onlineUrl;
  $agent->role = (String) $xml->role;

  return $agent;
}

function process_keyword_set($xml){

  $keyword_set = new  KeywordSet();

  $keyword_set->keyword = (String) $xml->keyword;
  $keyword_set->thesaurus =  (String) $xml->keywordThesaurus;

  return $keyword_set;
}

function process_geographic_coverage($xml){

  $coverage = new GeographicCoverage();

  $coverage->geographic_description = (String) $xml->geographicCoverage->geographicDescription;
  $coverage->bounding_box = Array (
    (String) $xml->boundingCoordinate->swestBoundingCoordinate,
    (String) $xml->boundingCoordinate->eastBoundingCoordinate,
    (String) $xml->boundingCoordinate->northBoundingCoordinate,
    (String) $xml->boundingCoordinate->southBoundingCoordinate);

  return $coverage;
}

function process_temporal_coverage($xml){

  $coverage = new TemporalCoverage();

  $coverage->begin = (String) $xml->rangeOfDates->beginDate->calendarDate;
  $coverage->end = (String) $xml->rangeOfDates->endDate->calendarDate;
  $coverage->single = (String) $xml->singleDateTime->calendarDate;

  return $coverage;
}

function process_taxonomic_coverage($xml){

  $coverage = new TaxonomicCoverage();

  $coverage->taxon_rank_name = (String) $xml->taxonomicClassification->taxonRankName;
  $coverage->ranxon_rank_value = (String) $xml->taxonomicClassification->taxonRankValue;
  $coverage->common_name = (String) $xml->taxonomicClassification->commonName;

  return $coverage;
}


function process_coverage($xml){

  $coverage = Array();

  $coverage[] = process_geographic_coverage( $xml->geographicCoverage);
  $coverage[] = process_temporal_coverage( $xml->temporalCoverage);
  $coverage[] = process_taxonomic_coverage( $xml->taxonomicCoverage);

  return $coverage;
}

function process_method($xml){

  $method = new Method();

  $method->method_step = (String) $xml->methodStep->description->para;
  $method->sampling = (String) $xml->sampling->studyExtent->description->para;
  $method->quality_control = (String) $xml->qualityControl->description->para;

  return $method;

}

function process_project($xml){

  $project = new Project();

  $project->title = (String) $xml->title;
  $project->personnel = process_agent($xml->personnel);
  $project->funding = (String) $xml->funding->para;
  $project->study_area_description = (String) $xml->studyAreaDescription->descriptor->descriptorValue;
  $project->design_description = (String) $xml->designDescription->description;

  return $project;
}

function process_dataset_element($xml){

  $dataset = new Dataset();

  $dataset->alternate_identifier = $xml->alternateIdentifier;

  $dataset->title  = (String) $xml->title;
  $dataset->creator = process_agent($xml->creator);
  $dataset->metadata_provider = process_agent($xml->metadataProvider);
  $dataset->associated_party = process_agent($xml->associatedParty);
  $dataset->pub_Date = (String) $xml->pubDate;
  $dataset->language = (String) $xml->language;
  $dataset->abstract = (String) $xml->abstract->para;
  $dataset->keyword_set = process_keyword_set($xml->keywordSet);
  $dataset->additional_info = (String) $xml->additionalInfo->para;
  $dataset->intellectual_rights =(String) $xml->intellectualRights-> para;
  $dataset->distribution = (String) $xml->distribution->online->url;
  $dataset->coverage = process_coverage($xml->coverage);
  $dataset->purpose = (String) $xml->purpose->para;
  $dataset->contact = process_agent($xml->contact);
  $dataset->methods = process_method($xml->methods); 
  $dataset->project = process_project($xml->project);

  return $dataset;
}

function process_bibliography($xml){

  $bibliography = Array();
  $bibliography[] = (String) $xml->bibliography->citation;

  return $bibliography;
}

function process_physical($xml){

  $physical = new Physical();

  $physical->object_name = (String) $xml->objectName;
  $physical->character_encoding = (String) $xml->characterEncoding;
  $physical->format = process_data_format($xml->dataFormat);
  $physical->distribution_url = (String) $xml->distribution->online->url;

  return $physical;
}

function process_data_format($xml){

  $data_format = new DataFormat();

  $data_format->format_name = (String) $xml->externallyDefinedFormat->formatName;
  $data_format->format_value = (String) $xml->externallyDefinedFormat->formatVersion;

  return $data_format;
}

function process_collection($xml){

  $collection = new Collection();

  $collection->parent_collection_identifier = (String) $xml->parentCollectionIdentifier;
  $collection->collection_identifier = (String) $xml->collectionIdentifier;
  $collection->collection_name = (String) $xml->collectionName;

  return $collection;
}

function process_additional_metadata_element($xml){

  $additional_metadata = new AdditionalMetadata();
  $gbif = $xml->metadata->gbif;

  $additional_metadata->date_stamp = (String) $gbif->dateStamp;
  $additional_metadata->hierarchy_level = (String)  $gbif->hierarchyLevel;
  $additional_metadata->citation = (String) $gbif->citation;
  $additional_metadata->bibliography = process_bibliography( $gbif->bibliography);
  $additional_metadata->physical = process_physical($gbif->physical);
  $additional_metadata->resource_logo_url = (String)  $gbif->resourceLogoUrl;
  $additional_metadata->collection = process_collection($gbif->collection);
  $additional_metadata->formation_period = (String) $gbif->formationPeriod;
  $additional_metadata->specimen_preservation_method = (String) $gbif->specimenPreservationMethod;
  $additional_metadata->living_time_period = (String) $gbif->livingTimePeriod;

  return $additional_metadata;
}

?>
