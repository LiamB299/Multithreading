#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <pthread.h>
#include <iomanip>
#include <mutex>
using namespace std;

void* thread(void * pass);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct device {
	string room;
	pid_t procID;
	string value;
	string time;
};

int icount=0, rc=0;
pthread_mutex_t mute = PTHREAD_MUTEX_INITIALIZER;
pthread_t p_thread;

device devices;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main() {
	ifstream infile;
	infile.open("device_data.txt");
	string sline;
	//cout<<"Work!!!";
	rc = pthread_mutex_unlock(&mute);

	while(infile>>sline) {
		icount++;

		devices.room = sline.substr(0, sline.find(','));
		sline.erase(0, sline.find(',')+1);
		devices.value = sline.substr(0, sline.find(','));
		sline.erase(0, sline.find(',')+1);
		devices.time = sline;

		pthread_create(&p_thread, NULL, thread, (void*)&devices);
		pthread_join(p_thread,0);
		//sleep(0.1); 
	}

	infile.close();
	rc = pthread_mutex_destroy(&mute);
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void* thread(void * pass) {
	rc = pthread_mutex_lock(&mute);	
	
	device *dev = (device*)pass;
	dev->procID = pthread_self();//getpid();
	cout<<"Created thread: "<<dev->procID<<endl;
	cout<<"Thread "<<icount<<" with ID "<<pthread_self()<<" received message: "<<endl; 
	cout<<"Device = "<<dev->room<<endl;
	cout<<"Value = "<<dev->value<<endl;

	int sec=0;
	stringstream ss(dev->time);
	ss >> sec;

	time_t ep = (time_t)sec;
	char epoch[150];								
	struct tm tstruct = *localtime(&ep);
	strftime(epoch, sizeof(epoch), "%H:%M:%S %d/%m/%Y", &tstruct);

	string stime(epoch);
	dev->time = stime;

	cout<<"Time = "<<dev->time<<endl;

	rc = pthread_mutex_unlock(&mute);
	
	pthread_exit(NULL);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++