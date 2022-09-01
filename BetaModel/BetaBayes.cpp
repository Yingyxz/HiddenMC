
/*******************************************************************************

This program imitates the regression (MLE) calculation via Bayes with
   a verrrry flat prior normal distribution.

*******************************************************************************/

// Global variables.
double *R, *M;

// Functions found below.
void GetData();
void Bayes ();

#include "..\..\0Functions\Functions.h"

int main () {

   // Read in daily return data for the S&P 500 and JPM. Five years of data
   //    from 2012 through 2016.
   GetData();

   // Compute a constant beta via Bayesian inference.
   Bayes ();

   Exit ();

}

////////////////////////////////////////////////////////////////////////////////
void Bayes () {

   int t;
   double r, m, mu, sigma2, s_e, s2;

   // This is s_e from the Regress function, but the value of s_e is virtually
   //    irrelevant (try s_e = 10.0).
   s_e  = 0.974;

   // This quantity is used frequently below.
   s2 = s_e * s_e;

   // Take the hyper standard deviation to be very large. Doesn't matter
   //    what mu is if sigma2 is this large.
   mu = 0.0;
   sigma2 = 100.0*100.0;

   for (t = 1; t <= 1258; t++) {

      r = R[t];
      m = M[t];

      // Update mu and sigma2.
      mu = (s2*mu + sigma2*r*m) / (s2 + m*m*sigma2);
      sigma2 = s2*sigma2 / (s2 + m*m*sigma2);

   }

   // The normal density function maximizes at the mean.
   printf ("MAP estimate of beta = %8.3f.\n", mu);

   return;

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



