

#ifndef CGENETICSELECT
#define CGENETICSELECT

//------------------------------------------------------------------------
//
//	Name: CGenAlg.h
//
//  Author: David Greenberg 2015
//
//  Desc: Genetic Selection functions. 
// Contains functions for Truncation Selection, Stochastic Uniform Selection, Reward-Based Selection, 
//------------------------------------------------------------------------

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "structures.h"
#include "utils.h"
#include "CParams.h"

using namespace std;

class geneticselection
{
public:
	geneticselection();
	~geneticselection();

	//***************************************
	//Selection Functions
	//***************************************

	static SGenome fitnessPropSelection(vector<SGenome> &m_vecPop, double m_totalfitness);
	static SGenome tournamentSelection(vector<SGenome> &m_vecPop, double m_totalfitness);
	static SGenome rewardBasedSelection(vector<SGenome> &m_vecPop, double m_totalfitness);
	static SGenome stochasticUniformSelection(vector<SGenome> &m_vecPop, double m_totalfitness);
	static SGenome truncationSelection(vector<SGenome> &m_vecPop, double m_totalfitness);
	static SGenome elitism(vector<SGenome> &m_vecPop, double m_totalfitness);

};

#endif