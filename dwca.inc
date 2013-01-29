<?php

include_once('includes/archive.inc');
include_once('includes/archive_file.inc');
include_once('includes/constants.inc');
include_once('includes/metadata.inc');
include_once('includes/parser.inc');


/**
 * Manage a DwC-A (Darwin Core Archive) to extract information from the zip file
 * and return it as arrays (hashes).
 */
class DwCA{

  private $dir;
  private $archive;
  private $core;
  private $extensions;
  private $metadata;

  /**
   * @param $term
   *   An array or a stdClass object that is a Drupal taxonomy term. Can include
   *   geo extensions.
   */
  public function __construct($path) {

    // DwC-A format validation
    $zip = new zipArchive();

    if (! $zip->open($path)){
      throw new Exception("Failed to open zip file\n");
    }

    if ( $zip->numFiles < 3){
      throw new Exception("This is not a DwC-A formated file\n");
    }

    if( ! $zip->locateName(Constants::META_FILENAME)){
      throw new Exception("This is not a recognized DwC-A formated file: ".Constants::META_FILENAME." mising\n");
    }

    // if everything works, the extraction proceed
    $status = TRUE;
    $hash   = hash_file('md5', $path);
    $dir    = dirname($path)."/".$hash;

    if(!file_exists($dir)){

      $status = $zip->extractTo($dir);

      if ( ! $status ){
        print "Error extracting $path to $dir";
        throw new Exception();
      }
    }
    
    $this->dir = $dir;
  }

  public function open(){

    // Read the xml by using simplexml library.
    $xml = simplexml_load_file($this->dir."/".Constants::META_FILENAME);

    //load the info from the CORE file
    $file = $this->generate_archive_file_from_xml($xml->core);
    $this->core = $file;

    //load the info from the EXTENSIONS file
    foreach($xml->extension as $extension){
      $file = $this->generate_archive_file_from_xml($extension);
      $this->extensions[] = $file;
    }

    // set the name of the metadata file
    $attributes = $xml->attributes();
    $eml = new EMLDocument();
    $eml->filename = (string) $attributes['metadata'];

    $this->metadata = $eml;

  }

  public function get_records(ArchiveFile $file, $start = 0, $limit = 50){

    $status = TRUE;
    $filename = $this->dir."/".$file->get_location();

    if(!file_exists($filename)){
      throw new Exception("$filename: not found. Problably due to an error while unziping the dwca file");
    }

    // Create and config the parser
    $parser = new ParserCSV();
    $parser->setDelimiter($file->get_fields_terminated_by());
    $parser->setColumnNames(array_values($file->get_fields()));

    // parsing occurrence.txt file looking for occurrences
    $iterator = new ParserCSVIterator($filename);
    $rows = $this->_parseItems($parser, $iterator, $start, $limit, $file->get_defaults());

    return $rows;
  }

