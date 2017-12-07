#ifndef __XSYDENCRYPTIONFILE__
	#define __XSYDENCRYPTIONFILE__
	#include "EasyCP_Common.h"
	#include "cryptopp\aes.h"
	#include "cryptopp\des.h"
	#include "cryptopp\modes.h"
	#include "cryptopp\filters.h"
	#include "cryptopp\md5.h"
	#include "cryptopp\sha.h"
	#include "cryptopp\sha3.h"
	#include "cryptopp\hex.h"
	
	namespace EasyCrossPlatform {
		namespace Encryption {
			namespace AES {
				#define EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 16
				#define EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 24
				#define EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256 32
				namespace ProfessionalMode {
					void AES_MakeKey(std::string &strKey, byte* key, unsigned int KeyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);//When u r making a key, make sure that the strKey's length does not exceed the keyLength variable.(Not 128 / 192 / 256 but 16 / 24 / 32)
					void AES_MakeIV(std::string &strIV, byte(&iv)[CryptoPP::AES::BLOCKSIZE]);
					void AES_ECBencrypt(byte *key, std::string& plaintext, std::string& ciphertext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);

					void AES_CBCencrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
						std::string& plaintext, std::string& ciphertext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);

					void AES_CFBencrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
						std::string& plaintext, std::string& ciphertext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);

					void AES_OFBencrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
						std::string& plaintext, std::string& ciphertext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);

					void AES_ECBdecrypt(byte *key, std::string& ciphertext, std::string& dectext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);

					void AES_CBCdecrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
						std::string& ciphertext, std::string& dectext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);

					void AES_CFBdecrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
						std::string& ciphertext, std::string& dectext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);

					void AES_OFBdecrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
						std::string& ciphertext, std::string& dectext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
				}
				namespace EasyMode {
					std::string AES_Encypt_ECB(std::string Password, std::string plaintext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
					std::string AES_Decrypt_ECB(std::string Password, std::string encryptedText, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
					std::string AES_Encrypt_CBC(std::string Password, std::string ivText, std::string plaintext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
					std::string AES_Decrypt_CBC(std::string Password, std::string ivText, std::string encryptedtext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
					std::string AES_Encrypt_CFB(std::string Password, std::string ivText, std::string plaintext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
					std::string AES_Decrypt_CFB(std::string Password, std::string ivText, std::string encryptedtext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
					std::string AES_Encrypt_OFB(std::string Password, std::string ivText, std::string plaintext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
					std::string AES_Decrypt_OFB(std::string Password, std::string ivText, std::string encryptedtext, unsigned int keyLength = EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128);
				}
			}
			namespace Hash {
				std::string getMd5(std::string &data, const char* Salt = "");
				std::string getSHA1(std::string &data, const char* Salt = "");
				std::string getSHA256(std::string &data, const char* Salt = "");
			}
		}
	}
#endif