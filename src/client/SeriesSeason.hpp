#include <string>
#include <cmath>
#include <json/json.h>

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
 * @version April 2020
 */
class SeriesSeason {
protected:
public:

   string title;
   string seriesSeason;
   string rating;
   string genre;
   string imageURL;

   SeriesSeason();
   SeriesSeason(string aTitle, string aSeriesSeason,
       string aRating, string aGenre, string anImageURL);
   SeriesSeason(const Json::Value& jsonObj);
   SeriesSeason(string jsonString);
   ~SeriesSeason();
   string toJsonString();
   Json::Value toJson();
   void fromJson(Json::Value json);
   void setValues(string aTitle, string aSeriesSeason,
       string aRating, string aGenre, string anImageURL);
   void print();
};
