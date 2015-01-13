#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//Chained list for white points
typedef struct white_point_struct{
  int i;
  int j;
  struct white_point_struct *next;
}t_points;


void OB();
void WB();
void WB_GWA();
void color_interpolation();
void color_interpolation_Laroche_Prescott();
void colormatrix();
void gamma_conversion(double);
void color();
void rgb2bmp(char *);
void edge_enhance();


void get_offsets(int *i, int *j, int region);
void order(float distance[], int regions[]);
float get_mean(t_points *list, char c);
t_points* select_points(t_points* list, float ratio);
t_points* link_tab( t_points* tab[]);
int list_count(t_points *list);
t_points* class(int a, int b, t_points *list);
int sign(int a); 

int pic[2460][3360];
int edges[2460][3360];

int r[2460][3360];
int g[2460][3360];
int b[2460][3360];
int e[2460][3360];

int y[2460][3360];
int cb[2460][3360];
int cr[2460][3360];


unsigned short int gamma_value[1024]={
	0,1,3,5,8,9,11,12,13,14,16,17,18,20,21,22,24,25,26,28,
		29,30,32,33,34,36,37,38,39,41,42,43,45,46,47,49,50,51,53,54,
		55,57,58,59,61,62,63,64,66,67,68,70,71,72,74,75,76,78,79,80,
		82,83,84,85,87,87,88,89,89,90,91,91,92,93,93,94,95,95,96,97,
		97,98,99,99,100,101,101,102,103,103,104,105,105,106,107,107,108,108,109,110,
		110,111,112,112,113,114,114,115,116,116,117,118,118,119,120,120,121,122,122,123,
		124,124,125,126,126,127,128,128,129,129,130,130,130,130,131,131,131,132,132,132,
		133,133,133,134,134,134,135,135,135,136,136,136,137,137,137,138,138,138,139,139,
		139,140,140,140,141,141,141,142,142,142,143,143,143,144,144,144,145,145,145,146,
		146,146,147,147,147,148,148,148,149,149,149,150,150,150,151,151,151,152,152,152,
		153,153,153,154,154,154,154,155,155,155,156,156,156,157,157,157,158,158,158,159,
		159,159,160,160,160,161,161,161,162,162,162,163,163,163,164,164,164,165,165,165,
		166,166,166,167,167,167,168,168,168,169,169,169,170,170,170,171,171,171,171,171,
		172,172,172,172,172,172,173,173,173,173,173,173,174,174,174,174,174,174,175,175,
		175,175,175,175,176,176,176,176,176,176,176,177,177,177,177,177,177,178,178,178, 
		178,178,178,179,179,179,179,179,179,180,180,180,180,180,180,181,181,181,181,181,
		181,182,182,182,182,182,182,183,183,183,183,183,183,184,184,184,184,184,184,185,
		185,185,185,185,185,186,186,186,186,186,186,187,187,187,187,187,187,188,188,188,
		188,188,188,189,189,189,189,189,189,189,190,190,190,190,190,190,191,191,191,191,
		191,191,192,192,192,192,192,192,193,193,193,193,193,193,194,194,194,194,194,194,
		195,195,195,195,195,195,196,196,196,196,196,196,197,197,197,197,197,197,198,198,
		198,198,198,198,199,199,199,199,199,199,200,200,200,200,200,200,201,201,201,201,
		201,201,201,202,202,202,202,202,202,203,203,203,203,203,203,204,204,204,204,204,
		204,205,205,205,205,205,205,206,206,206,206,206,206,207,207,207,207,207,207,208,
		208,208,208,208,208,209,209,209,209,209,209,210,210,210,210,210,210,211,211,211,
		211,211,211,212,212,212,212,212,212,213,213,213,213,213,213,213,213,213,213,214,
		214,214,214,214,214,214,214,214,214,214,214,215,215,215,215,215,215,215,215,215,
		215,215,215,216,216,216,216,216,216,216,216,216,216,216,216,217,217,217,217,217,
		217,217,217,217,217,217,217,217,218,218,218,218,218,218,218,218,218,218,218,218,
		219,219,219,219,219,219,219,219,219,219,219,219,220,220,220,220,220,220,220,220,
		220,220,220,220,221,221,221,221,221,221,221,221,221,221,221,221,222,222,222,222,
		222,222,222,222,222,222,222,222,223,223,223,223,223,223,223,223,223,223,223,223,
		223,224,224,224,224,224,224,224,224,224,224,224,224,225,225,225,225,225,225,225,
		225,225,225,225,225,226,226,226,226,226,226,226,226,226,226,226,226,227,227,227,
		227,227,227,227,227,227,227,227,227,228,228,228,228,228,228,228,228,228,228,228,
		228,229,229,229,229,229,229,229,229,229,229,229,229,229,230,230,230,230,230,230,
		230,230,230,230,230,230,231,231,231,231,231,231,231,231,231,231,231,231,232,232,
		232,232,232,232,232,232,232,232,232,232,233,233,233,233,233,233,233,233,233,233,
		233,233,234,234,234,234,234,234,234,234,234,234,234,234,235,235,235,235,235,235,
		235,235,235,235,235,235,235,236,236,236,236,236,236,236,236,236,236,236,236,237,
		237,237,237,237,237,237,237,237,237,237,237,238,238,238,238,238,238,238,238,238,
		238,238,238,239,239,239,239,239,239,239,239,239,239,239,239,240,240,240,240,240,
		240,240,240,240,240,240,240,241,241,241,241,241,241,241,241,241,241,241,241,241,
		242,242,242,242,242,242,242,242,242,242,242,242,243,243,243,243,243,243,243,243,
		243,243,243,243,244,244,244,244,244,244,244,244,244,244,244,244,245,245,245,245,
		245,245,245,245,245,245,245,245,246,246,246,246,246,246,246,246,246,246,246,246,
		247,247,247,247,247,247,247,247,247,247,247,247,247,248,248,248,248,248,248,248,
		248,248,248,248,248,249,249,249,249,249,249,249,249,249,249,249,249,250,250,250,
		250,250,250,250,250,250,250,250,250,251,251,251,251,251,251,251,251,251,251,251,
		251,252,252,252,252,252,252,252,252,252,252,252,252,253,253,253,253,253,253,253,
		253,253,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,255,255,255,
		255,255,255,255
};



