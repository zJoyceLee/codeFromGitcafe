#pragma once
static_assert(__cplusplus >= 201103L, "Compile with -std=c++11");

#include <vector>
#include <list>
#include <string>
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <exception>

#include "../globals.hpp"
#include "../BFSVisitor.hpp"
#include "../DFSVisitor.hpp"

template <
	typename EdgeInfo = DefaultEdgeInfo,
	typename VertexInfo = DefaultVertexInfo,
	typename GraphInfo = DefaultGraphInfo>
class adjacency_list_directed
{
public://types
	typedef std::size_t							size_type;

	typedef std::vector<std::list<
		std::pair<size_type, EdgeInfo> > >		content_type;

private://members
	content_type M_list;
	std::vector<VertexInfo> M_vertex;
	GraphInfo M_info;

	size_type M_vertex_size;
	size_type M_edge_size;

public://functions
	adjacency_list_directed(const GraphInfo & L_g = GraphInfo()) :
		M_list(),
		M_vertex(),
		M_info(L_g),
		M_vertex_size(0),
		M_edge_size(0)
	{}//default constructor

	explicit
	adjacency_list_directed(const adjacency_list_directed & L_rhs) :
		M_list(L_rhs.M_list),
		M_vertex_size(L_rhs.M_vertex),
		M_info(L_rhs.M_info),
		M_vertex_size(L_rhs.M_vertex_size),
		M_edge_size(L_rhs.M_edge_size)
	{}//copy constructor

	explicit
	adjacency_list_directed(
			const size_type L_vertex_size,
			const GraphInfo & L_g = GraphInfo()) :
		M_list(L_vertex_size),
		M_vertex(L_vertex_size),
		M_info(L_g),
		M_vertex_size(L_vertex_size),
		M_edge_size(0)
	{}//constructor(vertex_size, GraphInfo)

	explicit
	adjacency_list_directed(
            const std::initializer_list<VertexInfo> L_vert_init,
            const GraphInfo & L_g = GraphInfo()) :
        M_list(L_vert_init.size()),
        M_vertex(L_vert_init),
        M_info(L_g),
        M_vertex_size(L_vert_init.size()),
        M_edge_size(0)
    {}//constructor(vert_init, GraphInfo)

	adjacency_list_directed &
	operator=(const adjacency_list_directed & L_rhs) {
		if(&L_rhs == this)
			return (*this);

		this->M_list = L_rhs.M_list;
		this->M_vertex = L_rhs.M_vertex;
		this->M_info = L_rhs.M_info;
		this->M_vertex_size = L_rhs.M_vertex_size;
		this->M_edge_size = L_rhs.M_edge_size;

		return (*this);
	}//assign operator

	size_type
	vertex_num() const
	{ return this->M_vertex_size; }

	size_type
	edge_num() const
	{ return this->M_edge_size; }

	content_type
	data() const
	{ return this->M_list; }

	GraphInfo &
	graph_info()
	{ return this->M_info; }

	const GraphInfo &
	graph_info() const
	{ return this->M_info; }

	bool
	add_edge(
			const size_type L_source,
			const size_type L_target,
			const EdgeInfo & L_info = EdgeInfo());

	bool
	remove_edge(
			const size_type L_source,
			const size_type L_target);

	size_type
	add_vertex(const VertexInfo & L_info = VertexInfo());

	bool
	remove_vertex(const size_type L_vertex_id);

	std::pair<bool, VertexInfo>
	get_vertex(const size_type L_vertex_id) const {
		if(L_vertex_id < this->M_vertex_size)
			return std::make_pair(true, this->M_vertex[L_vertex_id]);
		return std::make_pair(false, VertexInfo());
	}//get_vertex(vertex_id)

	std::pair<bool, EdgeInfo>
	get_edge(const size_type L_source, const size_type L_target) const;

	bool
	put_vertex(const size_type L_vertex_id, const VertexInfo & L_info) {
		if(L_vertex_id >= this->M_vertex_size)
			return false;

		this->M_vertex[L_vertex_id] = L_info;
		return true;
	}//put_vertex(vertex_id, info)

	bool
	put_edge(
			const size_type L_source,
			const size_type L_target,
			const EdgeInfo & L_info);

	inline bool
	breadth_first_traverse(BFSVisitor<adjacency_list_directed> & L_vis)
	{
		bool * L_visited = new bool[this->M_vertex_size];
		std::fill_n(L_visited, this->M_vertex_size, false);

		for(size_type L_i = 0; L_i < this->M_vertex_size; ++L_i) {
			if(!L_vis.initialize_vertex(L_i, *this)) {
				delete[] L_visited;
				return false;
			}//if
		}//for

		for(size_type L_i = 0; L_i < this->M_vertex_size; ++L_i) {
			if(L_visited[L_i] == false) {
				if(!breadth_first_search_aux(L_i, L_visited, L_vis)) {
					delete[] L_visited;
					return false;
				}//if
			}//if
		}//for

		delete[] L_visited;
		return true;
	}//breadth_first_traverse(vis)

	inline bool
	breadth_first_search(
			const size_type L_start_vertex,
			BFSVisitor<adjacency_list_directed> & L_vis)
	{
		bool * L_visited = new bool[this->M_vertex_size];
		std::fill_n(L_visited, this->M_vertex_size, false);

		if(!breadth_first_search_aux(L_start_vertex, L_visited, L_vis)) {
			delete[] L_visited;
			return false;
		}//if

		delete[] L_visited;
		return true;
	}//breadth_first_search(start_vertex, vis)

	inline bool
	depth_first_traverse(DFSVisitor<adjacency_list_directed> & L_vis)
	{
		size_type * L_visited = new size_type[this->M_vertex_size];
		std::fill_n(L_visited, this->M_vertex_size, 0);

		for(size_type L_i = 0; L_i < this->M_vertex_size; ++L_i) {
			if(!L_vis.initialize_vertex(L_i, *this)) {
				delete[] L_visited;
				return false;
			}//if
		}//for

		for(size_type L_i = 0; L_i < this->M_vertex_size; ++L_i) {
			if(L_visited[L_i] == 0) {
				if(!depth_first_search_aux(L_i, L_visited, L_vis)) {
					delete[] L_visited;
					return false;
				}//if
			}//if
		}//for

		delete[] L_visited;
		return true;
	}//depth_first_traverse(vis)

	inline bool
	depth_first_search(
			const size_type L_start_vertex,
			DFSVisitor<adjacency_list_directed> & L_vis)
	{
		size_type * L_visited = new size_type[this->M_vertex_size];
		std::fill_n(L_visited, this->M_vertex_size, 0);

		if(!depth_first_search_aux(L_start_vertex, L_visited, L_vis)) {
			delete[] L_visited;
			return false;
		}//if
		return true;
	}//depth_first_search(start_vertex, vis)

private://functions
	bool
	breadth_first_search_aux(
			const size_type L_start_vertex,
			bool L_visited[],
			BFSVisitor<adjacency_list_directed> & L_vis);

	bool
	depth_first_search_aux(
			const size_type L_start_vertex,
			size_type L_visited[],
			DFSVisitor<adjacency_list_directed> & L_vis);
};//class adjacency_matrix_undirected
