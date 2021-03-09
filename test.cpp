#include "pbbs-include/get_time.h"
#include "pbbs-include/parse_command_line.h"
#include <cilk/cilk.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <random>
#include <cstdio>
#include <set>
#include <map>
#include "pbbs-include/sequence_ops.h"
#include "pbbs-include/random_shuffle.h"

using namespace std;

#define LARGE 1

#ifdef LARGE
using key_type = size_t;
#else
using key_type = unsigned int;
#endif

using par = pair<key_type, key_type>;

struct mapped {
    key_type k, v;
    mapped(key_type _k, key_type _v) : k(_k), v(_v) {};
    mapped(){};

    bool operator < (const mapped& m) 
        const { return k < m.k; }
    
    bool operator > (const mapped& m) 
        const { return k > m.k; }
    
    bool operator == (const mapped& m) 
        const { return k == m.k; }
};

std::mt19937_64& get_rand_gen() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937_64 generator(rd());
    return generator;
}


par* uniform_input(size_t n, size_t window, bool shuffle = false) {
    par *v = new par[n];

    parallel_for (size_t i = 0; i < n; i++) {
        uniform_int_distribution<> r_keys(1, window);
        key_type k = r_keys(get_rand_gen());
        key_type c = i; //r_keys(get_rand_gen());
        v[i] = make_pair(k, c);
    }

    auto addfirst = [] (par a, par b) -> par {
      return par(a.first+b.first, b.second);};
    auto vv = sequence<par>(v,n);
    pbbs::scan(vv,vv,addfirst,par(0,0),pbbs::fl_scan_inclusive);
    if (shuffle) pbbs::random_shuffle(vv);
    return v;
}

par* uniform_input_unsorted(size_t n, size_t window) {
    par *v = new par[n];

    parallel_for (size_t i = 0; i < n; i++) {
        uniform_int_distribution<> r_keys(1, window);

        key_type k = r_keys(get_rand_gen());
        key_type c = r_keys(get_rand_gen());

        v[i] = make_pair(k, c);
    }

    return v;
}

int main(int argc, char **argv) {
	// if (argc != 2) {
	// 	cout << "input: " << endl;
	// 	return 0;
	// }
  int c = atoi(argv[1]); // class
  int n = 10000000, m = n*10;
  par *v1 = uniform_input(n, 20);
  par *v2 = uniform_input(m, 20*(n/m));

  if (c == 0) {
    cout << "set:" << endl;
    
    set<key_type> s,s1,s2,sret;
    timer t,t1,t2;
    t.start();
    for (int i = 0; i < n; i++) s.insert(v1[i].first);
    t.stop();
    cout << "insert:" << t.get_total() << endl;

    t1.start();
    for (int i = 0; i < n; i++) s.erase(v1[i].first);
    t1.stop();
    cout << "delete:" << t1.get_total() << endl;

    for (int i = 0; i < n; i++) s1.insert(v1[i].first);
    for (int i = 0; i < m; i++) s2.insert(v2[i].first);
    
    t2.start();
    set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(sret, sret.begin()));
    t2.stop();
    cout << "union:" << t2.get_total() << endl;
    
  }
  else if (c == 1) {
    cout << "multiset:" << endl;

    multiset<key_type> s,s1,s2,sret;
    timer t,t1,t2;
    t.start();
    for (int i = 0; i < n; i++) s.insert(v1[i].first);
    t.stop();
    cout << "insert:" << t.get_total() << endl;

    t1.start();
    for (int i = 0; i < n; i++) s.erase(v1[i].first);
    t1.stop();
    cout << "delete:" << t1.get_total() << endl;

    for (int i = 0; i < n; i++) s1.insert(v1[i].first);
    for (int i = 0; i < m; i++) s2.insert(v2[i].first);
    
    t2.start();
    set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(sret, sret.begin()));
    t2.stop();
    cout << "union:" << t2.get_total() << endl;


  }
  else if (c == 2){
    cout << "map:" << endl;

    map<key_type, key_type> s,s1,s2;
    timer t,t1,t2;
    t.start();
    for (int i = 0; i < n; i++) s.insert(v1[i]);
    t.stop();
    cout << "insert:" << t.get_total() << endl;

    t1.start();
    for (int i = 0; i < n; i++) s.erase(v1[i]);
    t1.stop();
    cout << "delete:" << t1.get_total() << endl;

    for (int i = 0; i < n; i++) s1.insert(v1[i]);
    for (int i = 0; i < m; i++) s2.insert(v2[i]);
    
    t2.start();
    s1.merge(s2);
    t2.stop();
    cout << "union:" << t2.get_total() << endl;

  }
  else if (c == 3) {
    cout << "multimap:" << endl;

    multimap<key_type, key_type> s,s1,s2;
    timer t,t1,t2;
    t.start();
    for (int i = 0; i < n; i++) s.insert(v1[i]);
    t.stop();
    cout << "insert:" << t.get_total() << endl;

    t1.start();
    for (int i = 0; i < n; i++) s.erase(v1[i]);
    t1.stop();
    cout << "delete:" << t1.get_total() << endl;

    for (int i = 0; i < n; i++) s1.insert(v1[i]);
    for (int i = 0; i < m; i++) s2.insert(v2[i]);
    
    t2.start();
    s1.merge(s2);
    t2.stop();
    cout << "union:" << t2.get_total() << endl;
    

  }
  else if (c == 4) {
    cout << "vector" << endl;

    vector<mapped> s,s1,s2, sret; sret.reserve(m+n);
    timer t,t1,t2;
    t.start();
    for (int i = 0; i < n; i++) s.push_back(mapped(v1[i].first, v1[i].second));
    t.stop();
    cout << "insert:" << t.get_total() << endl;

    t1.start();
    for (int i = 0; i < n; i++) s.erase(mapped(v1[i].first, v1[i].second));
    t1.stop();
    cout << "delete:" << t1.get_total() << endl;

    for (int i = 0; i < n; i++) s1.push_back(mapped(v1[i].first, v1[i].second));
    for (int i = 0; i < m; i++) s2.push_back(mapped(v2[i].first, v2[i].second));
    
    t2.start();
    set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(sret));
    t2.stop();
    cout << "union:" << t2.get_total() << endl;
  }
  delete[] v1; delete[] v2;
  return 0;
}