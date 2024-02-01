#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main()
{
    auto* converter = new ConverterJSON();
    auto* invIndex = new InvertedIndex();
    std::cout << converter->getProgramName() << "" << std::endl;
    std::cout << "Version: " << converter->getProgramVersion() << "" << std::endl;
    invIndex->updateDocumentBase(converter->getTextDocuments());
    auto* searchServer = new SearchServer(invIndex, converter->getResponsesLimit());
    auto result = searchServer->search(converter->getRequests());

    std::vector <std::vector <std::pair <int, float>>> answersToJSON;
    for (int i = 0; i < result.size(); ++i)
    {
        std::vector <std::pair <int, float>> pairVec;
        for (int j = 0; j < result[i].size(); ++j)
        {
            auto pair = std::make_pair(static_cast<int>(result[i][j].docId), result[i][j].rank);
            pairVec.emplace_back(pair);
        }
        answersToJSON.emplace_back(pairVec);
    }

    converter->putAnswers(answersToJSON);
    std::cout << R"(Answers for requests from "requests.json" is added to "answers.json")" << std::endl;
    delete converter;
    delete invIndex;
    delete searchServer;
    converter = nullptr;
    invIndex = nullptr;
    searchServer = nullptr;
    return 0;
}