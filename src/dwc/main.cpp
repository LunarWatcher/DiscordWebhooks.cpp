#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "cpr/api.h"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

void sanitizeUrl(std::string& input) {
    input = std::regex_replace(input, std::regex("&amp;"), "&");
}

std::string parseSingle(const std::string& in, const std::regex& regex) {
    std::smatch match;
    if (!std::regex_search(in, match, regex)) {
        return "";
    }

    return match[1];
}

/**
 * Parsing XML with regex, because pugixml didn't work on basic fucking config
 * :') Unleash Tony the pony!
 */
std::vector<std::string> tonythepony(std::string inputXml, const std::string& lastEntry) {
    auto itemRegex = std::regex("<item>(.*?)</item>");

    auto urlRegex = std::regex("<link>(.*)</link>");
    auto dateRegex = std::regex("<pubDate>(.*)</pubDate>");

    std::vector<std::string> urls;
    std::string xml = std::regex_replace(inputXml, std::regex{"(\n|\r|\t+| {2,})"}, " ");

    auto grabbedLatest = false;
    for (std::sregex_iterator it(xml.begin(), xml.end(), itemRegex); it != std::sregex_iterator{}; ++it) {
        std::string item = (*it).str();
        std::cout << item << "\n----\n";

        auto url = parseSingle(item, urlRegex);

        auto date = parseSingle(item, dateRegex);

        if (date == lastEntry) {
            std::cout << "equal" << std::endl;
            break;
        }
            std::cout << lastEntry << " " << date << std::endl;
        if (url == "") {
            // No url? Invalid entry - skip

            continue;
        }
        if (!grabbedLatest) {
            grabbedLatest = true;
            urls.push_back(date);
        }

        sanitizeUrl(url);
        urls.push_back(url);
    }

    return urls;
}

int main() {
    using namespace std::literals;
    using namespace std::chrono;

    std::ifstream stream("channels.json");
    nlohmann::json config;
    stream >> config;

    while (true) {
        for (auto& source : config) {
            std::string url = source.at("url").get<std::string>();
            auto outputs = source.at("outputs").get<std::vector<std::string>>();
            auto outputAdditions = source.value("outputCustomMessage", std::vector<std::string>());

            std::string username = source.value("username", "Feeds");
            std::string lastEntry = source.value("last_entry", "");

            auto xml = cpr::Get(cpr::Url{url});
            auto res = tonythepony(xml.text, lastEntry);
            if (res.size() > 0) {
                source["last_entry"] = res[0];
            }

            for (size_t i = 1; i < res.size(); ++i) {

                for (size_t j = 0; j < outputs.size(); ++j) {
                    std::string addition = "";
                    if (outputAdditions.size() >= outputs.size()) {
                        addition = outputAdditions.at(j);
                    }
                    auto hook = outputs.at(j);

                    nlohmann::json payload{//
                            {"username", username}, //
                            {"content", addition + (addition.size() == 0 ? "" : " ") + res[i]}};
                    auto response = cpr::Post(cpr::Url{hook}, //
                            cpr::Header{{"Content-Type", "application/json"}}, //
                            cpr::Body(payload.dump()));
                    std::this_thread::sleep_for(500ms);
                }
            }
        }

        std::ofstream stream("channels.json");
        stream << config.dump(4);

        std::this_thread::sleep_for(20min);
    }

    return 0;
}
