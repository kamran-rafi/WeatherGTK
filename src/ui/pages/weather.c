/*
 * Handle's all the GUI for Showing weather data inside this page.
*/

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../components/components.h"
#include "../../utils/logger.h"
#include "glibconfig.h"
#include "gtk/gtkshortcut.h"
#include "../ui.h"
#include "../components/components.h"
#include "../../models/weather.h"
#include "../../core/core.h"
#include "../../utils/formatter.h"
#include "../../app.h"
#include "../../utils/network.h"
#include "../../utils/storage.h"

GtkWidget* root_page;
GtkWidget* weather_container;

/*
 * Before you laugh at me for having API_KEY directly inside the source code and pushed publicly,
 * You must know that it is free tier and you can use it to test or generate your own api key for free.
*/

const char* API_URL = "https://api.weatherapi.com/v1/forecast.json?key=417f11a692f7406d85b150359231008&days=14&q=";

/*
 * Render current location and temperature.
*/
GtkWidget* current_location_temperature(WeatherModel* weatherData){
    log_debug("Rendering current location and temperature...");

    GtkWidget* root = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, SPACE);
    gtk_widget_set_hexpand(root, TRUE);

    // Location and Temperature are shown in vertical widgets on left side.
    GtkWidget* locationAndTempratureContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // Location
    char* formattedLocation = get_formatted_location(weatherData->location);
    GtkWidget* location = gtk_label_new(formattedLocation);
    gtk_widget_add_css_class(location, "current-location");
    free(formattedLocation); // Clear memory.
    /////////////
    // Temperature
    char* formattedTemperature = get_formatted_temperature(weatherData->current.temperature);
    GtkWidget* temperature = gtk_label_new(formattedTemperature);
    gtk_widget_add_css_class(temperature, "current-temperature");
    gtk_label_set_xalign(GTK_LABEL(temperature), 0.0);
    gtk_box_append(GTK_BOX(locationAndTempratureContainer), location);
    gtk_box_append(GTK_BOX(locationAndTempratureContainer), temperature);
    free(formattedTemperature); // Clear memory.
    // Icon is shown on right side.
    GtkWidget* icon = get_icon("/home/kamran/Projects/GeoGTK/src/assets/placeholder.svg", 88);
    gtk_widget_set_halign(icon, GTK_ALIGN_END);
    gtk_widget_set_hexpand(icon, TRUE);
    gtk_widget_add_css_class(icon, "current-weather-icon");

    gtk_box_append(GTK_BOX(root), locationAndTempratureContainer);
    gtk_box_append(GTK_BOX(root), icon);

    return root;
}

/*
 * Creates and returns a weather condition Item to be shown inside the current_condtion() container.
*/
GtkWidget* current_condition_item(const char* title,const char* text){
    GtkWidget* root = gtk_box_new(GTK_ORIENTATION_VERTICAL, SPACE/2 /* Half space compared to make spacing. */);
    gtk_widget_add_css_class(root, "current-condition-item");
    gtk_widget_set_hexpand(root, TRUE);

    GtkWidget* icon = get_icon("/home/kamran/Projects/GeoGTK/src/assets/placeholder.svg", 42);
    gtk_widget_add_css_class(icon, "current-condition-item-icon");

    GtkWidget* type = gtk_label_new(title);
    gtk_widget_add_css_class(type, "current-condition-item-type");

    GtkWidget* data = gtk_label_new(text);
    gtk_widget_add_css_class(data, "current-condition-item-data");

    gtk_box_append(GTK_BOX(root), icon);
    gtk_box_append(GTK_BOX(root), type);
    gtk_box_append(GTK_BOX(root), data);

    return root;
}

