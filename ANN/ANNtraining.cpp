/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: ANNtraining.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3c4b 01-December-2012
 * Comments:
 *
 *******************************************************************************/

#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;


#include "ANNglobalDefs.h"
#include "ANNneuronClass.h"
#include "ANNtraining.h"
#include "ANNupdateAlgorithm.h"
#include "ANNformation.h"
#include "ANNparser.h"
#include "ANNdata.h"









/************************************************************ Train Neural Network Routines ******************************************************/



double calculateExperienceErrorForHypotheticalDecision(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, Experience * experience)
{
	double experienceBackPropagationPassError;

	storeNeuralNetworkBiasAndWeights(firstInputNeuronInNetwork);

	resetInputsAndClassTargets(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, experience);

	//VERIFICATION METHOD 1;
	experienceBackPropagationPassError = ANNBackPropogationPass(firstInputNeuronInNetwork, firstOutputNeuronInNetwork);

	/*//VERIFICATION METHOD 2;
	forwardPassStep(firstInputNeuronInNetwork);

	//NB calculate error of output neurons
	calculateErrorOfOutputNeurons(firstOutputNeuronInNetwork);

	experienceBackPropagationPassError = calculateErrorOfBackPropPass(firstOutputNeuronInNetwork);
	*/

	if(experienceBackPropagationPassError > MAX_ANN_BACK_PROPAGATION_ERROR)
	{
		experienceBackPropagationPassError = (MAX_ANN_BACK_PROPAGATION_ERROR-1.0);
	}

	restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuronInNetwork);

	return experienceBackPropagationPassError;
}



//DO NOT USE THIS; must use trainNeuralNetwork/trainNeuralNetworkSimple instead!
void feedNeuralNetworkWithASetOfExperiences(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, Experience * firstExperienceInList)
{
	Experience * currentExperience = firstExperienceInList;

	while(currentExperience->next != NULL)
	{
		resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

		double trainingErrorNotUsed;
		trainingErrorNotUsed = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);

		currentExperience = currentExperience->next;
	}
}


