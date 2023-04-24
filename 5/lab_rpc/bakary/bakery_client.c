/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "bakery.h"

void bakery_prog_1(char *host)
{
	CLIENT *clnt;
	struct BAKERY  *result_1;
	struct BAKERY  get_number_1_arg;
	struct BAKERY  *result_2;
	struct BAKERY  wait_queue_1_arg;
	struct BAKERY  *result_3;
	struct BAKERY  bakery_res_1_arg;

	clnt = clnt_create (host, BAKERY_PROG, BAKERY_VER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	srand(time(NULL));
	int interval = rand() % 3 + 1;
	sleep(interval);
	get_number_1_arg.pid = getpid();
	result_1 = get_number_1(&get_number_1_arg, clnt);
	if (result_1 == (struct BAKERY *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf("Client %d has number %d\n", get_number_1_arg.pid, result_1->num);

	srand(time(NULL));
	interval = rand() % 4 + 1;
	sleep(interval);

	wait_queue_1_arg.num = result_1->num;
	wait_queue_1_arg.pid = result_1->pid;
	result_2 = wait_queue_1(&wait_queue_1_arg, clnt);

	if (result_2 == (struct BAKERY *) NULL) {
		clnt_perror(clnt, "call failed");
	}
	sleep(3);

	bakery_res_1_arg.num = wait_queue_1_arg.num;
	bakery_res_1_arg.pid = wait_queue_1_arg.pid;
	result_3 = bakery_res_1(&bakery_res_1_arg, clnt);
	
	if (result_3 == (struct BAKERY *) NULL) {
		clnt_perror(clnt, "call failed");
	}
	printf("Client %d got response %c (sleep = %d)\n", get_number_1_arg.pid, result_3->result, interval);
	clnt_destroy (clnt);
}


int main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	bakery_prog_1 (host);
	exit (0);
}