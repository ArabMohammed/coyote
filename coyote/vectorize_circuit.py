from collections import defaultdict
from dataclasses import dataclass, field
from functools import reduce
from heapq import heappop, heappush
from math import exp
from random import randint, random, seed, choice
from typing import Any, Counter, List, Optional, Set, Dict, Tuple, Generator, cast, TypeVar
import networkx as nx # type: ignore

from .codegen import build_vector_program, codegen
from .disjoint_set import DisjointSet
from .coyote_ast import CompilerV2, Instr
from .synthesize_schedule import VecSchedule, synthesize_schedule
import time 
T = TypeVar('T')
seed(1)

MUL_PER_ROTATE = 1
ADD_PER_ROTATE = 0.1

COSTS_PER_ROTATE = defaultdict(int, {'+': ADD_PER_ROTATE, '*': MUL_PER_ROTATE, '-': ADD_PER_ROTATE})

######################################################################
######################################################################

def display_digraph(graph: nx.DiGraph):
    """
    Display a graph (directed or undirected) in the CLI by printing its nodes, edges, and attributes.
    
    Args:
        graph (nx.Graph): The graph to display (can be directed or undirected).
    """
    if isinstance(graph, nx.DiGraph):
        print("=== Directed Graph (DiGraph) ===")
        for u, v, attributes in graph.edges(data=True):
            print(f"  {u} --> {v} (Attributes: {attributes})")
    elif isinstance(graph, nx.Graph):
        print("=== Undirected Graph (Graph) ===")
        for u, v, attributes in graph.edges(data=True):
            print(f"  {u} -- {v} (Attributes: {attributes})")
    else:
        print("Unsupported graph type.")
        return
    # Display graph-level attributes (if any)
    if graph.graph:
        print("\nGraph Attributes:")
        for key, value in graph.graph.items():
            print(f"  {key}: {value}")

##############################################

def display_columns(columns: DisjointSet[T]):
    print("Disjoint Set Contents:")
    # Use the all_classes generator to get all equivalence classes
    for equivalence_class in columns.all_classes():
        print(f"Equivalence Class: {equivalence_class}")

######################################################################
##########################Done############################################

def instr_sequence_to_nx_graph(instrs: List[Instr]) -> nx.DiGraph:
    graph = nx.DiGraph()
    graph.graph['ops'] = {}
    for instr in instrs:
        graph.graph['ops'][instr.dest.val] = instr.op
        # if instr.op == '~':
        #     continue
        if instr.op == '~' and not (instr.lhs.reg and instr.rhs.reg):
            continue
        graph.add_edge((instr.lhs.val,), (instr.dest.val,))
        graph.add_edge((instr.rhs.val,), (instr.dest.val,))
        
    return graph

## Done
def grade_nx_graph(graph: nx.DiGraph, groups: List[Set[int]]):
    """
    checks all nodes in the graph. If any node 
    has an 'epoch' attribute (indicating it already 
    has a level assigned), it deletes the 'epoch' attribute.
    """
    print("===> welcome in grade_nx_graph : \n")
    for node in graph:
        if 'epoch' in graph.nodes[node]:
            del graph.nodes[node]['epoch']

    for i, group in enumerate(groups):
        
        for node in group:
            print(f'Setting {group} ==> {node} to {i}')
            graph.nodes[(node,)]['epoch'] = i

    def visit(node: int):
        if 'epoch' in graph.nodes[node]:
            return
        children = {c for c, _ in graph.in_edges(node)} # type: ignore
        heights = set()
        for child in children:
            if 'epoch' not in graph.nodes[child]:
                visit(child)
            heights.add(graph.nodes[child]['epoch'] + 1)
        print(f"assign epoch to node : {node} = {max(heights | {len(groups)})}  , heights :{heights} , len(groups) : {len(groups)} ")
        graph.nodes[node]['epoch'] = max(heights | {len(groups)})
    print("==> Loop over nodes : ")
    for node in graph:
        visit(node)

#######################################################################
######################Done#################################################

def producers(graph: nx.DiGraph, nbunch) -> Set[int]:
    return {p for p, _ in graph.in_edges(nbunch)} # type: ignore

#######################################################################
#Done#############################################

