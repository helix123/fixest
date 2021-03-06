% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/ESTIMATION_FUNS.R
\name{feNmlm}
\alias{feNmlm}
\title{Fixed effects nonlinear maximum likelihood models}
\usage{
feNmlm(fml, data, family = c("poisson", "negbin", "logit", "gaussian"),
  NL.fml, fixef, na_inf.rm = getFixest_na_inf.rm(), NL.start, lower,
  upper, NL.start.init, offset, start = 0, jacobian.method = "simple",
  useHessian = TRUE, hessian.args = NULL, opt.control = list(),
  nthreads = getFixest_nthreads(), verbose = 0, theta.init,
  fixef.tol = 1e-05, fixef.iter = 1000, deriv.tol = 1e-04,
  deriv.iter = 1000, warn = TRUE, notes = getFixest_notes(),
  combine.quick, ...)
}
\arguments{
\item{fml}{A formula. This formula gives the linear formula to be estimated (it is similar to a \code{lm} formula), for example: \code{fml = z~x+y}. To include cluster variables, you can 1) either insert them in this formula using a pipe (e.g. \code{fml = z~x+y|cluster1+cluster2}), or 2) either use the argument \code{cluster}. To include a non-linear in parameters element, you must use the argment \code{NL.fml}.}

\item{data}{A data.frame containing the necessary variables to run the model. The variables of the non-linear right hand side of the formula are identified with this \code{data.frame} names. Can also be a matrix.}

\item{family}{Character scalar. It should provide the family. The possible values are "poisson" (Poisson model with log-link, the default), "negbin" (Negative Binomial model with log-link), "logit" (LOGIT model with log-link), "gaussian" (Gaussian model).}

\item{NL.fml}{A formula. If provided, this formula represents the non-linear part of the right hand side (RHS). Note that contrary to the \code{fml} argument, the coefficients must explicitly appear in this formula. For instance, it can be \code{~a*log(b*x + c*x^3)}, where \code{a}, \code{b}, and \code{c} are the coefficients to be estimated. Note that only the RHS of the formula is to be provided, and NOT the left hand side.}

\item{fixef}{Character vector. The name/s of a/some variable/s within the dataset to be used as fixed-effects. These variables should contain the identifier of each observation (e.g., think of it as a panel identifier).}

\item{na_inf.rm}{Logical, default is \code{TRUE}. If the variables necessary for the estimation contain NA/Infs and \code{na_inf.rm = TRUE}, then all observations containing NA are removed prior to estimation and a note is displayed detailing the number of observations removed. Otherwise, an error is raised.}

\item{NL.start}{(For NL models only) A list of starting values for the non-linear parameters. ALL the parameters are to be named and given a staring value. Example: \code{NL.start=list(a=1,b=5,c=0)}. Though, there is an exception: if all parameters are to be given the same starting value, you can use the argument \code{NL.start.init}.}

\item{lower}{(For NL models only) A list. The lower bound for each of the non-linear parameters that requires one. Example: \code{lower=list(b=0,c=0)}. Beware, if the estimated parameter is at his lower bound, then asymptotic theory cannot be applied and the standard-error of the parameter cannot be estimated because the gradient will not be null. In other words, when at its upper/lower bound, the parameter is considered as 'fixed'.}

\item{upper}{(For NL models only) A list. The upper bound for each of the non-linear parameters that requires one. Example: \code{upper=list(a=10,c=50)}. Beware, if the estimated parameter is at his upper bound, then asymptotic theory cannot be applied and the standard-error of the parameter cannot be estimated because the gradient will not be null. In other words, when at its upper/lower bound, the parameter is considered as 'fixed'.}

\item{NL.start.init}{(For NL models only) Numeric scalar. If the argument \code{NL.start} is not provided, or only partially filled (i.e. there remain non-linear parameters with no starting value), then the starting value of all remaining non-linear parameters is set to \code{NL.start.init}.}

