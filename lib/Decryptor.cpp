#include "Decryptor.h"

Decryptor::Decryptor() = default;

Decryptor::node::node(uint8_t byte) : byte(byte), left(nullptr), right(nullptr) {}

void Decryptor::NormalizeTree(node* cur, bool del) {
    if (cur->left && cur->right) {
        if (del) {
            top = cur;
        }
        NormalizeTree(cur->left, false);
        NormalizeTree(cur->right, false);
    } else if (cur->left || cur->right) {
        node** next = (cur->left ? &cur->left : &cur->right);
        if (del) {
            if ((*next)->left || (*next)->right) {
                top = *next;
                NormalizeTree(top, true);
            } else {
                top = cur;
            }
        } else {
            *next = nullptr; // was false
        }
    } else {
        cur->left = nullptr;
        cur->right = nullptr;
    }
}

uint32_t Decryptor::Read(const std::vector<uint8_t> &input, uint32_t pos) {
    uint32_t out = 0;
    for (int k = 0; k < 4; k++) {
        out <<= 8;
        out += input[pos + k];
    }
    return out;
}

void Decryptor::DecryptTree(const std::vector<uint8_t> &input) {
    top = new node(0);
    del.push_back(top);

    for (int cur_byte = 0; cur_byte < 256; cur_byte++) {

        uint32_t code = Read(input, static_cast<uint32_t>(cur_byte * 4));

        auto mask = static_cast<uint32_t>(1 << 31);

        node* cur_node = top;

        for (int t = 0; t < 32; t++) {
            node** path = (code & mask ? &cur_node->right : &cur_node->left);
            mask >>= 1;

            if (*path == nullptr) {
                *path = new node(static_cast<uint8_t>(cur_byte));
                del.push_back(*path);
            }

            cur_node = *path;
        };
    }

    NormalizeTree(top);
    int trueZero = input[1024];
    node* cur = top;
    while (cur) {
        cur->byte = static_cast<uint8_t>(trueZero);
        cur = cur->left;
    }

}

void Decryptor::DecryptBlock(const std::vector<uint8_t> &input) {

    uint32_t length = Read(input, 1025);

    std::queue<uint8_t> out;

    uint32_t total_bit = 0;
    uint8_t  cur_bite = 7;
    uint32_t cur_byte = 1029;

    node* cur_node = top;

    while (total_bit < length) {
        auto t = static_cast<bool>(input[cur_byte] & (1 << cur_bite));

        cur_node = ( t ? cur_node->right : cur_node->left);

        if (!(cur_node->left || cur_node->right)) {
            out.push(cur_node->byte);
            cur_node = top;
        }

        if (cur_bite == 0) {
            cur_bite = 7;
            cur_byte++;
        } else {
            cur_bite--;
        }

        total_bit++;
    }

    output.resize(out.size());

    auto msg_length = static_cast<int>(out.size());
    for (int i = 0; i < msg_length; i++) {
        output[i] = out.front();
        out.pop();
    }

}

void Decryptor::flush() {
    output.resize(0);
}

Decryptor::~Decryptor() {
    for(auto &el : del) {
        delete el;
    }
}

std::vector<uint8_t> Decryptor::Decrypt(const std::vector<uint8_t> &input) {

    if(input.empty()) {
        return std::vector<uint8_t >{};
    }

    DecryptTree(input);

    DecryptBlock(input);

    return output;
}