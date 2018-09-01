#include <EasyCrossPlatform/Algorithms/Compression/Gzip.h>
#include <cryptopp/gzip.h>
#include <cryptopp/filters.h>

std::vector<byte> EasyCrossPlatform::Algorithms::Compression::GZip::Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel)
{
	std::string EncryptedData = "";
	CryptoPP::Gzip mEncoder(new CryptoPP::StringSink(EncryptedData),CompressionLevel);
	for (size_t i = 0; i < OriginalData.size(); i++) {
		mEncoder.Put(OriginalData[i], true);
	}
	mEncoder.MessageEnd(-1,true);
	return EasyCrossPlatform::Basis::String::StringUtil::toBytes(EncryptedData);
}

std::vector<byte> EasyCrossPlatform::Algorithms::Compression::GZip::Decrypt(const std::vector<byte>& EncryptedData)
{
	std::string DecryptedData = "";
	CryptoPP::Gunzip mDecoder(new CryptoPP::StringSink(DecryptedData));

	for (size_t i = 0; i < EncryptedData.size(); i++) {
		try {
			mDecoder.Put(EncryptedData[i], true);
		}
		catch (...) {
			throw std::runtime_error("Data is not a valid compressed data");
		}
	}
	mDecoder.MessageEnd(-1, true);
	return EasyCrossPlatform::Basis::String::StringUtil::toBytes(DecryptedData);
}