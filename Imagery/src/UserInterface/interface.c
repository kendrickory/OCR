#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#pragma GCC diagnostic pop
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include <ctype.h>
#include <SDL.h>
#include <stdbool.h>
#include <err.h>

#define DEFINE_GLOBALS

#include "../ImageProcessing/GrayScale.h"
#include "../ImageProcessing/NoiseReduction.h"
#include "../ImageProcessing/BlackAndWhite.h"
#include "../ImageProcessing/HoughTransform.h"
#include "utilsUI.h"
#include "../ImageProcessing/Rotation.h"
#include "../ImageProcessing.h"
#include "../ImageProcessing/HomographicTransphorm.h"
#include "../ImageProcessing/OrderPoints.h"
#include "../ImageProcessing/BlobDetection.h"
#include "../Verbose.h"
#include "../Result/construct.h"

int VERBOSE_LEVEL = 3;
const char* MODE_STRING[] = {
    FOREACH_MODE(GENERATE_STRING)
};

// GtK Items
GtkWidget	*main_window;
GtkWidget	*fixed1;
GtkWidget	*button_exploit;
GtkWidget	*img_chooser;
GtkWidget	*sudoku_img;
GtkWidget	*menu1;
GtkWidget	*menu_new;
GtkWidget	*menu_open;
GtkWidget	*menu_save;
GtkWidget	*menu_save_as;
GtkWidget	*menu_quit;
GtkWidget	*greyscale;
GtkWidget	*Preprocessing;
GtkWidget	*thresholding;
GtkWidget	*kuwahara;
GtkWidget	*rotl;
GtkWidget	*rotr;
GtkBuilder	*builder;
GtkWidget	*hough;
GtkWidget   *auto_pwn;
//Toggle events
//int GREYSCALE = 0;
//int GAUSSIAN = 0;
//int THRESHOLDING = 0;
//int KUWAHARA = 0;

//Angle for rotation
int ACC_ANGLE = 0;
//Last file 
char *last_file = "Image/actual.png";


int initInterface(int argc,char *argv[]){
	gtk_init(&argc, &argv); // init GtK
	
	// SETUP XML.glade -> GtK window
	builder = gtk_builder_new_from_file("main.glade");
	main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	
	g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder,NULL);
	
	// Init GTK objects
	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1")); 
	button_exploit = GTK_WIDGET(gtk_builder_get_object(builder, "button_exploit")); 
	img_chooser = GTK_WIDGET(gtk_builder_get_object(builder, "img_chooser"));
	menu1 = GTK_WIDGET(gtk_builder_get_object(builder, "menu1")); 
	menu_new = GTK_WIDGET(gtk_builder_get_object(builder, "menu_new"));
	menu_open = GTK_WIDGET(gtk_builder_get_object(builder, "menu_open"));
	menu_save = GTK_WIDGET(gtk_builder_get_object(builder,"menu_save"));
	menu_save_as = GTK_WIDGET(gtk_builder_get_object(builder, "menu_save_as"));
	menu_quit = GTK_WIDGET(gtk_builder_get_object(builder, "menu_quit"));
    greyscale = GTK_WIDGET(gtk_builder_get_object(builder, "Greyscale"));
    Preprocessing = GTK_WIDGET(gtk_builder_get_object(builder, "Preprocessing"));
	thresholding = GTK_WIDGET(gtk_builder_get_object(builder,"Thresholding"));
	kuwahara = GTK_WIDGET(gtk_builder_get_object(builder, "Kuwahara"));
	hough = GTK_WIDGET(gtk_builder_get_object(builder, "hough"));
	rotr = GTK_WIDGET(gtk_builder_get_object(builder, "rot_r"));		
	rotl = GTK_WIDGET(gtk_builder_get_object(builder, "rot_l"));	
    auto_pwn =  GTK_WIDGET(gtk_builder_get_object(builder, "button_exploit"));
    gtk_widget_show(main_window);
	
	//Init Sudoku_img
	sudoku_img = NULL; 

	gtk_main();
	return EXIT_SUCCESS;
}
int main(int argc, char **argv){
	initInterface(argc,argv);
}

