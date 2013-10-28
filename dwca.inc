<?php

/**
  
  DwCLibrary was build to open and query dwc files used by the GBIF Network

  by Aurelio Sanabria @ INBio (Costa Rica).
  
  Was based on the following resources:

      GBIF (2011). Darwin Core Archive Format, Reference Guide to the
        XML Descriptor File, April 2011, (contributed by Döring, M., Robertson, T., Remsen, D.),
        Copenhagen: Global Biodiversity Information Facility,16 pp.

      GBIF (2010). Darwin Core Archives – How-to Guide, version 1, released on 1 March 2011,
        (contributed by Remsen D, Braak, K, Döring M, Robertson, T), Copenhagen: Global
        Biodiversity Information Facility, 21 pp.

      GBIF (2010). Darwin Core Quick Reference Guide, version 1.3, released on 10 Mar 2012,
        (contributed by Wieczorek , J., De Giovanni , R., Vieglais , D. Remsen D.P., Döring, M,
        Robertson, T.), Copenhagen: Global Biodiversity Information Facility, 41 pp,.

      GBIF (2011). GBIF Metadata Profile – How-to Guide, (contributed by Ó
        Tuama, Eamonn, Braak, K. Remsen, D.), Copenhagen: Global Biodiversity Information
        Facility, 11 pp

      GBIF (2011). GBIF Metadata Profile, Reference Guide, Feb 2011,
        (contributed by O Tuama, E., Braak, K., Copenhagen: Global Biodiversity Information
        Facility,19 pp.

      And the Darwin Core Text Guide accessible online at:
        http://rs.tdwg.org/dwc/terms/guides/text/index.htm
 */


// DWC realted
// meta.xml related
include_once('includes/meta.inc');

// GBIF Metadata Profile objects
include_once('includes/gmp.inc');

// Utils
include_once('includes/eml_utils.inc');
include_once('includes/constants.inc');
include_once('includes/parser.inc');
include_once('includes/dwca_iterator.inc');


/**
 * Manage a DwC-A (Darwin Core Archive) to extract information from the zip file
 * and return it as arrays (hashes).
 */
class DwCA{


  private $case = -1;

  private $meta;
  private $gbif_eml;

  private $dir;
  private $zip;

  /**
   * Open the core file with the headers in case the dwca file is just a 
   * single csv file with headers.
   */
  private function __open_core_with_headers(){

    $core = new Core();

    $index = 0;

    if( "xml" === substr($this->zip->getNameIndex(0), -3) ){
      $index = 1;
    }

    // get the only existing file
    $file_in_zip = $this->zip->getNameIndex($index);

    // complete the full path of the file
    $filepath = $this->dir."/".$file_in_zip;

    $core->files = Array($filepath);


    $file = fopen($filepath, "r");

    // get the first line as headers
    $core->fields = fgetcsv( $file, 0, $core->fields_terminated_by,
      $core->fields_enclosed_by);


    $core->ignore_header_lines = 1; # The line of headers

    // close the file \o/ 
    fclose($file);

    return $core;
  }

  /**
   * Return the records from the core file only whitout the extensions
   */
  public function get_records($start = 0, $limit = 50){

    //always choose the first file of the list of locations.
    $file = $this->meta->core;
    $filename = $file->files[0];

    if(!file_exists($filename)){
      throw new Exception("$filename: not found. Problably due to an error while unziping the dwca file");
    }

    // Create and config the parser
    $parser = new ParserCSV();
    $parser->setDelimiter($file->fields_terminated_by);
    $parser->setColumnNames(array_values($file->fields));

    // parsing occurrence.txt file looking for occurrences
    $iterator = new ParserCSVIterator($filename);
    $rows = $this->_parseItems($parser, $iterator, $start, $limit, array());

    return $rows;
  }

  /**
   * Return the records from the core file only
   */
  public function get_full_records($iterator, $chunk_size = 50){

    //always choose the first file of the list of locations.
    $file = $this->meta->core;
    $filename = $file->files[0];

    if(!file_exists($filename)){
      throw new Exception("$filename: not found. Problably due to an error while unziping the dwca file");
    }

    $rows = Array();

    $iterator->next($chunk_size);
    $rows = $iterator->current();

    return $rows;
  }

