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



Patient* SearchBySSN(Patient* head, char searchedssn[10]){
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
#include <stdio.h>

void AverageWaitingTimeByPriority(Patient *head) {
    time_t now = time(NULL);
    time_t precise_time = now;

    // Compteurs pour chaque priorité
    int count1 = 0, count2 = 0, count3 = 0;
    double total1 = 0.0, total2 = 0.0, total3 = 0.0;

    for (Patient *p = head; p != NULL; p = p->next) {
        int jour, mois, annee, heure, minute;
        struct tm tm_arr = {0};

        // Lire la date et l'heure d'arrivée du patient
        sscanf(p->DateIn, "%d/%d/%d", &jour, &mois, &annee);
        sscanf(p->TimeIn, "%d:%d", &heure, &minute);

        tm_arr.tm_mday  = jour;
        tm_arr.tm_mon   = mois - 1;
        tm_arr.tm_year  = annee - 1900;
        tm_arr.tm_hour  = heure;
        tm_arr.tm_min   = minute;
        tm_arr.tm_sec   = 0;

        time_t arrival_time = mktime(&tm_arr);

        if (arrival_time != (time_t)-1) {
            double wait_minutes = difftime(precise_time, arrival_time) / 60.0;

            if (p->Priority == 1) {
                total1 += wait_minutes;
                count1++;
            } else if (p->Priority == 2) {
                total2 += wait_minutes;
                count2++;
            } else if (p->Priority == 3) {
                total3 += wait_minutes;
                count3++;
            }
        }
    }

    // Affichage des résultats
    printf("Temps d'attente moyen :\n");
    if (count1 > 0)
        printf(" - Priorité 1 : %.2f minutes\n", total1 / count1);
    else
        printf(" - Priorité 1 : Aucun patient\n");

    if (count2 > 0)
        printf(" - Priorité 2 : %.2f minutes\n", total2 / count2);
    else
        printf(" - Priorité 2 : Aucun patient\n");

    if (count3 > 0)
        printf(" - Priorité 3 : %.2f minutes\n", total3 / count3);
    else
        printf(" - Priorité 3 : Aucun patient\n");
}


void NumberOfPatientTreated(){
    int priority1=0;
    int priority2=0;
    int priority3=0;
    Patient *head = ReadPatientCSV("PatientHistory.CSV");
    for (Patient *p=head; p!=NULL; p = p->next){
        if (p->Priority==1){
            priority1++;
        }
        else if(p->Priority==2){
            priority2++;
        }
        else if(p->Priority==3){
            priority3++;
        }
    }
    printf("Il y a eu %d patients traités en priorité 1.", priority1);
    printf("Il y a eu %d patients traités en priorité 2.", priority2);
    printf("Il y a eu %d patients traités en priorité 3.", priority3);
}
