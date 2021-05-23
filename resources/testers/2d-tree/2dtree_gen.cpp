#include <iostream>
#include <functional>
#include <vector>
#include <complex>
#include <algorithm>
#include <set>
#include <fstream>

namespace std {
    double pow(double x, double y, double m) {
        if (y == 0) return 1;
        int z = pow(x, y / 2, m);
        if (std::fmod(y, 2) == 0)
            return std::fmod(z * z, m);
        else
            return std::fmod(z * z * pow(x, std::fmod(y, 2)), m);
    }
}

struct Point {
    Point(double _x, double _y) : m_x(_x), m_y(_y) {

    }

    double x() {
        return m_x;
    }

    double y() {
        return m_y;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const Point &point) {
        return ostream << "(" << point.m_x << ", " << point.m_y << ")";
    }

    double distance(const Point &that) const {
        return std::sqrt(pow(m_x - that.m_x, 2) + pow(m_y - that.m_y, 2));
    }

    bool operator<(const Point &that) const {
        return m_x < that.m_x || (m_x == that.m_x && m_y < that.m_y);
    }

    bool operator>(const Point &that) const {
        return m_x > that.m_x || (m_x == that.m_x && m_y > that.m_y);
    }

    bool operator<=(const Point &that) const {
        return m_x <= that.m_x && m_y <= that.m_y;
    }

    bool operator>=(const Point &that) const {
        return m_x >= that.m_x && m_y >= that.m_y;
    }

    bool operator==(const Point &that) const {
        return m_x == that.m_x && m_y == that.m_y;
    }

    bool operator!=(const Point &that) const {
        return !(operator==(that));
    }

    double m_x;
    double m_y;
};

class test_generator {
public:
    using points_vector = std::vector<Point>;

    test_generator() {
        generate_linear_rules();
        test_base_functional();
        for (size_t i = 0; i < linear_inputs.size(); ++i) {
            print_points_to_file("linear" + std::to_string(i + 1) + ".txt", linear_inputs[i]);
        }
        linear_test(1);
        linear_test_linear_iterator();
        linear_test(3);
        linear_test(4);
        linear_test(5);
    }

    points_vector generate_linear_test(std::function<Point(double)> rule) {
        points_vector points;
        for (size_t i = 0; i < difficulty; ++i) {
            points.push_back(rule(static_cast<double> (i)));
        }
        return points;
    }

    void to_sout(points_vector points) {
        for (size_t i = 0; i < points.size(); ++i) {
            std::cout << points[i] << " ";
        }
        std::cout << "\n";
    }

private:
    struct Printer {
        void print_category(std::string name) {
            std::cout << "TESTCATEGORY " << name << "\n";
        }

        void print_test_case_header(std::string file_name, size_t mode) {
            std::cout << mode << std::endl;
            std::cout << file_name << std::endl;

        }

        void print_separator() {
            std::cout << "TESTLOLKEKTEST" << std::endl;
        }
    };

    size_t difficulty = 10000;

    Printer out;

    std::vector<points_vector> linear_inputs;
    std::vector<std::set<Point>> linear_inputs_set;
    std::vector<std::string> mode_by_code = {"Base", "Contains", "Linear iterator", "Range", "Nearest", "NearestK"};

    void generate_linear_rules() {
        auto d = static_cast<double>(difficulty);
        std::vector<std::function<Point(size_t)>> linear_rules;

        linear_rules.emplace_back([](double i) { return Point(i, i); });
        linear_rules.emplace_back([](double i) { return Point(i * 0.7, -i * 2); });
        linear_rules.emplace_back(
                [](double i) { return Point(-i * 23.7, 2.3 * pow(1.113, i * 4 / 17)); });
        linear_rules.emplace_back(
                [](double i) { return Point(pow(1.012, i), i * 34 / log(std::max(i, 2.0))); });
        linear_rules.emplace_back(
                [](double i) {
                    return Point(-pow(i, 2.3) * cos(i), std::ceil(-pow(i, 0.8) * std::sin(i)));
                });
        linear_rules.emplace_back([d](double i) { return Point(pow(i - d / 3, 3), pow(i - 4 / 3, 5)); });
        linear_rules.emplace_back([d](double i) {
            return Point(std::sin(pow(1.07, i) / (i + 1)) / log(i + 2) * pow(i, 1.5),
                         cos(pow(1.1234, i * 4 / 7) / log(2 + 3 * i)) * log(i * 1.4 + 1)
                         * pow(i * 1.6535 + 1, (std::fmod(i, 19) - 5) / 6));
        });
        linear_rules.emplace_back([](double i) {
            return Point(std::sin(4 * i) / cos(5 * i),
                         (i / std::abs(1 + i * cos(i / (1 + std::sin(i))))) +
                         std::abs(i * cos(i / (std::sin(i) + 2))) * pow(-1, ((int) i) % 53));
        });
        linear_rules.emplace_back([](double i) {
            return Point((0.4 * i - 800 / (i + 1) + i) * pow(-1, std::fmod(i + 1, 123)) * cos(i),
                         cos(i * 4) * std::abs(i / log(i + 123) * sin(i) * cos(i)) * pow(i, 1.0123));
        });
        linear_rules.emplace_back([d](double i) {
            return Point(d / log(d - i + 12) -
                         i * pow(-1, std::fmod(pow(i, 5), 9218)) * pow(i, 0.985) * 3 * log(i + 1),
                         sin(i * 1.3123) * 12123 *
                         std::fmod(i / log(2 + i * std::max(cos(i / 2), std::pow(i, 1 / i))) * sin(i * 12),
                                   212 * std::max(cos(3 * i), sin(2 * i)))); // *
//
        });

        for (auto &linear_rule : linear_rules) {
            linear_inputs.push_back(generate_linear_test(linear_rule));
            linear_inputs_set.emplace_back(linear_inputs.back().begin(), linear_inputs.back().end());
        }
    }

