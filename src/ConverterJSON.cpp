#include <iostream>
#include <fstream>
#include <filesystem>
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
    std::filesystem::path answersFilePath = std::filesystem::path(".\\config\\answers.json");
    std::ofstream answersFile(answersFilePath);
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
        std::filesystem::path configFilePath = std::filesystem::path(".\\config\\config.json");
        if (!std::filesystem::exists(configFilePath))
            throw ConfigFileMissingException();
        std::ifstream configFile(configFilePath);
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
        abort();
    }
    catch (const ConfigFileEmptyException &ex)
    {
        std::cerr << ex.what() << std::endl;
        abort();
    }
    catch (const IncorrectVersionException &ex)
    {
        std::cerr << ex.what() << std::endl;
        abort();
    }
    this->configJSON.config.name = jsonConfig["config"]["name"];
    this->configJSON.config.version = jsonConfig["config"]["version"];
    this->configJSON.config.maxResponses = jsonConfig["config"]["max_responses"];
    for (const auto& item : jsonConfig["files"]) {
        this->configJSON.filePaths.emplace_back(item);
    }
}

void ConverterJSON::readRequestsJSON()
{
    nlohmann::json jsonRequests;
    try
    {
        std::filesystem::path requestsFilePath = std::filesystem::path(".\\config\\requests.json");
        if (!std::filesystem::exists(requestsFilePath))
            throw RequestsFileMissingException();
        std::ifstream requestsFile(requestsFilePath);
        if (!requestsFile.is_open())
            throw RequestsFileMissingException();
        requestsFile >> jsonRequests;
        requestsFile.close();
    }
    catch (const RequestsFileMissingException &ex)
    {
        std::cerr << ex.what() << std::endl;
        abort();
    }
    for (const auto& item : jsonRequests["requests"]) {
        this->requestsJSON.emplace_back(item);
    }
}
