// *************************************
// C++ routines for this program are taken from
// a translation of the FORTRAN code written by
// U.S. Department of Commerce NTIA/ITS
// Institute for Telecommunication Sciences
// *****************
// Irregular Terrain Model (ITM) (Longley-Rice)
// *************************************



#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <vector>

#define THIRD (1.0/3.0)

#include <LongleyRice.h>


/// This performs the Fortran DIM function.
/// result is x-y if x is greater than y; otherwise result is 0.0
double
FORTRAN_DIM(double x, double y)
{
  if (x > y)
    return x - y;
  else
    return 0.0;
}


double
aknfe(double v2)
{
  double a;
  if (v2 < 5.76)
    a = 6.02 + 9.11 * std::sqrt(v2) - 1.27 * v2;
  else
    a = 12.953 + 4.343 * std::log(v2);
  return a;
}


double
fht(double x, double pk)
{
  double w, fhtv;
  if (x < 200.0)
    {
      w = -std::log(pk);
      if (pk < 1.0e-5 || x * std::pow(w, 3.0) > 5495.0)
        {
          fhtv = -117.0;
          if (x > 1.0)
            fhtv += 17.372 * std::log(x);
        }
      else
        fhtv = 2.5e-5 * x * x / pk - 8.686 * w - 15.0;
    }
  else
    {
      fhtv = 0.05751 * x - 4.343 * std::log(x);
      if (x < 2000.0)
        {
          w = 0.0134 * x * std::exp(-0.005 * x);
          fhtv = (1.0 - w) * fhtv + w * (17.372 * std::log(x) - 117.0);
        }
    }
  return fhtv;
}


double
h0f(double r, double et)
{
  double a[5]{25.0, 80.0, 177.0, 395.0, 705.0};
  double b[5]{24.0, 45.0,  68.0,  80.0, 105.0};
  double q, x;
  auto it = static_cast<int>(et);
  if (it <= 0)
    {
      it = 1;
      q = 0.0;
    }
  else if (it >= 5)
    {
      it = 5;
      q = 0.0;
    }
  else
    q = et - it;
  x = std::pow(1.0 / r, 2.0);
  double h0fv = 4.343 * std::log((a[it - 1] * x + b[it - 1]) * x + 1.0);
  if (q != 0.0)
    h0fv = (1.0 - q) * h0fv + q * 4.343 * std::log((a[it] * x + b[it]) * x + 1.0);
  return h0fv;
}


double
ahd(double td)
{
  int i;
  double a[3] = {   133.4,    104.6,     71.8};
  double b[3] = {0.332e-3, 0.212e-3, 0.157e-3};
  double c[3] = {  -4.343,   -1.086,    2.171};
  if (td <= 10.0e3)
    i = 0;
  else if (td <= 70.0e3)
    i = 1;
  else
    i = 2;
  return a[i] + b[i] * td + c[i] * std::log(td);
}


double
adiff(double d, prop_type & prop, propa_type & propa)
{
  std::complex<double> prop_zgnd(prop.zgndreal, prop.zgndimag);
  thread_local double wd1, xd1, afo, qk, aht, xht;
  double a, q, pk, ds, th, wa, ar, wd, adiffv;
  if (d == 0)
    {
      q = prop.hg[0] * prop.hg[1];
      qk = prop.he[0] * prop.he[1] - q;
      if (prop.mdp < 0.0)
        q += 10.0;
      wd1 = std::sqrt(1.0 + qk / q);
      xd1 = propa.dla + propa.tha / prop.gme;
      q = (1.0-0.8*std::exp(-propa.dlsa/50.0e3))*prop.dh;
      q *= 0.78 * std::exp(-std::pow(q / 16.0, 0.25));
      afo = std::min(15.0, 2.171 * std::log(1.0 + 4.77e-4 * prop.hg[0] * prop.hg[1] * prop.wn * q));
      qk = 1.0 / std::abs(prop_zgnd);
      aht = 20.0;
      xht = 0.0;
      for(int j = 0; j < 2; ++j)
        {
          a = 0.5 * std::pow(prop.dl[j], 2.0) / prop.he[j];
          wa = std::pow(a * prop.wn, THIRD);
          pk = qk / wa;
          q = (1.607 - pk) * 151.0 * wa * prop.dl[j] / a;
          xht += q;
          aht += fht(q, pk);
        }
      adiffv = 0.0;
    }
  else
    {
      th = propa.tha + d * prop.gme;
      ds = d - propa.dla;
      q = 0.0795775 * prop.wn * ds * std::pow(th, 2.0);
      adiffv = aknfe(q * prop.dl[0] / (ds + prop.dl[0])) + aknfe(q * prop.dl[1] / (ds + prop.dl[1]));
      a = ds / th;
      wa = std::pow(a * prop.wn, THIRD);
      pk = qk / wa;
      q = (1.607 - pk) * 151.0 * wa * th + xht;
      ar = 0.05751 * q - 4.343 * std::log(q) - aht;
      q = (wd1 + xd1 / d) * std::min(((1.0 - 0.8 * std::exp(-d / 50.0e3)) * prop.dh * prop.wn), 6283.2);
      wd = 25.1 / (25.1 + std::sqrt(q));
      adiffv = ar * wd + (1.0 - wd) * adiffv + afo;
    }
  return adiffv;
}


