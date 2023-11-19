#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>

using namespace std;
deque<deque<int>> read_list(string filename) {
    deque<deque<int>> list_of_processes;
    ifstream my_file(filename);
    string curr_line;

    if (!my_file.is_open()) {
        std::cerr << "Error opening the CSV file." << std::endl;
        exit(1); // Return an error code
    }

    while (getline(my_file, curr_line)) {
        deque<int> row;
        stringstream lineStream(curr_line);
        string cell;
        while (getline(lineStream, cell, ',')) {
             int value = atoi(cell.c_str());
             row.push_back(value);
        }
        list_of_processes.push_back(row);
    }
    my_file.close();

    return list_of_processes;
}

bool sort_by_arrival(const deque<int>& a, const deque<int>& b) {
    return a[1] < b[1];
}

bool sort_by_burst(const deque<int>& a, const deque<int>& b) {
    return a[2] < b[2];
}

bool sort_by_priority(const deque<int>& a, const deque<int>& b) {
    return a[3] < b[3];
}

double* schedule_fcfs(deque<deque<int>> processes) {
    sort(processes.begin(), processes.end(), sort_by_arrival);
    double* array = new double[3];
    deque<int> turnaround;
    deque<int> wait;
    int wait_time = 0;
    int submission = 0;
    int completion = 0;
    int total_burst = 0;

    for (const auto& row: processes) {
        wait.push_back(wait_time-row.at(1));
        wait_time += row.at(2);

        submission = row.at(1);
        completion += row.at(2);
        turnaround.push_back(completion - submission);

        total_burst += row.at(2);
    }

    double turnaround_sum = accumulate(turnaround.begin(), turnaround.end(), 0);
    array[0] = turnaround_sum / processes.size();
    double wait_sum = accumulate(wait.begin(), wait.end(), 0);
    array[1] = wait_sum / processes.size();
    array[2] = static_cast<double>(processes.size()) / total_burst;
    return array;
}

double* schedule_sjfp(deque<deque<int>> processes) {
    int original_size = processes.size();
    sort(processes.begin(), processes.end(), sort_by_arrival);
    double* array = new double[3];
    deque<int> turnaround;
    deque<int> wait;
    int wait_time = processes.front().at(1);
    int total_burst = 0;
    int curr_start = 0;
    int next_start = processes.at(0).at(1);
    deque<deque<int>> temp_array;

    while (!processes.empty()) {
        while (true) {
            if (processes.empty()) break;
            if (processes.at(0).at(1) > next_start) {
                curr_start = next_start;
                next_start = processes.front().at(1);
                break;
            }
            processes.front().push_back(processes.front().at(2));
            temp_array.push_back(processes.front());
            total_burst += processes.front().at(2);
            processes.pop_front();
        }
        sort(temp_array.begin(), temp_array.end(), sort_by_burst);

        for (int i = curr_start; i < next_start; i++) {
            if (temp_array.empty()) break;
            wait_time++;
            temp_array.front().at(2)--;
            if (temp_array.front().at(2) == 0) {
                turnaround.push_back(wait_time - temp_array.front().at(1));
                wait.push_back(wait_time - temp_array.front().at(4) - temp_array.front().at(1));
                temp_array.pop_front();
            }
        }
    }

    for (const auto& row: temp_array) {
        wait_time += row.at(2);
        wait.push_back(wait_time - row.at(4) - row.at(1));
        turnaround.push_back(wait_time - row.at(1));
    }

    double turnaround_sum = accumulate(turnaround.begin(), turnaround.end(), 0);
    array[0] = turnaround_sum / original_size;
    double wait_sum = accumulate(wait.begin(), wait.end(), 0);
    array[1] = wait_sum / original_size;
    array[2] = static_cast<double>(original_size) / total_burst;

    return array;
}

double* schedule_priority(deque<deque<int>> processes) {
    int original_size = processes.size();
    sort(processes.begin(), processes.end(), sort_by_arrival);
    double* array = new double[3];
    deque<int> turnaround;
    deque<int> wait;
    int wait_time = processes.front().at(1);
    int total_burst = 0;
    int curr_start = 0;
    int next_start = processes.at(0).at(1);
    deque<deque<int>> temp_array;

    while (!processes.empty()) {
        while (true) {
            if (processes.empty()) break;
            if (processes.at(0).at(1) > next_start) {
                curr_start = next_start;
                next_start = processes.front().at(1);
                break;
            }
            processes.front().push_back(processes.front().at(2));
            temp_array.push_back(processes.front());
            total_burst += processes.front().at(2);
            processes.pop_front();
        }
        sort(temp_array.begin(), temp_array.end(), sort_by_priority);

        for (int i = curr_start; i < next_start; i++) {
            if (temp_array.empty()) break;
            wait_time++;
            temp_array.front().at(2)--;
            if (temp_array.front().at(2) == 0) {
                turnaround.push_back(wait_time - temp_array.front().at(1));
                wait.push_back(wait_time - temp_array.front().at(4) - temp_array.front().at(1));
                temp_array.pop_front();
            }
        }
    }

    for (const auto& row: temp_array) {
        wait_time += row.at(2);
        wait.push_back(wait_time - row.at(4) - row.at(1));
        turnaround.push_back(wait_time - row.at(1));
    }

    double turnaround_sum = accumulate(turnaround.begin(), turnaround.end(), 0);
    array[0] = turnaround_sum / original_size;
    double wait_sum = accumulate(wait.begin(), wait.end(), 0);
    array[1] = wait_sum / original_size;
    array[2] = static_cast<double>(original_size) / total_burst;

    return array;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments.\n");
        return -1;
    }

    deque<deque<int>> list_of_processes = read_list(argv[1]);

    double* fcfs = schedule_fcfs(list_of_processes);
    printf("--- FCFS ---\n");
    printf("Average Turnaround Time: %.3f\n"
            "Average Waiting Time: %.3f\n"
            "Throughput: %.3f\n\n",
            fcfs[0], fcfs[1], fcfs[2]);
    delete[] fcfs;
    double* sjfp = schedule_sjfp(list_of_processes);
    printf("--- SJFP ---\n");
    printf("Average Turnaround Time: %.3f\n"
            "Average Waiting Time: %.3f\n"
            "Throughput: %.3f\n\n",
            sjfp[0], sjfp[1], sjfp[2]);
    delete[] sjfp;
    double* priority = schedule_priority(list_of_processes);
    printf("--- Priority ---\n");
    printf("Average Turnaround Time: %.3f\n"
            "Average Waiting Time: %.3f\n"
            "Throughput: %.3f\n\n",
            priority[0], priority[1], priority[2]);
    delete[] priority;
            
    return 0;
}
