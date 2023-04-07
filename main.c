#include <gtk/gtk.h>
#include <curl/curl.h>

void my_curl(GtkWidget *widget, gpointer data) {
    CURL *curl;
    CURLcode res;
    FILE *file;
    const char *url = gtk_entry_get_text(GTK_ENTRY(data));

    // init libcurl
    curl = curl_easy_init();
    if (curl) {
        // open file
        file = fopen("index.html", "w");

        // set URL for GET request
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // set file for write data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // performs HTTP GET request
        res = curl_easy_perform(curl);

        // check if the request was successful
        if (res != CURLE_OK) {
            // Shows a popup with the error message
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error during downloading:\n%s", curl_easy_strerror(res));
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        } else {
            // Show a popup with the success message
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Complete download!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }

        // close the connection
        curl_easy_cleanup(curl);

        // close file
        fclose(file);
    }
}

int main(int argc, char **argv) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *button;

    // init GTK
    gtk_init(&argc, &argv);

    // create the window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Curl with GUI");
    
    // set the window as non-resizable
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // connects the management of the window closing event
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    // create a grid container to organize items
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // create a label for the text box
    label = gtk_label_new("URL:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // create a text box for URL input
    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 1, 0, 1, 1);

    // create a button to execute HTTP GET request
    button = gtk_button_new_with_label("Start request");
    g_signal_connect(button, "clicked", G_CALLBACK(my_curl), entry);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

    // show window
    gtk_widget_show_all(window);

    // starts the main event cycle
    gtk_main();

    return 0;
}