void trainNeuralNetworkSimple(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, int numEpochs, Experience * firstExperienceInDataSet, long numberOfExperiences)
{
	cout << "\n*****************************************************" << endl;
	cout << "Number of Input Neurons = " << numberOfInputNeurons << endl;
	cout << "Number of Output Neurons = " << numberOfOutputNeurons  <<endl;
	cout << "number of training Epochs = " << numEpochs <<endl;

	//now train the network on all examples for numEpochs
	resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuron);

	float finalError;
	for(int e=0;e < numEpochs;e++)
	{
		cout << "Neural Net Training Epoch: " << e << endl;

		Experience * currentExperience = firstExperienceInDataSet;

		//from start of dataSet -> end of dataSet
		for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
		{
			resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

			finalError = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);

			currentExperience = currentExperience->next;
		}
	}

	storeNeuralNetworkBiasAndWeights(firstInputNeuron);	//store these for future use of the network (such that they are not lost during future back props)

	#ifdef DEBUG_ANN_VERIFY_REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF
	long experiencesIncorrectlyDiagnosed = 0;
	#endif


	//Added by RBB; now test the network across all examples and calculate the average error
	Experience * currentExperience = firstExperienceInDataSet;
	float  sumingTestingError = 0.0F;
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);
		resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);
		float testingError = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);
		sumingTestingError = sumingTestingError + testingError;

		#ifdef DEBUG_ANN_VERIFY_REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF

		#ifdef DEBUG_ANN_PERFORM_ALTERNATE_ERROR_TOTAL
		currentExperience->numberOfAlternateOptionErrors = 0;
		#endif

		long originalClassTargetValue = currentExperience->classTargetValue;

		for(int alternateClassTargetValue=0; alternateClassTargetValue<numberOfOutputNeurons; alternateClassTargetValue++)
		{
			if(alternateClassTargetValue != currentExperience->classTargetValue)
			{
				currentExperience->classTargetValue = alternateClassTargetValue;
				restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);
				resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);
				float testingErrorOfAlternateOption = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);

				if(testingErrorOfAlternateOption < testingError)
				{
					#ifdef DEBUG_ANN_PERFORM_ALTERNATE_ERROR_TOTAL
					currentExperience->numberOfAlternateOptionErrors = currentExperience->numberOfAlternateOptionErrors+1;
					#endif
					cout << "\t testingErrorOfAlternateOption < testingError" << endl;
					cout << "experienceNum = " << experienceNum << endl;
					cout << "originalClassTargetValue = " << originalClassTargetValue << endl;
					cout << "alternateClassTargetValue = " << alternateClassTargetValue << endl;
					cout << "testingErrorOfAlternateOption = " << testingErrorOfAlternateOption << endl;
					experiencesIncorrectlyDiagnosed++;
				}
			}
		}

		currentExperience->classTargetValue= originalClassTargetValue;

		#endif

		currentExperience = currentExperience->next;

	}

	restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);


	#ifdef DEBUG_ANN_PERFORM_ALTERNATE_ERROR_TOTAL
	currentExperience = firstExperienceInDataSet;
	int numExperiencesWhichHaveFailures = 0;
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		if(currentExperience->numberOfAlternateOptionErrors > 0)
		{
			numExperiencesWhichHaveFailures++;
		}

		cout << "Experience " << experienceNum << " when tested gave " << currentExperience->numberOfAlternateOptionErrors << " preferred alternate options (total incorrect decisions / errors)" << endl;
		currentExperience = currentExperience->next;
	}
	#endif


	//cout << "Final training accuracy for e epochs (e det. from 10 fold cross validation) = " << (1.0F-finalError)*100.0F << "%" <<endl;
	cout << "Final averaged testing accuracy for e epochs (e from 10 fold cross validation) = " << (1.0F-(sumingTestingError/(double)numberOfExperiences))*100.0F << "%" <<endl;
	#ifdef DEBUG_ANN_VERIFY_REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF
	cout << "experiencesIncorrectlyDiagnosed = " << experiencesIncorrectlyDiagnosed << endl;
	cout << "Final Real Life accuracy [OLD] (based upon number of experiences correctly diagnosed) = " << (1.0F-((double)experiencesIncorrectlyDiagnosed/(double)numberOfExperiences))*100.0F << "%" <<endl;
	#ifdef DEBUG_ANN_PERFORM_ALTERNATE_ERROR_TOTAL
	cout << "Final Real Life accuracy [NEW] (based upon number of experiences correctly diagnosed) = " << (1.0F-((double)numExperiencesWhichHaveFailures/(double)numberOfExperiences))*100.0F << "%" <<endl;
	#endif
	#endif
	cout << "numberOfExperiences = " << numberOfExperiences << endl;
}



