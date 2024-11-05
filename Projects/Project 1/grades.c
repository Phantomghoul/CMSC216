
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MX 50

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void sortAssignments(int assignNO[], int assignVal[], int na){
    int i,j;


    for(i = 0; i<na;i++){
        for(j = i; j<na-1;j++){
            if(assignVal[j]==assignVal[j+1]){
                if(assignNO[j]>assignNO[j+1]){
                    swap(&assignVal[j],&assignVal[j+1]);
                    swap(&assignNO[j],&assignNO[j+1]);
                }
            }
            else if(assignVal[j]<assignVal[j+1]){
                swap(&assignVal[j], &assignVal[j + 1]);
                swap(&assignNO[j],&assignNO[j+1]);
            }
        }
    }
}
void applyPenalty(int scores[], int pen, int daysLate[],int na){
    int i;

    for(i = 0; i <na;i++){
        scores[i] -= daysLate[i]*pen;
        if(scores[i]<=0)scores[i] =0;
    }

}

double numericScore(int scores[], int weight[], int na, int nd){
    int i, droppedweight =0;
    double ans = 0, distweight =0;

    if(nd != 0) {
        for (i = nd; i < na; i++) {
            droppedweight += weight[i];
        }
    }
    distweight = droppedweight/(na-nd);
    for(i = 0; i <na-nd;i++){
        ans += (scores[i]*(weight[i]+distweight))/100;
    }
    return ans;
}

double Mean(int scores[], int na){
    int i;
    double ans =0;

    for(i = 0; i <na;i++){
        ans += scores[i];
    }
    return ans/na;
}

double StandardDeviation(double mean, int scores[], int na){
    int i;
    double ans = 0, val =0;

    for(i = 0; i <na;i++){
        val = scores[i] - mean;
        val *= val;
        ans += val;
    }

    ans /= na;

    return sqrt(ans);
}

int main() {


    int pen, nd,na,i,j,k, weightcheck =0;
    char stats,comma;

    int assignNO[MX], scores[MX], weight[MX], daysl[MX], assignVal[MX];
    int newAssignNO[MX], newScores[MX], newWeight[MX], newDaysl[MX];
    int newScoresPenalty[MX];

    double numeric_score, mean, standardDev;
    scanf("%d %d %c %d", &pen, &nd, &stats, &na);
    for(i = 0; i<na;i++){
        scanf("%d %c %d %c %d %c %d", &assignNO[i], &comma, &scores[i], &comma, &weight[i], &comma, &daysl[i]);
        newAssignNO[i] = assignNO[i];
        newScores[i] = scores[i];
        newWeight[i] = weight[i];
        newDaysl[i] = daysl[i];
        weightcheck += weight[i];
    }

    if(weightcheck != 100){
        printf("ERROR: Invalid values provided");
    }

    for(i = 0; i <na;i++){
        assignVal[i] = scores[i] * weight[i];
    }

    sortAssignments(newAssignNO, assignVal, na);


    for(i = 0; i <na;i++){
        for(j = 0; j<na;j++){
            if(newAssignNO[i] == assignNO[j]){
                newScores[i] = scores[j];
                newAssignNO[i] = assignNO[j];
                newDaysl[i] = daysl[j];
                newWeight[i] = weight[j];
                break;
            }
        }
    }


    for(i =0; i<na;i++){
        newScoresPenalty[i] = newScores[i];
    }
    applyPenalty(newScoresPenalty,pen,newDaysl, na);


    numeric_score = numericScore(newScoresPenalty, newWeight, na,nd);

    mean = Mean(newScoresPenalty, na);

    standardDev = StandardDeviation(mean, newScoresPenalty,na);

    printf("Numeric Score: %5.4f\n",numeric_score);
    printf("Points Penalty Per Day Late: %d\n", pen);
    printf("Number of Assignments Dropped: %d\n", nd);
    printf("Values Provided: \n");
    printf("Assignment, Score, Weight, Days Late\n");
    for(i =1; i <=na; i++){
        for(j = 0; j<na;j++){
            if(assignNO[j] == i) {
                printf("%d, %d, %d, %d\n", assignNO[j], scores[j], weight[j], daysl[j]);
                break;
            }
        }
    }
    if(stats == 'Y' || stats == 'y'){ printf("Mean: %5.4f, Standard Deviation: %5.4f", mean, standardDev); }

}
