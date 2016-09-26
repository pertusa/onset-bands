/****************
Copyright (c) 2005-2009 Antonio Pertusa Ibáñez <pertusa@dlsi.ua.es>
Copyright (c) 2005-2009 José Manuel Iñesta Quereda <inesta@dlsi.ua.es>
Copyright (c) 2009 Universidad de Alicante

This onset detection system is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or (at your
 option) any later version.

This onset detection system is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code.  If not, see <http://www.gnu.org/licenses/>.

 Comments are welcomed
 
**************/

#include "changes.h"
#include <math.h>

/*-------------------------------------------
 Peak selection algorithm
----------------------------------------------*/

void peakselection(vector<double> &onsetsvect, int total_time) 
{
	vector<double> tmpvector(total_time);

	// Inicialization
	for (int i=0; i<total_time; i++)
		tmpvector[i]=0;

	// Peak detection	
	for (int i=1; i< total_time-1; i++) {
		if (onsetsvect[i] > onsetsvect[i-1] && onsetsvect[i] >= onsetsvect[i+1])
			tmpvector[i]=onsetsvect[i];
	}
	// Copy output
	onsetsvect=tmpvector;
}

/*-------------------------------------------
 Compute temporal differences for each band
----------------------------------------------*/

void applyderivative(vector<double> &derivativevector, int context) 
{
	int ntime=derivativevector.size();

	vector<double> tmpvector(derivativevector);

	// Derivative if no context is considered (context=0)

	derivativevector[0]=0;
	if (!context) {
		for (int t=1; t<ntime; t++) derivativevector[t]=tmpvector[t]-tmpvector[t-1];
	}

	// Modified derivative when considering context

	else {
		for (int t=context; t<ntime-context; t++) 
		{
			double numerator=0.0;
			for (int i=1; i<=context; i++) 
				numerator+=i*(tmpvector[t+i]-tmpvector[t-i]);
			derivativevector[t]=numerator;
		}
		for (int t=0; t<context; t++) derivativevector[t]=0;
		for (int t=ntime-context; t<ntime; t++) derivativevector[t]=0;
	}
	
	// Keep only positive derivative values
	for (int i=0; i<ntime; i++)
		if (derivativevector[i]<0) derivativevector[i]=0;

}

/*----------------------------------
 Onset detection function o[t]
----------------------------------*/

void onsetfunction(Tmatrix bands, double maxbandsval, vector<double>&
output, int context, float mu) 
{
	int nbands=bands[0].size();
	int ntime=bands.size();

	// Create temporal matrix for storing results after relative difference function
	Tmatrix differential;
	differential.resize(ntime);
	for (int i=0; i<ntime; i++)
		differential[i].resize(nbands);

	// Compute relative difference function for each band in time

	vector<double> derivativevector(ntime);
	for (int i=0; i<nbands; i++) {

		for (int j=0; j<ntime; j++) 
			derivativevector[j]=bands[j][i];

		applyderivative(derivativevector,context);

		// Copy to temporal matrix 
		for (int j=0; j<ntime; j++)
			differential[j][i]=derivativevector[j];
	}

	// Compute o(t) for each time frame.

	output.resize(ntime);
	for (int t=context; t<ntime-context; t++) 
	{
		double sumabands=0, sumadiff=0;

		for (int j=0; j<nbands; j++)  {

			sumadiff+=differential[t][j];

			if (!context) {
				sumabands+=bands[t][j];
			}
			else {
				for (int k=1; k<=context; k++)
					sumabands+=k*bands[t+k][j];
			}
		}

		if (sumabands>mu) {
			output[t]=sumadiff/sumabands;
		}
		else output[t]=0;
	}
}

/*----------------------------------
 Thresholding 
----------------------------------*/

void applythreshold(vector<double> &vectchanges, double maxbandsval, int
context, float theta)
{
	double threshold;
	threshold= theta; 

	for (int i=0; i<(int)vectchanges.size(); i++)
		if (vectchanges[i] < threshold)
			vectchanges[i]=0;
}

/*----------------------------------
 Main onset detection function
----------------------------------*/

void computeonsets(int total_time, double *onsetsvect, Tmatrix &bands,
double maxbandsval, double timeres, int context, float theta, float mu)
{
	// Initialization
	vector<double> vectchanges;
	for (int i=0; i<total_time; i++) 
		onsetsvect[i]=0;

	// Onset detection	
	onsetfunction(bands,maxbandsval,vectchanges,context,mu);
	// Peak detection
	peakselection(vectchanges, total_time);
	// Thresholding
	applythreshold(vectchanges,maxbandsval,context,theta);

	for (int i=0; i<total_time; i++)
		onsetsvect[i]=vectchanges[i];
	
	// print onset times
	cout.precision(4);
        cout.setf(ios::fixed);
        for (int i=0; i<total_time; i++) {
          if (vectchanges[i]!=0) 
             cout << i*timeres << endl;
        }
}
