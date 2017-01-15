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
 * File Name: ANNdraw.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 4a7b 07-June-2016
 * Description: This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV]
 *
 *******************************************************************************/


#ifndef HEADER_ANN_DRAW
#define HEADER_ANN_DRAW

#include "LDreferenceClass.h"
#include "ANNneuronClass.h"
#include "ANNneuronConnectionClass.h"
#include "XMLparserClass.h"

#define NEURON_SPRITE_SCALE_FACTOR 0.005	//0.005

#define CPLUSPLUSERRORCORRECTION1 (10000)
//#define CPLUSPLUSERRORCORRECTION3

#define ANN_OUTPUT_Z_POSITION_CONNECTIONS (0.6)
#define ANN_OUTPUT_Z_POSITION_NODES (0.4)
#define ANN_OUTPUT_Z_POSITION_TEXT (0.2)
#define ANN_SVG_SCALE_FACTOR (100)
#define ANN_SVG_TEXT_SCALE_FACTOR (5)
#define ANN_SVG_NEURON_SIZE (10)
//#define ANN_SVG_SPRITE_TEXT_MAX_NUMBER_LINES (10)
#define ANN_SVG_SPRITE_TEXT_OFFSET_PER_LINE (10)

#define ANN_SPRITE_HEADER_NAME "\n0 Start OpenANN.exe Sprites\n"
#define ANN_SPRITE_TRAILER_NAME "0 End OpenANN.exe Sprites\n"
#define ANN_SPRITE_HEADER_NUM_LINES (2)
#define ANN_SPRITE_TRAILER_NUM_LINES (1)
#define ANN_SPRITE_HEADER_LENGTH 25
#define ANN_SPRITE_TRAILER_LENGTH 22



#define NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME "neuralNet.ldr"
#define NEURAL_NETWORK_VISUALISATION_SVG_FILE_NAME "neuralNet.svg"
#define NEURAL_NETWORK_VISUALISATION_PPM_FILE_NAME "neuralNet.ppm"

#define LDRAW_UNITS_PER_CM 25.189
	//create xml rules for above parameters


//#define ANN_SPRITE_TYPE_NEURON (1)			//kindOfNNSprite
//#define ANN_SPRITE_TYPE_CONNECTION (2)		//kindOfNNSprite

#define SPRITE_SUBMODEL_NEURON_FILE_NAME_NAME "SPRITE_SUBMODEL_NEURON_FILE_NAME"		//CHECKTHIS this can be changed between circle and ring depending upon High Resolution renderer; Ray Tracer versus LDVIEW [and taking into account other factors Eg LGEO parts library]
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR"
#define SPRITE_SUBMODEL_RANGE_SCALE_FACTOR_NAME "SPRITE_SUBMODEL_RANGE_SCALE_FACTOR"		//this value is now multiplied by LDRAW_UNITS_PER_CM

#define SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
#define SPRITE_TEXTUAL_MAX_NUM_OF_LINES (8)

#define SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO"
/*
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO
#ifdef SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO
#endif
*/





void fillInANNSpriteExternVariables();

/*top level sprite routine*/

//void checkThis();

bool ANNcreateNeuralNetworkSceneFiles(string sceneFileName, ANNneuron* firstNeuronInNetwork, bool addSprites, bool writeSVG, XMLparserTag** currentTagSVG);
	bool ANNcreateNeuralNetworkReferenceLists(string sceneFileName, LDreference* initialReference, ANNneuron* firstNeuronInNetwork, bool addSprites, int* numSpritesAdded, bool writeSVG, XMLparserTag** currentTagSVG);
	bool ANNcreateNeuralNetworkSceneFilesFromReferenceLists(string sceneFileName, bool addSprites, LDreference* initialReference, int numSpritesAdded);
		LDreference* ANNsearchNeuralNetworkAndCreateReferences(ANNneuron* firstNeuronInLayer, LDreference* currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, bool isSubnet, vec* positionOfSubnetNeuron, bool addSprites, bool writeSVG, XMLparserTag** currentTagSVG);
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
			void ANNsearchNeuralNetworkAndCreateReferencesReset(ANNneuron* firstNeuronInLayer);
			#endif
			bool ANNfillNeuronDisplayReference(LDreference* currentNeuronDispayReference, ANNneuron* neuron, bool hasSubnetNeuron, vec* positionOfsubnetNeuron, bool writeSVG, XMLparserTag** currentTagSVG);
			bool ANNfillANNneuronConnectionDisplayReference(LDreference* currentNeuronDispayReference, LDreference* backNeuronReference, LDreference* forwardNeuronReference, ANNneuronConnection* ANNneuronConnection, bool writeSVG, XMLparserTag** currentTagSVG);

			bool ANNdetermineSpriteInfoForNeuronAndAddSpriteToSpriteRefList(ANNneuron* neuron, LDreference** currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, vec* neuronReferenceRelativePosition, bool writeSVG, XMLparserTag** currentTagSVG);
			bool ANNdetermineSpriteInfoForANNneuronConnectionAndAddSpriteToSpriteRefList(ANNneuronConnection* ANNneuronConnection, LDreference** currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, vec* backNeuronReferenceRelativePosition, vec* forwardNeuronReferenceRelativePosition, bool writeSVG, XMLparserTag** currentTagSVG);
				bool ANNaddSpriteToSpriteReferenceList(vec* spriteSceneCoords, vec* eyeCoords, LDreference** currentListReference, string spriteReferenceFileName, int spriteDefaultColour, double spriteScaleFactor);
				void writeSpriteTextToSVG(string* spriteTextString, bool writeSVG, XMLparserTag** currentTagSVG, vec* referencePosition);

			void ANNgenerateTextualNeuronSpriteInfoString(ANNneuron* neuron, string* spriteTextString, int spriteColourArray[]);
			void ANNgenerateTextualANNneuronConnectionSpriteInfoString(ANNneuronConnection* ANNneuronConnection, string* spriteTextString, int spriteColourArray[]);


#endif
