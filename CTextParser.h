#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

class CTextParser
{
public:
	enum en_PARSER
	{
		// ��ū ���� ũ��
		en_TOKEN_MAX_LEN = 256,

		// ��ŵ�� ��ū
		en_TOKEN_SPACE = 0x20,
		en_TOKEN_BACKSPACE = 0x08,
		en_TOKEN_TAB = 0x09,
		en_TOKEN_LF = 0x0a,
		en_TOKEN_CR = 0x0d
	};

	// ������
	CTextParser();
	CTextParser(const char* szFileName);
	// �ı���
	virtual ~CTextParser();

	// ���� �ҷ�����
	// �Ľ��� ��� ������ �о �����Ѵ�.
	BOOL LoadFile(const char* szFileName);

	// �ʵ� ã��
	// '::'�� �����ڷ� '{'�� �ʵ� ����������
	// '}' �ʵ� ������������ �Ѵ�.
	BOOL SearchField(const char* szFieldName = nullptr);

	// �� ã��
	// ���� �ҷ��� ������ �Ľ��Ͽ� �Է��� Ű�� �����ϴ� ���� ã�Ƴ���.
	// SearchField()���� ���� �ʵ� ������ �Ľ��� �õ��Ѵ�.
	// SearchField()�� ȣ������ �ʾҰų� �������� ���, ������ ó������ ������ �˻��Ѵ�.
	BOOL GetValue(const char* szKey, int* pOutBuf);
	BOOL GetValue(const char* szKey, char* pOutBuf, int iOutBufSize);

protected:
	// �� ��ɾ� ����
	// �ּ�, ����, �����̽�, ""�� ��ū�� �����ϰ�, m_iCurPos�� �̵���Ų��.
	// GetNextToken() �Լ� ���ο��� ȣ��

	BOOL SkipNoneCommand();

	// ��ū ���
	// ���� ��ū�� ����ϰ�, ���� ��ū ���̸�ŭ m_iCurPos�� �����Ų��.
	// SearchField() �Լ� ���ο��� ȣ��
	// GetValue() �Լ� ���ο��� ȣ��

	BOOL GetNextToken(char** chppBuffer, int* pOutSize);
private:
	int m_iFileSize;		// ���� ũ��
	char* m_pRawData;		// ���Ͽ��� �о�� �����͸� ����

	int m_iFieldBeginPos;	// ������ �ʵ��� ���� ��ġ
	int m_iFieldEndPos;		// ������ �ʵ��� �� ��ġ
	int m_iCurPos;			// ���� �б� ��ġ
};