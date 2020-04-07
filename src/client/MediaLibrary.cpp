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

MediaLibrary::MediaLibrary(){
   initLibraryFromJsonFile("series.json");
}

MediaLibrary::~MediaLibrary() {
   media.clear();
}

bool MediaLibrary::initLibraryFromJsonFile(string jsonFileName){
   bool ret = false;
   Json::Reader reader;
   Json::Value root;
   std::ifstream json(jsonFileName.c_str(), std::ifstream::binary);
   bool parseSuccess = reader.parse(json,root,false);
   if(parseSuccess){
      Json::Value::Members mbr = root.getMemberNames();
      for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
         Json::Value jsonMedia = root[*i];
         SeriesSeason * aDesc = new SeriesSeason(jsonMedia);
         media[*i] = *aDesc;
         cout << "adding ";
         aDesc->print();
      }
      ret = true;
   }
   return ret;
}

bool MediaLibrary::toJsonFile(string jsonFileName){
   bool ret = false;
   Json::Value jsonLib;
   for(std::map<string,SeriesSeason>::iterator i = media.begin();
                                                         i!= media.end(); i++){
      string key = i->first;
      cout << key << " " << endl;
      SeriesSeason aMedia = media[key];
      Json::Value jsonMedia = aMedia.toJson();
      jsonLib[key] = jsonMedia;
   }
   Json::StyledStreamWriter ssw("  ");
   std::ofstream jsonOutFile(jsonFileName.c_str(), std::ofstream::binary);
   ssw.write(jsonOutFile, jsonLib);
   return true;
}

SeriesSeason MediaLibrary::get(string aTitle){
   SeriesSeason aMedia = media[aTitle];
   return aMedia;
}

vector<string> MediaLibrary::getTitles(){
   vector<string> myVec;
   for(map<string,SeriesSeason>::iterator it = media.begin();
                                              it != media.end();++it){
      myVec.push_back(it->first);
   }
   return myVec;
}
