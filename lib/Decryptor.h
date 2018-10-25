#ifndef UNTITLED5_DECRYPTOR_H
#define UNTITLED5_DECRYPTOR_H

#include <cstdint>
#include <vector>
#include <queue>
#include <stack>

class Decryptor {
private:

    struct node {
        node* left;
        node* right;
        uint8_t byte;
        explicit node(uint8_t byte);
    };

    node* top;
    std::vector<node*> del;
    void NormalizeTree(node* cur, bool del = true);
    uint32_t Read(const std::vector<uint8_t> &input, uint32_t pos);
    void DecryptTree(const std::vector<uint8_t> &input);
    std::vector<uint8_t> output;
    void DecryptBlock(const std::vector<uint8_t> &input);
    void flush();

public:
    Decryptor();
    ~Decryptor();
    std::vector<uint8_t> Decrypt(const std::vector<uint8_t> &input);
};


#endif //UNTITLED5_DECRYPTOR_H