def nx_columnize(_graph: nx.DiGraph, force_lanes: dict[int, int]):
    # graph = nx.quotient_graph(_graph.to_undirected(), lambda u, v: u in force_lanes and v in force_lanes and force_lanes[u] == force_lanes[v])
    # graph = _graph
    print("==> Welcome in nx_culminize")
    # The graph is converted to an undirected graph
    graph: nx.Graph = cast(nx.Graph, _graph.to_undirected())
    
    epochs: Dict[int, List[int]] = defaultdict(list)
    # Nodes are grouped by their epoch attribute
    for node in graph:
        epochs[_graph.nodes[node]['epoch']].append(node)

    num_epochs = max(epochs.keys()) + 1

    # print('epochs:', epochs)
    

    # bipartite pieces, indexed by (source, target) epoch
    pieces: Dict[Tuple[int, int], nx.graph.Graph] = {}
    for i in range(num_epochs): # i = source epoch
        for j in range(i + 1, num_epochs): # j = target epoch
            # part1 = set(epochs[i])
            # part2 = consumers(_graph, epochs[i]).intersection(set(epochs[j]))
            part1 = set(epochs[j])
            #print(f"part1 : {part1}") # {(6,)}
            #print(f"==> {producers(_graph, epochs[j])} , {epochs[i]}")
            # epochs[i] = [(2,), (5,)] i = 4
            part2 = producers(_graph, epochs[j]).intersection(epochs[i])
            # {(2,), (5,)}
            #print(f"part2 : {part2}")
            # part2 = epochs[j]
            #print(f'Parts of {i, j}: {part1, part2}')
            # have to make a copy so that it can be modified independently
            bp_subgraph: nx.Graph = nx.Graph(graph.subgraph(part1 | part2))
            bp_subgraph.add_nodes_from(part1, bipartite=0)
            bp_subgraph.add_nodes_from(part2, bipartite=1)

            if bp_subgraph.number_of_edges() == 0:
                # nothing to see here, moving on...
                continue
            print("==> Obtained bp_subgraph :")
            display_digraph(bp_subgraph)
            """
            6 -- 2 : weight = 3
            6 -- 5 : weight = 3
            """
            for x in part1:
                for y in part2:
                    weight = bp_subgraph.degree[x] + bp_subgraph.degree[y]
                    if bp_subgraph.has_edge(x, y):
                        print(f'Weighting {x, y} = {weight}')
                        bp_subgraph[x][y]['weight'] = weight

            # print(f'Putting {bp_subgraph.edges} in for {i, j}')
            pieces[i, j] = bp_subgraph
            # print(f'{i, j} weights: {bp_subgraph.edges(data=True)}')
    # print(pieces)
    columns: DisjointSet[tuple[int]] = DisjointSet()
    #total_degree = 0
    
    preloaded: dict[int, set[tuple[int]]] = defaultdict(set)
    #print(f"===> force lanes  : {force_lanes}")
    for node in force_lanes:
        preloaded[force_lanes[node]].add((node,))
    #print(f"===> preloaded.values : {preloaded.values()}")
    for group in preloaded.values():
        columns.new_class(*group)
    print("============> Loop over sorted keys : ===========>")
    for i, j in sorted(pieces.keys()):
        bp_piece = pieces[i, j]
        # print(f'Full bp piece {i, j}: {bp_piece.edges}')
        # create a set part of all nodes in the bipartite 
        # graph bp_piece that are labeled as bipartite
        part = set(n for n, d in bp_piece.nodes(data=True) if d['bipartite']) # type: ignore

        # TODO: this is not the right condition for marking an edge 'unmatchable'
        ## also check if the edge connects an unmatched vertex to one already matched with something in the same epoch
        print(f"===> initial bp_piece : {display_digraph(bp_piece)}")
        matchable_graph = nx.graphviews.subgraph_view(bp_piece, filter_edge=lambda u, v: not (columns.contains(u) and columns.contains(v))) # type: ignore
        #print(f"===> subgraph view : {display_digraph(matchable_graph)}")
        # matchable_graph = nx.graphviews.subgraph_view(matchable_graph, filter_edge=lambda u, v: not (u in force_lanes and v in force_lanes and force_lanes[u] != force_lanes[v]))
        
        # print(f'Marking edges {[(u, v) for u, v in bp_piece.edges if columns.contains(u) and columns.contains(v)]} as unmatchable')
        # print(f'{matchable_graph.edges} are all matchable')
        matching = nx.algorithms.max_weight_matching(matchable_graph, maxcardinality=True)
        #print(f"matching ==> {matching}")

        # print(f'Querying weights for {matching}')
        weight = sum(bp_piece[u][v]['weight'] for u, v in matching)
        print(f'<==> Matching for {i, j}: {matching} (weight={weight})')
        

        for u, v in matching:
            # check that u and v are not columns regardless 
            # if they are or not in the same set
            assert not (columns.contains(u) and columns.contains(v)), (u, v)

            if columns.contains(u):
                columns.add_to(v, u)
            elif columns.contains(v):
                columns.add_to(u, v)
            else:
                columns.add(u)
                columns.add_to(v, u)
       

        columns.add(*filter(lambda p: not columns.contains(p), part))

        rotation_graph = nx.graphviews.subgraph_view(graph, filter_edge=lambda u, v: (u, v) not in matching and (v, u) not in matching) # type: ignore

        #total_degree += max(rotation_graph.degree(), key=lambda n: n[1])[1]
    # list(map(print, map(sorted, columns.all_classes())))
    # quit()
    print("==> Display final columns content : ")
    display_columns(columns)
    for i, col in enumerate(columns.all_classes()):
        for node in col:
            _graph.nodes[node]['column'] = i

    return columns