\item{offset}{A formula or a numeric vector. An offset can be added to the estimation. If equal to a formula, it should be of the form (for example) \code{~0.5*x**2}. This offset is linearly added to the elements of the main formula 'fml'.}

\item{start}{Starting values for the coefficients in the linear part (for the non-linear part, use NL.start). Can be: i) a numeric of length 1 (e.g. \code{start = 0}, the default), ii) a numeric vector of the exact same length as the number of variables, or iii) a named vector of any length (the names will be used to initialize the appropriate coefficients).}

\item{jacobian.method}{(For NL models only) Character scalar. Provides the method used to numerically compute the Jacobian of the non-linear part. Can be either \code{"simple"} or \code{"Richardson"}. Default is \code{"simple"}. See the help of \code{\link[numDeriv]{jacobian}} for more information.}

\item{useHessian}{Logical. Should the Hessian be computed in the optimization stage? Default is \code{TRUE}.}

\item{hessian.args}{List of arguments to be passed to function \code{\link[numDeriv]{genD}}. Defaults is missing. Only used with the presence of \code{NL.fml}.}

\item{opt.control}{List of elements to be passed to the optimization method \code{\link[stats]{nlminb}}. See the help page of \code{\link[stats]{nlminb}} for more information.}

\item{nthreads}{Integer: Number of nthreads to be used (accelerates the algorithm via the use of openMP routines). The default is to use the total number of nthreads available minus two. You can set permanently the number of nthreads used within this package using the function \code{\link[fixest]{setFixest_nthreads}}.}

\item{verbose}{Integer, default is 0. It represents the level of information that should be reported during the optimisation process. If \code{verbose=0}: nothing is reported. If \code{verbose=1}: the value of the coefficients and the likelihood are reported. If \code{verbose=2}: \code{1} + information on the computing time of the null model, the cluster coefficients and the hessian are reported.}

\item{theta.init}{Positive numeric scalar. The starting value of the dispersion parameter if \code{family="negbin"}. By default, the algorithm uses as a starting value the theta obtained from the model with only the intercept.}

\item{fixef.tol}{Precision used to obtain the fixed-effects (ie cluster coefficients). Defaults to \code{1e-5}. It corresponds to the maximum absolute difference allowed between two coefficients of successive iterations. Argument \code{fixef.tol} cannot be lower than \code{10000*.Machine$double.eps}. Note that this parameter is dynamically controlled by the algorithm.}

\item{fixef.iter}{Maximum number of iterations in the step obtaining the fixed-effects (only in use for 2+ clusters). Default is 10000.}

\item{deriv.tol}{Precision used to obtain the fixed-effects derivatives. Defaults to \code{1e-4}. It corresponds to the maximum absolute difference allowed between two coefficients of successive iterations. Argument \code{deriv.tol} cannot be lower than \code{10000*.Machine$double.eps}.}

\item{deriv.iter}{Maximum number of iterations in the step obtaining the derivative of the fixed-effects (only in use for 2+ clusters). Default is 1000.}

\item{warn}{Logical, default is \code{TRUE}. Whether warnings should be displayed (concerns warnings relating to: convergence state, collinearity issues and observation removal due to only 0/1 outcomes or presence of NA values).}

\item{notes}{Logical. By default, two notes are displayed: when NAs are removed (to show additional information) and when some observations are removed because of only 0 (or 0/1) outcomes in a fixed-effect (in Poisson/Neg. Bin./Logit models). To avoid displaying these messages, you can set \code{notes = FALSE}. You can remove these messages permanently by using \code{setFixest_notes(FALSE)}.}

\item{combine.quick}{Logical. When you combine different variables to transform them into a single fixed-effects you can do e.g. \code{y ~ x | paste(var1, var2)}. The algorithm provides a shorthand to do the same operation: \code{y ~ x | var1^var2}. Because pasting variables is a costly operation, the internal algorithm may use a numerical trick to hasten the process. The cost of doing so is that you lose the labels. If you are interested in getting the value of the fixed-effects coefficients after the estimation, you should use \code{combine.quick = FALSE}. By default it is equal to \code{FALSE} if the number of observations is lower than 50,000, and to \code{TRUE} otherwise.}

