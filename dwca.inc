<?php

include_once('includes/test_utils.inc');

include_once('includes/archive.inc');
include_once('includes/archive_file.inc');
include_once('includes/constants.inc');
include_once('includes/metadata.inc');
include_once('includes/parser.inc');


/**
 * Manage a DwC-A (Darwin Core Archive) to extract information from the zip file
 * and return it as arrays (hashes).
 */
class DwCAFile implements DwCA{

  private $zip_path;
  private $hash;

  private $core_filename;
  private $core_config;
  private $core_mappings;

  private $dwca_file;

  private $parser;

  private $meta_filename;
  private $eml_filename;


  /**
   * @param $term
   *   An array or a stdClass object that is a Drupal taxonomy term. Can include
   *   geo extensions.
   */
  public function __construct() {

    // Default settings
    $this->separator = DWCA_DEFAULT_FIELD_SEPARATOR;

    $this->meta_filename = DWCA_META_FILE;
    $this->eml_filename = DWCA_EML_FILE;

    $this->core_config = Array();
    $this->core_mappings = Array();

    $this->parser = new ParserCSV();

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
  public function open($path){

    $this->hash = hash_file('md5', $path);

    $dwca_file = new zipArchive();
    if (! $dwca_file->open($path)){
      exit( "Error: Failed to open zip file\n" );
    }

    if ( $dwca_file->numFiles < 3){
      exit( "Error: This is not a DwC-A formated file\n" );
    }

    if( ! $dwca_file->locateName($this->meta_filename)){
      exit( "Error: This is not a DwC-A formated file: $this->meta_filename mising\n" );
    }

    $this->dwca_file = $dwca_file;
    $this->zip_path = $path;

    // ------------------------------------------------------------------------------------

    $filename = dirname($this->zip_path)."/".$this->hash."/".$this->meta_filename;
    $status = TRUE;

    if(!file_exists($filename)){
      $status = $this->dwca_file->extractTo(
        dirname($this->zip_path)."/".$this->hash,
        $this->meta_filename);

      if ( ! $status ){
        print 'Error extracting '.$this->meta_filename;
      }
    }

    // Read the xml by using simplexml library.
    $xml = simplexml_load_file($filename);

    // Get the eml filename
    $attributes = $xml->attributes();
    $this->eml_filename = (string) $attributes['metadata'];
    // ------

    // Get the core filename and configuration
    $core_attributes = $xml->core->attributes();
    $this->core_config = Array(
        'encoding'         => (string) $core_attributes['encoding'],
        'delimiter'        => stripcslashes( (string) $core_attributes['fieldsTerminatedBy'] ),
        'line_delimiter'   => stripcslashes( (string) $core_attributes['linesTerminatedBy'] ),
        'fields_enclosed'  => stripcslashes( (string) $core_attributes['fieldsEnclosedBy'] ),
        'row_type'         => (string) $core_attributes['rowType']
     );

    $this->core_filename = (string) $xml->core->files->location;
    // ------

    // Get the core mappings and configuration
    $core_id_attributes = $xml->core->id->attributes();

    $this->core_mappings[ (string) $core_id_attributes['index'] ] ='id';

    foreach($xml->core->field as $field ){
      $field_attributes = $field->attributes();
      $index = (string) $field_attributes['index'];
      $term = (string) $field_attributes['term'];
      $this->core_mappings[$index] = $term;
    }
    // ------

    // Config the parser
    $this->parser->setDelimiter($this->core_config['delimiter']);
    $this->parser->setColumnNames(array_values($this->core_mappings));

    return TRUE;
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
   * Return the information of the core extension as well as his type
   * (Taxon or Occurrence)
   * @return
   *  An array with the information from the core file.
   *  array( type=> , array(column => value, column => value, ....));
   */
  public function get_core_info($start = 0, $limit = 10){

    $filename = dirname($this->zip_path)."/".$this->hash."/".$this->core_filename;
    $status = TRUE;

    if(!file_exists($filename)){
      $status = $this->dwca_file->extractTo(
        dirname($this->zip_path)."/".$this->hash,
        $this->core_filename);

      if ( ! $status ){
        print 'Error extracting '.$this->eml_filename;
      }
    }

    // parsing occurrence.txt file looking for occurrences
    $iterator = new ParserCSVIterator($filename);

    $rows = $this->_parseItems($this->parser, $iterator, $start, $limit);

    return $rows;
  }

  /**
   * Return the information of the specified extensión
   * @param $extension
   *  The name of an extension as listed by the method list_contents();
   *
   * @return
   *  An array with the information from the core file.
   *    array( type=> , array(column => value, column => value, ....));
   *
   */
  public function get_extension_info($extension){
    print "Error: not yet implemented";
    return NULL;
  }

    /**
   * Return the columns mapped by the core file by reading the meta.xml file
   * @return Array
   *   An array of the columns mapped in the file
   */
  public function get_core_mappings(){
    return $this->core_mappings;
  }

  /**
   * Return path of the core file
   * @return String
   *   the path of the file in the filesystem
   */
  public function get_core_path(){
    return dirname($this->zip_path)."/".$this->hash."/".$this->core_filename;
  }

  /**
   * Return the parser
   * @return ParserCSV
   *   The instance of the Parser being used to parse the file.
   */
  public function get_parser(){
    return $this->parser;
  }

  /**
   * Parse all of the items from the CSV.
   *
   * @param ParserCSV $parser
   * @param ParserCSVIterator $iterator
   * @return
   *   An array of rows of the CSV keyed by the column names previously set
   */
  private function _parseItems(ParserCSV $parser, ParserCSVIterator $iterator, $start = 0, $limit = 0) {
    $parser->setLineLimit($limit);
    $parser->setStartByte($start);
    $rows = $parser->parse($iterator);
    return $rows;
  }
}

function test(){

    $this->core_mappings[ (string) $core_id_attributes['index'] ] ='id';
  $file = new DwCAFile();

  $file->open('dwca-benin.zip');

  var_dump ($file->get_metadata());
  print "\n\n\n\n";
  var_dump ($file->get_core_info());
}

//test();
