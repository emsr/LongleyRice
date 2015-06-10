

#include <iostream>
#include <fstream>

#include "ITM.h"

void ReadProfile(const std::string & filename, std::vector<double> & elevation);

int
main()
{
  std::vector<double> elevation;
  double tx_antenna_height, rx_antenna_height,
         permittivity, conductivity, refractivity,
         frequency;
  int climate, polarization;
  double confidence, reliability;
  double dbloss;
  char strmode[200] = "";
  int errnum;

  // Read profile data.
  std::string filename;
  std::cout << "\n  Enter profile file name: ";
#if defined(__cpp_lib_quoted_string_io)
  std::cin >> std::quoted(filename);
#else
  std::cin >> filename;
#endif
  ReadProfile(filename, elevation);
  if (elevation.size() < 5)
    std::cerr << "\n  Error reading profile data file \"" << filename << "\".\n";

  std::cout << std::endl << "  Enter transmit antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  std::cin >> tx_antenna_height;

  std::cout << std::endl << "  Enter receive antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  std::cin >> rx_antenna_height;

  std::cout << std::endl << "  Enter transmit frequency (>= " << 1.0 << ", <= " << 40000.0 << ") in MHz: ";
  std::cin >> frequency;

  std::cout << std::endl << "  Enter confidence (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
  std::cin >> confidence;

  std::cout << std::endl << "  Enter reliability (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
  std::cin >> reliability;

  char pol = '\0';
  std::cout << std::endl << "  Enter polarization (V|H): ";
  std::cin >> pol;
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

  std::cout << std::endl << "  Enter relative permittivity (>= " << 0.0 << ", <= " << 100.0 << "): ";
  std::cin >> permittivity;

  std::cout << std::endl << "  Enter conductivity (>= " << 0.00001 << ", <= " << 100.0 << ") in Siemens per meter: ";
  std::cin >> conductivity;

  std::cout << std::endl << "  Enter refractivity (>= " << 200.0 << ", <= " << 450.0 << ") in M-units: ";
  std::cin >> refractivity;

  std::cout << "  Radio Climates:\n";
  std::cout << "  1) Equatorial\n";
  std::cout << "  2) Continental Subtropical\n";
  std::cout << "  3) Maritime Tropical\n";
  std::cout << "  4) Desert\n";
  std::cout << "  5) Continental Temperate\n";
  std::cout << "  6) Maritime Temperate, Over Land\n";
  std::cout << "  7) Maritime Temperate, Over Sea\n";
  std::cout << "  Enter Climate: ";
  std::cin >> climate;

  point_to_point(elevation, tx_antenna_height, rx_antenna_height,
                 permittivity, conductivity, refractivity,
                 frequency, climate, polarization, confidence, reliability,
                 dbloss, strmode, errnum);

  std::cout << "\n  ITM Output\n";
  std::cout << "  Loss : " << dbloss << " dB\n";
  std::cout << "  Mode : " << strmode << '\n';
  std::cout << "  Error: " << errnum << '\n';

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

    elevation.resize(1 + 1 + static_cast<long>(num_points));
    elevation[0] = num_points;
    elevation[1] = delta_dist;

    for (long i = 0; i < static_cast<long>(num_points); ++i)
        dat >> elevation[2 + i];
}
