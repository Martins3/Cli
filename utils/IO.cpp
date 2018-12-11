#include "IO.hpp"
#include "json.hpp"
#include "../Resource.hpp"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

using json = nlohmann::json;
using namespace std;
static const std::string src_dir = "/home/shen/Core/Sharp/src/";

std::ostream &operator<<(std::ostream &os, const TimeStamp & t) { 
    std::tm * ptm = std::localtime(&(t.time_point));
    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buffer, 32, "%Y.%m,%d %a %H:%M:%S", ptm);  
    return os << std::string(buffer) << std::endl << t.desc;
}

void to_json(json& j, const TimeStamp& p){
    j =  json{
        {"time", p.time_point},
        {"desc", p.desc},
        {"tag", p.tag},
        {"introspection", p.introspection},
    };
}

void from_json(const json& j, TimeStamp& p){
    j.at("time").get_to(p.time_point);
    j.at("desc").get_to(p.desc);
    j.at("tag").get_to(p.tag);
    j.at("introspection").get_to(p.introspection);
}



void Loader::load(){
    std::ifstream infile(src_dir + "record.json");
    string line;
    vector<TimeStamp *> & records = Resource::getInstance().getTimeline();
#ifdef DEBUG
    cout << "Loaded Records:\n";
#endif
    while (std::getline(infile, line)){
        json j = json::parse(line);
#ifdef DEBUG
    cout << j << endl;
#endif
        TimeStamp r = j;
        TimeStamp * w = new TimeStamp(r);
        records.push_back(w);
    }
    infile.close();
}

// store the whole time into the file
void Loader::store(){
    vector<TimeStamp *> & records = Resource::getInstance().getTimeline();
    // when with in this way, the file already cleared !
    std::ofstream outfile(src_dir + "record.json");

#ifdef DEBUG
    cout << "Stored Records:\n";
#endif

    for(auto w : records){
        json j = *w;

#ifdef DEBUG
      cout << j << endl;
#endif
        outfile << j.dump() << endl;
    }
    outfile.close();
}

void Loader::add_one_record(TimeStamp & t){
    std::ofstream outfile(src_dir + "record.json", std::ios_base::app | std::ios_base::out);
    json j = t;
    outfile << j.dump() << endl;
    outfile.close();
}
