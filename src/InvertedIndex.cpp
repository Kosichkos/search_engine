#include "InvertedIndex.h"

void InvertedIndex::updateDocumentBase(const std::vector<std::string> &inputDocs)
{
    if (!docs.empty())
        docs.clear();
    for (const auto &doc : inputDocs)
       docs.push_back(doc);
    freqDictionary.clear();
    std::mutex indexation_status;
    std::vector <std::thread> indexationThreads;
    for (int index = 0; index < docs.size(); ++index)
    {
        indexationThreads.emplace_back(std::thread([this, index]()
        {
            std::stringstream ss(docs[index]);
            std::string word;
            while (ss >> word)
            {
                std::for_each(word.begin(), word.end(), [](char &c)
                {
                    c = tolower(c);
                });
                bool foundWord = false;
                for (auto it = freqDictionary.begin(); it != freqDictionary.end() && !foundWord; ++it)
                {
                    if (it->first == word)
                    {
                        foundWord = true;
                        bool foundDocId = false;
                        for (int i = 0; i < it->second.size() && !foundDocId; ++i)
                        {
                            if (it->second[i].doc_id == index)
                            {
                                foundDocId = true;
                                ++it->second[i].count;
                            }
                        }
                        if (!foundDocId)
                        {
                            Entry newEntry{static_cast<size_t>(index), 1};
                            it->second.push_back(newEntry);
                            std::sort(it->second.begin(), it->second.end(), Entry::compareDocId);
                        }
                    }
                }
                if (!foundWord)
                {
                    Entry newEntry{static_cast<size_t>(index), 1};
                    freqDictionary.insert(std::pair<std::string,
                                             std::vector<Entry>>(word, std::vector<Entry>{newEntry}));
                }
            }
        }));
    }

    for (auto &indexationThread : indexationThreads)
        indexationThread.join();
}

std::vector<Entry> InvertedIndex::getWordCount(const std::string &word)
{
    for (auto &pair : freqDictionary)
    {
        if (pair.first == word)
            return pair.second;
    }
    return std::vector<Entry>{};
}

std::map<std::string, std::vector<Entry>> *InvertedIndex::getFreqDictionary()
{
    return &this->freqDictionary;
}

int InvertedIndex::getDocsCount()
{
    return static_cast<int>(this->docs.size());
}