/*
 * Renders widgets inside a container to show current weather condition.
*/
GtkWidget* current_condition(WeatherModel* weatherData){

    log_debug("Rendering current condition...");

    GtkWidget* root = gtk_grid_new();
    gtk_widget_set_hexpand(root, TRUE);
    gtk_widget_add_css_class(root, "current-condition-container");

    // Grid, Child, Col, Row, Col-Span, Row-Span
    gtk_grid_attach(GTK_GRID(root), current_condition_item("Wind", "7.6 KM/h"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(root), current_condition_item("Humidity", "48%"), 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(root), current_condition_item("Visibility", "5.0 KM"), 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(root), current_condition_item("UV Index", "0.0"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(root), current_condition_item("Precipitation", "0.0 in"), 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(root), current_condition_item("Clouds", "0"), 2, 1, 1, 1);

    return root;
}

GtkWidget* hourly_forecast(){

    log_debug("Rending hourly forecast...");

    GtkWidget* root = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, SPACE);
    gtk_widget_set_hexpand(root, TRUE);
    gtk_widget_set_vexpand(root, TRUE);
    gtk_widget_add_css_class(root, "hourly-forecast-container");

    return root;
}

GtkWidget* display_weather(WeatherModel* weatherData){
    GtkWidget* weather_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, SPACE);
    gtk_widget_set_hexpand(weather_container, TRUE);

    // Weather data is devided into left and right sections.

    GtkWidget* left = gtk_box_new(GTK_ORIENTATION_VERTICAL, SPACE);
    gtk_widget_set_hexpand(left, TRUE);
    gtk_box_append(GTK_BOX(left), current_location_temperature(weatherData));
    gtk_box_append(GTK_BOX(left), current_condition(weatherData));
    gtk_box_append(GTK_BOX(left), hourly_forecast());

    GtkWidget* right = gtk_box_new(GTK_ORIENTATION_VERTICAL, SPACE);
    gtk_widget_set_hexpand(right, TRUE);
    gtk_widget_add_css_class(right, "right");

    
    gtk_box_append(GTK_BOX(weather_container), left);
    gtk_box_append(GTK_BOX(weather_container), right);

    return weather_container;
}

void load_page(){

    if(root_page && weather_container)
        gtk_box_remove(GTK_BOX(root_page), weather_container);

    WeatherModel* weatherData = get_weather_data();

    if(!weatherData){
        // Todo: Show a widget to ask user to search weather data.
        log_debug("No Weather Data found.");
    }
    else{
        weather_container = display_weather(weatherData);
        gtk_box_append(GTK_BOX(root_page), weather_container);
    }

    clear_weather_model(weatherData);
}

/*
 * [TODO] This callback function is used to search for weather location.
*/
void search_location(GtkWidget* widget, gpointer input){
    GtkEntryBuffer* buffer = gtk_entry_get_buffer(input);
    const char* location =  gtk_entry_buffer_get_text(buffer);
    log_info("Searching Location: %s", location);

    char* finalUrl = concat_strings(API_URL, location);

    // To make sure multiple searches are not made.
    if(!APP_STATE.is_loading){
        APP_STATE.is_loading = true;
        log_debug("Searching Weather Data");
        
        log_info(finalUrl);
        
        NetworkBuffer* buffer = fetch_api_data(finalUrl);

        write_content("weather.json", buffer->data);

        clear_network_buffer(buffer);
        
        APP_STATE.is_loading = false;

        load_page();
    }
    else{
        log_error("Previous weather search not finished yet.");
    }

    free(finalUrl);
}

/*
 * Todos: 
 * 1- [DONE] Add a Location Search Input.
 * 2- [DONE] Add Current Weather Temperature and Location Info.
 * 3- [DONE] Add current Weather Condition Container.
 * 4- [PENDING] Add Hourly Weather Forcast Container. 
 * 5- [PENDING] Add Daily Weather Forcast Container.
*/
GtkWidget* weather_page(){
    root_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, SPACE);

    // Appended input widget to search for location.
    gtk_box_append(GTK_BOX(root_page), read_input("Search location...", "Search", search_location));

    load_page();

    return root_page;
}