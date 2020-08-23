#include "CTextParser.h"

CTextParser::CTextParser()
{
    m_pRawData = nullptr;
    m_iFileSize = m_iFieldBeginPos = m_iFieldEndPos = m_iCurPos = 0;
}

CTextParser::CTextParser(const char* szFileName)
{
    if (!LoadFile(szFileName))
    {
        m_pRawData = nullptr;
        m_iFileSize = m_iFieldBeginPos = m_iFieldEndPos = m_iCurPos = 0;
    }
}

CTextParser::~CTextParser()
{
    if (m_pRawData != nullptr)
        delete[] m_pRawData;
}

BOOL CTextParser::LoadFile(const char* szFileName)
{
    if (m_pRawData != nullptr)
        delete[] m_pRawData;

    FILE* pFile;
    fopen_s(&pFile, szFileName, "rt");
    if (pFile == NULL)
        return FALSE;

    // ���� ũ�⸦ ����
    fseek(pFile, 0, SEEK_END);
    m_iFileSize = m_iFieldBeginPos = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    m_pRawData = new char[m_iFileSize];
    fread(m_pRawData, m_iFileSize, 1, pFile);

    fclose(pFile);
    return TRUE;
    return 0;
}

BOOL CTextParser::SearchField(const char* szFieldName)
{
    char* chpBuff;
    char chWord[en_TOKEN_MAX_LEN];
    int iLength = 0;

    m_iCurPos = m_iFieldBeginPos;
    m_iFieldEndPos = m_iFileSize;

    // �ʵ� �� ���Է� ��
    if (szFieldName == nullptr)
        return FALSE;

    iLength = 0;
    chpBuff = m_pRawData + m_iCurPos;

    while (GetNextToken(&chpBuff, &iLength))
    {
        memset(chWord, 0, en_TOKEN_MAX_LEN);
        memcpy(chWord, chpBuff, iLength);
        chpBuff = m_pRawData + m_iCurPos;

        // ������ "::" Ž��
        if (strcmp(chWord, "::") == 0)
        {
            if (GetNextToken(&chpBuff, &iLength))
            {
                memset(chWord, 0, en_TOKEN_MAX_LEN);
                memcpy(chWord, chpBuff, iLength);
                chpBuff = m_pRawData + m_iCurPos;
                // �ʵ�� Ž��
                if (0 == strcmp(szFieldName, chWord))
                {
                    if (GetNextToken(&chpBuff, &iLength))
                    {
                        memset(chWord, 0, en_TOKEN_MAX_LEN);
                        memcpy(chWord, chpBuff, iLength);
                        chpBuff = m_pRawData + m_iCurPos;

                        // �ʵ� ���� ���� Ž��
                        if (chWord[0] == '{')
                        {
                            m_iFieldBeginPos = m_iCurPos;
                            while (GetNextToken(&chpBuff, &iLength))
                            {
                                memset(chWord, 0, en_TOKEN_MAX_LEN);
                                memcpy(chWord, chpBuff, iLength);
                                chpBuff = m_pRawData + m_iCurPos;

                                // �ʵ� ���� ���� Ž��
                                if (chWord[0] == '}')
                                {
                                    m_iFieldEndPos = m_iCurPos;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    m_iFieldBeginPos = 0;
    m_iFieldEndPos = m_iFileSize;

    return 0;
}

BOOL CTextParser::GetValue(const char* szKey, int* pOutBuf)
{
    char* chpBuff;
    char chWord[en_TOKEN_MAX_LEN];
    int iLength = 0;
    m_iCurPos = m_iFieldBeginPos;
    chpBuff = m_pRawData + m_iCurPos;

    while (GetNextToken(&chpBuff, &iLength))
    {
        memset(chWord, 0, en_TOKEN_MAX_LEN);
        memcpy(chWord, chpBuff, iLength);
        chpBuff = m_pRawData + m_iCurPos;
        if (strcmp(szKey, chWord) == 0)
        {
            if (GetNextToken(&chpBuff, &iLength))
            {
                memset(chWord, 0, en_TOKEN_MAX_LEN);
                memcpy(chWord, chpBuff, iLength);
                chpBuff = m_pRawData + m_iCurPos;
                if (strcmp(chWord, "=") == 0)
                {
                    if (GetNextToken(&chpBuff, &iLength))
                    {
                        memset(chWord, 0, en_TOKEN_MAX_LEN);
                        memcpy(chWord, chpBuff, iLength);
                        *pOutBuf = atoi(chWord);
                        return TRUE;
                    }
                }
            }
            break;
        }
    }
    return FALSE;
}

BOOL CTextParser::SkipNoneCommand()
{
    char* chpBuff = m_pRawData + m_iCurPos;

    while (true)
    {
        if (*chpBuff == ',' || *chpBuff == '"' || *chpBuff == en_TOKEN_SPACE || *chpBuff == en_TOKEN_BACKSPACE || *chpBuff == en_TOKEN_TAB || *chpBuff == en_TOKEN_LF || *chpBuff == en_TOKEN_CR)
        {
            ++m_iCurPos;
            ++chpBuff;
            continue;
        }

        if (*chpBuff == '/')
        {
            ++m_iCurPos;
            ++chpBuff;
            while (true)
            {
                // "//" ����
                if (*chpBuff == '/')
                {
                    while (true)
                    {
                        ++m_iCurPos;
                        ++chpBuff;
                        if (*chpBuff == en_TOKEN_LF || *chpBuff == en_TOKEN_CR)
                        {
                            ++m_iCurPos;
                            ++chpBuff;
                            if (*chpBuff == '/')
                                continue;
                            break;
                        }
                    }

                    ++m_iCurPos;
                    ++chpBuff;
                }
                // "/*" ����
                if (*chpBuff == '*')
                {
                    while (true)
                    {
                        ++m_iCurPos;
                        ++chpBuff;
                        if (*chpBuff == '/')
                        {
                            ++m_iCurPos;
                            ++chpBuff;
                            break;
                        }
                    }
                    ++m_iCurPos;
                    ++chpBuff;
                }
                else
                    break;
            }
        }
        else
        {
            return TRUE;
        }

        if (m_iCurPos > m_iFileSize || m_iFieldEndPos)
            break;
    }
    return FALSE;
}

BOOL CTextParser::GetNextToken(char** chppBuffer, int* pOutSize)
{
    char* pToken;

    if (!SkipNoneCommand())
        return FALSE;

    *chppBuffer = m_pRawData + m_iCurPos;
    pToken = *chppBuffer;
    (*pOutSize) = 0;

    while (true)
    {
        if (*pToken == ',' || *pToken == '"' || *pToken == en_TOKEN_SPACE || *pToken == en_TOKEN_BACKSPACE || *pToken == en_TOKEN_TAB || *pToken == en_TOKEN_LF || *pToken == en_TOKEN_CR)
            break;

        ++pToken;
        ++m_iCurPos;
        ++(*pOutSize);

        if (m_iCurPos > m_iFileSize || m_iFieldEndPos)
            break;
    }

    return TRUE;
    return 0;
}
