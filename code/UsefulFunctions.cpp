#include "stdafx.h"
#include "UsefulFunctions.h"
#include "Errors.h"

namespace USEFUNC
{
	int strToint(const std::string & instr)
	{
		if (instr.length() == 0)
			ERROR::throwError("Error in strToint(). Input string can't be empty.");
		int number = 0, digitnum = 1;
		char onesymb;
		unsigned int i = 0;
		if (instr.at(0) == '-' || instr.at(0) == '+')
			i = 1;
		for (i; i < instr.length(); i++) {
			onesymb = instr.at(i);
			if (onesymb - '0' < 0 || onesymb - '0' > 9)
				ERROR::throwError("Error in strToint(). A non-numeric character in the input string.");
			number += (onesymb - '0') * digitnum;
			digitnum *= 10;
		}
		if (instr.at(0) == '-')
			number *= -1;
		return number;
	}

	unsigned int strTouint(const std::string & instr)
	{
		int res = strToint(instr);
		if (res < 0)
			res *= -1;
		return res;
	}

	std::string intTostr(int num)
	{
		std::string res = "";
		do
		{
			res.insert(res.begin(), ((num % 10) + '0'));
			num /= 10;
		} while (num != 0);
		return res;
	}
}