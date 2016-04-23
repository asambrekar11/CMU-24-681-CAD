#include "shell.h"

////////////////////////////////////////////////////////////////////////////////
//LOCAL FUNCTIONS FOR THIS FILE
////////////////////////////////////////////////////////////////////////////////

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

void GetBoundingBox(Vec3 &min, Vec3 &max, std::vector<float> vtx)
{
	auto nVtx=vtx.size()/3;
	// Cache bounding box
	if(0<nVtx)
	{
		float minx,miny,minz,maxx,maxy,maxz;
		minx=vtx[0];
		miny=vtx[1];
		minz=vtx[2];
		maxx=vtx[0];
		maxy=vtx[1];
		maxz=vtx[2];
		for(decltype(nVtx) i=0; i<nVtx; ++i)
		{
			minx = minx<vtx[i*3  ]?minx:vtx[i*3  ];
			miny = miny<vtx[i*3+1]?miny:vtx[i*3+1];
			minz = minz<vtx[i*3+2]?minz:vtx[i*3+2];
			maxx = maxx>vtx[i*3  ]?maxx:vtx[i*3  ];
			maxy = maxy>vtx[i*3+1]?maxy:vtx[i*3+1];
			maxz = maxz>vtx[i*3+2]?maxz:vtx[i*3+2];
		}
		min.Set(minx,miny,minz);
		max.Set(maxx,maxy,maxz);
	}
	else
	{
		min=Vec3(0.,0.,0.);
		max=Vec3(0.,0.,0.);
	}
}

void PrepareLatticeForConnection(Lattice3d<std::vector <Shell::VertexHandle>> &ltc, std::vector <float> vtx)
{
	Vec3 min,max;
	GetBoundingBox(min,max,vtx);
	// double d = L2Norm(max-min)/100.0;
// 	min = min - min*d; // Make absolutely sure that all vertices are inside.
// 	max = max + max*d;

	auto nVtx=vtx.size()/3;
	ltc.Create(100,100,100);
	ltc.SetDimension(min,max);
}

////////////////////////////////////////////////////////////////////////////////
//TEMPLATE SPECIALIZATION FOR HASHTABLES
////////////////////////////////////////////////////////////////////////////////

template<>
long long int HashTable<Shell::EdgeKey,std::vector<Shell::PolygonHandle>>::HashCode(const Shell::EdgeKey &key) const
{
	return key.edVtKey[0]+key.edVtKey[1];
}

template<>
long long int HashTable<long long int,std::vector<Shell::PolygonHandle>>::HashCode(const long long int &key) const
{
	return key;
}


////////////////////////////////////////////////////////////////////////////////
//POLYGON CLASS
////////////////////////////////////////////////////////////////////////////////

Polygon::~Polygon()
{
	for (auto &vtHd : vtx)
	{
		vtHd = nullptr;
	}
}

void Polygon::AddVertex(Vec3 &newVtx)
{
	vtx.push_back(&newVtx);
}

void Polygon::AddVertex(const Vec3 *newVtx)
{
	printf("Adding pointer to vertex\n");
	vtx.push_back(newVtx);
	printf("Added pointer to vertex\n");
}


const Polygon *Polygon::Add(const int nVtx, const Vec3 newVtx[])
{
	for (int i = 0; i<nVtx; i++)
	{
		vtx.push_back(&newVtx[i]);
	}
	return this;
}

const Polygon *Polygon::Add(const std::vector<Vec3> &newVtx)
{
	for (auto &vt : newVtx)
	{
		vtx.push_back(&vt);
	}
	return this;
}

const Polygon *Polygon::Add(const int nVtx, const Vec3 *newVtx[])
{
	for (int i = 0; i<nVtx; i++)
	{
		vtx.push_back(newVtx[i]);
	}
	return this;
}
// const Polygon *Polygon::Add(std::vector<const Vec3 *> newVtx)
// {
// 	for (auto &vt : newVtx)
// 	{
// 		vtx.push_back(vt);
// 	}
// 	return this;
// }

void Polygon::SetNormal(const Vec3 &normal)
{
	this->normal = normal;
}

void Polygon::SetColor(const MIColor &col)
{
	color = col;
}

const Vec3 Polygon::GetNormal() const
{
	return normal;
}

