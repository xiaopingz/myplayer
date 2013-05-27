#pragma once
#include <vector>
#include <string>
class PlayList
{
public:
	enum playMode{
		PM_Rand = 1,			//Ëæ»ú
		PM_Sequence = 2,		//Ë³Ðò
		PM_Repeat = 3,			//ÖØ¸´
		PM_Loop = 4				//Ñ­»·
	};
	typedef enum playMode			PMode;
protected:
	PlayList(void);
public:
	~PlayList(void);

	static	PlayList &				getInstance();

	void							setMode(PMode mode);
	PMode							getMode() const;

	std::string						getNextFile(int currentIndex) const;
	int								addFile(std::string	strFile);
	int								delFile(int iIndex);
	std::vector<std::string>				getAllFile() const;

protected:
	std::vector<std::string>				m_oFileList;
	PMode							m_iMode;
};
