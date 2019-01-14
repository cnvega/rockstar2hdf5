#include "halos.h"

FILE * new_ascii_file(char *origname, int part, char **header, int nhl);

int main(int argc, char **argv)
{
   char inname[LINE_MAX];
   FILE *infile, *outfile;
   char stmp[LINE_MAX];
   char **header;
   int64_t maxhalos;
   int64_t i, nheader=0, nhalos=0;
   int filectr;
   
   if (argc != 3) {
      printf("Usage: ./split_ascii <inFile> <N halos>\n");
      exit(EXIT_FAILURE);
   }
   sprintf(inname, "%s", argv[1]);
   maxhalos = atoi(argv[2]);
 
   // Open the files:
   if (!(infile = fopen(inname, "r"))) {
      fprintf(stderr, "Cannot read file: %s\n", inname);
      exit(EXIT_FAILURE);
   }
 
   // Count header lines:
   do {
      fgets(stmp, LINE_MAX, infile); 
      if (stmp[0] == '#')
         nheader++;
   } while (stmp[0] == '#');
   fseek(infile, 0, SEEK_SET);
   
   // Store the header:
   header = malloc(nheader*sizeof(char *));
   for (i=0; i<nheader; i++){
      header[i] = malloc(LINE_MAX*sizeof(char));
      fgets(header[i], LINE_MAX, infile); 
   } 
   
   // Parse the halos:
   filectr = 0; 
   outfile = new_ascii_file(inname, filectr, header, nheader);
   nhalos = 0;
   while(fgets(stmp, LINE_MAX, infile) != NULL) {
      if (nhalos == maxhalos) {
         fclose(outfile);
         filectr++; 
         outfile = new_ascii_file(inname, filectr, header, nheader);
         nhalos = 0;
      }
      fprintf(outfile, "%s", stmp);
      nhalos++;
   }
   
   fclose(infile);
   fclose(outfile);
   
   exit(EXIT_SUCCESS);
}


FILE * new_ascii_file(char *origname, int part, char **header, int nhl)
{
   char fname[LINE_MAX];
   FILE *f;
   int i;

   sprintf(fname, "%s.%d", origname, part);
   if (!(f = fopen(fname, "w"))) {
      fprintf(stderr, "Cannot create file: %s\n", fname);
      exit(EXIT_FAILURE);
   }

   for (i=0; i<nhl; i++) {
      fprintf(f, "%s", header[i]);
   }
   return f;
}


