#ifndef DESENCRYPT_H_001
#define DESENCRYPT_H_001
#include "types.h"
//#ifdef __IAP3000
//    #include "SFLib.h"
//    #define DES_createKey (((LIB_ASSISTANT_STRUCT*)LIB_ASSISTANT_PTR)->desCreateSubKey)
//    #define DES_encrypt   (((LIB_ASSISTANT_STRUCT*)LIB_ASSISTANT_PTR)->desEncrypt)
//    #define DES_decrypt   (((LIB_ASSISTANT_STRUCT*)LIB_ASSISTANT_PTR)->desDecrypt)
//#else
    #include "desbox.h"
    void DES_createKey(const BYTE* pKey,DESKEYSTRUCT* pKeyInfo);
    void DES_encrypt(BYTE* rawData,BYTE* encryptData,DESKEYSTRUCT* pSubKey);
    void DES_decrypt(BYTE* encryptData,BYTE* rawData,DESKEYSTRUCT* pSubKey);
//#endif

#endif
