/*
* teste_1.c
*/
#include<nuttx/config.h>
#include<stdio.h>
#include<errno.h>
#include<uORB/topics/teste_topic.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<px4_config.h>
#include<nuttx/sched.h>
#include<px4_tasks.h>
//#include<lib/systemlib/systemlib.h>
#include<lib/systemlib/err.h>
static bool thread_should_exit = false;
/**< daemon exit flag */
static bool thread_running = false;
/**< daemon status flag */
static int daemon_task;
/**< Handle of daemon task / thread */
/**
* daemon management function.
*/
__EXPORT int teste_1_main(int argc, char *argv[]);
/**
* Mainloop of daemon.
*/
int teste_1_daemon_app_main(int argc, char *argv[]);
/**
* Print the correct usage.
*/
static void usage(const char *reason);
static void
usage(const char *reason)
{
if (reason) {
warnx("%s\n", reason);
}
warnx("usage: daemon {start|stop|status} [-p <additional params>]\n\n");
}
/**
* The daemon app only briefly exists to start
* the background job. The stack size assigned in the
* Makefile does only apply to this management task.
*
* The actual stack size should be set in the call
* to task_create().
*/
int teste_1_main(int argc, char *argv[])
{
if (argc < 2) {
usage("missing command");
return 1;
}
if (!strcmp(argv[1], "start")) {
if (thread_running) {
warnx("daemon already running\n");
/* this is not an error */
return 0;
}
thread_should_exit = false;
daemon_task = px4_task_spawn_cmd("teste_1",SCHED_DEFAULT,SCHED_PRIORITY_DEFAULT,2000,teste_1_daemon_app_main,
                                 (argv) ? (char * const *)&argv[2] : (char * const*)NULL);

return 0;
}
if (!strcmp(argv[1], "stop")) {
thread_should_exit = true;
return 0;
}
if (!strcmp(argv[1], "status")) {
if (thread_running) {
warnx("\trunning\n");
} else {
warnx("\tnot started\n");
}
return 0;
}
usage("unrecognized command");
return 1;
}
int teste_1_daemon_app_main(int argc, char *argv[])
{
warnx("[daemon] starting\n");
thread_running = true;
printf("teste_1.c---Inicio\n");
struct teste_topic_s test = {.inc = 0};
orb_advert_t topic_handle = orb_advertise(ORB_ID(teste_topic), &test);
// naparte do publisher tem que ser orb_advert_t em vez de static int
sleep(10);
while (!thread_should_exit) {
for (int i=0;i<10;i++)
{
test.inc++;
if(orb_publish(ORB_ID(teste_topic),topic_handle,&test)!=OK){
printf("Erro orb_publish=%d\n",errno);
thread_running = false;
return ERROR;
}
sleep(10);
if(thread_should_exit)
{
printf("i=%d",i);
break;
}
}
break;
}
printf("teste_1.c---Fim\n");
warnx("[daemon] exiting.\n");
thread_running = false;
return OK;
}
