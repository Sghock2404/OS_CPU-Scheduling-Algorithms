/*
*RoundRobin.cpp
*Scott Hockaday
*Course: Operating Systems CS4328
*project 2
* due: 11/11/21
*/

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <queue>


struct process {
    int pid;
    int at_RR; //arrival time
    int bt_RR; //burst time
    int st_RR; //start time
    int ct_RR; //completion time
    int tat_RR; //turnaround time
    int wt_RR; //waiting time
    int rt_RR; //response time
};

bool compare1(process p1, process p2)
{
    return p1.at_RR < p2.at_RR;
}

bool compare2(process p1, process p2)
{
    return p1.pid < p2.pid;
}

int main() {

    int number_Of_Processes = 0;
    float quantum = 0.0; //time quantum (only for Round Robin)
    struct process p[100];
    auto average_tat_RR = 0.0; //turnaround time
    auto average_wt_RR = 0.0; //waiting time
    auto average_rt_RR = 0.0; //response time
    auto cpu_utilization = 0.0;
    auto total_tat_RR = 0;
    auto total_wt_RR = 0;
    auto total_rt_RR = 0;
    auto total_it_RR = 0; //idle time
    auto throughput = 0.0;
    int burst_remaining[100]{};
    int idx;

    std::cout << std::setprecision(2) << std::fixed;

    std::cout << "Enter the number of processes: ";
    std::cin >> number_Of_Processes;
    std::cout << "Enter time quantum: ";
    std::cin >> quantum;

    for (int i = 0; i < number_Of_Processes; i++) {
        std::cout << "Enter arrival time of process " << i + 1 << ": ";
        std::cin >> p[i].at_RR;
        std::cout << "Enter burst time of process " << i + 1 << ": ";
        std::cin >> p[i].bt_RR;
        burst_remaining[i] = p[i].bt_RR;
        p[i].pid = i + 1;
        std::cout << std::endl;
    }

    std::sort(p, p + number_Of_Processes, compare1);

    std::queue<int> q;
    int current_time = 0;
    q.push(0);
    int completed = 0;
    int mark[100];
    memset(mark, 0, sizeof(mark));
    mark[0] = 1;

    while (completed != number_Of_Processes) {
        idx = q.front();
        q.pop();

        if (burst_remaining[idx] == p[idx].bt_RR) {
            p[idx].st_RR = std::max(current_time, p[idx].at_RR);
            total_it_RR += p[idx].st_RR - current_time;
            current_time = p[idx].st_RR;
        }

        if (burst_remaining[idx] - quantum > 0) {
            burst_remaining[idx] -= quantum;
            current_time += quantum;
        }
        else {
            current_time += burst_remaining[idx];
            burst_remaining[idx] = 0;
            completed++;

            p[idx].ct_RR = current_time;
            p[idx].tat_RR = p[idx].ct_RR - p[idx].at_RR;
            p[idx].wt_RR = p[idx].tat_RR - p[idx].bt_RR;
            p[idx].rt_RR = p[idx].st_RR - p[idx].at_RR;

            total_tat_RR += p[idx].tat_RR;
            total_wt_RR += p[idx].wt_RR;
            total_rt_RR += p[idx].rt_RR;
        }

        for (int i = 1; i < number_Of_Processes; i++) {
            if (burst_remaining[i] > 0 && p[i].at_RR <= current_time && mark[i] == 0) {
                q.push(i);
                mark[i] = 1;
            }
        }
        if (burst_remaining[idx] > 0) {
            q.push(idx);
        }

        if (q.empty()) {
            for (int i = 1; i < number_Of_Processes; i++) {
                if (burst_remaining[i] > 0) {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }


    }

    average_tat_RR = (float)total_tat_RR / number_Of_Processes;
    average_wt_RR = (float)total_wt_RR / number_Of_Processes;
    average_rt_RR = (float)total_rt_RR / number_Of_Processes;
    cpu_utilization = ((p[number_Of_Processes - 1].ct_RR - total_it_RR) / (float)p[number_Of_Processes - 1].ct_RR) * 100;
    throughput = float(number_Of_Processes) / (p[number_Of_Processes - 1].ct_RR - p[0].at_RR);

    std::sort(p, p + number_Of_Processes, compare2);

    std::cout << std::endl;
    std::cout << "#P\t" << "Arrival\t" << "Burst\t"<< "Turnaround\t" << std::endl;

    for (int i = 0; i < number_Of_Processes; i++) {
        std::cout << p[i].pid << "\t" << p[i].at_RR << "\t" << p[i].bt_RR << "\t" << p[i].tat_RR << "\t" << "\n" << std::endl;
    }
    std::cout << "Average Turnaround Time = " << average_tat_RR << std::endl;
    std::cout << "CPU Utilization = " << cpu_utilization << "%" << std::endl;
    std::cout << "Throughput = " << throughput << " process/second" << std::endl;
}