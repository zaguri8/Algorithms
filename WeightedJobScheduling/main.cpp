

#include <iostream>
#include <algorithm>
#include <random>


/*
    Interval struct
*/
struct Interval {
    int start;
    int end;
    int weight;
    Interval(int start, int end, int weight): start(start), end(end), weight(weight) {}
    Interval(): start(0), end(0), weight(0) { }
    bool intersect(Interval& other) {
        if (other.start >= start && other.end <= end
            || start >= other.start && end <= other.end)
            return true;
        return false;
    }
};


/*
    Comparator for sorting intervals by end time
*/
class IntervalComparator {
public:
    IntervalComparator(Interval& i1): i1(i1) { }
    bool operator()(Interval& other) const {
        return  i1.end < other.end;
    }
private:
    Interval& i1;
};




int max(int x, int y) {
    return x > y ? x : y;
}



int partition(std::vector<Interval>& in, int begin, int mid, int end) {
    Interval pivot = in[end];
    int i = begin;
    int j = begin - 1;
    for (; i <= end; i++) {
        if (IntervalComparator(in[i])(pivot)) {
            j++;
            std::iter_swap(in.begin() + i, in.begin() + j);
        }
    }
    j = j + 1;
    std::iter_swap(in.begin() + j, in.begin() + end);
    return j;
}

void quickSort(std::vector<Interval>& in, int begin, int end) {
    if (begin < end) {
        int mid = (begin + end) / 2;
        int p = partition(in, begin, mid, end);
        quickSort(in, begin, p - 1);
        quickSort(in, p + 1, end);
    }
}

/*
    Find best weight for interval j
*/
int bestWeight(std::vector<int>& P, std::vector<int>& M, std::vector<Interval>& intervals, int j) {
    if (j < 0)
        return 0;
    else if (M[j] != -1) {
        return M[j];
    }
    else {
        M[j] = max(intervals[j].weight + bestWeight(P, M, intervals, P[j]), bestWeight(P, M, intervals, j - 1));
        return M[j];
    }
}


/*
    Find P for interval i
*/
int findP(const std::vector<Interval>& intervals, int i) {
    int left = 0;
    int right = i - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (intervals[mid].end < intervals[i].start) {
            if (intervals[mid + 1].end < intervals[i].start) {
                left = mid + 1;
            }
            else {
                return mid;
            }
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}


/*
    Find P for each interval
*/
std::vector<int> findAllP(const std::vector<Interval>& intervals) {
    std::vector<int> P(intervals.size(), -1);

    for (int i = 1; i < intervals.size(); ++i) {
        P[i] = findP(intervals, i);
    }
    return P;
}

/*
    Generate n random intervals
*/
std::vector<Interval> generateIntervals(int n) {
    std::vector<Interval> intervals;
    // random gen
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);

    for (int i = 0; i < n; i++) {
        int start = dis(gen);
        int end = dis(gen);
        int weight = 25 + dis(gen);
        if (start > end) {
            std::swap(start, end);
        }
        intervals.push_back(Interval(start, end, weight));
    }
    return intervals;
}

/*
    Reconstruct solution out of M and P
*/
std::vector<Interval> reconstructSolution(std::vector<int>& M, std::vector<int>& P, std::vector<Interval>& intervals, int j) {
    std::vector<Interval> solution;
    while (j >= 0) {
        if (M[j] == -1 || intervals[j].weight + M[P[j]] > M[j - 1]) {
            solution.push_back(intervals[j]);
            j = P[j];
        }
        else {
            j = j - 1;
        }
    }
    return solution;
}

/*
    1. Generate intervals
    2. Sort intervals by end time
    3. Find P
    4. Find M
    5. Reconstruct solution
*/
int main() {

    // 10 intervals
    std::vector<Interval> intervals = generateIntervals(10);
    quickSort(intervals, 0, intervals.size() - 1);
    std::vector<int> P = findAllP(intervals);
    std::vector<int> M(intervals.size(), -1);

    // print intervals
    for (int i = 0; i < intervals.size(); i++) 
        std::cout << intervals[i].start << " " << intervals[i].end << " " << intervals[i].weight << std::endl;
    

    // print Best weight
    std::cout << "Best weight: " << bestWeight(P, M, intervals, intervals.size() - 1) << std::endl;
    std::vector<Interval> solution = reconstructSolution(M, P, intervals, intervals.size() - 1);
    // print Solution
    std::cout << "Solution: " << std::endl;
    for (int i = 0; i < solution.size(); i++) {
        std::cout << solution[i].start << " " << solution[i].end << " " << solution[i].weight << std::endl;
    }

    return 0;
}