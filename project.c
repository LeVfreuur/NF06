#include <stdlib.h>
#include <stdio.h>
typedef struct Patient{
    char ssn[9];
    int age;
    char DateIn[11]; //pour un format DD/MM/YYYY
    char TimeIn[5]; //HH:MM
    int Priority;
    int Symptomes[7];
    struct Patient* next;
}Patient;

int calculatePriority(int Symptomes[]){
    int priority=3;
    if (Symptomes[1]==1||Symptomes[4]==1){
        priority=2;
    }
    if (Symptomes[6]==1||Symptomes[5]==1){
        priority=1;
    }
    return priority;
}

int  ReadPatientCSV(){
    FILE *file = fopen("patientTbT.csv", "r");
    if (file == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return 1;
    }
    

    
}
