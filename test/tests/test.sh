#!/bin/bash

python3 query_generator.py -s 64 -p dense -d patents
wait
python3 generate_wildcard_query.py -s 64 -prop dense -d patents -p 0.75 -n 1

