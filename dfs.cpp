#include "dfs.h"
#include "dfs.h"

Dfs::Dfs()
{
}

void Dfs::initialize_search()
{
    int i;

    for(i=1; i <= node_index;i++){
        processed[i] = discovered[i] = FALSE;
        parent[i] = -1;
    }

}