#######################################################################
#Done#################################################################

def rotation_cost(graph: nx.DiGraph, debug=False):
    spans: Dict[int, Set[int]] = defaultdict(set)
    for v1, v2 in graph.edges:
        span = graph.nodes[v2]['column'] - graph.nodes[v1]['column']
        print(f"==> source_node : {v1} , destination_node : {v2} , span :{span}")
        if span == 0:
            continue
        src = graph.nodes[v1]['epoch']
        spans[src].add(span)
    print(f"===> obtained spans : {spans}")
    # summing the lengths of the sets in spans.values()
    cost = sum(map(len, spans.values()))
    if debug:
        print(f'Spans: {spans}; {cost}')

    return cost, spans

#######################################################################
#Done#####################################################

def permute(graph, num_cols, num_epochs):
    # randomly seclect two columns 
    n1 = randint(0, num_cols - 1)
    n2 = randint(0, num_cols - 1)
    # Randomly selects epochs for which nodes will be swapped. 
    # Each epoch has a 50% chance of being included.
    epochs = set(filter(lambda _: random() < 0.5, range(num_epochs)))

    if n1 == n2:
        return permute(graph, num_cols, num_epochs)
    # s1 and s2 are sets of nodes in columns n1 and n2, respectively, 
    # and whose epoch values are in the selected epochs 
    s1 = {n for n, d in graph.nodes(data=True) if d['column'] == n1 and d['epoch'] in epochs}
    s2 = {n for n, d in graph.nodes(data=True) if d['column'] == n2 and d['epoch'] in epochs}

    if not s1 and not s2:
        return permute(graph, num_cols, num_epochs)

    return s1, s2, n1, n2

#######################################################################
#Done######################################################################

def lane_placement(graph: nx.DiGraph, force_lanes: dict[int, int], t=10, beta=0.05, rounds=10000):
    current, _ = rotation_cost(graph)
    num_cols = 1 + max(d for _, d in graph.nodes(data='column')) # type: ignore
    num_epochs = 1 + max(d for _, d in graph.nodes(data='epoch')) # type: ignore

    if num_cols == 1:
        return current

    best_cost = current
    best_graph = nx.DiGraph(graph)

    for _ in range(rounds):
        # update temperature
        t /= (1 + t * beta)

        # generate candidate solution
        s1, s2, n1, n2 = permute(graph, num_cols, num_epochs)
        # disallow changing lanes which have already been fixed
        fixed = set(force_lanes.keys())
        s1 = {n for n in s1 if not set(n).intersection(fixed)}
        s2 = {n for n in s2 if not set(n).intersection(fixed)}
        # s1 -= set(force_lanes.keys())
        # s2 -= set(force_lanes.keys())
        # apply permutation
        # move s1 nodes to column n2 and s2 nodes to column n1
        graph.add_nodes_from(s1, column=n2)
        graph.add_nodes_from(s2, column=n1)
        

        new_cost, _ = rotation_cost(graph)

        # print(f'Trying to permute {n1, n2} (acceptance = {min(100 * exp((current - new_cost) / t), 100)}%)')
        
        if new_cost < current or random() < exp((current - new_cost) / t):
            # print(f'\tCost {current} -> {new_cost}')
            current = new_cost
            
            # print('\taccepting!')
            # orig[n1], orig[n2] = orig[n2], orig[n1]
        else:
            graph.add_nodes_from(s1, column=n1)
            graph.add_nodes_from(s2, column=n2)

        if current < best_cost:
            best_cost = current
            best_graph = nx.DiGraph(graph)

    graph.update(best_graph)
    return best_cost
