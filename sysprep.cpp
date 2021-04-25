#include <iostream>
#include <limits.h>
#include <limits>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

string service_name = "sysprep.service";

char   hostname[HOST_NAME_MAX+1];
string newhostname;
string domainsuffix;

void start_banner();
void success_handler();
void error_handler();
int  set_hostname(string);
int  cleanup();


int main(void){
	start_banner();

	if (gethostname(hostname, sizeof(hostname)) != 0){
		cout << "Error while calling gethostname () function. Error code: " << errno << endl;
		error_handler();
		return errno;
		}
	cout << "Current hostname: " << hostname << endl << endl;
	cout << "Starting change hostname..." << endl;
	cout << "Enter new hostname: " << flush; cin >> newhostname;
	cout << "Enter domain suffix (press ENTER if not needed): " << flush;
	cin.ignore(numeric_limits <streamsize>::max(), '\n');
	getline(cin, domainsuffix);
	if (set_hostname(newhostname) == 0){
		cout << "New hostname applied successfully." << endl << endl;
		}
	else {
		error_handler();
		return -1;
		}


	cout << "Self-cleaning procedure started..." << endl;
	if (cleanup() != 0){
		error_handler();
		return -1;
		}
	cout << "Self-cleaning procedure completed successfully." << endl << endl;

	success_handler();
	return 0;
	}


void error_handler(){
	string press_anykey_message = "Errors occurred. Press any key to continue...";
	cout << endl << press_anykey_message << endl;
	cin.get();
	}

void success_handler(){
	string press_anykey_message = "All steps completed successfully. Press any key to continue...";
        cout << press_anykey_message << endl;
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

int cleanup(){
	string command1 = "systemctl disable ";
	string command2 = "rm /etc/systemd/system/";

	int ret_value1 = system((command1 + service_name).c_str());
	int ret_value2 = system((command2 + service_name).c_str());
	if (ret_value1 == 0 && ret_value2 == 0){
		cout << "Preparation tool successfully removed from boot process." << endl;
		return 0;
		}
	else{
		cout << "Errors occurred while uninstalling the Preparation tool." << endl;
		return -1;
		}
	}

void start_banner(){
	const char *banner =
		"****************************************************\n"
		"***	This is first boot after cloning VM.\n"
		"****************************************************\n";

	cout << endl << banner << endl;
	}
