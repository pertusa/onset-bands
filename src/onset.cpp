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

#include "defines.h"
#include "processwav.h"
#include "changes.h"
#include "info.h"
#include "params.h"
#include "bands.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

pinfo info;   // Wavefile information (global variable for efficiency)

/*----------------------------------------
  Get parameters from command line
 ---------------------------------------*/

void getParams(int argc, char *argv[], int &context, float &mu, bool &imu, float &theta, bool &csound, bool &genre, string &Oname)
{
	bool error=false;

	if (argc<2) error=true;
	
        for (int i=2; i<argc && !error; i++)
        {
        	if (!strcmp(argv[i],"-c")) {
			i++;
			if (i>=argc) error=true;
			else context=atoi(argv[i]);
        	}
        	else if (!strcmp(argv[i],"-m"))
        	{
			i++;
			if (i>=argc) error=true;
			else {
				mu=atof(argv[i]);
				imu=true;
			}
        	}
        	else if (!strcmp(argv[i],"-t"))
        	{
			i++;
			if (i>=argc) error=true;
			else theta=atof(argv[i]);
        	}
        	else if (!strcmp(argv[i],"-csound"))
        	{
        		csound=true;
        	}
        	else if (!strcmp(argv[i],"-o"))
        	{
        		genre=true;
        		i++;
        		if (i>=argc) error=true;
        		else Oname=argv[i];
        	}
        	else error=true;
        }
        if (error) {
        	cerr << "Syntax: " << argv[0] << " <filename.wav> [-c context | -t theta | -m mu | -csound | -o onsetfunction]" << endl;
        	cerr << "Default values: context=0, theta=" << NEWPAR << ", and mu= " << kSILENCE22 << endl;
        	cerr << "-o option prints values of o[t] over the threshold" << endl;
        	cerr << "-csound option creates files csound.orc and csound.sco that can be compiled using CSound" << endl;
        	exit(-1);
        }
}

/*------------------------------------------------------------------------------

 Function to print o[t] into a file (-o option)

-------------------------------------------------------------------------------*/

void printO(string filename, double *onsetsvector, int n)
{
	ofstream f(filename.c_str());
	
	if (f.is_open())
	{	
		for (int i=0; i<n; i++)
			f<< onsetsvector[i] << endl;
		f.close();
	}
	else cerr << "Error: could not open file " << filename << endl;	
}

/*------------------------------------------------------------------------------
	MAIN
-------------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {

	// Process arguments

	int context=0; 	// Default values
	float mu, theta=NEWPAR;
	bool imu=false, csound=false;
	bool genre=false; string genrefilename;
	getParams(argc, argv, context, mu, imu, theta, csound, genre, genrefilename);

	// Read wavefile, perform STFT and compute spectrum accross one semitone filter bank

	cerr << "Scanning input data..." << endl;
	Tmatrix outbandmatrix(1);
	vectorbands spectralbands;
	int n_time=input_data_wav(argv[1], spectralbands, outbandmatrix, info);

	// Onset detection. Onsets are stored in onsetsvector 

	cerr << "Computing onsets..." << endl;

	double *onsetsvector;
	if (!imu) { // Default silence threshold (depends on the samplerate)
	  if (info.samplerate==22050) mu=kSILENCE22;
	  else mu=kSILENCE44;
	}
	onsetsvector=(double*)malloc(sizeof(double)*(n_time));
	computeonsets(n_time,onsetsvector,outbandmatrix,info.maxbandsval,info.time_resolution,context,theta,mu);

        // generate CSound files with added ticks where onsets were detected
	if (csound) {
		generateorc(argv[1],info.samplerate);
		generatesco(onsetsvector, n_time, info.time_resolution);
	}
	if (genre) 
		printO(genrefilename,onsetsvector,n_time);
	
	delete onsetsvector;
}
