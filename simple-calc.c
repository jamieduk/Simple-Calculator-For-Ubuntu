#include <gtk/gtk.h>

// Function prototypes
void on_button_click(GtkWidget *widget, gpointer data);
void calculate_result();
void show_author_info();
void copy_to_clipboard(GtkWidget *widget);

// Global variables
GtkWidget *entry_display;
gchar operation;
gdouble first_number=0;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create main window
    GtkWidget *window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);
    
    // Create a grid layout
    GtkWidget *grid=gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create entry display (non-editable)
    entry_display=gtk_entry_new();
    gtk_widget_set_sensitive(entry_display, FALSE); // Make it non-editable
    gtk_grid_attach(GTK_GRID(grid), entry_display, 0, 0, 4, 1);

    // Define button labels
    const gchar *button_labels[]={
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", "C", "=", "+",
        "Copy", "About" // Add Copy button and About button
    };

    // Create buttons and attach them to the grid
    int num_buttons=sizeof(button_labels) / sizeof(button_labels[0]); // Calculate the number of buttons
    for (int i=0; i < num_buttons; i++) { // Loop through all buttons
        GtkWidget *button=gtk_button_new_with_label(button_labels[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(on_button_click), (gpointer)button_labels[i]);
        
        // Set uniform size for all buttons
        gtk_widget_set_size_request(button, 60, 60);
        
        gtk_grid_attach(GTK_GRID(grid), button, i % 4, (i / 4) + 1, 1, 1);
    }

    // Connect the destroy signal for the window
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets in the window
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}

// Callback function for button clicks
void on_button_click(GtkWidget *widget, gpointer data) {
    const gchar *label=(const gchar *)data;

    if (g_strcmp0(label, "C") == 0) {
        // Clear the entry
        gtk_entry_set_text(GTK_ENTRY(entry_display), "");
        first_number=0;
        operation='\0';
    } else if (g_strcmp0(label, "=") == 0) {
        calculate_result();
    } else if (g_strcmp0(label, "Copy") == 0) {
        copy_to_clipboard(widget);
    } else if (g_strcmp0(label, "About") == 0) {
        show_author_info();
    } else {
        // Append button label to the entry display
        gchar *current_text=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_display)));
        
        // If the label is an operation, set the first number and operation
        if (g_strcmp0(label, "+") == 0 || g_strcmp0(label, "-") == 0 ||
            g_strcmp0(label, "*") == 0 || g_strcmp0(label, "/") == 0) {
            first_number=atof(current_text);
            operation=label[0]; // Store the operation character
            gtk_entry_set_text(GTK_ENTRY(entry_display), "");
        } else {
            gchar *new_text=g_strjoin("", current_text, label, NULL);
            gtk_entry_set_text(GTK_ENTRY(entry_display), new_text);
            g_free(new_text);
        }
        g_free(current_text);
    }
}

// Function to calculate the result
void calculate_result() {
    gchar *current_text=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_display)));
    gdouble second_number=atof(current_text);
    gdouble result=0;

    switch (operation) {
        case '+':
            result=first_number + second_number;
            break;
        case '-':
            result=first_number - second_number;
            break;
        case '*':
            result=first_number * second_number;
            break;
        case '/':
            if (second_number != 0) {
                result=first_number / second_number;
            } else {
                gtk_entry_set_text(GTK_ENTRY(entry_display), "Error");
                g_free(current_text);
                return;
            }
            break;
        default:
            result=second_number; // If no operation is set, just return the second number
            break;
    }

    // Update entry display with the result
    gchar *result_text=g_strdup_printf("%.2f", result);
    gtk_entry_set_text(GTK_ENTRY(entry_display), result_text);
    g_free(result_text);
    g_free(current_text);
}

// Function to copy result to clipboard
void copy_to_clipboard(GtkWidget *widget) {
    GtkClipboard *clipboard=gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    const gchar *text=gtk_entry_get_text(GTK_ENTRY(entry_display));
    gtk_clipboard_set_text(clipboard, text, -1);
}

// Function to show author information
void show_author_info() {
    GtkWidget *dialog=gtk_message_dialog_new(NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Author: Jay Mee @ J~Net 2024");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

