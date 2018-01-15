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
 * File Name: ANNneuronClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m11a 10-January-2018
 * Comments:
 *
 *******************************************************************************/




#ifndef HEADER_ANN_NEURON_CLASS
#define HEADER_ANN_NEURON_CLASS

#include "ANNglobalDefs.hpp"


#define ANN_SUBNETS
//#define ANN_DEBUG
//#define DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS	//must also be defined in ANNparser.cpp




#define DEFAULT_FIRST_INPUT_NEURON_ID 1
#define DEFAULT_FIRST_OUTPUT_NEURON_ID 2

class ANNneuronConnection;

class ANNneuron
{
public:

	ANNneuron(void);
	ANNneuron(long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit);
	~ANNneuron(void);
	void initialiseNeuron();

	long id;
	long orderID;
	long layerID;
	long subnetID;

	double bias;
		double storedBias;					// (storedBias used for reset)
	double output;
	double classTarget;					//only relevant to output layer neurons
	double error;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	bool backNeuronMatchTemp;
	long memoryTrace;	//popularity
	bool intermediaryNeuronCreatedThisRound;
	bool neuronTypeInput;
	bool neuronTypeTopLevelCategory;
	bool neuronTypeIntermediary;
	bool printed;
	bool topLevelCategoryAlreadyExistsDeleteThisNewCopy;
	#endif
	bool printedXML;

	//for visualising highly structured networks [Eg used to visualise a 2D visual processing unit/subnet]
	bool spatialCoordinatesSet2D;
	bool spatialCoordinatesSet3D;
	long xPosRel;
	long yPosRel;
	long zPosRel;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	bool spatialCoordinatesSetClassification;
	double xPosRelFrac;
	double yPosRelFrac;
	double zPosRelFrac;
	#endif

	ANNneuron* nextNeuron;		//OLD: nextNeuronContainer

	bool hasFrontLayer;
	bool hasBackLayer;
	ANNneuron* firstNeuronInFrontLayer;
	ANNneuron* firstNeuronInBackLayer;

	vector<ANNneuronConnection*> frontANNneuronConnectionList;
	vector<ANNneuronConnection*> backANNneuronConnectionList;

	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	vector<ANNneuronConnection*> sideANNneuronConnectionList;
	#endif

	#ifdef ANN_SUBNETS
	bool isSubnet;	 	//alternative to being a subnet is being a distinct neuron

	//variables only used by subnets (abstract neurons)
	ANNneuron* firstNeuronInBackLayerOfSubnet;	//OLDname: firstNeuronContainerInBackLayerOfSubnet //this variable only used if this object is a subnet, not a neuron - the properties of the neurons in this list are direct copies of those in firstNeuronInFrontLayer
	ANNneuron* firstNeuronInFrontLayerOfSubnet;	//OLDname: firstNeuronContainerInFrontLayerOfSubnet

	bool isInputSubnet;		//input subnets are a special case - they do not require linking of their input neurons with a back layer (firstBackANNneuronConnectionContainer)
		//input subnets are used when a network is created with a selection of predefined subnets to cator for the preprocessing of different kinds of input information (Eg visual/audio etc)
	bool isOutputSubnet;	//output subnets are a special case - they do not require linking of their output neurons with a front layer (firstFrontANNneuronConnectionContainer)
	#endif
	
	#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
	bool GIAisConceptEntity;	//#ifdefGIA_NEURAL_NETWORK_SYMBOLIC_CORE: else instance neuron. #ifdefGIA_NEURAL_NETWORK_NON_SEMANTIC: else synapse artificial instance neuron (ie synapse between dendrites; or perhaps interneuron). Dendrodendritic synapses are connections between the dendrites of two different neurons. This is in contrast to the more common axodendritic synapse 
	string GIAentityName;
	bool GIAactiveForSubnetIdentification;
	bool GIAalreadyParsed;
	long GIAactivationAge;
	int GIAconceptIndexBitID;	//concept index bit neurons only
	//#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	int GIAconceptIndexType;	//concept neurons only
	int GIAconceptIndex;		//concept neurons only
	//#endif
	#endif
};


class ANNneuronClassClass
{
	public: void fillInNeuronIDProperties(ANNneuron* neuronToUpdate, long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit);

	private: void copyNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy);

	public: void averageNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy);
};

#endif