const std::vector <const Vec3 *> Polygon::GetVertex() const
{
	return vtx;
}

const MIColor Polygon::GetColor(const Polygon * plHd) const
{
	return plHd->color;
}

Vec3 Polygon::PolygonArea() const
{
    auto BC = GetBaryCenter();
    Vec3 Area(0.,0.,0.);
    for (int i = 0; i<vtx.size(); i++)
    {
        auto v1 = *vtx[i];
        auto v2 = i==vtx.size()-1?*vtx[0]:*vtx[i+1];
        Area = Area + cross(v1-BC,v2-BC);
    }
    return Area;
}

Vec3 Polygon::GetBaryCenter() const
{
    Vec3 BC(0.,0.,0.);
    for (auto vtHd : vtx)
    {
        BC = BC + *vtHd;
    }
    BC = BC / vtx.size();
    return BC;
}

////////////////////////////////////////////////////////////////////////////////
//SHELL::EDGEPOLYGONTABLE CLASS
////////////////////////////////////////////////////////////////////////////////

void Shell::EdgePolygonTable::Initialize(const Shell &shl)
{
	Cleanup();
	Resize(shl.GetNumPolygon());
	
	for (auto plHd : shl.AllPolygon())
	{
		auto plvtx = shl.GetPolygonVertex(plHd);
		for (int edIdx = 0; edIdx < plvtx.size(); edIdx++)
		{
			Shell::EdgeKey key;
			key.edVtKey[0] = shl.GetSearchKey(plvtx[edIdx]);
			key.edVtKey[1] = shl.GetSearchKey(plvtx[edIdx==(plvtx.size()-1)?0:edIdx+1]);
			auto curr_poly = (*this)[key];
			if (nullptr != curr_poly)
			{
				curr_poly->push_back(plHd);
			}
			else
			{
				std::vector<Shell::PolygonHandle> poly;
				poly.push_back(plHd);
				this->Update(key,poly);
			}
		}
	}
	
}

const std::vector<Shell::PolygonHandle> *Shell::EdgePolygonTable::FindPolygon(Shell::VertexHandle vtHd0, Shell::VertexHandle vtHd1) const
{
	Shell::EdgeKey key;
	key.edVtKey[0] = (long long int)vtHd0;
	key.edVtKey[1] = (long long int)vtHd1;
	return (*this)[key];
}

////////////////////////////////////////////////////////////////////////////////
//SHELL::VERTEXPOLYGONTABLE CLASS
////////////////////////////////////////////////////////////////////////////////

void Shell::VertexPolygonTable::Initialize(const Shell &shl)
{
// 	this->shl = shl;
	Cleanup();
	Resize(shl.GetNumPolygon());
	int i = 0;
	for (auto plHd : shl.AllPolygon())
	{
// 		auto plVtx = shl.GetPolygonVertex(plHd);
// 		printf("Number of vertex in polygon = %d\n", plVtx.size());
		for (auto vtHd : shl.GetPolygonVertex(plHd))
		{
			auto key = shl.GetSearchKey(vtHd);
// 			printf("Key is %lld\n",key);
			auto plVtptr=(*this)[key];

			if (plVtptr != nullptr)
			{
// 				printf("I already have some one!\n");
				plVtptr->push_back(plHd);
			}
			else
			{
// 				printf("%d\n",i);
				i++;
				std::vector <Shell::PolygonHandle> vtPl;
				vtPl.push_back(plHd);
// 				printf("Updating key %lld\n",key);
				this->Update(key,vtPl);
			}
		}
	}
	printf("VtxPoly size = %d\n",size());
}

const std::vector <Shell::PolygonHandle> *Shell::VertexPolygonTable::FindPolygon(Shell::VertexHandle vtHd) const
{
	auto key = (long long int)vtHd;
	return (*this)[key];
}

////////////////////////////////////////////////////////////////////////////////
//SHELL CLASS
////////////////////////////////////////////////////////////////////////////////

bool Shell::ReadBinarySTL(const char fn[],std::vector <float> &vtx, std::vector <float> &nom)
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
        
        //plygn.clear();
        //vtx.clear();
        
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
//         printf("Actually read %d\n",nTriActual);
	
		fclose(fp);
		return true;
    }
    printf("Error! Could not open file for reading!\n");
    return false;
}

