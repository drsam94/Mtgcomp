// (c) 2017 Sam Donow
#pragma once
#include <array>
#include <string_view>
#include <unordered_map>
#include <memory>

template <typename numeric_t, const size_t AlphabetSize>
class Trie {
  private:
    class Node {
      private:
        std::array<std::unique_ptr<Node>, AlphabetSize> children;
        numeric_t value;

      public:
        explicit Node(numeric_t v) : value(v) { }
        Node() = default;
        void addChild(size_t index, numeric_t val) { children[index].reset(new Node(val)); }
        Node *getChild(size_t index) { return children[index].get(); }
        const numeric_t getValue() const { return value; }
    };

    Node root;
    std::unordered_map<char, size_t> alphabetIndex;
  public:
    explicit Trie (const std::array<char, AlphabetSize> &alphabet) : root()
    {
        size_t val = 0;
        for (char c : alphabet) {
            alphabetIndex[c] = val++;
        }
    }
    Trie(Trie &) = delete;
    Trie(Trie &&) = delete;
    Trie &operator=(Trie &) = delete;
    Trie() = delete;

    std::pair<size_t, numeric_t> longestPrefix(const std::string_view &s)
    {
        std::pair<size_t, numeric_t> ret;
        auto &[len, val] = ret;
        Node *current = &root;
        while ((current = current->getChild(alphabetIndex[s[len]])) != nullptr) {
            ++len;
            val = current->getValue();
        }
        return ret;
    }

    bool insert(const std::string_view &s, numeric_t val)
    {
        Node *prev = &root;
        Node *current = &root;
        size_t ind = 0;
        while (current != nullptr) {
            prev = current;
            current = current->getChild(alphabetIndex[s[ind++]]);
        }
        if (ind != s.size())
            return false;
        prev->addChild(alphabetIndex[s[ind - 1]], val);
        return true;
    }
};
