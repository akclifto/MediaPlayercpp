#include <iostream>
#include <stdlib.h>
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
   imageURL = "";
}

SeriesSeason::SeriesSeason(string aTitle, string aSeriesSeason,
                           string aRating, string aGenre, string anImageURL)
{
   title = aTitle;
   seriesSeason = aSeriesSeason;
   imdbRating = aRating;
   genre = aGenre;
   imageURL = anImageURL;
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
      else if (*i == "imageURL")
      {
         imageURL = jsonM.asString();
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
         else if (*i == "imageURL")
         {
            imageURL = jsonM.asString();
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
   imageURL = "";
}

string SeriesSeason::getTitle(){
   return title;
}

string SeriesSeason::getSeriesSeason(){
   return seriesSeason;
}
string SeriesSeason::getImdbRating() {
   return imdbRating;
}
string SeriesSeason::getGenre(){
   return genre;
}
string SeriesSeason::getImageURL() {
   return imageURL;
}

string SeriesSeason::getSummary() {
   return summary;
}
vector<Episode> SeriesSeason::getEpisodeList(){
   return episodeList;
}

string SeriesSeason::toJsonString()
{
   string ret = "{}";
   Json::Value jsonLib;
   jsonLib["title"] = title;
   jsonLib["seriesSeason"] = seriesSeason;
   jsonLib["rating"] = imdbRating;
   jsonLib["genre"] = genre;
   jsonLib["imageURL"] = imageURL;
   ret = jsonLib.toStyledString();
   return ret;
}

Json::Value SeriesSeason::toJson()
{
   //string ret = "{}";
   Json::Value jsonLib;
   jsonLib["title"] = title;
   jsonLib["seriesSeason"] = seriesSeason;
   jsonLib["imdbRating"] = imdbRating;
   jsonLib["genre"] = genre;
   jsonLib["imageURL"] = imageURL;
   return jsonLib;
}

void SeriesSeason::setValues(string aTitle, string aSeriesSeason,
                             string aRating, string aGenre, string anImageURL)
{
   title = aTitle;
   seriesSeason = aSeriesSeason;
   imdbRating = aRating;
   genre = aGenre;
   imageURL = anImageURL;
}

void SeriesSeason::print()
{
   cout << "media " << title << " series-season " << seriesSeason 
         << " imdbRating " << imdbRating << " genre " << genre 
         << " image-URL " << imageURL << "\n";
}
