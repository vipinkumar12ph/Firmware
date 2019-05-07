/*
* teste_2.c
*
* Created on: 13 de Ago de 2015
*
Author: Pedro Silva
*/
#include<nuttx/config.h>
#include<stdio.h>
#include<errno.h>
#include<uORB/topics/teste_receive.h>
#include<uORB/topics/teste_topic.h>
__EXPORT int teste_2_main(int argc, char *argv[]);
int teste_2_main(int argc, char *argv[])
{
printf("teste_2.c---Inicio\n");
bool updated;
struct teste_receive_s test ;
static int topic_handle;
topic_handle = orb_subscribe(ORB_ID(teste_topic));
if(topic_handle==ERROR){
    printf("Error orb_subscribe (ERROR)=%d\n",errno);
    sleep(10);
    topic_handle = orb_subscribe(ORB_ID(teste_receive));
}
else if(topic_handle==-1){
    printf("Error orb_subscribe (-1)=%d\n",errno);
    sleep(10);
    topic_handle = orb_subscribe(ORB_ID(teste_receive));
}
if(orb_set_interval(topic_handle,1000)==ERROR){
    printf("Error orb_set_interval =%d\n",errno);
    return ERROR;
}
if(orb_copy(ORB_ID(teste_receive),topic_handle,&test)==ERROR){
    printf("Error orb_copy =%d\n",errno);
    if(orb_copy(ORB_ID(teste_receive),topic_handle,&test)==ERROR){
        printf("Error orb_copy =%d\n",errno);
        return ERROR;
        }
        }
printf("recebido=%d\n",test.inc);
int i=0;
while(i<10){
sleep(1);
//printf("While-i=%d\n",i);
if(orb_check(topic_handle,&updated)==ERROR){
printf("Error orb_check =%d\n",errno);
if(orb_check(topic_handle,&updated)==ERROR){
printf("Error orb_check =%d\n",errno);
return ERROR;
}
}
if(updated){
if(orb_copy(ORB_ID(teste_receive),topic_handle,&test)==ERROR){
printf("Error orb_copy =%d\n",errno);
if(orb_copy(ORB_ID(teste_receive),topic_handle,&test)==ERROR){
printf("Error orb_copy =%d\n",errno);
return ERROR;
}
}
printf("recebido=%d\n",test.inc);
i=test.inc;
}
}
printf("teste_2.c---Fim\n");
return OK;
}