void trainNeuralNetwork(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, int maxFolds, Experience * firstExperienceInDataSet, long numberOfExperiences, int maxEpochs)
{
	/*
	network structure

	X input neurons
	arbitary number hidden layers/subnets with arbitary number hidden neurons with
	Y output neurons
	*/

	/*
	currently hard coded specifically for new-thyroid.data/new-thyroid.names test data)
	*/

	/*
	procedure;

	Go through network 215xaverageNumberOfEpicsx215 times..

	... using each of the 215 cases as a test case, and the other 214 as training cases.

	for each of the 215 test cases;

	train on all 214 cases. then test on the test case. calculate the final error rate.

	average all these error rates over all permutations of test cases to get the final error rate.
	*/

	int foldNum; //number of folds for cross validation

	float totalTestingError = 0;
	float totalTrainingError = 0;
	float totalTrainingEpochs = 0;

	float * trainingEpochsNumberArray = new float[maxFolds];
	float * trainingErrorArray = new float[maxFolds];
	float * testingErrorArray = new float[maxFolds];

	for(foldNum=0; foldNum < maxFolds; foldNum++)
	{
		/*
		assumes Thyroid data is randomly distributed - if its not it should be -
		and takes the last 1/foldNum (*) number of data sets
		as test sets, and the first (foldNum-1)/foldNum as training sets.
		*/

		int numEpochs;
		float testingError;
		float trainingError;

		//train using all sets of data except for test set (= testPermutation)

		resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuron);

		trainingError = 1.0F;

		numEpochs = 0;

		float acceptableError = ACCEPTABLE_ERROR_RATE;

		#ifdef ANN_DEBUG
		/*
		cout << "q = int(float(numberOfExperiences) * float(foldNum)/float(maxFolds))" << endl;
		cout << "q = " << int(float(numberOfExperiences) * float(foldNum)/float(maxFolds)) << endl;
		cout << "q+int(float(numberOfExperiences)/float(maxFolds)) = " << int(float(numberOfExperiences) * float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds)) << endl;
		*/
		#endif
		
		float currentAverageTestingError;
		currentAverageTestingError = 1.0F;	//ADDED BY RBB 17 sept 08

		float recordedTestedError = 10.0F;	//this cannot be just 1.0 as errors slighly greater than 1.0F can be experienced.
			//it is initalised to a high error value so that ... so that overfitting can be identified

		bool overFittingData = false;

		long numberOfFirstExperienceInFoldTrainPartA = 0;
		long numberOfLastExperienceInFoldTrainPartA = int(float(numberOfExperiences) * float(foldNum)/float(maxFolds));
		Experience * firstExperienceInFoldTrainPartA = firstExperienceInDataSet;

		long numberOfFirstExperienceInFoldTrainPartB = int(float(numberOfExperiences) * float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds));
		long numberOfLastExperienceInFoldTrainPartB = numberOfExperiences;
		Experience * firstExperienceInFoldTrainPartB = findExperience(firstExperienceInDataSet, numberOfFirstExperienceInFoldTrainPartB);

		long numberOfFirstExperienceInFoldTestPart = (int(float(numberOfExperiences) * float(foldNum)/float(maxFolds)));
		long numberOfLastExperienceInFoldTestPart = (int(float(numberOfExperiences) * float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds)));
		Experience * firstExperienceInFoldTestPart = findExperience(firstExperienceInDataSet, numberOfFirstExperienceInFoldTestPart);

		//training
		while((currentAverageTestingError > acceptableError) && (numEpochs < maxEpochs) && !overFittingData)	//CHANGED FROM training to averagedTesting BY RBB 17 sept 08
		{
			//from start of dataSet -> beginning of test segment

			#ifdef ANN_DEBUG
			//cout << "int(float(numberOfExperiences) * float(foldNum)/float(maxFolds)) = " << int(float(numberOfExperiences) * float(foldNum)/float(maxFolds));
			#endif

			Experience * currentExperienceInFold;

			currentExperienceInFold = firstExperienceInFoldTrainPartA;

			for(int experienceNum = numberOfFirstExperienceInFoldTrainPartA; experienceNum < numberOfLastExperienceInFoldTrainPartA; experienceNum++)
			{
				//sets inputData into ANN
				//(NB normalisedInputData[experienceNum][0] is target class

				resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperienceInFold);

				trainingError = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);

				#ifdef ANN_DEBUG
				//cout << currentExperienceInFold->classTargetValue;				
				//cout << "\t" << "current trainingError 1 = " << trainingError << endl;
				#endif

				currentExperienceInFold = currentExperienceInFold->next;

			}
			//from end of test segment -> end of dataSet


			currentExperienceInFold = firstExperienceInFoldTrainPartB;

			for(int experienceNum = numberOfFirstExperienceInFoldTrainPartB; experienceNum<numberOfLastExperienceInFoldTrainPartB; experienceNum++)
			{
				resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperienceInFold);

				trainingError = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);

				#ifdef ANN_DEBUG
				//cout << currentExperienceInFold->classTargetValue;				
				//cout << "\t" << "current trainingError 2 = " << trainingError << endl;
				#endif

				currentExperienceInFold = currentExperienceInFold->next;

			}

			numEpochs++;

			//testing

			//START OF TESTING FOR TRAINING EPOCH ep

			//creates and stores copy of the trained neural network

			storeNeuralNetworkBiasAndWeights(firstInputNeuron);

			//now determine the number of epochs ep that result in best performance on the respective test partition

			float  sumingTestingError = 0.0F;

			//from start of test segment -> end of test segment

			currentExperienceInFold = firstExperienceInFoldTestPart;

			for(int experienceNum = numberOfFirstExperienceInFoldTestPart; experienceNum < numberOfLastExperienceInFoldTestPart; experienceNum++)
			{
				restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);

				resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperienceInFold);

				testingError = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);

				sumingTestingError = sumingTestingError + testingError;

				#ifdef ANN_DEBUG
				//cout << "current testingError = " << testingError << endl;
				#endif
				
				currentExperienceInFold = currentExperienceInFold->next;
			}

			//resets the trained neural network to the stored copy

			restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);

			currentAverageTestingError = sumingTestingError/(float(numberOfExperiences)/float(maxFolds));

			//if(testingError > (recordedTestedError+float(ERROR_VARIATION_ALLOWED)))
			if(currentAverageTestingError > (recordedTestedError+float(ERROR_VARIATION_ALLOWED)))
			{
				//Started to overfit data...

				#ifdef ANN_USE_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA
				if(numEpochs > ANN_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA)
				{
					cout << "Overfitting was detected and training was stopped" << endl;
					cout << "currentAverageTestingError = " << currentAverageTestingError << endl;
					overFittingData	= true;
				}
				#else
					cout << "Overfitting was detected and training was stopped" << endl;
					cout << "currentAverageTestingError = " << currentAverageTestingError << endl;
					overFittingData	= true;
				#endif
			}
			//else if(testingError > recordedTestedError)
			else if(currentAverageTestingError > recordedTestedError)
			{
				//cout << "Potential imminent overfitting..." << endl;
				//do not create a recordedTestedError. Wait to see if there is further testing error increase
			}
			else
			{
				//probably not overfitting the data...

				//recordedTestedError = testingError;
				recordedTestedError = currentAverageTestingError;
			}

			//END OF TESTING FOR TRAINING EPOCH ep
		}

		trainingEpochsNumberArray[foldNum] = numEpochs;
		trainingErrorArray[foldNum] = trainingError;
		//testingErrorArray[foldNum] = testingError;
		testingErrorArray[foldNum] = currentAverageTestingError;

		cout << "Results for fold number = " << foldNum << endl;
		cout << "Error on training Set = " << trainingError << endl;
		cout << "Accuracy of training Set = " << 100.0F*(1.0F-trainingError) << "%" << endl;
		//cout << "Error on test Set = " <<  testingError << endl;
		//cout << "Accuracy of test Set = " <<  100.0F*(1.0F-testingError) << "%" << endl;
		cout << "Error on Averaged test Set = " <<  currentAverageTestingError << endl;
		cout << "Accuracy of Averaged test Set = " << 100.0F*(1.0F- currentAverageTestingError) << "%" << endl;
		cout << "Number of epochs = " << numEpochs << "\n" << endl;

		totalTrainingEpochs = totalTrainingEpochs + numEpochs;
		totalTrainingError = totalTrainingError + trainingError;
		//totalTestingError = totalTestingError + testingError;
		totalTestingError = totalTestingError + currentAverageTestingError;
	}

	//calculates and prints out average error;

	//calculate averages;
	float averageTrainingError = totalTrainingError/float(maxFolds);
	float averageTestingError = totalTestingError/float(maxFolds);
	float averageTotalTrainingEpochs = totalTrainingEpochs/float(maxFolds);

	//calculate standard deviations;
	float sdTrainingError = calculateStandardDeviation(trainingErrorArray, maxFolds, averageTrainingError);
	float sdTestingError = calculateStandardDeviation(testingErrorArray, maxFolds, averageTestingError);
	float sdTotalTrainingEpochs = calculateStandardDeviation(trainingEpochsNumberArray, maxFolds, averageTotalTrainingEpochs);

	//time feedback (used to calculate the optimum number of hidden neurons
	/*
	struct tm *myTime;
	time_t myClock = time(NULL);
	char buf[BUFSIZ];
	myTime = localtime(&myClock);
	#ifdef ANN_DEBUG
	//cout << "time = " << asctime(myTime) << endl;
	#endif	
	*/

	cout << "\n*****************************************************" << endl;
	cout << "Number of Input Neurons = " << numberOfInputNeurons << endl;
	cout << "Number of Output Neurons = " << numberOfOutputNeurons  <<endl;
	cout << "Average Training Error = " << averageTrainingError <<endl;
	cout << "Average Training Accuracy = " << 100.0F*(1.0F-averageTrainingError) << "%" <<endl;
	cout << "Average Testing Error = " << averageTestingError <<endl;
	cout << "Average Testing Accuracy = " << 100.0F*(1.0F-averageTestingError) << "%" <<endl;
	cout << "Average number of training Epochs = " << averageTotalTrainingEpochs <<endl;
	cout << "Standard Deviation Training Error = " << sdTrainingError <<endl;
	cout << "Standard Deviation Testing Error = " << sdTestingError <<endl;
	cout << "Standard Deviation of number of training Epochs = " << sdTotalTrainingEpochs <<endl;


	//now train the network on all examples for the mean number of epochs across all folds
	resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuron);

	float finalError;
	for(int e=0;e < averageTotalTrainingEpochs;e++)
	{
		Experience * currentExperience = firstExperienceInDataSet;

		//from start of dataSet -> end of dataSet
		for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
		{
			resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);
			
			finalError = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);

			#ifdef ANN_DEBUG
			//cout << currentExperience->classTargetValue;			
			//cout << "\t finalError = " << finalError << endl;
			#endif
			
			currentExperience = currentExperience->next;
		}
	}

	storeNeuralNetworkBiasAndWeights(firstInputNeuron);	//store these for future use of the network (such that they are not lost during future back props)


