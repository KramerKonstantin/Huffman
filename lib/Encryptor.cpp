#include "Encryptor.h"

charinfo::charinfo() :  code(0), frequency(0), depth(1) {};

charinfo::charinfo(const uint32_t code, const uint64_t frequency, const uint8_t depth) : code(code), frequency(frequency), depth(depth) {}

HaffmanTree::node::node(const uint8_t id, uint32_t frequency) : id(id), frequency(frequency) {}

HaffmanTree::node::node() = default;

HaffmanTree::~HaffmanTree() {
    for(auto& el : del)
        delete el;
}

void HaffmanTree::initCharFrequency(const std::vector<uint8_t> &input) {
    chars.resize(256);
    for (int i = 0; i < 256; i++) {
        chars[i] = node(static_cast<uint8_t>(i), 0);
    }
    for (unsigned char i : input) {
        chars[i].frequency++;
    }
}

bool HaffmanTree::compartor::operator()(const node* l, const node* r) const {
    return l->frequency > r->frequency;
}

void HaffmanTree::createHeap() {
    for (int i = 0; i < 256; i++) {
        if (chars[i].frequency > 0)
            heap.push_back(&chars[i]);
    }

    std::make_heap(heap.begin(), heap.end(), comp);

    while (heap.size() > 1) {
        node* left = heap.front();
        std::pop_heap(heap.begin(), heap.end(), comp);
        heap.pop_back();

        node* right = heap.front();
        std::pop_heap(heap.begin(), heap.end(), comp);
        heap.pop_back();

        auto * new_node = new node(0, left->frequency + right->frequency);
        del.push_back(new_node);

        new_node->left = left;
        new_node->right = right;

        heap.push_back(new_node);
        std::push_heap(heap.begin(), heap.end(), comp);
    }
}

void HaffmanTree::createEncodeMap(const node* cur, const int16_t code, const uint8_t depth) {
    if (cur->left) { // && cur->right
        if (depth == 0) {
            createEncodeMap(cur->left, 0, static_cast<const uint8_t>(depth + 1));
            createEncodeMap(cur->right, 1, static_cast<const uint8_t>(depth + 1));
        }
        else {
            createEncodeMap(cur->left, code << 1, static_cast<const uint8_t>(depth + 1));
            createEncodeMap(cur->right, static_cast<const int16_t>(code << 1 | 1), static_cast<const uint8_t>(depth + 1));
        }

    }
    else {
        if (depth == 0) {
            encodeMap[cur->id] = charinfo(0, cur->frequency, 1);
        }
        else {
            encodeMap[cur->id] = charinfo(static_cast<const uint32_t>(code), cur->frequency, depth);
        }

    }
}
HaffmanTree::HaffmanTree(const std::vector<uint8_t> &input)
{
    initCharFrequency(input);
    createHeap();
    encodeMap.resize(256);
    createEncodeMap(heap[0]);
}

charinfo HaffmanTree::operator [](uint8_t id) {
    return encodeMap[id];
}

void Encryptor::EncryptTree(const std::vector<uint8_t> &input) {

    //costil alert
    for (int i = 0; i < 256; i++) {
        charinfo cur = (*encodeTree)[i];
        if (cur.code == 0 && cur.frequency > 0) {
            output[1024] = static_cast<unsigned char>(i);
        }
    }

    uint8_t max_depth = 0;
    for (int i = 0; i < 256; i++)
        max_depth = std::max((*encodeTree)[i].depth, max_depth);

    for (int i = 0; i < 256; i++) {
        auto info = (*encodeTree)[i];
        int cur_pos = i * 4;
        int cur_val = (info.code) << (sizeof(info.code) * 8 - info.depth);
        for (int k = 3; k >= 0; k--)
        {
            output[cur_pos + k] = (uint8_t) cur_val;
            cur_val >>= 8;
        }
    }
}


void Encryptor::EncryptBlock(const std::vector<uint8_t> &input) {

    size_t encode_byte_id = 1029;
    uint8_t byte_cur = 0;
    uint8_t bite_cur = 8;

    for (auto input_char : input)
    {
        auto info = (*encodeTree)[input_char];

        while (info.depth > 0) {

            int d = std::min(bite_cur, info.depth);

            bite_cur -= d;
            byte_cur <<= d;

            byte_cur += info.code >> (info.depth - d);

            auto t = static_cast<uint32_t>((1 << (info.depth - d)) - 1);

            info.code = (info.code) & t;

            info.depth -= d;

            if (bite_cur == 0) {
                bite_cur = 8;
                output[encode_byte_id] = byte_cur;
                byte_cur = 0;
                encode_byte_id++;
            }
        }
    }

    if (encode_byte_id < output.size()) {
        byte_cur <<= bite_cur;
        output[encode_byte_id] = byte_cur;
    }
}


std::vector<uint8_t> Encryptor::Encrypt(const std::vector<uint8_t> &input) {

    if(input.empty()) {
        return std::vector<uint8_t >{};
    }

    encodeTree = new HaffmanTree(input);

    uint32_t encrypt_length = 0;
    for (int i = 0; i < 256; i++) {
        auto cur = (*encodeTree)[i];
        encrypt_length += cur.depth * cur.frequency;
    }

    output.resize(((encrypt_length + 7) / 8) + 1029);

    for (int i = 1028; i >= 1025; i--) {
        output[i] = (uint8_t)encrypt_length;
        encrypt_length >>= 8;
    }

    EncryptTree(input);

    EncryptBlock(input);

    delete encodeTree;

    return output;
}