    void linear_test(size_t mode) {
        out.print_category(mode_by_code[mode] + " test");
        for (size_t r = 0; r < linear_inputs.size(); ++r) {
            for (size_t i = 0; i < linear_inputs.size(); ++i) {
                out.print_test_case_header("linear" + std::to_string(r + 1) + ".txt", mode);
                test_by_set(i, mode);
                if (i != linear_inputs.size() - 1) {
                    out.print_separator();
                }
            }
            if (r != linear_inputs.size() - 1) {
                out.print_separator();
            }
        }
    }

    void test_by_set(size_t o_r, size_t mode) {
        points_vector &for_input = linear_inputs[o_r];
        std::vector<std::string> cases;

        if (mode == 1) {
            for (size_t i = 0; i < for_input.size(); ++i) {
                cases.push_back(std::to_string(for_input[i].x()) + " " + std::to_string(for_input[i].y()));
            }
        } else if (mode == 3) {
            for (size_t i = 0; i < for_input.size(); i += 1 + (difficulty / 100)) {
                for (size_t j = i + 3; j < for_input.size(); j += difficulty / 10) {
                    double min_x, min_y, max_x, max_y;

                    std::vector<double> range_x({for_input[i].x(), for_input[j].x()});
                    std::vector<double> range_y({for_input[i].y(), for_input[j].y()});
                    std::sort(range_x.begin(), range_x.end());
                    std::sort(range_y.begin(), range_y.end());
                    min_x = range_x[0];
                    max_x = range_x[1];
                    min_y = range_y[0];
                    max_y = range_y[1];
                    cases.push_back(std::to_string(min_x) + " " + std::to_string(min_y)
                                    + " " + std::to_string(max_x) + " " + std::to_string(max_y)
                    );
                }
            }
        } else if (mode == 4) {
            for (size_t i = 0; i < for_input.size(); ++i) {
                cases.push_back(std::to_string(for_input[i].x()) + " " + std::to_string(for_input[i].y()));
            }
        } else if (mode == 5) {
            size_t count_max_k = std::min(static_cast<size_t>(100), difficulty);
            size_t count_k_cases = std::min(count_max_k / 10, count_max_k);
            for (size_t i = 0; i < for_input.size(); i += difficulty / 25) {
                size_t k = (i + static_cast<size_t>(std::abs(for_input[i].x()))) % count_max_k;
                cases.push_back(std::to_string(for_input[i].x()) + " " + std::to_string(for_input[i].y()) + " " +
                                std::to_string(k));

            }
        }
        std::cout << cases.size() << std::endl;
        for (auto &i : cases) {
            std::cout << i << std::endl;
        }
    }

    void test_base_functional() {
        out.print_category("Base functional");
        std::cout << 0 << std::endl;
    }

    void linear_test_linear_iterator() {
        out.print_category("Linear iterator");
        for (size_t i = 0; i < linear_inputs.size(); ++i) {
            out.print_test_case_header("linear" + std::to_string(i + 1) + ".txt", 2);
            if (i != linear_inputs.size() - 1) {
                out.print_separator();
            }
        }
    }

    void print_points_to_file(std::string file_name, points_vector &points) {
        std::ofstream out(file_name);
        for (auto p : points) {
            out << p.x() << " " << p.y() << "\n";
        }
        out.close();
    }
};

int main() {
//    std::cout << ;
    test_generator generator;
    //generator.to_sout(generator.generate_linear_test(4));

    return 0;
}
