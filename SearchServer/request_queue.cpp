#include "request_queue.h"

    RequestQueue::RequestQueue(const SearchServer& search_server)
        : searcher_(search_server),
        empty_docs_(0)
    {
    }

    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status_function) {

        return RequestQueue::AddFindRequest(raw_query, [&status_function](int document_id, DocumentStatus status_lambda, int rating) { return status_lambda == status_function; });
    }
    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {

        return RequestQueue::AddFindRequest(raw_query, [](int document_id, DocumentStatus status, int rating) { return status == DocumentStatus::ACTUAL; });
    }

    int RequestQueue::GetNoResultRequests() const {

        return empty_docs_;
    }