bool Shell::ReadAsciiSTL(const char fn[], std::vector <float> &vtx, std::vector <float> &nom)
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

void Shell::MakeShellFromVtxNom(const std::vector <float> &vtx, const std::vector <float> &nom)
{
	
// 	Lattice3d <std::vector <Shell::VertexHandle> > ltc;
// 	PrepareLatticeForConnection(ltc,vtx);

	Cleanup();
	int nVtx = 0;
	for(int i=0; i<vtx.size()/9; ++i)
	{
		const Vec3 vtNom(nom[i*9],nom[i*9+1],nom[i*9+2]);
		printf("Point 1: %lf %lf %lf\nPoint 2: %lf %lf %lf\n Point 3: %lf %lf %lf\n",
					vtx[i*9  ],vtx[i*9+1],vtx[i*9+2],
					vtx[i*9+3],vtx[i*9+4],vtx[i*9+5],
					vtx[i*9+6],vtx[i*9+7],vtx[i*9+8]);
		const Vec3 vtPos[3]=
		{
			Vec3(vtx[i*9  ],vtx[i*9+1],vtx[i*9+2]),
			Vec3(vtx[i*9+3],vtx[i*9+4],vtx[i*9+5]),
			Vec3(vtx[i*9+6],vtx[i*9+7],vtx[i*9+8]),
		};
		Shell::VertexHandle vtHd[3] = {nullptr, nullptr, nullptr};
		for(int i=0; i<3; ++i)
		{
// 			if (nullptr != vtHd[(i+2)%3])
// 			{
// 				Vec3 HdPos = GetVertexPosition(vtHd[(i+2)%3]);
// 				printf("%d HdPos = (%lf, %lf, %lf)\n",(i+2)%3,HdPos.xf(),HdPos.yf(),HdPos.zf());
// 			}
			vtHd[i]=nullptr;
			for (int j = 0; j<nVtx; j++)
			{
				if (this->vtx[j] == vtPos[i])
				{
					vtHd[i] = &(this->vtx[j]);
					break;
				}
			}
// 			auto idx=ltc.GetBlockIndex(vtPos[i]);
// 			if(true==ltc.IsInRange(idx))
// 			{
// 				for(auto tstVtHd : ltc.Elem(idx.x(),idx.y(),idx.z()))
// 				{
// 					auto tstpos = GetVertexPosition(tstVtHd);
// // 					printf("tstVtHd = (%lf, %lf, %lf), vtPos = (%lf, %lf, %lf)\n",tstpos.xf(),tstpos.yf(),tstpos.zf(),
// // 																				vtPos[i].xf(),vtPos[i].yf(), vtPos[i].zf());
// 					if(GetVertexPosition(tstVtHd)==vtPos[i])
// 					{
// 						vtHd[i]=tstVtHd;
// 						break;
// 					}
// 				}
// 			}
// 			else 
// 			{
// 				printf("Out of range\n");
// 			}
			if(nullptr==vtHd[i])
			{
// 				printf("Added vertex %d\nvtPos = (%lf, %lf, %lf)\n",nVtx,vtPos[i].xf(),vtPos[i].yf(),vtPos[i].zf());
				vtHd[i]=AddVertex(vtPos[i]); nVtx++;
				Vec3 HdPos = GetVertexPosition(vtHd[i]);
// 				printf("HdPos = (%lf, %lf, %lf)\n",HdPos.xf(),HdPos.yf(),HdPos.zf());
// 				if(true==ltc.IsInRange(idx))
// 				{
// 					ltc.Elem(idx.x(),idx.y(),idx.z()).push_back(vtHd[i]);
// 				}
			}
		}
		Vec3 plyvtx[3] = {	GetVertexPosition(vtHd[0]),
							GetVertexPosition(vtHd[1]),
							GetVertexPosition(vtHd[2])};
		printf("Vertex to be added:\n%lld\t(%lf %lf %lf)\n%lld\t(%lf %lf %lf)\n%lld\t(%lf %lf %lf)\n",
					vtHd[0],plyvtx[0].xf(), plyvtx[0].yf(), plyvtx[0].zf(),
					vtHd[1],plyvtx[1].xf(), plyvtx[1].yf(), plyvtx[1].zf(),
					vtHd[2],plyvtx[2].xf(), plyvtx[2].yf(), plyvtx[2].zf()	);
		Shell::PolygonHandle plHd;
		plHd = AddPolygon(3,vtHd);
		auto plVtHd = GetPolygonVertex(plHd);
		Vec3 checkvtx[3] = {	GetVertexPosition(plVtHd[0]),
								GetVertexPosition(plVtHd[1]),
								GetVertexPosition(plVtHd[2])};
		printf("Vertex added:\n%lld\t(%lf %lf %lf)\n%lld\t(%lf %lf %lf)\n%lld\t(%lf %lf %lf)\n",
					plVtHd[0],checkvtx[0].xf(), checkvtx[0].yf(), checkvtx[0].zf(),
					plVtHd[1],checkvtx[1].xf(), checkvtx[1].yf(), checkvtx[1].zf(),
					plVtHd[2],checkvtx[2].xf(), checkvtx[2].yf(), checkvtx[2].zf()	);
		
		SetPolygonNormal(plHd,vtNom);
	}
        
}


