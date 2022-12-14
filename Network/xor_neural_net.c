#include <stdlib.h>


#include <stdio.h>
#include <math.h>


double init_weight()
{
        return ((double)rand())/((double)RAND_MAX);
}
double sigmoid(double z)
{
        return 1/(1+exp(-z));
}
double dSigmoid(double x)
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





double practice(double input1,double input2, double hLayerB[], double outLayerB[], double hWeights[][2], double outWeights[][1])
{
        double inputs[2] = {input1,input2};
        double hiddenlayer[2];

        double outputlayer[1];
        for(size_t j =0;j<2;j++)
        {
            double activation = hLayerB[j];
            for(size_t k=0; k<2;k++)
            {
                 activation+=inputs[k]*hWeights[k][j];

            }
            hiddenlayer[j]=sigmoid(activation);

        }
                        // compute output layers activation
        for(size_t j=0;j<1;j++)
        {
             double activation = outLayerB[j];
             for(size_t k=0; k<2;k++)
             {
                    activation+=hiddenlayer[k]*outWeights[k][j];

             }
             outputlayer[j] = sigmoid(activation);
        }
        
        return outputlayer[0];

}



int main()
{
        static const size_t nmInp = 2;
        static const size_t nmHNodes = 2;
        static const size_t nmOut = 1;
        double hLayer[nmHNodes];
        double outLayer[nmOut];
        double hLayerB[nmHNodes];
        double outLayerB[nmOut];
        double hWeights[nmInp][nmHNodes];
        double outWeights[nmHNodes][nmOut];
        static const size_t numTrainingSets = 4;
        double training_inputs[4][2] = { {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} };
        double training_outputs[4][1] = { {0.0f},{1.0f},{1.0f},{0.0f} };

        double learning_rate = 0.1f;

        int trainingSetOrder[] = {0,1,2,3};


        for(size_t i=0; i<nmInp;i++)
        {

                for(size_t j=0;j<nmHNodes;j++)
                {
                        hWeights[i][j]=init_weight();
                }


        }

        for(size_t i=0; i<nmHNodes;i++)
        {
                hLayerB[i] = init_weight();
                for(size_t j=0;j<nmOut;j++)
                {
                        outWeights[i][j]=init_weight();
                }

        }
        for(size_t i=0;i<nmOut;i++)
        {
                outLayerB[i] = init_weight();

        }

        for(size_t n =0; n<100000; n++)
        {
                shuffle(trainingSetOrder,numTrainingSets);
                for(size_t x=0; x<numTrainingSets;x++)
                {
                        int i = trainingSetOrder[x];
                        // Forward pass
                        // compute hidden layer activation
                        for(size_t j =0;j<nmHNodes;j++)
                        {
                                double activation = hLayerB[j];
                                for(size_t k=0; k<nmInp;k++)
                                {
                                        activation+=training_inputs[i][k]*hWeights[k][j];

                                }
                                hLayer[j]=sigmoid(activation);

                        }
                        // compute output layers activation
                        for(size_t j=0;j<nmOut;j++)
                        {
                                double activation = outLayerB[j];
                                for(size_t k=0; k<nmHNodes;k++)
                                {
                                        activation+=hLayer[k]*outWeights[k][j];

                                }
                                outLayer[j] = sigmoid(activation);
                        }
			//Backprobagation
                        //Compute change in output weihghts
                        double deltaOutputs[nmOut];
                        for(size_t j=0; j<nmOut;j++)
                        {
                                double dError = (training_outputs[i][j]-outLayer[j]);
                                deltaOutputs[j] = dError*dSigmoid(outLayer[j]);


                        }
                        //Compute change in hidden weights
                        double deltaHiddens[nmHNodes];
                        for(size_t j=0; j<nmHNodes;j++)
                        {
                                double hError = 0.0f;
                                for(size_t k=0; k<nmOut;k++)
                                {
                                        hError += deltaOutputs[k]*outWeights[j][k];
                                }
                                deltaHiddens[j]= hError*dSigmoid(hLayer[j]);
                        }
                        //Apply change in outputs weights
                        for(size_t j=0; j<nmOut;j++)
                        {
                                outLayerB[j] += deltaOutputs[j]*learning_rate;
                                for(size_t k=0; k<nmHNodes;k++)
                                {
                                        outWeights[k][j]+=hLayer[k]*deltaOutputs[j]*learning_rate;
                                }
                        }
                        //Apply change to hidden weights
                        for(size_t j=0; j<nmHNodes; j++)
                        {
                                hLayerB[j] += deltaHiddens[j]*learning_rate;
                                for(size_t k = 0; k<nmInp; k++)
                                {
                                        hWeights[k][j] += training_inputs[i][k]*deltaHiddens[j]*learning_rate;

                                }
                        }


                }

        }
        printf("Final Hidden Weights\n");
	for(size_t j=0; j<nmHNodes;j++)
        {

                for(size_t k =0;k<nmInp;k++)
                {
                        printf("[ ");

                        printf("%f ",hWeights[k][j]);
                        printf("]\n ");

                }

        }


        printf(" Final Hidden Biases\n");
        for(size_t j=0; j<nmHNodes;j++)
        {       printf("[");

                printf("%f",hLayerB[j]);
                printf("]\n ");

        }
        printf("Final Output Weightd\n");
        for(size_t j=0; j<nmOut;j++)
        {

                for(size_t k=0; k< nmHNodes ;k++)
                {       printf("[");
                        printf("%f", outWeights[k][j]);
                        printf("]\n ");

                }

        }
        printf("Final Output Biases\n");
        for(size_t j=0; j<nmOut;j++)
        {
                printf("[");
                printf("%f ",outLayerB[j]);
                printf("]\n");

        }	
 	FILE *outputweights = NULL;     
        outputweights = fopen("Outputweights.txt","w+");
        for(size_t j=0; j<nmOut;j++)
        {
                for(size_t k=0; k<nmHNodes;k++)
                {
                        
                        double d = outWeights[k][j];
                        fprintf(outputweights,"%f\n",d);
                        
                        
                        

                }
        }


        fclose(outputweights);
	
	FILE *outputout = NULL;
        outputout = fopen("Outputweights.txt","r");
	char str[1000];
	while(fgets(str,1000,outputout)!=NULL)
	{
		char * vIn = str;
		double vOut = strtod(vIn,NULL);

		printf("%f\n",vOut);
	}	
	fclose(outputout);
        
        



	printf("R??sultat du XOR avec 0 et 0 comme entr??es %f\n", practice(0.0f,0.0f,hLayerB,outLayerB,hWeights,outWeights));
	printf("R??sultat du XOR avec 0 et 1 comme entr??es %f\n", practice(0.0f,1.0f,hLayerB,outLayerB,hWeights,outWeights));
	printf("R??sultat du XOR avec 1 et 0 comme entr??es %f\n", practice(1.0f,0.0f,hLayerB,outLayerB,hWeights,outWeights));
	printf("R??sultat du XOR avec 1 et 1 comme entr??es %f\n", practice(1.0f,1.0f,hLayerB,outLayerB,hWeights,outWeights));
	return 1;
        

}





