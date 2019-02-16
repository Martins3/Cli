#include "IO.hpp"
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>

using json = nlohmann::json;
using namespace std;
static const std::string src_dir = "/home/shen/Core/Sharp/src/";

// std::ostream &operator<<(std::ostream &os, const TimeStamp &t) {
  // std::tm *ptm = std::localtime(&(t.time_point));
  // char buffer[32];
  // // Format: Mo, 15.06.2009 20:20:00
  // std::strftime(buffer, 32, "%Y.%m,%d %a %H:%M:%S", ptm);
  // return os << std::string(buffer) << std::endl << t.desc;
// }

void to_json(json &j, const TimeStamp &p) {
  j = json{
      {"time", p.time_point},
      {"desc", p.desc},
      {"tag", p.tag},
  };
}

void from_json(const json &j, TimeStamp &p) {
  j.at("time").get_to(p.time_point);
  j.at("desc").get_to(p.desc);
  j.at("tag").get_to(p.tag);
}

void Loader::load() {
  std::ifstream infile(src_dir + "record.json");
  string line;
  while (std::getline(infile, line)) {
    json j = json::parse(line);
    TimeStamp r = j;
    TimeStamp *w = new TimeStamp(r);
    records.push_back(w);
  }
  infile.close();
}

void Loader::store() {
  // when with in this way, the file already cleared !
  std::ofstream outfile(src_dir + "record.json");

  for (auto w : records) {
    json j = *w;
    outfile << j.dump() << endl;
  }
  outfile.close();
}

void Loader::add_one_record(TimeStamp &t) {
  std::ofstream outfile(src_dir + "record.json",
                        std::ios_base::app | std::ios_base::out);
  json j = t;
  outfile << j.dump() << endl;
  outfile.close();
}
