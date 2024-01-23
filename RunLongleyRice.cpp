

#include <iostream>
#include <iomanip>
#include <fstream>

#include <LongleyRice.h>

void ReadProfile(const std::string & filename, std::vector<double> & elevation);

std::string
get_mode(int mode)
{
  using namespace std::string_literals;
  if (mode == 0)
    return "Line-Of-Sight"s;
  if (mode >= 8)
  {
    mode -= 8;
    auto str = "Double Horizon"s;
    if (mode == 1)
      str += ", Diffraction Dominant"s;
    else if (mode == 2)
      str += ", Troposcatter Dominant";
    return str;
  }
  else if (mode >= 4)
  {
    mode -= 4;
    auto str = "Single Horizon"s;
    if (mode == 1)
      str += ", Diffraction Dominant"s;
    else if (mode == 2)
      str += ", Troposcatter Dominant"s;
    return str;
  }
  return "Error"s;
}


int
main(int nargs, char**)
{
  using std::cin, std::cout, std::vector, std::string, std::quoted;

  vector<double> elevation;
  double tx_antenna_height, rx_antenna_height,
         permittivity, conductivity, refractivity,
         frequency;
  int climate, polarization;
  double confidence, reliability;
  double dbloss, delta_h;
  char strmode[200] = "";
  int errnum, mode;
  double time, location, situation;
  int mdvar;

  bool run_tls = false;
  if (nargs > 1)
    run_tls = true;

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

  if (run_tls)
  {
    cout << "\n  Enter time (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
    cin >> time;

    cout << "\n  Enter location (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
    cin >> location;

    cout << "\n  Enter situation (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
    cin >> situation;

    cout << "\n  Enter variability type: ";
    cin >> mdvar;
  }
  else
  {
    cout << "\n  Enter confidence (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
    cin >> confidence;

    cout << "\n  Enter reliability (>= " << 0.01 << ", <= " << 0.99 << ") %: ";
    cin >> reliability;
  }

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

  if (run_tls)
    point_to_pointMDH(elevation, tx_antenna_height, rx_antenna_height,
                   permittivity, conductivity, refractivity,
                   frequency, climate, polarization, time, location, situation, 
                   dbloss, mode, delta_h, errnum);
  else
    point_to_point(elevation, tx_antenna_height, rx_antenna_height,
                   permittivity, conductivity, refractivity,
                   frequency, climate, polarization, confidence, reliability,
                   dbloss, strmode, errnum);

  cout << "\n  ITM Output\n";
  cout << "  Loss : " << dbloss << " dB\n";
  if (run_tls)
  {
    cout << "  Mode  : " << get_mode(mode) << '\n';
    cout << "  DeltaH: " << delta_h << '\n';
  }
  else
    cout << "  Mode  : " << strmode << '\n';
  cout << "  Error : " << errnum << '\n';

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

    double max_index = 0.0;
    dat >> max_index;
    if (max_index <= 0.0)
        return;

    double delta_dist = 0.0;
    dat >> delta_dist;
    if (delta_dist <= 0.0)
        return;

    elevation.resize(1 + 1 + 1 + static_cast<long>(max_index));
    elevation[0] = max_index;
    elevation[1] = delta_dist;

    for (long i = 0; i <= static_cast<long>(max_index); ++i)
        dat >> elevation[2 + i];
}