bool Shell::WriteBinarySTL(const char fn[]) const
{
    FILE *fp = fopen(fn,"wb");
    if (nullptr != fp)
    {
        char comment[80] = "Binary STL written for shell";
        fwrite(comment,sizeof(char),80,fp);
        // 		std::vector<Triangle> binstl = SetupBinarySTL();
        int size = (int)plygn.size();
        auto *tri = (unsigned char *)(&size);
        fwrite(tri,1,4,fp);
        for (auto &i : plygn)
        {
            auto n = i.GetNormal();
            float nx = (float)n.xf();
            float ny = (float)n.yf();
            float nz = (float)n.zf();
            fwrite(&nx,1,4,fp);
            fwrite(&ny,1,4,fp);
            fwrite(&nz,1,4,fp);
            auto plVt = i.GetVertex();
            for (int j = 0; j<3; j++)
            {
                
                float px = (float)plVt[j]->xf();
                float py = (float)plVt[j]->yf();
                float pz = (float)plVt[j]->zf();
                fwrite(&px,1,4,fp);
                fwrite(&py,1,4,fp);
                fwrite(&pz,1,4,fp);
            }
            char nothing[2] = {'\0','\0'};
            fwrite(nothing,1,2,fp);
        }
        fclose(fp);
        return true;
    }
    printf("Could Not Open File\n");
    return false;
}

bool Shell::WriteAsciiSTL(const char fn[]) const
{
    FILE *fp = fopen(fn,"w");
    if (fp != nullptr)
    {
        fprintf(fp,"solid test\n");
        for (auto plHd : plygn)
        {
            auto plVt = plHd.GetVertex();
            auto nom = plHd.GetNormal();
            fprintf(fp,"\tfacet normal %e %e %e\n",nom.xf(),nom.yf(),nom.zf());
            fprintf(fp,"\t\touter loop\n");
            for (int j = 0; j<3; j++)
            {
                fprintf(fp,"\t\t\tvertex %e %e %e\n",plVt[j]->xf(),plVt[j]->yf(),plVt[j]->zf());
            }
            fprintf(fp,"\t\tendloop\n\tendfacet\n");
        }
        fprintf(fp,"endsolid test\n");
        fclose(fp);
        return true;
    }
    printf("Could Not Open File\n");
    return false;
}


long long int Shell::GetSearchKey(VertexHandle vtHd) const
{
	return (long long int)vtHd;
}

long long int Shell::GetSearchKey(PolygonHandle plHd) const
{
	return (long long int)plHd;
}

Vec3 Shell::GetNormal(Shell::PolygonHandle plHd) const
{
	return plHd->GetNormal();
}

Vec3 Shell::GetPolygonArea(Shell::PolygonHandle plHd) const
{
	return plHd->PolygonArea();
}

Vec3 Shell::GetBaryCenter(Shell::PolygonHandle plHd) const
{
    return plHd->GetBaryCenter();
}
	
