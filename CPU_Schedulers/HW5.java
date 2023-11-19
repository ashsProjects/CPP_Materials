import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Deque;
import java.util.LinkedList;
import java.util.List;

public class HW5 {
    public static void sortDequeByArrival(Deque<ArrayList<Integer>> data) {
        List<ArrayList<Integer>> dataList = new ArrayList<>(data);

        Collections.sort(dataList, new Comparator<ArrayList<Integer>>() {
            @Override
            public int compare(ArrayList<Integer> row1, ArrayList<Integer> row2) {
                // Assuming there are at least two columns in each row
                Integer value1 = row1.size() > 1 ? row1.toArray(new Integer[0])[1] : 0;
                Integer value2 = row2.size() > 1 ? row2.toArray(new Integer[0])[1] : 0;
                return value1.compareTo(value2);
            }
        });

        data.clear();
        data.addAll(dataList);
    }

    public static void sortDequeByPriority(Deque<ArrayList<Integer>> data) {
        List<ArrayList<Integer>> dataList = new ArrayList<>(data);

        Collections.sort(dataList, new Comparator<ArrayList<Integer>>() {
            @Override
            public int compare(ArrayList<Integer> row1, ArrayList<Integer> row2) {
                // Assuming there are at least two columns in each row
                Integer value1 = row1.size() > 1 ? row1.toArray(new Integer[0])[3] : 0;
                Integer value2 = row2.size() > 1 ? row2.toArray(new Integer[0])[3] : 0;
                return value1.compareTo(value2);
            }
        });

        data.clear();
        data.addAll(dataList);
    }

    public static Deque<ArrayList<Integer>> readCSVToDeque(String filename) throws IOException {
        Deque<ArrayList<Integer>> data = new LinkedList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] values = line.split(",");
                ArrayList<Integer> row = new ArrayList<>();
                for (String value : values) {
                    row.add(Integer.parseInt(value));
                }
                data.add(row);
            }
        }

        return data;
    }

    static double[] schedule_priority(Deque<ArrayList<Integer>> processes) {
        int original_size = processes.size();
        sortDequeByArrival(processes);
        double stats[] = new double[3];
        Deque<Integer> turnaround = new LinkedList<>();
        Deque<Integer> wait = new LinkedList<>();
        Integer wait_time = processes.peek().get(1);
        int total_burst = 0;
        int curr_start = 0;
        int next_start = processes.peek().get(1);
        Deque<ArrayList<Integer>> temp_array = new LinkedList<>();

        while (!processes.isEmpty()) {
            while (true) {
                if (processes.isEmpty()) break;
                if (processes.peek().get(1) > next_start) {
                    curr_start = next_start;
                    next_start = processes.peek().get(1);
                    break;
                }
                processes.getFirst().add(processes.getFirst().get(2));
                temp_array.push(processes.peek());
                total_burst += processes.peek().get(2);
                processes.pop();
            }
            sortDequeByPriority(temp_array);
            for (int i = curr_start; i < next_start; i++) {
                if (temp_array.isEmpty()) break;
                wait_time++;
                temp_array.peek().set(2, temp_array.peek().get(2)-1);
                if (temp_array.peek().get(2) == 0) {
                    turnaround.push(wait_time - temp_array.peek().get(1));
                    wait.push(wait_time - temp_array.peek().get(4) - temp_array.peek().get(1));
                    temp_array.removeFirst();
                }
            }
        }

        for (ArrayList<Integer> row: temp_array) {
            wait_time += row.get(2);
            wait.push(wait_time - row.get(4) - row.get(1));
            turnaround.push(wait_time - row.get(1));
        }

        double turnaround_sum = turnaround.stream().mapToInt(Integer::intValue).sum();
        stats[0] = turnaround_sum / original_size;
        double wait_sum = wait.stream().mapToInt(Integer::intValue).sum();
        stats[1] = wait_sum / original_size;
        stats[2] = ((double) original_size) / total_burst;

        return stats;

    }

    public static void main(String[] args) throws IOException{
        String filename = "test.csv"; // Replace with the actual CSV file path
        Deque<ArrayList<Integer>> csvData = readCSVToDeque(filename);

        double priority[] = schedule_priority(csvData);
        System.out.println(String.format("Turnaround: %.3f\nWaiting: %.3f\nThroughput: %.3f", priority[0], priority[1], priority[2]));
        
    }
    
}