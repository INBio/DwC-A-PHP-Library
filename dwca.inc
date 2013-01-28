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

    /*
    // Get the eml filename
    $attributes = $xml->attributes();
    $this->eml_filename = (string) $attributes['metadata'];

    // ------
    
    */

    $filename = dirname($this->zip_path)."/".$this->hash."/".$this->eml_filename;
    $status = TRUE;

    if(!file_exists($filename)){
      $status = $this->dwca_file->extractTo(
        dirname($this->zip_path)."/".$this->hash,
        $this->eml_filename);

      if ( ! $status ){
        print 'Error extracting '.$this->eml_filename;
      }
    }

    // Read the xml by using simplexml library.
    $xml = simplexml_load_file($filename);


    // Get the data Organization, data resource name and description.
    $meta_info = Array(
        'metadata' => TRUE,
        'data_provider_title'         => (string) $xml->dataset->creator->organizationName,
        'data_provider_description'  => '',
        'data_resource_title'        => (string) $xml->dataset->title,
        'data_resource_description' => (string) $xml->dataset->abstract->para
    );

    return $meta_info;
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
