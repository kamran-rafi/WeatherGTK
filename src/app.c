/*
 * The Application is launched from this file.
 * All the config and setup happen here.
*/
#include <gtk/gtk.h>
#include "../application.h"
#include "glib.h"
#include "utils/logger.h"
#include "ui/ui.h"
#include "app.h"

// Initializing global APP_STATE struct will all properties set to NULL.
AppState APP_STATE = {0};

// An event handler for closing application.
void destroy(GtkWindow* window, gpointer data){
    log_info("Destroying window.");
    gtk_window_close(window);
}

// TODO:: Refactor it out once I add Settings page for dynamic theme changing.
void setup_theme(){
    char* path = "./src/styles/night.css";
    GtkCssProvider* provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, path);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}
////////////////////

// Creates a main app window and all the widgets will go inside this window.
void launch(GtkApplication* app){
    // Creating a main window which contains our widgets for interaction.
    GtkWidget* window = gtk_application_window_new(app);
    // Setting some default styling for window.
    gtk_window_set_title(GTK_WINDOW(window), APP_NAME);
    gtk_window_set_default_size(GTK_WINDOW(window), APP_MIN_WIDTH, APP_MIN_HEIGHT);
    setup_theme();
    //////////////////////////////////////////////////////

    // Attach window close handler callback
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

    // Root container every other widget goes inside of it.
    GtkWidget* rootContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, SPACE);
    gtk_widget_add_css_class(rootContainer, "root-container");
    gtk_widget_set_hexpand(rootContainer, TRUE);
    gtk_widget_set_vexpand(rootContainer, TRUE);

    gtk_box_append(GTK_BOX(rootContainer), navbar());
    gtk_box_append(GTK_BOX(rootContainer), render_page());
    ////////////////////////////////////////////////////////////////

    // Set root container a child of window.
    gtk_window_set_child(GTK_WINDOW(window), rootContainer);
    // Display the window.
    gtk_window_present(GTK_WINDOW(window));
}

// The app starts here and after exiting returns a status code to main().
int app(int argc, char* argv[]){
    GtkApplication* app = gtk_application_new(APP_ID, G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(launch), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}