Shell::PolygonHandle Shell::FindNextPolygon(Shell::PolygonHandle plHd) const
{
	if (plHd == nullptr)
	{
		return &plygn[0];
	}
	for (int i = 0; i<plygn.size()-1; i++)
	{
		if (&plygn[i] == plHd)
		{
			return &plygn[i+1];
		}
	}
	return nullptr;
}

Shell::PolygonHandle Shell::FindPrevPolygon(Shell::PolygonHandle plHd) const
{
	if (plHd == nullptr)
	{
		return &plygn.back();
	}
	for (int i = 1; i<plygn.size(); i++)
	{
		if (&plygn[i] == plHd)
		{
			return &plygn[i-1];
		}
	}
	return nullptr;
}

Shell::VertexHandle Shell::FindNextVertex(Shell::VertexHandle vtHd) const
{
	if (vtHd == nullptr)
	{
		return &vtx[0];
	}
	for (int i = 0; i<vtx.size()-1; i++)
	{
		if (&vtx[i] == vtHd)
		{
			return &vtx[i+1];
		}
	}
	return nullptr;
}

Shell::VertexHandle Shell::FindPrevVertex(Shell::VertexHandle vtHd) const
{
	if (vtHd == nullptr)
	{
		return &vtx.back();
	}
	for (int i = 1; i<vtx.size(); i++)
	{
		if (&vtx[i] == vtHd)
		{
			return &vtx[i+1];
		}
	}
	return nullptr;
}

Shell::VertexHandle Shell::AddVertex(const Vec3 incoming)
{
	vtx.push_back(incoming);
	return &vtx.back();
}

Shell::PolygonHandle Shell::AddPolygon(const int nVtx, Shell::VertexHandle incoming[])
{
	Polygon poly;
	poly.Add(nVtx,incoming);
	plygn.push_back(poly);
	return &plygn.back();
}
// 
// Shell::PolygonHandle Shell::AddPolygon(std::vector<Shell::VertexHandle> incoming)
// {
// 	Polygon poly;
// 	poly.Add(incoming);
// 	plygn.push_back(poly);
// 	return &plygn.back();
// }

std::vector <Shell::VertexHandle> Shell::GetPolygonVertex(Shell::PolygonHandle plHd) const
{
	return plHd->GetVertex();
}

const Vec3 Shell::GetVertexPosition(Shell::VertexHandle vtHd) const
{
	return *vtHd;
// 	Vec3 result(0.,0.,0.);
// 	for (auto &v : vtx)
// 	{
// 		if (vtHd == &v)
// 		{
// 			result = v;
// 		}
// 	}
// 	return result;
}

void Shell::SetPolygonColor(Shell::PolygonHandle plHd, const MIColor col)
{
	auto edit_plHd = (Polygon *)plHd;
	edit_plHd->SetColor(col);
}

void Shell::SetPolygonColor(Shell::PolygonHandle plHd, const float col[])
{
	auto edit_plHd = (Polygon *)plHd;
	edit_plHd->SetColor(MIColor(col));
}

void Shell::EnableSearch()
{
	EdgeToPolygon.Initialize(*this);
	VertexToPolygon.Initialize(*this);
}

Shell::PolygonHandle Shell::GetNeighbour(Shell::PolygonHandle plHd, Shell::VertexHandle vtHd) const
{
	auto plvtx = GetPolygonVertex(plHd);
    int idx = 0;
	for (int i = 0; i<plvtx.size(); i++)
	{
		if (plvtx[i] == vtHd)
		{
			idx = i;
			break;
		}
	}
	int nextidx = idx==(plvtx.size()-1)?0:idx+1;
	auto plygrp = Shell::EdgeToPolygon.FindPolygon(plvtx[idx],plvtx[nextidx]);
	for (auto pl : *plygrp)
	{
		if (pl != plHd)
		{
			return pl;
		}
	}
	return nullptr;
}

Shell::PolygonHandle Shell::PickRandomPolygon() const
{
	return &plygn[rand()%plygn.size()];
}

bool Shell::ReadFromSTL(const char fn[])
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
    MakeShellFromVtxNom(vtx,nom);
	return state;
}

bool Shell::WriteToSTL(const char fn[], STLFILETYPE filetype) const
{
    if (filetype == BINARY)
    {
        return WriteBinarySTL(fn);
    }
	return WriteAsciiSTL(fn);
	
}