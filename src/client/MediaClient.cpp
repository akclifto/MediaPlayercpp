/**
 * Copyright 2020 Tim Lindquist, Adam Clifton 
 * <p>
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
 * Purpose: MediaClient is the controller for the MediaClientGUI to search and 
 * manage the media library through the gui application. 
 * <p>
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @author Adam Clifton akclifto@asu.edu
 *         Software Engineering, ASU
 * @MediaClient.cpp
 * @version April 2020
 */

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Check_Browser.H>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <sstream>
#include <thread>
#include <json/json.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "mediaclientstub.h"
#include "MediaClientGui.cpp"
#include "../server/MediaLibrary.hpp"

using namespace jsonrpc;
using namespace std;

std::string cmd;
void run()
{
   system(cmd.c_str());
}
void testCase(); //for debugging

class MediaClient : public MediaClientGui
{

public:
   string userId;
   string omdbkey;

   thread *playThread;
   mediaclientstub *library;

   /** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */
   static void ClickedX(Fl_Widget *w, void *userdata)
   {
      cout << "You clicked Exit" << endl;
      exit(1);
   }

   /**
    * Static search button callback method.
    */
   static void SearchCallbackS(Fl_Widget *w, void *data)
   {

      MediaClient *o = (MediaClient *)data;
      cout << "Search Clicked. You asked for a OMDb search of Season: " << o->seasonSrchInput->value() << " Series: " << o->seriesSrchInput->value() << endl;
      try
      {
         string url = "https://www.omdbapi.com/?r=json&apikey=";
         url = url + o->omdbkey;

         //Spaces must be converted to %20
         string query = o->seriesSrchInput->value();
         string urlEncodedQuery = "";
         for (int i = 0; i < query.length(); i++)
         {
            if (query.at(i) == ' ')
            {
               urlEncodedQuery += "%20";
            }
            else
            {
               urlEncodedQuery += query.at(i);
            }
         }
         /*
         * Another API call would have to be made here to get
         * the rest of the required information. Same as assignment 2.
         */

         // search and fetch for season/episode info
         url = url + "&t=" + urlEncodedQuery + "&season=" + o->seasonSrchInput->value();
         cout << "sending request url: " << url << endl;
         ostringstream os;
         curlpp::Easy myRequest;
         myRequest.setOpt(new curlpp::options::WriteStream(&os));
         //curlpp::options::Url myUrl(std::string(url));
         myRequest.setOpt(new curlpp::options::Url(url.c_str()));
         myRequest.perform();

         string seasonInfo = os.str();
         cout << "Season info: " << seasonInfo << endl;

         //search and fetch for series info
         string url2 = "https://www.omdbapi.com/?r=json&apikey=";
         url2 += o->omdbkey;
         url2 += "&t=" + urlEncodedQuery;
         cout << "sending request url: " << url2 << endl;

         ostringstream os2;
         myRequest.reset();
         myRequest.setOpt(new curlpp::options::WriteStream(&os2));
         myRequest.setOpt(new curlpp::options::Url(url2.c_str()));
         myRequest.perform();

         string seriesInfo = os2.str();
         cout << "Series info: " << seriesInfo << endl;

         // Get out of static method and parse URL info
         o->searchCallBack(seriesInfo, seasonInfo);
      }
      catch (curlpp::LogicError &e)
      {
         cout << e.what() << endl;
      }
      catch (curlpp::RuntimeError &e)
      {
         cout << e.what() << endl;
      }
   }

   /**
    * Method to parse search call back information regarding series and season search
    * results.
    * @param seriesInfo : string containing series information in JSON format
    * @param seasonInfo : string containing season information in JSON format
    * @return void.
    * */
   void searchCallBack(string seriesInfo, string seasonInfo)
   {

      bool flag = library->parseURLtoJSON(seriesInfo, seasonInfo);
      if (flag)
      {
         buildTree();
      }
      else
      {
         cout << "Error parsing URL call back." << endl;
      }
   }

