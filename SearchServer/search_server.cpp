#include "search_server.h"
#include <algorithm>
#include <cmath>
#include <numeric>

    SearchServer::SearchServer() = default;

    SearchServer::SearchServer(const std::string& stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text)) {
    }

    void SearchServer::AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings) {
        if (document_id < 0)
        {
            throw std::invalid_argument("ID ��������� �� ����� ���� ������ 0.");
        }
        else if (documents_.count(document_id))
        {
            throw std::invalid_argument("�������� � ����� ID ��� ����������.");
        }
        else
        {
            const std::vector<std::string> words = SplitIntoWordsNoStop(document);
            const double inv_word_count = 1.0 / static_cast<double>(words.size());
            for (const std::string& word : words) {
                word_to_document_freqs_[word][document_id] += inv_word_count;
            }
            documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
            documents_id_by_position_[static_cast<int>(documents_id_by_position_.size())] = document_id;
        }
    }

    int SearchServer::GetDocumentId(int index) const {
        return documents_id_by_position_.at(index);
    }

    int SearchServer::GetDocumentCount() const {
        return static_cast<int>(documents_.size());
    }

    std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query) const {
        return SearchServer::FindTopDocuments(raw_query, [](int document_id, DocumentStatus status, int rating) { return status == DocumentStatus::ACTUAL; });
    }

    std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query, DocumentStatus status_function) const {
        return SearchServer::FindTopDocuments(raw_query, [&status_function](int document_id, DocumentStatus status_lambda, int rating) { return status_lambda == status_function; });
    }

    std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const std::string& raw_query, int document_id) const {
        if (!IsValidWord(raw_query))
            throw std::invalid_argument("� ������ ������� ���������� ������������ �������.");
        else if (!IsValidMinus(raw_query))
            throw std::invalid_argument("������ ������������ ����� ��� ����� ��� ��������� ������ ������ ������ ����� ������");
        else
        {
            const Query query = ParseQuery(raw_query);
            std::vector<std::string> matched_words;
            for (const std::string& word : query.plus_words) {
                if (word_to_document_freqs_.count(word) == 0) {
                    continue;
                }
                if (word_to_document_freqs_.at(word).count(document_id)) {
                    matched_words.push_back(word);
                }
            }
            for (const std::string& word : query.minus_words) {
                if (word_to_document_freqs_.count(word) == 0) {
                    continue;
                }
                if (word_to_document_freqs_.at(word).count(document_id)) {
                    matched_words.clear();
                    break;
                }
            }
            return { matched_words, documents_.at(document_id).status };
        }
    }

    bool SearchServer::IsValidMinus(const std::string& word)
    {
        std::vector<std::string> words = SplitIntoWords(word);

        for (const std::string& str : words)
        {
            if (str[0] == '-' && str[1] == '-')
                return false;
            else if (str == "-")
                return false;
        }
        return true;
    }

    bool SearchServer::IsValidWord(const std::string& word)
    {
        return std::none_of(word.begin(), word.end(), [](char c)
            {
                return c >= '\0' && c < ' ';
            });
    }

    bool SearchServer::IsStopWord(const std::string& word) const {
        return stop_words_.count(word) > 0;
    }

    std::vector<std::string> SearchServer::SplitIntoWordsNoStop(const std::string& text) const {
        std::vector<std::string> words;
        if (!IsValidWord(text))
            throw std::invalid_argument("� ������ ��������� ���������� ������������ �������.");
        else
        {
            for (const std::string& word : SplitIntoWords(text)) {
                if (!IsStopWord(word)) {
                    words.push_back(word);
                }
            }
            return words;
        }
    }

    int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = std::accumulate(ratings.begin(), ratings.end(), 0);

        return rating_sum / static_cast<int>(ratings.size());
    }

    SearchServer::QueryWord SearchServer::ParseQueryWord(std::string text) const {
        bool is_minus = false;
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return { text, is_minus, IsStopWord(text) };
    }

    SearchServer::Query SearchServer::ParseQuery(const std::string& text) const {
        Query query;
        for (const std::string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                }
                else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    double SearchServer::ComputeWordInverseDocumentFreq(const std::string& word) const {
        return std::log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }
