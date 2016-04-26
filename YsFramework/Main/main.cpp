#include <stdio.h>
#include <string.h>
#include <ysshellext.h>
#include <ysclass.h>
#include "mihashtable.h"
#include "clustering.h"
#include "Anchor.h"

enum STLFILETYPE
{
    BINARY,
    ASCII
};

char *fgets_nak(char str[], int maxn, FILE *fp)
{
    if(nullptr != fgets(str,maxn,fp))
    {
        //Remove control code
        int i;
        for (i=0; str[i]!=0; i++)
        {
        }
        for (i--; 0<=i && 0==isprint(str[i]); i--)
        {
            str[i] = 0;
        }
        return str;
    }
    return nullptr;
}

bool IsCPULittleEndian()
{
    unsigned int one=1;
    auto *dat=(const unsigned char *)&one;
    if(1==dat[0])
    {
        return true;
    }
    return false;
}

STLFILETYPE TellSTLFiletype(const char fn[])
{
    FILE *fp=fopen(fn,"rb");
    if(nullptr!=fp)
    {
        char buf[1024];
        fread(buf,1,1024,fp);
        fclose(fp);
        
        bool solid,facet,loop,vertex;
        solid=false;
        facet=false;
        loop=false;
        vertex=false;
        for(int i=0; i<1018; i++)
        {
            if(strncmp(buf+i,"solid",5)==0)
            {
                solid=true;
            }
            else if(strncmp(buf+i,"facet",5)==0)
            {
                facet=true;
            }
            else if(strncmp(buf+i,"loop",4)==0)
            {
                loop=true;
            }
            else if(strncmp(buf+i,"vertex",6)==0)
            {
                vertex=true;
            }
        }
        
        if(true==solid && true==facet && true==loop && true==vertex)
        {
            return ASCII;
        }
    }
    return BINARY;
}

int BinaryToInt(const unsigned char dw[])
{
    int b0 = (int)dw[0];
    int b1 = (int)dw[1];
    int b2 = (int)dw[2];
    int b3 = (int)dw[3];
    return b0+b1*0x100+b2*0x10000+b3*0x1000000;
}

float BinaryToFloat(const unsigned char dw[])
{
    if(true==IsCPULittleEndian())
    {
        const float *fPtr=(const float *)dw;
        return *fPtr;
    }
    else
    {
        float value;
        auto *valuePtr=(unsigned char *)&value;
        valuePtr[0]=dw[3];
        valuePtr[1]=dw[2];
        valuePtr[2]=dw[1];
        valuePtr[3]=dw[0];
        return value;
    }
}

int Parse(int wordTop[], int wordLength[], int maxWords, char str[])
{
    enum
    {
        BLANK,
        VISIBLE
    };
    //state is 0 for blank and 1 for visible
    auto state = BLANK;
    int nw = 0;
    
    //check if character in string is printable or not
    for (int i = 0; str[i]!=0; i++)
    {
        switch (state)
        {
            case BLANK:
                if (' '!=str[i] && isprint(str[i]))
                {
                    if(nw<maxWords)
                    {
                        wordTop[nw] = i;
                        wordLength[nw] = 1;
                        nw++;
                        state = VISIBLE;
                    }
                    else
                    {
                        goto LOOPOUT;
                    }
                }
                break;
            case VISIBLE:
                if (' '!=str[i] && isprint(str[i]))
                {
                    wordLength[nw-1]++;
                }
                else
                {
                    state = BLANK;
                }
                break;
        }
    }
    
    //return number of words
LOOPOUT:
    return nw;
}

void GetWord(char wd[],char str[],int wordTop,int wordLength,int bufSize)
{
    int l=wordLength;
    if(bufSize<l)
    {
        l=bufSize;
    }
    strncpy(wd,str+wordTop,l);
    wd[l]=0;
}


