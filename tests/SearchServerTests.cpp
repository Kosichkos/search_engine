#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "SearchServer.h"

TEST(TestCaseSearchServer, TestSimple)
{
    const std::vector<std::string> docs =
    {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const std::vector<std::string> request = {"milk water", "sugar"};
    const std::vector<std::vector<RelativeIndex>> expected =
    {
            {
                    {2, 1},
                    {0, 0.7},
                    {1, 0.3}
            },
            {}
    };
    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    SearchServer srv(&idx, 5);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);

    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5)
{
    const std::vector <std::string> docs =
    {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
    };
    const std::vector <std::string> request = {"moscow is the capital of russia"};
    const std::vector<std::vector <RelativeIndex>> expected =
    {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.666666687},
                    {1, 0.666666687},
                    {2, 0.666666687}
            }
    };
    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    SearchServer srv(&idx, 5);
    std::vector<std::vector <RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestHobbit)
{
    const std::vector <std::string> docs =
    {
            "In a hole in the ground there lived a hobbit",
            "the mother of our particular hobbit what is a hobbit",
            "the mother of this hobbit was the fabulous belladonna took",
            "this hobbit was named bilbo baggins",
            "good morning said bilbo and he meant it",
            "the next day he had almost forgotten about gandalf"
    };
    const std::vector <std::string> request =
    {
            "this hobbit is bilbo",
            "his mother is the belladonna took",
            "gandalf"
    };
    const std::vector<std::vector <RelativeIndex>> expected =
    {
            {
                    {1, 1},
                    {3, 1},
                    {2, 0.666666687},
                    {0, 0.333333343},
                    {4, 0.333333343}
            },
            {
                    {2, 1},
                    {1, 0.6},
                    {0, 0.2},
                    {5, 0.2}
            },
            {
                    {5, 1}
            }
    };
    InvertedIndex idx;
    idx.updateDocumentBase(docs);

    SearchServer srv(&idx, 5);
    std::vector<std::vector <RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestSongsStrings)
{
    const std::vector <std::string> docs =
    {
            "come to me now don't let me go stay by my side don't let me go",
            "i hope you never let me down again",
            "bring me back to life never let me go",
            "never never let you go you are the one i'm searching for",
            "don't let me down don't let me down",
            "don't let me go little darling",
            "never gonna give you up never gonna let you down",
            "let go let go let go let go"
    };
    const std::vector <std::string> request =
    {
            "never let me down",
            "never let me go",
            "don't let me down",
            "don't let me go"
    };
    const std::vector<std::vector <RelativeIndex>> expected =
    {
            {
                {4, 1},
                {0, 0.833333313},
                {1, 0.666666687}
            },
            {
                {7, 1},
                {0, 0.875},
                {2, 0.625}
            },
            {
                {4, 1},
                {0, 0.875},
                {7, 0.5}
            },
            {
                {0, 1},
                {7, 0.888888896},
                {4, 0.666666687}
            }
    };
    InvertedIndex idx;
    idx.updateDocumentBase(docs);

    SearchServer srv(&idx, 3);
    std::vector<std::vector <RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestEmptyResult)
{
    const std::vector <std::string> docs =
    {
            "joy division new order",
            "metallica megadeth anthrax slayer",
            "nirvana pearl jam alice in chains soundgarden",
            "oasis blur pulp suede"
    };
    const std::vector <std::string> request =
    {
            "aphex twin",
            "depeche mode",
            "placebo"
    };
    const std::vector<std::vector <RelativeIndex>> expected =
    {
            {}, {}, {}
    };
    InvertedIndex idx;
    idx.updateDocumentBase(docs);

    SearchServer srv(&idx, 5);
    std::vector<std::vector <RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}