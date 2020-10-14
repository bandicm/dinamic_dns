
/** A program to automatically change dns records for dynamic IP addresses
 * Programmed by Marcel Bandić, contact: marcelb96@yahoo.com
 * Applicable in bind9 DNS servers. Support A records.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define IP 16

char domain_A[35]; //initialization in main()
char wan_ip[IP];
char host_file[100]; //initialization in main()

void wan_ip_txt() {
    system("dig +short myip.opendns.com @resolver1.opendns.com > wan_ip.txt");
}

int get_wan_ip(){
    char tmp_ip[IP];
    FILE *f;
    f = fopen("wan_ip.txt", "r");
    if (f==NULL) exit(1);
    fgets(tmp_ip, IP, f);
    int a = strcmp(wan_ip, tmp_ip);
    if (a) 
        strcpy(wan_ip, tmp_ip);
    return a;
    fclose(f);
}

void add_ip_in_A() {
    FILE *f;
    f = fopen(host_file, "r");
    if (f==NULL) exit(2);
    char hosts[100][300];
    int i;
    while (!(feof(f))) { 
        fgets(hosts[i], 300, f);
        i++;
    }
    fclose(f);
    
    int j, n, r=i;
    char tmp_A[45];
    strcpy(tmp_A, domain_A);
    strcat(tmp_A, wan_ip);
    for (i=0; i<r; i++) {
        n=1; 
        for (j=0; j<strlen(domain_A); j++)  
            if (hosts[i][j] != domain_A[j]) 
                n=0;
        
        if (n) 
            strcpy(hosts[i], tmp_A);
        
    }
    
    f = fopen(host_file, "w");
    if (f==NULL) exit(3);
    
    for (i=0; i<r; i++) 
        fputs(hosts[i], f);
   
   fclose(f);
}

void rndc_reload() {
    system("rndc reload");
}

int main () {
    strcpy(domain_A, "garfild.ml.     IN      A       ");
    strcpy(host_file, "/var/lib/bind/garfild.ml.hosts");
   
    while (1) {
		wan_ip_txt();
        if (get_wan_ip()) {
            add_ip_in_A(); 
            rndc_reload();
        }
        sleep(600); // 10 minutes	
	}
   
    return 0;
}