double
ascat(double d, prop_type & prop, propa_type & propa)
{
  std::complex<double> prop_zgnd(prop.zgndreal, prop.zgndimag);
  thread_local double ad, rr, etq, h0s;
  double h0, r1, r2, z0, ss, et, ett, th, q;
  double ascatv;
  if (d == 0.0)
    {
      ad = prop.dl[0] - prop.dl[1];
      rr = prop.he[1] / prop.he[0];
      if(ad < 0.0)
        {
          ad =- ad;
          rr = 1.0 / rr;
        }
      etq = (5.67e-6 * prop.ens - 2.32e-3) * prop.ens + 0.031;
      h0s = -15.0;
      ascatv = 0.0;
    }
  else
    {
      if (h0s > 15.0)
        h0 = h0s;
      else
        {
          th = prop.the[0] + prop.the[1] + d * prop.gme;
          r2 = 2.0 * prop.wn * th;
          r1 = r2 * prop.he[0];
          r2 *= prop.he[1];
          if (r1 < 0.2 && r2 < 0.2)
            return 1001.0;
          ss = (d - ad) / (d + ad);
          q = rr / ss;
          ss = std::max(0.1, ss);
          q = std::min(std::max(0.1, q), 10.0);
          z0 = (d - ad) * (d + ad) * th * 0.25 / d;
          et = (etq * std::exp(-pow(std::min(1.7, z0 / 8.0e3), 6.0)) + 1.0) * z0 / 1.7556e3;
          ett = std::max(et, 1.0);
          h0 = (h0f(r1, ett) + h0f(r2, ett)) * 0.5;
          h0 += std::min(h0, (1.38 - std::log(ett)) * std::log(ss) * std::log(q) * 0.49);
          h0 = FORTRAN_DIM(h0, 0.0);
          if (et < 1.0)
            h0 = et * h0 + (1.0 - et) * 4.343 * std::log(pow((1.0 + 1.4142 / r1)
               * (1.0 + 1.4142 / r2), 2.0) * (r1 + r2) / (r1 + r2 + 2.8284));
          if (h0 > 15.0 && h0s >= 0.0)
            h0 = h0s;
        }
      h0s = h0;
      th = propa.tha + d * prop.gme;
      ascatv = ahd(th * d) + 4.343 * std::log(47.7 * prop.wn * pow(th, 4.0))
             - 0.1 * (prop.ens - 301.0) * std::exp(-th * d / 40.0e3) + h0;
    }
  return ascatv;
}


ITM_EXPORT
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


ITM_EXPORT
void
qlrps(double fmhz, double zsys, double en0,
      int ipol, double eps, double sgm, prop_type &prop)
{
  double gma = 157.0e-9;
  prop.wn = fmhz / 47.7;
  prop.ens = en0;
  if (zsys != 0.0)
    prop.ens *= std::exp(-zsys / 9460.0);
  prop.gme = gma * (1.0 - 0.04665 * std::exp(prop.ens / 179.3));
  std::complex<double> zq, prop_zgnd(prop.zgndreal, prop.zgndimag);
  zq = std::complex<double> (eps, 376.62 * sgm / prop.wn);
  prop_zgnd = std::sqrt(zq - 1.0);
  if (ipol != 0.0)
    prop_zgnd /= zq;

  prop.zgndreal = prop_zgnd.real();
  prop.zgndimag = prop_zgnd.imag();
}


double
abq_alos(std::complex<double> r)
{ return r.real() * r.real() + r.imag() * r.imag(); }


double
alos(double d, prop_type & prop, propa_type & propa)
{
  std::complex<double> prop_zgnd(prop.zgndreal, prop.zgndimag);
  thread_local double wls;
  std::complex<double> r;
  double s, sps, q;
  double alosv;
  if (d == 0.0)
    {
      wls = 0.021 / (0.021 + prop.wn * prop.dh / std::max(10.0e3, propa.dlsa));
      alosv = 0.0;
    }
  else
    {
      q = (1.0 - 0.8 * std::exp(-d / 50.0e3)) * prop.dh;
      s = 0.78 * q * std::exp(-std::pow(q / 16.0, 0.25));
      q = prop.he[0] + prop.he[1];
      sps = q / std::sqrt(d * d + q * q);
      r = (sps - prop_zgnd) / (sps + prop_zgnd)*std::exp(-std::min(10.0,prop.wn*s*sps));
      q = abq_alos(r);
      if (q < 0.25 || q < sps)
        r *= std::sqrt(sps / q);
      alosv = propa.emd*d + propa.aed;
      q = 2.0 * prop.wn * prop.he[0] * prop.he[1] / d;
      if (q > 1.57)
        q = 3.14 - 2.4649 / q;
      alosv = (-4.343 * std::log(abq_alos(std::complex<double>(cos(q), -sin(q)) + r)) - alosv)
            * wls + alosv;
    }
  return alosv;
}


ITM_EXPORT
void
qlra(int kst[], int klimx, int mdvarx,
     prop_type &prop, propv_type &propv)
{
  std::complex<double> prop_zgnd(prop.zgndreal, prop.zgndimag);
  double q;
  for (int j = 0; j < 2; ++j)
    {
      if (kst[j] <= 0)
        prop.he[j] = prop.hg[j];
      else
        {
          q = 4.0;
          if (kst[j] != 1)
            q = 9.0;
          if (prop.hg[j] < 5.0)
            q *= std::sin(0.3141593*prop.hg[j]);
          prop.he[j] = prop.hg[j]
                     + (1.0 + q) * std::exp(-std::min(20.0, 2.0 * prop.hg[j] / std::max(1.0e-3, prop.dh)));
        }
      q = std::sqrt(2.0 * prop.he[j] / prop.gme);
      prop.dl[j] = q * std::exp(-0.07 * std::sqrt(prop.dh / std::max(prop.he[j], 5.0)));
      prop.the[j] = (0.65 * prop.dh * (q / prop.dl[j] - 1.0) - 2.0 * prop.he[j]) / q;
    }
  prop.mdp = 1;
  propv.lvar = std::max(propv.lvar, 3);
  if (mdvarx >= 0)
    {
      propv.mdvar = mdvarx;
      propv.lvar = std::max(propv.lvar, 4);
    }
  if (klimx > 0)
    {
      propv.klim = klimx;
      propv.lvar = 5;
    }
}


