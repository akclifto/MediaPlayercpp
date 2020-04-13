#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Tree_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

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
 * Purpose: Sample C++ FLTK view class. MediaClientGui constructs the view
 * for media app. This class is extended by the client controller which is
 * the MediaClient class. MediaClient defines the call-backs for UI controls.
 * It contains sample control functions that respond to button clicks and tree
 * selects.
 * This software is meant to run on Debian Wheezy Linux
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @author Adam Clifton akclifto@asu.edu
 *         Software Engineering, ASU
 * @file MediaClientGui.cpp
 * @version April 2020
 **/
class MediaClientGui : public Fl_Window
{
protected:
   Fl_Box *imageBox;
   Fl_JPEG_Image *jpgImage;

   /**
    * tree is the Fl_Tree object that occupies the left side of the window.
    * this tree control provides the ability to add and remove items and to
    * manipulate and query the tree when an exception occurs.
    */
   Fl_Tree *tree;

   /**
    * episodeInput is the Fl_Input object labelled Episode
    * Its to display the SeriesSeasons's Episode tile.
    */
   Fl_Input *episodeInput;

   /**
    * seriesSeasonInput is the Fl_Input object labelled SeriesSeason
    * Its to display the SeriesSeasons's tile.
    */
   Fl_Input *seriesSeasonInput;

   /**
    * genreInput is the Fl_Input object labelled Genre.
    * It provides for the display of the SeariesSeason's genre.
    */
   Fl_Input *genreInput;

   /**
    * summaryMLI is the Fl_Multiline_Input object in the lower right panel.
    * It provides for the display of a SeriesSeasons's summary.
    */
   Fl_Multiline_Input *summaryMLI;

   /**
    * ratingInput is the Fl_Input object labelled IMDB Rating.
    * It provides for the display of the IMDB rating for the SeriesSeason.
    */
   Fl_Input *ratingInput;

   /**
    * seasonSrchInput is the Fl_Input object labelled 'Which Season?' in the left panel.
    * It provides entry for Season OMDb search.
    */
   Fl_Input *seasonSrchInput;

   /**
    * seriesSrchInput is the Fl_Input object labelled 'Which Series?'.
    * It provides entry for Series OMDb search.
    */
   Fl_Input *seriesSrchInput;

   /**
    * searchButt is the Fl_Button object labelled Search.
    * After the user enters series (seasonSrchInput) and season (seriesSrchInput)
    * information, the searchButt is clicked to initiate a OMDb SeriesSeason
    * search. The results of the search are parsed and displayed in the tree.
    */
   Fl_Button *searchButt;

   /**
    * menubar is the Fl_Menu_bar object with menus: File and Series-Season
    */
   Fl_Menu_Bar *menubar;

public:
   //MediaClientGui(const char * name = "Ser321") : Fl_Window(635,350,name) {
   MediaClientGui(const char *name = "SER321") : Fl_Window(980, 540, name)
   {
      begin();

      menubar = new Fl_Menu_Bar(0, 0, this->w(), 25);
      menubar->add("File/Save");
      menubar->add("File/Restore");
      menubar->add("File/Tree Refresh");
      menubar->add("File/Exit");
      menubar->add("Series-Season/Add");
      menubar->add("Series-Season/Remove");
      menubar->add("Episode/Add");
      menubar->add("Episode/Remove");

      seasonSrchInput = new Fl_Input(115, 35, 110, 25);
      seasonSrchInput->label("Which Season?");
      seasonSrchInput->value("1");

      seriesSrchInput = new Fl_Input(110, 80, 115, 25);
      seriesSrchInput->label("Which Series?");
      seriesSrchInput->value("IT Crowd");

      searchButt = new Fl_Button(230, 80, 90, 25, "Search");

      // create a tree control at position x=10, y=10. Its 150 pixels wide
      // and window height less 20 pixels high. Add some sample tree nodes.
      tree = new Fl_Tree(10, 120, 325, this->h() - 135);
      // tree->add("Flintstones/Fred");
      // tree->add("Flintstones/Wilma");
      // tree->close("/Flintstones");

      /*
       * add a text input control at x=250, y=35 of width 200 pixels and
       * height of 25 pixels. Initialize it contents to media title.
       */
      /*
       * add a text input control at x=250, y=35 of width 200 pixels and
       * height of 25 pixels. Initialize it contents to media title.
       */
      seriesSeasonInput = new Fl_Input(450, 35, 175, 25);
      seriesSeasonInput->label("Series-Season");
      seriesSeasonInput->value("");
      seriesSeasonInput->readonly(1);

      genreInput = new Fl_Input(700, 35, 265, 25);
      genreInput->label("Genre");
      genreInput->value("");
      genreInput->readonly(1);

      episodeInput = new Fl_Input(405, 80, 220, 25);
      episodeInput->label("Episode");
      episodeInput->value("");
      episodeInput->readonly(1);

      ratingInput = new Fl_Input(750, 80, 175, 25);
      ratingInput->label("IMDB Rating");
      ratingInput->value("");
      ratingInput->readonly(1);

      imageBox = new Fl_Box(350, 145, 320, 380);
      jpgImage = new Fl_JPEG_Image("default.jpg");
      imageBox->image(jpgImage); 

      summaryMLI = new Fl_Multiline_Input(665, 180, 300, 300, 0);
      summaryMLI->wrap(1);
      summaryMLI->value("");
      summaryMLI->readonly(1);
      
      end();
      show();
   }
};
