#include <iostream>
#include <limits.h>
#include <limits>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

char   hostname[HOST_NAME_MAX+1];
string newhostname;
string domainsuffix;

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
	cout << "Enter domain suffix (press ENTER if not needed): " << flush;
	cin.ignore(numeric_limits <streamsize>::max(), '\n');
	getline(cin, domainsuffix);
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
	cin.get();
	}

void success_handler(){
	string press_anykey_message = "All steps completed successfully. Press any key to continue...";
        cout << endl << press_anykey_message << endl;
	cin.get();
	}


int set_hostname(string hostname){

	fstream hostname_file("/etc/hostname", ofstream::out | ofstream::trunc);
	fstream hosts_file("/etc/hosts", ofstream::out | ofstream::trunc);
	if (!hostname_file.is_open() || !hosts_file.is_open())
		{
		cout << "Unable to open /etc/hostname or(and) /etc/hosts file.";
		error_handler();
		return errno;
		}

	if (sethostname(hostname.c_str(), hostname.size()) !=0){
                cout << "Error while calling sethostname () function. Error code: " << errno << endl;
                error_handler();
                return errno;
                }

        hostname_file << hostname << endl;
        hostname_file.close();

	hosts_file << "127.0.0.1" << '\t' << "localhost" << endl;
	if (domainsuffix.empty()){
		hosts_file << "127.0.1.1" << '\t' << newhostname << endl;}
	else{
		string FQDN = newhostname + "." + domainsuffix;
        	hosts_file << "127.0.1.1" << '\t' << FQDN << '\t' << newhostname << endl;
        	}

	hosts_file.close();
	return 0;
	}

int change_machineid(){
	return 0;
	}