//reload img
void reload_img(char *path){
	int vertical = 60;
    int horizontal = 20;
	if(sudoku_img){
        	gtk_container_remove(GTK_CONTAINER(fixed1),sudoku_img); //If img already exist remove it
        	info_s("Deleting older input");
        }

	SDL_Surface *original_image = IMG_Load(path);
	SDL_Surface *image = SDL_ConvertSurfaceFormat(
	original_image, SDL_PIXELFORMAT_ARGB8888, 0);

	orderedPoints points;
	points.ul = (Point) {0, 0};
	points.ur = (Point) {image->w, 0};
	points.ll = (Point) {0, image->h};
	points.lr = (Point) {image->w, image->h};

	SDL_Surface *img = HomographicTransform(image, points, 516);
	save_image(img, "Image/resized.png");

    sudoku_img = gtk_image_new_from_file("Image/resized.png");
	gtk_container_add(GTK_CONTAINER(fixed1), sudoku_img);
    gtk_widget_show(sudoku_img);
    gtk_fixed_move(GTK_FIXED(fixed1), sudoku_img, horizontal, vertical);
    //set hte img at the right place
}

void on_button_close(){
	
	// check if the img is already loaded
	if(sudoku_img){
                gtk_container_remove(GTK_CONTAINER(fixed1),sudoku_img); //If img already exist remove it
                log_s("deleting older input");
        }
	warn_s("There isn't img yet");

}

void on_new_activated(){
	on_button_close();
}

void on_open_activated(){
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new("Choose a file",GTK_WINDOW(main_window),GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	gtk_widget_show_all(dialog);
	gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
	//If choosed a file
	if(resp==GTK_RESPONSE_OK){
		int vertical = 60;
        	int horizontal = 20;
		log_s("Working with this file as input : %s",gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
		char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        last_file = filename; 
        log_s("Last_file = %s",last_file);
        if(sudoku_img){
			gtk_container_remove(GTK_CONTAINER(fixed1),sudoku_img);
			info_s("Deleting older input");
		}


		SDL_Surface *original_image = IMG_Load(last_file);
		SDL_Surface *image = SDL_ConvertSurfaceFormat(original_image, SDL_PIXELFORMAT_ARGB8888, 0);
		
        save_image(image, "Image/actual.png");
        reload_img("Image/actual.png");
        last_file = "Image/actual.png";
        gtk_container_add(GTK_CONTAINER(fixed1), sudoku_img);
        gtk_widget_show(sudoku_img);
        gtk_fixed_move(GTK_FIXED(fixed1), sudoku_img, horizontal, vertical); //set the img at the right place
		gtk_widget_destroy(dialog);
        //last_file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        //puts(last_file);
		//SDL_Surface *image = load_image(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
		//save_image(image,"Image/gaussian.png");
    }
	//If choosed cancel
	else{
		info_s("Deleting dialogbox");
		gtk_widget_destroy(dialog);
	}
}
void on_close(){
     gtk_container_remove(GTK_CONTAINER(fixed1),sudoku_img);
     info_s("Deleting older input");
     last_file = "";
}
void on_quit_activated(){
	gtk_main_quit();
}

//greyscale
void on_greyscale_toggled(){
	log_s("Greyscale");
	puts(last_file);
    SDL_Surface *original_image = IMG_Load(last_file);
	SDL_Surface *image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
	
	//apply filter
	Apply_grayscale_filter(image);
	
	//Saves tmp + actual_img
	save_image(image,"Image/greyscale.png");
	
	reload_img("Image/greyscale.png");
    last_file = "Image/greyscale.png";
    
}
void on_gaussian_toggled(){
	log_s("Preprocessing");
        
	SDL_Surface *original_image = IMG_Load(last_file);
	SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
        
	//apply filter
	GaussianBlur_inPlace(image);
        
	//Saves tmp + actual_img
	save_image(image,"Image/gaussian.png");
    reload_img("Image/gaussian.png");

    last_file = "Image/gaussian.png";

}
//Thresholding
void on_th_toggled(){
	log_s("Thresholding");

    SDL_Surface *original_image = IMG_Load(last_file);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
        
	//apply filter
	AdaptiveThresholding_inPlace(image);
        
	//Saves tmp + set actual_img
	save_image(image,"Image/thresholding.png");
    reload_img("Image/thresholding.png");
    
    last_file = "Image/thresholding.png";

}

void on_Kuwahara_toggled(){
    log_s("Kuwahara");

    SDL_Surface *original_image = IMG_Load(last_file);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
    original_image, SDL_PIXELFORMAT_ARGB8888, 0);

    //apply filter
    KuwaharaFilter_inPlace(image);

    //Saves tmp + set actual_img
    save_image(image,"Image/Kuwahara.png");
    reload_img("Image/Kuwahara.png");

    last_file = "Image/Kuwahara.png";

}
void on_houg_toggled(){
    log_s("HoughTransform");
    
	SDL_Surface *original_image = IMG_Load(last_file);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);

    Apply_grayscale_filter(image);
    GaussianBlur_inPlace(image);
    AdaptiveThresholding_inPlace(image);
    DrawHoughlines(image, HoughTransform(image));
    
    save_image(image,"Image/Hough.png");
    reload_img("Image/Hough.png");
    last_file = "Image/Hough.png";

}
void on_rot_l_clicked(){
	log_s("Rotate Image Left");
    
	SDL_Surface *original_image = IMG_Load(last_file);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
  	ACC_ANGLE--;
	double angle = (ACC_ANGLE * 2) % 360;
	image = Rotation_shearing(image,angle);
	save_image(image,"Image/actualrot.png");
	reload_img("Image/actualrot.png");

}