   // Static menu callback method
   static void TreeCallbackS(Fl_Widget *w, void *data)
   {
      MediaClient *o = (MediaClient *)data;
      o->TreeCallback(); //call the instance callback method
   }

   /**
    * TreeCallback is a callback for tree selections, deselections, expand or
    * collapse.
    */
   void TreeCallback()
   {
      // Find item that was clicked
      Fl_Tree_Item *item = (Fl_Tree_Item *)tree->item_clicked();
      cout << "Tree callback. Item selected: ";
      if (item)
      {
         cout << item->label();
      }
      else
      {
         cout << "none";
      }
      cout << endl;
      string aStr("unknown");
      string aTitle(item->label());
      switch (tree->callback_reason())
      { // reason callback was invoked
      case FL_TREE_REASON_NONE:
      {
         aStr = "none";
         break;
      }
      case FL_TREE_REASON_OPENED:
      {
         aStr = "opened";
         break;
      }
      case FL_TREE_REASON_CLOSED:
      {
         aStr = "closed";
         break;
      }
      case FL_TREE_REASON_SELECTED:
      {

         aStr = "selected";
         if (library)
         {
            cout << "trying to get: " << item->label() << endl;
         }
         else
         {
            cout << "library entry not found" << endl;
            break;
         }
         if (item->depth() == 0 || item->depth() == 1)
         {

            aStr = "Library selected";

            if (item->depth() == 0)
            {
               item->label(userId.c_str());
            }
            seriesSeasonInput->value("Season Number");
            genreInput->value("Genre");
            episodeInput->value("Episode Name");
            ratingInput->value("IMDB Rating");
            summaryMLI->value("Plot Summary");
            break;
         }
         else if (item->depth() == 2)
         {
            string seriesName = library->getSeriesTitle(item->label());

            setImagefromURL(library->getSeriesPoster(seriesName));

            int epCount = library->getEpisodeListSize(seriesName);
            string epInput;
            if (epCount == 1)
            {
               epInput = " " + to_string(epCount) + " Episode in Library";
               episodeInput->value(epInput.c_str());
            }
            else
            {
               epInput = " " + to_string(epCount) + " Episodes in Library";
               episodeInput->value(epInput.c_str());
            }

            seriesSeasonInput->value(seriesName.c_str());
            genreInput->value(library->getSeriesGenre(seriesName).c_str());
            ratingInput->value(library->getSeriesImdbRating(seriesName).c_str());
            summaryMLI->value(library->getSeriesSummary(seriesName).c_str());
            break;
         }
         else if (item->depth() == 3)
         {

            Fl_Tree_Item *parent = item->parent();
            string parentLabel = parent->label();
            Episode epi = library->getEpisode(parentLabel, item->label());
            string seriesTitle = library->getSeriesTitle(parentLabel);
            setImagefromURL(library->getSeriesPoster(parentLabel));

            seriesSeasonInput->value(seriesTitle.c_str());
            genreInput->value(library->getSeriesGenre(parentLabel).c_str());
            episodeInput->value(epi.getName().c_str());
            ratingInput->value(epi.getImdbRating().c_str());
            summaryMLI->value(epi.getEpSummary().c_str());
            break;
         }
      }
      case FL_TREE_REASON_DESELECTED:
      {
         aStr = "deselected\n";
         break;
      }
      default:
      {
         break;
      }
      }
      cout << "Callback reason: " << aStr.c_str() << endl;
   }

   /**
    * Method to download image from URL and set to imageBox in the gui display
    * @param urlString : a string of url source for the image.
    * @return void.
    * */
   void setImagefromURL(string urlString)
   {
      int n = urlString.length();

      try
      {

         ostringstream stream;
         curlpp::Easy request;

         request.setOpt(new curlpp::options::WriteStream(&stream));
         request.setOpt(new curlpp::options::Url(urlString.c_str()));
         request.perform();
         string strOut = stream.str();

         std::ofstream imgstream;
         imgstream.open("seriesImage.jpg");
         imgstream << strOut;
         imgstream.close();
         jpgImage = new Fl_JPEG_Image("seriesImage.jpg");

         imageBox->image(jpgImage);
         imageBox->redraw();
      }
      catch (curlpp::LogicError &e)
      {
         cout << e.what() << endl;
      }
      catch (curlpp::RuntimeError &e)
      {
         cout << e.what() << endl;
      }
   }

