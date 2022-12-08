#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include<unistd.h>

#include <stdio.h>
#include <math.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/*
static const int numTrainingSets =10;
FILE *labels;
labels = fopen("train-labels.idx1-ubyte","rb");
int magic_number_lab;
int num_lab;
fread(&magic_number_lab,sizeof(int),1,labels);
fread(&num_lab,sizeof(int),1,labels);
double expected_result_write[num_lab];
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

double training_inputs_write[num_im][784];
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
*/
SDL_Surface* load_image(const char* path)
{


    SDL_Surface* pres = IMG_Load(path);

    if (pres==NULL)
            errx(EXIT_FAILURE,"%s",IMG_GetError());
    
    SDL_Surface* s = SDL_ConvertSurfaceFormat(pres,SDL_PIXELFORMAT_RGB888,0);

    if (s == NULL)
            errx(EXIT_FAILURE,"%s",SDL_GetError());
    SDL_FreeSurface(pres);
    return s;
}
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{

    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    r = average;
    b = average;
    g = average;
    Uint32 color = SDL_MapRGB(format,r,g,b);
    
    return color;
}

double* surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    double *grayscale = malloc(sizeof(double)*len);
    int lock = SDL_LockSurface(surface);
    if(lock!=0)
    {
            errx(EXIT_FAILURE, "Lock of surface failed");
    }
    int y = 0;
    for(size_t i  = 0; y<len; i++ )
    {


            pixels[i]= pixel_to_grayscale(pixels[i], format);
            y++;
	    grayscale[i] = (double)pixels[i];

    }
    SDL_UnlockSurface(surface);
    
    return grayscale;
}