# Done
def contract_edge(graph: nx.DiGraph, edge):
    u, v = edge
    contracted = nx.contracted_edge(graph, edge, self_loops=False)
    return nx.relabel_nodes(contracted, {u: u + v}, copy=False)

#######################################################################
#######################################################################
"""
def iterate_relax_schedule(graph: nx.DiGraph, groups: List[Set[int]], force_lanes: dict[int, int], max_iter=10, max_edges=5, sample_freq=0.3):
    # setup: add metadata to the graph
    grade_nx_graph(graph, groups)
    nx_columnize(graph, force_lanes)
    # anneal the cells into a good column arrangement
    base_cost = lane_placement(graph, force_lanes)

    for _ in range(max_iter):
        # annotate for cross edges, used to find contraction candidates
        ann = defaultdict(set) 
        for u, v in graph.edges:
            src = graph.nodes[u]['epoch']
            span = graph.nodes[v]['column'] - graph.nodes[u]['column']
            
            if span == 0: continue
            if src < len(groups): continue
            
            ann[src, span].add((u, v))

        # sort annotations by which ones appear the least frequently (corresponds to good candidates to contract)
        # then iterate through the edges in that order, stopping at max_edges
        edges_to_try: List = sum((list(ann[a]) for a in sorted(ann.keys(), key=lambda a:len(ann[a]))), [])[:max_edges]
        # print(f'Trying contracting {len(edges_to_try)} different edges...')
        options = [(base_cost, graph)]

        for edge in edges_to_try:
            contracted = contract_edge(graph, edge)
            grade_nx_graph(contracted, groups)
            contracted_cost = lane_placement(contracted, force_lanes)

            if contracted_cost <= base_cost:
                options.append((contracted_cost, contracted))

        # print(f'{len(options) - 1} option(s) better than (or equal to) the baseline!')

        if len(options) == 1:
            # print('Ending...')
            # none of the contraction options were better than the baseline, so exit out now
            break

        # otherwise, set the new graph to the current best and start over
        base_cost, graph = min(options)

    return graph, base_cost
def bfs_relax_schedule(graph: nx.DiGraph, groups: List[Set[int]], force_lanes: dict[int, int], max_edges=10, max_iter=20):
    # compute initial metadata
    grade_nx_graph(graph, groups)
    nx_columnize(graph, force_lanes)

    # anneal into a good column arrangement
    min_cost = lane_placement(graph, force_lanes, t=50, beta=0.01, rounds=50000)
    best_graph = graph

    queue = [graph] # queue of schedules to anneal

    for i in range(max_iter):
        if not len(queue):
            print('No more graphs to try!')
            break

        print(f'Iteration {i}/{max_iter}: {len(queue)} graphs left in the queue!')

        cur = queue.pop(0)

        # annotate cross edges, used to find contraction candidates
        ann = defaultdict(set)
        for u, v in cur.edges:
            src = cur.nodes[u]['epoch']
            span = cur.nodes[v]['column'] - cur.nodes[u]['column']

            if span == 0: continue
            if src < len(groups): continue

            ann[src, span].add((u, v))

        # sort annotations by which ones appear the least frequently (good candidates to contract)
        # then iterate through edges in that order, stopping at max_edges
        edges_to_try: list = sum((list(ann[a]) for a in sorted(ann.keys(), key=lambda a:len(ann[a]))), [])[:max_edges]
        print(f'Trying contracting {len(edges_to_try)} different edges...')

        all_candidate_edges = set().union(*ann.values())

        costs_this_iter = []
        contracted_graphs = []
        for edge in edges_to_try:
        # for _ in range(max_edges):
            # edges_to_contract = list(filter(lambda e: random() < 0.5, all_candidate_edges))
            # contracted = cur
            # num_contracted = 0
            # for edge in edges_to_contract:
            #     try:
            #         contracted = contract_edge(contracted, edge)
            #         num_contracted += 1
            #     except ValueError:
            #         pass
            contracted = contract_edge(cur, edge)
            # print(f'Contracted {num_contracted} edges')
            grade_nx_graph(contracted, groups)
            contracted_cost = lane_placement(contracted, force_lanes)

            if contracted_cost > min_cost:
                print(f'\t(failed to beat {min_cost}, skipping this one)')
                continue
            print('\tBetter than or equal to the baseline!')
            costs_this_iter.append(contracted_cost)
            contracted_graphs.append(contracted)

        if not costs_this_iter:
            continue
        min_cost, best_graph_idx = min(zip(costs_this_iter, range(len(costs_this_iter))))
        best_graph = contracted_graphs[best_graph_idx]
        queue += contracted_graphs

    return best_graph, min_cost
"""
#######################################################################
#Done######################################################################

