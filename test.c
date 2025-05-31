#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
typedef struct Patient{
    char ssn[10];
    char DoB[11];
    int age;
    char DateIn[11]; //pour un format DD/MM/YYYY
    char TimeIn[6]; //HH:MM
    int Priority;
    int Symptomes[7];
    int Postal;
    struct Patient* next;
}Patient;

int DoBToAge(char dob[10]){
    char *year = dob+6;
    int bornyear = atoi(year);
    int age = 2025-bornyear;
    return age;
}

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



Patient*  ReadPatientCSV(char fichier[]){
    FILE *file = fopen(fichier, "r");
    if (file == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return NULL;
    }

    char line[256]; //tableau pour stocker une ligne du fichier csv
    int lineNumber=0; //ligne du tableau lue
    Patient *head=NULL; //pointe vers la tête de la liste chaînée
    Patient *end=NULL; //fin de la liste

    int i;

    printf("Reading the CSV file...\n"); //ligne de débugage 


    while (fgets(line, sizeof(line), file)){  //fgets lis les lignes du csv une par une
        if (lineNumber == 0){
            lineNumber++; //pour éviter de lire la première ligne avec les catégories
            continue; //saute le reste de la boucle
        }

        Patient *newPatient = (Patient *)malloc(sizeof(Patient));
        if (!newPatient) {
            printf("Memory allocation error...\n");
            fclose(file);
            return NULL;
        }

        for (i=0;i<=8;i++){
            newPatient->ssn[i]=line[i];
            newPatient->ssn[9]='\0';
        }
        for (i=10;i<=19;i++){
            newPatient->DoB[i-10]=line[i];
            newPatient->DoB[10]='\0';

        }
        char Postal[6];
        for (i=21;i<=25;i++){
            Postal[i-21]=line[i]; // met le code postale dans une chaine de caractère
        }
        Postal[6]='\0';
        newPatient->Postal=atoi(Postal); //converti en une valeure entière le code postale stocké dans un int

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
        
        newPatient->age=DoBToAge(newPatient->DoB);
        
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

Patient* DeletePatient(Patient* head,Patient* patient){
    
     FILE*fpTbTr = fopen("patientTbT.csv","r");
    if (fpTbTr == NULL){
        printf("Error while openning 'patientTbT.csv'.");
    }

    FILE*fpTempw = fopen("temp.csv","w");
    if (fpTempw == NULL){
        printf("Error while openning 'temp.csv'.");
    }

    char line[256];
    int lineNumber=0;

    while (fgets(line,sizeof(line),fpTbTr))
    {

        if (strncmp(line,patient->ssn,8)!=0){
            fprintf(fpTempw,"%s",line);
        }
        lineNumber++;
    }
    fclose(fpTempw);
    fclose(fpTbTr);
    
    remove("patientTbT.csv");
    rename("temp.csv","patientTbT.csv");
    
    
    Patient*curr=head;

    if (patient != head){
        while (curr!=NULL && curr->next!=patient){
            curr=curr->next;
        }

        if (curr->next == patient){
            curr->next = patient->next;
            free(patient);
        }
    }
    else {
        head=head->next;
        free(patient);
    }
    return head;
}

Patient* AddPatientHistory(Patient*head,Patient*patient){

    //écriture dans le patientHistory

    FILE*fpHistory = fopen("PatientHistory.csv","a"); //'a' pour écrire à la fin du fichier
    if (fpHistory==NULL){
        printf("Error while openning 'PatientHistory.csv'.");
    }

    int i;

    fprintf(fpHistory,"%s,%s,",patient->ssn,patient->DoB);

        fprintf(fpHistory,"%d",patient->Postal);

    fprintf(fpHistory,",%s,%s",patient->DateIn,patient->TimeIn);
    for ( i = 0; i <= 6; i++){
        fprintf(fpHistory,",%d",patient->Symptomes[i]);
    }

    fprintf(fpHistory,"\n");
    
    fclose(fpHistory);

    //suprumier me patient de la linked list et du CSV en faisant appel a DelePatient

    head = DeletePatient(head,patient);
    return head;
}

Patient* SearchBySSN(Patient *head, char searchedssn[10]){
    Patient *curr = head;
    while (curr!=NULL && strcmp(curr->ssn,searchedssn)!=0){
        curr=curr->next;
    }
    if (curr!=NULL){
        printf("%s ", curr->ssn);
        printf("%d ", curr->age);
        printf("%s ", curr->DateIn);
        printf("%s ", curr->TimeIn);
        printf("%d ", curr->Priority);
        printf("%d \n", curr->Postal);
    }else{
        printf("The patient don't exist.");
    }
    return curr;
}

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
    printf("Average waiting time : \n");
    if (count1 > 0)
        printf(" - Priority 1 : %.2f minutes\n", total1 / count1);
    else
        printf(" - Priority 1 : No patient\n");

    if (count2 > 0)
        printf(" - Priority 2 : %.2f minutes\n", total2 / count2);
    else
        printf(" - Priority 2 : No patient\n");

    if (count3 > 0)
        printf(" - Priority 3 : %.2f minutes\n", total3 / count3);
    else
        printf(" - Priority 3 : No patient\n");
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
    printf("There was %d patients treated in priority 1.", priority1);
    printf("There was %d patients treated in priority 2.", priority2);
    printf("There was %d patients treated in priority 3.", priority3);
}

void ManuallyAdjustingPriority(Patient* head){
    Patient* tochange;
    char ManuallySSN[10];
    int NewPriority;
    printf("Please the enter the SSN of the patient you want to change the priority : ");
    scanf("%d", &ManuallySSN);
    tochange = SearchBySSN(head, ManuallySSN);
    printf("Enter the new priority for this patient");
    scanf("%d", &NewPriority);
    tochange->Priority = NewPriority;
}

int main() {
    printf("DÉMARRAGE OK\n"); //ligne pour débuguer
    Patient* list = NULL;
    Patient* HistoryList = NULL;
    int choix;
    int choixb; 
    char ssn[10];

    do {
        printf("\n======== MENU ========\n");
        printf("1. Read and display the patients\n");
        printf("2. Search a patient by SSN\n");
        printf("3. Add a patient to the history file (and delete it from the main)\n");
        printf("4. Reports \n");
        printf("5. Manually adjusting a patient priority\n");
        printf("6. Leave");
        printf("Your choice : ");
        scanf("%d", &choix);
        getchar(); // pour supprimer le '\n'

        switch (choix) {
            case 1:
                list = ReadPatientCSV("PatientTbT.csv");
                if (list == NULL) {
                    printf("The list is empty or an error came.\n");
                } else {
                    printf("\nPatients list grouped by priority:\n");
                    printf("--------------------------------------------------------------------------\n");

                    for (int priority = 1; priority <= 3; priority++) {
                        printf("PRIORITY %d:\n", priority);
                        Patient* current = list;
                        int found = 0;
                        while (current != NULL) {
                            if (current->Priority == priority) {
                                printf("%s %s %s %d %s %d ", current->ssn, current->DateIn, current->DoB, current->Priority, current->TimeIn, current->Postal);
                                for (int i = 0; i <= 6; i++) {
                                    printf("%d", current->Symptomes[i]);
                                }
                                printf("\n");
                                found = 1;
                            }
                            current = current->next;
                        }
                        if (!found) {
                            printf("No patients with priority %d\n", priority);
                        }
                        printf("--------------------------------------------------------------------------\n");
                    }
                }

                break;

            case 2:
                list = ReadPatientCSV("PatientTbt.csv");
                printf("Enter the SSN of the patient : ");
                fgets(ssn, sizeof(ssn), stdin);
                ssn[strcspn(ssn, "\n")] = '\0';
                SearchBySSN(list, ssn);
                break;

            case 3:
                HistoryList = ReadPatientCSV("PatientHistory.csv");
                printf("Enter the SSN of the patient to move to the history file : ");
                fgets(ssn, sizeof(ssn), stdin);
                ssn[strcspn(ssn, "\n")] = '\0';
                Patient* patientToMove = SearchBySSN(list, ssn);
                if (patientToMove != NULL) {
                    list = AddPatientHistory(list, patientToMove);
                    printf("The patient has been moved to the history file.\n");
                }
                break;

            case 4:   
                
                do{
                printf("1. Average patient wait times per priority level \n2. Number of patients treated per priority level\n3. Leave\nYour choice : ");
                scanf("%d",&choixb);

                switch(choixb){
                    case 1:
                        list = ReadPatientCSV("PatientTbT.csv");
                        AverageWaitingTimeByPriority(list);
                        break;
                    case 2:
                        NumberOfPatientTreated();
                    case 3:
                        printf("Back to menu...\n");
                        break;
                    default:
                        printf("Error, choose a number between 1 and 3 please \n");
                }

                }while (choixb!=3);

                break;
            case 5:
                list = ReadPatientCSV("PatientTbt.csv");
                ManuallyAdjustingPriority(list);
            case 6:
                printf("Closing the program...\n");
                break;

            default:
                printf("\nInvalid choice. Please enter a number between 1 and 5.\n");
        }
    } while (choix != 6);

    // Libération mémoire de la liste
    Patient *temp;
    while (list != NULL) {
        temp = list;
        list = list->next;
        free(temp);
    }

    return 0;
}