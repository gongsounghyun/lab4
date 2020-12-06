#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#define PORTA 4000
#define MAXLINE 1000
#define NAME_LEN 20
char bufname[NAME_LEN];
char bufmsg[MAXLINE] = " ";
char bufall[MAXLINE + NAME_LEN];
int maxfdp1;
int sock;
int namelen;
char ip[] = " ";
char name[] = " ";
fd_set read_fds;
struct sockaddr_in servaddr;

typedef struct{
	GtkWidget * entry, * textview, * name;
} Widgets;
int i = 0;

void msg(char *bufmsg){
	for(i = 0; i < maxfdp1; i++){
	printf("%s\n", bufmsg);
	FD_SET(0, &read_fds);
	FD_SET(sock, &read_fds);
	if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0){
		fprintf(stderr, "select fail");
		printf("\n");
		exit(1);
	}
	if (FD_ISSET(sock, &read_fds)) {
		int nbyte;
		if ((nbyte = recv(sock, bufmsg, MAXLINE, 0)) > 0) {
			bufmsg[nbyte] = 0;
			write(1, "\033[0G", 4);
			printf("%s", bufmsg);
			fprintf(stderr, "%s>", name);
		}
	}
	if (FD_ISSET(0, &read_fds)) {
		if (fgets(bufmsg, MAXLINE, stdin)) {
			fprintf(stderr, "\033[1A");
			sprintf(bufall,"%s>%s", name, bufmsg);
			if (send(sock, bufall, strlen(bufall), 0) < 0)
				puts("Error : Write error on socket.");
		}
	}
}

}

static void insert_text(GtkButton *button, Widgets *w){
	GtkTextBuffer *buffer;
	GtkTextMark *mark;
	GtkTextIter iter;
	const gchar *text;
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(w->textview));
	text = gtk_entry_get_text (GTK_ENTRY (w->entry));
	mark = gtk_text_buffer_get_insert (buffer);
	gtk_text_buffer_get_iter_at_mark (buffer, &iter,mark);
	sprintf(bufmsg, "%s", text);
	if (FD_ISSET(sock, &read_fds)) {
		if (gtk_text_buffer_get_char_count(buffer)){
			gtk_text_buffer_insert (buffer, &iter, "\n", 1);
		}
		gtk_text_buffer_insert (buffer, &iter, text, -1);
		mark = gtk_text_buffer_get_insert (buffer);
		gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW (w->textview) , mark);
	}
	if (FD_ISSET(0, &read_fds)) {
		if (gtk_text_buffer_get_char_count(buffer)){
			gtk_text_buffer_insert (buffer, &iter, "\n", 1);
		}
		gtk_text_buffer_insert (buffer, &iter, text, -1);
		mark = gtk_text_buffer_get_insert (buffer);
		gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW (w->textview) , mark);
	}
	msg(bufmsg);
}

int main (int argc, char *argv[]){
	strcpy(name, argv[3]);
	if (argc != 4) {
		printf("help : %s <SERVER_IP> <PORT_NUM> <PORT_NAME> \n", argv[0]);
		exit(0);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port = htons(PORTA);

	if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr))< 0){
		fprintf(stderr,"connect error");
		printf("\n");
		exit(1);
	}
	puts("connect success.");
	maxfdp1 = sock + 1;
	FD_ZERO(&read_fds);

	FD_SET(0, &read_fds);
	FD_SET(sock, &read_fds);

	GtkWidget *window, *scrolled_win, *hbox, *vbox, *insert;
	Widgets *w= g_slice_new (Widgets);
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "client");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_set_size_request(window, -1, 200);
	w->textview = gtk_text_view_new ();
	w->entry = gtk_entry_new ();
	insert = gtk_button_new_with_label ("Send Text");
	g_signal_connect (insert, "clicked",G_CALLBACK (insert_text), (gpointer) w);
	scrolled_win = gtk_scrolled_window_new (NULL,NULL);
	gtk_container_add (GTK_CONTAINER (scrolled_win),w->textview);
	hbox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 5 );
	gtk_box_pack_start(GTK_BOX(hbox), w->entry, FALSE,FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox), insert, FALSE, FALSE, 5);
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

