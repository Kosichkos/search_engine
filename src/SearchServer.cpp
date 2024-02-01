#include "SearchServer.h"

std::vector <std::vector <RelativeIndex>> SearchServer::search(const std::vector<std::string> &queriesInput)
{
    std::vector <std::vector <RelativeIndex>> finalResult;
    auto* freqDictionary = this->index->getFreqDictionary();
    for (int i = 0; i < queriesInput.size(); ++i)
    {
        std::vector <std::pair <std::string, int>> queryWords = this->parseString(queriesInput[i]);

        for (auto &pair : queryWords)
        {
            for (auto &it : *freqDictionary)
            {
                if (pair.first == it.first)
                {
                    for (auto &entry : it.second)
                        pair.second += static_cast<int>(entry.count);
                }
            }
        }

        std::sort(queryWords.begin(), queryWords.end(), [](auto pair1, auto pair2)
        {
            return pair1.second < pair2.second;
        });

        std::vector <std::vector <Entry>> requestEntries;
        for (auto &pair : queryWords)
        {
            auto entry = index->getWordCount(pair.first);
            requestEntries.emplace_back(entry);
        }

        std::vector <int> absRelevances;
        int docsCount = index->getDocsCount();
        for (int docId = 0; docId < docsCount; ++docId)
        {
            int absRelevance = 0;
            for (auto &wordEntries : requestEntries)
            {
                for (auto &wordEntry : wordEntries)
                {
                    if (wordEntry.doc_id == docId)
                        absRelevance += static_cast<int>(wordEntry.count);
                }
            }
            absRelevances.emplace_back(absRelevance);
        }

        int maxAbsRelevance = *max_element(absRelevances.begin(), absRelevances.end());
        if (maxAbsRelevance == 0)
        {
            finalResult.emplace_back(std::vector <RelativeIndex>{});
            continue;
        }

        std::vector <RelativeIndex> requestResult;
        for (int docId = 0; docId < docsCount; ++docId)
        {
            RelativeIndex rIndex{};
            rIndex.rank = static_cast<float>(absRelevances[docId]) / static_cast<float>(maxAbsRelevance);
            if (rIndex.rank != 0)
            {
                rIndex.docId = docId;
                requestResult.emplace_back(rIndex);
            }
        }
        std::sort(requestResult.begin(), requestResult.end(), [](RelativeIndex &a, RelativeIndex &b)
        {
            return (a.rank > b.rank || (a.rank == b.rank && a.docId < b.docId));
        });
        while (requestResult.size() > this->responsesLimit)
        {
            requestResult.pop_back();
        }
        finalResult.emplace_back(requestResult);
    }
    return finalResult;
}

std::vector <std::pair <std::string, int>> SearchServer::parseString(const std::string &text)
{
    std::vector <std::pair <std::string, int>> result;
    std::stringstream strStream(text);
    std::string word;
    while (strStream >> word)
    {
        auto pair = std::make_pair(word, 0);
        if (std::find(result.begin(), result.end(), pair) == result.end())
            result.push_back(pair);
    }
    return result;
}