ITM_EXPORT
void
lrprop(double d,
       prop_type &prop, propa_type &propa)  // PaulM_lrprop
{
  thread_local bool wlos, wscat;
  thread_local double dmin, xae;
  std::complex<double> prop_zgnd(prop.zgndreal,prop.zgndimag);
  double a0, a1, a2, a3, a4, a5, a6;
  double d0, d1, d2, d3, d4, d5, d6;
  bool wq;
  double q;
  int j;

  if (prop.mdp != 0)
    {
      for (j = 0; j < 2; ++j)
        propa.dls[j] = std::sqrt(2.0 * prop.he[j] / prop.gme);
      propa.dlsa = propa.dls[0] + propa.dls[1];
      propa.dla = prop.dl[0] + prop.dl[1];
      propa.tha = std::max(prop.the[0] + prop.the[1], -propa.dla * prop.gme);
      wlos = false;
      wscat = false;
      if (prop.wn < 0.838 || prop.wn > 210.0)
        prop.kwx = std::max(prop.kwx, 1);
      for (j = 0; j < 2; ++j)
        if (prop.hg[j] < 1.0 || prop.hg[j] > 1000.0)
          prop.kwx = std::max(prop.kwx, 1);
      for (j = 0; j < 2; ++j)
        if (std::abs(prop.the[j]) > 200.0e-3
         || prop.dl[j] < 0.1 * propa.dls[j]
         || prop.dl[j] > 3.0 * propa.dls[j])
        prop.kwx = std::max(prop.kwx, 3);
      if (prop.ens < 250.0 || prop.ens > 400.0
       || prop.gme < 75.0e-9 || prop.gme > 250.0e-9
       || prop_zgnd.real() <= std::abs(prop_zgnd.imag())
       || prop.wn  < 0.419 || prop.wn  > 420.0 )
        prop.kwx = 4;
      for (j = 0; j < 2; ++j)
        if (prop.hg[j] < 0.5 || prop.hg[j] > 3000.0)
          prop.kwx = 4;
      dmin = std::abs(prop.he[0] - prop.he[1]) / 200.0e-3;
      q = adiff(0.0, prop, propa);
      xae = std::pow(prop.wn * std::pow(prop.gme, 2), -THIRD);
      d3 = std::max(propa.dlsa, 1.3787 * xae + propa.dla);
      d4 = d3 + 2.7574 * xae;
      a3 = adiff(d3, prop, propa);
      a4 = adiff(d4, prop, propa);
      propa.emd = (a4 - a3) / (d4 - d3);
      propa.aed = a3 - propa.emd * d3;
     }
  if (prop.mdp >= 0)
    {
      prop.mdp = 0;
      prop.dist = d;
    }
  if (prop.dist > 0.0)
    {
      if (prop.dist > 1000.0e3)
        prop.kwx = std::max(prop.kwx, 1);
      if (prop.dist < dmin)
        prop.kwx = std::max(prop.kwx, 3);
      if (prop.dist < 1.0e3 || prop.dist > 2000.0e3)
        prop.kwx = 4;
    }
  if (prop.dist < propa.dlsa)
    {
      if (!wlos)
        {
          q = alos(0.0, prop, propa);
            d2 = propa.dlsa;
            a2 = propa.aed + d2 * propa.emd;
            d0 = 1.908 * prop.wn * prop.he[0] * prop.he[1];
              if (propa.aed >= 0.0)
              {
                d0 = std::min(d0, 0.5 * propa.dla);
                d1 = d0 + 0.25 * (propa.dla - d0);
              }
            else
              d1 = std::max(-propa.aed / propa.emd, 0.25 * propa.dla);
            a1 = alos(d1, prop, propa);
            wq = false;
            if (d0 < d1)
              {
                a0 = alos(d0, prop, propa);
                q = std::log(d2 / d0);
                propa.ak2 = std::max(0.0, ((d2 - d0) * (a1 - a0) - (d1 - d0) * (a2 - a0))
                                        / ((d2 - d0) * std::log(d1 / d0) - (d1 - d0) * q));
                wq = propa.aed >= 0.0 || propa.ak2 > 0.0;
                if (wq)
                  {
                    propa.ak1 = (a2 - a0 - propa.ak2 * q) / (d2 - d0);
                    if (propa.ak1 < 0.0)
                      {
                        propa.ak1 = 0.0;
                        propa.ak2 = FORTRAN_DIM(a2, a0) / q;
                        if (propa.ak2 == 0.0)
                          propa.ak1 = propa.emd;
                      }
                  }
              }
            if (!wq)
              {
                propa.ak1 = FORTRAN_DIM(a2, a1) / (d2 - d1);
                propa.ak2 = 0.0;
                if (propa.ak1 == 0.0)
                  propa.ak1 = propa.emd;
              }
            propa.ael = a2 - propa.ak1 * d2 - propa.ak2 * std::log(d2);
            wlos = true;
        }
      if (prop.dist > 0.0)
        prop.aref = propa.ael
                  + propa.ak1 * prop.dist
                  + propa.ak2 * std::log(prop.dist);
    }
  if (prop.dist <= 0.0 || prop.dist >= propa.dlsa)
    {
      if (!wscat)
        { 
          q = ascat(0.0, prop,propa);
          d5 = propa.dla + 200.0e3;
          d6 = d5 + 200.0e3;
          a6 = ascat(d6, prop, propa);
          a5 = ascat(d5, prop, propa);
          if (a5 < 1000.0)
            {
              propa.ems = (a6 - a5) / 200.0e3;
              propa.dx = std::max(propa.dlsa,
                                  std::max(propa.dla + 0.3 * xae * std::log(47.7 * prop.wn),
                                          (a5 - propa.aed - propa.ems * d5) / (propa.emd - propa.ems)));
              propa.aes = (propa.emd - propa.ems) * propa.dx + propa.aed;
            }
          else
            {
              propa.ems = propa.emd;
              propa.aes = propa.aed;
              propa.dx = 10.e6;
            }
          wscat = true;
        }
      if (prop.dist > propa.dx)
        prop.aref = propa.aes + propa.ems * prop.dist;
      else
        prop.aref = propa.aed + propa.emd * prop.dist;
    }
  prop.aref = std::max(prop.aref, 0.0);
}