def schedule_height(graph: nx.DiGraph, debug=False):
    print("=====================================>")
    print("====== welcome in schedule height ==>")
    cells: Dict[int, Dict[int, tuple]] = defaultdict(lambda: defaultdict(tuple))
    print()
    for node in graph:
        cells[graph.nodes[node]['epoch']][graph.nodes[node]['column']] += node
    print(f"===> cells : {cells}")
    columns: Dict[int, tuple] = defaultdict(tuple)
    for node in graph:
        columns[graph.nodes[node]['column']] += node
    print(f"===> columns : {columns}")
    """
    The lambda function takes two Counter objects (x and y) 
    and combines them using the | operator.
    In Counter, | performs an element-wise 
    maximum of the counts, meaning: For keys present in both counters,
    their counts are set to the larger of the two.
    For keys unique to one counter, their counts are preserved.
    """
    """
    sum(..., Counter()) takes an iterable of Counter objects 
    and adds them together, starting with an empty Counter(). 
    This combines all counters from the iterable into a single Counter
    """
    instr_counts: Counter = sum(
        (reduce
            (lambda x, y: x | y, 
                (
                     Counter(graph.graph['ops'][instr] for instr in column) 
                    for column in epoch.values()
                )
            ) for epoch in cells.values()
        ), Counter()
    )
    print(f"==> instr_counts : {instr_counts}")
    cost = sum(COSTS_PER_ROTATE[op] * count for op, count in instr_counts.items())
    print(f"Final cost : {cost}")
    if debug:
        print(f'Instruction counts: {instr_counts}; {cost}')

    return cost

#######################################################################
#######################################################################
"""
def anneal_relax_schedule(graph: nx.DiGraph, groups: List[Set[int]], force_lanes: dict[int, int], t=20, beta=0.001, rounds=100, plot=False, max_restarts=None):
    # initial metadata
    grade_nx_graph(graph, groups)
    nx_columnize(graph, force_lanes)

    # anneal into a good column arrangement
    current_cost = lane_placement(graph, force_lanes, t=50, beta=0.001, rounds=50000)
    current_cost += schedule_height(graph)

    cur = nx.DiGraph(graph)

    best_cost = current_cost
    best_graph = nx.DiGraph(graph)

    already_visited: Set[Tuple[Any, Any]] = set()

    cost_history = []
    num_restarts = 0

    for round in range(rounds):
        t /= (1 + t * beta)

        # generate candidate solution
        ann = defaultdict(set)
        for u, v in cur.edges:
            src = cur.nodes[u]['epoch']
            span = cur.nodes[v]['column'] - cur.nodes[u]['column']

            if span == 0: continue
            if src < len(groups): continue

            ann[src, span].add((u, v))

        all_candidate_edges = list(set().union(*ann.values()) - already_visited)
        if not all_candidate_edges:
            if max_restarts and num_restarts >= max_restarts:
                break
            cur = nx.DiGraph(best_graph)
            current_cost = best_cost
            already_visited = set()
            num_restarts += 1
            continue
        # all_candidate_edges.sort()
        edge_to_contract = choice(all_candidate_edges)
        
        raw_contracted = contract_edge(cur, edge_to_contract)
        contracted = cast(nx.DiGraph, nx.condensation(raw_contracted)) # candidate solution, condense any newly created SCCs to keep acyclic

        for node in contracted:
            contracted.nodes[node]['column'] = raw_contracted.nodes[next(iter(contracted.nodes[node]['members']))]['column']

        contracted.graph['ops'] = cur.graph['ops']

        relabeling = {num : sum(contracted.nodes[num]['members'], ()) for num in contracted}
        contracted = nx.relabel_nodes(contracted, relabeling)

        already_visited.add(edge_to_contract)
        
        # compute the new estimated (sub-annealed) cost
        grade_nx_graph(contracted, groups)
        contracted_cost = lane_placement(contracted, force_lanes)
        contracted_cost += schedule_height(contracted)

        # accept/reject the solution
        if contracted_cost < current_cost or random() < exp((current_cost - contracted_cost) / t):
            # accept!
            current_cost = contracted_cost
            cur = nx.DiGraph(contracted)
            already_visited = set()

        if current_cost < best_cost:
            best_cost = current_cost
            best_graph = nx.DiGraph(cur)

        cost_history.append(best_cost)

        print(f'Round {round}/{rounds}: current cost: {current_cost} (best seen = {best_cost}); prob of +1 cost = {exp(-1 / t) * 100:.1f}%')

    if plot:
        from matplotlib import pyplot as plt # type: ignore
        plt.plot(cost_history)
        plt.show()


    print('FINISHED ANNEALING')
    print('Final cost accounting:')
    rotation_cost(best_graph, debug=True)
    schedule_height(best_graph, debug=True)

    return best_graph, best_cost
"""
#######################################################################
#######################################################################

