#include <queue>
//typedef int feature_index;
//#define feature_index_null (-1)
typedef double result_t;
struct feature_index{
	int i;
	double v;
	feature_index(int ii = -1,double vv = 0.0):i(ii),v(vv){};
};
const feature_index feature_index_null = feature_index(-1);

struct q_item;

class sample_t{
public:
	virtual int operator[](feature_index) = 0;
};

class sample_container{
public:
	virtual int is_leaf() = 0;
	virtual void split_store(std::queue<q_item>,feature_index) = 0;
	virtual result_t get_result() = 0;
};

struct mask_container{};