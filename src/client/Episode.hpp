#include <string>
#include <json/json.h>

using namespace std;

class Episode {

protected:

  string name;
  string epRating;

public:

    Episode();
    Episode(string name, string epRating);
    Episode(const Json::Value& jsonObj);
    Episode(string jsonString);
    ~Episode();

    string getEpRating();
    string getName();
    string toJsonString();
    Json::Value toJson();
    void fromjson(Json::Value json);
    void print();
    string toString();

};