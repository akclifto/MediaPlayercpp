#include <iostream>
#include <stdlib.h>
#include "Episode.hpp"

using namespace std;

/**
 * Copyright 2020 Adam Clifton
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
 * Purpose: Episode is a class whose properties describe a single
 * Episode work - a single Episode that will make up a seriesSeason class object.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Adam Clifton akclifto@asu.edu
 *         Software Engineering, ASU
 * @version April 2020
 */

Episode::Episode()
{
    name = "";
    imdbRating = "";
    epSummary = "";
}

Episode::Episode(string name, string imdbRating, string epSummary)
{

    this->name = name;
    this->imdbRating = imdbRating;
    this->epSummary = epSummary;
}

Episode::Episode(const Json::Value &jsonObj)
{

    Json::Value::Members mbr = jsonObj.getMemberNames();
    for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end(); i++)
    {
        //cout << *i << " " << endl;
        Json::Value jsonM = jsonObj[*i];
        if (*i == "name")
        {
            name = jsonM.asString();
        }
        else if (*i == "imdbRating")
        {
            imdbRating = jsonM.asString();
        }
        else if (*i == "epSummary"){
            epSummary = jsonM.asString();
        }
    }
}

Episode::Episode(string jsonString)
{
    Json::Reader reader;
    Json::Value root;
    bool parseSuccess = reader.parse(jsonString, root, false);
    if (parseSuccess)
    {
        //cout << "successful parse" << endl;
        Json::Value::Members mbr = root.getMemberNames();
        for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end(); i++)
        {
            //cout << *i << " " << endl;
            Json::Value jsonM = root[*i];
            if (*i == "name")
            {
                name = jsonM.asString();
            }
            else if (*i == "imdbRating")
            {
                imdbRating = jsonM.asString();
            }
            else if (*i == "epSummary") {
                epSummary = jsonM.asString();
            }
        }
    }
    else
    {
        cout << "Episode constructor parse error with input: " << jsonString
             << endl;
    }
}

Episode::~Episode()
{
    name = "";
    imdbRating = "";
    epSummary = "";
}

string Episode::getImdbRating()
{
    return imdbRating;
}

string Episode::getName()
{
    return name;
}

string Episode::getEpSummary() {
    return epSummary;
}

string Episode::toJsonString()
{
    string ret = "{}";
    Json::Value jsonLib;
    jsonLib["name"] = name;
    jsonLib["imdbRating"] = imdbRating;
    jsonLib["epSummary"] = epSummary;
    ret = jsonLib.toStyledString();
    return ret;
}

Json::Value Episode::toJson()
{

    //string ret = "{}";
    Json::Value jsonLib;
    jsonLib["name"] = name;
    jsonLib["imdbRating"] = imdbRating;
    jsonLib["epSummary"] = epSummary;

    return jsonLib;
}

void Episode::fromjson(Json::Value json)
{

    //TODO
}

void Episode::print()
{

    cout << "name: " << name << ", imdbRating: " << imdbRating 
         << ", epSummary: " << epSummary << "\n";

}
