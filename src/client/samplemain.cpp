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
void run(){
   system(cmd.c_str());
}

/**
 * Copyright (c) 2020 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Music management.
 * This class extends the Gui component class MediaClientGui and demonstrates
 * sample control functions that respond to button clicks and menu item selects.
 * This software is meant to run on Linux and MacOS using g++.
 * Students may use this source code as the basis for their solution to
 * the programming projects in Ser321 to create a distributed series-season browser.
 * <p/>
 * This example depends on the library curlpp to download a file or make
 * a URL request. This should be used in problem solutions to search
 * OMDb for series, season and episode information. Download the library from:
 *   http://www.curlpp.org/
 * Extract the archive and from a terminal change to the directory created.
 * Execute the following commands looking for errors. Note the following steps
 * require that libcurl first be installed, if its not already present.
 *   cmake ./
 *   make
 *   sudo make install
 *   sudo ldconfig
 * To see the make options available do: make help
 * you may want to note the files that are installed by make install since there
 * does not appear to be any "uninstall" target in the makefile.
 *
 * To use this library, the compile and link must reference the appropriate
 * libraries:
 * g++ -std=c++14 SeriesSeasonBrowser.cpp -o exampleCurlPP -lstdc++ -ljsoncpp -lcurlpp -lcurl
 * If the includes and libraries are not installed in a location known to g++, then
 * the command will need -I and -L options which point g++ to the include and
 * library files respectively.
 *
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    samplemain.cpp
 * @date    January, 2020
 **/
class MediaClient : public MediaClientGui {

public:

   std::string userId;
   std::string omdbkey;

   std::thread * playThread;
   MediaLibrary * library;

/** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */
   static void ClickedX(Fl_Widget * w, void * userdata) {
      std::cout << "You clicked Exit" << std::endl;
      exit(1);
   }

