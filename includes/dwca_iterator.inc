<?php

include_once('parser.inc');

/**
 * Store every record retrieved from the DwC-A file.
 */
class DWCARecord{

  public $data = Array();

  public function __construct() {
    $this->data = Array();
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
}


/**
 * Iterate over a DwC-A file and keep track of the current line
 */
class DwCAIterator implements Iterator {

  public $meta;

  public $core_parser;
  public $core_iterator;

  public $parsers;
  public $iterators;

  public $status;

  public $current;


  public function __construct($meta) {

    $parsers = Array();
    $iterators = Array();

    $utils = $this->__create_parser_and_iterator($meta->core);

    $this->core_parser = $utils[0];
    $this->core_iterator = $utils[1];

    $extensions = $meta->extensions;
    $status = 0;

    foreach($extensions as $ext){
      $utils = $this->__create_parser_and_iterator($ext);

      $parsers[$ext->row_type] = $utils[0];
      $iterators[$ext->row_type] = $utils[1];
    }

    $this->status = $status;
    $this->parsers = $parsers;
    $this->iterators = $iterators;
    $this->meta = $meta;
  }

  private function __create_parser_and_iterator($archive){

    $parser = NULL;
    $iterator = NULL;

    // Create and config the parser
    $parser = new ParserCSV();
    $parser->setDelimiter($archive->fields_terminated_by);
    $parser->setColumnNames(array_values($archive->fields));

    // parsing occurrence.txt file looking for occurrences
    $iterator = new ParserCSVIterator($archive->files[0]);

    return Array($parser, $iterator);
  }

  private function __parseItems(ParserCSV $parser, $iterator,  $chunk_size = 50, $defaults = Array()) {

    $parser->setStartByte($iterator->currentPos());
    $parser->setLineLimit($chunk_size);
    $rows = $parser->parse($iterator, $defaults);

    return $rows;
  }

  /**
   * Return all the files to the start (Re-initialize all the variables).
   */
  public function rewind($pos = 0) {
    $this->__construct($this->meta);
  }

  public function next($chunk_size = 50) {


    $core_rows = $this->__parseItems($this->core_parser, $this->core_iterator, $chunk_size);
    
    // TODO: store current Line
    $this->status = $this->core_iterator->currentPos();


    $rows = array();
    $row = array();

    foreach ($core_rows as $core_row){

      $new_record = new DWCARecord();
      $new_record->data['core'] = $core_row; 

      $old1 = 0;
      $old2 = 0;

      foreach ( array_keys($this->parsers) as $key ){

        do{

          $iter_pos = $this->iterators[$key]->currentPos();
          $old1 = $old2;
          $old2 = $iter_pos;
         
          $row = $this->__parseItems($this->parsers[$key], $this->iterators[$key], 1);

          if ( empty($row) || empty($core_row) ){
            break;
          }

          if( $row[0]['coreid'] === $core_row['id'] ){
            $new_record->data[$key][] = $row; 
          }else{
            $this->iterators[$key]->rewind($old1);
            break;
          }
        }while(true);
      }
      $rows[] = $new_record;
    }

    $this->current = $rows;
  }

  public function valid() {
    return $this->core_iterator->valid();
  }

  public function current() {
    return $this->current;
  }

  public function currentPos() {
    return $this->status;
  }

  public function key() {
    return 'dwca';
  }
}

