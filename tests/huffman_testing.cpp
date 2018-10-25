#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <cstdlib>
#include <random>
#include <vector>
#include <utility>

#include "gtest/gtest.h"
#include "../app/FileCompressor.h"

Encryptor encryptor;
Decryptor decryptor;

TEST(correctness, one_element)
{
    std::vector<uint8_t> t;
    for(size_t i = 1; i < 100; i++)
    {
        t.resize(i*100, 1);
        EXPECT_EQ(decryptor.Decrypt(encryptor.Encrypt(t)), t);
    }
}

TEST(correctness, ten_elements_rand_pos)
{
    std::vector<uint8_t > t({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    for(size_t i = 0; i < 100; i++)
    {
        std::shuffle(t.begin(), t.end(), std::mt19937(std::random_device()()));
        EXPECT_EQ(decryptor.Decrypt(encryptor.Encrypt(t)), t);
    }
}

TEST(correctness, empty_data)
{
    EXPECT_EQ(decryptor.Decrypt(encryptor.Encrypt(std::vector<uint8_t>{})), std::vector<uint8_t>{});
}

TEST(correctness, random_data)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    std::vector<uint8_t> t;
    for (size_t j = 0; j < 100; ++j)
    {
        for (size_t i = 0; i < 100000; i++)
            t.push_back(static_cast<uint8_t>(rand()));

        EXPECT_EQ(decryptor.Decrypt(encryptor.Encrypt(t)), t);
        t.clear();
    }
}
TEST(correctness, all_symbols)
{
    std::vector<uint8_t> t;
    for(size_t i = 0; i < 100; i++)
    {
        for (size_t i = 0; i < 256; i++)
        {
            t.push_back(i);
        }
        std::shuffle(t.begin(), t.end(), std::mt19937(std::random_device()()));
        EXPECT_EQ(decryptor.Decrypt(encryptor.Encrypt(t)), t);
    }
}

TEST(correctness, default_text)
{
    std::string text = "\n"
                       "Любви, надежды, тихой славы\n"
                       "Недолго нежил нас обман,\n"
                       "Исчезли юные забавы,\n"
                       "Как сон, как утренний туман;\n"
                       "Но в нас горит еще желанье,\n"
                       "Под гнетом власти роковой\n"
                       "Нетерпеливою душой\n"
                       "Отчизны внемлем призыванье.\n"
                       "Мы ждем с томленьем упованья\n"
                       "Минуты вольности святой,\n"
                       "Как ждет любовник молодой\n"
                       "Минуты верного свиданья.\n"
                       "Пока свободою горим,\n"
                       "Пока сердца для чести живы,\n"
                       "Мой друг, отчизне посвятим\n"
                       "Души прекрасные порывы!\n"
                       "Товарищ, верь: взойдет она,\n"
                       "Звезда пленительного счастья,\n"
                       "Россия вспрянет ото сна,\n"
                       "И на обломках самовластья\n"
                       "Напишут наши имена!";
    std::vector<uint8_t> t(text.begin(), text.end());
    EXPECT_EQ(decryptor.Decrypt(encryptor.Encrypt(t)), t);
}