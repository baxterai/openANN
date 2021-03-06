/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmMemoryNetworkUpdate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3p3a 19-March-2021
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_MEMORY_NETWORK_UPDATE
#define HEADER_ANN_ALGORITHM_MEMORY_NETWORK_UPDATE

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNalgorithmBackpropagationUpdate.hpp"	//required for backPropogationForwardPassStep

#ifdef ANN_ALGORITHM_MEMORY_NETWORK

class ANNalgorithmMemoryNetworkUpdateClass
{
	private: ANNalgorithmBackpropagationUpdateClass ANNalgorithmBackpropagationUpdate;
	public: double ANNclassificationAndMemoryPass(ANNneuron* firstInputNeuronInNetwork, const ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult, double* trainingMemoryResult);
		private: void calculateBinaryOutputCode(const ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult);
		private: bool isNeuronOutputFire(const ANNneuron* currentNeuron);
		private: void memoryTraceForwardPassStep(ANNneuron* neuronBeingAccessed, double* trainingMemoryResult);
			private: void readAndUpdateMemoryTrace(ANNneuron* currentNeuronInLayer, double* trainingMemoryResult);
				private: void updateMemoryTraceBasedOnSimultaneouslyFiredNeuronsIeAssocation(ANNneuronConnection* currentANNneuronConnectionSide);
				private: void incrementMemoryTraceTallyBasedOnSimultaneouslyFiredNeuronsIeAssocation(double* trainingMemoryResult);
};



#endif

#endif