int main(int argc,char *argv[])
{
  int i,j,k;
	for(k=1;k<argc;k++)
	{
		printf("starting %s\n", argv[k]);
		FILE *fp;
		fp=fopen(argv[k],"rb");	//
		fseek(fp,862144,SEEK_SET);
		for(i=0;i<2460;i++)
			for(j=0;j<3360;j++)
			{
				pic[i][j]=0;
				fread(pic[i]+j,1,1,fp);
				pic[i][j]<<=8;
				fread(pic[i]+j,1,1,fp);
			}
		fclose(fp);	
		OB();
		edge_enhance();
		color_interpolation();
		WB();
		colormatrix();
		gamma_conversion(1.06);
		rgb2bmp(argv[k]);
	
	
	
	}

	return 0;
}


//*****
//*****
//*****
//*****
//*****
//*****RERERERERE
//*****GBGBGBGBGB
//*****
//*****
//*****
//*****



void OB()
{
	unsigned long int ob_sum = 0;
	int ob_avg = 0;
	int i, j;
	int E_OB_pixels = 34440, R_OB_pixels = 33210, B_OB_pixels = 34440, G_OB_pixels = 33102;
	
	
	//OB for E region
	ob_sum = 0;
	for(i = 0; i <2460 ; i = i+2)
	{
		for(j=0 ; j<5; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
		}
		for(j = 3294 ; j<3343; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
		}
	}
	ob_avg = ob_sum / E_OB_pixels;
	for(i = 6; i <2452 ; i = i+2)
		for(j = 6 ; j<3293; j = j+2)
		{
			if(pic[i][j] > ob_avg)
				pic[i][j] = pic[i][j] - ob_avg;
			else
				pic[i][j]=0;
		}
	
	
	//OB for R region
	ob_sum = 0;
	for(i = 0; i <2460 ; i = i+2)
	{
		for(j = 1 ; j<5; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
		}
		for(j = 3293 ; j<3343; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
		}
	}
	ob_avg = ob_sum / R_OB_pixels;
	for(i = 6; i <2453 ; i = i+2)
		for(j = 5 ; j<3293; j = j+2)
		{
			if(pic[i][j] > ob_avg)
				pic[i][j] = pic[i][j] - ob_avg;
			else
				pic[i][j]=0;
		}

		
	//OB for B region
	ob_sum = 0;
	for(i = 1; i <2460 ; i = i+2)
	{
		for(j = 0 ; j<5; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
		}
		for(j = 3294 ; j<3343; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
		}
	}
	ob_avg = ob_sum / B_OB_pixels;
	for(i = 5; i <2453 ; i = i+2)
		for(j = 6 ; j<3293; j = j+2)
		{
			if(pic[i][j] > ob_avg)
				pic[i][j] = pic[i][j] - ob_avg;
			else
				pic[i][j]=0;
		}


	//OB for G region
	ob_sum = 0;
	for(i = 1; i <2453 ; i = i+2)
	{
		for(j = 1 ; j<5; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
		}
		for(j = 3293 ; j<3343; j = j+2)
		{
			ob_sum = ob_sum + pic[i][j];
		}
	}
	ob_avg = ob_sum / G_OB_pixels;
	for(i = 7; i <2453 ; i = i+2)
	{
		for(j = 5 ; j<3293; j = j+2)
		{
			if(pic[i][j] > ob_avg)
				pic[i][j] = pic[i][j] - ob_avg;
			else
				pic[i][j]=0;
		}
	}
	
	printf("Optical Black ---OK!\n");
}