double* get_grayscale(const char* path)
{
	SDL_Surface* s = load_image(path);
        return surface_to_grayscale(s);	
}








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
	if(sum==0)
	{
		return exp(raw_inp)/sum+1;
	}
	if(sum==INFINITY)
	{
		return exp(raw_inp)/(sum/exp(raw_inp));
	}
	
	
	
	return exp(raw_inp)/sum;
}
int which_result(double expected_result[])
{
	for(int i=0;i<10;i++)
	{
		if(expected_result[i]==1)
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
double cross(double prob,int i)
{

	if(prob==0 ||log(prob)==NAN|| log(prob)==INFINITY)
	{
		return -1*log(prob+0.5);
	}
	

//	printf("%f\n    %d\n",(-1)*log(prob),i);
//	sleep(1);
	
	return (-1)*log(prob);
	

}
double sigmoid(double z)
{
	if((1+exp(-z))==0)
	{
		return 1/(1+exp(-z)+0.01);
	}

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
        return z*(1-z);
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


double train(double **trinp,double trout[][10],  int ntrainset)
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
  for(int i =0; i<ntrainset;i++)
  {
	  cross_result[i]=0;
  }
  for(int i=0; i<numInputs;i++)
  {
	  for(int j=0;j<numHiddenNodes;j++)
	  {
		  hiddenWeights[i][j]=init_weight();
	  }
  }
  
  for(int i=0; i<numHiddenNodes;i++)
  {
	  hiddenLayerBias[i] = init_weight();
	  for(int j=0;j<numOutputs;j++)
	  {
		  outputWeights[i][j]=init_weight();
	  }
  }
  
  for(int i=0;i<numOutputs;i++)
  {
	  outputLayerBias[i] = init_weight();
  }
  int trainingSetOrder[ntrainset] ;
  for(int i =0; i<ntrainset;i++)
  {
	  trainingSetOrder[i] = i;
  }
  FILE *startweight = NULL;
  startweight = fopen("StartWeight.txt","w+");
  for(int j=0; j<numOutputs;j++)
  {
       for(int k=0; k<numHiddenNodes;k++)
       {

              double d = outputWeights[k][j];
              fprintf(startweight,"%f\n",d);




        }
  }
  fclose(startweight);


  
  while(total_cross>0.05)
  {
	  
	
	
	
	shuffle(trainingSetOrder,ntrainset);
	for(int x=0; x<ntrainset;x++)
	{
		
		

		int i = trainingSetOrder[x];
		
		double *set = trinp[i];
		

		for(int j =0;j<numHiddenNodes;j++)
		{
			double activation = hiddenLayerBias[j];
			for(int k=0; k<numInputs;k++)
			{	
				
				activation+= *(set+k) * hiddenWeights[k][j];
				

			}
			hiddenLayer[j]=sigmoid(activation);
		}
		int index_expected = which_result(trout[i]);

		for(int j=0;j<numOutputs;j++)
		{
			double activation = outputLayerBias[j];
			for(int k=0; k<numHiddenNodes;k++)
			{
				activation+=hiddenLayer[k]*outputWeights[k][j];

			}
		
			
			outputLayer[j] = sigmoid(activation);
			if(index_expected==j)
                        {
                                

                        }

		}
		
		double Rawoutputs[numOutputs];
		for(int j=0; j<numOutputs;j++)
		{
			Rawoutputs[j]=outputLayer[j];
		}
		
              
                

		
		for(int j =0; j<numOutputs;j++)
		{
			
			
			outputLayer[j] = softmax(outputLayer[j],Rawoutputs);
		}

		
		cross_result[i] = cross(outputLayer[index_expected],i);
		
		
		double deltaOutputs[numOutputs];
		for(int j=0; j<numOutputs;j++)
		{
			double dError = (trout[i][j]-outputLayer[j]);
			deltaOutputs[j] = dError*Dsoftmax(index_expected,outputLayer[index_expected],j,outputLayer[j]);


		}
		double deltaHiddens[numHiddenNodes];
		for(int j=0; j<numHiddenNodes;j++)
		{
			double hError = 0.0f;
			for(int k=0; k<numOutputs;k++)
			{
				hError += deltaOutputs[k]*outputWeights[j][k];
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
			hiddenLayerBias[j] += deltaHiddens[j]*learning_rate;
			for(int k = 0; k<numInputs; k++)
			{
				hiddenWeights[k][j] += trinp[i][k]*deltaHiddens[j]*learning_rate; 

			}
		}

	}
	total_cross=0;
  	for(int i=0; i<ntrainset; i++)
  	{
          	total_cross+= cross_result[i];
  	}
	

 


  }

  FILE *outputweights = NULL;
  outputweights = fopen("Outputweights.txt","w+");
  for(int j=0; j<numOutputs;j++)
  {
       for(int k=0; k<numHiddenNodes;k++)
       {

              double d = outputWeights[k][j];
              fprintf(outputweights,"%f\n",d);

	      


        }
  }
  for(int j=0; j<numHiddenNodes;j++)
  {
	  for(int k=0; k<numInputs; k++)
	  {
		  double d = hiddenWeights[k][j];
                  fprintf(outputweights,"%f\n",d);
	  }
  }


  for(int j=0; j<numHiddenNodes;j++)
  {
	  double d = hiddenLayerBias[j];
	  fprintf(outputweights,"%f\n",d);

  }
  for(int j=0; j<numOutputs;j++)
  {
          double d = outputLayerBias[j];
          fprintf(outputweights,"%f\n",d);

  }


  fclose(outputweights);
  





  return 0;
}




size_t practice(double grayscale[])
{
	static const int numInputs = 784;
  	static const int numHiddenNodes = 30;
  	static const int numOutputs = 10;
  	
	double *hiddenLayer = malloc(sizeof(double)*numHiddenNodes);
	double *outputLayer = malloc(sizeof(double)*numOutputs);
  	
	double *hiddenLayerBias = malloc(sizeof(double)*numHiddenNodes);
  	
	double *outputLayerBias = malloc(sizeof(double)*numOutputs);
  	double hiddenWeights[numInputs][numHiddenNodes];
  	double outputWeights[numHiddenNodes][numOutputs];
	double activation =0;
	FILE *outputout = NULL;
        outputout = fopen("Outputweights.txt","r");
        char str[1000];
	int nhnw = 0;
	int now = 0;
        while(fgets(str,1000,outputout)!=NULL && nhnw<numHiddenNodes)
        {
                char * vIn = str;
                double vOut = strtod(vIn,NULL);
		outputWeights[nhnw][now];
		now++;
		if(now>=numOutputs)
		{
			now=0;
			nhnw++;
		}
        }
	now=0;
	nhnw=0;
	int ninp=0;
	while(fgets(str,1000,outputout)!=NULL && ninp<numInputs)
        {
                char * vIn = str;
                double vOut = strtod(vIn,NULL);
                hiddenWeights[ninp][nhnw];
                nhnw++;
                if(nhnw>=numHiddenNodes)
                {
                        nhnw=0;
                        ninp++;
                }
        }
	ninp=0;
	nhnw =0;
	while(fgets(str,1000,outputout)!=NULL && nhnw<numHiddenNodes)
        {
                char * vIn = str;
                double vOut = strtod(vIn,NULL);
                hiddenLayerBias[nhnw];
                nhnw++;
                
        }
	while(fgets(str,1000,outputout)!=NULL && nhnw<numOutputs)
        {
                char * vIn = str;
                double vOut = strtod(vIn,NULL);
                hiddenLayerBias[now];
                now++;

        }

	

	
        fclose(outputout);

        for(int j =0;j<numHiddenNodes;j++)
        {
          double activation = hiddenLayerBias[j];
          for(int k=0; k<numInputs;k++)
          {
              activation+=grayscale[k]*hiddenWeights[k][j];

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
	double Rawoutputs[numOutputs];
        for(int j=0; j<numOutputs;j++)
        {
             Rawoutputs[j]=outputLayer[j];
        }


        for(int j = 0; j<numOutputs;j++)
        {
            outputLayer[j] = softmax(outputLayer[j],Rawoutputs);
        }

        int max = 0;
        for(int j =0; j<numOutputs;j++)
        {
                if(outputLayer[j]>outputLayer[max])
                {
                        max = j;
                }

        }
        return max;


}
int main()
{

	double **training_inputs_artif;
	training_inputs_artif =  malloc(600*sizeof(double*));
	for(int i=0; i<600; i++)
	{
        	training_inputs_artif[i] = calloc(784,sizeof(double));
	}

	double training_label_artif[600][10];
	FILE *label = NULL;
        label = fopen("labels.txt","w+");

       	
	char buffer[200];
	size_t ind_im =0;
	for(int i=0; i<10; i++)
	{
        
        	for(int j = 1; j<=5; j++)
        	{
      
                	for(int k=1; k<=4;k++)
                	{
                
                        	for(int x =1; x<=4; x*=2)
                        	{
                
					
					int p = snprintf(buffer,200,"num/%d.%d.%d.1%d.png",i,j,k,x);
					
					double *pixs = get_grayscale(buffer);
					for(size_t m =0; m<784;m++)
					{
						pixs[m] /= 255;
					}
 	                               	*(training_inputs_artif+ind_im)= pixs;
                                	training_label_artif[ind_im][i] = 1;
                                	ind_im++;
                        	}
                	}
        	}

	}
	for(int j=0; j<600;j++)
        {


                double *d = training_label_artif[j];
                fprintf(label,"{");
                for(int x=0; x<10 ; x++)
                {
                        fprintf(label,"%f",d[x]);
                }

                fprintf(label,"}\n");





        }

        fclose(label);

	FILE *zero = NULL;
        zero = fopen("0.txt","w+");
	double *zerod = training_inputs_artif[0];
        fprintf(zero,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(zero,"%f\n",zerod[x]);
        }

        fprintf(zero,"}\n");
	fclose(zero);

	FILE *one = NULL;
        one = fopen("1.txt","w+");
        double *oned = training_inputs_artif[60];
        fprintf(one,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(one,"%f\n",oned[x]);
        }

        fprintf(one,"}\n");
        fclose(one);

	FILE *two = NULL;
        two = fopen("2.txt","w+");
        double *twod = training_inputs_artif[120];
        fprintf(two,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(two,"%f\n",twod[x]);
        }

        fprintf(two,"}\n");
        fclose(two);

	FILE *three = NULL;
        three = fopen("3.txt","w+");
        double *threed = training_inputs_artif[180];
        fprintf(three,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(three,"%f\n",threed[x]);
        }

        fprintf(three,"}\n");
        fclose(three);

	FILE *four = NULL;
        four = fopen("4.txt","w+");
        double *fourd = training_inputs_artif[240];
        fprintf(four,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(four,"%f\n",fourd[x]);
        }

        fprintf(four,"}\n");
        fclose(four);

	FILE *five = NULL;
        five = fopen("5.txt","w+");
        double *fived = training_inputs_artif[300];
        fprintf(five,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(five,"%f\n",fived[x]);
        }

        fprintf(five,"}\n");
        fclose(five);


	FILE *six = NULL;
        six = fopen("6.txt","w+");
        double *sixd = training_inputs_artif[360];
        fprintf(label,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(six,"%f\n",sixd[x]);
        }

        fprintf(six,"}\n");
        fclose(six);



	FILE *seven = NULL;
        seven = fopen("7.txt","w+");
        double *sevend = training_inputs_artif[420];
        fprintf(seven,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(seven,"%f\n",sevend[x]);
        }

        fprintf(seven,"}\n");
        fclose(seven);

	FILE *eight = NULL;
        eight = fopen("8.txt","w+");
        double *eightd = training_inputs_artif[480];
        fprintf(eight,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(eight,"%f\n",eightd[x]);
        }

        fprintf(eight,"}\n");
        fclose(eight);

	FILE *nine = NULL;
        nine = fopen("9.txt","w+");
        double *nined = training_inputs_artif[540];
        fprintf(nine,"{");
        for(int x=0; x<784 ; x++)
        {
               fprintf(nine,"%f\n",nined[x]);
        }

        fprintf(nine,"}\n");
        fclose(nine);






	
	train(training_inputs_artif,training_label_artif,600);


	


	
	return 1;

};
