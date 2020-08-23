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
		// 토큰 버퍼 크기
		en_TOKEN_MAX_LEN = 256,

		// 스킵할 토큰
		en_TOKEN_SPACE = 0x20,
		en_TOKEN_BACKSPACE = 0x08,
		en_TOKEN_TAB = 0x09,
		en_TOKEN_LF = 0x0a,
		en_TOKEN_CR = 0x0d
	};

	// 생성자
	CTextParser();
	CTextParser(const char* szFileName);
	// 파괴자
	virtual ~CTextParser();

	// 파일 불러오기
	// 파싱할 대상 파일을 읽어서 저장한다.
	BOOL LoadFile(const char* szFileName);

	// 필드 찾기
	// '::'를 구분자로 '{'를 필드 시작점으로
	// '}' 필드 종료지점으로 한다.
	BOOL SearchField(const char* szFieldName = nullptr);

	// 값 찾기
	// 먼저 불러온 파일을 파싱하여 입력한 키와 대응하는 값을 찾아낸다.
	// SearchField()에서 정한 필드 내에서 파싱을 시도한다.
	// SearchField()를 호출하지 않았거나 실패했을 경우, 파일의 처음부터 끝까지 검색한다.
	BOOL GetValue(const char* szKey, int* pOutBuf);
	BOOL GetValue(const char* szKey, char* pOutBuf, int iOutBufSize);

protected:
	// 비 명령어 생략
	// 주석, 개행, 스페이스, ""등 토큰을 생략하고, m_iCurPos를 이동시킨다.
	// GetNextToken() 함수 내부에서 호출

	BOOL SkipNoneCommand();

	// 토큰 얻기
	// 다음 토큰을 출력하고, 읽은 토큰 길이만큼 m_iCurPos를 진행시킨다.
	// SearchField() 함수 내부에서 호출
	// GetValue() 함수 내부에서 호출

	BOOL GetNextToken(char** chppBuffer, int* pOutSize);
private:
	int m_iFileSize;		// 파일 크기
	char* m_pRawData;		// 파일에서 읽어온 데이터를 저장

	int m_iFieldBeginPos;	// 데이터 필드의 시작 위치
	int m_iFieldEndPos;		// 데이터 필드의 끝 위치
	int m_iCurPos;			// 현재 읽기 위치
};