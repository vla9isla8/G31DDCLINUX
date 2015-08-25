#include <gtk/gtk.h>
#include <stdlib.h>
struct update_data_struct{
	GtkLabel *label1;
	GtkLabel *label2;
	double *mindBm;
	double *maxdBm;
};
gint delete_event( GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit();
    return FALSE;
}
gboolean update_data(gpointer user_data){
	
	char out[20];
	
	struct update_data_struct *data = user_data; 
	
	sprintf(out, "Signal level min: %.0f dBm",*(*data).mindBm);

	gtk_label_set_text_with_mnemonic((*data).label1,out);
	
	sprintf(out, "Signal level max: %.0f dBm",*(*data).maxdBm);
	
	gtk_label_set_text_with_mnemonic((*data).label2,out);
}

int show_window(int argc, char *argv[] , double *maxdBm, double *mindBm ){
	
	
	/* GtkWidget является типом хранения для виджетов */
    GtkWidget *window;
    
    GtkWidget *label1;
    
    GtkWidget *label2;
    
    GtkWidget *box1;
    
    /* Этот вызов используется во всех GTK программах. Аргументы из командной строки
     *   разбираются и возвращаются в приложение. */
    gtk_init (&argc, &argv);
    
    /* создается новое окно */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    gtk_window_set_title (GTK_WINDOW (window), "G31DDC");
    
    box1 = gtk_box_new (FALSE, 0);
	label1	=	gtk_label_new (NULL);
    label2	=	gtk_label_new (NULL);
	gtk_container_add (GTK_CONTAINER (window), box1);

    gtk_box_pack_start (GTK_BOX(box1), label1,  TRUE, TRUE, 20);
    
    gtk_box_pack_end (GTK_BOX(box1), label2,  TRUE, TRUE, 20);
    
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    
    struct update_data_struct *data	=	(struct update_data_struct*)malloc(sizeof(struct update_data_struct));
    (*data).label1 = (GtkLabel*)label1;
    (*data).label2 = (GtkLabel*)label2;
    (*data).mindBm = mindBm;
    (*data).maxdBm = maxdBm;
    
    gint func_ref = g_timeout_add(200, update_data,  (gpointer) data);
    
    
    //update_data(data);
    
    gtk_widget_show(label1);
    
    gtk_widget_show(label2);
    
    gtk_widget_show(box1);
    
    gtk_widget_show(window);

    gtk_main();
    
    g_source_remove (func_ref);
    
    return 0;
    
}