void WB()
{
  int i, j, k, l, m;
  int mean_cb[12], mean_cr[12], db[12], dr[12], regions[12];
  long long sum_cb = 0, sum_cr = 0, sum_db = 0, sum_dr = 0;
  int nb_pixels = 8049024;
  int lines_per_regions = 750 ; // 2250/3
  int columns_per_regions = 822; //3288/4
  int pixels_per_region = 616500 ; // 12 regions
  int count=0;
  int max, max_region;
  t_points *tab[20000];
  t_points *list = NULL;
  float mean_r, mean_b, mean_g;
  long long unsigned moy = 0;
  
  for(i=0; i<20000; i++)
    tab[i]=NULL;
  
  //converting to YCbCr
  for(i=6; i<2452; i++)
    for(j=5; j<3293; j++)
    {
     y[i][j]  =  round(0.299*(float)r[i][j] + 0.587*(float)g[i][j] + 0.114*(float)b[i][j]);
     if(y[i][j] >= 20000)
       y[i][j] = 19999;
     cb[i][j] = round(-0.147*(float)r[i][j] - 0.289*(float)g[i][j] + 0.436*(float)b[i][j]);
     cr[i][j] =  round(0.615*(float)r[i][j] - 0.515*(float)g[i][j] - 0.100*(float)b[i][j]);
    }
 
   //calculating means
  for(k=0; k<12; k++)
  {
    get_offsets(&i, &j, k);
    sum_cb = 0;
    sum_cr = 0;
    for(l=i; l<i+lines_per_regions; l++)
      for(m=j; m<j+columns_per_regions; m++)
      {
      sum_cb += cb[l][m];
      sum_cr += cr[l][m];
      }
    mean_cb[k] = round((long double)sum_cb / pixels_per_region);
    mean_cr[k] = round((long double)sum_cr / pixels_per_region); 
  }

  //calculating Db = ∑( Cb (i , j ) − M b ) / N for each region k
  for(k=0; k<12; k++)
  {
    get_offsets(&i, &j, k);
    sum_db = 0;
    sum_dr = 0;
    for(l=i; l<i+lines_per_regions; l++)
    {
      for(m=j; m<j+columns_per_regions; m++)
      {
	sum_db += abs(cb[l][m] - mean_cb[k]);
	sum_dr += abs(cr[l][m] - mean_cr[k]);
      }
    }
    db[k] = (long double)sum_db / pixels_per_region;
    dr[k] = (long double)sum_dr / pixels_per_region;
  }
    
  //Selecting the regions with high Db and Dr for color variation
  float distance[12];
  for(i=0; i<12; i++)
    distance[i]=sqrt(pow(db[i],2)+pow(dr[i],2));
  order(distance, regions);
    
  //abs(Cb (i, j) − (Mb + Db × sign(M b )) < 1.5 × Db
  //abs(Cr (i, j ) − (1.5 × Mr + Dr × sign(Mr )) < 1.5 × D r
  //we skip the 3 first regions with lowest Db, Dr
  for(k=3; k<12; k++)
  {
    get_offsets(&i, &j, regions[k]);
    for(l=i; l<i+lines_per_regions; l++)
      for(m=j; m<j+columns_per_regions; m++)
	if( abs(cb[l][m]-(mean_cb[regions[k]]+db[regions[k]]*sign(mean_cb[regions[k]]))) < 1.5*db[regions[k]] )
	  if( abs(cr[l][m]-(1.5*mean_cr[regions[k]]+dr[regions[k]]*sign(mean_cr[regions[k]]))) < 1.5*dr[regions[k]] )
	   tab[y[l][m]]=class(l,m,tab[y[l][m]]);	  
  }
  
  //Select the top 10% brightest pixels
  list = link_tab(tab);
  list = select_points(list, 0.1);
  
  //Apply Von Kries model
  //Computing max y and mean y
  for(i=6; i<2454; i++)
    for(j=5; j<3293; j++)
    {
      if(max<y[i][j])
	max=y[i][j];
      moy += y[i][j];
    }
    moy = moy/nb_pixels;
      
  mean_r = get_mean(list, 'r');
  mean_g = get_mean(list, 'g');
  mean_b = get_mean(list, 'b');
  
  double coeff=(double)(max+2*moy)/3.0;
  
  for(i=6; i<2452; i++)
    for(j=5; j<3293; j++)
    {
      r[i][j] *= coeff/mean_r;
      g[i][j] *= ((double)(max+3*moy)/4.0)/mean_g;
      b[i][j] *= coeff/mean_b;
    }
  
  //erase data
  list = select_points(list, 0.0);
  printf("White balance ---OK\n");    
}




