#include "CGenAlg.h"




//-----------------------------------constructor-------------------------
//
//	sets up the population with random floats
//
//-----------------------------------------------------------------------
CGenAlg::CGenAlg(int	  popsize,
	double	MutRat,
	double	CrossRat,
	int	  numweights) : m_iPopSize(popsize),
	m_dMutationRate(MutRat),
	m_dCrossoverRate(CrossRat),
	m_iChromoLength(numweights),
	m_dTotalFitness(0),
	m_cGeneration(0),
	m_iFittestGenome(0),
	m_dBestFitness(0),
	m_dWorstFitness(99999999),
	m_dAverageFitness(0)
{
	//initialise population with chromosomes consisting of random
	//weights and all fitnesses set to zero
	for (int i = 0; i < m_iPopSize; ++i)
	{
		m_vecPop.push_back(SGenome());

		for (int j = 0; j < m_iChromoLength; ++j)
		{
			m_vecPop[i].vecWeights.push_back(RandomClamped());
		}
	}
}


//---------------------------------Mutate--------------------------------
//
//	mutates a chromosome by perturbing its weights by an amount not 
//	greater than CParams::dMaxPerturbation
//-----------------------------------------------------------------------
void CGenAlg::Mutate(vector<double> &chromo)
{
	//traverse the chromosome and mutate each weight dependent
	//on the mutation rate
	for (int i = 0; i < chromo.size(); ++i)
	{
		//do we perturb this weight?
		if (RandFloat() < m_dMutationRate)
		{
			//add or subtract a small value to the weight
			chromo[i] += (RandomClamped() * CParams::dMaxPerturbation);
		}
	}
}

//----------------------------------GetChromoRoulette()------------------
//
//	returns a chromo based on roulette wheel sampling
//
//-----------------------------------------------------------------------
SGenome CGenAlg::GetChromoRoulette()
{
	//generate a random number between 0 & total fitness count
	double Slice = (double)(RandFloat() * m_dTotalFitness);

	//this will be set to the chosen chromosome
	SGenome TheChosenOne;

	//go through the chromosones adding up the fitness so far
	double FitnessSoFar = 0;

	for (int i = 0; i < m_iPopSize; ++i)
	{
		FitnessSoFar += m_vecPop[i].dFitness;

		//if the fitness so far > random number return the chromo at 
		//this point
		if (FitnessSoFar >= Slice)
		{
			TheChosenOne = m_vecPop[i];

			break;
		}

	}

	return TheChosenOne;
}

//-------------------------------------Crossover()-----------------------
//	
//  given parents and storage for the offspring this method performs
//	crossover according to the GAs crossover rate
//-----------------------------------------------------------------------
void CGenAlg::Crossover(const vector<double> &mum,
	const vector<double> &dad,
	vector<double>       &baby1,
	vector<double>       &baby2)
{
	//just return parents as offspring dependent on the rate
	//or if parents are the same
	if ((RandFloat() > m_dCrossoverRate) || (mum == dad))
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}

	//determine a crossover point
	int cp = RandInt(0, m_iChromoLength - 1);

	//create the offspring
	for (int i = 0; i < cp; ++i)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}

	for (int i = cp; i < mum.size(); ++i)
	{
		baby1.push_back(dad[i]);
		baby2.push_back(mum[i]);
	}


	return;
}

//-------------------------------------UniformCrossover()-----------------------
//	
//Combine a set of parents using a uniform crossover method
//-----------------------------------------------------------------------

void CGenAlg::UniformCrossover(const vector<double> &mum,
	const vector<double> &dad,
	vector<double>       &baby1,
	vector<double>       &baby2){

	//just return parents as offspring dependent on the rate
	//or if parents are the same
	if ((RandFloat() > m_dCrossoverRate) || (mum == dad))
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}


	//Retrieve the smaller parent
	//If the other is longer, then we add those genes on at the end
	int sizeOfParents = mum.size() > dad.size() ? dad.size() : mum.size();

	for (int i = 0; i < sizeOfParents; i++){
		if (RandFloat() > CParams::dUniformCrossoverRate){
			//Use the mothers genes if the number is greater than the crossover potentional
			baby1.push_back(mum[i]);
			baby2.push_back(dad[i]);
		}
		else{
			//use fathers otherwise
			baby2.push_back(mum[i]);
			baby1.push_back(dad[i]);
		}

	}
	return;

}

