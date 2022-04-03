#ifndef GAUSSTRAPMETA_H_
#define GAUSSTRAPMETA_H_

#include "BaseForce.h"
#include <vector>

template<typename number>
class GaussTrapMeta : public BaseForce<number> {
private:
	std::vector<int> _p1a;
	std::vector<int> _p2a;
	std::vector<int> _p1b;
	std::vector<int> _p2b;

public:
    int _ref_id;
	std::vector<BaseParticle<number> *>  _p1a_ptr;
	std::vector<BaseParticle<number> *>  _p2a_ptr;
	std::vector<BaseParticle<number> *>  _p1b_ptr;
	std::vector<BaseParticle<number> *>  _p2b_ptr;
	number xmin;
	number xmax;
    int N_grid; 
    number dX;
    std::vector<std::vector<double> >  potential_grid;
    int _mode;
	bool PBC;
	BaseBox<number> * _box_ptr;
	GaussTrapMeta ();
	virtual ~GaussTrapMeta() {}

	void get_settings (input_file &);
	void init (BaseParticle<number> **, int, BaseBox<number> *);

	virtual LR_vector<number> value(llint step, LR_vector<number> &pos);
	virtual number potential(llint step, LR_vector<number> &pos);

    int cutoff_int;

protected:
	LR_vector<number> _distance(LR_vector<number> u, LR_vector<number> v);
};

#endif // GAUSSTRAPMETA_H