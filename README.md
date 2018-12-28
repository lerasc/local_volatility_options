
# Local Volatility Option Pricing

We wish to price an American put or call option, written on some underlying with value S. 
Usually, it is assumed that S follows a geometric Brownian motion, 

<a href="https://www.codecogs.com/eqnedit.php?latex=$dS&space;=&space;(r-d)&space;S&space;dt&space;&plus;&space;\sigma~&space;S&space;~dW_t$" target="_blank"><img src="https://latex.codecogs.com/gif.latex?$dS&space;=&space;(r-d)&space;S&space;dt&space;&plus;&space;\sigma~&space;S&space;~dW_t$" title="$dS = (r-d) S dt + \sigma~ S ~dW_t$" /></a>,

where r and d are risk-free and dividend rate, respectively. 

More generally, one can assume that S follows a process  

<a href="https://www.codecogs.com/eqnedit.php?latex=$dS&space;=&space;(r-d)&space;S&space;dt&space;&plus;&space;\sigma(S)~&space;dW_t$" target="_blank"><img src="https://latex.codecogs.com/gif.latex?$dS&space;=&space;(r-d)&space;S&space;dt&space;&plus;&space;\sigma(S)~&space;dW_t$" title="$dS = (r-d) S dt + \sigma(S)~ dW_t$" /></a>

is now a general function of S. 
Such option pricing methods are known as *local volatility* models. 
Here, we have implemented a function in C++ that prices local volatility options efficiently, 
relying on the well-known binomial trees. Additionally, we provide a Python binding. 

The core of the implementation, the actual option pricing, is found in LVoptions_.cpp. 
The remaining files can be used to compile the Python interface. 
Build Python binding with <br>
>	python setup.py build_ext --inplace <br>	
Then, import the function from the file Python_wrapper.py