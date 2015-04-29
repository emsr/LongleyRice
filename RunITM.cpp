

#include <iostream>

#include "ITM.h"


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

}

