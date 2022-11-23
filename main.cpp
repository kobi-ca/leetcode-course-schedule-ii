#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>

class Solution {
public:

    static bool find_cycles(const int total,
                            const std::vector<std::vector<int>> &course_to_its_prereq,
                            const int start) {
        std::vector<int> visited(total);
        return find_cycles(visited, start, course_to_its_prereq);
    }
    static bool find_cycles(std::vector<int> visited,
                            const int val,
                            const std::vector<std::vector<int>> &course_to_its_prereq) {
        visited[val] = 1;
        const auto &childs = course_to_its_prereq[val];
        if (childs.empty()) {
            return false;
        }
        for (const auto v: childs) {
            if (visited[v]) {
                return true;
            }
            if (find_cycles(visited, v, course_to_its_prereq)) {
                return true;
            }
        }
        return false;
    }

    static void build_output(const int val,
                             std::vector<int>& visited,
                             std::vector<int>& out,
                             const std::vector<std::vector<int>>& prererq_to_course) {
        if (prererq_to_course[val].empty()){
            out.push_back(val);
            visited[val] = 1;
            return;
        }
        const auto& neighbours = prererq_to_course[val];
        for(const auto n : neighbours) {
            if(visited[n]) {
                continue;
            }
            build_output(n, visited, out, prererq_to_course);
        }
        out.push_back(val);
        visited[val] = 1;
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
            course_to_its_prereq[course].push_back(prereq);
        }

        for (int idx = 0; idx < numCourses; ++idx) {
            if (find_cycles(numCourses, prereq_to_course, idx)) {
                return {};
            }
        }

        std::vector<int> out;
        std::vector<int> visited(numCourses);

        int idx{};
        for(const auto& p : course_to_its_prereq) {
            if (p.empty()) {
                build_output(idx, visited, out, prereq_to_course);
            }
            ++idx;
        }

        return {out.rbegin(), out.rend()};
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
        std::clog << "{1, 0}, {2, 0}, {3, 1}, {3, 2} : ";
        std::vector<std::vector<int>> in{{1, 0},
                                         {2, 0},
                                         {3, 1},
                                         {3, 2}};
        const auto result = Solution::findOrder(4, in);
        print(result);
    }
    {
        std::clog << "{1, 0} : ";
        std::vector<std::vector<int>> in{{1, 0}};
        const auto result = Solution::findOrder(2, in);
        print(result);
    }
    {
        std::clog << "[] : ";
        std::vector<std::vector<int>> in;
        const auto result = Solution::findOrder(2, in);
        print(result);
    }
    {
        std::clog << "[] : ";
        std::vector<std::vector<int>> in;
        const auto result = Solution::findOrder(1, in);
        print(result);
    }
    {
        std::clog << "{0,1}, {1,0} : ";
        std::vector<std::vector<int>> in{{0,1}, {1,0}};
        const auto result = Solution::findOrder(2, in);
        print(result);
    }
    {
        std::clog << "{1,0} : ";
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
        std::clog << "{1,0}, {2,0}, {3,1}, {3, 2} : ";
        std::vector<std::vector<int>> in{{1,0}, {2,0}, {3,1}, {3, 2}};
        const auto result = Solution::findOrder(4, in);
        print(result);
    }
    {
        std::clog << "{1,0}, {1,2}, {0,1} : ";
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

    {
        std::clog << "{5,8},{3,5},{1,9},{4,5},{0,2},{7,8},{4,9} : ";
        std::vector<std::vector<int>> in{{5,8},{3,5},{1,9},{4,5},{0,2},{7,8},{4,9}};
        const auto result = Solution::findOrder(10, in);
        print(result);
    }

    {
        std::clog << "{1,0},{2,6},{1,7},{5,1},{6,4},{7,0},{0,5} : ";
        std::vector<std::vector<int>> in{{1,0},{2,6},{1,7},{5,1},{6,4},{7,0},{0,5}};
        const auto result = Solution::findOrder(8, in);
        print(result);
    }

    {
        std::clog << "{5,6},{0,2},{1,7},{5,9},{1,8},{3,4},{0,6},{0,7},{0,3},{8,9} : ";
        std::vector<std::vector<int>> in{{5,6},{0,2},{1,7},{5,9},{1,8},{3,4},{0,6},{0,7},{0,3},{8,9}};
        const auto result = Solution::findOrder(10, in);
        print(result);
    }

    {
        std::clog << "{3,0}, {0,1} : ";
        std::vector<std::vector<int>> in{{3,0}, {0,1}};
        const auto result = Solution::findOrder(4, in);
        print(result);
    }

    {
        std::clog << "{1,0},{0,3},{0,2},{3,2},{2,5},{4,5},{5,6},{2,4} : ";
        std::vector<std::vector<int>> in{{1,0},{0,3},{0,2},{3,2},{2,5},{4,5},{5,6},{2,4}};
        const auto result = Solution::findOrder(7, in);
        print(result);
    }

    std::clog << "done\n";
    return 0;
}