bool ReadBinarySTL(const char fn[], std::vector<float> &vtx, std::vector<float> &nom)
{
    FILE *fp=fopen(fn,"rb");
    if(nullptr!=fp)
    {
        unsigned char title[80];
        fread(title,1,80,fp); // Skip title
        
        unsigned char dw[4];
        fread(dw,4,1,fp);  // Read 4 bytes
        auto nTri=BinaryToInt(dw);
        //         printf("%d triangles\n",nTri);
        
        vtx.clear();
        nom.clear();
        
        //         printf("Clear vertex and polygon\n");
        //         plygn.resize(nTri);
        int nTriActual=0;
        
        for(int i=0; i<nTri; ++i)
        {
            //             printf("Triangle %d\n",nTriActual+1);
            unsigned char buf[50];  // 50 bytes per triangle
            if(50==fread(buf,1,50,fp))
            {
                float nx = BinaryToFloat(buf);
                float ny = BinaryToFloat(buf+4);
                float nz = BinaryToFloat(buf+8);
                for (int i = 0; i<3; i++)
                {
                    float px = BinaryToFloat(buf+12*i+12);
                    float py = BinaryToFloat(buf+12*i+16);
                    float pz = BinaryToFloat(buf+12*i+20);
                    vtx.push_back(px);
                    vtx.push_back(py);
                    vtx.push_back(pz);
                    nom.push_back(nx);
                    nom.push_back(ny);
                    nom.push_back(nz);
                }
                ++nTriActual;
            }
            else
            {
                break;
            }
        }
                 printf("Actually read %d\n",nTriActual);
        
        fclose(fp);
        return true;
    }
    printf("Error! Could not open file for reading!\n");
    return false;
}

bool ReadAsciiSTL(const char fn[], std::vector <float> &vtx, std::vector <float> &nom)
{
    FILE *fp=fopen(fn,"r");
    if(nullptr!=fp)
    {
        int state=0;  // 0:Outside facet  1:Inside facet
        int nVtxPerTri=0;
        int nTri = 0;
        float nx, ny, nz;
        char str[256];
        while(nullptr!=fgets(str,255,fp))
        {
            int wT[256],wL[256];
            auto nw = Parse(wT, wL, 256, str);
            if(1<=nw)
            {
                char wd[256];
                GetWord(wd, str, wT[0], wL[0], 255);
                
                if(0==state)
                {
                    if(0==strcmp(wd,"facet"))
                    {
                        state=1;
                        nVtxPerTri=0;
                        nTri++;
                        GetWord(wd,str,wT[1],wL[1],255);
                        if (0 == strcmp(wd,"normal"))
                        {
                            char xs[256],ys[256],zs[256];
                            GetWord(xs,str,wT[2],wL[2],255);
                            GetWord(ys,str,wT[3],wT[3],255);
                            GetWord(zs,str,wT[4],wL[4],255);
                            nx = atof(xs);
                            ny = atof(ys);
                            nz = atof(zs);
                        }
                    }
                }
                else if(1==state)
                {
                    if(0==strcmp(wd,"endfacet"))
                    {
                        state=0;
                    }
                    if(0==strcmp(wd,"vertex") && 4<=nw && nVtxPerTri<3)
                    {
                        char xs[256],ys[256],zs[256];
                        GetWord(xs,str,wT[1],wL[1],255);
                        GetWord(ys,str,wT[2],wT[2],255);
                        GetWord(zs,str,wT[3],wL[3],255);
                        vtx.push_back(atof(xs));
                        vtx.push_back(atof(ys));
                        vtx.push_back(atof(zs));
                        nom.push_back(nx);
                        nom.push_back(ny);
                        nom.push_back(nz);
                        nVtxPerTri++;
                    }
                }
            }
        }
        
        
        fclose(fp);
        
        //         printf("Found %d triangles.\n",nTri);
        return true;
    }
    printf("Error! Could not open file for reading!\n");
    return false;
}


void WriteBinarySTL(const char fn[], const YsShellExt &shl)
{
    FILE *fp = fopen(fn,"wb");
    if (nullptr != fp)
    {
        char comment[80] = "Binary STL written for shell";
        fwrite(comment,sizeof(char),80,fp);
        // 		std::vector<Triangle> binstl = SetupBinarySTL();
        int size = shl.GetNumPolygon();
        auto *tri = (unsigned char *)(&size);
        fwrite(tri,1,4,fp);
        for (auto &i : shl.AllPolygon())
        {
            auto n = shl.GetNormal(i);
            float nx = (float)n.xf();
            float ny = (float)n.yf();
            float nz = (float)n.zf();
            fwrite(&nx,1,4,fp);
            fwrite(&ny,1,4,fp);
            fwrite(&nz,1,4,fp);
            auto plVt = shl.GetPolygonVertex(i);
            for (int j = 0; j<3; j++)
            {
                auto vtPos = shl.GetVertexPosition(plVt[j]);
                float px = (float)vtPos.xf();
                float py = (float)vtPos.yf();
                float pz = (float)vtPos.zf();
                fwrite(&px,1,4,fp);
                fwrite(&py,1,4,fp);
                fwrite(&pz,1,4,fp);
            }
            char nothing[2] = {'\0','\0'};
            fwrite(nothing,1,2,fp);
        }
        fclose(fp);
    }
    else
    {
        printf("Could Not Open File\n");
    }
}

