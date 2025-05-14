#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct Patient{
    char ssn[9];
    char age[3];
    char DateIn[11]; //pour un format DD/MM/YYYY
    char TimeIn[5]; //HH:MM
    int Priority;
    int Symptomes[7];
    struct Patient* next;
}Patient;

int calculatePriority(int Symptomes[]){
    int priority=3;
    if (Symptomes[1]== 1||Symptomes[4]== 1){
        priority=2;
    }
    if (Symptomes[6]== 1 ||Symptomes[5]== 1 ){
        priority=1;
    }
    return priority;
}

void ajouterPatientParPriorite(Patient **head, Patient *newPatient) {
    if (*head == NULL || newPatient->Priority < (*head)->Priority) {
        // Insère en tête si la liste est vide ou si le patient est plus prioritaire que le premier
        newPatient->next = *head;
        *head = newPatient;
        return;
    }

    Patient *current = *head;

    // Cherche l'endroit où insérer : après tous les patients de priorité plus élevée ou égale
    while (current->next != NULL && current->next->Priority <= newPatient->Priority) {
        current = current->next;
    }

    // Insère le patient à cet endroit
    newPatient->next = current->next;
    current->next = newPatient;
}


int  ReadPatientCSV(){
    FILE *file = fopen("patientTbT.csv", "r");
    if (file == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return 1;
    }

    char line[256]; //tableau pour stocker une ligne du fichier csv
    int lineNumber=0; //ligne du tableau lue
    Patient *head=NULL; //pointe vers la tête de la liste chaînée


    while (fgets(line, sizeof(line), file)){  //fgets lis les lignes du csv une par une
        if (lineNumber == 0){
            lineNumber++; //pour éviter de lire la première ligne avec les catégories
            continue; //saute le reste de la boucle
        }

        Patient *newPatient = (Patient *)malloc(sizeof(Patient));
        if (!newPatient) {
            printf("Erreur d'allocation mémoire.\n");
            fclose(file);
            return 1;
        }

        int column=0;
        char *token=(strtok(line, ",")); //enlève les virgules de la ligne et met des \0 à la place. strtok renvoie un pointeur vers un morceau de la chaîne d'origine (par exemple vers le SSN). 

        /*On va se servir de strcpy, qui sert à copier chaque chaîne de caractère jusqu'à \0 ; strcpy(destination, pointeur argument)*/

        while (token != NULL) {
            if (column==0) strcpy(newPatient->ssn, token);

            else if(column==1) strcpy(newPatient->age, token);

            else if (column == 3) strcpy(newPatient->DateIn, token); 

            else if (column == 4) strcpy(newPatient->TimeIn, token); 

            else if(column>4){
                // Symptômes (colonnes 6 à 10)
                newPatient->Symptomes[column - 5] = atoi(token);  // Convertir les chaînes en int (0 ou 1)
            }
            token = strtok(NULL, ",");
            column++;
        }

        newPatient->Priority = calculatePriority(newPatient->Symptomes) ;
        newPatient->next = NULL;

        //Ici, il faudra ajouter la fonction qui ajoute le patient à la liste chainée en fonction de sa priorité

        lineNumber++;
        }
    fclose(file);
}

int main(){
    return 0;
    ReadPatientCSV();
}

