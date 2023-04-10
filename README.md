How to run the code: 

In the main function adjust follow parameters: 
1. Experiments::datagraphEigenMatrix: ->the path of the eigen matrix for the data graph
2. datagraph -> datagraph path
3. querygraph -> querygraph path
4. file_path -> path where you want to store the experiment result 

Then compile the code and run with following command:
./SubgraphMatching.out -dataset <datasetname> -qsize <query size> -qnumber <query number> -qprop <query property e.g dense>


How to interprete the experiment result:
* Column[0:1] Query_Name
* Column[1:14] Iteration count for LDF,LDF+EF,NLF,NLF+EF,GQL,GQL+EF,TSOF,TSOF+EF,CFL,CFL+EF,DPiso,DPiso+EF,KF 
* Column[14:15] Embedding count
* Column[15:28] Total time(second) for LDF,LDF+EF,NLF,NLF+EF,GQL,GQL+EF,TSOF,TSOF+EF,CFL,CFL+EF,DPiso,DPiso+EF,KF 
* Column[29:42] Candidate sum 
* Column[42:55] Matching order (second)
* Column[55:68] Preprocessing time (second)
* Column[68:81] Enumeration time (second)