   // Static menu callback method
   static void Menu_ClickedS(Fl_Widget *w, void *data)
   {
      MediaClient *o = (MediaClient *)data;
      o->Menu_Clicked(); //call the instance callback method
   }

   // Menu selection instance method that has ccess to instance vars.
   void Menu_Clicked()
   {
      char picked[80];
      menubar->item_pathname(picked, sizeof(picked) - 1);
      string selectPath(picked);
      cout << "Selected Menu Path: " << selectPath << endl;
      // Handle menu selections
      if (selectPath.compare("File/Save") == 0)
      {
         bool restSave = library->toJsonFile("series.json");
         if (restSave)
         {
            cout << "Save Successful" << endl;
         }
         else
         {
            cout << "Save unsuccessful" << endl;
         }
      }
      else if (selectPath.compare("File/Restore") == 0)
      {
         bool flag = library->initLibraryFromJsonFile("series.json");
         if (flag)
         {
            buildTree();
            cout << "Restore Successful" << endl;
         }
         else
         {
            cout << "Restore Unsuccessful" << endl;
         }
      }
      else if (selectPath.compare("File/Tree Refresh") == 0)
      {
         buildTree();
      }
      else if (selectPath.compare("File/Exit") == 0)
      {
         if (playThread && playThread->joinable())
         {
            playThread->join();
         }
         exit(0);
      }
      else if (selectPath.compare("Series-Season/Add") == 0)
      {

         fetchURLAddSeries();
      }
      else if (selectPath.compare("Series-Season/Remove") == 0)
      {
         actionRemoveSeries();
      }
      else if (selectPath.compare("Episode/Add") == 0)
      {

         actionAddEpisode();
      }
      else if (selectPath.compare("Episode/Remove") == 0)
      {

         actionRemoveEpisode();
      }
   }

   void actionRemoveSeries()
   {
      bool flag = library->removeSeries(seriesSeasonInput->value());
      if (flag)
      {
         buildTree();
      }
      else
      {
         cout << "Error removing series from library." << endl;
      }
   }

   void actionAddEpisode()
   {
      //TODO
      bool flag = true;
      if (flag)
      {
         buildTree();
         cout << "Episode added to library." << endl;
      }
      else
      {
         cout << "Error adding episode to library." << endl;
      }
   }

   void actionRemoveEpisode()
   {
      //TODO: episode not removing, changes not holding.
      bool flag = library->removeEpisode(seriesSeasonInput->value(), episodeInput->value());
      if (flag)
      {
         buildTree();
         cout << "Episode removed from library." << endl;
      }
      else
      {
         cout << "Error removing episode from library." << endl;
      }
   }

   void fetchURLAddSeries()
   {

      cout << "Search Clicked. You asked for a OMDb search of Season: "
           << seasonSrchInput->value() << " Series: "
           << seriesSrchInput->value() << endl;
      try
      {
         string url = "https://www.omdbapi.com/?r=json&apikey=";
         url = url + omdbkey;

         //Spaces must be converted to %20
         string query = seriesSrchInput->value();
         string urlEncodedQuery = "";
         for (int i = 0; i < query.length(); i++)
         {
            if (query.at(i) == ' ')
            {
               urlEncodedQuery += "%20";
            }
            else
            {
               urlEncodedQuery += query.at(i);
            }
         }
         /*
         * Another API call would have to be made here to get
         * the rest of the required information. Same as assignment 2.
         */

         // search and fetch for season/episode info
         url = url + "&t=" + urlEncodedQuery + "&season=" + seasonSrchInput->value();
         cout << "sending request url: " << url << endl;
         ostringstream os;
         curlpp::Easy myRequest;
         myRequest.setOpt(new curlpp::options::WriteStream(&os));
         //curlpp::options::Url myUrl(std::string(url));
         myRequest.setOpt(new curlpp::options::Url(url.c_str()));
         myRequest.perform();

         string seasonInfo = os.str();
         cout << "Season info: " << seasonInfo << endl;

         //search and fetch for series info
         string url2 = "https://www.omdbapi.com/?r=json&apikey=";
         url2 += omdbkey;
         url2 += "&t=" + urlEncodedQuery;
         cout << "sending request url: " << url2 << endl;

         ostringstream os2;
         myRequest.reset();
         myRequest.setOpt(new curlpp::options::WriteStream(&os2));
         myRequest.setOpt(new curlpp::options::Url(url2.c_str()));
         myRequest.perform();

         string seriesInfo = os2.str();
         cout << "Series info: " << seriesInfo << endl;

         // Get out of static method and parse URL info
         searchCallBack(seriesInfo, seasonInfo);
      }
      catch (curlpp::LogicError &e)
      {
         cout << e.what() << endl;
      }
      catch (curlpp::RuntimeError &e)
      {
         cout << e.what() << endl;
      }
   }

