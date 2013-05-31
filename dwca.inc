<?php

/**
  
  DwCLibrary was build to open and query dwc files used by the GBIF Network

  by Aurelio Sanabria INBio (Costa Rica).
  
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

      And the Darwin Core Text Guide accessible online at:
        http://rs.tdwg.org/dwc/terms/guides/text/index.htm
 */


// DWC realted
include_once('includes/meta.inc');
include_once('includes/gbif_eml.inc');

// Utils
include_once('includes/eml_utils.inc');
include_once('includes/constants.inc');
include_once('includes/parser.inc');


/**
 * Manage a DwC-A (Darwin Core Archive) to extract information from the zip file
 * and return it as arrays (hashes).
 */
class DwCA{


  private $case = -1;

  private $meta = new Archive();
  private $gbif_eml;

  private $dir;
  private $archive;




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

    // Create and config the parser
    $parser = new ParserCSV();
    $parser->setDelimiter($this->core->fields_terminated_by);
    $parser->setColumnNames(array_values($this->core->fields));
    $this->parser = $parser;

  }

  public function get_records(ArchiveFile $file, $start = 0, $limit = 50){

    $status = TRUE;
    $filename = $this->dir."/".$file->location;

    if(!file_exists($filename)){
      throw new Exception("$filename: not found. Problably due to an error while unziping the dwca file");
    }

    // Create and config the parser
    $parser = new ParserCSV();
    $parser->setDelimiter($file->fields_terminated_by);
    $parser->setColumnNames(array_values($file->fields));
    $this->parser = $parser;

    // parsing occurrence.txt file looking for occurrences
    $iterator = new ParserCSVIterator($filename);
    $rows = $this->_parseItems($parser, $iterator, $start, $limit, $file->defaults);

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

  private function generate_archive_file_from_xml($xml_archive_file){

    $attributes = $xml_archive_file->attributes();
    $file = new ArchiveFile();

    $file->fields_terminated_by = stripcslashes((string) $attributes['fieldsTerminatedBy'] );
    $file->fields_enclosed_by   = stripcslashes((string) $attributes['fieldsEnclosedBy'] );
    $file->lines_terminated_by  = stripcslashes((string) $attributes['linesTerminatedBy'] );
    $file->encoding             = (string) $attributes['encoding'];
    $file->row_type             = (string) $attributes['rowType'];
    $file->ignoreheader_lines   = (string) $attributes['ignoreHeader_lines'];
    $file->location             = (string) $xml_archive_file->files->location;
    $file->date_format          = 'DD/MM/YYYY';


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

    $file->fields = $fields;
    $file->defaults = $defaults;

    return $file;
  }


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

    if ( $zip->numFiles == 1){
      $result = TRUE;
      trigger_error("This DwC-A with a core file only\n", E_NOTICE);

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

    if ( $zip->numFiles == 2){
      if( $zip->locateName(Constants::EML_FILENAME)){
        $result = TRUE;
        trigger_error("This DwC-A with a EML.xml and core file\n", E_NOTICE);

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

    if ( $zip->numFiles >= 3 ){
      if( $zip->locateName(Constants::META_FILENAME, ZipArchive::FL_NOCASE|ZipArchive::FL_NODIR)){

        $result = TRUE;
        trigger_error("This a Full DwC-A\n", E_NOTICE);

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

    $result = __validate_case_1() || __validate_case_2() || __validate_case_3();

    return $result;

  }
}