   /**
    * Static search button callback method.
    */
   static void SearchCallbackS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      cout << "Search Clicked. You asked for a OMDb search of Season: " <<
         o->seasonSrchInput->value() << " Series: " <<
         o->seriesSrchInput->value() << endl;
      try{
         std::string url = "https://www.omdbapi.com/?r=json&apikey=";
         url = url + o->omdbkey;

         //Spaces must be converted to %20
         string query = o->seriesSrchInput->value();
         string urlEncodedQuery = "";
         for (int i = 0; i < query.length(); i++) {
            if (query.at(i) == ' ') {
               urlEncodedQuery += "%20";
            } else {
               urlEncodedQuery += query.at(i);
            }
         }
         /*
         * Another API call would have to be made here to get
         * the rest of the required information. Same as assignment 2.
         */
         url = url + "&t=" + urlEncodedQuery + "&season=" + o->seasonSrchInput->value();
         cout << "sending request url: " << url << endl;
         std::ostringstream os;
         curlpp::Easy myRequest;
         myRequest.setOpt(new curlpp::options::WriteStream(&os));
         //curlpp::options::Url myUrl(std::string(url));
         myRequest.setOpt(new curlpp::options::Url(url.c_str()));
         myRequest.perform();
         std::string aString = os.str();
         std::cout << aString << std::endl;
      }catch ( curlpp::LogicError & e ) {
         std::cout << e.what() << std::endl;
      }
      catch ( curlpp::RuntimeError & e ) {
         std::cout << e.what() << std::endl;
      }
   }

   // Static menu callback method
   static void TreeCallbackS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      o->TreeCallback(); //call the instance callback method
   }

   /**
    * TreeCallback is a callback for tree selections, deselections, expand or
    * collapse.
    */
   void TreeCallback() {
      // Find item that was clicked
      Fl_Tree_Item *item = (Fl_Tree_Item*)tree->item_clicked();
      cout << "Tree callback. Item selected: ";
      if ( item ) {
         cout << item->label();
      } else {
         cout << "none";
      }
      cout << endl;
      std::string aStr("unknown");
      std::string aTitle(item->label());
      switch ( tree->callback_reason() ) {  // reason callback was invoked
      case       FL_TREE_REASON_NONE: {aStr = "none"; break;}
      case     FL_TREE_REASON_OPENED: {aStr = "opened";break;}
      case     FL_TREE_REASON_CLOSED: {aStr = "closed"; break;}
      case   FL_TREE_REASON_SELECTED: {
         aStr = "selected";
         SeriesSeason md;
         if(library){
            cout << "trying to get: " << item->label() << endl;
            md = library->get(aTitle);
         }else{
            cout << "library entry not found" << endl;
            break;
         }
         cout << "media: "<< md.title << " " << md.seriesSeason << " "
              << md.rating << " " << md.genre << " " << md.imageURL
              << endl;
         episodeInput->value(md.title.c_str());
         seriesSeasonInput->value(md.seriesSeason.c_str());
         ratingInput->value(md.rating.c_str());
         genreInput->value(md.genre.c_str());
         break;
      }
      case FL_TREE_REASON_DESELECTED: {aStr = "deselected"; break;}
      default: {break;}
      }
      cout << "Callback reason: " << aStr.c_str() << endl;
   }

   // Static menu callback method
   static void Menu_ClickedS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      o->Menu_Clicked(); //call the instance callback method
   }

   // Menu selection instance method that has ccess to instance vars.
   void Menu_Clicked() {
      char picked[80];
      menubar->item_pathname(picked, sizeof(picked)-1);
      string selectPath(picked);
      cout << "Selected Menu Path: " << selectPath << endl;
      // Handle menu selections
      if(selectPath.compare("File/Save")==0){
         bool restSave = library->toJsonFile("series.json");
         cout << "Save not implemented" << endl;
      }else if(selectPath.compare("File/Restore")==0){
         cout << "Restore not implemented" << endl;
      }else if(selectPath.compare("File/Tree Refresh")==0){
         buildTree();
      }else if(selectPath.compare("File/Exit")==0){
         if(playThread && playThread->joinable()){
            playThread->join();
         }
         exit(0);
      }else if(selectPath.compare("Series-Season/Add")==0){
         cout << "Add not implemented" << endl;
      }else if(selectPath.compare("Series-Season/Remove")==0){
         cout << "Remove not implemented" << endl;
      }
   }

   /**
    * a static method to remove spaces, tabs, new lines and returns from the
    * begining or end of a string.
    */
   static std::string& trimMe (std::string& str) {
      // right trim
      while (str.length() > 0 && (str[str.length()-1] == ' '  ||
                                  str[str.length()-1] == '\t' ||
                                  str[str.length()-1] == '\n' ||
                                  str[str.length()-1] == '\r')){
         str.erase(str.length ()-1, 1);
      }
      // left trim
      while (str.length () > 0 && (str[0] == ' ' || str[0] == '\t')){
         str.erase(0, 1);
      }
      return str;
   }

   /**
    * a method to execute a command line command and to return
    * the resulting string.
    */
   std::string exec(const char* cmd) {
      FILE* pipe = popen(cmd, "r");
      if (!pipe) return "ERROR";
      char buffer[128];
      std::string result = "";
      while(!feof(pipe)) {
         if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
      }
      pclose(pipe);
      return result;
   }

   void buildTree(){
      vector<string> result = library->getTitles();
      cout << "server has titles";
      tree->clear();
      for(int i=0; i<result.size(); i++){
         cout << " " << result[i];
         SeriesSeason md = library->get(result[i]);
         cout << md.title << " " << md.seriesSeason << " " << md.rating
              << " " << md.genre << endl;
      }
      cout << endl;
      tree->redraw();
   }

   MediaClient(const char * name = "Tim", const char * key = "myKey") : MediaClientGui(name) {
      searchButt->callback(SearchCallbackS, (void*)this);
      menubar->callback(Menu_ClickedS, (void*)this);
      tree->callback(TreeCallbackS, (void*)this);
      callback(ClickedX);
      omdbkey = key;
      userId = "Tim.Lindquist";
      library = new MediaLibrary();
      buildTree();
   }
};

int main(int argc, char * argv[]) {
   std::string developer = (argc>1)?argv[1]:"Tim.Lindquist";
   std::string omdbkey = (argc>2)?argv[2]:"omdbkey";
   std::string windowTitle = developer + "'s SeriesSeason Browser";
   MediaClient cm(windowTitle.c_str(),omdbkey.c_str());
   return (Fl::run());
}