double
curve(double c1, double c2, double x1,
      double x2, double x3, double de)
{
  return (c1 + c2 / (1.0 + std::pow((de - x2) / x3, 2.0)))
       * std::pow(de / x1, 2.0) / (1.0 + std::pow(de / x1, 2.0));
}


ITM_EXPORT
double
avar(double zzt, double zzl, double zzc,
     prop_type &prop, propv_type &propv)
{
  thread_local int kdv;
  thread_local double dexa, de, vmd, vs0, sgl, sgtm, sgtp, sgtd, tgtd,
                      gm, gp, cv1, cv2, yv1, yv2, yv3, csm1, csm2, ysm1, ysm2,
                      ysm3, csp1, csp2, ysp1, ysp2, ysp3, csd1, zd, cfm1, cfm2,
                      cfm3, cfp1, cfp2, cfp3;
  double bv1[7]{-9.67,-0.62,1.26,-9.21,-0.62,-0.39,3.15};
  double bv2[7]{12.7,9.19,15.5,9.05,9.19,2.86,857.9};
  double xv1[7]{144.9e3,228.9e3,262.6e3,84.1e3,228.9e3,141.7e3,2222.e3};
  double xv2[7]{190.3e3,205.2e3,185.2e3,101.1e3,205.2e3,315.9e3,164.8e3};
  double xv3[7]{133.8e3,143.6e3,99.8e3,98.6e3,143.6e3,167.4e3,116.3e3};
  double bsm1[7]{2.13,2.66,6.11,1.98,2.68,6.86,8.51};
  double bsm2[7]{159.5,7.67,6.65,13.11,7.16,10.38,169.8};
  double xsm1[7]{762.2e3,100.4e3,138.2e3,139.1e3,93.7e3,187.8e3,609.8e3};
  double xsm2[7]{123.6e3,172.5e3,242.2e3,132.7e3,186.8e3,169.6e3,119.9e3};
  double xsm3[7]{94.5e3,136.4e3,178.6e3,193.5e3,133.5e3,108.9e3,106.6e3};
  double bsp1[7]{2.11,6.87,10.08,3.68,4.75,8.58,8.43};
  double bsp2[7]{102.3,15.53,9.60,159.3,8.12,13.97,8.19};
  double xsp1[7]{636.9e3,138.7e3,165.3e3,464.4e3,93.2e3,216.0e3,136.2e3};
  double xsp2[7]{134.8e3,143.7e3,225.7e3,93.1e3,135.9e3,152.0e3,188.5e3};
  double xsp3[7]{95.6e3,98.6e3,129.7e3,94.2e3,113.4e3,122.7e3,122.9e3};
  double bsd1[7]{1.224,0.801,1.380,1.000,1.224,1.518,1.518};
  double bzd1[7]{1.282,2.161,1.282,20.,1.282,1.282,1.282};
  double bfm1[7]{1.0,1.0,1.0,1.0,0.92,1.0,1.0};
  double bfm2[7]{0.0,0.0,0.0,0.0,0.25,0.0,0.0};
  double bfm3[7]{0.0,0.0,0.0,0.0,1.77,0.0,0.0};
  double bfp1[7]{1.0,0.93,1.0,0.93,0.93,1.0,1.0};
  double bfp2[7]{0.0,0.31,0.0,0.19,0.31,0.0,0.0};
  double bfp3[7]{0.0,2.00,0.0,1.79,2.00,0.0,0.0};
  thread_local bool ws, w1;
  double rt = 7.8, rl = 24.0, avarv, q, vs, zt, zl, zc;
  double sgt, yr;
  int temp_klim = propv.klim - 1;

  if (propv.lvar > 0)
    {
      switch (propv.lvar)
        {
        default:
          if (propv.klim <= 0 || propv.klim > 7)
            {
              propv.klim = 5;
              temp_klim = 4;
              {
                prop.kwx = std::max(prop.kwx, 2);
              }
            }
          cv1 = bv1[temp_klim];
          cv2 = bv2[temp_klim];
          yv1 = xv1[temp_klim];
          yv2 = xv2[temp_klim];
          yv3 = xv3[temp_klim];
          csm1 = bsm1[temp_klim];
          csm2 = bsm2[temp_klim];
          ysm1 = xsm1[temp_klim];
          ysm2 = xsm2[temp_klim];
          ysm3 = xsm3[temp_klim];
          csp1 = bsp1[temp_klim];
          csp2 = bsp2[temp_klim];
          ysp1 = xsp1[temp_klim];
          ysp2 = xsp2[temp_klim];
          ysp3 = xsp3[temp_klim];
          csd1 = bsd1[temp_klim];
          zd = bzd1[temp_klim];
          cfm1 = bfm1[temp_klim];
          cfm2 = bfm2[temp_klim];
          cfm3 = bfm3[temp_klim];
          cfp1 = bfp1[temp_klim];
          cfp2 = bfp2[temp_klim];
          cfp3 = bfp3[temp_klim];
        case 4:
          kdv = propv.mdvar;
          ws = kdv >= 20;
          if (ws)
            kdv -= 20;
          w1 = kdv >= 10;
          if (w1)
            kdv -= 10;
          if (kdv < 0 || kdv > 3)
            {
              kdv = 0;
              prop.kwx = std::max(prop.kwx, 2);
            }
        case 3:
          q = std::log(0.133 * prop.wn);
          gm = cfm1 + cfm2 / (std::pow(cfm3 * q, 2.0) + 1.0);
          gp = cfp1 + cfp2 / (std::pow(cfp3 * q, 2.0) + 1.0);
        case 2:
          dexa = std::sqrt(18.0e6 * prop.he[0])
               + std::sqrt(18.0e6 * prop.he[1])
               + std::pow((575.7e12 / prop.wn), THIRD);
        case 1:
          if (prop.dist < dexa)
            de = 130.0e3 * prop.dist / dexa;
          else
            de = 130.0e3 + prop.dist - dexa;
        }
      vmd = curve(cv1, cv2, yv1, yv2, yv3, de);
      sgtm = gm * curve(csm1, csm2, ysm1, ysm2, ysm3, de);
      sgtp = gp * curve(csp1, csp2, ysp1, ysp2, ysp3, de);
      sgtd = sgtp * csd1;
      tgtd = (sgtp - sgtd) * zd;
      if (w1)
        sgl = 0.0;
      else
        {
          q = (1.0 - 0.8 * std::exp(-prop.dist / 50.0e3)) * prop.dh * prop.wn;
          sgl = 10.0 * q / (q + 13.0);
        }
      if (ws)
        vs0 = 0.0;
      else
        vs0 = pow(5.0 + 3.0 * std::exp(-de / 100.0e3), 2.0);
      propv.lvar = 0;
    }
  zt = zzt;
  zl = zzl;
  zc = zzc;
  switch (kdv)
    {
    case 0:
      zt = zc;
      zl = zc;
      break;
    case 1:
      zl = zc;
      break;
    case 2:
      zl = zt;
    }
  if (std::abs(zt) > 3.1 || std::abs(zl) > 3.1 || std::abs(zc) > 3.1)
    prop.kwx = std::max(prop.kwx, 1);
  if (zt < 0.0)
    sgt = sgtm;
  else if (zt <= zd)
    sgt = sgtp;
  else
    sgt = sgtd + tgtd / zt;
  vs = vs0 + std::pow(sgt * zt, 2.0) / (rt + zc * zc)
           + std::pow(sgl * zl, 2.0) / (rl + zc * zc);
  if (kdv == 0)
    {
      yr = 0.0;
      propv.sgc = std::sqrt(sgt * sgt + sgl * sgl + vs);
    }
  else if (kdv == 1)
    {
      yr = sgt * zt;
      propv.sgc = std::sqrt(sgl * sgl + vs);
    }
  else if (kdv == 2)
    {
      yr = std::sqrt(sgt * sgt + sgl * sgl) * zt;
      propv.sgc = std::sqrt(vs);
    }
  else
    { yr = sgt * zt + sgl * zl;
      propv.sgc = std::sqrt(vs);
    }
  avarv = prop.aref - vmd - yr - propv.sgc * zc;
  if (avarv < 0.0)
    avarv *= (29.0 - avarv) / (29.0 - 10.0 * avarv);

  return avarv;
}


