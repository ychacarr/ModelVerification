#include "stdafx.h"
#include "UsefulFunctions.h"
#include "Errors.h"

namespace USEFUNC
{
	int strToint(const std::wstring & instr)
	{
		if (instr.length() == 0)
			ERROR::throwError(L"Error in strToint(). Input string can't be empty.");
		int number = 0, digitnum = 1;
		wchar_t onesymb;
		int i = instr.length() - 1, stopind = 0;
		if (instr.at(0) == L'-' || instr.at(0) == L'+')
			stopind = 1;
		for (i; i >= stopind; i--) {
			onesymb = instr.at(i);
			if (onesymb - L'0' < 0 || onesymb - L'0' > 9)
				ERROR::throwError(L"Error in strToint(). A non-numeric character in the input string.");
			number += (onesymb - L'0') * digitnum;
			digitnum *= 10;
		}
		if (instr.at(0) == L'-')
			number *= -1;
		return number;
	}

	unsigned int strTouint(const std::wstring & instr)
	{
		int res = strToint(instr);
		if (res < 0)
			res *= -1;
		return res;
	}

	std::wstring intTostr(int num)
	{
		std::wstring res = L"";
		do
		{
			res.insert(res.begin(), ((num % 10) + L'0'));
			num /= 10;
		} while (num != 0);
		return res;
	}

	void editalltolower(std::wstring &inwstr)
	{
		if (inwstr.length() == 0)
			ERROR::throwError(L"Error in alltolower(). Input string can't be empty.");
		std::locale loc;
		for (unsigned int i = 0; i < inwstr.length(); i++)
			inwstr.at(i) = std::tolower(inwstr.at(i), loc);
	}

	std::wstring cpyalltolower(const std::wstring &inwstr)
	{
		if (inwstr.length() == 0)
			ERROR::throwError(L"Error in alltolower(). Input string can't be empty.");

		std::wstring res;
		std::locale loc;

		for (unsigned int i = 0; i < inwstr.length(); i++)
			res.push_back(std::tolower(inwstr.at(i), loc));
		return std::wstring(res);
	}
}