def pq_relax_schedule(graph: nx.DiGraph, groups: List[Set[int]], force_lanes: Dict[int, int], rounds=200):
    @dataclass(order=True)
    class schedule:
        cost: int | float
        graph: nx.DiGraph=field(compare=False)
        edges: Optional[List[Tuple]]=field(compare=False)
    print("current : pq_relax_schedule ===> Visit : grade_nx_graph \n")
    grade_nx_graph(graph, groups)
    print("current : pq_relax_schedule ===> Visit : nx_columnize \n")
    nx_columnize(graph, force_lanes)
    # print(force_lanes) 
    # print(graph.nodes(data=True))
    for reg, lane in force_lanes.items():
        # print(f'Setting {graph.nodes[reg,]["column"]} to {lane}')
        graph.nodes[reg,]['column'] = lane

    cost_history = []
    best_history = []
    # current_cost = lane_placement(graph, force_lanes, t=50, beta=0.001, rounds=50000)
    current_cost = lane_placement(graph, force_lanes, t=50, beta=0.001, rounds=2)
    # add to the current cost : cost of used operations Ex : 
    # == 2 Add * 0.1 + 2 Mul * 1
    current_cost += schedule_height(graph)
    
    for reg, lane in force_lanes.items():
        # print(f'Setting {graph.nodes[reg,]["column"]} to {lane}')
        graph.nodes[reg,]['column'] = lane

    best = schedule(cost=current_cost, graph=nx.DiGraph(graph), edges=None)
    
    pqueue: List[schedule] = []
    heappush(pqueue, schedule(cost=best.cost, graph=best.graph, edges=None))
    ## added by me
    rounds = 5 
    print("<=======================================================>")
    print(" =======> Welcome in Core Cost Reducation Rotations : ")
    for r in range(rounds):
        if not len(pqueue):
            # print('No more graphs to try!')
            break

        cur = heappop(pqueue)

        best_history.append(best.cost)
        cost_history.append(cur.cost)
        # if len(pqueue):
        #     print(f'Round {r}/200: exploring {cur.cost}{" (new best!)" if cur < best else ""}')
        if cur < best:
            best = schedule(cost=cur.cost, graph=nx.DiGraph(cur.graph), edges=None)
        print("==> Get the cross Edges :")
        if cur.edges is None:
            # generate all candidate solutions
            cross_edges = set()
            for u, v in cur.graph.edges:
                src = cur.graph.nodes[u]['epoch']
                span = cur.graph.nodes[v]['column'] - cur.graph.nodes[u]['column']

                if span == 0: continue
                if src < len(groups): continue
                cross_edges.add((u, v))
            print(f"==> Found cross edges : {cross_edges}")
            cur.edges = list(cross_edges)
        
        if not len(cur.edges):
            continue
            
        # grab an edge, remove it from the list, and add its contraction to the queue
        edge_to_contract = cur.edges.pop()
        print(f"Work on edge ==> {edge_to_contract}")
        # for edge_to_contract in cross_edges:
            # print(f'\tedge {edge_to_contract}...')
        raw_contracted = contract_edge(cur.graph, edge_to_contract)
        contracted = cast(nx.DiGraph, nx.condensation(raw_contracted))

        for node in contracted:
            contracted.nodes[node]['column'] = raw_contracted.nodes[next(iter(contracted.nodes[node]['members']))]['column']

        contracted.graph['ops'] = cur.graph.graph['ops']
        # { 0 : (3,4) , 1 : (5,6) }
        relabeling = {num : sum(contracted.nodes[num]['members'], ()) for num in contracted}
        contracted = nx.relabel_nodes(contracted, relabeling)
        print("Obtained graph after contraction and CCS components elimination")
        display_digraph(contracted)
        print("==> Next iteration :")
        grade_nx_graph(contracted, groups)
        contracted_cost = lane_placement(contracted, force_lanes, t=50, beta=0.001, rounds=20000)
        contracted_cost += schedule_height(contracted)

        heappush(pqueue, schedule(cost=contracted_cost, graph=nx.DiGraph(contracted), edges=None))
        if len(cur.edges): # if there are still unexplored edges
            heappush(pqueue, schedule(cost=cur.cost, graph=nx.DiGraph(cur.graph), edges=cur.edges[:])) # put this back into the queue

    """from matplotlib import pyplot as plt
    plt.plot(cost_history)
    plt.plot(best_history)
    plt.show()"""

    with open('trace.csv', 'w') as f:
        f.write(','.join(map(str, cost_history)) + '\n')
        f.write(','.join(map(str, best_history)) + '\n')

    return best
            
