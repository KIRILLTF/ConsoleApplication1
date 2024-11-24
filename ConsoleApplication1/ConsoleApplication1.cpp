#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

struct Circle {
  double x, y, r;
};

bool is_inside_circle(double x, double y, const Circle& circle) {
  double dx = x - circle.x;
  double dy = y - circle.y;
  return dx * dx + dy * dy <= circle.r * circle.r;
}

int main() {
  Circle circles[3];
  circles[0] = {1.0, 1.0, 1.0};
  circles[1] = {1.5, 2.0, std::sqrt(5) / 2};
  circles[2] = {2.0, 1.5, std::sqrt(5) / 2};

  double minX = circles[0].x - circles[0].r;
  double maxX = circles[0].x + circles[0].r;
  double minY = circles[0].y - circles[0].r;
  double maxY = circles[0].y + circles[0].r;

  for (int i = 1; i < 3; ++i) {
    minX = std::min(minX, circles[i].x - circles[i].r);
    maxX = std::max(maxX, circles[i].x + circles[i].r);
    minY = std::min(minY, circles[i].y - circles[i].r);
    maxY = std::max(maxY, circles[i].y + circles[i].r);
  }

  double rect_area = (maxX - minX) * (maxY - minY);

  int start_points = 100;
  int end_points = 100000;
  int step = 500;

  std::ofstream results("results.csv");
  results << "N,Approximated_Area,Relative_Error\n";

  std::mt19937 rng(42);
  std::uniform_real_distribution<double> distX(minX, maxX);
  std::uniform_real_distribution<double> distY(minY, maxY);

  const double pi = 3.14159265358979323846;
  const double exact_area =
      0.25 * pi + 1.25 * std::asin(0.8) - 1;

  for (int num_points = start_points; num_points <= end_points;
       num_points += step) {
    int count_inside = 0;

    for (int i = 0; i < num_points; ++i) {
      double x = distX(rng);
      double y = distY(rng);

      if (is_inside_circle(x, y, circles[0]) &&
          is_inside_circle(x, y, circles[1]) &&
          is_inside_circle(x, y, circles[2])) {
        count_inside++;
      }
    }

    double approx_area = rect_area * count_inside / num_points;
    double relative_error =
        std::abs((approx_area - exact_area) / exact_area) * 100.0;

    results << num_points << "," << approx_area << "," << relative_error
            << "\n";

    std::cout << "N: " << num_points << ", Approximated Area: " << approx_area
              << ", Relative Error: " << relative_error << "%\n";
  }

  results.close();
  return 0;
}
