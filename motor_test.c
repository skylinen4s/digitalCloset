#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
//#include "digitalCloset_Motorcontrol.h"

FILE *fp, *fp1, *fp2;
char MotorA[2][50];
char MotorB[2][50];
void motor_control(int controlTime, int motorNumber, int orient);
void motor_init();
//void motor_test(int controlTime);

/*this .c file is not main code */
int main(void)
{
    motor_init();
    while(1){
        //motor_test(100);
        int motor;
        int time1;
        int direction;
        scanf("%d %d %d", &time1, &motor, &direction);

        motor_control(time1, motor, direction);
        sleep(2);
        /*motor_control(3, 1, 2);
        sleep(2);
        motor_control(3, 2, 1);
        sleep(5);
        motor_control(3, 2, 2);
        sleep(5);*/
        }
    return 0;
}


void motor_init(){
    int count=0;
    strcpy(MotorA[0], "/sys/class/gpio/gpio134/direction");
    strcpy(MotorA[1], "/sys/class/gpio/gpio135/direction");
    strcpy(MotorB[0], "/sys/class/gpio/gpio136/direction");
    strcpy(MotorB[1], "/sys/class/gpio/gpio137/direction");    

    for(count=0;count<2;count++){
        /*MOTOR A*/
        if ((fp = fopen("/sys/class/gpio/export", "w")) == NULL){
        printf("Cannot open export file.\n");
        exit(1);
        }
        fprintf( fp, "%d", (134+count) );
        fclose(fp);
        if ((fp = fopen(MotorA[count], "rb+")) == NULL){
            printf("Cannot open direction file.\n");
            exit(1);
        }
        /*MOTOR B*/
        if ((fp = fopen("/sys/class/gpio/export", "w")) == NULL){
        printf("Cannot open export file.\n");
        exit(1);
        }
        fprintf( fp, "%d", (136+count) );
        fclose(fp);
        if ((fp = fopen(MotorB[count], "rb+")) == NULL){
            printf("Cannot open direction file.\n");
            exit(1);
        }
    }
}

/*   controlTime = 0~1000
 *   motorNumber = 1 or 2   (A & B)
 *   orient = 1 or 2 (forward or backward)
 */
 
void motor_control(int controlTime, int motorNumber, int orient){
    while(controlTime > 0){
        if (motorNumber == 1 && orient == 1)
        {
            fp1 = fopen(MotorA[1], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorA[0], "rb+");
            fprintf(fp2, "low");
            printf("1 out\n");
            sleep(2);
            }
        else if (motorNumber == 1 && orient == 2)
        {
            fp1 = fopen(MotorA[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorA[1], "rb+");
            fprintf(fp2, "low");
            printf("1 in\n");
            sleep(2);
        }
        else if (motorNumber == 2 && orient == 1)
        {
            fp1 = fopen(MotorB[1], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorB[0], "rb+");
            fprintf(fp2, "low");
            printf("2 out\n");
            sleep(2);
        }
        else if (motorNumber == 2 && orient == 2)
        {
            fp1 = fopen(MotorB[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorB[1], "rb+");
            fprintf(fp2, "low");
            printf("2 in\n");
            sleep(2);
        }
        else{

        }
        fclose(fp1);
        fclose(fp2);
        controlTime--;
    }

        fp1 = fopen(MotorA[0], "rb+");
        fprintf(fp1, "low");
        fp2 = fopen(MotorA[1], "rb+");
        fprintf(fp2, "low");
        fclose(fp1);
        fclose(fp2);
        fp1 = fopen(MotorB[0], "rb+");
        fprintf(fp1, "low");
        fp2 = fopen(MotorB[1], "rb+");
        fprintf(fp2, "low");
        fclose(fp1);
        fclose(fp2);
}

