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
 * Copyright 2020 Tim Lindquist,
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
 * @version January 2020
 */

MediaLibrary::MediaLibrary()
{
   // initLibraryFromJsonFile("series.json");
   //TODO: uncomment about
}

MediaLibrary::~MediaLibrary()
{
   libraryMap.clear();
}

bool MediaLibrary::initLibraryFromJsonFile(string jsonFileName)
{ //load from library
   bool ret = false;
   Json::Reader reader;
   Json::Value root;
   std::ifstream json(jsonFileName.c_str(), std::ifstream::binary);
   bool parseSuccess = reader.parse(json, root, false);
   if (parseSuccess)
   {
      Json::Value::Members mbr = root.getMemberNames();
      for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end(); i++)
      {
         Json::Value jsonMedia = root[*i];
         SeriesSeason *aDesc = new SeriesSeason(jsonMedia);
         libraryMap[*i] = *aDesc;
         cout << "adding ";
         aDesc->print();
      }
      ret = true;
   }
   return ret;
}

bool MediaLibrary::toJsonFile(string jsonFileName)
{ //save to library

   bool ret = false;
   Json::Value jsonLib;
   Json::Value library;
   
   map<string, SeriesSeason>::iterator iter = libraryMap.begin();
   for(auto lib: libraryMap) 
   {
      // string key = i->first;
      // cout << key << " " << endl;

      Json::Value seriesObj = iter->second.toJson();
      jsonLib[lib.first] = seriesObj;
      library["series"] = jsonLib;
      iter++;
   }

   Json::Value master;

   int index =- 0;

   for(int i = 0; i < library.size(); i++) {
       master["library"][index++] = library;
   }

   Json::StyledStreamWriter ssw("  ");
   ofstream jsonOutFile(jsonFileName.c_str(), ofstream::binary);
   ssw.write(jsonOutFile, master);
   cout << "The library was saved to the following file: " << jsonFileName << endl;

   return true;




   // bool ret = false;
   // Json::Value jsonLib;
   // int index = -0;
   // for (map<string, SeriesSeason>::iterator i = libraryMap.begin();
   //      i != libraryMap.end(); i++)
   // {

   //    string key = i->first;
   //    cout << key << " " << endl;

   //    SeriesSeason ss = libraryMap[key];
   //    Json::Value seriesObj = ss.toJson();
   //    jsonLib[key] = seriesObj;
   // }
   // Json::Value master;
   // Json::Value library;

   // library["series"] = jsonLib;
   // master["library"] = library;

   // Json::StyledStreamWriter ssw("  ");
   // ofstream jsonOutFile(jsonFileName.c_str(), ofstream::binary);
   // ssw.write(jsonOutFile, master);

   // return true;
}

SeriesSeason MediaLibrary::get(string aTitle)
{
   SeriesSeason ss = libraryMap[aTitle];
   return ss;
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
