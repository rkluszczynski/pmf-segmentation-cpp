#ifndef EDGEELEMENT_HPP_INCLUDED
#define EDGEELEMENT_HPP_INCLUDED


template <typename T_REAL> struct edge_element
{
	T_REAL x, y;
	T_REAL angle;
	pmf_point<T_REAL> * pt;


	edge_element ( T_REAL xx,  T_REAL yy,  T_REAL aa )
		: x(xx), y (yy), angle(aa), pt(NULL)
	{}


	friend std::ostream& operator << (std::ostream& out, const edge_element<T_REAL> * ee)
	{
		out << " ( ";
		out.width(5);
		out.precision(3);  out << ee->x;
		out << " ; ";
		out.width(5);
		out.precision(3);  out << ee->y;
		out << " ) ~ [ ";
		out.width(5);
		out.precision(3);  out << ee->angle;
		out << " / ";
		#define RAD2DEG(X) ((X) * 180.0 / M_PI)
		out << RAD2DEG(ee->angle);
		#undef RAD2DEG
		out << " ] ";
		if (ee->pt)  out << " :: " << ee->pt;
		return out;
	}
};


#endif // EDGEELEMENT_HPP_INCLUDED
