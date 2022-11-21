#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <tuple>

class Solution {
public:
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
        const auto start = std::find_if(std::cbegin(course_to_its_prereq),
                                        std::cend(course_to_its_prereq),
                                        [](const auto& v){ return v.empty(); });
        if (start == std::cend(course_to_its_prereq)) {
            return out;
        }
        std::vector<int> visited(numCourses);
        std::vector<int> fifo;
        int idx{};
        for(const auto& v : std::as_const(course_to_its_prereq)) {
            if (v.empty()) {
                fifo.push_back(idx);
            }
            ++idx;
        }
        while(!fifo.empty()) {
            const auto b = fifo.front();
            fifo.erase(std::begin(fifo));
            if (visited[b]) {
                continue;
            }
            out.push_back(b);
            visited[b] = 1;
            const auto toinsert = prereq_to_course[b];
            fifo.insert(std::cend(fifo),
                         toinsert.cbegin(),
                         toinsert.cend());
        }

        if (out.size() != numCourses) {
            for (auto iter = visited.rbegin(); iter != visited.rend(); ++iter) {
                if (!*iter) {
                    out.insert(std::begin(out), std::distance(iter, visited.rend()) - 1);
                }
            }
        }
        return out;
    }
};

int main() {
    {
        std::vector<std::vector<int>> in{{1, 0},
                                         {2, 0},
                                         {3, 1},
                                         {3, 2}};
        const auto result = Solution::findOrder(4, in);
        std::copy(std::cbegin(result),
                  std::cend(result),
                  std::ostream_iterator<int>(std::clog, ","));
    }
    std::clog << '\n';
    {
        std::vector<std::vector<int>> in{{1, 0}};
        const auto result = Solution::findOrder(2, in);
        std::copy(std::cbegin(result),
                  std::cend(result),
                  std::ostream_iterator<int>(std::clog, ","));
    }
    std::clog << '\n';
    {
        std::vector<std::vector<int>> in;
        const auto result = Solution::findOrder(2, in);
        std::copy(std::cbegin(result),
                  std::cend(result),
                  std::ostream_iterator<int>(std::clog, ","));
    }
    std::clog << '\n';
    {
        std::vector<std::vector<int>> in;
        const auto result = Solution::findOrder(1, in);
        std::copy(std::cbegin(result),
                  std::cend(result),
                  std::ostream_iterator<int>(std::clog, ","));
    }
    std::clog << '\n';
    {
        std::vector<std::vector<int>> in{{0,1}, {1,0}};
        const auto result = Solution::findOrder(2, in);
        std::copy(std::cbegin(result),
                  std::cend(result),
                  std::ostream_iterator<int>(std::clog, ","));
    }
    std::clog << '\n';
    {
        std::vector<std::vector<int>> in{{1,0}};
        const auto result = Solution::findOrder(3, in);
        std::copy(std::cbegin(result),
                  std::cend(result),
                  std::ostream_iterator<int>(std::clog, ","));
    }
    std::clog << '\n';
    {
        std::vector<std::vector<int>> in{{0,1}, {0,2}, {1,2}};
        const auto result = Solution::findOrder(3, in);
        std::copy(std::cbegin(result),
                  std::cend(result),
                  std::ostream_iterator<int>(std::clog, ","));
    }
    std::clog << "\ndone\n";
    return 0;
}
