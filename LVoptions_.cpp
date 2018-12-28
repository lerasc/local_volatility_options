
#include "LVoptions_.h"
#include <algorithm>
#include <iostream>
#include <math.h> 
using namespace std;

double LVoption(			const CBack& sigma,
	          				double price,
							double strike,
							double r,
							double d,
							double T,
							int N,
							int type)
{
	/*
	Price of an option with underlying local volatility function sigma(S), i.e. the underlying follows a stochastic
	process of the form 

		dS = (r-d) S dt + sigma(S) dW_t 																			(1)

	for some function sigma. See for instance [1] for details on binomial local volatility pricing. 

	input parameters:
	----------------
	double price:     	asset price
	double strike:		strike price
	double r:			annual risk-free interest rate
	double d:			dividend yield
	double T:			time to maturity
	auto sigma:			the volatility function (see details below)
	int N:				number of time steps
	int type:			0 for call and 1 for put

	return value:
	------------
	double:				value of the option

	references:
	----------
	[1]	Local volatility modelling, Van Der Kamp, (2009), 
		available at https://essay.utwente.nl/59223/1/scriptie__R_van_der_Kamp.pdf

	details:
	-------
	Note that here the sigma is according to the definition (1) whereas the implementation described in [1] is according 
	to the convention dS/S = ... dt + sigma(S) dWt. Therefore, we have to divide sigma(S) by S in each function call to 
	follow [1]. 

 	The number of time discretizaton steps N should not be choosen too large since this can lead to numerical 
 	instabilities. Ideally, N is not larger than 50. A trick to reduce numerical fluctuations is to calculate the price
 	for both N and N+1, and then take the average of the two results. 		
	*/


	/********************** Initialization **********************/
	int i,j,k,l; 				// j = time-, i,k,l = spatial-components,
	double num,denom;			// stores numerator and denominator
	double prob; 				// probability of up move
	double h; 					// help variable
	double dt = T/N; 			// time step size
	double us[N+1];				// stores upmove probabilities
	double ds[N+1];				// stores downmove probabilities
	double S[N+1][N+1]; 		// sock price at node (i,j)
	double C[N+1][N+1];			// call price at node (i,j)
		

	/************* compute stock price at each node *************/
	S[0][0] = price; 		// initialization of spot price

	for(j = 0; j < N; j++)	// iterate over time slices
	{

		// start at k = [j/2] for optimal stability
		k 		= j/2;
		h 		= 0.5*exp(dt*(d-r))+0.5*exp((r-d+pow(sigma(S[j][k])/S[j][k],2))*dt);
		us[k] 	= h + sqrt(pow(h,2)-1);
		ds[k] 	= h - sqrt(pow(h,2)-1);

		// determine all the other up and down down to k
		for(l=k-1;l>=0;l--)
		{
			ds[l]  	= S[j][l+1]*us[l+1]/S[j][l];
			num 	= exp((2*(r-d)+pow(sigma(S[j][l])/S[j][k],2))*dt)-ds[l]*exp((r-d)*dt);
			denom 	= exp((r-d)*dt) - ds[l];

			if(num == 0.0 and denom == 0.0) us[l] = 1.0; // in case of 0 volatility 
			else us[l] = num/denom;  // generic case

			us[l] 	= max(us[l], 1 + 1e-4); // in case of very low volatility
			ds[l] 	= min(ds[l], 1 - 1e-4); // in case of very low volatility
		}

		// determine all the other up and down below k
		for(l=k+1;l<=j;l++) 
		{
			us[l]  	= S[j][l-1]*ds[l-1]/S[j][l];
			num 	= us[l]*exp((r-d)*dt)-exp((2*(r-d)+pow(sigma(S[j][l])/S[j][k],2))*dt); 
			denom 	= us[l] - exp((r-d)*dt); 

			if(num == 0.0 and denom == 0.0) ds[l] = 1.0; // in case of 0 volatility 
			else ds[l] = num/denom; // generic case

			us[l] 	= max(us[l], 1 + 1e-4); // in case of very low volatility
			ds[l] 	= min(ds[l], 1 - 1e-4); // in case of very low volatility
		}

		// calculate all the states S_i^{j+1} at time j+1 
		for(i = 0; i < j+1; i++) S[j+1][i] = S[j][i]*us[i]; 

		S[j+1][j+1] = S[j][j]*ds[j];
		
	}

	/*************** compute terminal payoffs ***************/
	for(i = N; i >= 0; i--)
	{
		if(type == 0) C[N][i] = max(S[N][i]-strike,0.0);
		else C[N][i] = max(strike-S[N][i],0.0);
	}


	/*************** work backwards ***************/
	for(j = N-1; j>= 0; j--)
	{
		for(i = j; i>= 0; i--)
		{
			prob 	= (S[j][i]*exp((r-d)*dt)-S[j+1][i+1])/(S[j+1][i]-S[j+1][i+1]);
			prob 	= min(prob,1.0);
			prob 	= max(prob,0.0); 
			C[j][i] = exp(-r*dt)*(prob*C[j+1][i] + (1-prob)*C[j+1][i+1]); 
			
			if(type == 0) C[j][i] = max(S[j][i]-strike,C[j][i]);
			else C[j][i] = max(strike-S[j][i],C[j][i]);
		}
	}

	if(C[0][0] < 0.0) return 0.0; // negative due to probability above 1 or below 0
	return C[0][0];
}