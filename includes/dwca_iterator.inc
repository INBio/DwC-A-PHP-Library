<?php

include_once('parser.inc');

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


class DwCAIterator implements Iterator {

  private $meta;

  private $core_parser;
  private $core_iterator;
  private $parsers;
  private $iterators;

  private $status;

  private $current;


  public function __construct($meta) {

    $parsers = Array();
    $iterators = Array();

    $utils = $this->__create_parser_and_iterator($meta->core);

    $this->core_parser = $utils[0];
    $this->core_iterator = $utils[1];

    $extensions = $meta->extensions;
    $status['core'] = 0;

    foreach($extensions as $ext){
      $utils = $this->__create_parser_and_iterator($ext);

      $parsers[$ext->row_type] = $utils[0];
      $iterators[$ext->row_type] = $utils[1];
      $status[$ext->row_type] = 0; 
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

#    var_dump($archive);
    // parsing occurrence.txt file looking for occurrences
    $iterator = new ParserCSVIterator($archive->files[0]);

    return Array($parser, $iterator);
  }

  private function __parseItems(ParserCSV $parser, $iterator, $start = 0, $limit = 0, $defaults = Array()) {
    $parser->setLineLimit($limit);
    $parser->setStartByte($start);
    $rows = $parser->parse($iterator, $defaults);
    return $rows;
  }

  public function rewind($pos = 0) {
    $this->__construct($this->meta);
  }

  public function next() {

    $new_record = new DWCARecord();


    $first_core_row = $this->__parseItems($this->core_parser, $this->core_iterator, $this->status['core'], 1);
    $this->status['core'] = $this->core_iterator->currentPos();

    $new_record->data['core'] = $first_core_row; 

    $row = array();

    foreach ( array_keys($this->parsers) as $key ){

      print $key."\n";

      do{
        print "Current line: ".$this->status[$key]."\n";
        $row = $this->__parseItems($this->parsers[$key], $this->iterators[$key], $this->status[$key], 1);
        if ( empty($row)){
          break;
        }

        print "coreid: ".$row[0]['coreid']." id: ".$first_core_row[0]['id']."\n";

        if( $row[0]['coreid'] === $first_core_row[0]['id'] ){

          if( $key === "http://rs.gbif.org/terms/1.0/Image" && isset($row[0]['http://purl.org/dc/terms/identifier'])){
            print $row[0]['http://purl.org/dc/terms/identifier']."\n";
          }

          $new_record->data[$key][] = $row; 
          $this->status[$key] = $this->iterators[$key]->currentPos();

        }else{
          break;
        }
      }while(true);
    }

    var_dump($new_record);
    $this->current = $new_record;
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