  /**
   * Return an array with a subset of the metadata contained in the eml.xml file
   * @return
   *  an array with the metadata in the format: array( provider= , project=, ...);
   */
  public function get_metadata(){

    $full_filename = $this->dir."/".$this->metadata->filename;

    if(!file_exists($full_filename)){
      throw new Exception("$full_filename: not found. Missing metadata file");
    }

    // Read the xml by using simplexml library.
    $xml = simplexml_load_file($full_filename);

    // Verify that is a GBIF EML Profile
    if( !$xml->getName() == 'eml' || !$xml->getNamespaces()['eml'] == 'eml' || !$xml->attributes()['system'] == 'http://gbif.org'){
      throw new Exception("Metadata file format not recognized");
    }

    $xml->dataset->

  $xml->dataset
    $xml->dataset->alternateIdentifier
    $xml->dataset->title xml:lang="en"
    $xml->dataset->creator
      $xml->dataset->creator->individualName
        $xml->dataset->creator->individualName->givenName
        $xml->dataset->creator->individualName->surName
      $xml->dataset->creator->organizationName
      $xml->dataset->creator->positionName
      $xml->dataset->creator->address
        $xml->dataset->creator->address->deliveryPoint
        $xml->dataset->creator->address->city
        $xml->dataset->creator->address->administrativeArea
        $xml->dataset->creator->address->postalCode
        $xml->dataset->creator->address->country
      $xml->dataset->creator->phone
      $xml->dataset->creator->electronicMailAddress
      $xml->dataset->creator->onlineUrl
    $xml->dataset->metadataProvider
      $xml->dataset->metadataProvider->individualName
        $xml->dataset->metadataProvider->individualName->givenName
        $xml->dataset->metadataProvider->individualName->surName
      $xml->dataset->metadataProvider->address
        $xml->dataset->metadataProvider->address->deliveryPoint
        $xml->dataset->metadataProvider->addresscity
        $xml->dataset->metadataProvider->address->administrativeArea
        $xml->dataset->metadataProvider->address->postalCode
        $xml->dataset->metadataProvider->address->country
      $xml->dataset->metadataProvider->phone
      $xml->dataset->metadataProvider->electronicMailAddress
      $xml->dataset->metadataProvider->onlineUrl
    $xml->dataset->associatedParty
      $xml->dataset->associatedParty->individualName
        $xml->dataset->associatedParty->individualName->surName
      $xml->dataset->associatedParty->phone
      $xml->dataset->associatedParty->role
    $xml->dataset->pubDate
    $xml->dataset->language
    $xml->dataset->abstract->para
    $xml->dataset->keywordSet
      $xml->dataset->keywordSet->keyword
      $xml->dataset->keywordSet->keywordThesaurus
    $xml->dataset->additionalInfo->para
    $xml->dataset->intellectualRights-> para
    $xml->dataset->distribution->online->url
    $xml->dataset->coverage
      $xml->dataset->coverage->geographicCoverage
        $xml->dataset->coverage->geographicCoverage->geographicDescription
        $xml->dataset->coverage->geographicCoverage->boundingCoordinates
          $xml->dataset->coverage->geographicCoverage->boundingCoordinate->swestBoundingCoordinate
          $xml->dataset->coverage->geographicCoverage->boundingCoordinate->eastBoundingCoordinate
          $xml->dataset->coverage->geographicCoverage->boundingCoordinate->northBoundingCoordinate
          $xml->dataset->coverage->geographicCoverage->boundingCoordinate->southBoundingCoordinate
      $xml->dataset->coverage->temporalCoverage
        $xml->dataset->coverage->temporalCoverage->rangeOfDates
          $xml->dataset->coverage->temporalCoverage->rangeOfDates->beginDate->calendarDate
          $xml->dataset->coverage->temporalCoverage->rangeOfDates->endDate->calendarDate
          
      $xml->dataset->coverage->temporalCoverage
        $xml->dataset->coverage->temporalCoverage->singleDateTime->calendarDate
      $xml->dataset->coverage->taxonomicCoverage
        $xml->dataset->coverage->taxonomicCoverage->generalTaxonomicCoverage
        $xml->dataset->coverage->taxonomicCoverage->taxonomicClassification
          $xml->dataset->coverage->taxonomicCoverage->taxonomicClassification->taxonRankName
          $xml->dataset->coverage->taxonomicCoverage->taxonomicClassification->taxonRankValue
          $xml->dataset->coverage->taxonomicCoverage->taxonomicClassification->commonName
    $xml->dataset->purpose->para
    $xml->dataset->contact
      $xml->dataset->contact->individualName
        $xml->dataset->contact->individualName->givenName
        $xml->dataset->contact->individualName->surName
      $xml->dataset->contact->organizationName
      $xml->dataset->contact->positionName
      $xml->dataset->contact->address
        $xml->dataset->contact->address->deliveryPoint
        $xml->dataset->contact->address->city
        $xml->dataset->contact->address->administrativeArea
        $xml->dataset->contact->address->postalCode
        $xml->dataset->contact->address->country
      $xml->dataset->contact->phone
      $xml->dataset->contact->electronicMailAddress
      $xml->dataset->contact->onlineUrl
    $xml->dataset->methods
      $xml->dataset->methods->methodStep
        $xml->dataset->methods->methodStep->description->para
      $xml->dataset->methods->sampling
        $xml->dataset->methods->sampling->studyExtent
          $xml->dataset->methods->sampling->studyExtent->description->para
        $xml->dataset->methods->sampling->samplingDescription->para
      $xml->dataset->methods->qualityControl
        $xml->dataset->methods->qualityControl->description->para
    $xml->dataset->project
      $xml->dataset->project->title
      $xml->dataset->project->personnel
        $xml->dataset->project->personnel->individualName
          $xml->dataset->project->personnel->individualName->surName
        $xml->dataset->project->personnel->role
      $xml->dataset->project->funding->para
      $xml->dataset->project->studyAreaDescription
        $xml->dataset->project->studyAreaDescription->descriptor
          $xml->dataset->project->studyAreaDescription->descriptor->descriptorValue
      $xml->dataset->project->designDescription
        $xml->dataset->project->designDescription->description
      $xml->additionalMetadata
        $xml->additionalMetadata->metadata
          $xml->additionalMetadata->metadata->gbif
            $xml->additionalMetadata->metadata->gbif->dateStamp
            $xml->additionalMetadata->metadata->gbif->hierarchyLevel
            $xml->additionalMetadata->metadata->gbif->citation identifier="doi:tims-ident.2135.ex43.33.d"
            $xml->additionalMetadata->metadata->gbif->bibliography
              $xml->additionalMetadata->metadata->gbif->bibliography->citation
            $xml->additionalMetadata->metadata->gbif->physical
              $xml->additionalMetadata->metadata->gbif->physical->objectName
              $xml->additionalMetadata->metadata->gbif->physical->characterEncoding
              $xml->additionalMetadata->metadata->gbif->physical->dataFormat
                $xml->additionalMetadata->metadata->gbif->physical->dataFormat->externallyDefinedFormat
                  $xml->additionalMetadata->metadata->gbif->physical->dataFormat->externallyDefinedFormat->formatName
                  $xml->additionalMetadata->metadata->gbif->physical->dataFormat->externallyDefinedFormat->formatVersion
              $xml->additionalMetadata->metadata->gbif->physical->distribution
                $xml->additionalMetadata->metadata->gbif->physical->distribution->online->url
            $xml->additionalMetadata->metadata->gbif->resourceLogoUrl
            $xml->additionalMetadata->metadata->gbif->collection
              $xml->additionalMetadata->metadata->gbif->collection->parentCollectionIdentifier
              $xml->additionalMetadata->metadata->gbif->collection->collectionIdentifier
              $xml->additionalMetadata->metadata->gbif->collection->collectionName
            $xml->additionalMetadata->metadata->gbif->formationPeriod
            $xml->additionalMetadata->metadata->gbif->specimenPreservationMethod
            $xml->additionalMetadata->metadata->gbif->livingTimePeriod

    return $this->metadata;
  }

