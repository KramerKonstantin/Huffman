#include "FileCompressor.h"


void encrypt(std::string file_in, std::string file_out) {
	const int blockSize = 1024 * 1024;

	std::ifstream in(file_in, std::ios_base::binary);
	std::ofstream out(file_out, std::ios_base::binary);

	if (in.fail() || out.fail()) {
		throw std::runtime_error("SUCH FILE NOT FOUND");
	}
	std::vector<uint8_t> data;

	printf("encrypted blocks:\n");
	uint64_t totalEncrypted = 0;

	std::vector<uint8_t> encrypted_data;
	auto c = static_cast<uint8_t >(in.get());

	Encryptor encryptor;

	while (!in.eof()) {

		for (int i = 0; i < blockSize && !in.eof(); i++) {
			data.push_back(c);
			c = static_cast<uint8_t >(in.get());
		}

		encrypted_data = encryptor.Encrypt(data);
		auto sz = uint32_t(encrypted_data.size());

		printf("%d -> %d (%.3f)\n", static_cast<int>(data.size()), sz , .0);
		totalEncrypted+= data.size();

		out.write(reinterpret_cast<char*>(&sz), sizeof(uint32_t));
		out.write((char*)(&encrypted_data[0]), sz);

		data.clear();
	}

	printf("total %d\n", static_cast<int>(totalEncrypted));

	in.close();
	out.close();
}

void decrypt(std::string file_in, std::string file_out) {
	std::ifstream in(file_in, std::ios_base::binary);
	std::ofstream out(file_out, std::ios_base::binary);

	if (in.fail() || out.fail()) {
		throw std::runtime_error("SUCH FILE NOT FOUND");
	}

	std::vector<uint8_t> data;
	std::vector<uint8_t> decrypted_data;
	uint32_t cur_size = 0;

	in.read(reinterpret_cast<char *>(&cur_size), sizeof(uint32_t));
	Decryptor decryptor;

	printf("\ndecrypted blocks:\n");

	while (!in.eof()) {

		data.resize(cur_size);

		printf("%d bytes\n", cur_size);

		in.read((char*)&(data[0]), cur_size);
		decrypted_data = decryptor.Decrypt(data);
		cur_size = static_cast<uint32_t>(decrypted_data.size());

		out.write((char*)(&decrypted_data[0]), decrypted_data.size());
		data.clear();

		in.read(reinterpret_cast<char *>(&cur_size), sizeof(uint32_t));
	}

	in.close();
	out.close();
}