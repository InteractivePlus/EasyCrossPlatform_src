#include <XSYDCompression.h>
#include <iostream>

std::vector<byte> EasyCrossPlatform::Compression::GZip::Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel)
{
	std::string EncryptedData = "";
	CryptoPP::Gzip mEncoder(new CryptoPP::StringSink(EncryptedData),CompressionLevel);
	for (size_t i = 0; i < OriginalData.size(); i++) {
		mEncoder.Put(OriginalData[i], true);
	}
	mEncoder.MessageEnd(-1,true);
	return EasyCrossPlatform::Parser::StringUtil::toBytes(EncryptedData);
}

std::vector<byte> EasyCrossPlatform::Compression::GZip::Decrypt(const std::vector<byte>& EncryptedData)
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
	return EasyCrossPlatform::Parser::StringUtil::toBytes(DecryptedData);
}

std::vector<byte> EasyCrossPlatform::Compression::Deflate::Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel)
{
	std::string EncryptedData = "";
	CryptoPP::ZlibCompressor mEncoder(new CryptoPP::StringSink(EncryptedData),CompressionLevel);
	for (size_t i = 0; i < OriginalData.size(); i++) {
		mEncoder.Put(OriginalData[i], true);
	}
	mEncoder.MessageEnd(-1, true);

	return EasyCrossPlatform::Parser::StringUtil::toBytes(EncryptedData);
}

std::vector<byte> EasyCrossPlatform::Compression::Deflate::Decrypt(const std::vector<byte>& EncryptedData)
{
	std::string DecryptedData = "";
	CryptoPP::ZlibDecompressor mDecoder(new CryptoPP::StringSink(DecryptedData));
	for (size_t i = 0; i < EncryptedData.size(); i++) {
		try {
			mDecoder.Put(EncryptedData[i], true);
		}
		catch (...) {
			throw std::runtime_error("Data is not a valid compressed data");
		}
	}
	mDecoder.MessageEnd(-1, true);
	return EasyCrossPlatform::Parser::StringUtil::toBytes(DecryptedData);
}

std::vector<byte> EasyCrossPlatform::Compression::Brotli::Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel)
{
	std::vector<byte> mResult = std::vector<byte>();
	size_t mArrLength = OriginalData.size();
	uint8_t* mArray = new uint8_t[mArrLength];
	size_t newArrLength = mArrLength * 3U;
	uint8_t* mNewArray = new uint8_t[newArrLength];
	
	int mCompressionLevel = static_cast<int>(std::round(static_cast<double>(CompressionLevel) * 11.0 / 9.0));

	for (unsigned int i = 0U; i < mArrLength; i++) {
		mArray[i] = static_cast<uint8_t>(OriginalData[i]);
	}
	
	int CResult = BrotliEncoderCompress(mCompressionLevel,BROTLI_DEFAULT_WINDOW,BROTLI_DEFAULT_MODE,mArrLength,mArray,&newArrLength,mNewArray);
	if (CResult == BROTLI_FALSE) {
		mResult.clear();
	}
	else { //CResult == BROTLI_TRUE
		mResult.reserve(newArrLength);
		for (unsigned int i = 0U; i < newArrLength; i++) {
			mResult.push_back(static_cast<byte>(mNewArray[i]));
		}
	}
	delete[] mArray;
	delete[] mNewArray;
	return mResult;
}

std::vector<byte> EasyCrossPlatform::Compression::Brotli::Decrypt(const std::vector<byte>& EncryptedData)
{
	std::vector<byte> mResult = std::vector<byte>();
	size_t mArrLength = EncryptedData.size();
	uint8_t* mArray = new uint8_t[mArrLength];
	size_t newArrLength = mArrLength * 20U;
	uint8_t* mNewArray = new uint8_t[newArrLength];
	
	for (unsigned int i = 0U; i < mArrLength; i++) {
		mArray[i] = static_cast<uint8_t>(EncryptedData[i]);
	}

	BrotliDecoderResult CResult = BrotliDecoderDecompress(mArrLength, mArray, &newArrLength, mNewArray);
	if (CResult == BROTLI_DECODER_RESULT_ERROR) {
		throw std::runtime_error("Data is not a valid compressed data");
		mResult.clear();
	}
	else { //CResult == BROTLI_DECODER_RESULT_SUCCESS
		mResult.reserve(newArrLength);
		for (unsigned int i = 0U; i < newArrLength; i++) {
			mResult.push_back(static_cast<byte>(mNewArray[i]));
		}
	}
	delete[] mArray;
	delete[] mNewArray;
	return mResult;
}
