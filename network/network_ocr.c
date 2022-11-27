static const int numTrainingSets =10;
double init_weight()
{
        return ((double)rand())/((double)RAND_MAX);
}
double sigmoid(double z)
{
        return 1/(1+exp(-z));
}
double dSigmoid(double z)
{
        return x*(1-x);
}

void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

double main(int argc, const *argv[], double trinp,double trout, static const int ntrainset)
{
  const double learning_rate = 0.1f;

  static const int numInputs = 784;
  static const int numHiddenNodes = 100;
  static const int numOutputs = 10;
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];
  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];
  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];
  for(int i=0; i<numInputs;i++)
  {
	  for(j=0;j<numHiddenNodes;j++)
	  {
		  hiddenWeights[i][j]=init_weight()
	  }
  }
  for(int i=0; i<numHiddenNodes;i++)
  {
	  hiddenLayerBias[i] = init_weight();
	  for(int j=0;j<numOutputs;i++)
	  {
		  outputWeights[i][j]=init_weight();
	  }
  }
  for(int i=0;i<numOutputs;i++)
  {
	  outputLayerBias[i] = init_weight();
  }
  int trainingSetOrder[] = {0,1,2,3};
  for(int n =0; n<10000;n++)
  {
	
	shuffle(trainigSetOrder,numTrainingSets);
	for(int x=0; x<numTrainingSets;x++)
	{
		int i = trainingSetOrder[x];
		for(int j =0;j<numHiddenNodes;j++)
		{
			double activation = hiddenLayerBias[j];
			for(int k=0; k<numInputs;k++)
			{
				activation+=training_inputs[i][k]*hiddenWeights[k][j];

			}
			hiddenLayer[j]=sigmoid(activation);
		}
		for(int j=0;j<numOutputs;j++)
		{
			double activation = outputLayerBias[j];
			for(int k=0; k<numHiddenNodes;k++)
			{
				activation+=hiddenLayer[k]*outputWeights[k][j];

			}
			outputLayer[j] = sigmoid(activation);
		}
		double deltaOutputs[numOutputs]
		for(int j=0; j<numOutputs;j++)
		{
			double dError = (trout[i][j]-outputLayer[j]);
			deltaOutputs[j] = dError*dSigmoid(outputLayer[j]);


		}
		double deltaHiddens[numHiddenNodes];
		for(int j=0; j<numHiddenNodes;j++)
		{
			double hError = 0.0f;
			for(int k=0; k<numOutputs;k++)
			{
				hError += deltaOutputs[k]*outputWeights[k][j];
			}
			deltaHiddens[j]= hError*dSigmoid(hiddenLayer[j]);
		}
		for(int j=0; j<numOutputs;j++)
		{
			outputLayerBias[j] += deltaOutputs[j]*learning_rate;
		       for(int k=0; k<numHiddenNodes;k++)
		       {
			       outputWeights[k][j]+=hiddenLayer[k]*deltaOutputs[j]*learning_rate;
		       }	
		}
		for(int j=0; j<numHiddenNodes; j++)
		{
			HiddenLayerBias[j] += deltaHiddens[j]*learning_rate;
			for(int k = 0; k<numInputs; k++)
			{
				hiddenWeights[k][j] += trinp[i][k]*deltaHiddens[j]*learning_rate; 

			}
		}

	}

  }


  printf("Final Hidden Weights\n");
  for(int j=0; j<numHiddenNodes;j++)
      {

           for(int k =0;k<numInputs;k++)
           {
                    printf("[ ");

                    printf("%f ",hiddenWeights[k][j]);
                    printf("]\n ");
           }

       }


   printf(" Final Hidden Biases\n");
   for(int j=0; j<numHiddenNodes;j++)
        {    
	     	printf("[");

                printf("%f",hiddenLayerBias[j]);
                printf("]\n ");

        }
   printf("Final Output Weightd\n");
   for(int j=0; j<numOutputs;j++)
   {

        for(int k=0; k< numHiddenNodes ;k++)
           {       printf("[");
                   printf("%f", outputWeights[k][j]);
                   printf("]\n ");

            }
    }
    printf("Final Output Biases\n");
    for(int j=0; j<numOutputs;j++)
    {
            printf("[");
	    printf("%f ",outputLayerBias[j]);
            printf("]\n");

    }




  return 0;
}




size_t practice(double grayscale[])
{
        for(size_t j =0;j<numHiddenNodes;j++)
        {
          double activation = hiddenLayerBias[j];
          for(size_t k=0; k<numInputs;k++)
          {
              activation+=grayscale[k]*hiddenWeights[k][j];

          }
          hiddenLayer[j]=sigmoid(activation);
        }
        for(size_t j=0;j<numOutputs;j++)
        {
            double activation = outputLayersBias[j];
            for(size_t k=0; k<numHiddenNodes;k++)
            {
               activation+=hiddenLayer[k]*outputWeights[k][j];

            }
            outputLayer[j] = sigmoid(activation);
        }
        size_t max = 0;
        for(size_t j =0; j<numOutputs;j++)
        {
                if(outputLayer[j]>outputLayer[max])
                {
                        max = j;
                }

        }
        return max+1;


}

