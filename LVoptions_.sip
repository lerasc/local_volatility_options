%Module LVoptions_

%ModuleHeaderCode
    #include "LVoptions_.h"
%End

struct CBack {
    virtual double operator()(double x) const;
};

double LVoption	(		const CBack& f,
          				double price,
						double strike,
						double r,
						double d,
						double T,
						int N,
						int type);

