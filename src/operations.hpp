#ifndef OPERATIONS_HPP_INCLUDED
#define OPERATIONS_HPP_INCLUDED

#define REAL double

ConfigurationList<REAL> *
pmf_generate (
			REAL fieldHeight,
			REAL fieldWidth,
			char * outputFile,
			time_t seed
		)
;

#endif // OPERATIONS_HPP_INCLUDED