void color_interpolation()
{
	int i,j;
	
	//R region

	for( i = 0; i <2460 ; i = i+2)
		for( j = 5 ; j<3293; j = j+2)
		{
			r[i][j] = pic[i][j];			
			g[i][j] = ( pic[i-1][j] + pic[i+1][j] )/2;
			b[i][j] = ( pic[i-1][j-1] + pic[i-1][j+1] + pic[i+1][j-1] + pic[i+1][j+1] )/4;
			

		}

	//G region

	for( i = 1; i <2460 ; i = i+2)
		for( j = 5 ; j<3293; j = j+2)
        {
			g[i][j] = pic[i][j];			
			r[i][j] = ( pic[i-1][j] + pic[i+1][j] )/2;
			b[i][j] = ( pic[i][j-1] + pic[i][j+1] )/2;
		}
		
	//E region

	for( i = 0; i <2460 ; i = i+2)
		for( j = 6 ; j<3293; j = j+2)
		{
		
				b[i][j] = ( pic[i-1][j] + pic[i+1][j] )/2;
				g[i][j] = ( pic[i-1][j-1] + pic[i-1][j+1] + pic[i+1][j-1] + pic[i+1][j+1] )/4;
				r[i][j] = ( pic[i][j-1] + pic[i][j+1] )/2;

		}

	//B region

	for( i = 1; i <2460 ; i = i+2)
		for( j = 6 ; j<3293; j = j+2)
		{
			b[i][j] = pic[i][j];			
			r[i][j] = ( pic[i-1][j-1] + pic[i-1][j+1] + pic[i+1][j-1] + pic[i+1][j+1] )/4;
			g[i][j] = ( pic[i][j-1] + pic[i][j+1] )/2;
		}
	

	printf("Color interpolation ---OK!\n");

}


