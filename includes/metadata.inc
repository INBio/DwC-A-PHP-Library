<?php

class Adreess{
  private $address;
  private $city;
  private $province;
  private $country;
  private $postalCode;
}

class Agent{
  private $firstName;
  private $lastName;
  private $organisation;
  private $position;
  private $address;
}

class Citation{
  private $identifier;
  private $citation;
  private $phone;
  private $email;
  private $role;
  private $homepage;
}

class Attribute{
  private $category;
  private $localeBundle;
  private $name;
  private $value;
}

class BBox{

  // x = east/west=longitude, -180/180
  // y = north/south = latitude, -90/90
  private $max;
  private $min;

}

class BibliographicCitationSet{

  private $bibliographicCitations = Array();
}

class GeospatialCoverage{

  private $description;
  private $keywords = Array();
  private $boundingCoordinates;
}


class KeywordSet{

  private $keywordThesaurus;
  private $keywords = Array();
}

class LocaleBundle{

  private $locale;
  private $charset;
}


class PhysicalData{

  private $charset;
  private $distributionUrl;
  private $format;
  private $formatVersion;
  private $name;
}

class Point{

  const MAX_LONGITUDE = 180.0;
  const MIN_LONGITUDE = -180.0;
  const MAX_LATITUDE = 90.0;
  const MIN_LATITUDE = -90.0;
  // x (east/west), -180/180
  private $longitude;
  // y (north/south), -90/90
  private $latitude;
}


class Project{
  private $title;
  private $personnel;
  private $funding;
  private $studyAreaDescription;
}

class StudyAreaDescription{

  private $name = StudyAreaDescriptor::GENERIC;
  private $citableClassificationSystem = "false";
  private $descriptorValue;
  private $designDescription;
}

class TaxonKeyword{

  private $scientificName;
  private $rank;
  private $commonName;
}


class TaxonomicCoverage{

  private $description;
#  private $taxonKeywords = new ArrayList<TaxonKeyword>();
  private $taxonKeywords = Array();
}


class TemporalCoverage{

  private $startDate;
  private $endDate;
  private $formationPeriod;
  private $livingTimePeriod;
}


class TimeKeyword{
  private $start;
  private $end;
}


?>
