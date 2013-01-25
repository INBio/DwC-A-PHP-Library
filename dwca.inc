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

  /**
   * Initialize the class, set defaults and check if the file exist
   * @param $path
   *  A string with the path of the file to access in the file system.
   *
   * @return
   *  0 if the file exist and in the right format.
   *  1 if error.
   */
  public function open(){

    // Read the xml by using simplexml library.
    $xml = simplexml_load_file($this->dir."/".Constants::META_FILENAME);

    //load the info from the CORE file
    $core_file = new ArchiveFile();

    $core_attributes = $xml->core->attributes();

    $core_file->set_fields_terminated_by(stripcslashes((string) $core_attributes['fieldsTerminatedBy'] ));
    $core_file->set_fields_enclosed_by(  stripcslashes((string) $core_attributes['fieldsEnclosedBy'] ));
    $core_file->set_lines_terminated_by( stripcslashes((string) $core_attributes['linesTerminatedBy'] ));
    $core_file->set_encoding(            (string) $core_attributes['encoding']);
    $core_file->set_row_type(            (string) $core_attributes['rowType']);
    $core_file->set_ignoreheader_lines(  (string) $core_attributes['ignoreHeader_lines']);
    $core_file->set_location(            (string) $xml->core->files->location);
    $core_file->set_date_format(         'DD/MM/YYYY');

    // Get the core mappings and configuration
    $core_id_attributes = $xml->core->id->attributes();
    $fields[ (string) $core_id_attributes['index'] ] ='id';
    $defaults = Array();

    foreach($xml->core->field as $field ){
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

    $core_file->set_fields($fields);
    $core_file->set_defaults($defaults);
    $this->core = $core_file;

    //load the info from the EXTENSIONS file
    






    /*
    // Get the eml filename
    $attributes = $xml->attributes();
    $this->eml_filename = (string) $attributes['metadata'];

    // ------
    
    */
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
    // ------

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


}
