/**
 * Copyright 2020, Adam Clifton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <p>
 * Purpose: Episode is a class whose properties describe a single
 * Episode work - a single Episode that will make up a seriesSeason class object.
 * Ser321 Principles of Distributed Software Systems
 * @author Adam Clifton akclifto@asu.edu
 *         Software Engineering, ASU
 * @file Episode.h
 * @version April 2020
 */

#ifndef EPISODE_H
#define EPISODE_H

#include <string>
#include <json/json.h>

using namespace std;


class Episode
{
private: 
    string name;
    string imdbRating;
    string epSummary;
    
protected:

public:
    /* All Constructors and Destructor*/
    Episode();
    Episode(string name, string imdbRating, string epSummary);
    Episode(const Json::Value &jsonObj);
    Episode(string jsonString);
    ~Episode();

    /* All getters*/
    string getImdbRating();
    string getName();
    string getEpSummary();

    /**
     * Method to serialize data to Json string.
     * @return string of Json-formatted data.
     **/
    string toJsonString();

    /**
     * Method to serialize data to Json object
     * @return Json::Value object
     * */
    Json::Value toJson();

    /**
     * Method to print Episode object informatio to console.
     * @return void
     * */
    void print();
};

#endif