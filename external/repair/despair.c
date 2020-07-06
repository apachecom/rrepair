

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct
  { int left,right;
  } Tpair;

int u; // |text| and later current |C| with gaps

int *C; // compressed text

int c;  // real |C|

int alph; // max used terminal symbol

Tpair *R; // rules

int n; // |R|

char *ff;
FILE *f;
int maxdepth = 0;

int expand (int i, int d)

   { int ret = 1;
     char c;
     while (i >= alph)
       { ret += expand(R[i-alph].left,d+1);
         i = R[i-alph].right; d++;
       }
     c = i;
     if (fwrite(&c,sizeof(char),1,f) != 1)
  { fprintf (stderr,"Error: cannot write file %s\n",ff);
    exit(1);
  }
     if (d > maxdepth) maxdepth = d;
     return ret;
   }

void main (int argc, char **argv)

   { char fname[1024]; char outname[1024];
     char *text;
     FILE *Tf,*Rf,*Cf;
     int i,len,c,u;
     struct stat s;
     if (argc != 2)
  { fprintf (stderr,"Usage: %s <filename>\n"
        "Decompresses <filename> from its .C and .R "
        "extensions.\n Decompressed file is <filename>.out\n"
        "This is a version for prefix-free parsing\n",argv[0]);
    exit(1);
  }
     strcpy(fname,argv[1]);
     strcat(fname,".R");
     if (stat (fname,&s) != 0)
  { fprintf (stderr,"Error: cannot stat file %s\n",fname);
    exit(1);
  }
     len = s.st_size;
     Rf = fopen (fname,"r");
     if (Rf == NULL)
  { fprintf (stderr,"Error: cannot open file %s for reading\n",fname);
    exit(1);
  }
     if (fread(&alph,sizeof(int),1,Rf) != 1)
  { fprintf (stderr,"Error: cannot read file %s\n",fname);
    exit(1);
  }
     n = (len-sizeof(int))/sizeof(Tpair);
     R = (void*)malloc(n*sizeof(Tpair));
     if (fread(R,sizeof(Tpair),n,Rf) != n)
  { fprintf (stderr,"Error: cannot read file %s\n",fname);
    exit(1);
  }
     fclose(Rf);

     strcpy(fname,argv[1]);
     strcat(fname,".C");
     if (stat (fname,&s) != 0)
  { fprintf (stderr,"Error: cannot stat file %s\n",fname);
    exit(1);
  }
     c = len = s.st_size/sizeof(int);
     Cf = fopen (fname,"r");
     if (Cf == NULL)
  { fprintf (stderr,"Error: cannot open file %s for reading\n",fname);
    exit(1);
  }


     strcpy(outname,argv[1]);
     strcat(outname,".out");
     Tf = fopen (outname,"w");
     if (Tf == NULL)
  { fprintf (stderr,"Error: cannot open file %s for writing\n",outname);
    exit(1);
  }
     u = 0; f = Tf; 
     for (;len>0;len--)
  { if (fread(&i,sizeof(int),1,Cf) != 1)
       { fprintf (stderr,"Error: cannot read file %s\n",fname);
         exit(1);
       }
    u += expand(i,0);
  }
     fclose(Cf);
     if (fclose(Tf) != 0)
  { fprintf (stderr,"Error: cannot close file %s\n",outname);
    exit(1);
  }
     exit(0);
   }

