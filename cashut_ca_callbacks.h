#ifndef CASHUT_CA_CALLBACKS_H
#define CASHUT_CA_CALLBACKS_H

#include <gtk/gtk.h>

// Calculatrice

G_MODULE_EXPORT void ca_append(char *string, gpointer label);
G_MODULE_EXPORT void ca_append_0(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_1(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_2(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_3(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_4(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_5(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_6(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_7(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_8(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_9(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_append_decimal(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_divide(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_multiply(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_subtract(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_add(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_percent(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_clear(GtkWidget *widget, gpointer label);
G_MODULE_EXPORT void ca_equals(GtkWidget *widget, gpointer label);


#endif /* CASHUT_CA_CALLBACKS_H */
