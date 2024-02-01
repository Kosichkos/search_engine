#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <thread>
#include <algorithm>
#include <mutex>


struct Entry
{
    size_t doc_id, count;

    bool operator == (const Entry& other) const
    {
        return (doc_id == other.doc_id && count == other.count);
    }

    static bool compareDocId(Entry &a, Entry &b)
    {
        return a.doc_id < b.doc_id;
    }
};

class InvertedIndex
{
private:
    std::vector <std::string> docs;
    std::map <std::string, std::vector<Entry>> freqDictionary;
public:
    InvertedIndex() = default;

    ~InvertedIndex() = default;

    std::map <std::string, std::vector<Entry>>* getFreqDictionary();
    int getDocsCount();
    void updateDocumentBase(const std::vector<std::string> &inputDocs);
    std::vector<Entry> getWordCount(const std::string &word);
};