#ifdef TEMP_OUTPUT_HIGH_ERROR_EXPERIENCES
	Experience * firstExperienceInHighErrorExperienceList = new Experience();
	Experience * currentExperienceInHighErrorExperienceList = firstExperienceInHighErrorExperienceList;
#endif

	long experiencesIncorrectlyDiagnosed = 0;

	//Added by RBB; now test the network across all examples and calculate the average error
	Experience * currentExperience = firstExperienceInDataSet;
	float  sumingTestingError = 0.0F;
	long numberOfExperiencesContributingTowardsFinalTestingError = 0;

	#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
	//debugPrintNNOutputs = true;
	#endif

	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);

		resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

		float testingError = ANNBackPropogationPass(firstInputNeuron, firstOutputNeuron);

		#ifdef ANN_DEBUG
		//cout << currentExperience->classTargetValue;
		//cout << "\t testingError = " << testingError << endl;		
		#endif

	#ifdef DEBUG_DISREGARD_HIGH_ERROR_EXPERIENCES
		if(testingError < 0.8)
		{
			sumingTestingError = sumingTestingError + testingError;

			numberOfExperiencesContributingTowardsFinalTestingError++;
		}
		#ifdef TEMP_OUTPUT_HIGH_ERROR_EXPERIENCES
			else
			{
				Experience * newExperience = new Experience();
				copyExperiences(currentExperienceInHighErrorExperienceList, currentExperience);
				currentExperienceInHighErrorExperienceList->next = newExperience;
				currentExperienceInHighErrorExperienceList = currentExperienceInHighErrorExperienceList->next;
			}
		#endif
	#else
		sumingTestingError = sumingTestingError + testingError;

		numberOfExperiencesContributingTowardsFinalTestingError++;
	#endif

		if(testingError > 0.5)
		{
			experiencesIncorrectlyDiagnosed++;
		}

		currentExperience = currentExperience->next;
	}

	restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);

	delete trainingEpochsNumberArray;
	delete trainingErrorArray;
	delete testingErrorArray;


