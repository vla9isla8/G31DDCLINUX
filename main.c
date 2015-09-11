#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <curses.h>         	// requires curses library
#include "g31ddcapi.h"
#include <time.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
struct g31ddc_devicelist_data{
	G31DDC_DEVICE_INFO *List;
	int Count;
};
gboolean update_data(gpointer user_data){	
	char out[20];
	
	//struct update_data_struct* data = (struct update_data_struct*)user_data; 
	
	//long int sec = *data->num/1000,ms = *data->num%1000;
	
	//sprintf(out, "Time: %2ld sec %3ld ms",sec,ms);

	//gtk_label_set_text(data->label1,out);
}

static void activate (GtkApplication *app, gpointer user_data){
	GtkWidget *window;
	GtkWidget *label_box;
	GtkWidget *label1;
	GtkWidget *comboBox1;
	GtkWidget *combo_box;
	GtkWidget *container;
	struct g31ddc_devicelist_data *deviceList = (struct g31ddc_devicelist_data*)user_data;
	int i=0;
	
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Window");
	gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
	container 	= 	gtk_box_new (FALSE,0);
	label_box 	= 	gtk_box_new (FALSE,0);
	label1		=	gtk_label_new (NULL);
	char *out;
	if(deviceList->List!= NULL && deviceList->Count > 0){
		comboBox1	=	gtk_combo_box_text_new();
		for(i=0; i < deviceList->Count;i++){
			char *SN =deviceList->List[i].SerialNumber;
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBox1),SN);
		}
		out = malloc(20*sizeof(char));
		sprintf(out,"Found %d devices",deviceList->Count);
		
		combo_box 	= gtk_box_new (FALSE,0);
		gtk_container_add(GTK_CONTAINER(combo_box), comboBox1);
		gtk_container_add(GTK_CONTAINER(container), combo_box);
		
	}else{
		out =	"Devices not found";
	}
	gtk_label_set_text(GTK_LABEL(label1),out);
	gtk_container_add (GTK_CONTAINER(label_box), label1);
	gtk_container_add (GTK_CONTAINER(container), label_box);
	gtk_container_add (GTK_CONTAINER (window), container);
	gtk_widget_show_all (window);
}

int main(int argc, char **argv){
	int status;
	if(!radio_load_api()){
		printf("api loaded with code \"%d\"\n",status);
		return 0;
	}
	G31DDC_DEVICE_INFO *deviceList;
	int deviceCount;
	getDeviceList(deviceList,&deviceCount);
	
	struct g31ddc_devicelist_data *dl = (struct g31ddc_devicelist_data*)malloc(sizeof(struct g31ddc_devicelist_data));
	dl->List = deviceList;
	dl->Count= deviceCount;
	GtkApplication *app;
	
	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	
	g_signal_connect (app, "activate", G_CALLBACK (activate), dl);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;

}
