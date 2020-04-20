/**
 * Copyright 2020 Tim Lindquist, Adam CLifton
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
 * @file MediaLibrary.cpp
 * @version April 2020
 */

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "MediaLibrary.hpp"

using namespace std;

MediaLibrary::MediaLibrary()
{
   initLibraryFromJsonFile("series.json");

}

MediaLibrary::~MediaLibrary()
{
   libraryMap.clear();
}

bool MediaLibrary::initLibraryFromJsonFile(string jsonFileName)
{ //load from library

   bool ret = false;

   libraryMap.clear();
   Json::Reader reader;
   Json::Value root;

   ifstream json(jsonFileName.c_str(), ifstream::binary);
   bool parseSucess = reader.parse(json, root);

   if (parseSucess)
   {
      for (Json::Value::const_iterator lib = root.begin(); lib != root.end(); lib++)
      {

         for (Json::Value::const_iterator series = (*lib).begin();
              series != (*lib).end(); series++)
         {

            for (Json::Value::const_iterator input = (*series).begin();
                 input != (*series).end(); input++)
            {
               
               cout << *input << endl;
              
               SeriesSeason ss(*input);
               addSeries(ss);
            }
         }
      }
      cout << jsonFileName << " initialized and loaded to the library.\n"
           << endl;
      ret = true;
   }
   return ret;
}

bool MediaLibrary::toJsonFile(string jsonFileName)
{ //save to library

   bool ret = false;
   Json::Value master;
   Json::Value seriesArr;

   Json::Value seriesObj;

   for (auto &lib : libraryMap)
   {
      seriesObj = lib.second.toJson();
      seriesArr["series"] = seriesObj;
      master["library"].append(seriesArr); //append creates an array object
   }

   //write to file
   Json::StyledStreamWriter ssw("  ");
   ofstream jsonOutFile(jsonFileName.c_str(), ofstream::binary);
   ssw.write(jsonOutFile, master);
   cout << "The library was saved to the following file: " << jsonFileName << endl;

   return true;
}

SeriesSeason MediaLibrary::getSeries(string aTitle)
{
   SeriesSeason series;
   for(auto &ss : libraryMap){
      if(ss.second.getTitle() == aTitle){
         series =  ss.second;
      }
   }
   return series;
}

Json::Value MediaLibrary::jsonGetSeries(string seriesName) {

   SeriesSeason series;
   for(auto &ss : libraryMap){
      if(ss.second.getTitle() == seriesName){
         series =  ss.second;
      }
   }
   return series.toJson();
}


map<string, SeriesSeason> MediaLibrary::getLibrary()
{
   return libraryMap;
}


int MediaLibrary::getEpisodeListSize(string title){

   return getSeries(title).getEpisodeList().size();
}

void MediaLibrary::addSeries(SeriesSeason seriesSeason)
{

   bool flag = false;
   for (auto &ss : libraryMap)
   {
      if (ss.second.getTitle() == seriesSeason.getTitle() &&
          ss.second.getSeriesSeason() == seriesSeason.getSeriesSeason())
      {
         flag = true;
         break;
      }
   }

   if (flag)
   {
      cout << seriesSeason.getTitle() << " already included in the library!" << endl;
   }
   else
   {
      libraryMap.insert(pair<string, SeriesSeason>(seriesSeason.getTitle(), seriesSeason));
      cout << seriesSeason.getTitle() << " was added to the library." << endl;
   }
}

bool MediaLibrary::removeSeries(string title)
{

   if (libraryMap.size() == 0)
   {
      cout << "The Library is empty!" << endl;
      return false;
   }

   int remove = libraryMap.erase(title);
   // cout << to_string(remove) << endl;
   if(remove != 0){
      cout << title << " was removed from the library."
           << "\n\n";
      return true;
   }

   cout << title << " was not found in the library!" << endl;
   return false;
}


bool MediaLibrary::removeEpisode(string series, string episode) {
   
   return getSeries(series).removeEpisode(episode);
}

vector<string> MediaLibrary::getTitles()
{
   vector<string> myVec;

   //print();
   for (map<string, SeriesSeason>::const_iterator it = libraryMap.begin();
        it != libraryMap.end(); ++it)
   {
      cout << "iter output: " << it->first << endl;
      myVec.push_back(it->first);
   }
   return myVec;
}