void colormatrix()
{
 //
 // [R']=[ 1+sa+sb   -sa       -sb     ][R]
 // [G']=[ -sc       1+sc+sd   -sd     ][G]
 // [B']=[ -se       -sf       1+se+sf ][B]  
 // 	 
	unsigned short int i,j;
	double temp_r,temp_g,temp_b;
	//double sa=0.265625,sb=0.17;
	//double sc=0.21,sd=0.21;
	//double se=0.19,sf=0.19;
	
	for(i=0;i<2460;i++){
		for(j=0;j<3360;j++){
			temp_r=r[i][j];
			temp_g=g[i][j];
			temp_b=b[i][j];
			
			//r[i][j]=(1.0+sa+sb)*temp_r - (sa)*temp_g - (sb)*temp_b;
			r[i][j]=(double)((1.2665625)*temp_r+(-0.28125)*temp_g+(0.0)*temp_b);//paul
            //r[i][j]=(double)((6.3329)*temp_r+(-2.7900)*temp_g+(1.5002)*temp_b);       
			if (r[i][j]>16360)//1022.5*16
				r[i][j]=16360;
			else if (r[i][j]<0)
				r[i][j]=0;
			
			//g[i][j]=(1.0+sc+sd)*temp_g - (sc)*temp_r   - (sd)*temp_b;
			g[i][j]=(double)((-0.1875)*temp_r+(1.203125)*temp_g+(-0.046875)*temp_b);//paul
            //g[i][j]=(double)((0.1362)*temp_r+(2.6597)*temp_g+(0.0751)*temp_b);
			if (g[i][j]>16360)//1022.5*16
				g[i][j]=16360;
	        else if (g[i][j]<0)
				g[i][j]=0;
			
			//b[i][j]=(1.0+se+sf)*temp_b - (se)*temp_r - (sf)*temp_g ;
			b[i][j]=(double)((-0.078125)*temp_r+(-0.296875)*temp_g+(1.359375)*temp_b);//paul
            //b[i][j]=(double)((1.2656)*temp_r+(-1.5919)*temp_g+(4.7861)*temp_b);
			if (b[i][j]>16360)//1022.5*16
				b[i][j]=16360;
			else if (b[i][j]<0)
				b[i][j]=0;
		}
	}
	printf("colormatrix ---OK!\n");
}


void gamma_conversion(double gamma_cur)
{
    int i,j;
	for(i=0;i<2460;i++)
	{
		for(j=0;j<3360;j++)
		{
			if (pow(r[i][j],gamma_cur)>16383){
				r[i][j]=16383;
			}else{
			    r[i][j]=pow(r[i][j],gamma_cur);
			}

			if (pow(g[i][j],gamma_cur)>16383){
				g[i][j]=16383;
			}else{
				g[i][j]=pow(g[i][j],gamma_cur);
			}

			if (pow(b[i][j],gamma_cur)>16383){
				b[i][j]=16383;
			}else{
				b[i][j]=pow(b[i][j],gamma_cur);
			}


			r[i][j]=gamma_value[r[i][j]/16];//16=2^14/2^10
			g[i][j]=gamma_value[g[i][j]/16];
			b[i][j]=gamma_value[b[i][j]/16];  
		}
	}
	
	printf("gamma ---OK!\n");


}

void rgb2bmp(char *name)
{

	FILE *fp;
	int zero=0;
	char filename[100];
	strcpy(filename,name);
	strcat(filename,".loic.bmp");

	fp=fopen(filename,"wb");

	fwrite(&"BM",1,2,fp);	
	int temp=54+3264*2448*3;
	fwrite(&temp,4,1,fp);
	int bmpcount, bmpi, bmpj;
	
	for(bmpcount=0;bmpcount<4;bmpcount++)//reserved
		fwrite(&zero,1,1,fp);

	temp=54;//start address of datas
	fwrite(&temp,4,1,fp);
	temp=40;//size of [0E-35]
	fwrite(&temp,1,1,fp);
	fwrite(&zero,1,1,fp);
	fwrite(&zero,1,1,fp);
	fwrite(&zero,1,1,fp);
	int width=3264;//width
	fwrite(&width,4,1,fp);
	int height=2448;//height
	fwrite(&height,4,1,fp);
	temp=1;//color planes
	fwrite(&temp,2,1,fp);
	temp=24;//bits/pixel
	fwrite(&temp,2,1,fp);
	fwrite(&zero,4,1,fp);//Compression format
	
	temp=3264*2448*3;//data size->3264*2448*3
	fwrite(&temp,4,1,fp);
	fwrite(&zero,4,1,fp);//width pixels/meter
	fwrite(&zero,4,1,fp);//height pixels/meter
	fwrite(&zero,2,1,fp);//color tables
	fwrite(&zero,1,1,fp);//
	fwrite(&zero,1,1,fp);
	fwrite(&zero,4,1,fp);//color indexes
//body
	for(bmpi=2453;bmpi>=6;bmpi--)
		for(bmpj=17;bmpj<3281;bmpj++)
		{
		    fwrite(&(b[bmpi][bmpj]),1,1,fp);
			fwrite(&(g[bmpi][bmpj]),1,1,fp);
			fwrite(&(r[bmpi][bmpj]),1,1,fp);
		}
	fclose(fp);
	printf("%s output finished.\n",name);
}



