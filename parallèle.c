#include <stdlib.h>
#include <stdio.h>

float TimeConvertion(char time[6]){
    char hourCara[3];
    char minCara[3];

    hourCara[0]=time[0];
    hourCara[1]=time[1];

    minCara[0]=time[3];
    minCara[1]=time[4];

    hourCara[2]='\0';
    minCara[2]='\0';

    float hourInt=atoi(hourCara);
    float minInt=atoi(minCara);

    float FinalTime=hourInt +minInt/60;

    return FinalTime;


}

//int PriorityEscalationSystem(int level,char time){}


int main(){

    float result=TimeConvertion("10:30");
    printf("The result is %f",result);

    return 0;
}