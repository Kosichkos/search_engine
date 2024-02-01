#include <iostream>
#include <fstream>
#include "EngineExceptions.h"
#include "ConverterJSON.h"
#include "version.h"

ConverterJSON::ConverterJSON()
{
    this->readConfigJSON();
    this->readRequestsJSON();
}

std::vector<std::string> ConverterJSON::getTextDocuments()
{
    std::vector<std::string> textDocuments;
    for (int i = 0; i < this->configJSON.filePaths.size(); ++i)
    {
        char* path = const_cast<char*>(configJSON.filePaths[i].c_str());
        std::ifstream textFile(path);
        if (!textFile.is_open())
            std::cerr << "File " << path << " not found." << std::endl;
        else
        {
            std::string result;
            std::stringstream buffer;
            buffer << textFile.rdbuf();
            result = buffer.str();
            textDocuments.emplace_back(result);
        }
    }
    return textDocuments;
}

int ConverterJSON::getResponsesLimit() const
{
    return this->configJSON.config.maxResponses;
}

std::vector<std::string> ConverterJSON::getRequests()
{
    return this->requestsJSON;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>> &answers)
{
    nlohmann::json jsonAnswers;
    std::ofstream answersFile(".\\config\\answers.json");
    int requestInd = 1;
    for (const auto& pairVec : answers)
    {
        nlohmann::json request;
        request["result"] = !pairVec.empty();
        if (!pairVec.empty())
        {
            nlohmann::json relevance = nlohmann::json::array();
            for (const auto& pair : pairVec)
            {
                nlohmann::json obj = {
                        {"docid", pair.first},
                        {"rank", pair.second}
                };
                relevance.push_back(obj);
            }
            request["relevance"] = relevance;
        }
        std::string requestNumber =
                (requestInd >= 100) ? std::to_string(requestInd) : (requestInd >= 10) ?
                "0" + std::to_string(requestInd) : "00" + std::to_string(requestInd);
        jsonAnswers["answers"]["request" + requestNumber] = request;
        ++requestInd;
    }
    answersFile << std::setw(4) << jsonAnswers << std::endl;
    answersFile.close();
}

std::string ConverterJSON::getProgramName() const
{
    return this->configJSON.config.name;
}

std::string ConverterJSON::getProgramVersion() const
{
    return this->configJSON.config.version;
}

void ConverterJSON::readConfigJSON()
{
    nlohmann::json jsonConfig;
    try
    {
        std::ifstream configFile(".\\config\\config.json");
        if (!configFile.is_open())
            throw ConfigFileMissingException();
        configFile >> jsonConfig;
        configFile.close();
        if (jsonConfig["config"].empty())
            throw ConfigFileEmptyException();
        if (jsonConfig["config"]["version"] != VERSION)
            throw IncorrectVersionException();
    }
    catch (const ConfigFileMissingException &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    catch (const ConfigFileEmptyException &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    catch (const IncorrectVersionException &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    this->configJSON.config.name = jsonConfig["config"]["name"];
    this->configJSON.config.version = jsonConfig["config"]["version"];
    this->configJSON.config.maxResponses = jsonConfig["config"]["max_responses"];
    for (auto it = jsonConfig["files"].begin(); it != jsonConfig["files"].end(); ++it)
        this->configJSON.filePaths.emplace_back(it.value());
}

void ConverterJSON::readRequestsJSON()
{
    nlohmann::json jsonRequests;
    try
    {
        std::ifstream requestsFile(".\\config\\requests.json");
        if (!requestsFile.is_open())
            throw RequestsFileMissingException();
        requestsFile >> jsonRequests;
        requestsFile.close();
    }
    catch (const RequestsFileMissingException &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    for (auto it = jsonRequests["requests"].begin(); it != jsonRequests["requests"].end(); ++it)
        this->requestsJSON.emplace_back(it.value());
}