\item{...}{Not currently used.}
}
\value{
An \code{femlm} object.
\item{coefficients}{The named vector of coefficients.}
\item{coeftable}{The table of the coefficients with their standard errors, z-values and p-values.}
\item{loglik}{The loglikelihood.}
\item{iterations}{Number of iterations of the algorithm.}
\item{n}{The number of observations.}
\item{nparams}{The number of parameters of the model.}
\item{call}{The call.}
\item{fml}{The linear formula of the call.}
\item{ll_null}{Log-likelihood of the null model (i.e. with the intercept only).}
\item{pseudo_r2}{The adjusted pseudo R2.}
\item{message}{The convergence message from the optimization procedures.}
\item{sq.cor}{Squared correlation between the dependent variable and the expected predictor (i.e. fitted.values) obtained by the estimation.}
\item{hessian}{The Hessian of the parameters.}
\item{fitted.values}{The fitted values are the expected value of the dependent variable for the fitted model: that is \eqn{E(Y|X)}.}
\item{cov.unscaled}{The variance-covariance matrix of the parameters.}
\item{se}{The standard-error of the parameters.}
\item{scores}{The matrix of the scores (first derivative for each observation).}
\item{family}{The ML family that was used for the estimation.}
\item{residuals}{The difference between the dependent variable and the expected predictor.}
\item{sumFE}{The sum of the fixed-effects for each observation.}
\item{offset}{The offset formula.}
\item{NL.fml}{The nonlinear formula of the call.}
\item{bounds}{Whether the coefficients were upper or lower bounded. -- This can only be the case when a non-linear formula is included and the arguments 'lower' or 'upper' are provided.}
\item{isBounded}{The logical vector that gives for each coefficient whether it was bounded or not. This can only be the case when a non-linear formula is included and the arguments 'lower' or 'upper' are provided.}
\item{fixef_vars}{The names of each cluster.}
\item{fixef_id}{The list (of length the number of clusters) of the cluster identifiers for each observation.}
\item{fixef_sizes}{The size of each cluster.}
\item{obsRemoved}{In the case there were clusters and some observations were removed because of only 0/1 outcome within a cluster, it gives the row numbers of the observations that were removed.}
\item{fixef_removed}{In the case there were clusters and some observations were removed because of only 0/1 outcome within a cluster, it gives the list (for each cluster) of the cluster identifiers that were removed.}
\item{theta}{In the case of a negative binomial estimation: the overdispersion parameter.}

 @seealso
See also \code{\link[fixest]{summary.fixest}} to see the results with the appropriate standard-errors, \code{\link[fixest]{fixef.fixest}} to extract the cluster coefficients, and the functions \code{\link[fixest]{esttable}} and \code{\link[fixest]{esttex}} to visualize the results of multiple estimations.

And other estimation methods: \code{\link[fixest]{feols}}, \code{\link[fixest]{femlm}}, \code{\link[fixest]{feglm}}, \code{\link[fixest]{fepois}}, \code{\link[fixest]{fenegbin}}.
}
\description{
This function estimates maximum likelihood models (e.g., Poisson or Logit) with non-linear in parameters right-hand-sides and is efficient to handle any number of fixed effects. If you do not use non-linear in parameters right-hand-side, use \code{\link[fixest]{femlm}} or \code{\link[fixest]{feglm}} instead (design is simpler).
}
\details{
This function estimates maximum likelihood models where the conditional expectations are as follows:

Gaussian likelihood:
\deqn{E(Y|X)=X\beta}{E(Y|X) = X*beta}
Poisson and Negative Binomial likelihoods:
\deqn{E(Y|X)=\exp(X\beta)}{E(Y|X) = exp(X*beta)}
where in the Negative Binomial there is the parameter \eqn{\theta}{theta} used to model the variance as \eqn{\mu+\mu^2/\theta}{mu+mu^2/theta}, with \eqn{\mu}{mu} the conditional expectation.
Logit likelihood:
\deqn{E(Y|X)=\frac{\exp(X\beta)}{1+\exp(X\beta)}}{E(Y|X) = exp(X*beta) / (1 + exp(X*beta))}

When there are one or more clusters, the conditional expectation can be written as:
\deqn{E(Y|X) = h(X\beta+\sum_{k}\sum_{m}\gamma_{m}^{k}\times C_{im}^{k}),}
where \eqn{h(.)} is the function corresponding to the likelihood function as shown before. \eqn{C^k} is the matrix associated to cluster \eqn{k} such that \eqn{C^k_{im}} is equal to 1 if observation \eqn{i} is of category \eqn{m} in cluster \eqn{k} and 0 otherwise.

When there are non linear in parameters functions, we can schematically split the set of regressors in two:
\deqn{f(X,\beta)=X^1\beta^1 + g(X^2,\beta^2)}
with first a linear term and then a non linear part expressed by the function g. That is, we add a non-linear term to the linear terms (which are \eqn{X*beta} and the cluster coefficients). It is always better (more efficient) to put into the argument \code{NL.fml} only the non-linear in parameter terms, and add all linear terms in the \code{fml} argument.

To estimate only a non-linear formula without even the intercept, you must exclude the intercept from the linear formula by using, e.g., \code{fml = z~0}.

The over-dispersion parameter of the Negative Binomial family, theta, is capped at 10,000. If theta reaches this high value, it means that there is no overdispersion.
}
\examples{

# This section covers only non-linear in parameters examples
# For linear relationships: use femlm instead

# Generating data for a simple example
n = 100
x = rnorm(n, 1, 5)**2
y = rnorm(n, -1, 5)**2
z1 = rpois(n, x*y) + rpois(n, 2)
base = data.frame(x, y, z1)

# Estimating a 'linear' relation:
est1_L = femlm(z1 ~ log(x) + log(y), base)
# Estimating the same 'linear' relation using a 'non-linear' call
est1_NL = feNmlm(z1 ~ 1, base, NL.fml = ~a*log(x)+b*log(y), NL.start = list(a=0, b=0))
# we compare the estimates with the function esttable (they are identical)
esttable(est1_L, est1_NL)

# Now generating a non-linear relation (E(z2) = x + y + 1):
z2 = rpois(n, x + y) + rpois(n, 1)
base$z2 = z2

# Estimation using this non-linear form
est2_NL = feNmlm(z2~0, base, NL.fml = ~log(a*x + b*y),
               NL.start = list(a=1, b=2), lower = list(a=0, b=0))
# we can't estimate this relation linearily
# => closest we can do:
est2_L = femlm(z2~log(x)+log(y), base)

# Difference between the two models:
esttable(est2_L, est2_NL)

# Plotting the fits:
plot(x, z2, pch = 18)
points(x, fitted(est2_L), col = 2, pch = 1)
points(x, fitted(est2_NL), col = 4, pch = 2)


}
\references{
Berge, Laurent, 2018, "Efficient estimation of maximum likelihood models with multiple fixed-effects: the R package FENmlm." CREA Discussion Papers, 13 (\url{https://wwwen.uni.lu/content/download/110162/1299525/file/2018_13}).

For models with multiple fixed-effects:

Gaure, Simen, 2013, "OLS with multiple high dimensional category variables", Computational Statistics & Data Analysis 66 pp. 8--18

On the unconditionnal Negative Binomial model:

Allison, Paul D and Waterman, Richard P, 2002, "Fixed-Effects Negative Binomial Regression Models", Sociological Methodology 32(1) pp. 247--265
}
\author{
Laurent Berge
}
