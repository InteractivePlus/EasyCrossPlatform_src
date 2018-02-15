#include <XSYDEncryption.h>


void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_MakeKey(std::string &strKey, byte * key, unsigned int KeyLength)
{
	unsigned int strKeyXH = 0;
	const char* mystrKeyArray = strKey.c_str();
	for(strKeyXH = 0; strKeyXH < strKey.length(); strKeyXH++) {
		key[strKeyXH] = (byte)mystrKeyArray[strKeyXH];
	}
	memset(key + strKey.length(), 0x00, KeyLength - strKey.length());
}

void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_MakeIV(std::string &strIV, byte(&iv)[CryptoPP::AES::BLOCKSIZE])
{
	unsigned int strIVXH = 0;
	const char* mystrIVArray = strIV.c_str();
	for (strIVXH = 0; strIVXH < strIV.length(); strIVXH++) {
		iv[strIVXH] = (byte)mystrIVArray[strIVXH];
	}
	memset(iv + strIV.length(), 0x00, CryptoPP::AES::BLOCKSIZE - strIV.length());
}

void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_ECBencrypt(byte *key, std::string& plaintext, std::string& ciphertext, unsigned int keyLength)
{
	CryptoPP::AES::Encryption AESenc(key, keyLength);
	CryptoPP::ECB_Mode_ExternalCipher::Encryption ECBenc(AESenc);
	CryptoPP::StreamTransformationFilter stfEncryptor(ECBenc, new CryptoPP::StringSink(ciphertext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
	stfEncryptor.MessageEnd();
}


void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_CBCencrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
	std::string& plaintext, std::string& ciphertext, unsigned int keyLength)
{
	CryptoPP::AES::Encryption AESenc(key, keyLength);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption CBCenc(AESenc, iv);
	CryptoPP::StreamTransformationFilter stfEncryptor(CBCenc, new CryptoPP::StringSink(ciphertext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
	stfEncryptor.MessageEnd();
}


void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_CFBencrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
	std::string& plaintext, std::string& ciphertext, unsigned int keyLength)
{
	CryptoPP::AES::Encryption AESenc(key, keyLength);
	CryptoPP::CFB_Mode_ExternalCipher::Encryption CFBenc(AESenc, iv);
	CryptoPP::StreamTransformationFilter stfEncryptor(CFBenc, new CryptoPP::StringSink(ciphertext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
	stfEncryptor.MessageEnd();
}

void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_OFBencrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
	std::string& plaintext, std::string& ciphertext, unsigned int keyLength)
{
	CryptoPP::AES::Encryption AESenc(key, keyLength);
	CryptoPP::OFB_Mode_ExternalCipher::Encryption OFBenc(AESenc, iv);
	CryptoPP::StreamTransformationFilter stfEncryptor(OFBenc, new CryptoPP::StringSink(ciphertext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
	stfEncryptor.MessageEnd();
}
void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_ECBdecrypt(byte *key, std::string& ciphertext, std::string& dectext, unsigned int keyLength)
{
	CryptoPP::AES::Decryption AESdec(key, keyLength);
	CryptoPP::ECB_Mode_ExternalCipher::Decryption ECBdec(AESdec);
	CryptoPP::StreamTransformationFilter stfDecryptor(ECBdec, new CryptoPP::StringSink(dectext));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
	stfDecryptor.MessageEnd();
}


void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_CBCdecrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
	std::string& ciphertext, std::string& dectext, unsigned int keyLength)
{
	CryptoPP::AES::Decryption AESdec(key, keyLength);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption CBCdec(AESdec, iv);
	CryptoPP::StreamTransformationFilter stfDecryptor(CBCdec, new CryptoPP::StringSink(dectext));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
	stfDecryptor.MessageEnd();
}


void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_CFBdecrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
	std::string& ciphertext, std::string& dectext, unsigned int keyLength)
{
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption CFBdec(key, keyLength, iv);
	CryptoPP::StreamTransformationFilter stfDecryptor(CFBdec, new CryptoPP::StringSink(dectext));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
	stfDecryptor.MessageEnd();
}


void EasyCrossPlatform::Encryption::AES::ProfessionalMode::AES_OFBdecrypt(byte *key, byte(&iv)[CryptoPP::AES::BLOCKSIZE],
	std::string& ciphertext, std::string& dectext, unsigned int keyLength)
{
	CryptoPP::OFB_Mode<CryptoPP::AES>::Decryption CFBdec(key, keyLength, iv);
	CryptoPP::StreamTransformationFilter stfDecryptor(CFBdec, new CryptoPP::StringSink(dectext));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
	stfDecryptor.MessageEnd();
}

std::string EasyCrossPlatform::Encryption::AES::EasyMode::AES_Encypt_ECB(std::string Password, std::string plaintext, unsigned int keyLength)
{
	if (Password.length() > keyLength) {
		throw std::runtime_error("The Length of Encryption Key cannot exeed the keyLength");
	}
	else if (keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256) {
		throw std::runtime_error("The Length of Encryption Key specified is invalid");
	}
	else if (plaintext.empty()) {
		throw std::runtime_error("The data prepared for encryption should not be empty");
	}
	std::string tempResult;
	byte *myKey = new byte[keyLength];
	ProfessionalMode::AES_MakeKey(Password, myKey, keyLength);
	ProfessionalMode::AES_ECBencrypt(myKey, plaintext, tempResult, keyLength);
	delete[] myKey;
	return tempResult;
}

std::string EasyCrossPlatform::Encryption::AES::EasyMode::AES_Decrypt_ECB(std::string Password, std::string encryptedText, unsigned int keyLength)
{
	if (Password.length() > keyLength) {
		throw std::runtime_error("The Length of Encryption Key cannot exeed the keyLength");
	}
	else if (keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256) {
		throw std::runtime_error("The Length of Encryption Key specified is invalid");
	}
	else if (encryptedText.empty()) {
		throw std::runtime_error("The data prepared for decryption should not be empty");
	}
	std::string tempResult;
	byte *myKey = new byte[keyLength];
	ProfessionalMode::AES_MakeKey(Password, myKey, keyLength);
	ProfessionalMode::AES_ECBdecrypt(myKey, encryptedText, tempResult, keyLength);
	delete[] myKey;
	return tempResult;
}

std::string EasyCrossPlatform::Encryption::AES::EasyMode::AES_Encrypt_CBC(std::string Password, std::string ivText, std::string plaintext, unsigned int keyLength)
{
	if (Password.length() > keyLength) {
		throw std::runtime_error("The Length of Encryption Key cannot exeed the keyLength");
	}
	else if (keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256) {
		throw std::runtime_error("The Length of Encryption Key specified is invalid");
	}
	else if (ivText.length() > CryptoPP::AES::BLOCKSIZE) {
		throw std::runtime_error("The Length of Encryption IV cannot exeed the Crypto::AES::BLOCKSIZE Limit");
	}
	else if (plaintext.empty()) {
		throw std::runtime_error("The data prepared for encryption should not be empty");
	}
	std::string tempResult;
	byte *myKey = new byte[keyLength];
	byte myIV[CryptoPP::AES::BLOCKSIZE];
	ProfessionalMode::AES_MakeKey(Password, myKey, keyLength);
	ProfessionalMode::AES_MakeIV(ivText, myIV);
	ProfessionalMode::AES_CBCencrypt(myKey, myIV, plaintext, tempResult, keyLength);
	delete[] myKey;
	return tempResult;
}

std::string EasyCrossPlatform::Encryption::AES::EasyMode::AES_Decrypt_CBC(std::string Password, std::string ivText, std::string encryptedtext, unsigned int keyLength)
{
	if (Password.length() > keyLength) {
		throw std::runtime_error("The Length of Encryption Key cannot exeed the keyLength");
	}
	else if (keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256) {
		throw std::runtime_error("The Length of Encryption Key specified is invalid");
	}
	else if (ivText.length() > CryptoPP::AES::BLOCKSIZE) {
		throw std::runtime_error("The Length of Encryption IV cannot exeed the Crypto::AES::BLOCKSIZE Limit");
	}
	else if (encryptedtext.empty()) {
		throw std::runtime_error("The data prepared for decryption should not be empty");
	}
	std::string tempResult;
	byte *myKey = new byte[keyLength];
	byte myIV[CryptoPP::AES::BLOCKSIZE];
	ProfessionalMode::AES_MakeKey(Password, myKey, keyLength);
	ProfessionalMode::AES_MakeIV(ivText, myIV);
	ProfessionalMode::AES_CBCdecrypt(myKey, myIV, encryptedtext, tempResult, keyLength);
	delete[] myKey;
	return tempResult;
}

std::string EasyCrossPlatform::Encryption::AES::EasyMode::AES_Encrypt_CFB(std::string Password, std::string ivText, std::string plaintext, unsigned int keyLength)
{
	if (Password.length() > keyLength) {
		throw std::runtime_error("The Length of Encryption Key cannot exeed the keyLength");
	}
	else if (keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256) {
		throw std::runtime_error("The Length of Encryption Key specified is invalid");
	}
	else if (ivText.length() > CryptoPP::AES::BLOCKSIZE) {
		throw std::runtime_error("The Length of Encryption IV cannot exeed the Crypto::AES::BLOCKSIZE Limit");
	}
	else if (plaintext.empty()) {
		throw std::runtime_error("The data prepared for encryption should not be empty");
	}
	std::string tempResult;
	byte *myKey = new byte[keyLength];
	byte myIV[CryptoPP::AES::BLOCKSIZE];
	ProfessionalMode::AES_MakeKey(Password, myKey, keyLength);
	ProfessionalMode::AES_MakeIV(ivText, myIV);
	ProfessionalMode::AES_CFBencrypt(myKey, myIV, plaintext, tempResult, keyLength);
	delete[] myKey;
	return tempResult;
}

std::string EasyCrossPlatform::Encryption::AES::EasyMode::AES_Decrypt_CFB(std::string Password, std::string ivText, std::string encryptedtext, unsigned int keyLength)
{
	if (Password.length() > keyLength) {
		throw std::runtime_error("The Length of Encryption Key cannot exeed the keyLength");
	}
	else if (keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256) {
		throw std::runtime_error("The Length of Encryption Key specified is invalid");
	}
	else if (ivText.length() > CryptoPP::AES::BLOCKSIZE) {
		throw std::runtime_error("The Length of Encryption IV cannot exeed the Crypto::AES::BLOCKSIZE Limit");
	}
	else if (encryptedtext.empty()) {
		throw std::runtime_error("The data prepared for decryption should not be empty");
	}
	std::string tempResult;
	byte *myKey = new byte[keyLength];
	byte myIV[CryptoPP::AES::BLOCKSIZE];
	ProfessionalMode::AES_MakeKey(Password, myKey, keyLength);
	ProfessionalMode::AES_MakeIV(ivText, myIV);
	ProfessionalMode::AES_CFBdecrypt(myKey, myIV, encryptedtext, tempResult, keyLength);
	delete[] myKey;
	return tempResult;
}

std::string EasyCrossPlatform::Encryption::AES::EasyMode::AES_Encrypt_OFB(std::string Password, std::string ivText, std::string plaintext, unsigned int keyLength)
{
	if (Password.length() > keyLength) {
		throw std::runtime_error("The Length of Encryption Key cannot exeed the keyLength");
	}
	else if (keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256) {
		throw std::runtime_error("The Length of Encryption Key specified is invalid");
	}
	else if (ivText.length() > CryptoPP::AES::BLOCKSIZE) {
		throw std::runtime_error("The Length of Encryption IV cannot exeed the Crypto::AES::BLOCKSIZE Limit");
	}
	else if (plaintext.empty()) {
		throw std::runtime_error("The data prepared for encryption should not be empty");
	}
	std::string tempResult;
	byte *myKey = new byte[keyLength];
	byte myIV[CryptoPP::AES::BLOCKSIZE];
	ProfessionalMode::AES_MakeKey(Password, myKey, keyLength);
	ProfessionalMode::AES_MakeIV(ivText, myIV);
	ProfessionalMode::AES_OFBencrypt(myKey, myIV, plaintext, tempResult, keyLength);
	delete[] myKey;
	return tempResult;
}

std::string EasyCrossPlatform::Encryption::AES::EasyMode::AES_Decrypt_OFB(std::string Password, std::string ivText, std::string encryptedtext, unsigned int keyLength)
{
	if (Password.length() > keyLength) {
		throw std::runtime_error("The Length of Encryption Key cannot exeed the keyLength");
	}
	else if (keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_192 && keyLength != EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_256) {
		throw std::runtime_error("The Length of Encryption Key specified is invalid");
	}
	else if (ivText.length() > CryptoPP::AES::BLOCKSIZE) {
		throw std::runtime_error("The Length of Encryption IV cannot exeed the Crypto::AES::BLOCKSIZE Limit");
	}
	else if (encryptedtext.empty()) {
		throw std::runtime_error("The data prepared for decryption should not be empty");
	}
	std::string tempResult;
	byte *myKey = new byte[keyLength];
	byte myIV[CryptoPP::AES::BLOCKSIZE];
	ProfessionalMode::AES_MakeKey(Password, myKey, keyLength);
	ProfessionalMode::AES_MakeIV(ivText, myIV);
	ProfessionalMode::AES_OFBdecrypt(myKey, myIV, encryptedtext, tempResult, keyLength);
	delete[] myKey;
	return tempResult;
}

std::string EasyCrossPlatform::Encryption::Hash::getMd5(std::string &data, const char* Salt)
{
	if (data.empty()) {
		throw std::runtime_error("Sorry, but original data for hash should not be empty");
	}
	std::string tmpData(data);
	if (Salt != NULL) {
		tmpData += Salt;
	}
	CryptoPP::MD5 hash;
	byte digest[CryptoPP::MD5::DIGESTSIZE];

	hash.CalculateDigest(digest, (byte*)tmpData.c_str(), tmpData.length());

	CryptoPP::HexEncoder encoder;
	std::string output;
	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	return output;
}

std::string EasyCrossPlatform::Encryption::Hash::getSHA1(std::string & data, const char* Salt)
{
	if (data.empty()) {
		throw std::runtime_error("Sorry, but original data for hash should not be empty");
	}
	std::string tmpData(data);
	if (Salt != NULL) {
		tmpData += Salt;
	}
	CryptoPP::SHA1 hash;
	byte digest[CryptoPP::SHA1::DIGESTSIZE];

	hash.CalculateDigest(digest, (byte*)tmpData.c_str(), tmpData.length());

	CryptoPP::HexEncoder encoder;
	std::string output;
	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	return output;
}

std::string EasyCrossPlatform::Encryption::Hash::getSHA256(std::string & data, const char* Salt)
{
	if (data.empty()) {
		throw std::runtime_error("Sorry, but original data for hash should not be empty");
	}
	std::string tmpData(data);
	if (Salt != NULL) {
		tmpData += Salt;
	}
	CryptoPP::SHA256 hash;
	byte digest[CryptoPP::SHA256::DIGESTSIZE];

	hash.CalculateDigest(digest, (byte*)tmpData.c_str(), tmpData.length());

	CryptoPP::HexEncoder encoder;
	std::string output;
	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	return output;
}

std::string EasyCrossPlatform::Encryption::Base64::base64Encode(std::string const & plaintext)
{
	if (plaintext.empty()) {
		throw std::runtime_error("Sorry, but encode content should not be empty");
	}
	std::string out;

	int val = 0;
	int valb = -6;
	for (unsigned char c : plaintext)
	{
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back(EasyCrossPlatform::Encryption::Base64::alphabet[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6)
		out.push_back(EasyCrossPlatform::Encryption::Base64::alphabet[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4)
		out.push_back(EasyCrossPlatform::Encryption::Base64::padding);
	return out;
}

std::string EasyCrossPlatform::Encryption::Base64::base64Decode(std::string const & encryptedtext)
{
	if (encryptedtext.empty()) {
		throw std::runtime_error("Sorry, but the decode content should not be empty");
	}
	std::string out;
	std::vector<int> T(256, -1);

	for (int i(0); i < 64; ++i)
		T[EasyCrossPlatform::Encryption::Base64::alphabet[i]] = i;

	int val = 0;
	int valb = -8;
	for (unsigned char c : encryptedtext)
	{
		if (T[c] == -1)
			break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0)
		{
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}

std::string EasyCrossPlatform::Encryption::RSA::getPrivateKeyAsString(CryptoPP::RSA::PrivateKey const & prvKey)
{
	std::string out;

	CryptoPP::StringSink stringSink(out);
	prvKey.DEREncode(stringSink);
	out = EasyCrossPlatform::Encryption::Base64::base64Encode(out);
	return out;
}

std::string EasyCrossPlatform::Encryption::RSA::getPublicKeyAsString(CryptoPP::RSA::PublicKey const & pubKey)
{
	std::string out;

	CryptoPP::StringSink stringSink(out);
	pubKey.DEREncode(stringSink);
	out = EasyCrossPlatform::Encryption::Base64::base64Encode(out);
	return out;
}

CryptoPP::RSA::PrivateKey EasyCrossPlatform::Encryption::RSA::getPrivateKeyAsCryptoPPType(std::string const & prvKey)
{
	CryptoPP::RSA::PrivateKey key;

	// de-base54 user key
	std::string prvKeyDecoded = EasyCrossPlatform::Encryption::Base64::base64Decode(prvKey);
	// create cryptopp rsa key out of user key (decoded string)
	CryptoPP::StringSource stringSource(prvKeyDecoded, true);
	key.BERDecode(stringSource);
	return key;
}

CryptoPP::RSA::PublicKey EasyCrossPlatform::Encryption::RSA::getPublicKeyAsCryptoPPType(std::string const & pubKey)
{
	CryptoPP::RSA::PublicKey key;

	// de-base64 user key
	std::string pubKeyDecoded = EasyCrossPlatform::Encryption::Base64::base64Decode(pubKey);
	// create cryptopp rsa key out of user key (decoded string)
	CryptoPP::StringSource stringSource(pubKeyDecoded, true);
	key.BERDecode(stringSource);
	return key;
}

std::pair<std::string, std::string> EasyCrossPlatform::Encryption::RSA::generateKeyPairStr(unsigned int keyLength)
{
	std::pair<CryptoPP::RSA::PublicKey, CryptoPP::RSA::PrivateKey> myKeyResult = generateKeyPair(keyLength);
	return std::make_pair<std::string, std::string>(getPublicKeyAsString(myKeyResult.first), getPrivateKeyAsString(myKeyResult.second));
}

std::pair<CryptoPP::RSA::PublicKey, CryptoPP::RSA::PrivateKey> EasyCrossPlatform::Encryption::RSA::generateKeyPair(unsigned int keyLength)
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction params;
	CryptoPP::RSA::PrivateKey prvKey;
	CryptoPP::RSA::PublicKey pubKey;
	params.GenerateRandomWithKeySize(rng, keyLength); // keySize >= msglen * 8
	prvKey = CryptoPP::RSA::PrivateKey(params);
	pubKey = CryptoPP::RSA::PublicKey(params);
	return std::pair<CryptoPP::RSA::PublicKey, CryptoPP::RSA::PrivateKey>(pubKey,prvKey);
}

std::string EasyCrossPlatform::Encryption::RSA::pubKeyEncrypt(std::string const & msg, std::string const & pubKey, unsigned int keyLength)
{
	//MsgLen <= keySize / 8
	//Data split.
	std::string output;

	CryptoPP::RSA::PublicKey myPubKey = getPublicKeyAsCryptoPPType(pubKey);
	output = pubKeyEncrypt(msg, myPubKey, keyLength);
	return output;
}

std::string EasyCrossPlatform::Encryption::RSA::pubKeyEncrypt(std::string const & msg, CryptoPP::RSA::PublicKey & pubKey, unsigned int keyLength)
{
	CryptoPP::AutoSeededRandomPool rng;
	std::string output;

	CryptoPP::RSAES_OAEP_SHA_Encryptor myEncryptor(pubKey);

	if (msg.length() > myEncryptor.FixedMaxPlaintextLength()) {
		output = pubKeyEncrypt(msg.substr(0, myEncryptor.FixedMaxPlaintextLength()), pubKey, keyLength) + pubKeyEncrypt(msg.substr(myEncryptor.FixedMaxPlaintextLength()),pubKey,keyLength);
	}
	else {
		CryptoPP::StringSource ss1(msg, true,
			new CryptoPP::PK_EncryptorFilter(rng, myEncryptor,
				new CryptoPP::StringSink(output)
			) // PK_EncryptorFilter
		); // StringSource
	}
	return output;
}

std::string EasyCrossPlatform::Encryption::RSA::pubKeyDecrypt(std::string const & encryptedText, std::string const & pubKey, unsigned int keyLength)
{
	std::string output;

	CryptoPP::RSA::PublicKey myPubKey = getPublicKeyAsCryptoPPType(pubKey);
	output = pubKeyDecrypt(encryptedText, myPubKey, keyLength);
	return output;
}

std::string EasyCrossPlatform::Encryption::RSA::pubKeyDecrypt(std::string const & encryptedText, CryptoPP::RSA::PublicKey & pubKey, unsigned int keyLength)
{
	CryptoPP::AutoSeededRandomPool rng;
	std::string output;
	CryptoPP::RSAES_OAEP_SHA_Decryptor myDecryptor(pubKey);
	if (encryptedText.length() > myDecryptor.FixedCiphertextLength()) {
		output = pubKeyDecrypt(encryptedText.substr(0, myDecryptor.FixedCiphertextLength()), pubKey, keyLength) + pubKeyDecrypt(encryptedText.substr(myDecryptor.FixedCiphertextLength()), pubKey, keyLength);
	}
	else {
		CryptoPP::StringSource ss2(encryptedText, true,
			new CryptoPP::PK_DecryptorFilter(rng, myDecryptor,
				new CryptoPP::StringSink(output)
			) // PK_DecryptorFilter
		); // StringSource
	}
	return output;
}

std::string EasyCrossPlatform::Encryption::RSA::privateKeyEncrypt(std::string const & msg, std::string const & prvKey, unsigned int keyLength)
{
	std::string output;

	CryptoPP::RSA::PrivateKey myPrvKey = getPrivateKeyAsCryptoPPType(prvKey);
	output = pubKeyEncrypt(msg, myPrvKey, keyLength);
	return output;
}

std::string EasyCrossPlatform::Encryption::RSA::privateKeyEncrypt(std::string const & msg, CryptoPP::RSA::PrivateKey & prvKey, unsigned int keyLength)
{
	CryptoPP::AutoSeededRandomPool rng;
	std::string output;

	CryptoPP::RSAES_OAEP_SHA_Encryptor myEncryptor(prvKey);

	if (msg.length() > myEncryptor.FixedMaxPlaintextLength()) {
		output = privateKeyEncrypt(msg.substr(0, myEncryptor.FixedMaxPlaintextLength()), prvKey, keyLength) + pubKeyEncrypt(msg.substr(myEncryptor.FixedMaxPlaintextLength()), prvKey, keyLength);
	}
	else {
		CryptoPP::StringSource ss1(msg, true,
			new CryptoPP::PK_EncryptorFilter(rng, myEncryptor,
				new CryptoPP::StringSink(output)
			) // PK_EncryptorFilter
		); // StringSource
	}
	return output;
}

std::string EasyCrossPlatform::Encryption::RSA::privateKeyDecrypt(std::string const & encryptedText, std::string const & prvKey, unsigned int keyLength)
{
	std::string output;

	CryptoPP::RSA::PrivateKey myPrvKey = getPrivateKeyAsCryptoPPType(prvKey);
	output = pubKeyDecrypt(encryptedText, myPrvKey, keyLength);
	return output;
}

std::string EasyCrossPlatform::Encryption::RSA::privateKeyDecrypt(std::string const & encryptedText, CryptoPP::RSA::PrivateKey & prvKey, unsigned int keyLength)
{
	CryptoPP::AutoSeededRandomPool rng;
	std::string output;
	CryptoPP::RSAES_OAEP_SHA_Decryptor myDecryptor(prvKey);
	if (encryptedText.length() > myDecryptor.FixedCiphertextLength()) {
		output = pubKeyDecrypt(encryptedText.substr(0, myDecryptor.FixedCiphertextLength()), prvKey, keyLength) + pubKeyDecrypt(encryptedText.substr(myDecryptor.FixedCiphertextLength()), prvKey, keyLength);
	}
	else {
		CryptoPP::StringSource ss2(encryptedText, true,
			new CryptoPP::PK_DecryptorFilter(rng, myDecryptor,
				new CryptoPP::StringSink(output)
			) // PK_DecryptorFilter
		); // StringSource
	}
	return output;
}
