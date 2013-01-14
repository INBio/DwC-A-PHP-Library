<?php

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
  public function __construct($path) {

    // library includes
    feeds_include_library('pclzip.lib.php', 'pclzip');   // For open Zip files
    feeds_include_library('ParserCSV.inc', 'feeds'); // For parse CSV files

    $this->parser = new ParserCSV();

    // Default settings
    $this->separator = DWCA_DEFAULT_FIELD_SEPARATOR;
    $this->meta_filename = DWCA_META_FILE;
    $this->eml_filename = DWCA_EML_FILE;

    $this->core_config = Array();
    $this->core_mappings = Array();
    $this->zip_path = $path;
    $this->hash = hash_file('md5', $path);

    $this->create($path);
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
  public function create($path){

    //TODO: validate the right format (a.k.a. create the DwC-A library)
    $this->dwca_file = new PclZip($path);

    // Verify that the file is open
    if(!$this->dwca_file){
      print "Error: Failed to open zip file";
    }

    $dwca_file_list = self::list_contents();

    if ( $dwca_file_list < 3){
      print "Error: This is not a DwC-A formated file";
      return FALSE;
    }

    $meta = FALSE;

    foreach ($dwca_file_list as $file){
      if($file == $this->meta_filename){
        $meta = TRUE;
      }
    }

    if (!$meta ){
      print 'Error: This is not a DwC-A formatted file';
      return FALSE;
    }

    $this->_config();


    return TRUE;
  }

  /**
   * List the contents of the zip file.
   * @return
   *  An array (hash) with the name of the file and the category
   *  array(
   *    array( type=> , name=> ),
   *    array( type=> , name=> ),
   *    ...);
   */
  public function list_contents(){

    $zipfile_list = $this->dwca_file->listContent();

    $files = Array();

    foreach ($zipfile_list as $file ){
      $files[] = $file['filename'];
    }
    return $files;
  }

  /**
   * Return an array with a subset of the metadata contained in the eml.xml file
   * @return
   *  an array with the metadata in the format: array( provider= , project=, ...);
   */
  public function get_metadata(){


    $filename = dirname($this->zip_path)."/".$this->hash."/".$this->eml_filename;

    if(!file_exists($filename)){
      $file = $this->dwca_file->extract(
          PCLZIP_OPT_PATH, dirname($this->zip_path)."/".$this->hash,
          PCLZIP_OPT_BY_NAME, $this->eml_filename);

      if ($file[0]['status'] != 'ok'){
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

    if(!file_exists($filename)){
      $file = $this->dwca_file->extract(
          PCLZIP_OPT_PATH, dirname($this->zip_path)."/".$this->hash,
          PCLZIP_OPT_BY_NAME, $this->core_filename);

      if ($file[0]['status'] != 'ok'){
        print 'Error extracting '.$this->core_filename;
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

  private function _config(){

    $filename = dirname($this->zip_path)."/".$this->hash."/".$this->meta_filename;

    if(!file_exists($filename)){
      $file = $this->dwca_file->extract(
          PCLZIP_OPT_PATH, dirname($this->zip_path)."/".$this->hash,
          PCLZIP_OPT_BY_NAME, $this->meta_filename);

      if ($file[0]['status'] != 'ok'){
        print 'Error extracting '.$this->meta_filename;
      }
    }

    // Read the xml by using simplexml library.
    $xml = simplexml_load_file($filename);

    // Get the eml filename
    $this->eml_filename = (string) $xml->attributes()['metadata'];
    // ------

    // Get the core filename and configuration
    $this->core_config = Array(
        'encoding'         => (string) $xml->core->attributes()['encoding'],
        'delimiter'        => stripcslashes( (string) $xml->core->attributes()['fieldsTerminatedBy'] ),
        'line_delimiter'   => stripcslashes( (string) $xml->core->attributes()['linesTerminatedBy'] ),
        'fields_enclosed'  => stripcslashes( (string) $xml->core->attributes()['fieldsEnclosedBy'] ),
        'row_type'         => (string) $xml->core->attributes()['rowType']
     );

    $this->core_filename = (string) $xml->core->files->location;
    // ------

    // Get the core mappings and configuration

    $this->core_mappings[ (string) $xml->core->id->attributes()['index'] ] ='id';

    foreach($xml->core->field as $field ){
      $index = (string) $field->attributes()['index'];
      $term = (string) $field->attributes()['term'];
      $this->core_mappings[$index] = $term;
    }
    // ------

    // Config the parser
    $this->parser->setDelimiter($this->core_config['delimiter']);
    $this->parser->setColumnNames(array_values($this->core_mappings));
  }
}
