

#include <iostream>
#include <fstream>
#include <vector>


#if defined(_WIN32) || defined(_WIN64)
#  define DllExport __declspec(dllexport)
#else
#  define DllExport [[visibility("default")]]
#endif

#ifndef _MSC_VER
#  define _stdcall
#endif

DllExport _stdcall
void
point_to_point(double elev[], double tht_m, double rht_m,
               double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
               double frq_mhz, int radio_climate, int pol, double conf, double rel,
               double & dbloss, char * strmode, int & errnum);
			   
DllExport _stdcall
double
ITMDLLVersion();

void ReadProfile(const std::string & filename, std::vector<double> & elevation);


int
main()
{
  std::vector<double> elevation;
  double tht_m, rht_m,
         eps_dielect, sgm_conductivity, eno_ns_surfref,
         frq_mhz;
  int radio_climate, pol;
  double conf, rel;
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
  std::cin >> tht_m;

  std::cout << std::endl << "  Enter receive antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  std::cin >> rht_m;

  std::cout << std::endl << "  Enter transmit frequency (>= " << 1.0 << ", <= " << 40000.0 << ") in MHz: ";
  std::cin >> frq_mhz;

  std::cout << std::endl << "  Enter confidence (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
  std::cin >> conf;

  std::cout << std::endl << "  Enter reliability (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
  std::cin >> rel;

  char cpol = '\0';
  std::cout << std::endl << "  Enter polarization (V|H): ";
  std::cin >> cpol;
  if ( cpol == 'v' || cpol == 'V' )
  {
    pol = 1;
  }
  else if ( cpol == 'h' || cpol == 'H' )
  {
    pol = 0;
  }
  else
  {
    pol = 1;
  }

  std::cout << std::endl << "  Enter relative permittivity (>= " << 0.0 << ", <= " << 100.0 << "): ";
  std::cin >> eps_dielect;

  std::cout << std::endl << "  Enter conductivity (>= " << 0.00001 << ", <= " << 100.0 << ") in Siemens per meter: ";
  std::cin >> sgm_conductivity;

  std::cout << std::endl << "  Enter refractivity (>= " << 200.0 << ", <= " << 450.0 << ") in M-units: ";
  std::cin >> eno_ns_surfref;

  std::cout << "  Radio Climates:\n";
  std::cout << "  1) Equatorial\n";
  std::cout << "  2) Continental Subtropical\n";
  std::cout << "  3) Maritime Tropical\n";
  std::cout << "  4) Desert\n";
  std::cout << "  5) Continental Temperate\n";
  std::cout << "  6) Maritime Temperate, Over Land\n";
  std::cout << "  7) Maritime Temperate, Over Sea\n";
  std::cout << "  Enter Climate: ";
  std::cin >> radio_climate;

  point_to_point(elevation.data(), tht_m, rht_m,
                 eps_dielect, sgm_conductivity, eno_ns_surfref,
                 frq_mhz, radio_climate, pol, conf, rel,
                 dbloss, strmode, errnum);

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