#ifdef TEMP_OUTPUT_HIGH_ERROR_EXPERIENCES
	ofstream experienceDataSetOfStreamObject("highErrorExperiences.data");
	addExperiencesToOFStream(&experienceDataSetOfStreamObject, firstExperienceInHighErrorExperienceList);
	experienceDataSetOfStreamObject.close();
#endif

	//cout << "Final training accuracy for e epochs (e det. from 10 fold cross validation) = " << (1.0F-finalError)*100.0F << "%" <<endl;
	cout << "Final averaged testing accuracy for e epochs (e from 10 fold cross validation) = " << (1.0F-(sumingTestingError/(double)numberOfExperiencesContributingTowardsFinalTestingError))*100.0F << "%" <<endl;
	cout << "Final Real Life accuracy (based upon number of experiences correctly diagnosed) = " << (1.0F-((double)experiencesIncorrectlyDiagnosed/(double)numberOfExperiencesContributingTowardsFinalTestingError))*100.0F << "%" <<endl;
	cout << "numberOfExperiencesContributingTowardsFinalTestingError = " << numberOfExperiencesContributingTowardsFinalTestingError << endl;
	cout << "numberOfExperiences = " << numberOfExperiences << endl;
}




/************************************************************ End Neural Network Routines ******************************************************/



























