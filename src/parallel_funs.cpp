/*******************************************************************
 * ________________________                                        *
 * || Parallel functions ||                                        *
 * ------------------------                                        *
 *                                                                 *
 * Author: Laurent R. Berge                                        *
 *                                                                 *
 * Group of functions doing simple things... but in parallel.      *
 *                                                                 *
 * The functions don't do much more than what their names suggest. *
 *                                                                 *
 ******************************************************************/


#include <Rcpp.h>
#include <math.h>
#include <vector>
#ifdef _OPENMP
    #include <omp.h>
#else
    #define omp_get_thread_num() 0
    #define omp_get_num_threads() 1
#endif
#include <cmath>
#include <stdio.h>
#include <Rmath.h>

using namespace Rcpp;

// [[Rcpp::plugins(openmp)]]

// This file contains misc femlm functions parallelized with the omp library

// [[Rcpp::export]]
NumericVector cpppar_exp(NumericVector x, int nthreads){
	// parallel exponentiation using omp

	int n = x.length();
	NumericVector res(n);

	#pragma omp parallel for num_threads(nthreads)
	for(int i = 0 ; i < n ; ++i) {
		res[i] = exp(x[i]);
	}

	return(res);
}

// [[Rcpp::export]]
NumericVector cpppar_log(NumericVector x, int nthreads){
	// parallel exponentiation using omp

	int n = x.length();
	NumericVector res(n);

	#pragma omp parallel for num_threads(nthreads)
	for(int i = 0 ; i < n ; ++i) {
		res[i] = log(x[i]);
	}

	return(res);
}

// [[Rcpp::export]]
NumericVector cpppar_log_a_exp(int nthreads, double a, NumericVector mu, NumericVector exp_mu){
	// faster this way

	int n = mu.length();
	NumericVector res(n);

	#pragma omp parallel for num_threads(nthreads)
	for(int i=0 ; i<n ; ++i) {
		if(mu[i] < 200){
			res[i] = log(a + exp_mu[i]);
		} else {
			res[i] = mu[i];
		}
	}

	return(res);
}

// [[Rcpp::export]]
NumericVector cpppar_lgamma(NumericVector x, int nthreads){
	// parallel lgamma using omp

	int n = x.length();
	NumericVector res(n);

	#pragma omp parallel for num_threads(nthreads)
	for(int i = 0 ; i < n ; ++i) {
		res[i] = lgamma(x[i]);
	}

	return(res);
}

// [[Rcpp::export]]
NumericVector cpppar_digamma(NumericVector x, int nthreads){
	// parallel digamma using omp

	int n = x.length();
	NumericVector res(n);

	#pragma omp parallel for num_threads(nthreads)
	for(int i = 0 ; i < n ; ++i) {
		res[i] = R::digamma(x[i]);
	}

	return(res);
}

// [[Rcpp::export]]
NumericVector cpppar_trigamma(NumericVector x, int nthreads){
	// parallel trigamma using omp

	int n = x.length();
	NumericVector res(n);

	#pragma omp parallel for num_threads(nthreads)
	for(int i = 0 ; i < n ; ++i) {
		res[i] = R::trigamma(x[i]);
	}

	return(res);
}

inline double poisson_linkinv(double x){
    return x < -36 ? DOUBLE_EPS : exp(x);
}

// [[Rcpp::export]]
NumericVector cpppar_poisson_linkinv(NumericVector x, int nthreads){

    int n = x.length();
    NumericVector res(n);

    #pragma omp parallel for num_threads(nthreads)
    for(int i = 0 ; i < n ; ++i) {
        res[i] = poisson_linkinv(x[i]);
    }

    return(res);
}


// [[Rcpp::export]]
bool cpppar_poisson_validmu(SEXP x, int nthreads){

    int n = Rf_length(x);
    double *px = REAL(x);
    bool res = true;

    #pragma omp parallel for num_threads(nthreads)
    for(int i=0 ; i<n ; ++i){
        double x_tmp = px[i];
        if(std::isinf(x_tmp) || x_tmp <= 0){
            res = false;
        }
    }

    return res;
}


// [[Rcpp::export]]
NumericVector cpppar_logit_linkfun(NumericVector x, int nthreads){
	// parallel trigamma using omp

	int n = x.length();
	NumericVector res(n);

	#pragma omp parallel for num_threads(nthreads)
	for(int i = 0 ; i < n ; ++i) {
	    double x_tmp = x[i];
		res[i] = log(x_tmp) - log(1 - x_tmp);
	}

	return(res);
}