void edge_enhance()
{
  int i,j;
  
  for(i = 5; i <2453 ; i++)
    for(j = 5 ; j<3293; j++)
      edges[i][j] = round((-1.0/8.0)*(double)(pic[i-2][j-2] + pic[i-2][j] + pic[i-2][j+2] + pic[i][j-2] + pic[i][j+2] + pic[i+2][j-2] + pic[i+2][j] + pic[i+2][j+2])) + pic[i][j];

  for(i = 5; i <2453 ; i++)
     for(j = 5 ; j<3293; j++)
     {
	pic[i][j] += edges[i][j];
	if(pic[i][j] < 0)
	  pic[i][j] = 0;
     }
  
    printf("edge enhancement ---OK!\n");
}




int sign(int a)
{
  if(a>=0)
    return 1;
  else
    return -1;
}


//Adds a t_point to an ordered list. 
//if list is null, then add the luminance of [a][b] in the list
//if list is not null, then compare the luminance to the values in the list and add it at the correct position (increasing values)
t_points* class(int a, int b, t_points *list)
{ 
  t_points* p_point = malloc(sizeof(t_points));
  
  p_point->i = a;
  p_point->j = b;
  p_point->next = NULL;
  
  if(list == NULL)
     return p_point;
  
  t_points *new_list = list;
  t_points *previous = list;
  int true = 0;
  while( y[a][b] > y[new_list->i][new_list->j] )
  {
    if(new_list->next != NULL)
    {
      previous = new_list;
      new_list = new_list->next;
    }
    else            //add p_point at the end of the list
    {
      new_list->next = p_point;
      return list;
    }
  }
  if(previous == list) //there is only one element in the list, we want to add p_point before it.
  {
    p_point->next = new_list;
    return p_point;
  }
  
  //there are multiple elements in the list and we want to insert p_point between two of them
  previous->next = p_point;
  p_point->next = new_list;  
  return list;
}
 
  
int list_count(t_points *list)
{
  if(list == NULL)
    return 0;
  
  int count = 1;
  
  t_points *new_list = list;
  while(new_list->next != NULL)
  {
    count++;
    new_list = new_list->next;
  }
  return count;
}

//it allows the conversion of a table of lists to a single (long) list.
//linking the last element of tab[i] to the first of tab[i+j] (j can vary !)
t_points* link_tab( t_points* tab[])
{
  int i=0,j;
  t_points *list = NULL;
  t_points *list_beginning = NULL;
  
  //Searching for first element of the list
  while( (tab[i] == NULL) && (i < 16360) )
    i++;

  if(i < 16360)
    list_beginning = tab[i];

  //linking the last element of tab[i] to the first of tab[i+j] (j can vary !)
  while( i < (16360-1) )
  {
    //looking for non null element
    if(tab[i] == NULL)
    {
      i++;
      continue;
    }
    
    //going at end of list
    list = tab[i];
    while(list->next != NULL)
      list = list->next;
    
    //looking for next non nul element
    j = 1;
    while(((i+j) < 16360) && (tab[i+j] == NULL))
      j++;
    
    //linking the two
    if((i+j) < 16360)
      list->next = tab[i+j];
    
    i++;
  }
  
  return list_beginning;
}


//selects ratio (e.g 0.1, it is a percentage) elements of the list, discards the others
t_points* select_points(t_points* list, float ratio)
{
  if(list == NULL)
    return NULL;
  
  int count = list_count(list);
  int i;
  t_points * next = NULL;
  
  
  for(i=0; i<(int)((1.0-ratio)*count);i++){
    next = list->next;
    free(list);
    list=next;
  }
  
  return list;
}


