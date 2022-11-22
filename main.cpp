#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <cassert>

class Solution {
public:
    static void swap_vals(std::vector<int> &out, int x, int y) {
        auto iterx = std::find(out.begin(), out.end(), x);
        auto itery = std::find(out.begin(), out.end(), y);
        assert(iterx != out.end() && itery != out.end());
        using std::iter_swap;
        iter_swap(iterx, itery);
    }

    static bool find_cycles(const std::vector<std::vector<int>> &course_to_its_prereq,
                            const int start) {
        std::vector<int> fifo; fifo.push_back(start);
        std::vector<int> visited(course_to_its_prereq.size());
        // BFS
        while(!fifo.empty()){
            const auto val = fifo.front();
            if (visited[val]){
                return true;
            }
            visited[val] = 1;
            const auto& toinsert = course_to_its_prereq[val];
            for(const auto v : toinsert) {
                if (visited[v]) {
                    return true;
                }
                if (!visited[v] &&
                    std::find(fifo.cbegin(), fifo.cend(), v) == std::end(fifo)) {
                    fifo.push_back(v);
                }
            }
            fifo.erase(std::begin(fifo));
        }
        return false;
    }

    static std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        if (prerequisites.empty()) {
            std::vector<int> out(numCourses);
            std::generate(out.begin(), out.end(),
                          [i = numCourses - 1] () mutable { return i--; });
            return out;
        }
        std::vector<std::vector<int>> prereq_to_course(numCourses);
        std::vector<std::vector<int>> course_to_its_prereq(numCourses);
        for(const auto& v : std::as_const(prerequisites)) {
            const auto prereq = v[1];
            const auto course  = v[0];
            prereq_to_course[prereq].push_back(course);
            auto &course_to_its_prereq_ref = course_to_its_prereq[prereq];
            // find cycles
            if (std::find(course_to_its_prereq_ref.begin(),
                          course_to_its_prereq_ref.end(),
                          course) != std::end(course_to_its_prereq_ref)) {
                return {};
            }
            course_to_its_prereq[course].push_back(prereq);
        }

        std::vector<int> out;
        std::vector<int> visited(numCourses);
        std::vector<int> queue;
        int idx{};
        for(const auto& v : std::as_const(course_to_its_prereq)) {
            if (v.empty()) {
                queue.push_back(idx);
            }
            ++idx;
        }
        for(const auto start : std::as_const(queue)) {
            if (find_cycles(prereq_to_course, start)) {
                return {};
            }
        }
        // not a single one with a zero prereq?
        if (queue.empty()) {
            return out;
        }
        while(!queue.empty()) {
            const auto b = queue.back();
            queue.pop_back();
            if (visited[b]) {
                continue;
            }
            out.push_back(b);
            visited[b] = 1;
            const auto toinsert = prereq_to_course[b];
            for(const auto val : std::as_const(toinsert)) {
                if (visited[val]) {
                    swap_vals(out, val, b);
                } else if (std::end(queue) == std::find(queue.cbegin(), queue.cend(), val)) {
                        queue.push_back(val);
                }
            }
        }

        return out;
    }
};

namespace {
    void print(const std::vector<int>& result) {
        if (result.empty()) {
            std::clog << "[]\n";
            return;
        }
        std::copy(std::cbegin(result),
                  std::cend(result),
                  std::ostream_iterator<int>(std::clog, ","));
        std::clog << '\n';
    }
}

int main() {
    {
        std::vector<std::vector<int>> in{{1, 0},
                                         {2, 0},
                                         {3, 1},
                                         {3, 2}};
        const auto result = Solution::findOrder(4, in);
        print(result);
    }
    {
        std::vector<std::vector<int>> in{{1, 0}};
        const auto result = Solution::findOrder(2, in);
        print(result);
    }
    {
        std::vector<std::vector<int>> in;
        const auto result = Solution::findOrder(2, in);
        print(result);
    }
    {
        std::vector<std::vector<int>> in;
        const auto result = Solution::findOrder(1, in);
        print(result);
    }
    {
        std::vector<std::vector<int>> in{{0,1}, {1,0}};
        const auto result = Solution::findOrder(2, in);
        print(result);
    }
    {
        std::vector<std::vector<int>> in{{1,0}};
        const auto result = Solution::findOrder(3, in);
        print(result);
    }
    {
        std::clog << "{0,1}, {0,2}, {1,2} : ";
        std::vector<std::vector<int>> in{{0,1}, {0,2}, {1,2}};
        const auto result = Solution::findOrder(3, in);
        print(result);
    }
    {
        std::vector<std::vector<int>> in{{1,0}, {2,0}, {3,1}, {3, 2}};
        const auto result = Solution::findOrder(4, in);
        print(result);
    }
    {
        std::vector<std::vector<int>> in{{1,0}, {1,2}, {0,1}};
        const auto result = Solution::findOrder(3, in);
        print(result);
    }
    {
        std::clog << "{0,1}, {0,2}, {1,3}, {3,0} : ";
        std::vector<std::vector<int>> in{{0,1}, {0,2}, {1,3}, {3,0}};
        const auto result = Solution::findOrder(4, in);
        print(result);
    }
    std::clog << "done\n";
    return 0;
}
