
/*******************************************************************************

This program generates the time series beta_t with the full model and reports
   it to an output file for viewing.

*******************************************************************************/

// Global variables.
double *R, *M;

// Functions found below.
void GetData();
void FullModel ();

#include "..\..\0Functions\Functions.h"

int main () {

   // Read in daily return data for the S&P 500 and JPM. Five years of data
   //    from 2012 through 2016.
   GetData();

   // Compute the beta_t time series with the full model.
   FullModel ();

   Pause ();

}

////////////////////////////////////////////////////////////////////////////////
void FullModel () {

   int t;
   double r, m, mu, sigma2, s2, s_e, s_d;
   FILE *fp;

   // Open the output file.
   fp = fopen ("Beta.txt", "w");

   // Hyper-parameters, from the BetaBayes program.
   mu = 1.302;
   sigma2 = 0.034 * 0.034;

   // Parameters as estimated in MLEParameters.cpp.
   s_e = 0.968;
   s_d = 0.015;

   // This quantity is used frequently below.
   s2 = s_e * s_e;

   for (t = 1; t <= 1258; t++) {

      r = R[t];
      m = M[t];

      // Update mu and sigma2.
      mu = (s2*mu + (sigma2 + s_d*s_d)*r*m) / (s2 + m*m*(sigma2 + s_d*s_d));
      sigma2 = s2*(sigma2 + s_d*s_d) / (s2 + m*m*(sigma2 + s_d*s_d));

      // Report the updated MAP estimate of beta. Time is in months.
      // The normal density function maximizes at its mean.
      fprintf (fp, "%5.3f  %5.3f\n", 60.0*t/1258.0, mu);

   }

   // Close the output file.
   fclose (fp);

   printf ("View the full model beta time series with BetaPlot.tex.\n");
   printf ("End of period MAP beta is %8.3f.\n", mu);

}

////////////////////////////////////////////////////////////////////////////////
// Allocate array space and read in S&P 500 and JPM daily returns for a 5 year
//    period.
////////////////////////////////////////////////////////////////////////////////
void GetData() {

   int t;
   char input[100];
   FILE *fp;

   // Array for JPM returns.
   R = (double *) calloc (1300, sizeof (double));

   // Array for S&P500 returns.
   M = (double *) calloc (1300, sizeof (double));

   fp = fopen ("JPMSPDaily.txt", "r");

   // Read in column headers
   fgets (input, 99, fp);

   // Now read in return data for 1258 days.
   for (t = 1; t <= 1258; t++) {

      fgets (input, 99, fp);
      sscanf (input, "%lf %lf", M+t, R+t);

   }

   fclose (fp);

   return;

}


