

#include <iostream>


// pol: 0-Horizontal, 1-Vertical
// radio_climate: 1-Equatorial, 2-Continental Subtropical, 3-Maritime Tropical,
//                4-Desert, 5-Continental Temperate, 6-Maritime Temperate, Over Land,
//                7-Maritime Temperate, Over Sea
// conf, rel: .01 to .99
// elev[]: [num points - 1], [delta dist(meters)], [height(meters) point 1], ..., [height(meters) point n]
// errnum: 0- No Error.
//         1- Warning: Some parameters are nearly out of range.
//                     Results should be used with caution.
//         2- Note: Default parameters have been substituted for impossible ones.
//         3- Warning: A combination of parameters is out of range.
//                     Results are probably invalid.
//         Other-  Warning: Some parameters are out of range.
//                          Results are probably invalid.

#define DllExport  __declspec( dllexport )

void
DllExport _stdcall
point_to_point(double elev[], double tht_m, double rht_m,
               double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
               double frq_mhz, int radio_climate, int pol, double conf, double rel,
               double & dbloss, char * strmode, int & errnum);


int
main()
{
  double * elev;
  double tht_m, rht_m,
         eps_dielect, sgm_conductivity, eno_ns_surfref,
         frq_mhz;
  int    radio_climate, pol;
  double conf, rel;
  double dbloss;
  char * strmode;
  int    errnum;

  point_to_point(elev, tht_m, rht_m,
                 eps_dielect, sgm_conductivity, eno_ns_surfref,
                 frq_mhz, radio_climate, pol, conf, rel,
                 dbloss, strmode, errnum);
  // pol: 0-Horizontal, 1-Vertical
  // radio_climate: 1-Equatorial, 2-Continental Subtropical, 3-Maritime Tropical,
  //                4-Desert, 5-Continental Temperate, 6-Maritime Temperate, Over Land,
  //                7-Maritime Temperate, Over Sea
  // conf, rel: .01 to .99
  // elev[]: [num points - 1], [delta dist(meters)], [height(meters) point 1], ..., [height(meters) point n]
  // errnum: 0- No Error.
  //         1- Warning: Some parameters are nearly out of range.
  //                     Results should be used with caution.
  //         2- Note: Default parameters have been substituted for impossible ones.
  //         3- Warning: A combination of parameters is out of range.
  //                     Results are probably invalid.
  //         Other-  Warning: Some parameters are out of range.
  //                          Results are probably invalid.

}

