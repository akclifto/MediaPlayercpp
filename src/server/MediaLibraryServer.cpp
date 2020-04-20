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
 * Purpose: MediaLibrary Server is the server used for the client application for
 * the Media Library client and GUI applicaton.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @author Adam Clifton akclifto@asu.edu
 *         Software Engineering, ASU
 * @file MediaLibraryServer.cpp
 * @version April 2020
 */

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
// the include below requires that you've built the jsonrpccpp package manually
// with the switchs as follows:
// cmake ../ -DTCP_SOCKET_SERVER=YES -DTCP_SOCKET_CLIENT=YES
// make
// make doc    // this will build html docs in the build/doc/html directory
// sudo make install
//#include <jsonrpccpp/server/connectors/tcpsocketserver.h>
#include <json/json.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>

#include "mediaserverstub.h"
#include "MediaLibrary.hpp"

using namespace jsonrpc;
using namespace std;

class MediaLibraryServer : public mediaserverstub
{

private:
    MediaLibrary *library;
    int portNum;

protected:
public:
    MediaLibraryServer(AbstractServerConnector &connector, int portNum);
    ~MediaLibraryServer();
    virtual string serviceInfo();
    virtual bool toJsonFile(const string &filename);
    virtual bool initLibraryFromJsonFile(const string &filename);
    virtual bool parseURLtoJSON(const string &seriesInfo, const string &seasonInfo);
    virtual bool removeSeries(const string &seriesName);
    virtual bool addEpisode(const string &seriesName, const string &episodeName);
    virtual bool removeEpisode(const string &seriesName, const string &episodeName);
    virtual Json::Value getSeries(const string &seriesName);
    virtual Json::Value getEpisode(const string &seriesName, const string &episodeName);
    virtual int getEpisodeListSize(const string &seriesName);
    virtual Json::Value getLibraryTitles();
    virtual Json::Value getEpisodeTitles(const string &seriesName);
    virtual string getSeriesTitle(const string &itemLabel);
    virtual string getSeriesPoster(const string &seriesName);
    virtual string getSeriesGenre(const string &seriesName);
    virtual bool checkSeriesExists(const string &itemLabel);
    virtual string getSeriesImdbRating(const string &itemLabel);
    virtual string getSeriesSummary(const string &itemLabel);
    virtual string getEpisodeName(const string &seriesName, const string &episodeName);
    virtual string getEpisodeImdb(const string &seriesName, const string &episodeName);
    virtual string getEpisodeSummary(const string &seriesName, const string &episodeName);
};

/**
 * Constructor
 * */
MediaLibraryServer::MediaLibraryServer(AbstractServerConnector &connector, int port) : mediaserverstub(connector)
{

    library = new MediaLibrary();
    portNum = port;
}

/**
 * Destructor
 * */
MediaLibraryServer::~MediaLibraryServer()
{
    delete library;
}

/**
 * Method to provide server service information to the client.  
 * @return message of server service information.
 * */
string MediaLibraryServer::serviceInfo()
{
    string msg = "Media Library management server.";
    stringstream ss;
    ss << portNum;
    cout << "serviceInfo called. Returning: " << msg << endl;
    return msg.append(ss.str());
}

/**
 * Method to save library to file using JsonRPC.
 * @param filename : name of the file to be saved to the library. 
 * @return true if file saved successfully, false otherwise.
 * */
bool MediaLibraryServer::toJsonFile(const string &filename)
{
    cout << "Saving media library to file: " << filename << endl;
    ;
    return library->toJsonFile(filename);
}

/**
 * Method to restore media library from file usin JsonRPC.
 * @param filename : name of file/filepath to restore library from file.
 * @return true if library restored successfully, false otherwise.
 * */
bool MediaLibraryServer::initLibraryFromJsonFile(const string &filename)
{
    cout << "Restoring library from file: " << filename << endl;
    return library->initLibraryFromJsonFile(filename);
}

