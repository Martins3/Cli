#include "IO.hpp"
#include "json.hpp"
#include "../Resource.hpp"
#include <iomanip>
#include <fstream>
#include <set>
#include <string>


  
using json = nlohmann::json;
using namespace std;
static const std::string src_dir = "/home/shen/Core/Sharp/src/";
void to_json(json& j, const TimeStamp& p){
    j =  json{
        {"time", p.time_point},
        {"desc", p.desc},
    };
}

void from_json(const json& j, TimeStamp& p){
    j.at("time").get_to(p.time_point);
    j.at("desc").get_to(p.desc);
}

void Loader::load(){
    std::ifstream infile(src_dir + "record.json");
    string line;
    auto records = Resource::getInstance().getTimeline();
    while (std::getline(infile, line)){
        json j = json::parse(line);
        TimeStamp r = j;
        TimeStamp * w = new TimeStamp(r);
        
        records.push_back(w);
    }
    infile.close();
}

// store the whole time into the file
void Loader::store(std::vector<TimeStamp *> & s){
    std::ofstream outfile(src_dir + "record.json");
    auto records = Resource::getInstance().getTimeline();
    for(auto w : records){
        json j = *w;
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
