
# Local Volatility Option Pricing

We wish to price an American put or call option, written on some underlying with value S. 
Usually, it is assumed that S follows a geometric Brownian motion. 
More generally, one can assume that S follows a process where the volatility is itself dependent on S. 
Such option pricing methods are known as *local volatility* models. 
Here, we have implemented a function in C++ that prices local volatility options efficiently, relying on the well-known binomial trees. 
Additionally, we provide a Python binding based on sip (https://www.ics.uci.edu/~dock/manuals/sip/sipref.html). 

The core of the implementation, the actual option pricing, is found in LVoptions_.cpp. 
The remaining files just serve to build the Python interface. 
Build the Python binding with 

>	python setup.py build_ext --inplace <br>	

Then, import the function from the file Python_wrapper.py