/**
 * Method to add series to library from omdb search results using JsonRPC.
 * @param seriesInfo : information about the series searched
 * @param seasonInfo : information about the season searched
 * @return true if added to library successfully, false otherwise
 * */
bool MediaLibraryServer::parseURLtoJSON(const string &seriesInfo, const string &seasonInfo)
{
    cout << "Adding new series to library" << endl;
    return library->parseURLtoJSON(seriesInfo, seasonInfo);
}

/**
 * Method to remove series from library using the GUI top-menu functionality,
 * with JsonRPC.
 * @param seriesName : name of the series to remove.
 * @return true if series is removed  successfully, false otherwise
 * */
bool MediaLibraryServer::removeSeries(const string &seriesName)
{
    cout << "Removing " << seriesName << " from the Library.\n";
    return library->removeSeries(seriesName);
}

/**
 * Method to add episode to the library using the GUI to-p-menu functionality, 
 * with JsonRPC
 * @param seriesName : name of the series containing episode to add
 * @param episodeName : name of the episode to add to library.
 * @return true if episode added correctly, false otherwise.
 * */
bool MediaLibraryServer::addEpisode(const string &seriesName, const string &episodeName)
{
    cout << "Adding " << episodeName << " to " << seriesName << endl;
    return library->getSeries(seriesName).addEpisode(episodeName);
}

/**
 * Method to remove episode from series using the GUI top-menu functionality.
 * @param seriesName : name of the series containing the episode
 * @param episodeName : name of the episode to remove
 * @return true if episode removed successfully, false otherwise
 * */
bool MediaLibraryServer::removeEpisode(const string &seriesName, const string &episodeName)
{
    cout << "Removing " << episodeName << " from " << seriesName << endl;
    return library->getSeries(seriesName).removeEpisode(episodeName);
}

/**
 * Method to get series info in a Json object using JsonRPC.
 * @param seriesName : name of the series to retrieve information
 * @return Json object containing series information
 * */
Json::Value MediaLibraryServer::getSeries(const string &seriesName)
{
    cout << "Getting series information for: " << seriesName << endl;
    return library->jsonGetSeries(seriesName);
}

/**
 * Method to get Json object of episode information using JsonRPC.
 * @param seriesName : name of the series containing episode
 * @param episodeName : name of the episode to retrieve
 * @return Json object containing episode information.
 * */
Json::Value MediaLibraryServer::getEpisode(const string &seriesName, const string &episodeName)
{
    cout << "Getting episode information for: " << episodeName << endl;
    return library->getSeries(seriesName).jsonGetEpisode(episodeName);
}

/**
 * Method to get episode list size using JsonRPC.
 * @param seriesName : name of the series to get episode list size
 * @return int size of episode list.
 * */
int MediaLibraryServer::getEpisodeListSize(const string &seriesName)
{
    cout << "Getting episode list size for: " << seriesName << endl;
    return library->getEpisodeListSize(seriesName);
}

/**
 * Method to get list of series titles in the library in form of Json
 * object using JsonRPC.
 * @return Json Object containing list of series titles in library
 * */
Json::Value MediaLibraryServer::getLibraryTitles()
{
    cout << "Getting library titles." << endl;
    return library->jsonGetTitles();
}

/**
 * Method to get list of Episode titles for a given series,
 * using JsonRPC.
 * @param seriesName : name of series to retrieve episode titles.
 * @return Json object of Episode titles.
 * */
Json::Value MediaLibraryServer::getEpisodeTitles(const string &seriesName)
{
    cout << "Getting episode titles for: " << seriesName << endl;
    return library->getSeries(seriesName).jsonGetEpisodeTitles();
}

/**
 * Method to get series' title using JsonRPC.
 * @param itemLabel : gui label to signify with series title to retrieve
 * @return string title of series
 * */
