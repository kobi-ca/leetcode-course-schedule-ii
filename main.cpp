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

    static std::vector<int> sort(const std::vector<int> &input,
                                 const std::vector<std::vector<int>>& prereq_to_course) {
        std::vector<int> out;
        for(const int val : input) {
            if (out.empty()) {
                out.push_back(val);
                continue;
            }
            for(auto iter = out.begin(); iter != out.end(); ++iter) {
                const auto curr = *iter;
                if (std::find(prereq_to_course[val].begin(),
                              prereq_to_course[val].end(), curr) != prereq_to_course[val].end()) {
                    iter = out.insert(iter, val);
                }
            }
        }
        return out;
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
            if (find_cycles(numCourses, prereq_to_course, start)) {
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
            auto toinsert = prereq_to_course[b];
            toinsert = sort(toinsert, prereq_to_course);
            queue.insert(std::end(queue), toinsert.begin(), toinsert.end());
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

    {
        std::clog << "{5,8},{3,5},{1,9},{4,5},{0,2},{7,8},{4,9} : ";
        std::vector<std::vector<int>> in{{5,8},{3,5},{1,9},{4,5},{0,2},{7,8},{4,9}};
        const auto result = Solution::findOrder(10, in);
        print(result);
    }

    std::clog << "done\n";
    return 0;
}
