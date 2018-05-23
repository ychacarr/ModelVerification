#pragma once
#ifndef H_TXTANALYZE
#define H_TXTANALYZE
#include "Entity.h"
#include "Relation.h"
#include "IOModule.h"

namespace TXTANALYZE
{
	class TxtAnalyzer
	{
	private:
		unsigned int ID;
		static unsigned int Counter;
	public:
		TxtAnalyzer();
		~TxtAnalyzer();
		bool Analyze(const INPUT::IO & FileReader, const MODEL::Entity & inEnt, std::wstring & outname);
		std::wstring GenParam(const std::wstring & inname);
		std::wstring GenParam(const MODEL::Entity & inEnt);
		std::wstring GenParam(const MODEL::Relation & inRel);
	};
}
#endif // !H_TXTANALYZE