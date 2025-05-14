#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct Patient{
    char ssn[10];
    char DoB[11];
    int age;
    char DateIn[11]; //pour un format DD/MM/YYYY
    char TimeIn[6]; //HH:MM
    int Priority;
    int Symptomes[7];
    int Postal[5];
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



Patient*  ReadPatientCSV(){
    FILE *file = fopen("patientTbT.csv", "r");
    if (file == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return NULL;
    }

    char line[256]; //tableau pour stocker une ligne du fichier csv
    int lineNumber=0; //ligne du tableau lue
    Patient *head=NULL; //pointe vers la tête de la liste chaînée
    Patient *end=NULL; //fin de la liste

    int i;

    while (fgets(line, sizeof(line), file)){  //fgets lis les lignes du csv une par une
        if (lineNumber == 0){
            lineNumber++; //pour éviter de lire la première ligne avec les catégories
            continue; //saute le reste de la boucle
        }

        Patient *newPatient = (Patient *)malloc(sizeof(Patient));
        if (!newPatient) {
            printf("Erreur d'allocation mémoire.\n");
            fclose(file);
            return NULL;
        }

        for (i=0;i<=8;i++){
            newPatient->ssn[i]=line[i];
            newPatient->ssn[8]='\0';
        }
        for (i=10;i<=19;i++){
            newPatient->DoB[i-10]=line[i];
            newPatient->DoB[10]='\0';

        }
        for (i=21;i<=25;i++){
            newPatient->Postal[i-21]=line[i]-'0';
        }
        for (i= 27; i <= 36; i++){
            newPatient->DateIn[i-27]=line[i];
            newPatient->DateIn[10]='\0';
        }
        for (i = 38; i <=42; i++){
            newPatient->TimeIn[i-38]=line[i];
            newPatient->TimeIn[5]='\0';
        }
        for (i = 44; i <=56;i++ ){
            if (i%2==0){
                newPatient->Symptomes[(i-44)/2]=line[i]-'0';
            }
        }
        
        
        newPatient->Priority = calculatePriority(newPatient->Symptomes) ;
        newPatient->next = NULL;
        
        if (head==NULL){ //Si la liste est vide, donc pour la première itération
            head=newPatient;
            head->next=NULL;
            end=head;
        }
        
        //placement dans l'ordre par rapport à son priority
        else {
    if (newPatient->Priority == 1) {
        newPatient->next = head;
        head = newPatient;
    }

    else if (newPatient->Priority == 2) {
        if (head->Priority >= 2) {
            newPatient->next = head;
            head = newPatient;
        } else {
            Patient *current = head;
            while (current->next != NULL && current->next->Priority == 1) {
                current = current->next;
            }
            newPatient->next = current->next;
            current->next = newPatient;
            
            if (newPatient->next == NULL) {
                end = newPatient;
            }
        }
    }

    else {  // Priority == 3
        end->next = newPatient;
        end = newPatient;
    }
}


        //Ici, il faudra ajouter la fonction qui ajoute le patient à la liste chainée en fonction de sa priorité

        lineNumber++;
    }
    fclose(file);
    return head;
}

int main() {
    Patient* list = ReadPatientCSV();
    if (list == NULL) {
        printf("La liste est vide ou une erreur est survenue lors de la lecture.\n");
        return 1;
    }

    printf("Liste des patients triés par priorité :\n");
    printf("--------------------------------------------------------------------------\n");

    int i;
    Patient* current = list;
    while (current != NULL) {
        printf("%s %s %s %d %s ", (current->ssn), (current->DateIn), (current->DoB), (current->Priority), (current->TimeIn));
        for (i=0;i<=4;i++){
            printf("%d",current->Postal[i]);
        }
        printf(" ");
        for (i=0;i<=6;i++){
            printf("%d",current->Symptomes[i]);
        }
        printf("\n");
        current = current->next;
    }

    return 0; //brrrrrr
}







