import networkx as nx
import random
import argparse
import itertools

def graph_parser(path):
    # Open the file and read its contents
    with open(path, 'r') as f:
        lines = f.readlines()

    # Initialize an empty graph object
    G = nx.Graph()

    # Parse the vertices and edges
    for line in lines:
        tokens = line.strip().split()
        if tokens[0] == 'v':
            # Add a new vertex with its attributes
            vertex_id = int(tokens[1])
            vertex_attributes = {'label': int(tokens[2]), 'degree': int(tokens[3])}
            G.add_node(vertex_id, **vertex_attributes)
        elif tokens[0] == 'e':
            # Add a new edge with its attributes
            edge_start = int(tokens[1])
            edge_end = int(tokens[2])
            G.add_edge(edge_start, edge_end)
    
    return G

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

    
    G = graph_parser(opath)
    with open(path,'w') as f: 
        f.write('t ' + str(len(G.nodes())) + ' ' + str(len(G.edges())) + '\n')
        for node in G.nodes():
            f.write('v ' + str(node) + ' ' + str(mapping(label_dict,G.nodes[node]['label'])) + ' ' +  str(G.degree(node)) + '\n')
        # str(subgraph.nodes[node]['label'])
        for edge in G.edges():
            f.write('e ' + str(edge[0]) + ' ' + str(edge[1]) + ' ' +  '0' + '\n')


def mapping_label2(opath,path):    
    G = graph_parser(opath)
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
def randomwalk(datagraph_path,size,prop): 
    G = graph_parser(datagraph_path)
    current_node = random.choice(list(G.nodes()))
    query = nx.Graph()
    counter = 0

    while query.number_of_nodes() < size:
        if counter > 500:
            print('counter exceed 500')
            return randomwalk(datagraph_path,size,prop)

        neighbors = list(G.neighbors(current_node))

        destination = random.choice(neighbors)
        query.add_node(current_node,label=G.nodes[current_node]['label']) 
        current_node = destination
        counter+=1
    
    for edge in G.edges():
        if edge[0] in query.nodes() and edge[1] in query.nodes():
            query.add_edge(edge[0],edge[1])

    counter = 0
    for node in query.nodes():
        mapping = {node:counter}
        query = nx.relabel_nodes(query,mapping)
        counter += 1
    
        
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
            return randomwalk(datagraph_path,size,prop)

    else:
        print('query not generated successfully')
        return randomwalk(datagraph_path,size,prop)





def wildcard(query,percentage,wildcard_label,query_number):
    G = graph_parser(query)
    size = G.number_of_nodes() 
    wild_num = int(size * percentage)
    rand_nums = random.sample(range(size),wild_num)
    print(rand_nums)
    for e in rand_nums:
        G.nodes[e]["label"] = wildcard_label

    store_path = '/Users/zhiweizeng/Desktop/Thesis/notes/wildcard/youtube/dense_{0}_{1}_{2}.graph'.format(size,query_number,percentage)
    with open(store_path,'w') as f: 
        f.write('t ' + str(len(G.nodes())) + ' ' + str(len(G.edges())) + '\n')
        for node in G.nodes():
            f.write('v ' + str(node) + ' ' + str(G.nodes[node]['label']) + ' ' +  str(G.degree(node)) + '\n')
        for edge in G.edges():
            f.write('e ' + str(edge[0]) + ' ' + str(edge[1]) + ' ' +  '0' + '\n')
    
            
        
  

                
def main(): 
    # for i in range(2,20):
    #     for e in [0,0.25,0.5,0.75,1]:
    #         wildcard("../reallife_dataset/youtube/query_graph/query_dense_32_{0}.graph".format(i),e,25,i)
    
    parser = argparse.ArgumentParser()
    parser.add_argument('-s','--size',help='Query size')
    parser.add_argument('-p','--property',help='Query property') 
    parser.add_argument('-d','--dataset',help='Dataset name') 

    args = parser.parse_args() 
    size = int(args.size) 
    prop = args.property
    dataset = args.dataset

    for x in range(1,201):
        #size = 50
        query_number = x
        #prop = "sparse"
        #dataset = "wordnet"
        G = randomwalk('reallife_dataset/{0}/data_graph/wordnet.graph'.format(dataset),size,prop)
        store_path = 'reallife_dataset/{3}/query_graph/query_{0}_{1}_{2}.graph'.format(prop,size,query_number,dataset)
        with open(store_path,'w') as f: 
            f.write('t ' + str(len(G.nodes())) + ' ' + str(len(G.edges())) + '\n')
            for node in G.nodes():
                f.write('v ' + str(node) + ' ' + str(G.nodes[node]['label']) + ' ' +  str(G.degree(node)) + '\n')
            for edge in G.edges():
                f.write('e ' + str(edge[0]) + ' ' + str(edge[1]) + ' ' +  '0' + '\n')
    
    # original_path = '/Users/zhiweizeng/Desktop/Thesis/reallife_dataset/youtube/data_graph/youtube.graph'
    # path = 'mydataset/youtube/data_graph/25-0/youtube.graph'
    # mapping_label2(original_path,path) 

    # dense = [16,24,32]
    # for size in dense: 
    #     for i in range(1,201):
    #         original_path = '/Users/zhiweizeng/Desktop/Thesis/reallife_dataset/youtube/query_graph/query_dense_{0}_{1}.graph'.format(size,i)
    #         path = 'mydataset/youtube/query_graph/25-0/query_dense_{0}_{1}.graph'.format(size,i)
    #         mapping_label2(original_path,path)
    
    # parser = argparse.ArgumentParser()
    # parser.add_argument('-s','--size',help='Query size',default=100)
    # parser.add_argument('-p','--path',help='Path to save query',default='mydataset/test.graph') 

    # args = parser.parse_args() 
    # query_size = int(args.size) 
    # query_path = args.path
    
    # generate_small_world_subgraph(query_size,query_path)
    # print('query with size {0} and path {1} generated successfully'.format(query_size,query_path))

if __name__ == "__main__":
    main()