bool MediaLibrary::checkSeries(string seriesName) {
   
   for(auto &series: libraryMap){
      if(series.second.getTitle() == seriesName){
         return true;
      }
   }
   return false;

}

Json::Value MediaLibrary::jsonGetTitles() {

   vector<string> myVec = getTitles();
   Json::Value ssTitles;
   for(int i = 0; i < myVec.size(); i++) {
      ssTitles[i] = myVec[i];
   }
   return ssTitles;
}

bool MediaLibrary::parseURLtoJSON(string seriesInfo, string seasonInfo)
{
   
   Json::Reader reader;
   Json::Value root;

   Json::Value seriesObj;
   Json::Value epiObj;
   //run through series info
   bool parseSuccess = reader.parse(seriesInfo, root, false);
   string season;
   string title;

   if (parseSuccess)
   {
      // cout << "successful parse" << endl;
      Json::Value::Members mbr = root.getMemberNames();
      for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end(); i++)
      {
         Json::Value jsonM = root[*i];
         if (*i == "Title")
         {
            title = jsonM.asString();
            // cout << "title: " << jsonM.asString() << endl;
         }
         else if (*i == "Genre")
         {
            seriesObj["genre"] = jsonM.asString();
            // cout << "Genre: " << jsonM.asString() << endl;
         }
         else if (*i == "Poster")
         {
            seriesObj["poster"] = jsonM.asString();
            // cout << "poster: " << jsonM.asString() << endl;
         }
         else if (*i == "Plot")
         {
            seriesObj["plotSummary"] = jsonM.asString();
            epiObj["epSummary"] = jsonM.asString();
            // cout << "Plot: " << jsonM.asString() << endl;
         }
         else if (*i == "Ratings")
         {

            for (Json::Value::ArrayIndex i = 0; i != jsonM.size(); i++)
            {
               if (jsonM[i].isMember("Value"))
               {
                  seriesObj["imdbRating"] = jsonM[i]["Value"].asString();
                  // cout << "Rating: " << jsonM[i]["Value"].asString() << endl;
               }
            }
         }
      }
   }
   else
   {
      cout << "Error parsing seasonInfo string." << endl;
      return false;
   }

   //run through season info
   parseSuccess = reader.parse(seasonInfo, root, false);
   if (parseSuccess)
   {
      // cout << "successful parse" << endl;
      Json::Value::Members mbr = root.getMemberNames();
      for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end(); i++)
      {
         Json::Value jsonM = root[*i];
         Json::Value epiInfo;
         if (*i == "Season")
         {
            seriesObj["seriesSeason"] = jsonM.asString();
            seriesObj["title"] = title + " - Season " + jsonM.asString();
            // cout << "season: " << jsonM.asString() << endl;
         }
         else if (*i == "Episodes")
         {

            for (Json::Value::ArrayIndex i = 0; i != jsonM.size(); i++)
            {
               if (jsonM[i].isMember("Title"))
               {
                  epiObj["name"] = jsonM[i]["Title"].asString();
                  // cout << "Ep Title: " << jsonM[i]["Title"].asString() << endl;
               }
               if (jsonM[i].isMember("imdbRating"))
               {
                  epiObj["imdbRating"] = jsonM[i]["imdbRating"].asString();
                  // cout << "imdbRating: " << jsonM[i]["imdbRating"].asString() << endl;
               }
               seriesObj["episodes"].append(epiObj);
            }
         }
      }
      // cout << "Series so far:  \n"
      //      << seriesObj.toStyledString() << endl;
      SeriesSeason ss(seriesObj);
      addSeries(ss);
      return true;
   }
   else
   {
      cout << "Error parsing seasonInfo string." << endl;
      return false;
   }
}

void MediaLibrary::print()
{

   cout << "\nPRINTING LIBRARY: " << endl;
   for (map<string, SeriesSeason>::const_iterator iter = libraryMap.begin();
        iter != libraryMap.end(); iter++)
   {
      
      SeriesSeason ss = iter->second;
      cout << ss.getTitle() << ": \n";
      cout << ss.toJsonString() << endl;
   }
}
