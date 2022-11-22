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

    static std::vector<int> sort(const std::vector<int> &input,
                                 const std::vector<std::vector<int>>& prereq_to_course) {
        std::vector<int> out(input.begin(), input.end());
        for (auto iter = out.begin(); iter != out.end(); ++iter) {
            const auto curr = *iter;
            for (auto inner = std::next(iter); inner != out.end(); ++inner) {
                const auto inner_val = *inner;
                if (std::find(prereq_to_course[inner_val].begin(),
                              prereq_to_course[inner_val].end(), curr) !=
                              prereq_to_course[inner_val].end()) {
                    std::iter_swap(iter, inner);
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

        std::vector<int> out(numCourses);
        std::vector<int> visited(numCourses);
        int idx{};
        auto lower_iter = out.begin();
        for(const auto& v : std::as_const(course_to_its_prereq)) {
            if (v.empty()) {
                *lower_iter = idx;
                visited[idx] = 1;
                ++lower_iter;
            }
            ++idx;
        }
        idx = 0;
        auto upper_iter = out.rbegin();
        for(const auto& v : std::as_const(prereq_to_course)) {
            if (v.empty()) {
                *upper_iter = idx;
                visited[idx] = 1;
                ++upper_iter;
            }
            ++idx;
        }
        //for(const auto start : std::as_const(queue)) {
        for (int idx = 0; idx < numCourses; ++idx) {
            if (find_cycles(numCourses, prereq_to_course, idx)) {
                return {};
            }
        }
        while(std::any_of(visited.begin(), visited.end(),
                          [](const int i){ return i == 0;})) {
            auto not_visited = std::find(visited.begin(), visited.end(), 0);
            if (not_visited == visited.end()) {
                return out;
            }
            std::vector<int> queue;
            queue.push_back(std::distance(visited.begin(), not_visited));
            while(!queue.empty()) {
                const auto b = queue.front();
                queue.erase(queue.begin());
                if (visited[b]) {
                    continue;
                }
                *lower_iter = b;
                ++lower_iter;
                visited[b] = 1;
                auto toinsert = prereq_to_course[b];
                toinsert = sort(toinsert, prereq_to_course);
                for(const auto val : toinsert) {
                    if (std::find(queue.begin(), queue.end(),val) != std::end(queue) ||
                        visited[val]) {
                        continue;
                    }
                    queue.insert(std::end(queue), val);
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

    std::clog << "done\n";
    return 0;
}
