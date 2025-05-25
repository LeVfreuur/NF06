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

    printf("Lecture du fichier CSV...\n"); //ligne de débugage 


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
        printf("Ce patient n'existe pas");
    }
    return curr;
}

int main() {
    printf("DÉMARRAGE OK\n"); //ligne pour débuguer
    Patient* list = NULL;
    int choix;
    char ssn[10];

    do {
        printf("\n======== MENU ========\n");
        printf("1. Lire et afficher les patients (depuis CSV)\n");
        printf("2. Rechercher un patient par SSN\n");
        printf("3. Ajouter un patient à l'historique (et le supprimer de la liste)\n");
        printf("4. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar(); // pour supprimer le '\n'

        switch (choix) {
            case 1:
                list = ReadPatientCSV();
                if (list == NULL) {
                    printf("La liste est vide ou une erreur est survenue lors de la lecture.\n");
                } else {
                    printf("\nListe des patients triés par priorité :\n");
                    printf("--------------------------------------------------------------------------\n");
                    Patient* current = list;
                    while (current != NULL) {
                        printf("%s %s %s %d %s %d ", current->ssn, current->DateIn, current->DoB, current->Priority, current->TimeIn, current->Postal);
                        for (int i = 0; i <= 6; i++) {
                            printf("%d", current->Symptomes[i]);
                        }
                        printf("\n");
                        current = current->next;
                    }
                }
                break;

            case 2:
                printf("Entrez le SSN du patient à rechercher : ");
                fgets(ssn, sizeof(ssn), stdin);
                ssn[strcspn(ssn, "\n")] = '\0'; // enlever le \n
                SearchBySSN(list, ssn);
                break;

            case 3:
                printf("Entrez le SSN du patient à déplacer vers l'historique : ");
                fgets(ssn, sizeof(ssn), stdin);
                ssn[strcspn(ssn, "\n")] = '\0';
                Patient* patientToMove = SearchBySSN(list, ssn);
                if (patientToMove != NULL) {
                    list = AddPatientHistory(list, patientToMove);
                    printf("Le patient a été déplacé dans l'historique.\n");
                }
                break;

            case 4:
                printf("Fermeture du programme...\n");
                break;

            default:
                printf("Choix invalide. Veuillez entrer un numéro entre 1 et 4.\n");
        }
    } while (choix != 4);

    // Libération mémoire de la liste
    Patient *temp;
    while (list != NULL) {
        temp = list;
        list = list->next;
        free(temp);
    }

    return 0;
}