/************************************************************ Store/Record Neural Network weights and biases Routines ******************************************************/


void storeNeuralNetworkBiasAndWeights(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nvoid storeNeuralNetworkBiasAndWeights(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	NeuronContainer * currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference -> nextNeuronContainer != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA neuron has been selected for recording Bias And front neuron connection Weights" << endl;
		cout << "NeuronContainer ID = " << currentNeuronReference->neuron->id << endl;
		cout << "NeuronContainer ID Order = " << currentNeuronReference->neuron->orderID << endl;
		cout << "NeuronContainer ID Layer = " << currentNeuronReference->neuron->layerID << endl;
		cout << "NeuronContainer ID Subnet = " << currentNeuronReference->neuron->subnetID << endl;
		#endif

		if(neuronBeingAccessed->hasBackLayer)	//input layer does not have biases [CHECK ANNTHIS; NB in ANN, an input layer to a subnet may have a bias, however regardless of this, storeNeuralNetworkBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			currentNeuronReference->neuron->storedBias = currentNeuronReference->neuron->bias;

			#ifdef ANN_DEBUG
			cout << "NeuronContainer Bias = " << currentNeuronReference->neuron->bias << endl;			//NB first/input layer biases are irrelevant
			#endif
		}

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, storeNeuralNetworkBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			NeuronConnectionContainer * currentNeuronReferenceInFrontLayer = currentNeuronReference->firstFrontNeuronConnectionContainer;

			while(currentNeuronReferenceInFrontLayer->nextNeuronConnectionContainer != NULL)
			{
				currentNeuronReferenceInFrontLayer->neuronConnection->storedWeight = currentNeuronReferenceInFrontLayer->neuronConnection->weight;

				#ifdef ANN_DEBUG
				cout << "Front NeuronContainer Connection Weight = " << currentNeuronReferenceInFrontLayer->neuronConnection->weight << endl;
				#endif

				currentNeuronReferenceInFrontLayer = currentNeuronReferenceInFrontLayer->nextNeuronConnectionContainer;
			}

		#ifdef ANN_ADVANCED
			if(currentNeuronReference->isSubnet)
			{
				storeNeuralNetworkBiasAndWeights(currentNeuronReference->firstNeuronContainerInBackLayerOfSubnet);
			}
		#endif
		}

		currentNeuronReference = currentNeuronReference -> nextNeuronContainer;
	}

	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		storeNeuralNetworkBiasAndWeights(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}


/************************************************************ End Store/Record Neural Network weights and biases Routines **************************************************/




/************************************************************ Restore Neural Network with previously recorded weights and biases Routines ******************************************************/


