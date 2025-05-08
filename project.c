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
    char line[256]; //tableau pour stocker une ligne du fichier csv
    int lineNumber=0; //ligne du tableau lue
    while (fgets(line, sizeof(line), file)){
        if (lineNumber == 0){
            lineNumber++; //pour éviter de lire la première ligne avec les catégories
            continue; //saute le reste de la boucle
        }

        char *token=(strtok(line, ",")); //enlève les virgules de la ligne et sépare chaque catégorie

        
    }

}