inline double logit_linkinv(double x){
    return x < -30 ? DOUBLE_EPS : (x > 30) ? 1-DOUBLE_EPS : 1 / (1 + 1 / exp(x));
}

// [[Rcpp::export]]
NumericVector cpppar_logit_linkinv(NumericVector x, int nthreads){
	// parallel trigamma using omp

	int n = x.length();
	NumericVector res(n);

#pragma omp parallel for num_threads(nthreads)
	for(int i = 0 ; i < n ; ++i) {
		// res[i] = 1 / (1 + 1 / exp(x[i]));
		res[i] = logit_linkinv(x[i]);
	}

	return(res);
}

inline double logit_mueta(double x){
    if(fabs(x) > 30){
        return DOUBLE_EPS;
    } else {
        double exp_x = exp(x);
        return (1 / ((1 + 1 / exp_x) * (1 + exp_x)));
    }
}

// [[Rcpp::export]]
NumericVector cpppar_logit_mueta(NumericVector x, int nthreads){
	// parallel trigamma using omp

	int n = x.length();
	NumericVector res(n);

#pragma omp parallel for num_threads(nthreads)
	for(int i = 0 ; i < n ; ++i) {
	    // double exp_x = exp(x[i]);
		// res[i] = 1 / ((1 + 1 / exp_x) * (1 + exp_x));
		res[i] = logit_mueta(x[i]);
	}

	return(res);
}

// [[Rcpp::export]]
NumericVector cpppar_logit_devresids(NumericVector y, NumericVector mu, NumericVector wt, int nthreads){

	int n = mu.length();
	NumericVector res(n);
	bool isWeight = wt.length() != 1;

	if(isWeight){
		#pragma omp parallel for num_threads(nthreads)
		for(int i = 0 ; i < n ; ++i) {
			if(y[i] == 1){
				res[i] = - 2 * log(mu[i]) * wt[i];
			} else if(y[i] == 0){
				res[i] = - 2 * log(1 - mu[i]) * wt[i];
			} else {
			    double y_tmp = y[i];
			    double mu_tmp = mu[i];
				res[i] = 2 * wt[i] * (y_tmp*log(y_tmp/mu_tmp) + (1 - y_tmp)*log((1 - y_tmp)/(1 - mu_tmp)));
			}
		}
	} else {
		#pragma omp parallel for num_threads(nthreads)
		for(int i = 0 ; i < n ; ++i) {
			if(y[i] == 1){
				res[i] = - 2 * log(mu[i]);
			} else if(y[i] == 0){
				res[i] = - 2 * log(1 - mu[i]);
			} else {
			    double y_tmp = y[i];
			    double mu_tmp = mu[i];
				// res[i] = 2 * (y[i]*log(y[i]/mu[i]) + (1 - y[i])*log((1 - y[i])/(1 - mu[i])));
				res[i] = 2 * (y_tmp*log(y_tmp/mu_tmp) + (1 - y_tmp)*log((1 - y_tmp)/(1 - mu_tmp)));
			}
		}
	}


	return(res);
}


// [[Rcpp::export]]
NumericMatrix cpppar_crossprod(NumericMatrix X, NumericVector w, int nthreads){

	int N = X.nrow();
	int K = X.ncol();

	bool isWeight = false;
	if(w.length() > 1){
		isWeight = true;
	}

	NumericMatrix res(K, K);

	int nValues = K * K;
	NumericVector values(nValues);

	// computation
#pragma omp parallel for num_threads(nthreads)
	for(int index=0 ; index<nValues ; index++){
		int k_row = index % K;
		int k_col = index / K;

		if(k_row <= k_col){
			double val = 0;

			if(isWeight){
				for(int i=0 ; i<N ; ++i){
					val += X(i, k_row) * w[i] * X(i, k_col);
				}
			} else {
				for(int i=0 ; i<N ; ++i){
					val += X(i, k_row) * X(i, k_col);
				}
			}

			values(index) = val;
		}

	}

	// save
	for(int index=0 ; index<nValues ; index++){
		int k_row = index % K;
		int k_col = index / K;

		if(k_row <= k_col){
			res(k_row, k_col) = values(index);
			if(k_row != k_col){
				res(k_col, k_row) = values(index);
			}
		}

	}

	return(res);
}



