#!/bin/bash 
#This 


if [ -e "./wildcard_queries" ]; then 
    rm -rf ./wildcard_queries

    echo "remove wildcard_queries
"
fi

dataset32=("hprd" "yeast" "eu2005" "dblp" "patents" "youtube") 
dataset20=("human" "wordnet") 
query_size_32=("16" "24" "32")
query_size_20=("12" "16" "20")
prop=("dense" "sparse")
percentage=("0.25" "0.5" "0.75" "1.0")

mkdir wildcard_queries 

for dataset in "${dataset32[@]}"
do 
    mkdir ./wildcard_queries/${dataset}
    for p in "${percentage[@]}"
    do 
        mkdir ./wildcard_queries/${dataset}/${p}
        for prop in "${prop[@]}"
        do
            for size in "${query_size_32[@]}"
            do
                for query_number in {1..200}
                do
                query_path="/Users/zhiweizeng/Desktop/Thesis/reallife_dataset/${dataset}/query_graph/query_${prop}_${size}_${query_number}.graph"
                python3 generate_wildcard_query.py -s "$size" -prop $prop -d $dataset -q "$query_path" -p $p -n $query_number
                echo "$size" -prop $prop -d $dataset -q $query_path -p $p -n $query_number
                done
            done
        done 
    done
done



for dataset in "${dataset20[@]}"
do 
    mkdir ./wildcard_queries/${dataset}
    for p in "${percentage[@]}"
    do 
        mkdir ./wildcard_queries/${dataset}/${p}
        for prop in "${prop[@]}"
        do
            for size in "${query_size_20[@]}"
            do
                for query_number in {1..200}
                do
                query_path="/Users/zhiweizeng/Desktop/Thesis/reallife_dataset/${dataset}/query_graph/query_${prop}_${size}_${query_number}.graph"
                python3 generate_wildcard_query.py -s "$size" -prop $prop -d $dataset -q "$query_path" -p $p -n $query_number
                echo "$size" -prop $prop -d $dataset -q $query_path -p $p -n $query_number
                done
            done
        done 
    done
done






# for dataset in "${dataset20[@]}"
# do 
#     mkdir ./wildcard_queries/$dataset
#     for p in "${percentage[@]}"
#     do 
#         mkdir ./wildcard_queries/$dataset/$p
#     done
# done