string MediaLibraryServer::getSeriesTitle(const string &itemLabel)
{
    return library->getSeries(itemLabel).getTitle();
}

/**
 * Method to get the post URL using JsonRPC.
 * @param seriesName : name of the series related to the poster URL
 * @return string of poster URL link.
 * */
string MediaLibraryServer::getSeriesPoster(const string &seriesName)
{
    return library->getSeries(seriesName).getPoster();
}

/**
 * Method to get series genre using JsonRPC.
 * @param seriesName : name of the series to retrieve genre
 * @return string of genre(s) for the series.
 * */
string MediaLibraryServer::getSeriesGenre(const string &seriesName)
{
    return library->getSeries(seriesName).getGenre();
}

/**
 * Method to check if series is present in library using JsonRPC.
 * @param itemLabel : name of the series to check against the library
 * @return true if exists in library, false otherwise
 * */
bool MediaLibraryServer::checkSeriesExists(const string &itemLabel)
{
    return library->checkSeries(itemLabel);
}

/**
 * Method to get imdbRating for a series using JsonRPC.
 * @param itemLabel : gui label of series name to get imdbRating
 * @return string of imdbRating
 * */
string MediaLibraryServer::getSeriesImdbRating(const string &itemLabel)
{
    return library->getSeries(itemLabel).getImdbRating();
}

/**
 * Method to get Series Summary using JsonRPC.
 * @param itemLabel : gui label for series name
 * @return string of series summary information
 * */
string MediaLibraryServer::getSeriesSummary(const string &itemLabel)
{
    return library->getSeries(itemLabel).getSummary();
}

/**
 * Method to get Episode name using JsonRPC.
 * @param seriesName : name of the series containing Episode
 * @param episodeName : gui label with episode name
 * @return string of episode name
 * */
string MediaLibraryServer::getEpisodeName(const string &seriesName, const string &episodeName)
{
    return library->getSeries(seriesName).getEpisode(episodeName).getName();
}

/**
 * Method to get Episode imdbRating using JsonRPC.
 * @param seriesName : name of the series containing Episode
 * @param episodeName : gui label with episode name
 * @return string of episode imdbRating
 * */
string MediaLibraryServer::getEpisodeImdb(const string &seriesName, const string &episodeName)
{
    return library->getSeries(seriesName).getEpisode(episodeName).getImdbRating();
}

/**
 * Method to get Episode summary using JsonRPC.
 * @param seriesName : name of the series containing Episode
 * @param episodeName : gui label with episode name
 * @return string of episode summary
 * */
string MediaLibraryServer::getEpisodeSummary(const string &seriesName, const string &episodeName)
{
    return library->getSeries(seriesName).getEpisode(episodeName).getEpSummary();
}

void exiting()
{
    cout << "Server has been terminated.  Exiting normally." << endl;
}

/**
 * Main method to initialize server.
 * */
int main(int argc, char *argv[])
{

    int port = 8888;
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }
    HttpServer httpserver(port);
    MediaLibraryServer mls(httpserver, port);
    // to use tcp sockets instead of http uncomment below (comment above), and the include
    // for tcpsocketserver.h above. If not running locally, you will need to input ip & port
    // from command line for both server and client programs.
    //TcpSocketServer tcpserver("localhost",port);
    //StudentServer ss(tcpserver, port);
    std::atexit(exiting);
    auto ex = [](int i) {
        cout << "server terminating with signal " << i << endl;
        // ss.StopListening();
        exit(0);
        //return 0;
    };
    // ^C
    signal(SIGINT, ex);
    // abort()
    signal(SIGABRT, ex);
    // sent by kill command
    signal(SIGTERM, ex);
    // ^Z
    signal(SIGTSTP, ex);
    cout << "Media Library Server listening on port. " << port << "\nServer Ready." << endl;

    mls.StartListening();
    while (true)
    {
    }
    //int c = getchar();
    mls.StopListening();
    return 0;
}
