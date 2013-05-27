
#include "PlayList.h"
#include <assert.h>

#include "../common/PSettings.h"

PlayList::PlayList(void)
{
	PSettings & setting = PSettings::getInstance();
	m_iMode = (PMode )setting.getConfig("play-mode").m_value.intvalue;
	int iPlayNums = setting.getConfig("play-list-nums").m_value.intvalue;
	//从文件中读取列表, to be continued
}

PlayList::~PlayList(void)
{
	//写入文件， to be continued
}

PlayList & PlayList::getInstance()
{
	static PlayList p;
	return p;
}

void PlayList::setMode( PMode mode )
{
	m_iMode = mode;
}

PlayList::PMode PlayList::getMode() const
{
	return m_iMode;
}

std::string PlayList::getNextFile(int curIndex) const
{
	int i;
	switch(m_iMode)
	{
	case PlayList::PM_Loop:
		i = curIndex + 1;
		if(i>m_oFileList.size()-1)
			i = 0;
		break;
	case PlayList::PM_Rand:
		i=rand();
		break;
	case PlayList::PM_Repeat:
		i = curIndex;
		break;
	case PlayList::PM_Sequence:
		i = curIndex + 1;
		if(i>m_oFileList.size()-1)
			i = -1;
	default:
		assert(0);
	}
	assert(i>=0 && i<m_oFileList.size());
	if(i <0)
		return std::string();
	else
		return m_oFileList[i];
}

int PlayList::addFile( std::string strFile )
{
	m_oFileList.push_back(strFile);
	return m_oFileList.size() - 1;
}

std::vector<std::string> PlayList::getAllFile() const
{
	return m_oFileList;
}

int PlayList::delFile( int iIndex )
{
	if(iIndex<0  || iIndex>=m_oFileList.size())
		return 1;
	std::vector<std::string>::iterator it = m_oFileList.begin();
	for(; it!=m_oFileList.end();it++)
		if((*it) == m_oFileList[iIndex])
			m_oFileList.erase(it);
	return 1;
}