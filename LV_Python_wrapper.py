
import numpy as np

from LVoptions_ import *

def LV_option( option_type, price, strike, sigma, r, d, T, N ):
	"""
	Python wrapper around C++ implemenation of local volatility pricer (see LVoptions_.cpp for details). 

	input:
	-----
	option_type:	'P' or 'C'
	price:			spot price
	strike:			strike price
	sigma:			volatility function, according to convention dS = (r-d) S dt + sigma(S) dW_t 
	r:				risk free rate
	d:				dividend rate
	T:				time to maturity (in units of years)
	N:				size of binomial tree (see LVoptions_.cpp)

	return value:
	------------
	value of the option

	details:
	--------
 	The number of time discretizaton steps N should not be choosen too large since this can lead to numerical 
 	instabilities. Ideally, N is not larger than 50. But since such large values can lead to fluctuations, here we 
 	calculate the option price both for N and for N +1 and return the average of the two results. This helps to reduce 
 	fluctuations. 		
	"""

	# the following code is just an interface to the pre-compiled C++ code. We need the 'cback' wrapper to pass an
	# adequately callable function to C++. 

	assert(option_type in ['P','C']),"Option type must be 'C' or 'P'."

	def cback(f):
		class PyCBack(CBack):
			def __call__(self, x):
				return f(x)
		return PyCBack()

	if(option_type=='C'): 	Type = 0
	else: 					Type = 1

	option1 = LVoption(cback(sigma),price,strike,r,d,T,N,Type)
	option2 = LVoption(cback(sigma),price,strike,r,d,T,N+1,Type)
	option  = 0.5*(option1+option2)

	return option


if __name__=='__main__':

	# test code: 
	price 		= 20.0
	strike 		= 18.0
	r 			= 0.00
	d 			= 0.00
	T 			= 1.0
	N 			= 20
	sigma 		= lambda x: 0.2*x**1.0 							# geometric Brownian motion
	Co 			= LVPython('C',price,strike,sigma,r,d,T,N)		# price of call option 

	print('Call option price: ',Co)
	