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

        for(size_t n =0; n<10000; n++)
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
	for(int j=0; j<nmHNodes;j++)
        {

                for(int k =0;k<nmInp;k++)
                {
                        printf("[ ");

                        printf("%f ",hWeights[k][j]);
                        printf("]\n ");

                }

        }


        printf(" Final Hidden Biases\n");
        for(int j=0; j<nmHNodes;j++)
        {       printf("[");

                printf("%f",hLayerB[j]);
                printf("]\n ");

        }
        printf("Final Output Weightd\n");
        for(int j=0; j<nmOut;j++)
        {

                for(int k=0; k< nmHNodes ;k++)
                {       printf("[");
                        printf("%f", outWeights[k][j]);
                        printf("]\n ");

                }

        }
        printf("Final Output Biases\n");
        for(int j=0; j<nmOut;j++)
        {
                printf("[");
                printf("%f ",outLayerB[j]);
                printf("]\n");

        }
/*      
        FILE* outputweights = fopen("Outputweights.txt","w+");
        for(size_t j=0; j<numOutputs;j++)
        {
                for(size_t k=0; k<numHiddenNodes;k++)
                {
                        char *s ="";
                        double d = outputWeights[k][j];
                        sprintf(s,"%.2lf",d);
                        
                        fputs(s,outputweights);
                        fputs("\n", outputweights);

                }
        }
        fclose(outputweights);
        
        
        FILE* hiddenweights = fopen("Hiddenweights","w+");
        for(size_t j=0; j<numHiddenNodes;j++)
        {
                for(int k =0 ; k<numInputs; k++)
                {
                        char *s="";
                        double d = hiddenWeights[k][j];
                        sprintf(s,"%.2lf",d);
                        
                        fputs(s,hiddenweights);
                        fputs("\n",hiddenweights);
                }
        }
        fclose(hiddenweights);

        FILE* outputbias = fopen("Outputbias", "w+");
        for(size_t j=0; j<numOutputs;j++)
        {
                char *s="";
                double d = outputLayerBias[j];
                sprintf(s,"%.2lf",d);
                
                fputs(s,outputbias);
                fputs("\n",outputbias);
        }


        fclose(outputbias);
FILE* hiddenbias = fopen("Hiddenbias","w+");
        for(size_t j=0; j<numHiddenNodes; j++)
        {
                char *s ="";
                double d = hiddenLayerBias[j];
                sprintf(s,"%.2lf",d);
                
                fputs(s,hiddenbias);
                fputs("\n",hiddenbias);
        }
        fclose(hiddenbias);

*/
        return 0;

}
	
