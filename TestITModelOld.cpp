//
//  COPYRIGHT:  Copyright 2006-2015
//              Alion Science and Technology
//              US Govt retains rights in accordance
//              with DoD FAR Supp 252.227 - 7013.
//


#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>


#if defined(_WIN32) || defined(_WIN64)
#  define DllExport __declspec(dllexport)
#else
#  define DllExport [[gnu::visibility("default")]]
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


/**
 *  This is the classic TIREM test suite adapted for ITM.
 *  It consists of 23 tests with hardcoded terrain profile and parameters.
 *  Answers are checked against stored values.
 */

void WriteProfile(const std::string & filename, const std::vector<double> & elevation);

void TestITM();

int
main()
{
    TestITM();
}

void
TestITM()
{

    enum Status
    {
        PASS,
        WARN,
        FAIL
    };

    //  Test results from TIREM-3.18
    const unsigned long num_tests = 23;
    const double loss_318[num_tests] {
      166.6181, 135.1391, 111.7738, 186.2227, 186.2227,
      185.3160, 158.4971, 133.6371, 207.3199, 251.1229,
      186.5378, 178.6134, 186.2227, 186.2227, 251.6470,
      140.0370, 140.0370, 98.64220, 102.0177, 117.8867,
      178.5336, 110.8756, 146.3228
    };
    const double fsplss_318[num_tests] {
      111.7177, 111.7187, 111.7738, 109.2815, 109.2815,
      109.2815, 109.2820, 89.28152, 129.2815, 152.8033,
      109.2815, 109.2815, 109.2815, 109.2815, 152.8033,
      100.2264, 100.2264, 85.50602, 85.50602, 85.58260,
      110.8756, 110.8756, 104.1510
    };
    double loss_delta[num_tests], abs_loss_delta[num_tests];
    double fsplss_delta[num_tests], abs_fsplss_delta[num_tests];
    int status[num_tests], num_passes = 0, num_warnings = 0, num_errors = 0;
    std::vector<std::string> polar_string{ "ERR", "VERT", "HORZ" };

    const double dstinc = 500.0;
    const double elev[1 + 1 + 250] {
	    250.0,
		dstinc,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   2.0,   3.0,   1.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        2.0,   3.0,   5.0,   5.0,   3.0,
        3.0,   2.0,   1.0,   5.0,   6.0,
        6.0,  11.0,  26.0,  38.0,  52.0,
       61.0,  63.0,  74.0,  91.0, 103.0,
      117.0, 126.0, 139.0, 160.0, 177.0,
      185.0, 190.0, 198.0, 210.0, 223.0,
      235.0, 244.0, 245.0, 249.0, 253.0,
      260.0, 268.0, 275.0, 283.0, 285.0,
      297.0, 301.0, 304.0, 307.0, 309.0,
      311.0, 312.0, 311.0, 310.0, 308.0,
      307.0, 305.0, 305.0, 305.0, 305.0,
      308.0, 313.0, 325.0, 338.0, 350.0,
      351.0, 345.0, 340.0, 325.0, 318.0,
      326.0, 347.0, 362.0, 363.0, 346.0,
      348.0, 364.0, 369.0, 382.0, 382.0,
      370.0, 375.0, 390.0, 382.0, 347.0,
      323.0, 338.0, 376.0, 413.0, 409.0,
      384.0, 393.0, 442.0, 469.0, 460.0,
      450.0, 444.0, 447.0, 461.0, 476.0,
      455.0, 434.0, 431.0, 417.0, 432.0,
      452.0, 457.0, 463.0, 464.0, 445.0,
      416.0, 411.0, 437.0, 475.0, 497.0,
      497.0, 508.0, 552.0, 593.0, 605.0,
      615.0, 635.0, 651.0, 656.0, 671.0,
      717.0, 761.0, 633.0, 481.0, 356.0,
      233.0, 171.0, 131.0, 105.0, 118.0,
      183.0, 260.0, 229.0, 162.0, 104.0,
       61.0,  71.0, 103.0,  95.0,  61.0,
       63.0,  62.0,  79.0, 107.0, 104.0,
       87.0,  70.0,  59.0,  52.0,  49.0,
       50.0,  40.0,  35.0,  36.0,  28.0,
       27.0,  27.0,  18.0,  10.0,   5.0,
        5.0,   3.0,   3.0,   2.0,   3.0,
        2.0,   1.0,   1.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0,
        0.0,   0.0,   0.0,   0.0,   0.0
    };

    WriteProfile("Profile.txt", std::vector<double>{elev, elev + 250});

    std::cout.precision( 7 );
    std::cout.flags( std::cout.flags() | std::ios::showpoint );

    double tx_antenna_height = 10.0;
    double rx_antenna_height = 10.0;
    double frequency = 100.0;
    double refractivity = 301.0;
    double conductivity = 0.028;
    double permittivity = 15.0;
    int polarization = 1;
    int climate = 5;
    double confidence = 0.50, reliability = 0.50;
    double dbloss = 0.0;
    char strmode[200] = "";
    int errnum = 0;

    unsigned long num_profile_points = 0;

    std::string vrsion;

    unsigned long offset = 0;

    //   Set variables for each run
    for ( unsigned long i = 1; i <= num_tests; ++i )
    {
        if ( i == 1 )
        {
            num_profile_points  =  185;
            offset =  60;
        }
        else if ( i == 2 )
        {
            tx_antenna_height = 1000.0;
        }
        else if ( i == 3 )
        {
            tx_antenna_height = 10000.0;
        }
        else if ( i == 4 )
        {
            tx_antenna_height =  10.0;
            num_profile_points  =  140;
            offset =  80;
        }
        else if ( i == 6 )
        {
            rx_antenna_height = 100.0;
        }
        else if ( i == 7 )
        {
            rx_antenna_height = 1000.0;
        }
        else if ( i == 8 )
        {
            rx_antenna_height = 10.0;
            frequency = 10.0;
        }
        else if ( i == 9 )
        {
            frequency = 1000.0;
        }
        else if ( i == 10 )
        {
            frequency = 15000.0;
        }
        else if ( i == 11 )
        {
            frequency = 100.0;
            refractivity = 201.0;
        }
        else if ( i == 12 )
        {
            refractivity = 400.0;
        }
        else if ( i == 13 )
        {
            refractivity = 301.0;
            polarization = 0;
        }
        else if ( i == 14 )
        {
            polarization = 1;
        }
        else if ( i == 15 )
        {
            frequency = 15000.0;
        }
        else if ( i == 16 )
        {
            frequency = 100.0;
            num_profile_points  = 50;
            offset = 160;
            conductivity = 0.01;
            permittivity = 8.0;
        }
        else if ( i == 17 )
        {
            conductivity = 0.5;
            permittivity = 40.0;
        }
        else if ( i == 18 )
        {
            conductivity = 0.028;
            permittivity = 15.0;
            num_profile_points  = 10;
            offset = 60;
        }
        else if ( i == 19 )
        {
            tx_antenna_height = 0.1;
            rx_antenna_height = 0.1;
        }
        else if ( i == 20 )
        {
            offset = 190;
            tx_antenna_height = 10.0;
            rx_antenna_height = 10.0;
        }
        else if ( i == 21 )
        {
            num_profile_points  = 168;
            offset = 70;
            tx_antenna_height = 1.0;
            rx_antenna_height = 1.0;
        }
        else if ( i == 22 )
        {
            tx_antenna_height = 2000.0;
            rx_antenna_height = 2000.0;
        }
        else if ( i == 23 )
        {
            num_profile_points  = 78;
            offset = 0;
            tx_antenna_height = 2.0;
            rx_antenna_height = 2.0;
        }

        std::cout << "  ***  RUN # " << std::setw(2) << i << " ***" << std::endl;

        bool write_offsets = true;
        if ( write_offsets )
        {
            std::cout << std::endl;
            std::cout << " OFFSET = " << offset << std::endl;
            std::cout << " START = " << offset + 1 << std::endl;
            std::cout << " END = " << offset + num_profile_points << std::endl;
        }

        std::vector<double> elevation(1 + 1 + num_profile_points);
		elevation[0] = static_cast<double>(num_profile_points);
		elevation[1] = dstinc;
		std::copy(elev + offset, elev + offset + num_profile_points, elevation.begin() + 2);

        // Write debugging profile.
        //if (i == 18)
        //  WriteProfile("Profile18.txt", num_profile_points, dstinc, elev + offset);

        point_to_point(elevation.data(), tx_antenna_height, rx_antenna_height,
                       permittivity, conductivity, refractivity,
                       frequency, climate, polarization, confidence, reliability,
                       dbloss, strmode, errnum);
        double fsplss = 32.45 + 20.0 * log10(frequency) + 20.0 * log10(elevation[0] * elevation[1] / 1000.0);


        // Display ITM output

        std::cout <<std::endl;
        std::cout << " INPUT:  TX HT  = " << std::setw(15) << tx_antenna_height << " (M) " << std::endl;
        std::cout << "         RX HT  = " << std::setw(15) << rx_antenna_height << " (M) " << std::endl;
        std::cout << "         FREQ   = " << std::setw(15) << frequency << " (MHZ) " << std::endl;
        std::cout << "         POINTS = " << std::setw(12) << static_cast<long>(elevation[0]) << std::endl;
        std::cout << "         PTHLEN = " << std::setw(15) << elevation[0] * elevation[1] << " (M) " << std::endl;
        std::cout << "         REFRAC = " << std::setw(15) << refractivity << " (N) " << std::endl;
        std::cout << "         CONDUC = " << std::setw(15) << conductivity << " (S/M) " << std::endl;
        std::cout << "         PERMIT = " << std::setw(15) << permittivity << std::endl;
        std::cout << "         POLARZ = " << polar_string[polarization] << std::endl;
        std::cout << " OUTPUT: VER #  = " << ITMDLLVersion() << std::endl;
        std::cout << "         MODE   = " << strmode << std::endl;
        std::cout << "         LOSS   = " << std::setw(15) << dbloss << " (DB) " << std::endl;
        std::cout << "         FSPLSS = " << std::setw(15) << fsplss << " (DB) " << std::endl;
        std::cout << "         ERROR  = " << errnum << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

        //
        //  Compare against old TIREM results.
        //
        loss_delta[ i - 1 ] = dbloss - loss_318[ i - 1 ];
        abs_loss_delta[ i - 1 ] = std::abs( loss_delta[ i - 1 ] );
        fsplss_delta[ i - 1 ] = fsplss - fsplss_318[ i - 1 ];
        abs_fsplss_delta[ i - 1 ] = std::abs( fsplss_delta[ i - 1 ] );
        if ( abs_loss_delta[ i - 1 ] > 1.0
          || abs_fsplss_delta[ i - 1 ] > 1.0 )
        {
            status[ i - 1 ] = FAIL;
            ++num_errors;
        }
        else if ( abs_loss_delta[ i - 1 ] > 0.1
               || abs_fsplss_delta[ i - 1 ] > 0.1 )
        {
            status[ i - 1 ] = WARN;
            ++num_warnings;
        }
        else
        {
            status[ i - 1 ] = PASS;
            ++num_passes;
        }
    }

    std::cout << std::endl;
    std::cout << "Passes   : " << std::setw( 2 ) << num_passes << std::endl;
    std::cout << "Warnings : " << std::setw( 2 ) << num_warnings << std::endl;
    for ( unsigned long i = 0; i < num_tests; ++i )
    {
        if ( status[i] == WARN )
        {
            std::cout << "  Test " << std::setw( 2 ) << i + 1
                      << "  Delta " << std::setw( 8 ) << loss_delta[ i ]
                      << std::endl;
        }
    }
    std::cout << "Errors   : " << std::setw( 2 ) << num_errors << std::endl;
    for ( unsigned long i = 0; i < num_tests; ++i )
    {
        if ( status[i] == FAIL )
        {
            std::cout << "  Test " << std::setw( 2 ) << i + 1
                      << "  Delta " << std::setw( 8 ) << loss_delta[ i ]
                      << std::endl;
        }
    }

    return;
}

//  Write terrain profile to a file for debugging.
void
WriteProfile(const std::string & filename,
             const std::vector<double> & elevation)
{
    std::ofstream bug(filename);
    for (auto elev : elevation)
    {
        bug << elev << '\n' << std::showpoint;
    }
}
