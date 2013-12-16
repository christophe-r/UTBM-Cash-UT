#include <string.h>
#include <stdlib.h>

#include <glib.h>

#include "cashut_ca_callbacks.h"


//extern GtkBuilder *builder_cashut;


/**** Calculatrice **********************************/

char buffer[512] = "0";
char buffer_screen[512] = "0";

float num1, num2, new_num;
char operator = 0;

void ca_append(char *string, gpointer label) {
	strcat(buffer, string);
	strcat(buffer_screen, string);

	sprintf(buffer_screen, "%.2f", atof(buffer));
	
	gtk_label_set_text(label, buffer_screen);
} 

void ca_append_0(GtkWidget *widget, gpointer label) {
	ca_append("0", label);
}

void ca_append_1(GtkWidget *widget, gpointer label) {
	ca_append("1", label);
}

void ca_append_2(GtkWidget *widget, gpointer label) {
	ca_append("2", label);
}

void ca_append_3(GtkWidget *widget, gpointer label) {
	ca_append("3", label);
}

void ca_append_4(GtkWidget *widget, gpointer label) {
	ca_append("4", label);
}

void ca_append_5(GtkWidget *widget, gpointer label) {
	ca_append("5", label);
}

void ca_append_6(GtkWidget *widget, gpointer label) {
	ca_append("6", label);
}

void ca_append_7(GtkWidget *widget, gpointer label) {
	ca_append("7", label);
}

void ca_append_8(GtkWidget *widget, gpointer label) {
	ca_append("8", label);
}

void ca_append_9(GtkWidget *widget, gpointer label) {
	ca_append("9", label);
}

void ca_append_decimal(GtkWidget *widget, gpointer label) {
	ca_append(".", label);
}

void ca_divide(GtkWidget *widget, gpointer label) {
	if (!operator) {
		num1 = atof(buffer);
		strcpy(buffer, "");
		strcpy(buffer_screen, "");
		operator = '/';
	}
}

void ca_multiply(GtkWidget *widget, gpointer label) {
	if (!operator) {
		num1 = atof(buffer);
		strcpy(buffer, "");
		strcpy(buffer_screen, "");
		operator = '*';
	}
}

void ca_subtract(GtkWidget *widget, gpointer label) {
	if (!operator) {
		num1 = atof(buffer);
		strcpy(buffer, "");
		strcpy(buffer_screen, "");
		operator = '-';
	}
}

void ca_add(GtkWidget *widget, gpointer label) {
	if (!operator) {
		num1 = atof(buffer);
		strcpy(buffer, "");
		strcpy(buffer_screen, "");
		operator = '+';
	}
}

void ca_percent(GtkWidget *widget, gpointer label) {
		strcat(buffer_screen, "%");
		gtk_label_set_text(label, buffer_screen);
}

void ca_clear(GtkWidget *widget, gpointer label) {
	strcpy(buffer, "0");
	strcpy(buffer_screen, "0.00");
	gtk_label_set_text(label, buffer_screen);
}


void ca_equals(GtkWidget *widget, gpointer label) {
	

	if (operator) {
		num2 = atof(buffer);
		
		char last_char_ascii;
		last_char_ascii = buffer_screen[(strlen(buffer_screen)-1)];


		switch (operator) {
			case '+':
				
				if( (int)last_char_ascii == 37 ){ // Si le dernier char est un "%"
					new_num = num1 * (1 + num2 / 100);
				} else {
					new_num = num1 + num2;
				}

				break;
			case '-':

				if( (int)last_char_ascii == 37 ){ // Si le dernier char est un "%"
					new_num = num1 * (1 - num2 / 100);
				} else {
					new_num = num1 - num2;
				}

				break;
			case '*':
				new_num = num1 * num2;
				break;
			case '/':
				new_num = num1 / num2;
				break;
		}
		
		operator = 0;
	
		sprintf(buffer, "%f", new_num);
		sprintf(buffer_screen, "%.2f", new_num);
		
		
		//fprintf(stdout, "%f ---- %f ---- %f\n", num1, num2, new_num);


		gtk_label_set_text(label, buffer_screen);
	}
}



