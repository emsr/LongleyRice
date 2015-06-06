
#if defined(_MSC_VER)
#  define ITM_EXPORT __declspec(dllexport)
#  define ITM_LOCAL
#elif defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 480) && (__cplusplus >= 201103)
#  define ITM_EXPORT [[gnu::visibility("default")]]
#  define ITM_LOCAL [[gnu::visibility("hidden")]]
#elif defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 303)
#  define ITM_EXPORT __attribute__ ((visibility("default")))
#  define ITM_LOCAL __attribute__ ((visibility("hidden")))
#else
#  define ITM_EXPORT
#  define ITM_LOCAL
#endif

#include <vector>

struct ITM_EXPORT prop_type
{
  double aref;
  double dist;
  double hg[2];
  double wn;
  double dh;
  double ens;
  double gme;
  double zgndreal;
  double zgndimag;
  double he[2];
  double dl[2];
  double the[2];
  int kwx;
  int mdp;
};

struct ITM_EXPORT propv_type
{
  double sgc;
  int lvar;
  int mdvar;
  int klim;
};

struct ITM_EXPORT propa_type
{
  double dlsa;
  double dx;
  double ael;
  double ak1;
  double ak2;
  double aed;
  double emd;
  double aes;
  double ems;
  double dls[2];
  double dla;
  double tha;
};

ITM_EXPORT
void
qlrpfl(const std::vector<double> & elev, int klimx, int mdvarx,
       prop_type & prop, propa_type & propa, propv_type & propv);

///
/// pol: 0-Horizontal, 1-Vertical
/// radio_climate: 1-Equatorial, 2-Continental Subtropical, 3-Maritime Tropical,
///                4-Desert, 5-Continental Temperate, 6-Maritime Temperate, Over Land,
///                7-Maritime Temperate, Over Sea
/// conf, rel: .01 to .99
/// elev[]: [num points - 1], [delta dist(meters)], [height(meters) point 1], ..., [height(meters) point n]
/// errnum: 0- No Error.
///         1- Warning: Some parameters are nearly out of range.
///                     Results should be used with caution.
///         2- Note: Default parameters have been substituted for impossible ones.
///         3- Warning: A combination of parameters is out of range.
///                     Results are probably invalid.
///         Other-  Warning: Some parameters are out of range.
///                          Results are probably invalid.
///
ITM_EXPORT
void
point_to_point(const std::vector<double> & elev, double tht_m, double rht_m,
               double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
               double frq_mhz, int radio_climate, int pol, double conf, double rel,
               double & dbloss, char * strmode, int & errnum);

///
/// pol: 0-Horizontal, 1-Vertical
/// radio_climate: 1-Equatorial, 2-Continental Subtropical, 3-Maritime Tropical,
///                4-Desert, 5-Continental Temperate, 6-Maritime Temperate, Over Land,
///                7-Maritime Temperate, Over Sea
/// timepct, locpct, confpct: .01 to .99
/// elev[]: [num points - 1], [delta dist(meters)], [height(meters) point 1], ..., [height(meters) point n]
/// propmode:  Value   Mode
///             -1     mode is undefined
///              0     Line of Sight
///              5     Single Horizon, Diffraction
///              6     Single Horizon, Troposcatter
///              9     Double Horizon, Diffraction
///             10     Double Horizon, Troposcatter
/// errnum: 0- No Error.
///         1- Warning: Some parameters are nearly out of range.
///                     Results should be used with caution.
///         2- Note: Default parameters have been substituted for impossible ones.
///         3- Warning: A combination of parameters is out of range.
///                     Results are probably invalid.
///         Other-  Warning: Some parameters are out of range.
///                          Results are probably invalid.
///
ITM_EXPORT
void
point_to_pointMDH(const std::vector<double> & elev, double tht_m, double rht_m,
                  double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
                  double frq_mhz, int radio_climate, int pol, double timepct, double locpct, double confpct, 
                  double & dbloss, int & propmode, double & deltaH, int & errnum);

/// pol: 0-Horizontal, 1-Vertical
/// radio_climate: 1-Equatorial, 2-Continental Subtropical, 3-Maritime Tropical,
///                4-Desert, 5-Continental Temperate, 6-Maritime Temperate, Over Land,
///                7-Maritime Temperate, Over Sea
/// conf, rel: .01 to .99
/// elev[]: [num points - 1], [delta dist(meters)], [height(meters) point 1], ..., [height(meters) point n]
/// errnum: 0- No Error.
///         1- Warning: Some parameters are nearly out of range.
///                     Results should be used with caution.
///         2- Note: Default parameters have been substituted for impossible ones.
///         3- Warning: A combination of parameters is out of range.
///                     Results are probably invalid.
///         Other-  Warning: Some parameters are out of range.
///                          Results are probably invalid.
ITM_EXPORT
void
point_to_pointDH(const std::vector<double> & elev, double tht_m, double rht_m,
                 double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
                 double frq_mhz, int radio_climate, int pol, double conf, double rel,
                 double &dbloss, double &deltaH, int &errnum);

/// pol: 0-Horizontal, 1-Vertical
/// TSiteCriteria, RSiteCriteria:
///		   0 - random, 1 - careful, 2 - very careful
/// radio_climate: 1-Equatorial, 2-Continental Subtropical, 3-Maritime Tropical,
///                4-Desert, 5-Continental Temperate, 6-Maritime Temperate, Over Land,
///                7-Maritime Temperate, Over Sea
/// ModVar: 0 - Single: pctConf is "Time/Situation/Location", pctTime, pctLoc not used
///         1 - Individual: pctTime is "Situation/Location", pctConf is "Confidence", pctLoc not used
///         2 - Mobile: pctTime is "Time/Locations (Reliability)", pctConf is "Confidence", pctLoc not used
///         3 - Broadcast: pctTime is "Time", pctLoc is "Location", pctConf is "Confidence"
/// pctTime, pctLoc, pctConf: .01 to .99
/// errnum: 0- No Error.
///         1- Warning: Some parameters are nearly out of range.
///                     Results should be used with caution.
///         2- Note: Default parameters have been substituted for impossible ones.
///         3- Warning: A combination of parameters is out of range.
///                     Results are probably invalid.
///         Other-  Warning: Some parameters are out of range.
///                          Results are probably invalid.
/// NOTE: strmode is not used at this time.
ITM_EXPORT
void
area(long ModVar, double deltaH, double tht_m, double rht_m,
     double dist_km, int TSiteCriteria, int RSiteCriteria, 
     double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
     double frq_mhz, int radio_climate, int pol, double pctTime, double pctLoc,
     double pctConf, double &dbloss, char *strmode, int &errnum);

ITM_EXPORT
double
ITMAreadBLoss(long ModVar, double deltaH, double tht_m, double rht_m,
              double dist_km, int TSiteCriteria, int RSiteCriteria, 
              double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
              double frq_mhz, int radio_climate, int pol, double pctTime, double pctLoc,
              double pctConf);

ITM_EXPORT
double
ITMDLLVersion();
