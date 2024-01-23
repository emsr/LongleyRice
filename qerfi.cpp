/**
g++ qerfi.cpp -o qerfi
./qerfi
 */

#include <iostream>
#include <cmath>

double
qerfi(double q)
{
  double x, t, v;
  double c0  = 2.515516698;
  double c1  = 0.802853;
  double c2  = 0.010328;
  double d1  = 1.432788;
  double d2  = 0.189269;
  double d3  = 0.001308;

  x = 0.5 - q;
  t = std::max(0.5 - std::abs(x), 0.000001);
  t = std::sqrt(-2.0 * std::log(t));
  v = t - ((c2 * t + c1) * t + c0) / (((d3 * t + d2) * t + d1) * t + 1.0);
  if (x < 0.0)
    v = -v;
  return v;
}

int
main()
{
  for (int i = -20; i <= 20; ++i)
  {
    auto q = 0.1 * i;
    std::cout << ' ' << q << ' ' << qerfi(q) << ' ' << std::erfc(q) << '\n';;
  }
}
