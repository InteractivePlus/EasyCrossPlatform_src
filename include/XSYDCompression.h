#ifndef __XSYDCompressionFILE__
	#define __XSYDCompressionFILE__
	#include <EasyCP_Common.h>
	#include <cryptopp/gzip.h>
	#include <cryptopp/zlib.h>
	#include <cryptopp/filters.h>
	#include <brotli/encode.h>
	#include <brotli/decode.h>
	#include <XSYDStringUtil.h>
	namespace EasyCrossPlatform {
		namespace Compression {
			namespace GZip { //HTTP Gzip Encoding, Deflate Alg with Gzip Structure.
				std::vector<byte> Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel = 9U); //Compression Level from 1U to 9U
				std::vector<byte> Decrypt(const std::vector<byte>& EncryptedData);
			}
			namespace Deflate { //HTTP Deflate Encoding, Deflate Alg with Zlib Structure.
				std::vector<byte> Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel = 9U); //Compression Level from 1U to 9U
				std::vector<byte> Decrypt(const std::vector<byte>& EncryptedData);
			}
			namespace Brotli { //HTTP br Encoding, Brotli Algorithm
				std::vector<byte> Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel = 11U); //Compression Level from 1U to 11U
				std::vector<byte> Decrypt(const std::vector<byte>& EncryptedData);
			}
		}
	}
#endif