void
hzns(const std::vector<double> & pfl, prop_type & prop)
{
  bool wq;
  int np;
  double xi, za, zb, qc, q, sb, sa;

  np = static_cast<int>(pfl[0]);
  xi = pfl[1];
  za = pfl[2] + prop.hg[0];
  zb = pfl[np+2] + prop.hg[1];
  qc = 0.5 * prop.gme;
  q = qc * prop.dist;
  prop.the[1] = (zb - za) / prop.dist;
  prop.the[0] = prop.the[1] - q;
  prop.the[1] = -prop.the[1] - q;
  prop.dl[0] = prop.dist;
  prop.dl[1] = prop.dist;
  if (np >= 2)
    {
      sa = 0.0;
      sb = prop.dist;
      wq = true;
      for (int i = 1; i < np; ++i)
        {
          sa += xi;
          sb -= xi;
          q = pfl[i+2] - (qc * sa + prop.the[0]) * sa - za;
          if (q > 0.0)
            {
              prop.the[0] += q / sa;
              prop.dl[0] = sa;
              wq = false;
            }
          if (!wq)
            {
              q = pfl[i+2] - (qc * sb + prop.the[1]) * sb - zb;
              if (q > 0.0)
                {
                  prop.the[1] += q / sb;
                  prop.dl[1] = sb;
                }
            }
        }
    }
}


void
z1sq1(const std::vector<double> & z,
      double x1, double x2,
      double & z0, double & zn)
{
  double xn, xa, xb, x, a, b;
  int n, ja, jb;
  xn = z[0];
  xa = int(FORTRAN_DIM(x1 / z[1], 0.0));
  xb = xn - int(FORTRAN_DIM(xn, x2 / z[1]));
  if (xb <= xa)
    {
      xa = FORTRAN_DIM(xa, 1.0);
      xb = xn - FORTRAN_DIM(xn, xb + 1.0);
    }
  ja = static_cast<int>(xa);
  jb = static_cast<int>(xb);
  n = jb - ja;
  xa = xb - xa;
  x = -0.5 * xa;
  xb += x;
  a = 0.5 * (z[ja+2] + z[jb+2]);
  b = 0.5 * (z[ja+2] - z[jb+2]) * x;
  for (int i = 2; i <= n; ++i)
    {
      ++ja;
      x += 1.0;
      a += z[ja+2];
      b += z[ja+2] * x;
    }
  a /= xa;
  b *= 12.0 / ((xa * xa + 2.0) * xa);
  z0 = a - b * xb;
  zn = a + b * (xn - xb);
}


double
qtile(int nn, double a[], int ir)
{
  double q, r; 
  int m, n, i, j, j1, i0, k;
  bool done = false;
  bool goto10 = true;

  m = 0;
  n = nn;
  k = std::min(std::max(0, ir), n);
  while (!done)
    {
      if (goto10)
        {
          q = a[k];
          i0 = m;
          j1 = n;
        }
      i = i0;
      while (i <= n && a[i] >= q)
        ++i;
      if (i > n)
        i = n;
      j = j1;
      while (j >= m && a[j] <= q)
        --j;
      if (j < m)
        j = m;
      if (i < j)
        {
          r = a[i];
          a[i] = a[j];
          a[j] = r;
          i0 = i + 1;
          j1 = j - 1;
          goto10 = false;
        }
      else if (i < k)
        {
          a[k] = a[i];
          a[i] = q;
          m = i + 1;
          goto10 = true;
        }
      else if (j > k)
        {
          a[k] = a[j];
          a[j] = q;
          n = j - 1;
          goto10 = true;
        }
      else
        done = true;
    }

  return q;
}


