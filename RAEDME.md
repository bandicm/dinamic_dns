# About 

A program to automatically change dns records for dynamic IP addresses
Programmed by Marcel Bandić, contact: marcelb96@yahoo.com
Applicable in bind9 DNS servers. Support A records.

## How to run

You can only run the program on the bind9 DNS server. It runs with 
"sudo ./dinamic_dns". Before the flow, you must enter the domain in 
the doamin.conf file and capture the path to the file zone in the 
hosts.conf file since the override in the bind9 configuration.

## Development information

This program is on v0.2-beta version. Changes to version v0.1:
	- Supports logs in real time
	- Fixed errors that occurred while writing to the zone file.
    - Add a logging function. For easier monitoring of program status.
    - Enable additional functions to read configuration files for domain and host file. Because of this the program can now work for any domain or location of the host file without editing the code or compiling the program.
    - The function to retrieve the new IP address has now been upgraded and should prevent the program from crashing due to non-response from the server.
    - The function for converting the IP address in the host file has been improved.
