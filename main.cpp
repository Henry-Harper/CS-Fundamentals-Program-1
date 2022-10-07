// Henry Harper 10/06/2022


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

    srand((unsigned int)time(nullptr)); // set seed to system time

    cout << "time: " << time(NULL) << endl;

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


    // Start Code for Question 2A:---------------------------------------------------------------------------------------

    // prompt user
    cout << "Print results for Question 2a? (Y/y): ";
    cin >> userInput;
    checkInput(userInput);

    // Question 2 variables (using ints for my sanity and to keep things simple)

    const int MTBF = 500;           // mean time between failures (given)
    const int restTime = 10;        // restoration time (given)

    int server1Time = 0;            // keeps track of current time (in hours) for server 1
    int server2Time = 0;            // keeps track of current time (in hours) for server 2
    int server1TBF;                 // where random time between failure is stored for server 1
    int server2TBF;         // where random time between failure is stored for server 2

    // while 20 years have not passed for either server:
    while(server1Time < 175200 || server2Time < 175200){ // 175200 == 20 years converted to hours
        // server 1:
        if(server1Time < 175200){
            float randNum = float(rand()) / float((RAND_MAX)); // generate random num
            server1TBF = -1 * (MTBF) * log(randNum); // simulate exponential distribution for uptime
            server1Time += server1TBF;
            cout << "<Server 1 Down: " << server1Time << " to " << server1Time + restTime << ">";
            server1Time += restTime;
        }
        else
            cout << setw(33) << "                  "; // print nothing if server 2 still going

        // server 2:
        if(server2Time < 175200) {
            float randNum2 = float(rand()) / float((RAND_MAX)); // generate random num
            server2TBF = -1 * (MTBF) * log(randNum2); // simulate exponential distribution for uptime
            server2Time += server2TBF;
            cout << setw(20) << "<Server 2 Down: " << server2Time << " to " << server2Time + restTime << ">\n";
            server2Time += restTime;
        }
        else
            cout << setw(20) << "\n"; // print nothing if server 1 is still going
    }

    //QUESTION 2B================================================================================
    // if overlap between downtimes, output how long it took

    int server1DownTime = 0;
    int server2DownTime = 0;
    int server1RestoredTime;
    int server2RestoredTime;
    int failTime;
    int averageFailTime = 0;
    int simCount = 0;
    bool overlap = false;

    cout << "\n\nStart simulation for Question 2b?: (Y/y)";
    cin >> userInput;
    checkInput(userInput);

    while(userInput == 'Y' || userInput == 'y') {
        while (!overlap) {
            // server 1:
            float randNum = float(rand()) / float((RAND_MAX)); // generate random num
            server1TBF = -1*(MTBF) * log(randNum); // simulate exponential distribution for uptime
            server1DownTime += server1TBF;
            server1RestoredTime = server1DownTime + 10;

            float randNum2 = float(rand()) / float((RAND_MAX)); // generate random num
            server2TBF = -1*(MTBF) * log(randNum2); // simulate exponential distribution for uptime
            server2DownTime += server2TBF;
            server2RestoredTime = server2DownTime + 10;

            // check for overlap
            if (server1DownTime <= server2RestoredTime && server1RestoredTime >= server2DownTime) {
                overlap = true;
                failTime = server2DownTime;
            }
            if (server2DownTime <= server1RestoredTime && server2RestoredTime >= server1DownTime) {
                overlap = true;
                failTime = server1DownTime;
            }
        }
        cout << "Overlap found: S1 down at " << server1DownTime << "hr, S2 down at " << server2DownTime << "hr" << endl;
        cout << "System failed at: " << failTime << "hours" << endl;
        averageFailTime += failTime;

        // RE INITIALIZE VARIABLES (Took so long to find this bug) :(
        server1DownTime = 0;
        server2DownTime = 0;
        overlap = false;

        cout << "Start another simulation? (Y/y) (n to find average): ";
        cin >> userInput;
        simCount++;

        srand(time(nullptr));
    }


    averageFailTime = averageFailTime / simCount;
    cout << "Average System Failure for " << simCount << " simulations: " << averageFailTime << " hours" << endl;

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
