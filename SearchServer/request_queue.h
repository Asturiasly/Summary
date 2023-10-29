#pragma once

#include <deque>

#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status_function);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;

private:
    struct QueryResult {
        std::string request;
        std::size_t document_count;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& searcher_;
    int empty_docs_;
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    std::vector<Document> result = searcher_.FindTopDocuments(raw_query, document_predicate);

    requests_.push_back({ raw_query, result.size()});

    if (result.empty())
    {
        ++empty_docs_;
    }

    if (requests_.size() == min_in_day_ + 1)
    {
        requests_.pop_front();
        --empty_docs_;
    }

    return result;
}