//-----------------------------------Epoch()-----------------------------
//
//	takes a population of chromosones and runs the algorithm through one
//	 cycle.
//	Returns a new population of chromosones.
//
//-----------------------------------------------------------------------
vector<SGenome> CGenAlg::Epoch(vector<SGenome> &old_pop)
{
	//assign the given population to the classes population
	m_vecPop = old_pop;

	//reset the appropriate variables
	Reset();

	//sort the population (for scaling and elitism)
	sort(m_vecPop.begin(), m_vecPop.end());

	//calculate best, worst, average and total fitness
	CalculateBestWorstAvTot();

	//create a temporary vector to store new chromosones
	vector <SGenome> vecNewPop;

	//Now to add a little elitism we shall add in some copies of the
	//fittest genomes. Make sure we add an EVEN number or the roulette
	//wheel sampling will crash
	if (!(CParams::iNumCopiesElite * CParams::iNumElite % 2))
	{
		GrabNBest(CParams::iNumElite, CParams::iNumCopiesElite, vecNewPop);
	}

	//Initialize the parent genomes
	SGenome mum;
	SGenome dad;

	//now we enter the GA loop

	//repeat until a new population is generated
	while (vecNewPop.size() < m_iPopSize)
	{

		//Select the type of selection which should be performed
		switch (CParams::dSelectionType){
		case 0:
			//grab two chromosones
			mum = GetChromoRoulette();
			dad = GetChromoRoulette();
			break;
		case 1:
			//Use Tournament Selection to choose parents
			mum = geneticselection::tournamentSelection(m_vecPop, m_dTotalFitness);
			dad = geneticselection::tournamentSelection(m_vecPop, m_dTotalFitness);
			break;
		default:
			//grab two chromosones
			mum = GetChromoRoulette();
			dad = GetChromoRoulette();
			break;



		}




		//create some offspring via crossover
		vector<double>		baby1, baby2;

		//Choose which crossover type to use
		switch (CParams::CrossoverType){
		case 0:
			//The original Crossover
			Crossover(mum.vecWeights, dad.vecWeights, baby1, baby2);
			break;
		case 1:
			UniformCrossover(mum.vecWeights, dad.vecWeights, baby1, baby2);
			break;
		default:
			//Used as default in case the value is not set for some reason
			Crossover(mum.vecWeights, dad.vecWeights, baby1, baby2);
			break;
		}


		//now we mutate
		Mutate(baby1);
		Mutate(baby2);

		//now copy into vecNewPop population
		vecNewPop.push_back(SGenome(baby1, 0));
		vecNewPop.push_back(SGenome(baby2, 0));
	}

	//finished so assign new pop back into m_vecPop
	m_vecPop = vecNewPop;

	return m_vecPop;
}


//-------------------------GrabNBest----------------------------------
//
//	This works like an advanced form of elitism by inserting NumCopies
//  copies of the NBest most fittest genomes into a population vector
//--------------------------------------------------------------------
void CGenAlg::GrabNBest(int	            NBest,
	const int	      NumCopies,
	vector<SGenome>	&Pop)
{
	//add the required amount of copies of the n most fittest 
	//to the supplied vector
	while (NBest--)
	{
		for (int i = 0; i < NumCopies; ++i)
		{
			Pop.push_back(m_vecPop[(m_iPopSize - 1) - NBest]);
		}
	}
}

//-----------------------CalculateBestWorstAvTot-----------------------	
//
//	calculates the fittest and weakest genome and the average/total 
//	fitness scores
//---------------------------------------------------------------------
void CGenAlg::CalculateBestWorstAvTot()
{
	m_dTotalFitness = 0;

	double HighestSoFar = 0;
	double LowestSoFar = 9999999;

	for (int i = 0; i < m_iPopSize; ++i)
	{
		//update fittest if necessary
		if (m_vecPop[i].dFitness > HighestSoFar)
		{
			HighestSoFar = m_vecPop[i].dFitness;

			m_iFittestGenome = i;

			m_dBestFitness = HighestSoFar;
		}

		//update worst if necessary
		if (m_vecPop[i].dFitness < LowestSoFar)
		{
			LowestSoFar = m_vecPop[i].dFitness;

			m_dWorstFitness = LowestSoFar;
		}

		m_dTotalFitness += m_vecPop[i].dFitness;


	}//next chromo

	m_dAverageFitness = m_dTotalFitness / m_iPopSize;
}

//-------------------------Reset()------------------------------
//
//	resets all the relevant variables ready for a new generation
//--------------------------------------------------------------
void CGenAlg::Reset()
{
	m_dTotalFitness = 0;
	m_dBestFitness = 0;
	m_dWorstFitness = 9999999;
	m_dAverageFitness = 0;
}

