#include "MediaLibrary.hpp"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
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

               // cout << *input << endl;
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

   for (auto lib : libraryMap)
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
   SeriesSeason ss = libraryMap[aTitle];
   return ss;
}

map<string, SeriesSeason> MediaLibrary::getLibrary()
{
   return libraryMap;
}

void MediaLibrary::addSeries(SeriesSeason seriesSeason)
{

   bool flag = false;
   for (auto ss : libraryMap)
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
      this->libraryMap.insert(pair<string, SeriesSeason>(seriesSeason.getTitle(), seriesSeason));
      this->SeriesSeasonList.insert(pair<string, SeriesSeason>(seriesSeason.getTitle(), seriesSeason));
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

   for (auto ss : libraryMap)
   {
      if (ss.second.getTitle() == title)
      {
         libraryMap.erase(ss.second.getTitle());
         cout << ss.second.getTitle() << " was removed from the library."
              << "\n\n";
         return true;
      }
   }

   cout << title << " was not found in the library!" << endl;
   return false;
}

vector<string> MediaLibrary::getTitles()
{
   vector<string> myVec;
   for (map<string, SeriesSeason>::iterator it = libraryMap.begin();
        it != libraryMap.end(); ++it)
   {
      myVec.push_back(it->first);
   }
   return myVec;
}

void MediaLibrary::parseURLtoJSON()
{
   //TODO:  this.
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