void WriteAsciiSTL(const char fn[], const YsShellExt &shl)
{
    FILE *fp = fopen(fn,"w");
    if (fp != nullptr)
    {
        fprintf(fp,"solid test\n");
        for (auto plHd : shl.AllPolygon())
        {
            auto plVt = shl.GetPolygonVertex(plHd);
            auto nom = shl.GetNormal(plHd);
            fprintf(fp,"\tfacet normal %e %e %e\n",nom.xf(),nom.yf(),nom.zf());
            fprintf(fp,"\t\touter loop\n");
            for (int j = 0; j<3; j++)
            {
                auto vtPos = shl.GetVertexPosition(plVt[j]);
                fprintf(fp,"\t\t\tvertex %e %e %e\n",vtPos.xf(),vtPos.yf(),vtPos.zf());
            }
            fprintf(fp,"\t\tendloop\n\tendfacet\n");
        }
        fprintf(fp,"endsolid test\n");
        fclose(fp);
    }
    else
    {
        printf("Could Not Open File\n");
    }
}

bool ReadFromSTL(const char fn[], YsShellExt &shl)
{
    bool state;
    std::vector <float> vtx, nom;
    auto filetype = TellSTLFiletype(fn);
    if (BINARY == filetype)
    {
        state = ReadBinarySTL(fn,vtx,nom);
    }
    else
    {
        state = ReadAsciiSTL(fn,vtx,nom);
    }
    MakeShellFromVtxNom(shl, vtx, nom);
    return state;
}

void WriteToSTL(const YsShellExt &shl,const char fn[], STLFILETYPE filetype)
{
    if (filetype == BINARY)
    {
        WriteBinarySTL(fn,shl);
    }
    else
    {
        return WriteAsciiSTL(fn,shl);
    }
    
}

int main()
{
	srand((int)time(NULL));
//    if (argc==3)
//    {
    YsShellExt shl;
    char fn[256],str[256];
    printf("Enter file you want to open : ");
    fgets_nak(fn, 255, stdin);
    bool state = ReadFromSTL(fn,shl);
    if (state == true)
    {
        printf("Enter number of clusters : ");
        fgets_nak(str,255,stdin);
        int k = atoi(str);
        shl.EnableSearch();
        printf("Read STL File\nVertices : %d, Triangles : %d\n",shl.GetNumVertex(),shl.GetNumPolygon());
        printf("Starting clustering\n");
        LloydCluster cluster(shl,k);
        cluster.MakeCluster(shl);
        printf("Finished clustering\n");
        AnchorVertex anchor(k);
        anchor.Initialize(shl, cluster);
        printf("Initialized Anchor\n");
        anchor.MakeAnchorVertex();
        printf("Made Anchor Vertex\n");
        anchor.BinAnchorVertex();
        printf("Created bin for Anchor Vertex\n");
        anchor.AssignLabel();
        printf("Assigned labels\n");

        for(int i=0;i<k;i++)
        {
            printf("Extracted Edges: %d\n",i);
            anchor.ExtractEdges(i);
            
        }

        printf("Extracted edges\n");
        anchor.FindAverageAnchorVertex();
        
        YsShellExt newShell;
        anchor.IndexLabelling(newShell);
        char fout[256];
        strcpy(fout,fn);
        int l = (int)strlen(fout);
        fout[l-4] = 0;
        WriteToSTL(newShell,strcat(fout,"_modified.stl"), ASCII);
    }
    else
    {
        printf("Could not open file.\nCheck file name\n");
        printf("Please keep the stl files in the same directory as the executable\n");
    }

    return 0;
}