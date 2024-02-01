#ifndef SEARCHENGINE_SEARCHSERVER_H
#define SEARCHENGINE_SEARCHSERVER_H
#include <vector>
#include <set>
#include <map>
#include "InvertedIndex.h"

struct RelativeIndex
{
    size_t docId;
    float rank;
    bool operator == (const RelativeIndex& other) const
    {
        return (docId == other.docId && rank == other.rank);
    }
};

class SearchServer
{
private:
    InvertedIndex* index;
    int responsesLimit;
public:
    SearchServer(InvertedIndex* idx, int rLimit) : index(idx), responsesLimit(rLimit) {};
    ~SearchServer() = default;
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queriesInput);
    std::vector <std::pair <std::string, int>> parseString(const std::string &text);
};

#endif //SEARCHENGINE_SEARCHSERVER_H