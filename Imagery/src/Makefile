# Makefile

CPPFLAGS = -MMD 
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic `pkg-config --cflags sdl2 SDL2_image` -O2 -g3
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = main.c ImageProcessing.c ImageProcessing/GrayScale.c ImageProcessing/Pixels.c ImageProcessing/BlackAndWhite.c Result/construct.c ImageProcessing/NoiseReduction.c ImageProcessing/Morphology.c ImageProcessing/FloodFill.c ImageProcessing/BlobDetection.c ImageProcessing/OrderPoints.c Utils.c ImageProcessing/Rotation.c ImageProcessing/HoughTransform.c ImageProcessing/GradientCalculation.c ImageProcessing/CannyFilter.c ImageProcessing/NonMaxSuppression.c ImageProcessing/DoubleThreshold.c ImageProcessing/SquareCutter.c NeuralNetwork/MatUtils.c NeuralNetwork/NeuralNet.c NeuralNetwork/NeuralNetInit.c NeuralNetwork/TrainNeuralNet.c NeuralNetwork/LoadDataSet.c solver/sudoku.c Verbose.c ImageProcessing/HomographicTransphorm.c

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all:
	$(MAKE) main
	cd UserInterface && $(MAKE)

main: ${OBJ}

.PHONY: clean
clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program

	${RM} *.bmp # remove out files

	cd UserInterface && $(MAKE) clean # Clean UserInterface directory


-include ${DEP}

# END
