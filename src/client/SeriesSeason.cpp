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
 * @file SeriesSeason.cpp
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
         // cout << "title name: " << title << endl;
      }
      else if (*i == "seriesSeason")
      {
         seriesSeason = jsonM.asString();
         // cout << "season NO.: " << seriesSeason << endl;
      }
      else if (*i == "imdbRating")
      {
         imdbRating = jsonM.asString();
         // cout << "Rating: " << imdbRating << endl;
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
      else if (*i == "episodes")
      {

         for (int j = 0; j < jsonM.size(); j++)
         {
            Episode epi(jsonM[j]);
            addEpisode(epi);
         }
      }
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
            // cout << "title name: " << title << endl;
         }
         else if (*i == "seriesSeason")
         {
            seriesSeason = jsonM.asString();
            // cout << "season NO.: " << seriesSeason << endl;
         }
         else if (*i == "imdbRating")
         {
            imdbRating = jsonM.asString();
            // cout << "Rating: " << imdbRating << endl;
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
         else if (*i == "episodes")
         {
            
            for (int j = 0; j < jsonM.size(); j++)
            {
               Episode epi = Episode(jsonM[j]);
               addEpisode(epi);
            }
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
   Episode epi = episodeList[name];
   return epi;

}

bool SeriesSeason::checkEpisodes()
{
   return (!episodeList.size() == 0);
}

bool SeriesSeason::addEpisode(Episode epi)
{

   for (auto &ep : episodeList)
   {
      if (ep.second.getName() == epi.getName())
      {
         cout << epi.getName() << " is already on the episode list!" << endl;
         return false;
      }
   }
   this->episodeList.insert(pair<string, Episode>(epi.getName(), epi));
   // cout << epi.getName() << " was added to the episode list for " << this->getTitle() << endl;
   return true;
}


bool SeriesSeason::removeEpisode(string name)
{
   // cout << "Epi list size: " << episodeList.size() << endl;

   if (episodeList.size() == 0)
   {
      cout << "The episode list is empty!" << endl;
      return false;
   }
   for (auto epi : episodeList)
   {
      if (epi.second.getName() == name)
      {
         this->episodeList.erase(name);
         cout << epi.second.getName() << " was removed from the library."
              << "\n\n";
         // cout << "Epi list size: " << episodeList.size() << endl;
         return true;
      }
   }
   cout << name << " was not found in the episode list.\n";
   return false;

}

vector<string> SeriesSeason::getEpisodeTitles()
{

   vector<string> epVector;
   for(map<string, Episode>::iterator iter = episodeList.begin(); 
               iter != episodeList.end(); ++iter)
   {
      epVector.push_back(iter->first);
      // cout << iter->first << endl;
   }   
   return epVector;
}

string SeriesSeason::toJsonString()
{
   string ret = "{}";
   Json::Value jsonLib;

   jsonLib["title"] = title;
   jsonLib["seriesSeason"] = seriesSeason;
   jsonLib["imdbRating"] = imdbRating;
   jsonLib["genre"] = genre;
   jsonLib["poster"] = poster;
   jsonLib["plotSummary"] = plotSummary;
   int index = 0;
   for (auto epi : episodeList)
   {
      jsonLib["episodes"][index++] = epi.second.toJson();
   }

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
   int index = 0;
   for (auto epi : episodeList)
   {
      jsonLib["episodes"][index++] = epi.second.toJson();
   }

   return jsonLib;
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
   cout << "media: " << title << "\nseries-season: " << seriesSeason
        << "\nimdbRating: " << imdbRating << "\ngenre: " << genre
        << "\nimage-URL: " << poster
        << "\nSummary: " << plotSummary << "\n";

   for (auto epi : episodeList)
   {
      epi.second.print();
   }
}
