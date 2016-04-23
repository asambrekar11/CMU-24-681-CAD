#include <stdio.h>
#include <string.h>
#include <vector_nak.h>
#include <mihashtable.h>
#include <shell.h>
#include <clustering.h>
#include <Anchor.h>

int main(int argc, char *argv[])
{

    if (argc==3)
    {
        int k = atoi(argv[2]);
        Shell shl;
//        shl.ReadFromSTL(argv[1]);
        shl.EnableSearch();
        LloydCluster cluster(k);
        cluster.MakeCluster(shl);
        AnchorVertex anchor(k);
        anchor.Initialize(shl, cluster);
        anchor.MakeAnchorVertex();
        anchor.BinAnchorVertex();
        anchor.AssignLabel();
        
        for(int i=0;i<k;i++)
        {
            anchor.ExtractEdges(i);
        }
        
        
        
        
//        Shell newShell = anchor.IndexLabelling();
//        newShell.WriteToSTL("modified.stl", BINARY);
    }

    return 0;
}