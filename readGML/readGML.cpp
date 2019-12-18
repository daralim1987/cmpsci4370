/********************************************************************************
*
*	readGML.cpp:	Code for reading in a gml file and storing it as a set of 
*               linked lists.
*                 
*
*                       Sharlee Climer
*                       March 2019
*
*      
*
*********************************************************************************/
  

#include "readGML.h"
#include "network.h"


int main(int argc, char ** argv)
{
  if (argc != 2)
    fatal("Usage:\n  readGML input.gml"); 

  FILE *input;
  timer t;

  t.start("Timer started");
  
  if ((input = fopen(argv[1], "r")) == NULL) 
    fatal("File could not be opened.\n");

  int min = MAX_NUM_NODES;   // hold min node number
  int max = -MAX_NUM_NODES;  // hold max node number
  char strng[STRNG_LENGTH];  // hold temporary strings
  int startOne; // 1 if start node number is 1, 0 if start number is 0

  while (1) { // throw away everything until get to "graph" declaration
    if(feof(input)) fatal("no graph to read in input file");

    fscanf(input, "%s", strng); // read in string from file

    if(strncmp(strng, "graph", 5) == 0) 
      break; // stop when get to "graph" declaration
  }

  // find number of nodes in graph
  int numNodes = 0; // number of nodes
  int numEdges = 0; // number of edges

  while (1) { // throw away everything until get to "edge" declaration
    if(feof(input)) fatal("no edges to read in input file");

    fscanf(input, "%s", strng); // read in string from file

    if(strncmp(strng, "id", 2) == 0) { 
      fscanf(input, "%s", strng); // read in id value
      int num = atoi(strng);

      if(min > num) min = num; // update minimum node number
      if(max < num) max = num; // update maximum node number
      numNodes++; // one more node found
    }

    if(strncmp(strng, "edge", 4) == 0) 
      break; // stop when read in first "edge"
  }

  std::cout << "\nNode numbers range from " << min <<" to " << max << std::endl;
  std::cout << "Reading and storing graph from " << argv[1] << "...\n" << std::endl;

  startOne = min;
  rewind(input); // start at beginning of input file again

  if(startOne < 0) fatal("Node numbers can not be negative");
  if ((startOne != 0) && (startOne != 1)) 
    warning("Smallest node number should be 0 or 1");

  // record node id numbers
  int ptr = 0; // pointer for filling nodeNumbers array
  int *id; // hold the node IDs as given in input file
  if ((id = new int[numNodes]) == NULL)
    fatal("memory not allocated");

  while (1) { // throw away everything until get to "graph" declaration
    if(feof(input)) fatal("no graph to read in input file");

    fscanf(input, "%s", strng); // read in string from file

    if(strncmp(strng, "graph", 5) == 0) 
      break; // stop when get to "graph" declaration
  }

  while (1) { // record node id numbers 
    if(feof(input)) fatal("no edges to read in input file");

    fscanf(input, "%s", strng); // read in string from file
  
    if(strncmp(strng, "id", 2) == 0) { 
      fscanf(input, "%s", strng); // read in id value
      int num = atoi(strng);

      if((num < min) || (num > max))
	fatal("error while reading in node numbers");

      id[ptr++] = num; // record node id number
      //std::cout << ptr << ", " << id[ptr-1]<< std::endl;
    }

    if(strncmp(strng, "edge", 4) == 0) 
      break; // stop when read in first "edge"
  }

  if (ptr != numNodes)
    fatal("Error reading in node numbers");

  int *idInv; // invert the ID numbers for easy look-up
  if ((idInv = new int[max+1]) == NULL)
    fatal("memory not allocated");

  //std::cout << "id: " ;
  for (int i = 0; i < max+1; i++)
    idInv[i] = -1; // initialize values
  for (int i = 0; i < numNodes; i++) {
    //std::cout << id[i] << " ";
    if(idInv[id[i]] != -1)
      fatal("Error recording index for ID number");
    idInv[id[i]] = i; // record index for given id number
  }
  //std::cout << std::endl;

  if (0) {
    std::cout << "idInv: ";
    for (int i = 0; i < max+1; i++)
      std::cout << idInv[i] << " ";
    std::cout << std::endl;
  }

  if(DIRECTED)
    fatal("Directed graphs aren't currently supported");
    
  // create network with numNodes vertices
  Network sparseNet(numNodes, DIRECTED); // if DIRECTED = 0, undirected
  int dupEdges = 0; // record number of duplicate edges

  while (!feof(input)) { // read edges until reach end of file
    fscanf(input, "%s", strng); // read in string from file

    if(!strncmp(strng, "[", 1) == 0) // check for correct character
      fatal("No '[' after edge declaration");

    fscanf(input, "%s", strng); // read in start from file
    if(!strncmp(strng, "source", 6) == 0) // check for correct character
      fatal("No 'source' declaration");

    fscanf(input, "%s", strng); // read in start node from file
    int source = atoi(strng);

    if((source < min) || (source > max))
      fatal("Invalid node number");

    //std::cout << "source: " << idInv[source];

    fscanf(input, "%s", strng); // read in target from file
    if(!strncmp(strng, "target", 6) == 0) // check for correct character
      fatal("No 'target' declaration");

    fscanf(input, "%s", strng); // read in target node from file
    int target = atoi(strng);

    if((target < min) || (target > max))
      fatal("Invalid node number");

    //std::cout << ", target: " << idInv[target];

    fscanf(input, "%s", strng); // read in weight from file

    float weight = 1.0;
    if(strncmp(strng, "weight", 6) == 0) {// check to see if weight specified
      fscanf(input, "%s", strng); // read in string from file
      weight = atof(strng);
      fscanf(input, "%s", strng); // read in string from file
    }

    //std::cout << ", weight: " << weight << std::endl;
    
    if(!sparseNet.addEdge(idInv[source],idInv[target],weight))
	  dupEdges++;
      //warning("Duplicate edge in input");
    
    else
      numEdges++; // count number of edges

	if(numEdges % 10000000 == 0) // message every 10 million edges
	  std::cout << numEdges / 1000000 << " million edges read" << std::endl;

    if(!strncmp(strng, "]", 1) == 0) // check for correct character
      fatal("No ']' after edge declaration");

    if (feof(input)) break; // break is at end of file

    fscanf(input, "%s", strng); // read in string from file

    if(strncmp(strng, "]", 1) == 0) // check for end of graph
      break;

    if(!strncmp(strng, "edge", 4) == 0) // check for correct character
      fatal("No 'edge' declaration");
  }

  //std::cout << std::endl;
 
  fclose(input);

  if (sparseNet.getNumEdges() != numEdges)
    fatal("error recording edges in network");

  std::cout << numEdges << " edges explored" << std::endl;
  std::cout << dupEdges << " duplicate edges not counted in edge count" << std::endl;
  //if (dupEdges > 0)
  //std::cout << "Highest edge weight recorded for duplicate edges" << std::endl;

  delete [] id;
  delete [] idInv;

  t.stop("Timer stopped");
  std::cout << t << " seconds" << std::endl;

  return 1;
}

