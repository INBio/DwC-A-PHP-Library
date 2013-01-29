<?php

include_once('includes/archive.inc');
include_once('includes/archive_file.inc');
include_once('includes/constants.inc');
include_once('includes/metadata.inc');
include_once('includes/eml_utils.inc');
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
    // Process the <dataset> element of the GBIF EML Profile
    $this->metadata->dataset = process_dataset_element($xml->dataset);

    // Process the <additionalMetadata> element of the GBIF EML Profile
    //$this->metadata->aditional_metadata = process_additional_metadata_element($xml->additionalMetadata);

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

  private function process_address($xml){

    $address = new Address();

    $address->deliveryPoint = $xml->deliveryPoint;
    $address->city = $xml->city;
    $address->administrativeArea = $xml->administrativeArea;
    $address->postalCode = $xml->postalCode;
    $address->contry = $xml->country;

    return $address;
  }

  private function process_agent($xml){

    $agent = new Agent()

    $agent->given_name = $xml->individualName->givenName
    $agent->last_name = $xml->individualName->surName
    $agent->organisation_name = $xml->organizationName
    $agent->position_name = $xml->positionName
    $agent->address = process_address( $xml->address);
    $agent->phone = $xml->phone
    $agent->electronic_mail_address = $xml->electronicMailAddress
    $agent->online_url = $xml->onlineUrl
    $agent->role = $xml->role

    return $agent;
  }

  private process_keyword_set($xml){

    $keyword_set = new  KeywordSet();

    $keyword_set->keyword = $xml->keyword;
    $keyword_set->thesaurus =  $xml->keywordThesaurus;

    return $keyword_set;
  }

  private function process_geographic_coverage($xml){

    $coverage = new GeographicCoverage();

    $coverage->geographic_description = $xml->geographicCoverage->geographicDescription;
    $coverage->bounding_box = Array (
          $xml->boundingCoordinate->swestBoundingCoordinate,
          $xml->boundingCoordinate->eastBoundingCoordinate,
          $xml->boundingCoordinate->northBoundingCoordinate,
          $xml->boundingCoordinate->southBoundingCoordinate);

    return $coverage;
  }

  private function process_temporal_coverage($xml){

    $coverage = new TemporalCoverage();

    $coverage->begin = $xml->rangeOfDates->beginDate->calendarDate;
    $coverage->end = $xml->rangeOfDates->endDate->calendarDate;
    $coverage->single = $xml->singleDateTime->calendarDate;

    return $coverage;
  }

  private function process_taxonomic_coverage($xml){

      $coverage = new TaxonomicCoverage();

      $coverage->taxon_rank_name = $xml->taxonomicClassification->taxonRankName;
      $coverage->ranxon_rank_value = $xml->taxonomicClassification->taxonRankValue;
      $coverage->common_name = $xml->taxonomicClassification->commonName;

      return $coverage;
  }


  private function process_coverage($xml){

    $coverage = Array();

    $coverage[] = process_geographic_coverage( $xml->geographicCoverage);
    $coverage[] = process_temporal_coverage($xml->temporalCoverage);
    $coverage[] = process_taxonomic_coverage( $xml->taxonomicCoverage);

    return $coverage;
  }

  private function process_method($xml){

    $method = new Method()

    $method->method_step = $xml->methodStep->description->para;
    $method->sampling = $xml->sampling->studyExtent->description->para;
    $method->quality_control = $xml->qualityControl->description->para;

    return $method;

  }

  private function process_project($xml){

    $project = new Project();

    $project->title = $xml->project->title;
    $project->personnel = process_agent($xml->project->personnel);
    $project->funding = $xml->project->funding->para;
    $project->study_area_description = $xml->project->studyAreaDescription->descriptor->descriptorValue;
    $project->design_description = $xml->project->designDescription->description;

    return $project;
  }

  private function process_dataset_element($xml){

    $dataset = new Dataset();
  
    $dataset-> alternateIdentifier = $xml->alternateIdentifier;
    $dataset->title xml:lang="en" = $xml->title xml:lang="en";
    $dataset->creator = process_agent($xml->creator);
    $dataset->metadataProvider = process_agent($xml->metadataProvider);
    $dataset->associatedParty = process_agent($xml->associatedParty);
    $dataset->pubDate = $xml->pubDate;
    $dataset->language = $xml->language;
    $dataset->abstract = $xml->abstract->para;
    $dataset->keywordSet = process_keyword_set($xml->keywordSet);
    $dataset->additionalInfo->para = $xml->additionalInfo->para
    $dataset->intellectualRights =$xml->intellectualRights-> para;
    $dataset->distribution = $xml->distribution->online->url;
    $dataset->coverage = process_coverage($xml->coverage);
    $dataset->purpose = $xml->purpose->para;
    $dataset->contact = process_agent($xml->contact);
    $dataset->methods = process_methods($xml->methods); 
    $dataset->project = process_project($xml->project);
  }

  /*
  private function process_additional_metadata_element(){

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
  }
*/
}
