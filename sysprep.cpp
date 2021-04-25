#include <iostream>
#include <limits.h>
#include <limits>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

char   hostname[HOST_NAME_MAX+1];
char   domainname[HOST_NAME_MAX+1];
string newhostname;

void success_handler();
void error_handler();
int  set_hostname(string);


int main(void){
	if (gethostname(hostname, sizeof(hostname)) != 0){
		cout << "Error while calling gethostname () function. Error code: " << errno << endl;
		error_handler();
		return errno;
		}
	cout << "Current hostname: " << hostname << endl;
	cout << "Enter new hostname: " << flush; cin >> newhostname;
	if (set_hostname(newhostname) == 0){
		cout << "New hostname applied successfully." << endl;
		}
	else {
		error_handler();
		return -1;
		}

	success_handler();
	return 0;
	}


void error_handler(){
	string press_anykey_message = "Press any key to continue...";
	cout << endl << press_anykey_message << endl;
	cin.ignore(numeric_limits <streamsize>::max(), '\n');
	cin.get();
	}

void success_handler(){
	string press_anykey_message = "All steps completed successfully. Press any key to continue...";
        cout << endl << press_anykey_message << endl;
	cin.ignore(numeric_limits <streamsize>::max(), '\n');
	cin.get();
	}


int set_hostname(string hostname){

        if (getdomainname(domainname, sizeof(domainname)) != 0){
                cout << "Error while calling getdomainname () function. Error code: " << errno << endl;
                error_handler();
                return errno;
                }
        cout << "Current domainname: " << hostname << endl;
	return 0;


	fstream hostname_file("/etc/hostname", ofstream::out | ofstream::trunc);
	fstream hosts_file("/etc/hosts", ofstream::out | ofstream::trunc);
	if (!hostname_file.is_open() || !hosts_file.is_open())
		{
		cout << "Unable to open /etc/hostname or(and) /etc/hosts file.";
		error_handler();
		return errno;
		}

        hostname_file << hostname << endl;
        hostname_file.close();

	if (sethostname(hostname.c_str(), hostname.size()) !=0){
                cout << "Error while calling sethostname () function. Error code: " << errno << endl;
                error_handler();
                return errno;
                }


	return 0;
	}
