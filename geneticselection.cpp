#include "geneticselection.h"


geneticselection::geneticselection()
{
}


geneticselection::~geneticselection()
{
}

//***********************************************************
//Fitness Proportiante Selection 
// Selects based on a proportion
//***********************************************************
SGenome geneticselection::fitnessPropSelection(vector<SGenome> &m_vecPop, double m_totalfitness){
	


	return m_vecPop[0];

}

//***********************************************************
//Tournament Selection
// Runs tournaments to select the winner
//***********************************************************

SGenome geneticselection::tournamentSelection(vector<SGenome> &m_vecPop, double m_totalfitness){
	

	
	
	//Retrieve the size of the population
	int m_vecSize = m_vecPop.size();

	//Randomly select the initial best value
	//REASON: makes selecting every other part of the set easier
	SGenome best = m_vecPop[RandInt(0,m_vecSize-1)];

	//Holder for future individuals in the tournament
	SGenome ind;
	//Run a tournament utilizing 1 to n members of the population
	for (int i = 0; i < CParams::dTournamentNumber - 1; ++i){

		//Randomly select a member of the population
		ind = m_vecPop[RandInt(0, m_vecSize-1)];

		if (ind.dFitness > best.dFitness){
			//The new individual is better than the initial individual
			best = ind;
		}

	}

	//Tournament has run it's course
	//return the best option selected
	return best;
}

//***********************************************************
//Reward Based Selection
//Selects based on proportionate reward
//***********************************************************

SGenome geneticselection::rewardBasedSelection(vector<SGenome> &m_vecPop, double m_totalfitness){
	return m_vecPop[0];
}

//***********************************************************
//Stochastic Uniform Selection

//***********************************************************

SGenome geneticselection::stochasticUniformSelection(vector<SGenome> &m_vecPop, double m_totalfitness){
	return m_vecPop[0];
}

//***********************************************************
// Truncation Selection
//***********************************************************

SGenome geneticselection::truncationSelection(vector<SGenome> &m_vecPop, double m_totalfitness){
	return m_vecPop[0];
}

//***********************************************************
// Elitism
//***********************************************************

SGenome geneticselection::elitism(vector<SGenome> &m_vecPop, double m_totalfitness){
	return m_vecPop[0];
}