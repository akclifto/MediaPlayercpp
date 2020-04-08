#include <iostream>
#include <stdlib.h>
#include <algorithm>
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
 * Purpose: SeriesSeason is a class whose properties describe a single
 * media work - a single SeriesSeason.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @author Adam Clifton akclifto@asu.edu
 *         Software Engineering, ASU
 * @version April 2020
 */
SeriesSeason::SeriesSeason()
{
   title = "";
   seriesSeason = "";
   imdbRating = "";
   genre = "";
   poster = "";
   plotSummary = "";
}

SeriesSeason::SeriesSeason(string aTitle, string aSeriesSeason,
                           string aRating, string aGenre,
                           string poster, string plotSummary)
{
   this->title = aTitle;
   this->seriesSeason = aSeriesSeason;
   this->imdbRating = aRating;
   this->genre = aGenre;
   this->poster = poster;
   this->plotSummary = plotSummary;
}

SeriesSeason::SeriesSeason(const Json::Value &jsonObj)
{

   Json::Value::Members mbr = jsonObj.getMemberNames();
   for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end(); i++)
   {
      //cout << *i << " " << endl;
      Json::Value jsonM = jsonObj[*i];
      if (*i == "title")
      {
         title = jsonM.asString();
      }
      else if (*i == "seriesSeason")
      {
         seriesSeason = jsonM.asString();
      }
      else if (*i == "imdbRating")
      {
         imdbRating = jsonM.asString();
      }
      else if (*i == "genre")
      {
         genre = jsonM.asString();
      }
      else if (*i == "poster")
      {
         poster = jsonM.asString();
      }
      else if (*i == "plotSummary")
      {
         plotSummary = jsonM.asString();
      }
      //TODO add episode array
   }
}

SeriesSeason::SeriesSeason(string jsonString)
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
         if (*i == "title")
         {
            title = jsonM.asString();
         }
         else if (*i == "seriesSeason")
         {
            seriesSeason = jsonM.asString();
         }
         else if (*i == "imdbRating")
         {
            imdbRating = jsonM.asString();
         }
         else if (*i == "genre")
         {
            genre = jsonM.asString();
         }
         else if (*i == "poster")
         {
            poster = jsonM.asString();
         }
         else if (*i == "plotSummary")
         {
            plotSummary = jsonM.asString();
         }
      }
   }
   else
   {
      cout << "SeriesSeason constructor parse error with input: " << jsonString
           << endl;
   }
}

SeriesSeason::~SeriesSeason()
{
   title = "";
   seriesSeason = "";
   imdbRating = "";
   genre = "";
   poster = "";
   plotSummary = "";
}

string SeriesSeason::getTitle()
{
   return title;
}

string SeriesSeason::getSeriesSeason()
{
   return seriesSeason;
}

string SeriesSeason::getImdbRating()
{
   return imdbRating;
}

string SeriesSeason::getGenre()
{
   return genre;
}

string SeriesSeason::getPoster()
{
   return poster;
}

string SeriesSeason::getSummary()
{
   return plotSummary;
}

map<string, Episode> SeriesSeason::getEpisodeList()
{
   return episodeList;
}

Episode SeriesSeason::getEpisode(string name)
{

   map<string, Episode>::iterator iter = episodeList.find(name);
   if (iter == episodeList.end())
   {
      return Episode();
   }
   else
   {
      return iter->second;
   }
}

bool SeriesSeason::checkEpisodes()
{
   return (!episodeList.size() == 0);
}

void SeriesSeason::addEpisode(Episode epi)
{
   this->episodeList.insert(pair<string, Episode>(epi.getName(), epi));
}

bool SeriesSeason::removeEpisode(string name)
{

   int removed = episodeList.erase(name);
   if (removed == 0)
   {
      return false;
   }
   else
   {
      return true;
   }
}

string SeriesSeason::toJsonString()
{
   string ret = "{}";
   Json::Value jsonLib;
   jsonLib["title"] = title;
   jsonLib["seriesSeason"] = seriesSeason;
   jsonLib["rating"] = imdbRating;
   jsonLib["genre"] = genre;
   jsonLib["poster"] = poster;
   jsonLib["plotSummary"] = plotSummary;
   ret = jsonLib.toStyledString();
   return ret;
}

Json::Value SeriesSeason::toJson()
{

   Json::Value jsonLib;
   jsonLib["title"] = title;
   jsonLib["seriesSeason"] = seriesSeason;
   jsonLib["imdbRating"] = imdbRating;
   jsonLib["genre"] = genre;
   jsonLib["poster"] = poster;
   jsonLib["plotSummary"] = plotSummary;
   return jsonLib;
}

void SeriesSeason::fromJson(Json::Value json)
{
   //TODO
}

void SeriesSeason::setValues(string aTitle, string aSeriesSeason,
                             string aRating, string aGenre,
                             string poster, string plotSummary)
{
   this->title = aTitle;
   seriesSeason = aSeriesSeason;
   imdbRating = aRating;
   genre = aGenre;
   poster = poster;
   plotSummary = plotSummary;
}

void SeriesSeason::print()
{
   cout << "media: " << title << ", series-season: " << seriesSeason
        << ", imdbRating: " << imdbRating << ", genre: " << genre
        << ", image-URL: " << poster
        << ", Summary: " << plotSummary << "\n";

   for (auto epi : episodeList)
   {
      epi.second.print();
   }
}
