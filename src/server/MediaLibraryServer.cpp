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

class MediaLibraryServer : public mediaserverstub {

     private: 
        MediaLibrary *library;
        int portNum;

    protected:
    public:
        MediaLibraryServer(AbstractServerConnector &connector, int portNum, string filename);
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

};

//TODO:  implement the above