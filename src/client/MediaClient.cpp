#include "MediaClientGui.cpp"
#include "MediaLibrary.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Check_Browser.H>
#include <stdio.h>
#include <iostream>
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

using namespace std;

std::string cmd;
void run()
{
   system(cmd.c_str());
}
void testCase(); //TODO: be sure to remove this

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
class MediaClient : public MediaClientGui
{

public:
   string userId;
   string omdbkey;

   thread *playThread;
   MediaLibrary *library;

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
         std::string url = "https://www.omdbapi.com/?r=json&apikey=";
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
         url = url + "&t=" + urlEncodedQuery + "&season=" + o->seasonSrchInput->value();
         cout << "sending request url: " << url << endl;
         ostringstream os;
         curlpp::Easy myRequest;
         myRequest.setOpt(new curlpp::options::WriteStream(&os));
         //curlpp::options::Url myUrl(std::string(url));
         myRequest.setOpt(new curlpp::options::Url(url.c_str()));
         myRequest.perform();
         string aString = os.str();
         cout << aString << endl;
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
            SeriesSeason md;
            if (library)
            {
               cout << "trying to get: " << item->label() << endl;
               md = library->get(aTitle);
            }
            else
            {
               cout << "library entry not found" << endl;
               break;
            }
            cout << "media: " << md.getTitle() << " " << md.getSeriesSeason() << " "
               << md.getImdbRating() << " " << md.getGenre() << " " << md.getPoster()
               << endl;
            episodeInput->value(md.getTitle().c_str());
            seriesSeasonInput->value(md.getSeriesSeason().c_str());
            ratingInput->value(md.getImdbRating().c_str());
            genreInput->value(md.getGenre().c_str());
            break;
         }
         case FL_TREE_REASON_DESELECTED:
         {
            aStr = "deselected";
            break;
         }
         default:
         {
            break;
         }
      }
      cout << "Callback reason: " << aStr.c_str() << endl;
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
         cout << "Save not implemented" << endl;
      }
      else if (selectPath.compare("File/Restore") == 0)
      {
         cout << "Restore not implemented" << endl;
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
         cout << "Add not implemented" << endl;
      }
      else if (selectPath.compare("Series-Season/Remove") == 0)
      {
         cout << "Remove not implemented" << endl;
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
      vector<string> result = library->getTitles();
      cout << "Server has titles: \n";
      tree->clear();
      for (int i = 0; i < result.size(); i++)
      {
         cout << result[i];
         SeriesSeason md = library->get(result[i]);
         cout << " " << md.getTitle() << " " << md.getSeriesSeason()
              << " " << md.getImdbRating()
              << " " << md.getGenre() << endl;
      }
      cout << endl;
      tree->redraw();
   }

   MediaClient(const char *name = "Tim", const char *key = "myKey") : MediaClientGui(name)
   {
      searchButt->callback(SearchCallbackS, (void *)this);
      menubar->callback(Menu_ClickedS, (void *)this);
      tree->callback(TreeCallbackS, (void *)this);
      callback(ClickedX);
      omdbkey = key;
      userId = "Adam.Clifton";
      library = new MediaLibrary();
      //buildTree();  //TODO:
   }
};

int main(int argc, char *argv[])
{
   string developer = (argc > 1) ? argv[1] : "Adam.Clifton";
   string omdbkey = (argc > 2) ? argv[2] : "omdbkey";
   string windowTitle = developer + "'s SeriesSeason Browser";
   MediaClient cm(windowTitle.c_str(), omdbkey.c_str());
   return (Fl::run());
}

/**
 * Method for testing and debugging. 
 */
void testCase()
{

   MediaLibrary ml;
   ml.toJsonFile("test.json");
   ml.initLibraryFromJsonFile("test.json");
   ml.print();

   //initial debugging
   SeriesSeason s1("series 1", "12", "10/10", "action", "link.com", "this is details");
   SeriesSeason s2("series 2", "10", "05/10", "comedy", "website.com", "some plot details");
   Episode eps1("eps 1", "5.0", "Epi plot");
   Episode eps2("eps 2", "5.0", "Epi plot second");
   Episode eps3("Series 2 ep 1", "3.0", "Epi plotting");
   s1.addEpisode(eps1);
   s1.addEpisode(eps2);
   s2.addEpisode(eps3);
   int size = s1.getEpisodeList().size();
   size = s1.getEpisodeList().size();
   cout << "S1 epi size: " << s1.getEpisodeList().size() << endl;
   MediaLibrary mll;
   mll.addSeries(s1);
   mll.addSeries(s2);
   mll.addSeries(s1);

   cout << mll.toJsonFile("test.json") << endl;
   mll.initLibraryFromJsonFile("test.json");
   cout << "size of the libMap: " << mll.getLibrary().size();
   mll.print();
   cout << mll.toJsonFile("OUTPUT.json") << endl;
   return;
}
