#include <gnome.h>
void quit ( GtkWidget *window, gpointer data){
gtk_main_quit();
}
void item_selected ( GtkWidget *button, gpointer data){
	printf("파일을 프린트합니다.");
}
static GnomeUIInfo filemenu[] = { GNOMEUIINFO_MENU_NEW_ITEM ("새 파일", "Menu Hint", NULL, NULL ), GNOMEUIINFO_MENU_OPEN_ITEM (NULL, NULL), GNOMEUIINFO_MENU_SAVE_AS_ITEM (NULL, NULL), GNOMEUIINFO_SEPARATOR, GNOMEUIINFO_MENU_EXIT_ITEM (NULL, NULL), GNOMEUIINFO_END};
static GnomeUIInfo editmenu[] = { GNOMEUIINFO_MENU_FIND_ITEM (NULL, NULL), GNOMEUIINFO_END };
static GnomeUIInfo menubar[] = { GNOMEUIINFO_MENU_FILE_TREE (filemenu), GNOMEUIINFO_MENU_EDIT_TREE (editmenu), GNOMEUIINFO_END };
static GnomeUIInfo toolbar[] = { GNOMEUIINFO_ITEM_STOCK (NULL, "파일 프린트", item_selected, GNOME_STOCK_PIXMAP_PRINT), GNOMEUIINFO_ITEM_STOCK (NULL, "프로그램 종료", gtk_main_quit, GNOME_STOCK_PIXMAP_EXIT), GNOMEUIINFO_END };
int main (int argc, char *argv[]){
	GtkWidget *app;
	gnome_program_init ("gnome1", "0.1", LIBGNOMEUI_MODULE, argc, argv, GNOME_PARAM_NONE);
	app = gnome_app_new("gnome1", "GNOME 메뉴 및 툴바");
	gtk_window_set_default_size ( GTK_WINDOW(app), 300, 200);
	g_signal_connect ( app, "destroy", G_CALLBACK (quit), NULL);
	gnome_app_create_menus ( GNOME_APP(app), menubar);
	gnome_app_create_toolbar ( GNOME_APP(app), toolbar);
	gtk_widget_show(app);
	gtk_main();
	return 0;
}