//Computes the mean of a channel (r, g, b, y) for a list of pixels
//use get_mean(... , 'r') or 'b' or 'g'
float get_mean(t_points *list, char c)
{
  int (*channel)[3360];
  long long unsigned mean = 0;
  t_points * new_list = list;
  
  if((list == NULL) || ((c != 'r') && (c != 'b') && (c != 'g') && (c != 'y')))
    return 1;
  
  //Selecting the correct channel
  switch(c)
  {
    case 'r':
      channel = r; 
      break;
    case 'b':
      channel = b;
      break;
    case 'g':
      channel = g;
    case 'y':
      channel = y;
  }
  
  while(new_list != NULL)
  {
    mean += channel[new_list->i][new_list->j];
    new_list = new_list->next;
  }
  
  return (float)mean/list_count(list);   
}


   
//order of a table of 12 elements in increasing order.
//returns a table of index.
void order(float distance[], int regions[])
{
  int i,j;
  float temp;
  int tmp;
  
  for(i=0; i<12; i++)
    regions[i]=i;
  
  for(i=0; i<12; i++)
    for(j=i+1; j<12; j++)
      if(distance[i] > distance[j])
      {
	temp = distance[i];
	distance[i] = distance[j];
	distance[j] = temp;
	tmp = regions[i];
	regions[i] = regions[j];
	regions[j] = tmp;
      }
}

    

//region between 0 and 11
void get_offsets(int *i, int *j, int region)
{
  int lines_per_regions = 750 ; // 2250/3
  int columns_per_regions = 822; //3288/4
  
  *i = (region / 4) * lines_per_regions + 6;
  *j = (region % 4) * columns_per_regions + 5;
}


//LaRoche-Prescott method
void color_interpolation_Laroche_Prescott()
{
  double alpha, beta;
  int i,j;
 
  
  //guessing g value of E region
  for(i = 6; i <2452 ; i = i+2)
    for(j = 6 ; j<3293; j = j+2)
    {
      pic[i][j] = round((float)( pic[i-1][j-1] + pic[i-1][j+1] + pic[i+1][j-1] + pic[i+1][j+1] )/4);
      g[i][j] = pic[i][j];      
    }
  
   //guessing g values of R region
  for(i = 6; i <2453 ; i = i+2)
    for(j = 5 ; j<3293; j = j+2)
    {
      alpha = abs(round((float)(pic[i][j-2]+pic[i][j+2])/2-pic[i][j]));
      beta = abs(round((float)(pic[i-2][j]+pic[i+2][j])/2-pic[i][j]));
      
      if(alpha < beta)
	g[i][j] = round((float)(pic[i][j-1] + pic[i][j+1])/2);
      else if(alpha > beta)
	g[i][j] = round((float)(pic[i-1][j] + pic[i+1][j])/2);
      else
	g[i][j] = round((float)(pic[i-1][j-1] + pic[i-1][j+1] + pic[i+1][j-1] + pic[i+1][j+1])/4);
      r[i][j] = pic[i][j];
    }
    
    //guessing g values of B region
  for(i = 5; i <2453 ; i = i+2)
    for(j = 6 ; j<3293; j = j+2)
    {
      alpha = abs(round((float)(pic[i][j-2]+pic[i][j+2])/2-pic[i][j]));
      beta = abs(round((float)(pic[i-2][j]+pic[i+2][j])/2-pic[i][j]));
      
      if(alpha < beta)
	g[i][j] = round((float)(pic[i][j-1] + pic[i][j+1])/2);
      else if(alpha > beta)
	g[i][j] = round((float)(pic[i-1][j] + pic[i+1][j])/2);
      else
	g[i][j] = round((float)(pic[i-1][j-1] + pic[i-1][j+1] + pic[i+1][j-1] + pic[i+1][j+1])/4);
      b[i][j] = pic[i][j];
    }   

    //interpolation in E region
  for(i = 6; i <2452 ; i = i+2)
    for(j = 6 ; j<3293; j = j+2)
    {
      r[i][j] = round((float)(r[i][j-1]+r[i][j+1]-g[i][j-1]-g[i][j+1])/2 + g[i][j]);
      if(r[i][j] < 0)
	r[i][j] = 0;
      b[i][j] = round((float)(b[i-1][j]+b[i+1][j]-g[i-1][j]-g[i+1][j])/2 + g[i][j]);
      if(b[i][j] < 0)
	b[i][j] = 0;
    }
    
   //interpolation in R region
  for(i = 6; i <2453 ; i = i+2)
    for(j = 5 ; j<3293; j = j+2)
    {
      b[i][j] = round((float)(b[i-1][j-1]+b[i+1][j+1]+b[i-1][j+1]+b[i+1][j-1]-g[i-1][j-1]-g[i+1][j+1]-g[i-1][j+1]-g[i+1][j-1])/4 + g[i][j]);
      if(b[i][j] < 0)
	b[i][j] = 0;
    }
  
  //interpolation in B region
  for(i = 5; i <2453 ; i = i+2)
    for(j = 6 ; j<3293; j = j+2)
    {
       r[i][j] = round((float)(r[i-1][j-1]+r[i+1][j+1]+r[i-1][j+1]+r[i+1][j-1]-g[i-1][j-1]-g[i+1][j+1]-g[i-1][j+1]-g[i+1][j-1])/4 + g[i][j]);
      if(r[i][j] < 0)
	r[i][j] = 0;
    }
    
  //interpolation in G region
  for(i = 7; i <2453 ; i = i+2)
    for(j = 5 ; j<3293; j = j+2)
    {
      g[i][j] = pic[i][j];
      r[i][j] = round((float)(r[i-1][j]+r[i+1][j]-g[i-1][j]-g[i+1][j])/2 + g[i][j]);
      if(r[i][j] < 0)
	r[i][j] = 0;
      b[i][j] = round((float)(b[i][j-1]+b[i][j+1]-g[i][j-1]-g[i][j+1])/2 + g[i][j]); 
      if(b[i][j] < 0)
	b[i][j] = 0;
    }
      
  printf("Color interpolation ---OK!\n");

}