void on_rot_r_clicked(){
	log_s("Rotate Image Right"); 
	SDL_Surface *original_image = IMG_Load(last_file);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    ACC_ANGLE++; 
	double angle =(ACC_ANGLE* 2) % 360 ;
	image = Rotation_shearing(image,angle);
	save_image(image,"Image/actualrot.png");
	reload_img("Image/actualrot.png");

}
void on_reset_rot(){
    ACC_ANGLE = 0;    
    reload_img(last_file);
    	
	SDL_Surface *original_image = IMG_Load("Image/actual.png");
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    save_image(image,"Image/actualrot.png");

}

void on_autosolve(){
	log_s("AutoSolve"); 
	//SDL_Surface *original_image = IMG_Load(last_file);
    //SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        //original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    
    t_options options = {
        last_file,          //inputfile
        "Image/auto.png",   //output
		NULL,
        0,
        100000,
        100,
        8225,
        0.25,
        GUI
    };

    processImage(options);
    reload_img("Image/auto.png");
    last_file = "Image/auto.png";
}
void biggest_blob(){
    
	log_s("Biggest Blob"); 
	SDL_Surface *original_image = IMG_Load(last_file);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
	BiggestBlob_result blob;
    blob = findBiggestBlob(image);
	SDL_Surface *img = blob.res;
    save_image(img,"Image/biggest_blob.png");
    reload_img("Image/biggest_blob.png");
    //last_file = "Image/biggest_blob.png";
}
void on_SmallBlob_toggled(){
    log_s("Smallest Blob");

	SDL_Surface *original_image = IMG_Load(last_file);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    removeSmallBlob(image, 50, WHITE, BLACK);
    save_image(image,"Image/SmallBlob.png");
    reload_img("Image/SmallBlob.png");
    last_file = "Image/SmallBlob.png";
}
void on_Homographic_toggled(){
    
    log_s("Homographic");

    t_options options = {
        last_file,          //inputfile
        "Image/homo.png",   //output
		NULL,
        0,
        100000,
        100,
        8225,
        0.25,
        GUI
    };
	SDL_Surface *original_image = IMG_Load(last_file);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    
	BiggestBlob_result blob;
    blob = findBiggestBlob(image);
	SDL_Surface *img = blob.res;
    orderedPoints points = findGridCorner(img, NULL, options);
    SDL_Surface *out = HomographicTransform(image, points, 252);
    
    save_image(out,"Image/Homo.png");
    reload_img("Image/Homo.png");
    last_file = "Image/Homo.png";
}
