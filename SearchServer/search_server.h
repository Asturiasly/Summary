#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "document.h"
#include "string_processing.h"

const int MAX_RESULT_DOCUMENT_COUNT = 5;
constexpr double INACCURACY = 1e-6;

class SearchServer {
public:

    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words);
    explicit SearchServer();
    explicit SearchServer(const std::string& stop_words_text);

    template <typename Predicate>
    std::vector<Document> FindTopDocuments(const std::string& raw_query, Predicate status) const;

    void AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings);
    std::vector<Document> FindTopDocuments(const std::string& raw_query) const;
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentStatus status_function) const;
    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, int document_id) const;
    int GetDocumentId(int index) const;
    int GetDocumentCount() const;

private:
    std::map<int, int> documents_id_by_position_;
    std::set<std::string> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, DocumentData> documents_;

    struct QueryWord {
        std::string data;
        bool is_minus;
        bool is_stop;
    };

    struct Query {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

    template <typename Predicate>
    std::vector<Document> FindAllDocuments(const Query& query, Predicate status) const;

    static bool IsValidMinus(const std::string& word);
    static bool IsValidWord(const std::string& word);
    bool IsStopWord(const std::string& word) const;
    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const;
    static int ComputeAverageRating(const std::vector<int>& ratings);
    QueryWord ParseQueryWord(std::string text) const;
    Query ParseQuery(const std::string& text) const;
    double ComputeWordInverseDocumentFreq(const std::string& word) const;
};

template <typename StringContainer>
SearchServer::SearchServer(const StringContainer& stop_words)
    : stop_words_(MakeUniqueNonEmptyStrings(stop_words))// Extract non-empty stop words
{
    using namespace std::string_literals;
    if (!all_of(stop_words_.begin(), stop_words_.end(), IsValidWord)) {
        throw std::invalid_argument("Some of stop words are invalid"s);
    }
}

template <typename Predicate>
std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query, Predicate status) const {
    using namespace std::string_literals;
    if (!IsValidWord(raw_query))
        throw std::invalid_argument("в тексте запроса содержатся недопустимые символы."s);
    else if (!IsValidMinus(raw_query))
        throw std::invalid_argument("нельзя использовать минус без слова или добавлять больше одного минуса перед словом"s);

        const Query query = ParseQuery(raw_query);

        auto matched_documents = FindAllDocuments(query, status);

        std::sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                if (std::abs(lhs.relevance - rhs.relevance) < INACCURACY) {
                    return lhs.rating > rhs.rating;
                }
                else {
                    return lhs.relevance > rhs.relevance;
                }
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
}

template <typename Predicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query& query, Predicate status) const {
    std::map<int, double> document_to_relevance;
    for (const std::string& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
        for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
            for (const auto [id, rating_status] : documents_)
            {
                if (id == document_id)
                {
                    if (status(id, rating_status.status, rating_status.rating))
                    {
                        document_to_relevance[document_id] += term_freq * inverse_document_freq;
                    }
                }
            }
        }
    }

    for (const std::string& word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
            document_to_relevance.erase(document_id);
        }
    }

    std::vector<Document> matched_documents;
    for (const auto [document_id, relevance] : document_to_relevance) {
        matched_documents.push_back(
            { document_id, relevance, documents_.at(document_id).rating });
    }
    return matched_documents;
}