void restoreNeuralNetworkWithStoredBiasAndWeights(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nvoid restoreNeuralNetworkWithStoredBiasAndWeights(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	NeuronContainer * currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference -> nextNeuronContainer != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA neuron has been selected for Reseting with previously stored Bias And front neuron connection Weights" << endl;
		cout << "NeuronContainer ID = " << currentNeuronReference->neuron->id << endl;
		cout << "NeuronContainer ID Order = " << currentNeuronReference->neuron->orderID << endl;
		cout << "NeuronContainer ID Layer = " << currentNeuronReference->neuron->layerID << endl;
		cout << "NeuronContainer ID Subnet = " << currentNeuronReference->neuron->subnetID << endl;
		#endif

		if(neuronBeingAccessed->hasBackLayer)	//input layer does not have biases[CHECK ANNTHIS; NB in ANN, an input layer to a subnet may have a bias, however regardless of this, restoreNeuralNetworkWithStoredBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			currentNeuronReference->neuron->bias = currentNeuronReference->neuron->storedBias;

			#ifdef ANN_DEBUG
			cout << "NeuronContainer Bias = " << currentNeuronReference->neuron->bias << endl;				//NB first/input layer biases are irrelevant
			#endif
		}


		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, restoreNeuralNetworkWithStoredBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			NeuronConnectionContainer * currentNeuronReferenceInFrontLayer = currentNeuronReference->firstFrontNeuronConnectionContainer;

			while(currentNeuronReferenceInFrontLayer->nextNeuronConnectionContainer != NULL)
			{
				currentNeuronReferenceInFrontLayer->neuronConnection->weight = currentNeuronReferenceInFrontLayer->neuronConnection->storedWeight;

				#ifdef ANN_DEBUG
				cout << "Front NeuronContainer Connection Weight = " << currentNeuronReferenceInFrontLayer->neuronConnection->weight << endl;
				#endif

				currentNeuronReferenceInFrontLayer = currentNeuronReferenceInFrontLayer->nextNeuronConnectionContainer;

			}

		#ifdef ANN_ADVANCED
			if(currentNeuronReference->isSubnet)
			{
				restoreNeuralNetworkWithStoredBiasAndWeights(currentNeuronReference->firstNeuronContainerInBackLayerOfSubnet);
			}
		#endif
		}

		currentNeuronReference = currentNeuronReference -> nextNeuronContainer;
	}


	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		restoreNeuralNetworkWithStoredBiasAndWeights(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}



/************************************************************ End Restore Neural Network with previously recorded weights and biases Routines **************************************************/




/************************************************************ Reset Neural Network with random biases and weights Routines ******************************************************/

void resetNeuralNetworkWithRandomBiasAndWeights(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nvoid resetNeuralNetworkWithRandomBiasAndWeights(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	NeuronContainer * currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference -> nextNeuronContainer != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA neuron has been selected for Reseting with Random Bias And front neuron connection Weights" << endl;
		cout << "NeuronContainer ID = " << currentNeuronReference->neuron->id << endl;
		cout << "NeuronContainer ID Order = " << currentNeuronReference->neuron->orderID << endl;
		cout << "NeuronContainer ID Layer = " << currentNeuronReference->neuron->layerID << endl;
		cout << "NeuronContainer ID Subnet = " << currentNeuronReference->neuron->subnetID << endl;
		#endif

		if(neuronBeingAccessed->hasBackLayer)	//input layer does not have biases [CHECK ANNTHIS; NB in ANN, an input layer to a subnet may have a bias, however regardless of this, resetNeuralNetworkWithRandomBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_USE_NEGATIVE_BIASES_AND_WEIGHTS
			currentNeuronReference->neuron->bias = (double(rand() * 1.0F)/(double(RAND_MAX)+0.0F));
			#else
			#ifdef TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS
			currentNeuronReference->neuron->bias = ((double(rand() * 2.0F)/(double(RAND_MAX)))-1.0F)*TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS_BIAS_MULT;
			#else
			#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_RANDOMISE_BIASES
			currentNeuronReference->neuron->bias = 0.5;
			#else
			#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
			currentNeuronReference->neuron->bias = (double(777 * 2.0F)/(double(RAND_MAX)))-1.0F;
			#else
			#ifdef DEBUG_TH_ANN_USE_ORIGINAL_RANDOMISATION
			currentNeuronReference->neuron->bias = (double(rand() * 2.0F)/(double(RAND_MAX)+1.0F))-1.0F;
			#else
			currentNeuronReference->neuron->bias = (double(rand() * 2.0F)/(double(RAND_MAX)))-1.0F;
			#endif
			//cout << "currentNeuronReference->neuron->bias = " << currentNeuronReference->neuron->bias << endl;
			#endif
			#endif
			#endif
			#endif

			#ifdef ANN_DEBUG
			cout << "NeuronContainer Bias = " << currentNeuronReference->neuron->bias << endl;
			#endif
		}

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, resetNeuralNetworkWithRandomBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			NeuronConnectionContainer * currentNeuronReferenceInFrontLayer = currentNeuronReference->firstFrontNeuronConnectionContainer;

			while(currentNeuronReferenceInFrontLayer->nextNeuronConnectionContainer != NULL)
			{
				#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_USE_NEGATIVE_BIASES_AND_WEIGHTS
				currentNeuronReferenceInFrontLayer->neuronConnection->weight = (double(rand() * 1.0F)/(double(RAND_MAX)+0.0F));
				#else
				#ifdef TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS
				currentNeuronReferenceInFrontLayer->neuronConnection->weight = ((double(rand() * 2.0F)/(double(RAND_MAX)))-1.0F)*TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS_WEIGHT_MULT;
				#else
				#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_RANDOMISE_WEIGHTS
				currentNeuronReferenceInFrontLayer->neuronConnection->weight = -0.1;
				#else
				#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
				currentNeuronReferenceInFrontLayer->neuronConnection->weight = (double(888 * 2.0F)/(double(RAND_MAX)))-1.0F;
				#else
				#ifdef DEBUG_TH_ANN_USE_ORIGINAL_RANDOMISATION
				currentNeuronReferenceInFrontLayer->neuronConnection->weight = (double(rand() * 2.0F)/(double(RAND_MAX)+1.0F))-1.0F;
				#else
				currentNeuronReferenceInFrontLayer->neuronConnection->weight = (double(rand() * 2.0F)/(double(RAND_MAX)))-1.0F;
				#endif
				#endif
				#endif
				#endif
				#endif

				#ifdef ANN_DEBUG
				//cout << "currentNeuronReferenceInFrontLayer->neuronConnection->weight = " << currentNeuronReferenceInFrontLayer->neuronConnection->weight << endl;				
				cout << "Front NeuronContainer Connection Weight = " << currentNeuronReferenceInFrontLayer->neuronConnection->weight << endl;
				#endif

				currentNeuronReferenceInFrontLayer = currentNeuronReferenceInFrontLayer->nextNeuronConnectionContainer;
			}

		#ifdef ANN_ADVANCED
			if(currentNeuronReference->isSubnet)
			{
				resetNeuralNetworkWithRandomBiasAndWeights(currentNeuronReference->firstNeuronContainerInBackLayerOfSubnet);
			}
		#endif
		}

		currentNeuronReference = currentNeuronReference -> nextNeuronContainer;
	}


	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		resetNeuralNetworkWithRandomBiasAndWeights(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}