double
qerf(double z)
{
  double b1 = 0.319381530, b2 = -0.356563782, b3 = 1.781477937;
  double b4 = -1.821255987, b5 = 1.330274429;
  double rp = 4.317008, rrt2pi = 0.398942280;
  double qerfv;
  auto x = z;
  auto t = std::abs(x);
  if (t >= 10.0)
    qerfv = 0.0;
  else
    {
      t = rp / (t + rp);
      qerfv = std::exp(-0.5 * x * x) * rrt2pi * ((((b5 * t + b4) * t + b3) * t + b2) * t + b1) * t;
    }
  if (x < 0.0)
    qerfv = 1.0 - qerfv;
  return qerfv;
}


double
d1thx(const std::vector<double> & pfl, double x1, double x2)
{
  int np, ka, kb, n, k, j;
  double d1thxv, sn, xa, xb;

  np = static_cast<int>(pfl[0]);
  xa = x1 / pfl[1];
  xb = x2 / pfl[1];
  d1thxv = 0.0;
  if (xb - xa < 2.0)  // exit out
    return d1thxv;
  ka = static_cast<int>(0.1 * (xb - xa + 8.0));
  ka = std::min(std::max(4, ka), 25);
  n = 10 * ka - 5;
  kb = n - ka + 1;
  sn = n - 1;
  std::vector<double> s(n + 2);
  s[0] = sn;
  s[1] = 1.0;
  xb = (xb - xa) / sn;
  k = static_cast<int>(xa + 1.0);
  xa -= static_cast<double>(k);
  for (j = 0; j < n; ++j)
    {
      while (xa > 0.0 && k < np)
        {
          xa -= 1.0;
          ++k;
        }
      s[j+2] = pfl[k+2] + (pfl[k+2] - pfl[k+1]) * xa;
      xa += xb;
    }
  z1sq1(s, 0.0, sn, xa, xb);
  xb = (xb - xa) / sn;
  for(j = 0; j < n; ++j)
    {
      s[j+2] -= xa;
      xa += xb;
    }
  d1thxv = qtile(n - 1, s.data() + 2, ka - 1) - qtile(n - 1, s.data() + 2, kb - 1);
  d1thxv /= 1.0 - 0.8 * std::exp(-(x2 - x1) / 50.0e3);

  return d1thxv;
}


ITM_EXPORT
void
qlrpfl(const std::vector<double> & pfl, int klimx, int mdvarx,
       prop_type & prop, propa_type & propa, propv_type & propv)
{
  int np, j;
  double xl[2], q, za, zb;

  prop.dist = pfl[0] * pfl[1];
  np = static_cast<int>(pfl[0]);
  hzns(pfl, prop);
  for (j = 0; j < 2; ++j)
    xl[j] = std::min(15.0 * prop.hg[j], 0.1 * prop.dl[j]);
  xl[1] = prop.dist - xl[1];
  prop.dh = d1thx(pfl, xl[0], xl[1]);
  if (prop.dl[0] + prop.dl[1] > 1.5 * prop.dist)
    {
      z1sq1(pfl, xl[0], xl[1], za, zb);
      prop.he[0] = prop.hg[0] + FORTRAN_DIM(pfl[2], za);
      prop.he[1] = prop.hg[1] + FORTRAN_DIM(pfl[np+2], zb);
      for (j = 0; j < 2; ++j)
        prop.dl[j] = std::sqrt(2.0 * prop.he[j] / prop.gme)
                   * std::exp(-0.07 * std::sqrt(prop.dh / std::max(prop.he[j], 5.0)));
      q = prop.dl[0] + prop.dl[1];

      if (q <= prop.dist)
        {
          q = std::pow(prop.dist / q, 2.0);
          for (j = 0; j < 2; ++j)
            {
              prop.he[j] *= q;
              prop.dl[j] = std::sqrt(2.0 * prop.he[j] / prop.gme)
                         * std::exp(-0.07 * std::sqrt(prop.dh / std::max(prop.he[j], 5.0)));
            }
        }
      for(j = 0; j < 2; ++j)
        {
          q = std::sqrt(2.0 * prop.he[j] / prop.gme);
          prop.the[j] = (0.65 * prop.dh * (q / prop.dl[j] - 1.0) - 2.0 * prop.he[j]) / q;
        }
    }
  else
    {
      z1sq1(pfl, xl[0], 0.9 * prop.dl[0], za, q);
      z1sq1(pfl, prop.dist - 0.9 * prop.dl[1], xl[1], q, zb);
      prop.he[0] = prop.hg[0] + FORTRAN_DIM(pfl[2], za);
      prop.he[1] = prop.hg[1] + FORTRAN_DIM(pfl[np+2], zb);
    }
  prop.mdp = -1;
  propv.lvar = std::max(propv.lvar, 3);
  if (mdvarx >= 0)
    {
      propv.mdvar = mdvarx;
      propv.lvar = std::max(propv.lvar, 4);
    }
  if (klimx > 0)
    {
      propv.klim = klimx;
      propv.lvar = 5;
    }
  lrprop(0.0, prop, propa);
}

double
deg2rad(double d)
{ return d * 3.1415926535897 / 180.0; }

