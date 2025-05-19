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

float TimeConvertion(char time[6]){
    char hourCara[3];
    char minCara[3];

    hourCara[0]=time[0];
    hourCara[1]=time[1];

    minCara[0]=time[3];
    minCara[1]=time[4];

    hourCara[2]='\0';
    minCara[2]='\0';

    float hourFloat=atoi(hourCara);
    float minFloat=atoi(minCara);

    float FinalTime=hourFloat + minFloat/60.0;

    return FinalTime;
}

int PriorityEscalationSystem(int level,char PassageTime[6], char ActualTime[6]){
       float PassageTimeDecimal=TimeConvertion(PassageTime);
       float ActualTimeDecimal=TimeConvertion(ActualTime);

       if (level==1){
        return 1;
       }

       if (level==2){
            if (ActualTimeDecimal-PassageTimeDecimal>=2){
                return 1;
            }
            else{
                return 2;
            }
       }

       else{
            if (ActualTimeDecimal-PassageTimeDecimal>=3){
                return 2;
            }
            else{
                return 3;
            }
       }
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

/*
int main(){
    FILE* fp=fopen("patientTbT.csv","r");
    char line[256];
    if (fp==NULL){
        printf("Error");
    }

   while (fgets(line,sizeof(line),fp) != NULL){
    printf("%s",line);
   }

   fclose(fp);
    return 0;
    }
*/

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



int DoBToAge(char dob[10]){
    char *year = dob+6;
    int bornyear = atoi(year);
    int age = 2025-bornyear;
    return age;
}



*Patient SearchBySSN(*head, char searchedssn[10]){
    Patient *curr = head;
    while (curr!=NULL || *curr->ssn==searchedssn){
        curr=curr->next;
    }
    if (curr!=NULL){
        printf("%s", curr->ssn);
        printf("%d", curr->age);
        printf("%s", curr->DateIn);
        printf("%s", curr->TimeIn);
        printf("%d", curr->Priority);
        printf("%d", curr->Postal);
    }
    return curr;
}

//Generating reports

#include <time.h>

// Définition de la structure DateHeure
typedef struct {
    int jour;
    int mois;
    int annee;
    int heure;
    int minute;
    int seconde;
} DateHour;

// Fonction qui retourne la date et l'heure actuelles
DateHour ExactDate() {
    time_t now;
    struct tm *local;
    DateHour dh;

    time(&now);               // Obtenir le temps actuel
    local = localtime(&now);  // Convertir en temps local

    dh.jour = local->tm_mday;
    dh.mois = local->tm_mon + 1;       // Les mois vont de 0 à 11
    dh.annee = local->tm_year + 1900;  // Les années commencent à 1900
    dh.heure = local->tm_hour;
    dh.minute = local->tm_min;
    dh.seconde = local->tm_sec;

    return dh;
}

float AverageWaitingTime(Patient *head, DateHour precisedate){
    Patient *curr=head;
    float wait;
    while (curr!=NULL){
        wait=
    }

}