  /**
   * TODO: Refactoring  
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
    $this->metadata->additional_metadata = process_additional_metadata_element($xml->additionalMetadata);

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

  private function __from_xml_to_core_file($xml_core_element){

    $attributes = $xml_core_element->attributes();
    $file = new Core();

    //process the attributes
    $file->fields_terminated_by = stripcslashes((string) $attributes['fieldsTerminatedBy'] );
    $file->fields_enclosed_by   = stripcslashes((string) $attributes['fieldsEnclosedBy'] );
    $file->lines_terminated_by  = stripcslashes((string) $attributes['linesTerminatedBy'] );
    $file->encoding             = (string) $attributes['encoding'];
    $file->row_type             = (string) $attributes['rowType'];
    $file->ignore_header_lines   = (string) $attributes['ignoreHeaderLines'];
    $file->date_format          = 'DD/MM/YYYY';


    // locations =========================================
    $locations = $xml_core_element->files->location;
    $files = array();

    // TODO: Falta agregar el fonder en el que esta el archivo.
    foreach ( $locations as $location){

      $index =  $this->zip->locateName($location, ZipArchive::FL_NOCASE|ZipArchive::FL_NODIR);
      $real_location = $this->zip->getNameIndex($index);

      $files[] = $this->dir.'/'.(String)$real_location;
    }
     
    $file->files = $files;
    // ===================================================

    $fields = array();
    // TODO: use this defaults
    $defaults = array();

    // id ================================================
    // Get the core mappings and configuration
    if(isset($xml_core_element->id)){
      $id_attributes = $xml_core_element->id->attributes();
      $fields[ (string) $id_attributes['index'] ] ='id';
    }
    // ===================================================

    // fields ======((====================================

    foreach($xml_core_element->field as $field ){
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

    $file->fields = $fields;

    // ===================================================

    return $file;
  }

  private function __from_xml_to_extensions_array($xml_extension_elements){

    $extensions = array();

    foreach( $xml_extension_elements as $xml_extension_element){

      $file = new Extension();
      $attributes = $xml_extension_element->attributes();

      //process the attributes
      $file->fields_terminated_by = stripcslashes((string) $attributes['fieldsTerminatedBy'] );
      $file->fields_enclosed_by   = stripcslashes((string) $attributes['fieldsEnclosedBy'] );
      $file->lines_terminated_by  = stripcslashes((string) $attributes['linesTerminatedBy'] );
      $file->encoding             = (string) $attributes['encoding'];
      $file->row_type             = (string) $attributes['rowType'];
      $file->ignore_header_lines   = (string) $attributes['ignoreHeaderLines'];
      $file->date_format          = 'DD/MM/YYYY';


      // locations =========================================
      $locations = $xml_extension_element->files->location;
      $files = array();

      // TODO: Falta agregar el fonder en el que esta el archivo.
      foreach ( $locations as $location){

        $index =  $this->zip->locateName($location, ZipArchive::FL_NOCASE|ZipArchive::FL_NODIR);
        $real_location = $this->zip->getNameIndex($index);

        $files[] = $this->dir.'/'.(String)$real_location;
      }

      $file->files = $files;
      // ===================================================

      $fields = array();
      // TODO: use this defaults
      $defaults = array();

      // id ================================================
      // Get the core mappings and configuration
      if(isset($xml_extension_element->coreid)){
        $id_attributes = $xml_extension_element->coreid->attributes();
        $fields[ (string) $id_attributes['index'] ] ='coreid';
      }
      // ===================================================

      // fields ======((====================================

      foreach($xml_extension_element->field as $field ){
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

      $file->fields = $fields;

      // ===================================================
      $extensions[] = $file;
    }

    return $extensions;
  }

  private function __metaxml_file_to_xml(){
    $index = $this->zip->locateName(Constants::META_FILENAME, ZipArchive::FL_NOCASE|ZipArchive::FL_NODIR);
    $file_in_zip = $this->zip->getNameIndex($index); 

    $file_path = $this->dir."/".$file_in_zip;

    // Read the xml by using simplexml library.
    $xml = simplexml_load_file($file_path);

    return $xml;
  }

  /**
   * @param $path
   *    the path of the file
   */
  public function __construct($path) {

    // DwC-A format validation
    $zip = new zipArchive();

    if (! $zip->open($path)){
      throw new Exception("Failed to open zip file\n");
    }

    // if everything works, the extraction proceed
    $status = TRUE;
    $hash   = hash_file('md5', $path);
    $dir    = dirname($path)."/".$hash;

    if(!file_exists($dir)){

      $status = $zip->extractTo($dir);

      if ( ! $status ){
        trigger_error("Error extracting $path to $dir", E_ERROR);
      }
    }
    
    $this->dir = $dir;
    $this->zip = $zip;

  }

