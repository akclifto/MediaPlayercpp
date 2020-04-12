#include <string>
#include <cmath>
#include <json/json.h>
#include "Episode.hpp"

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
 * Purpose: SeriesSeason is a class whose properties describe a single
 * media work -- a single SeriesSeason.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @author Adam Clifton akclifto@asu.edu
 *         Software Engineering, ASU
 * @file SeriesSeason.hpp
 * @version April 2020
 */
class SeriesSeason
{

protected:
    string title;
    string seriesSeason;
    string imdbRating;
    string genre;
    string poster;
    string plotSummary;
    map<string, Episode> episodeList;

public:
    /*All Constructors and Destructor*/
    SeriesSeason();
    SeriesSeason(string aTitle, string aSeriesSeason,
                 string aImdbRating, string aGenre, string poster, string plotSummary);
    SeriesSeason(const Json::Value &jsonObj);
    SeriesSeason(string jsonString);
    ~SeriesSeason();

    /*All getters */
    string getTitle();
    string getSeriesSeason();
    string getImdbRating();
    string getGenre();
    string getPoster();
    string getSummary();
    map<string, Episode> getEpisodeList();
    vector<string> getEpisodeTitles();

    /**
     * Method to get episode from the episodeList by name.
     * @param name : name of the episode
     * @return Episode object 
     **/
    Episode getEpisode(string name);

    /**
     * Method to add episode to the episodeList for a SeriesSeason object.
     * @param name : name of the episode to add to the list. 
     * @return void 
     **/
    void addEpisode(Episode name);

    /**
     * Method to remove episode from the episodeList for a series.
     * @param name : string name of the episode to remove
     * @return true if removed successfully, false otherwise. 
     **/
    bool removeEpisode(string name);

    /**
     * Method to check if episode list is empty
     * @return true if episode list is <b>not</b> empty, false otherwise.
     **/
    bool checkEpisodes();

    /**
     * Method to display String data in JSON file format for write output.
     * @return String of formatted SeriesSeason data.
     * */
    string toJsonString();

    /**
     * Method to serialize data to JSON file.
     * @return Json::Value object
     **/
    Json::Value toJson();

    /**
     * Method to parse data from Json::Value object to construct SeriesSeason
     * class object.
     * @param seriesInfo : string with series information from URL callback
     * @param seasonInfo : string with season information from URL callback
     * @return void
     **/
    void fromJson(string seriesInfo, string seasonInfo);

    /**
     * Method to set values to SeriesSeason Object.  Provdied with base code. 
     * @return void.
     **/
    void setValues(string aTitle, string aSeriesSeason,
                   string aRating, string aGenre, string poster, string plotSummary);

    /**
     * Method to print SeriesSeason object information to console.
     * @return void. 
     **/
    void print();
};
