#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void calculate_stats(vector<int>& numbers)
{
    sort(numbers.begin(), numbers.end());
    double mean, median, mode;
    size_t size = numbers.size();

    //calculate mean
    double sum = 0.0;
    for (int value: numbers) {
        sum+=value;
    }
    mean = sum / size;
    //calculate median
    if (size % 2 == 0) median = (numbers[size/2-1] + numbers[size/2]) / 2.0;
    else median = numbers[size/2];
    //calculate mode
    int num = numbers[0];
    mode = num;
    int count = 1;
    int count_mode = 1;

    for (size_t i = 1; i < size; i++) {
        if (numbers[i] == num) count++;
        else {
            if (count > count_mode) {
                count_mode = count;
                mode = num;
            }
            count = 1;
            num = numbers[i];
        }
    }

    //print all stats
    cout << "Data: ";
    for (size_t i = 0; i < size - 1; i++) {
        cout << numbers[i] << ", ";
    }
    cout << numbers[size-1];
    cout << endl;
    cout << "Mean: " << mean << endl;
    cout << "Median: " << median << endl;
    cout << "Mode: " << mode << endl;
}

int main(int argc, char **argv)
{
    vector<int> user_numbers;
    bool c_flag = false;
    bool f_flag = false;
    int input_option;

    if (argc <= 1) {
        string input;
        while (true) {
            cout << "Please enter an integer value or 'q' to quit: " << endl;
            cin >> input;
            if (input == "q") break;
            try {
                int num = stoi(input);
                user_numbers.push_back(num);
            }
            catch (const invalid_argument& e) {
                throw(invalid_argument("The input must be a integer"));
            }
        }
        if (user_numbers.empty()) throw(invalid_argument("You must enter at least 1 number"));
        calculate_stats(user_numbers);

        return 0;
    }

    while ((input_option = getopt(argc, argv, "cf")) != -1) {
        switch (input_option) {
            case 'c':
                if (f_flag) {
                    throw(invalid_argument("-c and -f are mutually exclusive"));
                    return 1;
                }
                c_flag = true;
                break;
            case 'f':
                if (c_flag) {
                    throw(invalid_argument("-c and -f are mutually exclusive"));
                    return 1;
                }
                f_flag = true;
                break;
            case '?':
                throw(invalid_argument("Unknown option"));
                break;
            default:
                return 1;
        }
    }//end while

    if (c_flag) {
        for (int index = optind; index < argc; index++) {
            try {
                int num = stoi(argv[index]);
                user_numbers.push_back(num);
            }
            catch (const invalid_argument& e) {
                throw(invalid_argument("The input must be a integer"));
            }
        }
        if (user_numbers.empty()) throw(invalid_argument("You must enter at least 1 number"));
        calculate_stats(user_numbers);
    }
    else if (f_flag) {
        string val_read;
        string line_read;
        ifstream file_name(argv[optind]);

        if (!file_name.is_open()) {
            throw(runtime_error("File could not be opened"));
        }

        while (getline(file_name, line_read)) {
            stringstream ss(line_read);
            string token;
            while (getline(ss, token, ',')) {
                try {
                    int num = stoi(token);
                    user_numbers.push_back(num);
                }
                catch (const invalid_argument& e) {
                    throw(invalid_argument("The input must be a integer"));
                }
            }
        }
        if (user_numbers.empty()) throw(invalid_argument("No numbers read from file"));
        calculate_stats(user_numbers);
    }
}
