#ifndef __XSYDENCRYPTIONFILE__
	#define __XSYDENCRYPTIONFILE__
	#include "EasyCP_Common.h"
	#include "cryptopp/aes.h"
	#include "cryptopp/des.h"
	#include "cryptopp/modes.h"
	#include "cryptopp/filters.h"
	#include "cryptopp/md5.h"
	#include "cryptopp/sha.h"
	#include "cryptopp/sha3.h"
	#include "cryptopp/hex.h"
	#include "cryptopp/base64.h"
	#include "cryptopp/osrng.h"
	#include "cryptopp/integer.h"
	#include "cryptopp/rsa.h"
	
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
			namespace Base64 {
				char const constexpr alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
				char const constexpr padding('=');
				std::string base64Encode(std::string const &plaintext);
				std::string base64Decode(std::string const &encryptedtext);
			}
			namespace RSA {
				#define EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_1024 1024
				#define EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048 2048
				#define EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_3072 3072
				#define EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_4096 4096
				#define EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_5120 5120
				std::string getPrivateKeyAsString(CryptoPP::RSA::PrivateKey const &prvKey);
				std::string getPublicKeyAsString(CryptoPP::RSA::PublicKey const &pubKey);
				CryptoPP::RSA::PrivateKey getPrivateKeyAsCryptoPPType(std::string const &prvKey);
				CryptoPP::RSA::PublicKey getPublicKeyAsCryptoPPType(std::string const &pubKey);
				std::pair<std::string, std::string> generateKeyPairStr(unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::pair<CryptoPP::RSA::PublicKey, CryptoPP::RSA::PrivateKey> generateKeyPair(unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::string pubKeyEncrypt(std::string const &msg, std::string const &pubKey, unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::string pubKeyEncrypt(std::string const &msg, CryptoPP::RSA::PublicKey &pubKey, unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::string pubKeyDecrypt(std::string const &encryptedText, std::string const &pubKey, unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::string pubKeyDecrypt(std::string const &encryptedText, CryptoPP::RSA::PublicKey &pubKey, unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::string privateKeyEncrypt(std::string const &msg, std::string const &prvKey, unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::string privateKeyEncrypt(std::string const &msg, CryptoPP::RSA::PrivateKey &prvKey, unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::string privateKeyDecrypt(std::string const &encryptedText, std::string const &prvKey, unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);
				std::string privateKeyDecrypt(std::string const &encryptedText, CryptoPP::RSA::PrivateKey &prvKey, unsigned int keyLength = EASYCROSSPLATFORM_RSAENCRYPTION_KEYSIZE_2048);

			}
		}
	}
#endif