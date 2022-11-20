#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>

class Solution {
public:
    static std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
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
        if (start == std::cend(prereq_to_course)) {
            return out;
        }
        const int dist = std::distance(std::cbegin(course_to_its_prereq), start);
        std::vector<int> visited(numCourses);
        std::vector<int> fifo;
        fifo.push_back(dist);
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
            return {};
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
    return 0;
}
