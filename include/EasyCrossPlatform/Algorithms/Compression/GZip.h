#ifndef __EASYCROSSPLATFORM_FILE_GZIPH_INCLUDED__
	#define __EASYCROSSPLATFORM_FILE_GZIPH_INCLUDED__
	#include <EasyCrossPlatform/Internal/EasyCP_Common.h>
	#include <EasyCrossPlatform/Basis/String/StringUtil.h>
	namespace EasyCrossPlatform {
        namespace Algorithms{
            namespace Compression {
                namespace GZip { //HTTP Gzip Encoding, Deflate Alg with Gzip Structure.
                    std::vector<byte> Encrypt(const std::vector<byte>& OriginalData, const unsigned int CompressionLevel = 9U); //Compression Level from 1U to 9U
                    std::vector<byte> Decrypt(const std::vector<byte>& EncryptedData);
                }
            }
        }
	}
#endif