#######################################################################
#Done######################################################################
# yield tuples of nodes belonging to the same epoch(stage) 
def get_stages(graph: nx.DiGraph) -> Generator[Tuple[int], None, None]:
    cur_stage = 0
    
    epoch_grouping = defaultdict(set)
    for node in graph.nodes:
        epoch_grouping[graph.nodes[node]['epoch']].add(node)
        
    # print(f'Epoch grouping: {epoch_grouping}')
    
    while True:
        stage = ()
        for node in graph.nodes:
            if graph.nodes[node]['epoch'] == cur_stage:
                stage += node
        if not stage:
            break
        yield stage
        cur_stage += 1

#######################################################################
#######################################################################
def get_lanes(graph: nx.DiGraph) -> List[int]:
    print("\n===============> welcome in get lanes ======================> ")
    # filter(lambda n: isinstance(n, int), ...)
    # lanes = [0] * (1 + max((3,5,9)))
    lanes = [0] * (1 + max(filter(lambda n: isinstance(n, int), sum(graph.nodes, ()))))
    """
    graph.nodes(data=True)
    {
        (1, 2): {'column': 0},
        (3, 4): {'column': 1},
        (5,): {'column': 2}
        (nb_instr= 5) + 1
        lanes is initialized to [0, 0, 0, 0, 0, 0]
        [0, 0, 0, 1, 1, 2]
    }

    """
    # nodes [(2, 6) , (0,) , (1,) , (3,), (5,) ,(4,)]
    # lanes : [0, 0, 0, 0, 0, 0, 0]
    for node in graph.nodes:
        for instr in node:
            lanes[instr] = graph.nodes[node]['column']
    return lanes
        

def vectorize(comp: CompilerV2, lanes_out=[], sched_out=[], extra_force_lanes: dict[int, int]={}):
    # compute the schedule
    print("==> welcome in vectorize function \n")
    loaded_groups = [set().union(*(comp.loaded_regs[g] for g in group)) for group in comp.input_groups]
    print(f"loaded groups : {loaded_groups}")# [{0}, {1}, {3}, {4}] 
    # [{0, 18, 12, 6}, {1, 19, 13, 7}](hamming_dis)
    # each set contain values of the same set 
    loaded_lanes = {next(iter(comp.loaded_regs[g])): comp.force_lanes[g] for g in comp.force_lanes} | extra_force_lanes
    print(f"loaded lanes : {loaded_lanes}")
    graph = instr_sequence_to_nx_graph(comp.code)
    print(f"====> Initial graph : ")
    display_digraph(graph)
    # filter(function, iterable)
    # Applies function to each element in iterable (in this case, graph.nodes) 
    # and retains only those elements where the function returns True
    # in this case function <==> lambda n: all(isinstance(m, int) for m in n
    actual_instrs = list(filter(lambda n: all(isinstance(m, int) for m in n), graph.nodes))
    graph = nx.DiGraph(nx.subgraph(graph, actual_instrs))
    print(f"====> Updated graph : ")
    display_digraph(graph)
    print(f"====> Run pq_relax_schedule function :")
    # relaxed_schedule = pq_relax_schedule(graph, loaded_groups, loaded_lanes, rounds=200).graph
    relaxed_schedule = pq_relax_schedule(graph, loaded_groups, loaded_lanes, rounds=2).graph
     
    # relaxed_schedule, _ = anneal_relax_schedule(graph, loaded_groups, t=20, beta=0.001, rounds=200)

    # print('Column mapping:')
    # for node in relaxed_schedule.nodes:
        # print(node, relaxed_schedule.nodes[node]['column'])

    # shift to start at column 1 :)
    min_column = min(relaxed_schedule.nodes[node]['column'] for node in relaxed_schedule)
    for node in relaxed_schedule.nodes:
        relaxed_schedule.nodes[node]['column'] -= min_column

    # warp_size = max(columns) + 1
    warp_size = max(relaxed_schedule.nodes[node]['column'] for node in relaxed_schedule) + 1
    # return a list containing the column corresponding to each 
    # instruction index Ex  (Int 0: col 0 , Int 1 : col1 , Int2 : col0)
    # ==> [0,1,0]

    lanes = get_lanes(relaxed_schedule)
    vector_program: List[VecSchedule] = []
    # schedule will contain the execution order of each instruction
    # in the program 
    schedule = [0] * len(comp.code) 
    # # yield tuples of nodes belonging to the same epoch(stage) 
    for stage in get_stages(relaxed_schedule):
        stage_instrs = [comp.code[i] for i in stage]
        stage_sched = synthesize_schedule(stage_instrs, warp_size, lanes)
        for s, i in zip(stage_sched, stage_instrs):
            assert isinstance(i.dest.val, int)
            schedule[i.dest.val] = s + len(vector_program)
        vector_program += build_vector_program(stage_instrs, lanes, stage_sched)

    active_lanes: List[List[int]] = [[] for _ in range(max(schedule) + 1)]
    for instr in comp.code:
        assert isinstance(instr.dest.val, int)
        # return a list of list of columns of instructions that will be executed 
        # at the same time according ot the schedule 
        active_lanes[schedule[instr.dest.val]].append(lanes[instr.dest.val])
    print(f'==> Final : Active lanes: {active_lanes}')
    """
    inv_schedule = [[-1] * (max(lanes) + 1) for _ in range(max(schedule) + 1)]
    for instr in comp.code:
        assert isinstance(instr.dest.val, int)
        inv_schedule[schedule[instr.dest.val]][lanes[instr.dest.val]] = instr.dest.val
    """
    # print(relaxed_schedule.nodes)
    lanes_out[:] = lanes
    sched_out[:] = schedule

    return codegen(vector_program, relaxed_schedule, lanes, schedule, warp_size)


