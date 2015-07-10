#include <gtk/gtk.h>


int show_window(int argc, char *argv[] , double *maxdBm, double *mindBm){
	
	/* GtkWidget является типом хранения для виджетов */
    GtkWidget *window;
    GtkWidget *label1;
    GtkWidget *label2;
    
    /* Этот вызов используется во всех GTK программах. Аргументы из командной строки
     *   разбираются и возвращаются в приложение. */
    gtk_init (&argc, &argv);
    
    /* создается новое окно */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   
	
    void update_labeles(){
		
		char out[20];
		
		sprintf(out, "Signal level min: %.0f",*mindBm);
    
		label1 = gtk_label_new (out);
		
		sprintf(out, "Signal level max: %.0f",*maxdBm);
		
		label2 = gtk_label_new (out);
		
	}
    update_labeles();
    
    /* Устанавливаем ширину окантовки окна */
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

	
	gtk_misc_set_alignment (GTK_MISC (label1), 0.5, 0);
	
	gtk_misc_set_alignment (GTK_MISC (label2), 0.5, 50);
	
    
    gtk_widget_show (label1);
    
    gtk_widget_show (label2);
    
    /* Вывод на экран окна. */
    gtk_widget_show (window);
    
    /* Все GTK приложения содержат gtk_main(). Здесь заканчивается контроль
     * и происходит ожидание(нажатие клавиши или мышки) */
    gtk_main ();
    
    return 0;
    
}
