#include "contianer.h"

typedef feature_index (*feature_chose_function)(sample_container&,mask_container&);

typedef int node_index;
#define node_index_null (-1)

union node_data{
	node_index n[2];
	result_t v;
};

struct tree_node{
	feature_index f;
	node_data d;
	//del
	node_index operator()(sample_t& s)
	{
		return d.n[s[f]];
	}
};

struct decision_tree{
	std::vector<tree_node> tree;
	void add_node(node_index p,int type,feature_index index){
		tree[p].d.n[type] = tree.size();
		tree_node t;
		t.f = index;
		t.d.n[0] = t.d.n[1] = node_index_null;
		tree.push_back(t);
	}
	void add_leave(node_index p,int type,result_t res){
		tree[p].d.n[type] = tree.size();
		tree_node t;
		t.f = feature_index_null;
		t.d.v = res;
		tree.push_back(t);
	}
};

struct q_item{
	node_index p;
	int type;
	sample_container& s;
	q_item(node_index pp,int tt,sample_container&ss):p(pp),type(tt),s(ss){}
	q_item operator=(const q_item& q){p = q.p;type = q.type;s = q.s;return *this;}
};

decision_tree generate_tree(sample_container& sample,mask_container mask,feature_chose_function func){
	decision_tree tree;
	feature_index i;
	std::queue<q_item> qs;
	q_item tn(node_index_null,0,sample);
	qs.push(tn);
	while(!qs.empty()){
		tn = qs.front();
		sample = tn.s;
		if(!sample.is_leaf()){
			i = func(sample,mask);
			sample.split_store(qs,i);
			tree.add_node(tn.p,tn.type,i);
		}
		else
			tree.add_leave(tn.p,tn.type,sample.get_result());
		qs.pop();
	}
	return tree;
}