
/** A program to automatically change dns records for dynamic IP addresses
 * Programmed by Marcel Bandić, contact: marcelb96@yahoo.com
 * Applicable in bind9 DNS servers. Support A records.
 * Read more at README.md
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define IP 70

char domain_A[30];
char new_ip[IP];
char old_ip[IP];
char host_file[100];

void logs(int a);
void read_files(FILE* f, char tmp[]);
void read_domain();
void get_old_ip();
void get_new_ip_txt();
void add_ip_in_A();

int main () {
    read_domain();
	logs(1);
    while (1) {		
		get_old_ip();
		logs(2);
		get_new_ip_txt();
		logs(3);
        if (strcmp(new_ip, old_ip)) {
            add_ip_in_A();
            logs(4);
            system("rndc reload");
			logs(5);
        }
        logs(6);
        sleep(300); // 5 minutes
	}
    return 0;
}

void read_files(FILE* f, char tmp[]) {
	char c;
	int i=0;
    do {
		c = fgetc(f);
		if (c == '\n') {
			tmp[i++] = '\0';
			break;
		}
		tmp[i++]=c;		
	} while (1);
}

void logs(int a) {
	char log[100];
	system("date > date");
	FILE *f;
	f = fopen("date", "r");
	if (f==NULL) exit(3);
	read_files(f, log);
	fclose(f);
	switch (a) {
		case 1: {
			strcat(log, " Loading domain.conf and hosts.conf file... ");
			break;
		}
		case 2: {
			strcat(log, " Loading previous IP... ");
			strcat(log, old_ip);
			break;
		}
		case 3: {
			strcat(log, " Loading new IP... ");
			strcat(log, new_ip);
			break;
		}
		case 4: {
			strcat(log, " Writing IP address to host file... ");
			break;
		}
		case 5: {
			strcat(log, " Reload bind9 DNS server... ");
			break;
		}
		case 6: {
			strcat(log, " Sleep 10 minutes... ");
			break;
		}
		default: {
			printf("ERROR");
		}
	}
	strcat(log, "\n");
	printf("%s", log);
    f = fopen("console.log", "a");
    if (f==NULL) exit(3);
    fputs(log, f);
    fclose(f);
}

void read_domain(){
    FILE *f;
    f = fopen("domain.conf", "r");
    if (f==NULL) exit(1);
    read_files(f, domain_A);
    fclose(f);
    f = fopen("hosts.conf", "r");	 
    if (f==NULL) exit(2);
    read_files(f, host_file);
    fclose(f);
}

void get_old_ip() {			
    FILE *f;
    char command[100];
    strcpy(command, "dig ");
    strcat(command, domain_A);
    strcat(command, " @localhost +short > dns_ip");
    
	do {
		system(command);
		f = fopen("dns_ip", "r");
		if (f==NULL) exit(4);
		read_files(f, old_ip);
		fclose(f);
	} while(!(strcmp(old_ip, ";; connection timed out; no servers could be reached") && strlen(old_ip)));
}


void get_new_ip_txt() {
	FILE *f;
	do {
		system("dig +short myip.opendns.com @resolver1.opendns.com > wan_ip");
		f = fopen("wan_ip", "r");
		if (f==NULL) exit(4);
		read_files(f, new_ip);
		fclose(f);
	} while(!(strcmp(new_ip, ";; connection timed out; no servers could be reached") && strlen(new_ip)));
}


void add_ip_in_A() {
    char record[100];
    strcpy(record, "sed -i 's/");
    strcat(record, old_ip);
    strcat(record, "/");
    strcat(record, new_ip);
    strcat(record, "/g' ");
    strcat(record, host_file);
    system(record);
}
