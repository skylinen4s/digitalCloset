#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
FILE *fp, *fp1, *fp2,* fp3;
char MotorA[3][50];
char MotorB[3][50];
void motor_control(int controlTime, int motorNumber, int orient);
void motor_init();


/*this .c file is not main code */
#if 0
int main(void)
{
    motor_init();
    while(1){
        motor_control(10, 1, 1);
        //sleep(10);
        motor_control(10, 1, 2);
        //sleep(10);
        motor_control(10, 2, 1);
        //sleep(10);
        motor_control(10, 2, 2);
        //sleep(10);
        }
    return 0;
}
#endif

void motor_init(){
    int count=0;
    strcpy(MotorA[0], "/sys/class/gpio/gpio134/direction");
    strcpy(MotorA[1], "/sys/class/gpio/gpio135/direction");
    strcpy(MotorA[2], "/sys/class/gpio/gpio136/direction");
    strcpy(MotorB[0], "/sys/class/gpio/gpio137/direction");
    strcpy(MotorB[1], "/sys/class/gpio/gpio138/direction");
    strcpy(MotorB[2], "/sys/class/gpio/gpio139/direction");

    

    for(count=0;count<3;count++){
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
        fprintf( fp, "%d", (137+count) );
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
            fp1 = fopen(MotorA[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorA[1], "rb+");
            fprintf(fp2, "high");
            fp3 = fopen(MotorA[2], "rb+");
            fprintf(fp3, "low");
            printf("1 out\n");
            }
        else if (motorNumber == 1 && orient == 2)
        {
            fp1 = fopen(MotorA[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorA[2], "rb+");
            fprintf(fp2, "high");
            fp3 = fopen(MotorA[1], "rb+");
            fprintf(fp3, "low");
            printf("1 in\n");
        }
        else if (motorNumber == 2 && orient == 1)
        {
            fp1 = fopen(MotorB[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorB[1], "rb+");
            fprintf(fp2, "high");
            fp3 = fopen(MotorB[2], "rb+");
            fprintf(fp3, "low");
            printf("2 out\n");
        }
        else if (motorNumber == 2 && orient == 2)
        {
            fp1 = fopen(MotorB[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorB[2], "rb+");
            fprintf(fp2, "high");
            fp3 = fopen(MotorB[1], "rb+");
            fprintf(fp3, "low");
            printf("2 in\n");
        }
        else{

        }
        sleep(1);
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        controlTime--;
    }
}
