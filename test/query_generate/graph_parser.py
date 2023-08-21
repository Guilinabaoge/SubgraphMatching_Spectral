import networkx as nx

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