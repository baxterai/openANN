/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmMemoryNetworkUpdate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3l1a 02-June-2017
 * Comments:
 *
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