//
//  Point-To-Point Mode Calculations
//
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
//
ITM_EXPORT
void
point_to_point(const std::vector<double> & elev, double tht_m, double rht_m,
               double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
               double frq_mhz, int radio_climate, int pol, double conf, double rel,
               double &dbloss, char *strmode, int &errnum)
{
  prop_type prop;
  propv_type propv;
  propa_type propa;
  double zsys = 0;
  double eno, enso, q;
  long ja, jb, i, np;
  double fs;

  prop.hg[0] = tht_m;
  prop.hg[1] = rht_m;
  propv.klim = radio_climate;
  prop.kwx = 0;
  propv.lvar = 5;
  prop.mdp = -1;
  auto zc = qerfi(conf);
  auto zr = qerfi(rel);
  np = static_cast<long>(elev[0]);
  eno = eno_ns_surfref;
  enso = 0.0;
  q = enso;
  if (q <= 0.0)
  {
    ja = 3.0 + 0.1 * elev[0];
    jb = np - ja + 6;
    for (i = ja - 1; i < jb; ++i)
      zsys += elev[i];
    zsys /= (jb - ja + 1);
    q = eno;
  }
  propv.mdvar = 12;
  qlrps(frq_mhz, zsys, q, pol, eps_dielect, sgm_conductivity, prop);
  qlrpfl(elev, propv.klim, propv.mdvar, prop, propa,propv);
  fs = 32.45 + 20.0 * std::log10(frq_mhz) + 20.0 * std::log10(prop.dist / 1000.0);
  q = prop.dist - propa.dla;
  if (int(q) < 0.0)
    strcpy(strmode, "Line-Of-Sight Mode");
  else
    {
      if (int(q) == 0.0)
        strcpy(strmode, "Single Horizon");
      else if (int(q) > 0.0)
        strcpy(strmode, "Double Horizon");
      if (prop.dist <= propa.dlsa || prop.dist <= propa.dx)
        strcat(strmode, ", Diffraction Dominant");
      else if( prop.dist > propa.dx)
        strcat(strmode, ", Troposcatter Dominant");
    }
  dbloss = avar(zr, 0.0, zc, prop, propv) + fs;
  errnum = prop.kwx;
}


//
// pol: 0-Horizontal, 1-Vertical
// radio_climate: 1-Equatorial, 2-Continental Subtropical, 3-Maritime Tropical,
//                4-Desert, 5-Continental Temperate, 6-Maritime Temperate, Over Land,
//                7-Maritime Temperate, Over Sea
// timepct, locpct, confpct: .01 to .99
// elev[]: [num points - 1], [delta dist(meters)], [height(meters) point 1], ..., [height(meters) point n]
// propmode:  Value   Mode
//             -1     mode is undefined
//              0     Line of Sight
//              5     Single Horizon, Diffraction
//              6     Single Horizon, Troposcatter
//              9     Double Horizon, Diffraction
//             10     Double Horizon, Troposcatter
// errnum: 0- No Error.
//         1- Warning: Some parameters are nearly out of range.
//                     Results should be used with caution.
//         2- Note: Default parameters have been substituted for impossible ones.
//         3- Warning: A combination of parameters is out of range.
//                     Results are probably invalid.
//         Other-  Warning: Some parameters are out of range.
//                          Results are probably invalid.
//
ITM_EXPORT
void
point_to_pointMDH(const std::vector<double> & elev, double tht_m, double rht_m,
                  double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
                  double frq_mhz, int radio_climate, int pol, double timepct, double locpct, double confpct, 
                  double & dbloss, int & propmode, double & deltaH, int & errnum)
{
  prop_type prop;
  propv_type propv;
  propa_type propa;
  double zsys = 0;
  double ztime, zloc, zconf;
  double eno, enso, q;
  long ja, jb, i, np;
  double fs;

  propmode = -1;  // mode is undefined
  prop.hg[0] = tht_m;
  prop.hg[1] = rht_m;
  propv.klim = radio_climate;
  prop.kwx = 0;
  propv.lvar = 5;
  prop.mdp = -1;
  ztime = qerfi(timepct);
  zloc = qerfi(locpct);
  zconf = qerfi(confpct);

  np = static_cast<long>(elev[0]);
  eno = eno_ns_surfref;
  enso = 0.0;
  q = enso;
  if (q <= 0.0)
  {
    ja = 3.0 + 0.1 * elev[0];
    jb = np - ja + 6;
    for (i = ja - 1; i < jb; ++i)
      zsys += elev[i];
    zsys /= (jb - ja + 1);
    q = eno;
  }
  propv.mdvar = 12;
  qlrps(frq_mhz, zsys, q, pol, eps_dielect, sgm_conductivity, prop);
  qlrpfl(elev, propv.klim, propv.mdvar, prop, propa, propv);
  fs = 32.45 + 20.0 * std::log10(frq_mhz) + 20.0 * std::log10(prop.dist / 1000.0);
  deltaH = prop.dh;
  q = prop.dist - propa.dla;
  if (int(q) < 0.0)
    propmode = 0;  // Line-Of-Sight Mode
  else
    {
      if (int(q) == 0.0)
        propmode = 4;  // Single Horizon
      else if (int(q) > 0.0)
        propmode = 8;  // Double Horizon
      if (prop.dist <= propa.dlsa || prop.dist <= propa.dx)
        propmode += 1; // Diffraction Dominant
      else if (prop.dist > propa.dx)
        propmode += 2; // Troposcatter Dominant
    }
  dbloss = avar(ztime, zloc, zconf, prop, propv) + fs;      //avar(time,location,confidence)
  errnum = prop.kwx;
}