  /**
   * Parse all of the items from the CSV.
   *
   * @param ParserCSV $parser
   * @param ParserCSVIterator $iterator
   * @return
   *   An array of rows of the CSV keyed by the column names previously set
   */
  private function _parseItems(ParserCSV $parser, ParserCSVIterator $iterator, $start = 0, $limit = 0, $defaults = Array()) {
    $parser->setLineLimit($limit);
    $parser->setStartByte($start);
    $rows = $parser->parse($iterator, $defaults);
    return $rows;
  }


  function get_dir(){
    return dir;
  }
  function set_dir($new_dir){
    $this->dir = $new_dir;
  }

  function get_archive(){
    return $this->archive;
  }
  function set_archive($new_archive){
    $this->archive = $new_archive;
  }

  function get_core(){
    return $this->core;
  }
  function set_core($new_core){
    $this->core = $new_core;
  }

  function get_extensions(){
    return $this->extensions;
  }
  function set_extensions($new_extensions){
    $this->extensions = $new_extensions;
  }

  private function generate_archive_file_from_xml($xml_archive_file){

    $attributes = $xml_archive_file->attributes();
    $file = new ArchiveFile();

    $file->set_fields_terminated_by(stripcslashes((string) $attributes['fieldsTerminatedBy'] ));
    $file->set_fields_enclosed_by(  stripcslashes((string) $attributes['fieldsEnclosedBy'] ));
    $file->set_lines_terminated_by( stripcslashes((string) $attributes['linesTerminatedBy'] ));
    $file->set_encoding(            (string) $attributes['encoding']);
    $file->set_row_type(            (string) $attributes['rowType']);
    $file->set_ignoreheader_lines(  (string) $attributes['ignoreHeader_lines']);
    $file->set_location(            (string) $xml_archive_file->files->location);
    $file->set_date_format(         'DD/MM/YYYY');


    $id_attributes = FALSE; 

    // Get the core mappings and configuration
    if(isset($xml_archive_file->id)){
      $id_attributes = $xml_archive_file->id->attributes();
    }else{
      // The standard uses diferent element names for the id's if present
      // in the core file or in the extensions files
      $id_attributes = $xml_archive_file->coreid->attributes();
    }

    $fields[ (string) $id_attributes['index'] ] ='id';
    $defaults = Array();

    foreach($xml_archive_file->field as $field ){
      $field_attributes = $field->attributes();
      $index = (string) $field_attributes['index'];
      $term = (string) $field_attributes['term'];

      if( empty($index) ){
        $value = (string) $field_attributes['default'];
        $defaults[$term] = $value;
      }else{
        $fields[$index] = $term;
      }
    }

    $file->set_fields($fields);
    $file->set_defaults($defaults);

    return $file;

  }

}
