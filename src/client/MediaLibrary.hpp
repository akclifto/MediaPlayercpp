#include <string>
#include <map>
#include <vector>
#include <json/json.h>
#include "SeriesSeason.hpp"

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
 * @file MediaLibrary.hpp
 * @version April 2020
 */
class MediaLibrary
{  
   private:

      std::map<string, SeriesSeason> libraryMap;       // map to store library objects.
      
      /**
       * Method to add a series and season to the library from the top menu button action.
       * Method submits api call for series and season information, then passed to parse to 
       * library series and episode objects. 
       * @return void.
       * */
      void fetchURLAddSeries() ;
      
      /**
       * Method to remove series from the library using top menu Series-Season/Remove button.
       * @return void
       * */
      void actionRemoveSeries();


      /**
       * Method to add episode for a series to the library using top menu Episode/Add button.
       * @return void.
       * */
      void actionAddEpisode();

      /**
       * Method to remove Episode from a series in the library using the top menu 
       * Episode/Remove button.
       * @return void
       * */
      void actionRemoveEpisode();

   protected:

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
       * Method default code from sample.
       * */
      string serviceInfo();

      /**
       * Method to get SeriesSeason object from library by title
       * @param aTitle : title of the series to return from the library
       * @return SeriesSeason object if found in library.
       **/
      SeriesSeason getSeries(string aTitle);

      /**
       * Method to get map of library.
       * @return map data structure for SeriesSeason objects. 
       **/
      map<string, SeriesSeason> getLibrary();

      /**
       * Method to get the size of the episode list for a given series.
       * @return size of episode list.
       * */
      int getEpisodeListSize(string title);

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
       * Method to remove episode from the library by searching its series and title
       * @param series : title of the series to find episode.
       * @param episode : title of the episode to remove 
       * @return true if successfully removed from library, false otherwise.  
       * */
      bool removeEpisode(string series, string episode);

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
      bool parseURLtoJSON(string seriesInfo, string seasonInfo);
      /**
       * Method to print everything in the library.
       * @return void.
       **/
      void print();

};
