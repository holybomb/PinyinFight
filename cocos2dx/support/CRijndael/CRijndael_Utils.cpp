/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CRijndael_Utils.h"
#include "CRijndael.h"

#define DEFAULT_KEY_SIZE 16
#define DEFAULT_BLOCK_SIZE 16

namespace   cocos2d {

static CRijndael* AES = NULL;

void CRijndael_Utils::MakeKeyAES(const char* password)
{
	if (AES) delete AES;

	AES = new CRijndael;
	// ��������
	AES->MakeKey(password, CRijndael::sm_chain0);
	int length = AES->GetKeyLength();
	if (DEFAULT_KEY_SIZE != length)
	{
		printf("Get default key length is error./n");
	}
}

//////////////////////////////////////////////////////////////////////////
// I ��ͨ�ӽ���
// ����block���ӣ�����block�Ĳ���
// ʡ�ڴ�
void CRijndael_Utils::EncryptAES(char* pBuffer, size_t size)
{
	if (AES == NULL) return;
	// ����
	AES->Encrypt(pBuffer, pBuffer, size);
}

void CRijndael_Utils::DecryptAES(char* pBuffer, size_t size)
{
	if (AES == NULL) return;
	// ����
	AES->Decrypt(pBuffer, pBuffer, size);
}

//////////////////////////////////////////////////////////////////////////
// II ��ȫ�ӽ���
// �Զ����䵽block��������
// ��Ҫ����һ�����������
char* CRijndael_Utils::FullEncryptAES(char* pBuffer, size_t size, size_t& outLen)
{	
	// ����
	if (AES == NULL) return NULL;

	outLen = padded_size(size);
	size_t padding_bytes = outLen - size;

	char *tmp;
	char pad = (padding_bytes % DEFAULT_BLOCK_SIZE);

	tmp = new char[outLen + 1];
	memcpy(tmp, pBuffer, size);
	memset(tmp + size, pad, padding_bytes);
	memset(tmp + outLen, 0, 1);
	AES->Encrypt(tmp, tmp, outLen);
	return tmp;
}

char* CRijndael_Utils::FullDecryptAES(char* pBuffer, size_t size, size_t& outLen)
{
	// ����
	if (AES == NULL) return NULL;

	AES->Decrypt(pBuffer, pBuffer, size);

	int padding_bytes = pBuffer[size - 1];
	char *tmp;

	/* a buffer with size % BLOCKSIZE == 0 is padded with a
	 * full block with zeros
	 */
	if (padding_bytes == 0)
		padding_bytes = 16;

	outLen = size - padding_bytes;
	tmp = new char[outLen + 1];
	memcpy(tmp, pBuffer, outLen);
	memset(tmp + outLen, 0, 1);
	return tmp;
}

int CRijndael_Utils::padded_size(int size) {

	int nblocks = size / DEFAULT_BLOCK_SIZE;
	return (nblocks + 1) * DEFAULT_BLOCK_SIZE;
}

}//namespace   cocos2d 