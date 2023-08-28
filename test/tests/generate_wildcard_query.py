import os
import random
import graph_parser as gp
import argparse

def wildcard(query,percentage,query_size,query_number,dataset,prop):
    G = gp.graph_parser(query)
    size = G.number_of_nodes()
    wild_num = int(size * percentage)
    rand_nums = random.sample(range(size),wild_num)

    wildcard_label = {
    'yeast': 71,
    'human': 44,
    'hprd': 307,
    'wordnet': 5,
    'patents': 20,
    'youtube': 25,
    'dblp': 15,
    'eu2005': 40
    }

    for e in rand_nums:
        G.nodes[e]["label"] = wildcard_label[dataset]

    store_path =  f"wildcard_queries/{dataset}/{percentage}/query_{prop}_{query_size}_{query_number}.graph"
    os.makedirs(os.path.dirname(store_path), exist_ok=True)
    with open(store_path,'w') as f: 
        f.write('t ' + str(len(G.nodes())) + ' ' + str(len(G.edges())) + '\n')
        for node in G.nodes():
            f.write('v ' + str(node) + ' ' + str(G.nodes[node]['label']) + ' ' +  str(G.degree(node)) + '\n')
        for edge in G.edges():
            f.write('e ' + str(edge[0]) + ' ' + str(edge[1]) + ' ' +  '0' + '\n')


# query_size = 128
# prop = "dense"
# dataset = "youtube"
# percentage = 0.75
# query_number = 1
# query_path = f"wildcard_queries/{dataset}/0.0/query_{prop}_{query_size}_1.graph"
# wildcard(query_path,percentage,query_size,query_number,dataset,prop)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-s','--size',help='Query size')
    parser.add_argument('-prop','--property',help='Query property')
    parser.add_argument('-d','--dataset',help='Dataset name')
    parser.add_argument('-p','--wildcard_percentage',help='Wildcard percentage')
    parser.add_argument('-n','--query_number',help='Query number')

    args = parser.parse_args()
    query_size = int(args.size)
    prop = args.property
    dataset = args.dataset
    percentage = float(args.wildcard_percentage)
    query_number = int(args.query_number)
    query_path = f"wildcard_queries/{dataset}/0.0/query_{prop}_{query_size}_{query_number}.graph"
    wildcard(query_path,percentage,query_size,query_number,dataset,prop)

if __name__ == '__main__':
    main()