#include <EasyCrossPlatform/Algorithms/Compression/Brotli.h>
#include <brotli/encode.h>
#include <brotli/decode.h>

std::vector<byte> EasyCrossPlatform::Algorithms::Compression::Brotli::Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel)
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

std::vector<byte> EasyCrossPlatform::Algorithms::Compression::Brotli::Decrypt(const std::vector<byte>& EncryptedData)
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