# coyote = coyote_compiler()

# @coyote.define_circuit(a=matrix(3, 3), b=matrix(3, 3))
# def matmul(a, b):
#     return [recursive_sum([a[i][k] * b[k][j] for k in range(len(a))]) for i in range(len(a)) for j in range(len(a))]

# @coyote.define_circuit(sig=vector(4), ker=vector(2))
# def conv(sig, ker):
#     output = []
#     for offset in range(len(sig) - len(ker) + 1):
#         output.append(recursive_sum([sig[offset + i] * ker[i] for i in range(len(ker))]))
#     return output


# @coyote.define_circuit(xs=vector(3), ys=vector(3))
# def distances(xs, ys):
#     return [(x - y) * (x - y) for x in xs for y in ys]


# def cond(b, true, false):
#     return b * true + (Var('1') + b) * false

# @coyote.define_circuit(c12=scalar(), c23=scalar(), c13=scalar(), o123=scalar(), o132=scalar(), o213=scalar(), o231=scalar(), o312=scalar(), o321=scalar())
# def sort_3(c12, c23, c13, o123, o132, o213, o231, o312, o321):
#     return cond(c12, 
#                 (cond(c23, 
#                     o123,
#                     cond(c13, o132, o312))), 
#                 (cond(c13,
#                     o213,
#                     cond(c23, o231, o321))))


# @coyote.define_circuit(cs=vector(3), os=vector(6))
# def sort_3_packed(cs, os):
#     return cond(cs[0], 
#                 (cond(cs[1], 
#                     os[0],
#                     cond(cs[2], os[1], os[4]))), 
#                 (cond(cs[2],
#                     os[2],
#                     cond(cs[1], os[3], os[5]))))


# @coyote.define_circuit(a=scalar(), b=scalar())
# def func(a, b):
#     return a * b + a + b


# @coyote.define_circuit(v1=vector(4), v2=vector(4))
# def dot_product(v1, v2):
#     return recursive_sum([a * b for a, b in zip(v1, v2)])


# COYOTE_VERSION = 2
# BENCHMARK = 'distances'

# if COYOTE_VERSION == 1:
#     scalar_code = coyote.instantiate(BENCHMARK)
#     vector_code, _ = coyote.vectorize()
# elif COYOTE_VERSION == 2:

#     groups, outputs = coyote.get_outputs(BENCHMARK)

#     # comp = Compiler({}, input_groups=groups)
#     comp = CompilerV2(groups)
#     tops = []
#     for out in outputs:
#         print(out)
#         tops.append(comp.compile(out))


#     # calc = SimilarityCalculator(comp.code)
#     # calc.calculate(tops)

#     scalar_code = list(map(str, comp.code))
#     vector_code = vectorize(comp)

# print('\n'.join(scalar_code))
# print('=' * 20)
# print('\n'.join(vector_code))
