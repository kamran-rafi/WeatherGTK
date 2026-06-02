#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <gtk/gtk.h>
#include <stdbool.h>

int app(int argc, char *argv[]);

/* 
 * Instead of defining global variables different files for 
 * managing global states, bundle them here and use where-ever
 * is needed.
*/
typedef struct {
    GtkWidget* page_container; // Used in page.c
    GtkWidget* current_page; // Used in page.c
    GtkWidget* prev_focused_button; // Used in navbar.c
    bool is_loading; // Used to track if something is loading in backend.
} AppState;

extern AppState APP_STATE;

#endif