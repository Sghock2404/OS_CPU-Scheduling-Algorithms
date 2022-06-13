/*
*FCFS.cpp
*Scott Hockaday
*Course: Operating Systems CS4328
*project 2
* due: 11/11/21
*/

//libraries

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <iomanip>

struct process {
    int pid;
    int at_FCFS; //arrival time
    int bt_FCFS; //burst time
    int st_FCFS; //start time
    int ct_FCFS; //completion time
    int tat_FCFS; //turnaround time
    int wt_FCFS; //waiting time
    int rt_FCFS; //response time
};

bool compareArrival(process p1, process p2)
{
    return p1.at_FCFS < p2.at_FCFS;
}

bool compareID(process p1, process p2)
{
    return p1.pid < p2.pid;
}

int main() {

    int number_Of_Processes;
    struct process p[100];
    auto average_tat = 0.0; //average turnaroundtime
    auto average_wt = 0.0; //average wait time
    auto average_rt = 0.0; //average response time
    auto cpu_utilization = 0.0;
    auto total_tat = 0; //total turnaround time
    auto total_wt = 0; // total waiting time
    auto total_rt = 0; //total response time
    auto total_it = 0; //total idle time
    auto throughput = 0.0;

    std::cout << std::setprecision(2) << std::fixed;

    std::cout << "Enter the number of processes: ";
    std::cin >> number_Of_Processes;

    for (int i = 0; i < number_Of_Processes; i++) {
        std::cout << "Enter arrival time of process " << i + 1 << ": ";
        std::cin >> p[i].at_FCFS;
        std::cout << "Enter burst time of process " << i + 1 << ": ";
        std::cin >> p[i].bt_FCFS;
        p[i].pid = i + 1;
        std::cout << std::endl;
    }

    std::sort(p, p + number_Of_Processes, compareArrival);

    for (int i = 0; i < number_Of_Processes; i++) {
        p[i].st_FCFS = (i == 0) ? p[i].at_FCFS : std::max(p[i - 1].ct_FCFS, p[i].at_FCFS);
        p[i].ct_FCFS = p[i].st_FCFS + p[i].bt_FCFS;
        p[i].tat_FCFS = p[i].ct_FCFS - p[i].at_FCFS;
        p[i].wt_FCFS = p[i].tat_FCFS - p[i].bt_FCFS;
        p[i].rt_FCFS = p[i].st_FCFS - p[i].at_FCFS;

        total_tat += p[i].tat_FCFS;
        total_wt += p[i].wt_FCFS;
        total_rt += p[i].rt_FCFS;
        total_it += (i == 0) ? (p[i].at_FCFS) : (p[i].st_FCFS - p[i - 1].ct_FCFS);
    }

    average_tat = (float)total_tat / number_Of_Processes;
    average_wt = (float)total_wt / number_Of_Processes;
    average_rt = (float)total_rt / number_Of_Processes;
    cpu_utilization = ((p[number_Of_Processes - 1].ct_FCFS - total_it) / (float)p[number_Of_Processes - 1].ct_FCFS) * 100;
    throughput = float(number_Of_Processes) / (p[number_Of_Processes - 1].ct_FCFS - p[0].at_FCFS);

    std::sort(p, p + number_Of_Processes, compareID);

    std::cout << std::endl;
    std::cout << "#P\t" << "Arrival\t" << "Burst\t" << "Turnaround time\t" << std::endl;

    for (int i = 0; i < number_Of_Processes; i++) {
        std::cout << p[i].pid << "\t" << p[i].at_FCFS << "\t" << p[i].bt_FCFS << "\t" << "\t" << p[i].tat_FCFS << "\t" << std::endl;
    }
    std::cout << "Average Turnaround Time = " << average_tat << std::endl;
    std::cout << "CPU Utilization = " << cpu_utilization << "%" << std::endl;
    std::cout << "Throughput = " << throughput << " process/second" << std::endl;
}