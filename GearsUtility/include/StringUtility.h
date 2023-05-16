#ifndef GEARS_STRING_UTILITY_H
#define GEARS_STRING_UTILITY_H

#include <string_view>
#include <string>
#include <vector>
#include <memory>

namespace GLib::Utility
{
    /**
    * @brief •¶š—ñ‚ğ•ªŠ„‚µ‚Ä•Ô‚·
    * @param str •ªŠ„‚·‚é•¶š—ñ
    * @param delim •ªŠ„‚Ég‚¤‹æØ‚è•¶š
    */
    static std::vector<std::string> Split(std::string_view str, char delim)
    {

        std::vector<std::string> tokens;
        auto begin = str.begin();
        auto end = str.end();
        tokens.reserve(std::count(str.begin(), str.end(), delim) + 1);

        while (begin != end)
        {
            auto pos = std::find(begin, end, delim);
            tokens.emplace_back(std::string_view(&(*begin), pos - begin));
            if (pos != end) begin = ++pos;
        }

        return tokens;
    }

    class RopeString
    {
    private:
        struct RopeNode
        {
        public:
            std::unique_ptr<RopeNode> left;
            std::unique_ptr<RopeNode> right;
            size_t length;
            std::string str;

        public:
            RopeNode(const char* s, size_t len) : length{ len }, str{ s, len }
            {}
            RopeNode(const std::string& s) : RopeNode{ s.c_str(), s.length() }
            {}

        public:
            bool isLeaf() const
            {
                return !left && !right;
            }
        };
    private:
        std::unique_ptr<RopeNode> root;
        size_t size;

    public:
        RopeString() : size{ 0 }, root{ nullptr }
        {}

        RopeString(const std::string& str) : size{ str.length() }, root{ std::make_unique<RopeNode>(str) }
        {}


    public:
        void append(const std::string& str)
        {
            if (str.empty()) return;
            if (!root) root = std::make_unique<RopeNode>(str);
            else root = merge(std::move(root), std::move(std::make_unique<RopeNode>(str)));
            size += str.length();
        }

        std::string substr(size_t start, size_t len)
        {
            if (start < 0 || start + len > size) return "";
            if (!root) return "";
            return std::string{ substr(root.get(), start, len) };
        }

        char at(size_t index)
        {
            if (index < 0 || index >= size || !root) throw std::out_of_range("Index out of range");
            return at(root.get(), index);
        }

        size_t find(const std::string& str, size_t index = 0)
        {
            if (!root || index < 0 || index + str.length() > size) return -1;
            return find(root.get(), str, index);
        }

        size_t length() const
        {
            return size;
        }

        std::string ToString()
        {
            if (!root) return "";
            return std::string(root->str);
        }
    private:
        std::unique_ptr<RopeNode> merge(std::unique_ptr<RopeNode> left, std::unique_ptr<RopeNode> right)
        {
            auto node = std::make_unique<RopeNode>("", left->length + right->length);
            node->left = std::move(left);
            node->right = std::move(right);
            return node;
        }

        std::string substr(RopeNode* node, size_t start, size_t len)
        {
            if (start == 0 && len <= node->length) return node->str;
            if (len <= node->left->length - start) return substr(node->left.get(), start, len);
            if (start >= node->left->length) return substr(node->right.get(), start - node->left->length, len);

            std::string left = substr(node->left.get(), start, node->left->length - start);
            std::string right = substr(node->right.get(), 0, len - (node->left->length - start));
            return std::move(left) + std::move(right);
        }

        char at(RopeNode* node, size_t index)
        {
            if (!node->left && !node->right) return node->str[index];
            if (index < node->left->length) return at(node->left.get(), index);
            else return at(node->right.get(), index - node->left->length);
        }

        size_t find(const RopeNode* node, const std::string& str, size_t index) const
        {
            std::string_view str_view{ str };
            if (node->isLeaf()) return node->str.find(str_view, index);

            if (index + str_view.length() <= node->left->length) return find(node->left.get(), str, index);
            else if (index >= node->left->length) return find(node->right.get(), str, index - node->left->length);
            else
            {
                auto pos1 = find(node->left.get(), str.substr(0, node->left->length - index), index);
                if (pos1 != -1) return pos1;
                return find(node->right.get(), str.substr(node->left->length - index), 0);
            }
        }
    };
};

#endif // !GEARS_STRING_UTILITY_H