//
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
//
ITM_EXPORT
void
point_to_pointDH(const std::vector<double> & elev, double tht_m, double rht_m,
                 double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
                 double frq_mhz, int radio_climate, int pol, double conf, double rel,
                 double & dbloss, double & deltaH, int & errnum)
{
  char strmode[100];
  prop_type prop;
  propv_type propv;
  propa_type propa;
  double zsys = 0;
  double zc, zr;
  double eno, enso, q;
  long ja, jb, i, np;
  double fs;

  prop.hg[0] = tht_m;
  prop.hg[1] = rht_m;
  propv.klim = radio_climate;
  prop.kwx = 0;
  propv.lvar = 5;
  prop.mdp = -1;
  zc = qerfi(conf);
  zr = qerfi(rel);
  np = static_cast<long>(elev[0]);
  eno = eno_ns_surfref;
  enso = 0.0;
  q = enso;
  if (q <= 0.0)
  {
    ja = 3.0 + 0.1 * elev[0];
    jb = np - ja + 6;
    for (i = ja - 1; i < jb; ++i)
      zsys += elev[i];
    zsys /= (jb - ja + 1);
    q = eno;
  }
  propv.mdvar = 12;
  qlrps(frq_mhz, zsys, q, pol, eps_dielect, sgm_conductivity, prop);
  qlrpfl(elev, propv.klim, propv.mdvar, prop, propa, propv);
  fs = 32.45 + 20.0 * std::log10(frq_mhz) + 20.0 * std::log10(prop.dist / 1000.0);
  deltaH = prop.dh;
  q = prop.dist - propa.dla;
  if (int(q) < 0.0)
    strcpy(strmode, "Line-Of-Sight Mode");
  else
    {
      if (int(q) == 0.0)
        strcpy(strmode, "Single Horizon");
      else if (int(q) > 0.0)
        strcpy(strmode, "Double Horizon");
      if (prop.dist <= propa.dlsa || prop.dist <= propa.dx)
        strcat(strmode, ", Diffraction Dominant");
      else if (prop.dist > propa.dx)
        strcat(strmode, ", Troposcatter Dominant");
    }
  dbloss = avar(zr, 0.0, zc, prop, propv) + fs;      //avar(time,location,confidence)
  errnum = prop.kwx;
}


//
//  Area Mode Calculations
//
// pol: 0-Horizontal, 1-Vertical
// TSiteCriteria, RSiteCriteria:
//           0 - random, 1 - careful, 2 - very careful
// radio_climate: 1-Equatorial, 2-Continental Subtropical, 3-Maritime Tropical,
//                4-Desert, 5-Continental Temperate, 6-Maritime Temperate, Over Land,
//                7-Maritime Temperate, Over Sea
// ModVar: 0 - Single: pctConf is "Time/Situation/Location", pctTime, pctLoc not used
//         1 - Individual: pctTime is "Situation/Location", pctConf is "Confidence", pctLoc not used
//         2 - Mobile: pctTime is "Time/Locations (Reliability)", pctConf is "Confidence", pctLoc not used
//         3 - Broadcast: pctTime is "Time", pctLoc is "Location", pctConf is "Confidence"
// pctTime, pctLoc, pctConf: .01 to .99
// errnum: 0- No Error.
//         1- Warning: Some parameters are nearly out of range.
//                     Results should be used with caution.
//         2- Note: Default parameters have been substituted for impossible ones.
//         3- Warning: A combination of parameters is out of range.
//                     Results are probably invalid.
//         Other-  Warning: Some parameters are out of range.
//                          Results are probably invalid.
//
// NOTE: strmode is not used at this time.
//
ITM_EXPORT
void
area(long ModVar, double deltaH, double tht_m, double rht_m,
     double dist_km, int TSiteCriteria, int RSiteCriteria, 
     double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
     double frq_mhz, int radio_climate, int pol, double pctTime, double pctLoc,
     double pctConf, double & dbloss, char *, int & errnum)
{
  prop_type prop;
  propv_type propv;
  propa_type propa;
  double zt, zl, zc, xlb;
  double fs;
  long ivar;
  double eps, eno, sgm;
  long ipol;
  int kst[2];

  kst[0] = static_cast<int>(TSiteCriteria);
  kst[1] = static_cast<int>(RSiteCriteria);
  zt = qerfi(pctTime);
  zl = qerfi(pctLoc);
  zc = qerfi(pctConf);
  eps = eps_dielect;
  sgm = sgm_conductivity;
  eno = eno_ns_surfref;
  prop.dh = deltaH;
  prop.hg[0] = tht_m;
  prop.hg[1] = rht_m;
  propv.klim = static_cast<int32_t>(radio_climate);
  prop.ens = eno;
  prop.kwx = 0;
  ivar = static_cast<long>(ModVar);
  ipol = static_cast<long>(pol);
  qlrps(frq_mhz, 0.0, eno, ipol, eps, sgm, prop);
  qlra(kst, propv.klim, ivar, prop, propv);
  if (propv.lvar < 1)
    propv.lvar = 1;
  lrprop(dist_km * 1000.0, prop, propa);
  fs = 32.45 + 20.0 * std::log10(frq_mhz) + 20.0 * std::log10(prop.dist / 1000.0);
  xlb = fs + avar(zt, zl, zc, prop, propv);
  dbloss = xlb;
  if (prop.kwx == 0)
    errnum = 0;
  else
    errnum = prop.kwx;
}

ITM_EXPORT
double
ITMAreadBLoss(long ModVar, double deltaH, double tht_m, double rht_m,
              double dist_km, int TSiteCriteria, int RSiteCriteria, 
              double eps_dielect, double sgm_conductivity, double eno_ns_surfref,
              double frq_mhz, int radio_climate, int pol, double pctTime, double pctLoc,
              double pctConf)
{
    char strmode[200];
    int errnum;
    double dbloss;

    area(ModVar, deltaH, tht_m, rht_m, dist_km, TSiteCriteria, RSiteCriteria, 
         eps_dielect, sgm_conductivity, eno_ns_surfref,
         frq_mhz, radio_climate, pol, pctTime, pctLoc,
         pctConf, dbloss, strmode, errnum);

    return dbloss;
}

ITM_EXPORT
double
ITMDLLVersion()
{ return 7.0; }
