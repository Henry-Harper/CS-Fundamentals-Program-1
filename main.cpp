/* Henry Harper 10/06/2022
 *
 * 1) Using a pseudo random number generator function [rand()]:
 * - generate a workload for a system that is composed of 1000 processes.
 * - Assume that processes arrive with an expected average arrival rate of 2 processes / second (poisson distribution)
 * - The service time (requested duration on the CPU) = 1 second , (Exponential distribution)
 *
 * - Your outcome should be printing out a list of tuples in the format of:
 *      <process ID, arrival time, requested service time>
 *
 * 2) A Computing system is composed of two servers that are mirrors of each other
 */

#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

// struct to hold ID, arrival time, service time for each process in question 1
struct process{
    int ID;
    float arrivalTime;
    float serviceTime;
};

// struct to hold downTime
struct server{
    int downTime;
    int restoredTime;
};

// function prototypes
float findServiceTime(float);
float findArrivalTime(float);
char checkInput(char);

int main(){
    // Start Code for Question 1:---------------------------------------------------------------------------------------

    process processes[1000]; // create array of structs to hold all values
    float actualAvgServiceTime = 0; // calculated later
    char userInput;

    // prompt user so they don't get spammed instantly with 1000 lines
    cout << "Print results for Question 1? (Y/y): ";
    cin >> userInput;
    checkInput(userInput); // check for invalid input


    srand((unsigned int)time(NULL)); // set seed to system time

    for(int i = 0; i < 1000; i++){
        processes[i].ID = i+1; // set process ID from 1-1000

        processes[i].serviceTime = findServiceTime(processes[i].serviceTime); // find service time
        actualAvgServiceTime += processes[i].serviceTime; // find total service time for average

        processes[i].arrivalTime = findArrivalTime(processes[i].arrivalTime); // find arrival time
        processes[i].arrivalTime += processes[i-1].arrivalTime; // add previous time

        // print results for each process
        cout << std::setprecision(5) << "<" << processes[i].ID << ", " << processes[i].arrivalTime << ", "
             << processes[i].serviceTime << ">" << endl;
    }
    cout << "Actual Average Arrival Time: " << processes[999].arrivalTime / 1000 << endl; // divide totals by n for avg
    cout << "Actual Average Service Time: " << actualAvgServiceTime / 1000 << endl;


    // Start Code for Question 2:---------------------------------------------------------------------------------------

    // prompt user
    cout << "Print results for Question 2a? (Y/y): ";
    cin >> userInput;
    checkInput(userInput);

    // Question 2 variables (using ints for my sanity and to keep things simple)

    const int MTBF = 500;           // mean time between failures (given)
    const int restTime = 10;        // restoration time (given)

    int server1Time = 0;            // keeps track of current time (in hours) for server 1
    int server2Time = 0;            // keeps track of current time (in hours) for server 2
    int server1uptimeHours;         // where random time between failure is stored for server 1
    int server2uptimeHours;         // where random time between failure is stored for server 2

    srand((unsigned int)time(NULL)); // set seed to system time

    // while 20 years have not passed for either server:
    while(server1Time < 175200 || server2Time < 175200){ // 175200 == 20 years converted to hours
        // server 1:
        if(server1Time < 175200){
            float randNum = float(rand()) / float((RAND_MAX)); // generate random num
            server1uptimeHours = -1 * (MTBF) * log(randNum); // simulate exponential distribution for uptime
            server1Time += server1uptimeHours;
            cout << setw(35) << "<Server 1 Down hour: " << server1Time << " to " << server1Time + restTime << ">";
            server1Time += restTime;
        }
        else
            cout << setw(52) << "                  "; // print nothing if server 2 still going

        // server 2:
        if(server2Time < 175200) {
            float randNum2 = float(rand()) / float((RAND_MAX)); // generate random num
            server2uptimeHours = -1 * (MTBF) * log(randNum2); // simulate exponential distribution for uptime
            server2Time += server2uptimeHours;
            cout << setw(35) << "<Server 2 Down hour: " << server2Time << " to " << server2Time + restTime << ">\n";
            server2Time += restTime;
        }
        else
            cout << setw(35) << "\n"; // print nothing if server 1 is still going
    }

    //QUESTION 2B================================================================================
    // if overlap between downtimes, output how long it took

    int server1DownTime = 0;
    int server2DownTime = 0;
    int server1RestoredTime;
    int server2RestoredTime;
    int server1TBF;
    int server2TBF;

    bool overlap = false;
    int timetoFail;

    while(!overlap){
        // server 1:
        float randNum = float(rand()) / float((RAND_MAX)); // generate random num
        server1TBF = -1 * (MTBF) * log(randNum); // simulate exponential distribution for uptime
        server1DownTime += server1TBF;
        server1RestoredTime = server1DownTime + 10;

        float randNum2 = float(rand()) / float((RAND_MAX)); // generate random num
        server2TBF = -1 * (MTBF) * log(randNum2); // simulate exponential distribution for uptime
        server2DownTime += server2TBF;
        server2RestoredTime = server2DownTime + 10;

        // server 1 downtime = 10 , restored = 20
        // server 2 downtime = 21 , restored = 31

        // s1d + 1 = 11 , <= 31 true
        // s1r - 1 = 19 , >= 21 false

        // server 1 downtime = 10 , restored = 20
        // server 2 downtime = 15 , restored = 25

        // s1d + 1 = 11 , <= 25 true
        // s1r - 1 = 19 , >= 15 true
        if(server1DownTime <= server2RestoredTime && server1RestoredTime >= server2DownTime){
            overlap = true;
            timetoFail = server1DownTime;
        }
        if(server2DownTime <= server1RestoredTime && server2RestoredTime >= server1DownTime){
            overlap = true;
            timetoFail = server2DownTime;
        }
    }
    cout << "\nOverlap found: S1 down @" << server1DownTime << ", S2 down @" << server2DownTime << endl;
    cout << "Time for system to fail: " << timetoFail << endl;

    return 0;
}

// loops user until they type y or Y
char checkInput(char input){
    while(input != 'Y' && input != 'y'){
        cout << "ERROR: Invalid input. Enter 'Y' or 'y' to run: ";
        cin >> input;
    }
    return input;
}

// generates random service time simulating exponential distribution given average rate = 1.0
float findServiceTime(float serviceTime){
    const float avgServiceRate = 1.0;

    float randNum = float(rand())/float((RAND_MAX)); // generate random num
    serviceTime = -1*(avgServiceRate) * log(randNum); // simulate exponential distribution
    return serviceTime;
}

// generates random arrival time simulating poisson distribution given average rate = 2.0
float findArrivalTime(float arrivalTime){
    const float avgArrivalRate = 2.0;

    float randNum2 = float(rand())/float((RAND_MAX)); // generate second set of random nums
    arrivalTime = (-1*avgArrivalRate) * log(randNum2); // generate inter-arrival times
    return arrivalTime;
}
