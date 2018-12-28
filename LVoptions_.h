#if !defined(SIPTEST_H_INCLUDED)
#define SIPTEST_H_INCLUDED

struct CBack {
    virtual double operator()(double x) const {
        return x;
    }
    virtual ~CBack() {}
};

double LVoption(	const CBack& f,
          				double price,
						double strike,
						double r,
						double d,
						double T,
						int N,
						int type);

#endif
