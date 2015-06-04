#pragma once

#include <vector>



using namespace std;
//-----------------------------------------------------------------------
//
//	create a structure to hold each genome
//-----------------------------------------------------------------------
struct SGenome
{
	vector<double>	vecWeights;

	double          dFitness;

	SGenome() :dFitness(0){}

	SGenome(vector<double> w, double f) : vecWeights(w), dFitness(f){}

	//overload '<' used for sorting
	friend bool operator<(const SGenome& lhs, const SGenome& rhs)
	{
		return (lhs.dFitness < rhs.dFitness);
	}
};


class structures
{
public:
	structures();
	~structures();


};

