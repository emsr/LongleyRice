

#include <iostream>
#include <fstream>

#include "ITM.h"


int
main()
{
  double * elevation;
  double tx_antenna_height, rx_antenna_height,
         permittivity, conductivity, refractivity,
         frequency;
  int    climate, polarization;
  double conf, rel;
  double dbloss;
  char * strmode;
  int    errnum;

  // Read profile data.
  std::string filename;
  std::cout << "\n  Enter profile file name: ";
#if defined(__cpp_lib_quoted_string_io)
  std::cin >> std::quoted(filename);
#else
  std::cin >> filename;
#endif
  std::ifstream profile_in(filename);
  if ( profile_in.fail() )
  {
#if defined(__cpp_lib_quoted_string_io)
    std::cerr << "\n  Error: Unable to open profile data file " << std::quoted(filename) << "for reading.\n";
#else
    std::cerr << "\n  Error: Unable to open profile data file " << filename << "for reading.\n";
#endif
    return 1;
  }


  std::cout << std::endl << "  Enter transmit antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  std::cin >> tx_antenna_height;

  std::cout << std::endl << "  Enter receive antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  std::cin >> rx_antenna_height;

  std::cout << std::endl << "  Enter transmit frequency (>= " << 1.0 << ", <= " << 40000.0 << ") in MHz: ";
  std::cin >> frequency;

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
                 frequency, climate, polarization, conf, rel,
                 dbloss, strmode, errnum);

  return 0;
}