   /**
    * a static method to remove spaces, tabs, new lines and returns from the
    * begining or end of a string.
    */
   static string &trimMe(string &str)
   {
      // right trim
      while (str.length() > 0 && (str[str.length() - 1] == ' ' ||
                                  str[str.length() - 1] == '\t' ||
                                  str[str.length() - 1] == '\n' ||
                                  str[str.length() - 1] == '\r'))
      {
         str.erase(str.length() - 1, 1);
      }
      // left trim
      while (str.length() > 0 && (str[0] == ' ' || str[0] == '\t'))
      {
         str.erase(0, 1);
      }
      return str;
   }

   /**
    * a method to execute a command line command and to return
    * the resulting string.
    */
   string exec(const char *cmd)
   {
      FILE *pipe = popen(cmd, "r");
      if (!pipe)
         return "ERROR";
      char buffer[128];
      string result = "";
      while (!feof(pipe))
      {
         if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
      }
      pclose(pipe);
      return result;
   }

   void buildTree()
   {
      Json::Value libTitles = library->getLibraryTitles();
      vector<string> seriesTitles;
      for (const auto &index : libTitles)
      {
         seriesTitles.push_back(index.asString());
      }
      // cout << "Server has titles: \n";
      tree->clear();

      for (const auto &res : seriesTitles)
      {

         string series = library->getSeriesTitle(res);
         Json::Value epTitleRes = library->getEpisodeTitles(res);
         vector<string> epTitles;
         for (const auto &index : epTitleRes)
         {
            epTitles.push_back(index.asString());
         }

         string st = "Library/" + series + "/";
         for (const auto &ep : epTitles)
         {
            string fin = st + library->getEpisodeName(series, ep);
            tree->add(fin.c_str());
         }
      }
      cout << endl;
      tree->redraw();
   }

   MediaClient(const char *name = "Adam", const char *key = "myKey", mediaclientstub *stub = NULL) : MediaClientGui(name)
   {
      searchButt->callback(SearchCallbackS, (void *)this);
      menubar->callback(Menu_ClickedS, (void *)this);
      tree->callback(TreeCallbackS, (void *)this);
      callback(ClickedX);
      omdbkey = key;
      userId = "Adam.Clifton";
      library = stub;
      buildTree();
   }
};

int main(int argc, char *argv[])
{
   //connect to server
   string host = (argc > 1) ? argv[1] : "http://127.0.0.1";
   string port = (argc > 2) ? argv[2] : "8888";
   cout << "Connecting to host" << host << ":" << port << endl;

   string developer = (argc > 1) ? argv[3] : "Adam.Clifton";
   string omdbkey = (argc > 2) ? argv[4] : "omdbkey";
   string windowTitle = developer + "'s SeriesSeason Browser";

   HttpClient client(host + ":" + port);
   mediaclientstub stub(client);

   MediaClient cm(windowTitle.c_str(), omdbkey.c_str(), &stub);
   return (Fl::run());
}