  public function __set($dt, $vl) {
    if(property_exists($this, $dt)){
      $this->{$dt} = $vl;
    }else{
      throw new Exception("Undefined variables");
    }
  }

  public function __get($dt) {
    if(property_exists($this, $dt)){
      return $this->{$dt};
    }else{
      throw new Exception("Undefined variables");
    }
  }

  /**
    Case 1:
    A metafile is not required if there is a single core data file where the first
    line provides the names of the Darwin Core terms represented in
    the published data. The column labels must exactly match the
    supported Darwin Core terms.

    If the core data file does not contain such a “header row” then the
    columns names must be stored in a metafile.
   */
  private function __validate_case_1(){
    $result = FALSE;

    if ( $this->zip->numFiles == 1){
      $result = TRUE;

      // Set the type of the file to help in the processing.
      $this->case = 1;
    }


    return $result;
  }

  /**
    Case 2:
    A metafile is not required if an archive contains a resource metadata
    document that is named “EML.xml” If the resource metadata document
    is named other than “EML.xml,” a metafile is required. The
    requirements are based on the same conditions as above concerning the
    presence/absence of a ‘header row’ in the core data file.
   */
  private function __validate_case_2(){

    $result = FALSE;

    if ( $this->zip->numFiles == 2){

      $result = $this->zip->locateName(Constants::EML_FILENAME, ZipArchive::FL_NOCASE|ZipArchive::FL_NODIR);

      if( FALSE !== $this->zip->locateName(Constants::EML_FILENAME, ZipArchive::FL_NOCASE|ZipArchive::FL_NODIR) ){
        $result = TRUE;

        // Set the type of the file to help in the processing.
        $this->case = 2;
      }
    }


    return $result;
  }

  /**
    Case 3:
    A metafile is required when one or more extensions are used to extend
    the data published in the core data file. The metafile provides the map
    that links the two data files together.
   */ 
  private function __validate_case_3(){

    $result = FALSE;

    if ( $this->zip->numFiles >= 3 ){
      if( FALSE !== $this->zip->locateName(Constants::META_FILENAME, ZipArchive::FL_NOCASE|ZipArchive::FL_NODIR)){
        $result = TRUE;

        // Set the type of the file to help in the processing.
        $this->case = 3;
      }
    }


    return $result;
  }


  /**
   * Makes a shallow validation of the number of files in the darwin core archive.
   */
  public function validate(){

    $seems_valid = $this->__validate_case_1() || $this->__validate_case_2() || $this->__validate_case_3();

    if ($seems_valid == FALSE){
        throw new Exception("The file supplied seems to be unvalid"); 
    }

    return $seems_valid;
  }

  /**
   * Open the DwC-A and validate it depending in the Cases specified by the DwC-A
   * specification
   */
  public function open(){

    if( !$this->validate() ){
      return;
    }

    $archive = new Archive();
    $core = new Core();
    $extensions = array();


    switch($this->case){
        
      case 1: # only core file
        $core = $this->__open_core_with_headers();
        break;

      case 2: # core file and EML.xml file
        $core = $this->__open_core_with_headers();
        break;
        // the metadata is processed in the method DWCA::get_metadata

      case 3: # with meta.xml file
        // Read the xml by using simplexml library.
        $xml = $this->__metaxml_file_to_xml();

        //load the info from the CORE file
        $core = $this->__from_xml_to_core_file($xml->core);
        //load the info from the EXTENSIONS file
        $extensions = $this->__from_xml_to_extensions_array($xml->extension);
        break;

      case -1:
        print "something went terribly wrong, verify your imput file and try again";
        break;
    }


    $archive->extensions = $extensions;
    $archive->core = $core;
    $this->meta = $archive;
  }
}

