#include "EasyCrossPlatform.h"
#include <iostream>
//AES Encryption Test
int main(int argc, char** args){
	std::string myInput;
	std::string myEncryptionPassword;
	std::cout << "Please enter the string for Encryption.(50 characters limit)" << std::endl;
	std::cin >> myInput;
	std::cout << "Please enter the password for Encryption.(16 characters limit)" << std::endl;
	std::cin >> myEncryptionPassword;
	std::cout << "Encryption Result: " << EasyCrossPlatform::Encryption::AES::EasyMode::AES_Encypt_ECB(myEncryptionPassword,myInput,EasyCrossPlatform::Encryption::AES::EASYCROSSPLATFORM_AESENCRYPTION_KEYSIZE_128) << std::endl;
	return 0;
}