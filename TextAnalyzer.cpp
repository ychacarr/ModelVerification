#include "stdafx.h"
#include "TextAnalyzer.h"
#include "UsefulFunctions.h"
#include "Sentence.h"
#include "Errors.h"
#include <fstream>

namespace TXTANALYZE
{
	unsigned int TxtAnalyzer::Counter = 0;

	TxtAnalyzer::TxtAnalyzer()
	{
		ID = ++Counter;
		#ifdef _DEBUG
			std::cout << "\nTxtAnalyzer(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	TxtAnalyzer::~TxtAnalyzer()
	{
		#ifdef _DEBUG
			std::cout << "~TxtAnalyzer(). Object with ID = " << ID << " was destroyed.\n";
		#endif // _DEBUG
	}

	bool TxtAnalyzer::Analyze(const MODEL::Entity &inEnt, std::string & outname)
	{
		Sentence sentenceanalyze(inEnt.getDeterm());
		outname.clear();
		std::string keywords, testkey;
		std::size_t notfound = std::string::npos;
		//TEST CODE START
		{
			std::ifstream InFile;
			//std::ifstream InFile("txt files/TestDictionary.txt", std::ios_base::in);
			//if (!InFile.is_open())
			//	ERROR::throwError("Error in TxtAnalyzer::Analyze(). Can't open the file. ID - Entity.", inEnt.getID());

			//while (!InFile.eof())	{
			//	std::string strsynon;
			//	std::getline(InFile, testkey, '\n');
			//	std::getline(InFile, strsynon, '\n');
			bool endFlag = false;
				for (unsigned int i = 0; i < sentenceanalyze.getWordcount(); i++) {
					endFlag = false;
					if (sentenceanalyze.getWord(i).length() > 3) {
						std::cout << '\n' << sentenceanalyze.getWord(i) << " now analyze.";
						InFile.open("txt files/TestDictionary.txt", std::ios_base::in);
						if (!InFile.is_open())
							ERROR::throwError("Error in TxtAnalyzer::Analyze(). Can't open the file. ID - Entity.", inEnt.getID());
						while (!InFile.eof() && !endFlag) {
							std::string strsynon;
							std::getline(InFile, testkey, '\n');
							std::getline(InFile, strsynon, '\n');

							if (notfound != strsynon.find(sentenceanalyze.getWord(i))) {
								// ƒл€ исключени€ повторного добавлени€ ключевых слов
								// ¬озможно излиш€€ проверка, но пока пусть будет
								if (notfound == keywords.find(testkey)) {
									keywords += testkey + " ";
									endFlag = true;
								}
							}
						}
						InFile.close();
						std::cout << '\n' << sentenceanalyze.getWord(i) << " analyze end.";
					}
				}
			InFile.close();
			if (keywords.length() > 1)
				keywords.erase(keywords.end() - 1);
			testkey.clear();
			InFile.open("txt files/TestStandard.txt", std::ios_base::in);
			
			if (keywords.length() != 0) {
				while (!InFile.eof()) {
					std::string standdeterm;
					std::getline(InFile, testkey);
					std::getline(InFile, standdeterm);
					if (notfound != standdeterm.find(keywords)) {
						std::cout << "\nANALYZE TEST. \n INPUT NAME - " + inEnt.getName() + ". ANALYZED NAME - " + testkey + '\n';
						InFile.close();
						outname.append(testkey);
						return true;
					}
				}
			}
			InFile.close();
		}
		std::cout << "\nANALYZE TEST. \n FAIL TO ANALYZE.\n";
		// TEST CODE END
		outname.append(inEnt.getName());
		return false;
	}

	std::string TxtAnalyzer::GenParam(const std::string & inname)
	{
		if (inname.length() == 0)
			return std::string("");
		std::string res = "";
		res += inname.at(0);
		res += inname.at((inname.length() / 2));
		res += inname.at(inname.length() - 1);
		res += USEFUNC::intTostr(inname.length());
		return res;
	}

	std::string TxtAnalyzer::GenParam(const MODEL::Entity & inEnt)
	{
		return GenParam(inEnt.getName());
	}

	std::string TxtAnalyzer::GenParam(const MODEL::Relation & inRel)
	{
		std::string res = "";
		res = GenParam(inRel.getParentEnt());
		res += '[' + inRel.getType() + ']';
		res += GenParam(inRel.getChildEnt());
		return res;
	}

}