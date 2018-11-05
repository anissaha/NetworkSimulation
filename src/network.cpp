#include "network.h"
#include "random.h"

void Network::resize(const size_t& size) {
	values.clear();
	std::vector<double> new_ (size,0.0);
	RNG.normal(new_);
	values = new_;
}

bool Network::add_link(const size_t& a, const size_t& b) {
	if(a != b and a>=0 and b>=0 and a<size() and b<size()) {
		std::multimap<size_t,size_t>::iterator it;
		for(it=links.begin(); it!=links.end(); ++it) {
			if(it->first == a and it->second == b) return false;
			if(it->first == b and it->second == a) return false;
		}
		links.insert(std::pair<size_t,size_t>(a,b));
		links.insert(std::pair<size_t,size_t>(b,a));
		return true;
	}
	return false;
}

size_t Network::random_connect(const double& mean_deg) {
	links.clear();
	size_t count(0);
	for(size_t i(0); i<size(); ++i) {
		int nbr(RNG.poisson(mean_deg));
		std::vector<int> nodes (nbr,0);
		RNG.uniform_int(nodes, 0, size()-1);
		for(int j(0); j<nbr; ++j) {
			if (add_link(i,nodes[j])) {
				++count;
			}
		}
	}
	return count;
}

size_t Network::set_values(const std::vector<double>& nodes_val) {
	size_t n (nodes_val.size());
	size_t s (size());
	if (n < size()) {s = n;}
	for (size_t i(0); i<s; ++i) {
		values[i] = nodes_val[i];
	}
	return s;
}

size_t Network::size() const {
	return values.size();
}

size_t Network::degree(const size_t &_n) const {
	//return neighbors(_n).size();
	return links.count(_n);
}

double Network::value(const size_t &_n) const {
	return values[_n];
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> sorted_values(values);
	for(size_t i(0); i<size(); ++i) {
		for(size_t j(i+1); j<size(); ++j) {
			if(sorted_values[i] < sorted_values[j]) {
				double a (sorted_values[i]);
				sorted_values[i] = sorted_values[j];
				sorted_values[j] = a;
			}
		}
	}
	return sorted_values;
}

std::vector<size_t> Network::neighbors(const size_t& n) const {
	std::vector<size_t> neighbors;
	std::multimap<size_t,size_t>::const_iterator it;
	for(it=links.begin(); it!=links.end(); ++it) {
		if(it->first == n) {
			neighbors.push_back(it->second);
		}
	}
	return neighbors;
}

