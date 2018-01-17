#pragma once
#ifndef H_TXTANALYZE
#define H_TXTANALYZE
#include "Entity.h"
#include "Relation.h"

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
		bool Analyze(const MODEL::Entity & inEnt, std::string & outname);
		std::string GenParam(const std::string & inname);
		std::string GenParam(const MODEL::Entity & inEnt);
		std::string GenParam(const MODEL::Relation & inRel);
	};
}
#endif // !H_TXTANALYZE