#include "SeriesSeason.hpp"
#include <string>
#include <map>
#include <vector>
#include <json/json.h>

using namespace std;

/**
 * Copyright 2020 Tim Lindquist, Adam Clifton 
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Purpose: MediaLibrary is a class defining the interface between clients
 * and the server. The server implementation of MediaLibrary
 * provides storage for description of multiple media works -- SeriesSeason(s).
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @author Adam Clifton akclifto@asu.edu
 *         Software Engineering, ASU
 * @version April 2020
 */
class MediaLibrary
{
protected:
   map<string, SeriesSeason> libraryMap;       // map to store library objects.
   map<string, SeriesSeason> SeriesSeasonList; // may not need

public:
   MediaLibrary();
   ~MediaLibrary();

   /**
    * Method to initialize library from a JSON file.
    * @param jsonFileName : path/name of the JSON file used to initialize library.
    * @return true if library parsed and implemented correctly, false otherwise.
    **/
   bool initLibraryFromJsonFile(string jsonFileName);

   /**
    * Method to write library to JSON file. 
    * @param jsonFileName : name of the file to be written
    * @return true if written to file correctly, false otherwise
    **/
   bool toJsonFile(string jsonFileName);

   /**
    * Method default code. TODO: 
    * */
   string serviceInfo();

   /**
    * Method to get SeriesSeason object from library by title
    * @param aTitle : title of the series to return from the library
    * @return SeriesSeason object if found in library.
    **/
   SeriesSeason get(string aTitle);

   /**
    * Method to get map of library.
    * @return map data structure for SeriesSeason objects. 
    **/
   map<string, SeriesSeason> getLibrary();

   /**
    * Method to add series to the library
    * @param seriesSeason : SeriesSeason object to add to the library 
    **/
   void addSeries(SeriesSeason seriesSeason);

   /**
    * Method to remove series from the library by searching its title
    * @param title : title of the series to remove. 
    * @return true if successfully removed from library, false otherwise.  
    * */
   bool removeSeries(string title);

   /**
    * Method to return vector of series title from the library.
    * @return vector of series' titles from library. 
    **/
   vector<string> getTitles();

   /**
    * Method tom parse string data from URL request into JSON files for SeriesSeason
    * and Episode(s), then creates objects for both.
    * 
    **/
   void parseURLtoJSON();
   /**
    * Method to print everything in the library.
    * @return void.
    **/
   void print();

};