/************************************************************ End Reset Neural Network with random biases and weights Routines **************************************************/





/************************************************************ Reset Input/ClassTarget Neurons Neural Network Routines ******************************************************/
void resetInputsAndClassTargets(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, Experience * currentExperienceInDataSet)
{
	#ifdef ANN_DEBUG
	cout << "\nvoid resetInputsAndClassTargets(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, int testSegment)" << endl;
	#endif

	//sets inputData into ANN
	//(NB normalisedInputData[i][0] is target class

	NeuronContainer * currentNeuronReference = firstInputNeuron;
	ExperienceInput * currentExperienceInputInExperience = currentExperienceInDataSet->firstExperienceInput;
	for(long i = 0; i < numberOfInputNeurons; i++)
	{
		#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_2
		cout << "currentExperienceInputInExperience->inputValue = " << currentExperienceInputInExperience->inputValue << endl;
		#endif
		currentNeuronReference->neuron->output = currentExperienceInputInExperience->inputValue;	 //normalisedInputData[testSegment][(i+1)];
		currentNeuronReference = currentNeuronReference->nextNeuronContainer;
		currentExperienceInputInExperience = currentExperienceInputInExperience->next;
	}

	currentNeuronReference = firstOutputNeuron;
	for(long i = 0; i < numberOfOutputNeurons; i++)
	{
		currentNeuronReference->neuron->classTarget = 0.0F;

		if(i == currentExperienceInDataSet->classTargetValue)
		{
			currentNeuronReference->neuron->classTarget = 1.0F;
		}

		currentNeuronReference = currentNeuronReference->nextNeuronContainer;
	}

#ifdef ANN_ADVANCED
	/*CHECK ANNTHIS: anything required for subnet inputs/outputs?*/
#endif

}

/************************************************************ End Reset Input/ClassTarget Neurons Neural Network Routines ******************************************************/



float calculateStandardDeviation(float* array, int length, float average)
{
	float sum = 0;

	for(int i=0;i<length;i++)
	{
		sum = sum + pow((array[i]-average),2);
	}

	float sd = sqrt(sum/float(length));

	return sd;
}