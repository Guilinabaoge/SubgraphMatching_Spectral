import networkx as nx
import random
import argparse
import itertools
import graph_parser as gp
import os
import time



def mapping(dict,label):
    for key in dict.keys():
        if label in key:
            return dict[key]
   
    raise ValueError('Invalid label: {0}'.format(label))
        

def mapping_label(opath,path): 
    label_dict = {} 
    for x in range(5):
        start = x*5
        end = start+5
        label_dict[range(start,end)] = x 

    
    G = gp.graph_parser(opath)
    with open(path,'w') as f: 
        f.write('t ' + str(len(G.nodes())) + ' ' + str(len(G.edges())) + '\n')
        for node in G.nodes():
            f.write('v ' + str(node) + ' ' + str(mapping(label_dict,G.nodes[node]['label'])) + ' ' +  str(G.degree(node)) + '\n')
        # str(subgraph.nodes[node]['label'])
        for edge in G.edges():
            f.write('e ' + str(edge[0]) + ' ' + str(edge[1]) + ' ' +  '0' + '\n')


def mapping_label2(opath,path):    
    G = gp.graph_parser(opath)
    with open(path,'w') as f: 
        f.write('t ' + str(len(G.nodes())) + ' ' + str(len(G.edges())) + '\n')
        for node in G.nodes():
            f.write('v ' + str(node) + ' ' + str(0) + ' ' +  str(G.degree(node)) + '\n')
        # str(subgraph.nodes[node]['label'])
        for edge in G.edges():
            f.write('e ' + str(edge[0]) + ' ' + str(edge[1]) + ' ' +  '0' + '\n')
        

def generate_small_world_subgraph(size,path):

    subgraph = nx.connected_watts_strogatz_graph(size, int(size/4), 0.3)

    with open(path,'w') as f: 
        
        f.write('t ' + str(len(subgraph.nodes())) + ' ' + str(len(subgraph.edges())) + '\n')
        for node in subgraph.nodes():
            f.write('v ' + str(node) + ' ' + '0' + ' ' +  str(subgraph.degree(node)) + '\n')
        # str(subgraph.nodes[node]['label'])
        for edge in subgraph.edges():
            f.write('e ' + str(edge[0]) + ' ' + str(edge[1]) + ' ' +  '0' + '\n')

# Generate a subgraph of datagraph by randomwalk
def randomwalk(datagraph,size,prop):
    current_time = time.strftime("%Y-%m-%d %H:%M:%S")
    print("Current system time 1:", current_time)
    G = datagraph
    current_node = random.choice(list(G.nodes()))
    query = nx.Graph()
    counter = 0

    while query.number_of_nodes() < size:
        if counter > 500:
            print('counter exceed 500')
            return randomwalk(datagraph,size,prop)
        neighbors = list(G.neighbors(current_node))

        #TODO handle the situation of isolated node, where no label exist
        destination = random.choice(neighbors)
        query.add_node(current_node,label=G.nodes[current_node]['label']) 
        current_node = destination
        counter += 1

    current_time = time.strftime("%Y-%m-%d %H:%M:%S")
    print("Current system time 2:", current_time)
    for edge in G.edges():
        if edge[0] in query.nodes() and edge[1] in query.nodes():
            query.add_edge(edge[0],edge[1])

    counter = 0
    for node in query.nodes():
        mapping = {node:counter}
        query = nx.relabel_nodes(query,mapping)
        counter += 1

    current_time = time.strftime("%Y-%m-%d %H:%M:%S")
    print("Current system time 3:", current_time)

    if nx.is_connected(query) and query.number_of_nodes() == size:
        degrees = dict(query.degree())
        avg_degree = sum(degrees.values()) / len(degrees)
        print(avg_degree)
        if prop == "dense" and avg_degree >= 3:
                print('query generated successfully')
                return query
        elif prop == "sparse" and avg_degree < 3:
                print('query generated successfully')
                return query
        else:
            print('Invalid prop: {0} or unsatisfy degree: {1}'.format(prop,avg_degree))
            return randomwalk(datagraph,size,prop)

    else:
        print('query not generated successfully')
        return randomwalk(datagraph,size,prop)


 
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-s','--size',help='Query size')
    parser.add_argument('-p','--property',help='Query property')
    parser.add_argument('-d','--dataset',help='Dataset name')

    args = parser.parse_args()
    size = int(args.size)
    prop = args.property
    dataset = args.dataset

    datagraphpath = f"../reallife_dataset/{dataset}/data_graph/{dataset}.graph"
    # query_store_path = f"{dataset}/{size}/"


    for query_number in range(1,51):
        datagraph = gp.graph_parser(datagraphpath)
        G = randomwalk(datagraph,size,prop)
        store_path = f"wildcard_queries/{dataset}/0.0/query_{prop}_{size}_{query_number}.graph"
        # if not os.path.exists(store_path):
        #     os.makedirs(store_path)
        os.makedirs(os.path.dirname(store_path), exist_ok=True)
        with open(store_path,'w') as f: 
            f.write('t ' + str(len(G.nodes())) + ' ' + str(len(G.edges())) + '\n')
            for node in G.nodes():
                f.write('v ' + str(node) + ' ' + str(G.nodes[node]['label']) + ' ' +  str(G.degree(node)) + '\n')
            for edge in G.edges():
                f.write('e ' + str(edge[0]) + ' ' + str(edge[1]) + ' ' +  '0' + '\n')

if __name__ == "__main__":
    main()
