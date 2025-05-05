#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define YELLOW_P 1.0
#define RED_P 1.5
#define GREEN_P 2.0

#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_RESET   "\033[0m"

char *main_menu_t = "Select an option:\n1. Book parking spot\n2. View booking history\n3. Exit\n";
char *booking_menu_t = "----------------------------------------------\nEnter 1 to book, 2 to return to previous menu:\n";

typedef struct {
    char registration[100];
    char zone[100];
    char zonecl[100];
    int hours;
    float price;
    time_t timestamp;
} Booking;

void booking_menu(){
    Booking b;
    printf(booking_menu_t);
    char n1;
    scanf(" %c", &n1);
    while(n1 != '1' && n1 != '2'){
        printf("That option doesn't exist! Please try another input:\n");
        scanf(" %c", &n1);
    }
    if(n1 == '1'){
        Booking b;
        printf("Car registration:"); scanf(" %[^\n]%*c", b.registration);
        printf("Zone:"); scanf(" %[^\n]%*c", b.zone);
        printf("Zone color (Yellow/Red/Green):"); scanf(" %[^\n]%*c", b.zonecl);
        printf("Number of hours:"); scanf(" %d", &b.hours);
        b.timestamp = time(NULL);

        float pricemult;
        if(strcmp(b.zonecl, "Yellow") == 0){
            pricemult = YELLOW_P;
        } else if(strcmp(b.zonecl, "Red") == 0){
            pricemult = RED_P;
        } else if(strcmp(b.zonecl, "Green") == 0){
            pricemult = GREEN_P;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid zone color!\n");
            booking_menu();
            return;
        }

        b.price = b.hours * pricemult;

        FILE *fout = fopen("BookingHistory.txt", "a");
        struct tm *currentTime = localtime(&b.timestamp);
        fprintf(fout, "[%02d/%02d/%04d %02d:%02d:%02d]\t",
            currentTime->tm_mday, currentTime->tm_mon + 1, currentTime->tm_year + 1900,
            currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
        fprintf(fout, "%s - %s - %s Zone - %d Hours - %.2f RON\n", b.registration, b.zone, b.zonecl, b.hours, b.price);
        fclose(fout);
        printf("Your parking spot has been booked.\n");
        booking_menu();
        return;
    }
}

void history_menu() {
    printf("----------------------------------------------\n");
    FILE *fin = fopen("BookingHistory.txt", "r");
    if (fin == NULL) {
        printf("You have no booking history.\n");
    } else {
        char line[256];
        while (fgets(line, sizeof(line), fin) != NULL) {
            char colored_line[512];
            strcpy(colored_line, line);

            char *pos;
            if ((pos = strstr(colored_line, "Yellow")) != NULL) {
                *pos = '\0'; // Terminate string before the word
                printf("%s%sYellow%s%s", colored_line, COLOR_YELLOW, COLOR_RESET, pos + strlen("Yellow"));
            } else if ((pos = strstr(colored_line, "Red")) != NULL) {
                *pos = '\0';
                printf("%s%sRed%s%s", colored_line, COLOR_RED, COLOR_RESET, pos + strlen("Red"));
            } else if ((pos = strstr(colored_line, "Green")) != NULL) {
                *pos = '\0';
                printf("%s%sGreen%s%s", colored_line, COLOR_GREEN, COLOR_RESET, pos + strlen("Green"));
            } else {
                printf("%s", colored_line); // fallback
            }
        }
        fclose(fin);
    }
    printf("----------------------------------------------\nEnter anything to return to previous menu:");
    char dummy[10];
    scanf("%s", dummy);
    system("clear");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void main_menu(){
    printf(main_menu_t);
    char n;
    scanf("%c", &n);
    while(n != '1' && n != '2' && n != '3'){
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("That option doesn't exist! Please try another input:\n");
        scanf("%c", &n);
    }
    if(n == '1'){
        system("clear");
        booking_menu();
        main_menu();
    } else if(n == '2'){
        system("clear");
        history_menu();
        main_menu();
    } else return;

}

int main(){
    main_menu();
}