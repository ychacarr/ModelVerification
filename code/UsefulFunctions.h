#pragma once
#ifndef H_USEFULFUNC
#define H_USEFULFUNC

namespace USEFUNC
{

	int strToint(const std::wstring &instr);
	unsigned int strTouint(const std::wstring &instr);
	std::wstring intTostr(int num);
	void editalltolower(std::wstring &inwstr);
	std::wstring cpyalltolower(const std::wstring &inwstr);
	std::string ws2s(const std::wstring& wstr);
	std::wstring s2ws(const std::string& str);
}
#endif // !H_USEFULFUNC