// [[Rcpp::export]]
NumericVector cpppar_xwy(NumericMatrix X, NumericVector y, NumericVector w, int nthreads){

	int N = X.nrow();
	int K = X.ncol();

	bool isWeight = false;
	if(w.length() > 1){
		isWeight = true;
	}

	NumericVector res(K);

	// computation
#pragma omp parallel for num_threads(nthreads)
	for(int k=0 ; k<K ; ++k){

		double val = 0;
		if(isWeight){
			for(int i=0 ; i<N ; ++i){
				val += X(i, k) * w[i] * y[i];
			}
		} else {
			for(int i=0 ; i<N ; ++i){
				val += X(i, k) * y[i];
			}
		}
		res[k] = val;
	}


	return(res);
}



// [[Rcpp::export]]
NumericVector cpppar_xbeta(NumericMatrix X, NumericVector beta, int nthreads){

	int N = X.nrow();
	int K = X.ncol();

	NumericVector res(N);

	// computation
#pragma omp parallel for num_threads(nthreads)
	for(int i=0 ; i<N ; ++i){

		double val = 0;
		for(int k=0 ; k<K ; ++k){
			val += X(i, k) * beta[k];
		}
		res[i] = val;
	}

	return(res);
}


// [[Rcpp::export]]
NumericMatrix cpppar_matprod(NumericMatrix x, NumericMatrix y, int nthreads){
	// => simply x %*% y

	int N = x.nrow();
	int K = x.ncol();

	NumericMatrix xy(N, K);

	// computing xy
#pragma omp parallel for num_threads(nthreads)
	for(int i=0 ; i<N ; ++i){
		for(int k=0 ; k<K ; ++k){
			double value = 0;
			for(int l=0 ; l<K ; ++l){
				value += x(i, l) * y(l, k);
			}
			xy(i, k) = value;
		}
	}

	return(xy);
}


// [[Rcpp::export]]
List cpppar_which_na_inf_vec(SEXP x, int nthreads){
    /*
        This function takes a vector and looks at whether it contains NA or infinite values
        return: flag for na/inf + logical vector of obs that are na/inf
        x is ALWAYS a numeric vector
        std::isnan, std::isinf are OK since cpp11 required
        do_any_na_inf: if high suspicion of NA present: we go directly constructing the vector is_na_inf
        in the "best" case (default expected), we need not construct is_na_inf
    */

    int nobs = Rf_length(x);
    double *px = REAL(x);
    bool anyNAInf = false;
    bool any_na = false;    // return value
    bool any_inf = false;   // return value

    /*
        we make parallel the anyNAInf loop
        why? because we want that when there's no NA (default) it works as fast as possible
        if there are NAs, single threaded mode is faster, but then we circumvent with the do_any_na_inf flag
    */

    // no need to care about the race condition
    // "trick" to make a break in a multi-threaded section
    #pragma omp parallel num_threads(nthreads)
    {
        int i = omp_get_thread_num()*nobs/omp_get_num_threads();
        int stop = (omp_get_thread_num()+1)*nobs/omp_get_num_threads();
        double x_tmp = 0;
        for(; i<stop && !anyNAInf ; ++i){
            x_tmp = px[i];
            if(std::isnan(x_tmp) || std::isinf(x_tmp)){
                anyNAInf = true;
            }
        }
    }

    // object to return: is_na_inf
    LogicalVector is_na_inf(anyNAInf ? nobs : 1);

    if(anyNAInf){
        // again: no need to care about race conditions
        #pragma omp parallel for num_threads(nthreads)
        for(int i=0 ; i<nobs ; ++i){
            double x_tmp = px[i];
            if(std::isnan(x_tmp)){
                is_na_inf[i] = true;
                any_na = true;
            } else if(std::isinf(x_tmp)){
                is_na_inf[i] = true;
                any_inf = true;
            }
        }
    }

    // Return
    List res;
    res["any_na"] = any_na;
    res["any_inf"] = any_inf;
    res["any_na_inf"] = any_na || any_inf;
    res["is_na_inf"] = is_na_inf;

    return res;
}

