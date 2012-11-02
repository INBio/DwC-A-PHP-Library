<?php

// ----- BEGIN Constants

/**
 * Default name of the GBIF eml profile
 */
if (!defined('DWCA_EML_FILE')) {
  define( 'DWCA_EML_FILE', 'eml.xml' );
}

/**
 * Default name of the DwC-A metadata
 */
if (!defined('DWCA_META_FILE')) {
  define( 'DWCA_META_FILE', 'meta.xml' );
}

/**
 * Default Field separator in the txt files
 */
if (!defined('DWCA_DEFAULT_FIELD_SEPARATOR')) {
  define( 'DWCA_DEFAULT_FIELD_SEPARATOR', "\t" );
}

/**
 * Default core rowType
 */
if (!defined('DWCA_DEFAULT_ROWTYPE')) {
  define( 'DWCA_DEFAULT_ROWTYPE', 'http://rs.tdwg.org/dwc/terms/Occurrence' );
}

// ----- END Constants


/**
 * Manage a DwC-A (Darwin Core Archive) to extract information from the zip file
 * and return it as arrays (hashes).
 */
interface DwCA {


  /**
   * Initialize the class, set defaults and check if the file exist
   * @param $path
   *  A string with the path of the file to access in the file system.
   *
   * @return
   *  0 if the file exist and in the right format.
   *  1 if error.
   */
  public function create($path);

  /**
   * List the contents of the zip file.
   * @return
   *  An array (hash) with the name of the file and the category
   *  array(
   *    array( type=> , name=> ),
   *    array( type=> , name=> ),
   *    ...);
   */
  public function list_contents();

  /**
   * Return an array with a subset of the metadata contained in the eml.xml file
   * @return
   *  an array with the metadata in the format: array( provider= , project=, ...);
   */
  public function get_metadata();

  /**
   * Return the information of the core extension as well as his type
   * (Taxon or Occurrence)
   * @param $start
   *   initial line position
   * @param $limit
   *   item offset
   *
   * @return
   *  An array with the information from the core file.
   *  array( type=> , array(column => value, column => value, ....));
   */
  public function get_core_info($start = 0, $limit = 10);

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
  public function get_extension_info($extension);

  /**
   * Return the columns mapped by the core file by reading the meta.xml file
   * @return Array
   *   An array of the columns mapped in the file
   */
  public function get_core_mappings();

  /**
   * Return the parser
   * @return ParserCSV
   *   The instance of the Parser being used to parse the file.
   */
  public function get_parser();

}
