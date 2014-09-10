#pragma once

#include <string>
#include <list>
#include <unordered_map>

typedef std::pair< int, int >                                     TCoordinates;
typedef std::list< TCoordinates >                                 TCoordinatesContainer;
typedef std::unordered_map< std::string, TCoordinatesContainer >  TDataContainer;