// [[Rcpp::export]]
List cpppar_which_na_inf_mat(NumericMatrix mat, int nthreads){
    // almost identical to cpppar_which_na_inf_vec but for R matrices. Changes:
    // - main argument becomes NumericMatrix
    // - k-for loop within the i-for loop
    /*
       This function takes a matrix and looks at whether it contains NA or infinite values
       return: flag for na/inf + logical vector of obs that are Na/inf
       std::isnan, std::isinf are OK since cpp11 required
       do_any_na_inf: if high suspicion of NA present: we go directly constructing the vector is_na_inf
       in the "best" case (default expected), we need not construct is_na_inf
    */

    int nobs = mat.nrow();
    int K = mat.ncol();
    bool anyNAInf = false;
    bool any_na = false;    // return value
    bool any_inf = false;   // return value

    /*
        we make parallel the anyNAInf loop
        why? because we want that when there's no NA (default) it works as fast as possible
        if there are NAs, single threaded mode is faster, but then we circumvent with the do_any_na_inf flag
    */

    // no need to care about the race condition
    // "trick" to make a break in a multi-threaded section
    #pragma omp parallel num_threads(nthreads)
    {
        int i = omp_get_thread_num()*nobs/omp_get_num_threads();
        int stop = (omp_get_thread_num()+1)*nobs/omp_get_num_threads();
        double x_tmp = 0;
        for(; i<stop && !anyNAInf ; ++i){
            for(int k=0 ; k<K ; ++k){
                x_tmp = mat(i, k);
                if(std::isnan(x_tmp) || std::isinf(x_tmp)){
                    anyNAInf = true;
                }
            }
        }
    }

    // object to return: is_na_inf
    LogicalVector is_na_inf(anyNAInf ? nobs : 1);

    if(anyNAInf){
        #pragma omp parallel for num_threads(nthreads)
        for(int i=0 ; i<nobs ; ++i){
            double x_tmp = 0;
            for(int k=0 ; k<K ; ++k){
                x_tmp = mat(i, k);
                if(std::isnan(x_tmp)){
                    is_na_inf[i] = true;
                    any_na = true;
                    break;
                } else if(std::isinf(x_tmp)){
                    is_na_inf[i] = true;
                    any_inf = true;
                    break;
                }
            }
        }
    }

    // Return
    List res;
    res["any_na"] = any_na;
    res["any_inf"] = any_inf;
    res["any_na_inf"] = any_na || any_inf;
    res["is_na_inf"] = is_na_inf;

    return res;
}



// [[Rcpp::export]]
List cpppar_cond_means(NumericMatrix mat_vars, IntegerVector treat, int nthreads = 1){
    // conditional means: function did_means

    int N = mat_vars.nrow();
    int K = mat_vars.ncol();

    // objects to return:
    IntegerVector na_vect(K);
    NumericMatrix mean_mat(K, 2);
    NumericMatrix sd_mat(K, 2);
    IntegerMatrix n_mat(K, 2);
    IntegerVector n_01(2);


    // computation
#pragma omp parallel for num_threads(nthreads)
    for(int k=0 ; k<K ; ++k){

        double sum_0=0, sum_1=0;
        double sum2_0=0, sum2_1=0;
        int n_0=0, n_1=0, n_na=0;
        double x_tmp=0;

        for(int i=0 ; i<N ; ++i){

            x_tmp = mat_vars(i, k);

            if(std::isnan(x_tmp) || std::isinf(x_tmp)){
                ++n_na;
            } else {
                if(treat[i] == 0){
                    sum_0 += x_tmp;
                    sum2_0 += x_tmp*x_tmp;
                    ++n_0;
                } else {
                    sum_1 += x_tmp;
                    sum2_1 += x_tmp*x_tmp;
                    ++n_1;
                }
            }
        }

        // saving
        double m_0 = sum_0/n_0;
        double m_1 = sum_1/n_1;
        mean_mat(k, 0) = m_0;
        mean_mat(k, 1) = m_1;

        sd_mat(k, 0) = sqrt(sum2_0/(n_0 - 1) - m_0*sum_0/(n_0 - 1));
        sd_mat(k, 1) = sqrt(sum2_1/(n_1 - 1) - m_1*sum_1/(n_1 - 1));

        n_mat(k, 0) = n_0;
        n_mat(k, 1) = n_1;

        na_vect[k] = n_na;
    }

    // number of obs per treat case
    for(int i=0 ; i<N ; ++i){
        if(treat[i] == 0){
            ++n_01[0];
        } else {
            ++n_01[1];
        }
    }

    List res;
    res["means"] = mean_mat;
    res["sd"] = sd_mat;
    res["n"] = n_mat;
    res["n_01"] = n_01;
    res["na"] = na_vect;

    return res;
}



































