#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

class ConverterJSON
{
private:
    struct
    {
        struct
        {
            std::string name;
            std::string version;
            int maxResponses = 0;
        } config;
        std::vector<std::string> filePaths;
    } configJSON;
    std::vector<std::string> requestsJSON;
    void readConfigJSON();
    void readRequestsJSON();
public:
    ConverterJSON();
    ~ConverterJSON() = default;
    std::vector<std::string> getTextDocuments();
    std::string getProgramName() const;
    std::string getProgramVersion() const;
    int getResponsesLimit() const;
    std::vector<std::string> getRequests();
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>> &answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H