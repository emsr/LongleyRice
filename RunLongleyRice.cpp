

#include <iostream>
#include <iomanip>
#include <fstream>

#include <LongleyRice.h>

void ReadProfile(const std::string & filename, std::vector<double> & elevation);

int
main()
{
  using std::cin, std::cout, std::vector, std::string, std::quoted;

  vector<double> elevation;
  double tx_antenna_height, rx_antenna_height,
         permittivity, conductivity, refractivity,
         frequency;
  int climate, polarization;
  double confidence, reliability;
  double dbloss;
  char strmode[200] = "";
  int errnum;

  // Read profile data.
  string filename;
  cout << "\n  Enter profile file name: ";
#if defined(__cpp_lib_quoted_string_io)
  cin >> quoted(filename);
#else
  cin >> filename;
#endif
  ReadProfile(filename, elevation);
  if (elevation.size() < 5)
    std::cerr << "\n  Error reading profile data file \"" << filename << "\".\n";

  cout << "\n  Enter transmit antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  cin >> tx_antenna_height;

  cout << "\n  Enter receive antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  cin >> rx_antenna_height;

  cout << "\n  Enter transmit frequency (>= " << 1.0 << ", <= " << 40000.0 << ") in MHz: ";
  cin >> frequency;

  cout << "\n  Enter confidence (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
  cin >> confidence;

  cout << "\n  Enter reliability (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
  cin >> reliability;

  char pol = '\0';
  cout << "\n  Enter polarization (V|H): ";
  cin >> pol;
  if ( pol == 'v' || pol == 'V' )
  {
    polarization = 1;
  }
  else if ( pol == 'h' || pol == 'H' )
  {
    polarization = 0;
  }
  else
  {
    polarization = 1;
  }

  cout << "\n  Enter relative permittivity (>= " << 0.0 << ", <= " << 100.0 << "): ";
  cin >> permittivity;

  cout << "\n  Enter conductivity (>= " << 0.00001 << ", <= " << 100.0 << ") in Siemens per meter: ";
  cin >> conductivity;

  cout << "\n  Enter refractivity (>= " << 200.0 << ", <= " << 450.0 << ") in M-units: ";
  cin >> refractivity;

  cout << "  Radio Climates:\n";
  cout << "  1) Equatorial\n";
  cout << "  2) Continental Subtropical\n";
  cout << "  3) Maritime Tropical\n";
  cout << "  4) Desert\n";
  cout << "  5) Continental Temperate\n";
  cout << "  6) Maritime Temperate, Over Land\n";
  cout << "  7) Maritime Temperate, Over Sea\n";
  cout << "  Enter Climate: ";
  cin >> climate;

  point_to_point(elevation, tx_antenna_height, rx_antenna_height,
                 permittivity, conductivity, refractivity,
                 frequency, climate, polarization, confidence, reliability,
                 dbloss, strmode, errnum);

  cout << "\n  ITM Output\n";
  cout << "  Loss : " << dbloss << " dB\n";
  cout << "  Mode : " << strmode << '\n';
  cout << "  Error: " << errnum << '\n';

  return 0;
}


//  Read a terrain profile.
void
ReadProfile(const std::string & filename,
            std::vector<double> & elevation)
{
    elevation.clear();

    std::ifstream dat(filename);
    if (dat.fail())
        return;

    double num_points = 0.0;
    dat >> num_points;
    if (num_points <= 0.0)
        return;

    double delta_dist = 0.0;
    dat >> delta_dist;
    if (delta_dist <= 0.0)
        return;

    elevation.resize(1 + 1 + 1 + static_cast<long>(num_points));
    elevation[0] = num_points;
    elevation[1] = delta_dist;

    for (long i = 0; i <= static_cast<long>(num_points); ++i)
        dat >> elevation[2 + i];
}
