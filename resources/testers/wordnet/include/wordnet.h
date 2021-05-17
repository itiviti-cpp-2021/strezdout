#pragma once
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using ancestor_t = std::unordered_map<int, int>;
using adjacency_t = std::vector<int>;
using digraph_t = std::vector<adjacency_t>;
using word_synset_map_t = std::unordered_map<std::string, std::vector<int>>;
using int_bijection_t = std::unordered_map<int, int>;

class Digraph
{
public:
    Digraph() = default;

    Digraph(const digraph_t & digraph);

    Digraph(digraph_t && digraph);

    const adjacency_t & get_adjacent(int v) const;

    std::size_t num_verts() const;

    friend std::ostream & operator<<(std::ostream & out, const Digraph & digraph);

private:
    digraph_t digraph;
};

class ShortestCommonAncestor
{
public:
    ShortestCommonAncestor() = default;

    ShortestCommonAncestor(const Digraph & dg);

    ShortestCommonAncestor(Digraph && dg);

    // calculates length of shortest common ancestor path from node with id 'v' to node with id 'w'
    int length(int v, int w) const;

    // returns node id of shortest common ancestor of nodes v and w
    int ancestor(int v, int w) const;

    // calculates length of shortest common ancestor path from node subset 'subset_a' to node subset 'subset_b'
    int length_subset(const std::set<int> & subset_a, const std::set<int> & subset_b) const;

    // returns node id of shortest common ancestor of node subset 'subset_a' and node subset 'subset_b'
    int ancestor_subset(const std::set<int> & subset_a, const std::set<int> & subset_b) const;

    int length_subset(const std::vector<int> & subset_a, const std::vector<int> & subset_b);

    int ancestor_subset(const std::vector<int> & subset_a, const std::vector<int> & subset_b);

private:
    template <class It>
    ancestor_t get_ancestors(It first, It last) const;
    const ancestor_t & vector_ancestors(const std::vector<int> & subset);
    std::pair<int, int> find_sca(const ancestor_t & a, const ancestor_t & b) const;

    std::pair<int, int> find_sca(int v, int w) const;
    std::pair<int, int> find_sca(const std::set<int> & subset_a, const std::set<int> & subset_b) const;
    // we cache vectors so this won't be marked const
    std::pair<int, int> find_sca(const std::vector<int> & subset_a, const std::vector<int> & subset_b);

    struct VectorHasher
    {
        std::size_t operator()(const std::vector<int> & v) const;
    };

    Digraph digraph;
    std::unordered_map<const std::vector<int>, const ancestor_t, VectorHasher> ancestor_cache;
};

class WordNet
{
public:
    WordNet(const std::string & synsets_fn, const std::string & hypernyms_fn);

    // currently this seems like the easiest way
    using iterator = word_synset_map_t::iterator;

    // get iterator to list all nouns stored in WordNet
    iterator begin();
    iterator nouns();
    iterator end();

    // returns 'true' iff 'word' is stored in WordNet
    bool is_noun(const std::string & word) const;

    // returns gloss of "shortest common ancestor" of noun1 and noun2
    std::string sca(const std::string & noun1, const std::string & noun2);

    // calculates distance between noun1 and noun2
    int distance(const std::string & noun1, const std::string & noun2);

private:
    std::unordered_map<int, int> construct_sca(std::ifstream & file);
    void construct_synsets(std::ifstream & file, const int_bijection_t & id_map);
    void parse_synset_line(const std::string & line, const int_bijection_t & id_map);

    ShortestCommonAncestor sca_impl;
    word_synset_map_t word_synset_map;
    std::vector<std::string> definitions;
};

class Outcast
{
public:
    Outcast(WordNet & wordnet);

    // returns outcast word
    std::string outcast(const std::vector<std::string> & nouns);

private:
    int word_distance(const std::vector<std::string> & nouns, std::size_t i);

    WordNet & wordnet;
};
