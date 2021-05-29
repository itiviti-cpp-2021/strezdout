#include "primitives.h"

#include <charconv>
#include <iostream>
#include <set>
#include <vector>
#define cin std::cin
#define cout std::cout

class tester_2dtree
{
    void time_start()
    {
        cout << "TIME_START\n";
    }
    void time_end()
    {
        cout << "TIME_END\n";
    }

    void base_functional()
    {
        time_start();
        std::vector<Point> points = {{1, 0}, {123, -12}, {-28, 2}, {-34, 12}};
        std::vector<Rect> rects = {{{-200, -200}, {200, 200}},
                                   {{-50, -50}, {50, 50}},
                                   {{0, 0}, {2, 3}}};
        time_end();
        for (auto p : points) {
            cout << p.x() << " " << p.y() << ": ";
            for (size_t i = 0; i < points.size(); ++i) {
                cout << (p == points[i]) << " vs " << (p != points[i]) << " " << p.distance(points[i]) << " " << points[i].distance(p) << ", ";
            }
            cout << "\n";
        }

        for (auto r : rects) {
            cout << r.xmin() << " " << r.ymin() << " " << r.xmax() << " " << r.ymax() << ": ";
            for (auto p : points) {
                cout << r.contains(p) << " " << r.distance(p) << " ";
            }
            for (size_t i = 0; i < rects.size(); ++i) {
                cout << r.intersects(rects[i]) << " vs " << rects[i].intersects(r) << " ";
            }
            cout << "\n";
        }
    }
    void test_contains()
    {
        size_t n;
        cin >> n;
        std::vector<Point> points;
        for (size_t i = 0; i < n; ++i) {
            double x, y;
            cin >> x >> y;
            points.emplace_back(x, y);
        }

        std::vector<bool> answers;
        time_start();
        for (size_t i = 0; i < n; ++i) {
            answers.push_back(pointSet.contains(points[i]));
        }
        time_end();

        for (size_t i = 0; i < n; ++i) {
            cout << answers[i] << " ";
        }
        cout << std::endl;
    }
    void to_cout_uniq(std::vector<Point> & points)
    {
        std::sort(points.begin(), points.end(), [](Point & a, Point & b) {
            return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
        });
        cout << "There are " << points.size() << " points\n";
        for (size_t i = 0; i < points.size(); ++i) {
            cout << points[i].x() << "," << points[i].y() << " ";
            if (i % 300 == 299) {
                cout << std::endl;
            }
        }
        cout << std::endl;
    }
    void test_linear_iterator()
    {
        std::vector<Point> set1, set2;
        time_start();
        for (auto it = pointSet.begin(); it != pointSet.end(); ++it) {
            set1.push_back(*it);
        }
        for (auto it = pointSet.begin(); it != pointSet.end(); it++) {
            set2.push_back(*it);
        }
        std::vector<Point> set3(pointSet.begin(), pointSet.end());
        time_end();
        to_cout_uniq(set1);
        to_cout_uniq(set2);
        to_cout_uniq(set3);
    }
    void test_range()
    {
        size_t n;
        cin >> n;
        std::vector<Rect> rects;
        for (size_t i = 0; i < n; ++i) {
            double x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            Point p1(x1, y1), p2(x2, y2);
            rects.emplace_back(p1, p2);
        }

        std::vector<std::vector<Point>> answer;
        time_start();
        for (size_t i = 0; i < rects.size(); ++i) {
            auto its = pointSet.range(rects[i]);
            answer.emplace_back(its.first, its.second);
        }
        time_end();
        for (size_t i = 0; i < answer.size(); ++i) {
            to_cout_uniq(answer[i]);
        }
    }
    void test_nearest()
    {
        size_t n;
        cin >> n;
        std::vector<Point> points;
        for (size_t i = 0; i < n; ++i) {
            double x, y;
            cin >> x >> y;
            points.emplace_back(x, y);
        }

        std::vector<bool> answer_exists;
        std::vector<Point> answer;
        time_start();
        for (size_t i = 0; i < points.size(); ++i) {
            auto nst = pointSet.nearest(points[i]);
            answer_exists.push_back(nst.has_value());
            if (nst.has_value()) {
                answer.emplace_back(nst.value());
            }
        }
        time_end();
        size_t answer_counter = 0;
        for (size_t i = 0; i < points.size(); ++i) {
            cout << "Nearest " << points[i] << " " << answer_exists[i];
            if (answer_exists[i]) {
                cout << " dist " << points[i].distance(answer[answer_counter++]);
            }
            cout << std::endl;
        }
    }
    void test_nearest_k()
    {
        size_t n;
        cin >> n;
        std::vector<std::pair<Point, size_t>> points_and_k;
        for (size_t i = 0; i < n; ++i) {
            double x, y;
            size_t k;
            cin >> x >> y >> k;
            points_and_k.emplace_back(Point(x, y), k);
        }

        std::vector<std::pair<size_t, double>> answer;
        time_start();
        for (size_t i = 0; i < points_and_k.size(); ++i) {
            Point & from_point = points_and_k[i].first;
            auto nst = pointSet.nearest(from_point, points_and_k[i].second);
            std::vector<Point> solver(nst.first, nst.second);
            double max_dist = -1;
            for (auto p : solver) {
                max_dist = std::max(max_dist, from_point.distance(p));
            }
            answer.emplace_back(solver.size(), max_dist);
        }
        time_end();
        for (size_t i = 0; i < points_and_k.size(); ++i) {
            cout << "NearestK from " << points_and_k[i].first.x() << ", " << points_and_k[i].first.y() << " " << points_and_k[i].second << " points: count = " << answer[i].first << " max_d = " << answer[i].second;
            cout << std::endl;
        }
    }
    void one_test_case(unsigned int test_mode)
    {
        if (test_mode == 0) {
            base_functional();
        }
        else {
            std::string input_file_name;
            cin >> input_file_name;
            if (input_file_name != last_points_input_file) {
                pointSet = kdtree::PointSet(input_file_name);
            }
            switch (test_mode) {
            case 1:
                test_contains();
                break;
            case 2:
                test_linear_iterator();
                break;
            case 3:
                test_range();
                break;
            case 4:
                test_nearest();
                break;
            case 5:
                test_nearest_k();
                break;
            }
        }
        cout << "TEST_DONE\n";
    }

    std::string last_points_input_file = "___";
    kdtree::PointSet pointSet;
    size_t memory_limit_factor = 100000000;

public:
    void test()
    {
        cout << "SUITE_START\n";
        cout << "SUITE_INIT\n";

        std::string test_mode_str;
        while (std::getline(cin, test_mode_str)) {
            if (test_mode_str.empty()) {
                continue;
            }
            if (test_mode_str == "-1") {
                break;
            }
            one_test_case(std::stoi(test_mode_str));
        }
    }
};
int main()
{
    tester_2dtree tester;
    tester.test();
}
