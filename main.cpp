#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>


//class Solution {
//public:
//    static std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
//        std::vector<std::vector<int>> graph(numCourses);
//        for(const auto& v : std::as_const(prerequisites)) {
//            const int to = v[1];
//            const int from = v[0];
//            graph[to].push_back(from);
//        }
//        const auto start = std::find_if(std::cbegin(graph),
//                                        std::cend(graph),
//                                        [](const auto& inner){ return inner.empty(); });
//        std::vector<int> out;
//        // no empty one - no start
//        // empty means no one is pointing to you.
//        if (start == std::cend(graph)) {
//            return out;
//        }
//
//        return out;
//    }
//};


class Solution {
public:
    static std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        std::vector<std::vector<int>> graph(numCourses);
        for(const auto& v : std::as_const(prerequisites)) {
            const int to = v[1];
            const int from = v[0];
            graph[to].push_back(from);
        }
        const auto start = std::find_if(std::cbegin(graph),
                                        std::cend(graph),
                                        [](const auto& inner){ return inner.empty(); });
        std::vector<int> out;
        // no empty one - no start
        // empty means no one is pointing to you.
        if (start == std::cend(graph)) {
            return out;
        }

        return out;
    }
};

int main() {
    std::vector<std::vector<int>> in{ {1, 0}, {2, 0}, {3, 1}, {3, 2} };
    const auto result = Solution::findOrder(4, in);
    return 0;
}
