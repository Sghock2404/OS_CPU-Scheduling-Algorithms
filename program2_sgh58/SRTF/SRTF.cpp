/*
*SRTF.cpp
*Scott Hockaday
*Course: Operating Systems CS4328
*project 2
*due: 11/11/21
*/


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstring>


struct process {
    int pid;
    int at_SRTF; //arrival time
    int bt_SRTF; //burst time
    int st_SRTF; //start time
    int ct_SRTF; //completion time
    int tat_SRTF; //turnaround time
    int wt_SRTF; //waiting time
    int rt_SRTF; //response time
};

int main() {

    int number_Of_Processes;
    struct process p[100];
    auto average_tat_SRTF = 0.0;
    auto average_wt_SRTF = 0.0;
    auto average_rt_SRTF = 0.0;
    auto cpu_utilization_SRTF = 0.0;
    int total_tat_SRTF = 0;
    int total_wt_SRTF = 0;
    int total_rt_SRTF = 0;
    int total_it_SRTF = 0;
    float throughput;
    int burst_remaining[100]{};
    int is_completed[100];
    memset(is_completed, 0, sizeof(is_completed));

    std::cout << std::setprecision(2) << std::fixed;

    std::cout << "Enter the number of processes: ";
    std::cin >> number_Of_Processes;

    for (int i = 0; i < number_Of_Processes; i++) {
        std::cout << "Enter arrival time of process " << i + 1 << ": ";
        std::cin >> p[i].at_SRTF;
        std::cout << "Enter burst time of process " << i + 1 << ": ";
        std::cin >> p[i].bt_SRTF;
        p[i].pid = i + 1;
        burst_remaining[i] = p[i].bt_SRTF;
        std::cout << std::endl;
    }

    int current_time = 0;
    int completed = 0;
    int prev = 0;

    while (completed != number_Of_Processes) {
        int index = -1;
        int minimum = 10000;
        for (int i = 0; i < number_Of_Processes; i++) {
            if (p[i].at_SRTF <= current_time && is_completed[i] == 0) {
                if (burst_remaining[i] < minimum) {
                    minimum = burst_remaining[i];
                    index = i;
                }
                if (burst_remaining[i] == minimum) {
                    if (p[i].at_SRTF < p[index].at_SRTF) {
                        minimum = burst_remaining[i];
                        index = i;
                    }
                }
            }
        }

        if (index != -1) {
            if (burst_remaining[index] == p[index].bt_SRTF) {
                p[index].st_SRTF = current_time;
                total_it_SRTF += p[index].st_SRTF - prev;
            }
            burst_remaining[index] -= 1;
            current_time++;
            prev = current_time;

            if (burst_remaining[index] == 0) {
                p[index].ct_SRTF = current_time;
                p[index].tat_SRTF = p[index].ct_SRTF - p[index].at_SRTF;
                p[index].wt_SRTF = p[index].tat_SRTF - p[index].bt_SRTF;
                p[index].rt_SRTF = p[index].st_SRTF - p[index].at_SRTF;

                total_tat_SRTF += p[index].tat_SRTF;
                total_wt_SRTF += p[index].wt_SRTF;
                total_rt_SRTF += p[index].rt_SRTF;

                is_completed[index] = 1;
                completed++;
            }
        }
        else {
            current_time++;
        }
    }

    int min_at = 10000000; //minimum arrival time
    int max_ct = -1; //maximum completion time
    for (int i = 0; i < number_Of_Processes; i++) {
        min_at = std::min(min_at, p[i].at_SRTF);
        max_ct = std::max(max_ct, p[i].ct_SRTF);
    }

    average_tat_SRTF = (float)total_tat_SRTF / number_Of_Processes;
    average_wt_SRTF = (float)total_wt_SRTF / number_Of_Processes;
    average_rt_SRTF = (float)total_rt_SRTF / number_Of_Processes;
    cpu_utilization_SRTF = ((max_ct - total_it_SRTF) / (float)max_ct) * 100;
    throughput = float(number_Of_Processes) / (max_ct - min_at);

    std::cout << std::endl << std::endl;

    std::cout << "#P\t" << "Arrival\t" << "Burst\t" << "Turnaround\t" << std::endl;

    for (int i = 0; i < number_Of_Processes; i++) {
        std::cout << p[i].pid << "\t" << p[i].at_SRTF << "\t" << p[i].bt_SRTF << "\t" << p[i].tat_SRTF << "\t" << std::endl;
    }
    std::cout << "Average Turnaround Time = " << average_tat_SRTF << std::endl;
    std::cout << "CPU Utilization = " << cpu_utilization_SRTF << "%" << std::endl;
    std::cout << "Throughput = " << throughput << " process/second" << std::endl;
}