#include <EasyCrossPlatform.h>
#include <iostream>

int main(int argc, char** args) {
	/*
	Sometimes the compressed string cannot be correctly displayed. Please set the breakpoint to see the real data
	The more data, the better compression size and percent will be.
	*/
	std::string infoToCompress = "23333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333";
	unsigned int CompressionLevel = 9U; //Compression Level from 1U to 9U
	std::vector<byte> InfoAfterCompression = EasyCrossPlatform::Compression::GZip::Encrypt(EasyCrossPlatform::Parser::StringUtil::toBytes(infoToCompress), CompressionLevel);
	std::cout << "Compressed:";
	for (unsigned int i = 0U; i < InfoAfterCompression.size(); i++) {
		std::cout << static_cast<char>(InfoAfterCompression[i]);
	}
	std::cout << std::endl;
	std::vector<byte> InfoAfterDecompresison;
	try {
		InfoAfterDecompresison = EasyCrossPlatform::Compression::GZip::Decrypt(InfoAfterCompression);
	}
	catch (std::runtime_error e) {
		std::cout << "Error! Block is not a valid compression data!" << std::endl;
	}
	std::string stringAfterDecompression = EasyCrossPlatform::Parser::StringUtil::fromBytes(InfoAfterDecompresison);
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Decompressed:" << stringAfterDecompression << std::endl;
	system("pause");
	return 0;
}