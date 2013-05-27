//////////////////////////////////////////////////////////////////////////
/*
�Դ������Ĺ���Լ��С��0ʱ��ʾ���ִ��󣬴������Ϣ�ɸù�������¼��
����������ʱ�ԵĹ�������Ҫ���л����͡�
*/
//////////////////////////////////////////////////////////////////////////
#ifndef _ERRORCODE_H
#define _ERRORCODE_H

#include <string>
#include <map>
#include <Windows.h>

class ErrorCode
{
public:
	ErrorCode(void);
	virtual ~ErrorCode(void);

	int							addError(const char * strError); //����ֵΪerrorcode��
	
	const char *				getError(int errorCode) const;						//����errorCode����strError

protected:
	int					m_iCurrentErrorCode;									//���浱ǰ��С�Ĵ�����
	std::map<int,std::string>	m_oError;
};
LPCWSTR string2LPCWSTR(std::string str);


#endif


