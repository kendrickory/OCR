static const int numTrainingSets =10;
FILE *labels;
labels = fopen("train-labels.idx1-ubyte","rb");
int magic_number_lab;
int num_lab;
fread(&magic_number_lab,sizeof(int),1,labels);
fread(&num_lab,sizeof(int),1,labels);
double expected_result[num_lab];
unsigned char label;
while(fread(&label,sizeof(unsigned char),1,labels) != EOF))
{
	expected_result.append((double)label);

}
FILE *images;
images = fopen("train-images.idx3-ubyte","rb");
int magic_number_image;
int num_im;
fread(&magic_number_image,sizeof(int),1,images);
fread(&num_im,sizeof(int),1,images);

double training_inputs[num_im][784];
unsigne char pix;
int x =0;
int y=0;
while(fread(&pix,sizeof(unsigned char),1,images) != EOF))
{
	
        training_inputs[x].append((double)label);
	y++;
	if(y>=784)
	{
		y=0;
		x++;
	}

}

fclose(labels);
fclose(images);
double init_weight()
{
        return ((double)rand())/((double)RAND_MAX);
}
double softmax(double raw_inp, double raw_list[])
{
	double sum = 0;
	for(size_t i=0; i<10;i++)
	{
		sum+= exp(raw_list[i]);
	}
	return raw_inp/sum;
}
size_t which_result(double expected_result[])
{
	for(size_t i=0;i<10;i++)
	{
		if(expected_result[i]==0)
		{
			return i;
		}
	}
}
double Dsoftmax(size_t x,double num,size_t y,double denum)
{
	if(y==x)
	{
		return num*(1-num);
	}
	return (-1)*num*denum;
}
double cross(double prob)
{
	return (-1)*log(prob);
}
double sigmoid(double z)
{
        return 1/(1+exp(-z));
}
double dcross(double z, size_t expected, size_t output)
{
	if(expected==output)
	{
		return z-1;
	}
	else
	{
		return z;
	}
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


double train(double trinp[],double trout[], static const int ntrainset)
{
  const double learning_rate = 0.1f;

  static const int numInputs = 784;
  static const int numHiddenNodes = 30;
  static const int numOutputs = 10;
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];
  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];
  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];
  
  double cross_result[ntrainset];
  
  double total_cross = 100;
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
  int trainingSetOrder[ntraingset] ;
  for(int i =0; i<ntrainset;i++)
  {
	  trainingSetOrder[i] = i;
  }
  while(total_cross>0.05)
  {
	
	shuffle(trainingSetOrder,trainingset);
	for(int x=0; x<numTrainingSets;x++)
	{
		int i = trainingSetOrder[x];
		for(int j =0;j<numHiddenNodes;j++)
		{
			double activation = hiddenLayerBias[j];
			for(int k=0; k<numInputs;k++)
			{
				activation+=trinp[i][k]*hiddenWeights[k][j];

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
		size_t index_expected = wich_result(training_inputs[i]);
		double Rawoutput[numOutputs] = outputLayer;
		for(intj=0; j<numOutputs;j++)
		{
			outputLayer[j] = softmax(outputLayer[j],Rawoutput);
		}
		
		cross_result[i] = cross(outputLayer[index_expected]);
		
		
		double deltaOutputs[numOutputs]
		for(int j=0; j<numOutputs;j++)
		{
			double dError = (trout[i][j]-outputLayer[j]);
			deltaOutputs[j] = dError*dsoftmax(index_expected,outputLayer[index_expected],j,outputLayer[j]);


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
	total_cross=0;
  	for(size_t i=0; i<numtrainset; i++)
  	{
          	total_cross+= cross_result[i];
  	}

 


  }

  FILE *outputweights = NULL;
  outputweights = fopen("Outputweights.txt","w");
  for(size_t j=0; j<nmOut;j++)
  {
       for(size_t k=0; k<nmHNodes;k++)
       {

              double d = outWeights[k][j];
              fprintf(outputweights,"%f\n",d);




        }
  }
  for(size_t j=0; j<numhiddenNodes;j++)
  {
	  for(size_t k=0; k<numInputs; k++)
	  {
		  double d = hiddenWeights[k][j];
                  fprintf(outputweights,"%f\n",d);
	  }
  }


  for(size_t j=0; j<numhiddenNodes;j++)
  {
	  double d = hiddenBias[j];
	  fprintf(outputweights,"%f\n",d);

  }
  for(size_t j=0; j<numOuputs;j++)
  {
          double d = OuputLayerBias[j];
          fprintf(outputweights,"%f\n",d);

  }


  fclose(outputweights);
  





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
	double Rawoutput[numOutputs] = outputLayer;
            for(intj=0; j<numOutputs;j++)
            {
                outputLayer[j] = softmax(outputLayer[j],Rawoutput);
            }

        size_t max = 0;
        for(size_t j =0; j<numOutputs;j++)
        {
                if(outputLayer[j]>outputLayer[max])
                {
                        max = j;
                }

        }
        return max;


}
double train();
