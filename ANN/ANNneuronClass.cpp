/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: ANNneuronClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3f6a 27-July-2015
 * Comments:
 *
 *******************************************************************************/

#include "ANNneuronClass.h"

/************************************************************ Neural Network Class Definitions* *****************************************************/


ANNneuron::ANNneuron(void)
{
	id = 0;		//none = 0
	orderID = 0;	//none = 0
	layerID = 0;	//none = 0
	subnetID = 0;	//none = 0

	//incase network is never trained, but output for visualisation purposes, set these values to dummy values
	bias = 0;		//this value must be initialised explicity
	storedBias = 0; 	//this value must be initialised explicity
	output = 0;		//this value must be initialised explicity
	classTarget = 0;	//this value must be initialised explicity
	error = 0;		//this value must be initialised explicity

	spatialCoordinatesSet = 0;
	xPosRel = 0;		//this value must be initialised explicity
	yPosRel = 0;		//this value must be initialised explicity
	zPosRel = 0;		//this value must be initialised explicity

	nextNeuron = NULL;

	hasFrontLayer = false;
	hasBackLayer = false;
	firstNeuronInFrontLayer = NULL;
	firstNeuronInBackLayer = NULL;

#ifdef ANN_ADVANCED
	isSubnet = false;

	firstNeuronInBackLayerOfSubnet = NULL;
	firstNeuronInFrontLayerOfSubnet = NULL;

	isInputSubnet = false;
	isOutputSubnet = false;
#endif
}

ANNneuron::ANNneuron(long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit)
{
	id = IDinit;
	orderID = orderIDinit;
	layerID = layerIDinit;
	subnetID = subnetIDinit;

	//bias = 0;		 //this value must be initialised explicity
	//storedBias = 0; 	//this value must be initialised explicity
	//output = 0; 		//this value must be initialised explicity
	//classTarget = 0; 	//this value must be initialised explicity
	//error = 0; 		//this value must be initialised explicity

	spatialCoordinatesSet = 0;
	//xPosRel = 0;		//this value must be initialised explicity
	//yPosRel = 0;		//this value must be initialised explicity
	//zPosRel = 0;		//this value must be initialised explicity

	nextNeuron = NULL;

	hasFrontLayer = false;
	hasBackLayer = false;
	firstNeuronInFrontLayer = NULL;
	firstNeuronInBackLayer = NULL;

#ifdef ANN_ADVANCED
	isSubnet = false;

	firstNeuronInBackLayerOfSubnet = NULL;
	firstNeuronInFrontLayerOfSubnet = NULL;

	isInputSubnet = false;
	isOutputSubnet = false;
#endif
}

ANNneuron::~ANNneuron(void)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = frontANNneuronConnectionList.begin(); connectionIter != frontANNneuronConnectionList.end(); connectionIter++)
	{
		if(*connectionIter != NULL)
		{
			#ifdef GIA_FREE_MEMORY_DEBUG
			//cout << "deleting: frontANNneuronConnectionList connection: " << (*connectionIter)->frontNeuronID << endl;
			#endif
			delete* connectionIter;
		}
	}

	/*
	for(vector<ANNneuronConnection*>::iterator connectionIter = backANNneuronConnectionList.begin(); connectionIter != backANNneuronConnectionList.end(); connectionIter++)
	{
		if(*connectionIter != NULL)
		{
			#ifdef GIA_FREE_MEMORY_DEBUG
			//cout << "deleting: backANNneuronConnectionList connection: " << (*connectionIter)->frontNeuronID << endl;
			#endif
			delete* connectionIter;
		}
	}
	*/

	if(firstNeuronInBackLayerOfSubnet != NULL)
	{
		delete firstNeuronInBackLayerOfSubnet;
	}

	if(firstNeuronInFrontLayerOfSubnet != NULL)
	{
		delete firstNeuronInFrontLayerOfSubnet;
	}

	if(nextNeuron != NULL)
	{
		delete nextNeuron;
	}
	else
	{
		if(firstNeuronInFrontLayer != NULL)
		{
			delete firstNeuronInFrontLayer;
		}
	}
}





void fillInNeuronIDProperties(ANNneuron* neuronToUpdate, long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit)
{
	neuronToUpdate->id = IDinit;
	neuronToUpdate->orderID = orderIDinit;
	neuronToUpdate->layerID = layerIDinit;
	neuronToUpdate->subnetID = subnetIDinit;
}


void copyNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy)
{
	neuronToUpdate->bias = neuronToCopy->bias;
	neuronToUpdate->storedBias = neuronToCopy->storedBias;
	neuronToUpdate->output = neuronToCopy->output;
	neuronToUpdate->classTarget = neuronToCopy->classTarget;
	neuronToUpdate->error = neuronToCopy->error;
}

void averageNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy)
{
	neuronToUpdate->bias = (neuronToUpdate->bias + neuronToCopy->bias)/2.0;
	neuronToUpdate->storedBias = (neuronToUpdate->storedBias + neuronToCopy->storedBias)/2.0;
	neuronToUpdate->output = (neuronToUpdate->output + neuronToCopy->output)/2.0;
	neuronToUpdate->classTarget = (neuronToUpdate->classTarget + neuronToCopy->classTarget)/2.0;
	neuronToUpdate->error = (neuronToUpdate->error + neuronToCopy->error)/2.0;
}


/************************************************************ End Neural Network Class Definitions* *************************************************/