void WB_GWA()
{
  unsigned long long r_sum = 0, g_sum = 0, b_sum=0, e_sum=0;
  double r_mean, g_mean, b_mean,e_mean;
  double nb_pixel_per_channel = 2010612 ;
  double r_coeff, b_coeff;
  
  int i,j;
  
  //R pixels
  for(i = 6; i <2452 ; i = i+2)
    for(j = 5 ; j<3293; j = j+2)
      r_sum += pic[i][j];
      
  //E pixels
  for(i = 6; i <2452 ; i = i+2)
    for(j = 6 ; j<3293; j = j+2)
      e_sum += pic[i][j];

  //G pixels
  for(i = 7; i <2452 ; i = i+2)
    for(j = 5 ; j<3293; j = j+2)
      g_sum += pic[i][j];

  //B pixels
  for(i = 7; i <2452 ; i = i+2)
    for(j = 6 ; j<3293; j = j+2)
      b_sum += pic[i][j];
    
  r_mean = (long double)r_sum / nb_pixel_per_channel;
  e_mean = (long double)e_sum / nb_pixel_per_channel;
  g_mean = (long double)g_sum / nb_pixel_per_channel;
  b_mean = (long double)b_sum / nb_pixel_per_channel;
  
  r_coeff = g_mean / r_mean;
  b_coeff = g_mean / b_mean;
  
  //R pixels normalization with respect to G pixels average
  for(i = 6; i <2452 ; i = i+2)
    for(j = 5 ; j<3293; j = j+2)
    {
      pic[i][j] = round((float)pic[i][j] * r_coeff);
      if(pic[i][j] > 16383)
	pic[i][j] = 16283;
    }
  
  //B pixels normalization with respect to G pixels average
  for(i = 7; i <2452 ; i = i+2)
    for(j = 6 ; j<3293; j = j+2)
    {
      pic[i][j] = round((float)pic[i][j] * b_coeff);
      if(pic[i][j] > 16383)
	pic[i][j] = 16283;
    }
    
    printf("White balance GWA --OK!\n");
}