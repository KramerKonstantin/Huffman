#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
#include<map>
#include<stack>

struct charinfo {
    uint32_t code;
    uint64_t frequency;
    uint8_t depth;
    charinfo();
    charinfo(uint32_t code, uint64_t frequency, uint8_t depth);
};

class HaffmanTree {
private:
    struct node {
        uint32_t frequency;
        uint8_t id;
        node* left = nullptr;
        node* right = nullptr;
        node(uint8_t id, uint32_t frequency);
        node();
    };

    std::vector<node*> del;
    std::vector<node> chars;
    void initCharFrequency(const std::vector<uint8_t> &input);
    std::vector<node*> heap;
    struct compartor {
        bool operator()(const node* l, const node* r) const;
    } comp;
    void createHeap();
    std::vector<charinfo> encodeMap;
    void createEncodeMap(const node* cur, int16_t code = 0, uint8_t depth = 0);

public:
    HaffmanTree(const std::vector<uint8_t> &input);
    ~HaffmanTree();
    charinfo operator [](uint8_t id);
};

class Encryptor {
private:
    HaffmanTree * encodeTree{};
    std::vector<uint8_t> output;
    void EncryptTree(const std::vector<uint8_t> &input);
    void EncryptBlock(const std::vector<uint8_t> &input);

public:
    Encryptor() = default;
    std::vector<uint8_t> Encrypt(const std::vector<uint8_t> &input);
};