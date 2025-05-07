#include <stdlib.h>
typedef struct node{
    int ssn;
    int age;
    int DateIn;
    int TimeIn;
    int Priority;
}

int PriorityOrder(){
    FILE *file = fopen("patientTbT.csv", "r");
    if (!file) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return 1;
    }

    char line[256];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file)) {
        // Sauter l'en-tête
        if (lineNumber == 0) {
            lineNumber++;
            continue;
        }

        int sum = 0;
        int column = 0;
        char *token = strtok(line, ",");

        while (token != NULL) {
            // Symptômes commencent à la 6e colonne (index 5)
            if (column >= 5) {
                sum += atoi(token); // convertit le token en entier et l'ajoute à la somme
            }
            column++;
            token = strtok(NULL, ",");
        }

        printf("Ligne %d : Somme des symptômes = %d\n", lineNumber, sum);
        lineNumber++;
    }

    fclose(file);
    return 0;
}

int main(){
    int y;
    printf("yo testt");
    scanf("%d", &y);
    return 0;
}
