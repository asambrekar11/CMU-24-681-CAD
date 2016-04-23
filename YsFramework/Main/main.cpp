#include <stdio.h>
#include <string.h>
#include <vector_nak.h>
#include <mihashtable.h>
#include <shell.h>
#include <clustering.h>
#include <Anchor.h>

int main(int argc, char *argv[])
{
	srand((int)time(NULL));
    if (argc==3)
    {
		int k = atoi(argv[2]);
		Shell shl;
		shl.ReadFromSTL(argv[1]);
		shl.EnableSearch();
		printf("Read STL File\nVertices : %d, Triangles : %d\n",shl.GetNumVertex(),shl.GetNumPolygon());
		printf("Starting clustering\n");
		LloydCluster cluster(k);
		cluster.MakeCluster(shl);
		printf("Finished clustering\n");
		AnchorVertex anchor(k);
		anchor.Initialize(shl, cluster);
		printf("Initialized Anchor\n");
		anchor.MakeAnchorVertex();
		printf("Made Anchor Vertex\n");
		anchor.BinAnchorVertex();
		printf("Created bon for Anchor Vertex\n");
		anchor.AssignLabel();
		printf("Assigned labels\n");
	
		for(int i=0;i<k;i++)
		{
			anchor.ExtractEdges(i);
		}
	
        printf("Extracted edges\n");
        
        
       	Shell newShell = anchor.IndexLabelling();
	   	newShell.WriteToSTL("modified.stl", BINARY);
    }

    return 0;
}