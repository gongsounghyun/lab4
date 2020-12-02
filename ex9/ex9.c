#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int one[5] = {0,};
int i = 0;

const char * password = "hidden";
void quit ( GtkWidget *window, gpointer data){
	gtk_main_quit();
}
void input_one(){ one[i]=1; i++;}
void input_two(){ one[i]=2; i++;}
void input_three(){ one[i]=3; i++;}
void input_four(){ one[i]=4; i++;}
void input_five(){ one[i]=5; i++;}
void input_six(){ one[i]=6; i++;}
void input_seven(){ one[i]=7; i++;}
void input_eight(){ one[i]=8; i++;}
void input_nine(){ one[i]=9; i++;}
void input_zero(){ one[i]=0; i++;}
void clear (){
	for(int j=0;j<5;j++){one[j] = 0;}
	i=0;
}
void input_plus (GtkWidget *entry, gpointer data){
	const char *text = gtk_entry_get_text(GTK_ENTRY( (GtkWidget *) entry));
	char result[44];
	int value;
	for(i=0;i<5;i++){
		printf("%d\n", one[i]);
		value+=one[i];
	}
	sprintf(result, "%d", value);
	gtk_label_set_text (GTK_LABEL( (GtkWidget *) data),result);
}
void input_minus (GtkWidget *entry, gpointer data){
	const char *text = gtk_entry_get_text(GTK_ENTRY( (GtkWidget *) entry));
	char result[44];
	int value;
	for(i=0;i<5;i++){
		printf("%d\n", one[i]);
		value-=one[i];
	}
	sprintf(result, "%d", value);
	gtk_label_set_text (GTK_LABEL( (GtkWidget *) data),result);

}
void input_gap (GtkWidget *entry, gpointer data){
	const char *text = gtk_entry_get_text(GTK_ENTRY( (GtkWidget *) entry));
	char result[44];
	int value;
	for(i=0;i<5;i++){
		printf("%d\n", one[i]);
		if(one[i]==0)
			continue;
		else
			value*=one[i];
	}
	sprintf(result, "%d", value);
	gtk_label_set_text (GTK_LABEL( (GtkWidget *) data),result);
}
void input_nanugi (GtkWidget *entry, gpointer data){
	const char *text = gtk_entry_get_text(GTK_ENTRY( (GtkWidget *) entry));
	char result[44];
	float value;
	for(i=0;i<5;i++){
		printf("%d\n", one[i]);
		value/=one[i];
	}
	sprintf(result, "%.1f", value);
	gtk_label_set_text (GTK_LABEL( (GtkWidget *) data),result);
}

int main (int argc, char *argv[]){
	GtkWidget *window;
	GtkWidget *message_label;
	GtkWidget *result;
	GtkWidget *Plus, *Minus, *Gap, *Nanugi;
	GtkWidget *first_hbox, *second_hbox;
	GtkWidget *one, *two, *three, *four, *five, *six, *seven, *eight, *nine, *zero;	
	GtkWidget *vbox, *inputbox;
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "사용자 입력창" );
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 400);
	g_signal_connect (window, "destroy", G_CALLBACK (quit), NULL);
	message_label = gtk_label_new("이름과 패스워드를 입력하세요.");
	
	one = gtk_button_new_with_label("1");
	two = gtk_button_new_with_label("2");
	three = gtk_button_new_with_label("3");
	four = gtk_button_new_with_label("4");
	five = gtk_button_new_with_label("5");
	six = gtk_button_new_with_label("6");
	seven = gtk_button_new_with_label("7");
	eight = gtk_button_new_with_label("8");
	nine = gtk_button_new_with_label("9");
	zero = gtk_button_new_with_label("0");
	
	Plus = gtk_button_new_with_label("+");
	Minus = gtk_button_new_with_label("-");
	Gap = gtk_button_new_with_label("*");
	Nanugi = gtk_button_new_with_label("/");
	result = gtk_button_new_with_label("second");
	
	first_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5 );
	second_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5 );
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
	gtk_box_pack_start(GTK_BOX(vbox), message_label, FALSE, FALSE, 10);

	gtk_box_pack_start(GTK_BOX(vbox), one, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), two, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), three, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), four, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), five, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), six, TRUE, FALSE, 0);	
	gtk_box_pack_start(GTK_BOX(vbox), seven, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), eight, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), nine, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), zero, TRUE, FALSE, 0);
	
	g_signal_connect (one, "clicked", G_CALLBACK(input_one), message_label);
	g_signal_connect (two, "clicked", G_CALLBACK(input_two), message_label);
	g_signal_connect (three, "clicked", G_CALLBACK(input_three), message_label);
	g_signal_connect (four, "clicked", G_CALLBACK(input_four), message_label);
	g_signal_connect (five, "clicked", G_CALLBACK(input_five), message_label);
	g_signal_connect (six, "clicked", G_CALLBACK(input_six), message_label);
	g_signal_connect (seven, "clicked", G_CALLBACK(input_seven), message_label);
	g_signal_connect (eight, "clicked", G_CALLBACK(input_eight), message_label);
	g_signal_connect (nine, "clicked", G_CALLBACK(input_nine), message_label);
	g_signal_connect (zero, "clicked", G_CALLBACK(input_zero), message_label);
	
	inputbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(inputbox), vbox, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(inputbox), Plus, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(inputbox), Minus, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(inputbox), Gap, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(inputbox), Nanugi, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(inputbox), result, TRUE, FALSE, 5);
	
	g_signal_connect (result, "clicked", G_CALLBACK(clear), message_label);
	g_signal_connect (Plus, "clicked", G_CALLBACK(input_plus), message_label);
	g_signal_connect (Minus, "clicked", G_CALLBACK(input_minus), message_label);
	g_signal_connect (Gap, "clicked", G_CALLBACK(input_gap), message_label);
	g_signal_connect (Nanugi, "clicked", G_CALLBACK(input_nanugi), message_label);
	
	gtk_container_add(GTK_CONTAINER(window), inputbox);
	gtk_widget_show_all(window);
	gtk_main ();
	return 0;
}
