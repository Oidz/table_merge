/* Professor Knowles, 
 * this program models a set of table merge operations in a database using 
 * trees of disjoint sets that implement 
 * path compression and the union by rank heuristic. It was an assignment
 * for week 3 of my current Data Structures and Algorithms course.
 
 * The input format is: 
 * [n, number of tables] [m, number of merge queries to perform]
 * [list of space-separated integers 1-n, the number of rows for each table]
 * [destination table] [source table to merge into destination]
 * [destination table] [source table]
 * [destination table] [source table] ... for m merge queries.

 */


#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

struct Table {
  int size;
  int parent;
  int rank;
};

/* Unions two trees (sets, tables) using the union by rank heuristic. 
 * If one set is larger than the other, the root node of the smaller set
 * is "hung," set to point to it. If they are the same size, one is hung under
 * the other and the rank of the root is incremented.
 * Each tree contains one or more nodes of tables, and the sum of the
 * rows in all of the tables in a tree is the total size of the set.

 * @param dest_table - the index of the destination table in the Table array.
 * @param src_table - the index of the source.
 * @param sets - the table array
 */
void merge(int dest_table, int src_table, struct Table *sets);


/* Recursively gets the root node of a tree and does path compression. 
 * Starting from the bottom of the tree, each node in the path
 * to the root from a child node has their parent attribute set
 * to the index of the parent in the Table array. The parent is 
 * used to keep track of the size and rank of the tree (the table) and every
 * element in it.
 
 * @param set_index - the index of the node in the Table array. 
 * @param sets - the Table array.
 * @return - returns the index of the root node in the Table array
 */
int get_parent(int set_index, struct Table *sets);

/* Checks if the newly merged table's size is greater than the current maximum
 */
int max(int new_table_size, int current_maximum_size);

// accessed by main() and merge() to set the maximum table size
int MAX_TBLSIZE = INT_MIN;

int main() {

  int n, m;

  scanf("%d", &n); // number of tables.
  scanf("%d", &m); // number of merge requests to perform

  // initialize n tables
  struct Table* sets;
  sets = (struct Table*) malloc(n * sizeof(struct Table));
  
  // get all of the table sizes
  for(int i = 0; i < n; ++i) {
    sets[i].parent = i;

    int this_tablesize;
    scanf("%d", &this_tablesize);
    sets[i].size = this_tablesize;
    sets[i].rank = 0;

    MAX_TBLSIZE = max(this_tablesize, MAX_TBLSIZE);
  }

  // get all of the merge requests and merge the tables
  for(int i = 0; i < m; ++i) {
    int dest_table, src_table;
    scanf("%d %d", &dest_table, &src_table);

    // to account for the first table index being "1" in the "sets" array
    dest_table--; src_table--;
    
    merge(dest_table, src_table, sets);
    printf("%d\n", MAX_TBLSIZE);
  }

  return 0;
}


// merges (unions) two tables (sets)
void merge(int dest_table, int src_table, struct Table *sets) {

  // find the actual parents (which will have the actual sizes
  // attached) of the current destination and source nodes
  int top_dest = get_parent(dest_table, sets);
  int top_src = get_parent(src_table, sets);

  // int value used to track the most recently set size.
  // if that size is greater than the current maximum after the conditional
  // then we set the maximum to it.
  int current_size;

  if (top_dest != top_src) {

    if(sets[top_dest].rank > sets[top_src].rank) {
      sets[top_src].parent = top_dest;

      sets[top_dest].size += sets[top_src].size;
      current_size = sets[top_dest].size;

    }
    else {
      sets[top_dest].parent = top_src;
      if(sets[top_dest].rank == sets[top_src].rank) {
	sets[top_src].rank += 1;
      }

      // since we are keeping the source as the root node,
      // we increment its size. It will be the node that is always
      // queried for the size of itself and any of its children
      sets[top_src].size += sets[top_dest].size;

      current_size = sets[top_src].size;
    }
  }


  // set the maximum size by comparing it with the size we just set
  MAX_TBLSIZE = max(current_size, MAX_TBLSIZE);
    
}



// recursively gets and sets the parent of a node
// and all of its parents to the root node for path compression
int get_parent(int set_index, struct Table *sets) {

  // find parent and compress path
  if(set_index != sets[set_index].parent) {
    sets[set_index].parent = get_parent(sets[set_index].parent, sets);
  }
  return sets[set_index].parent;
}


int max(int new_table_size, int current_maximum_size) {

  return new_table_size > current_maximum_size ? new_table_size : current_maximum_size;
}
