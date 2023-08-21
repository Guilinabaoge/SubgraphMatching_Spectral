import walker
import graph_parser as gp


dataset = "patents"
datagraphpath = f"../reallife_dataset/{dataset}/data_graph/{dataset}.graph"
G = gp.graph_parser(datagraphpath)
walks = walker.random_walks(G, n_walks=15, walk_len=10)
print(walks)