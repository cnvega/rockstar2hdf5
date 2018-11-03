#include "halos.h"

FILE * new_binary_file(char *origname, int part, char **header, int64_t nhl);

int main(int argc, char **argv)
{
   char inname[LINE_MAX];
   FILE *infile, *outfile;
   char stmp[LINE_MAX];
   char **header;
   struct Halo_Data halo;
   int64_t maxhalos;
   int64_t i, hif, nheader=0, nhalos=0;
   int filectr;
   
   if (argc != 3) {
      printf("Usage: ./split_binary <inFile> <N halos>\n");
      exit(EXIT_FAILURE);
   }
   sprintf(inname, "%s", argv[1]);
   maxhalos = atoi(argv[2]);
 
   // Open the files:
   if (!(infile = fopen(inname, "r"))) {
      fprintf(stderr, "Cannot read file: %s\n", inname);
      exit(EXIT_FAILURE);
   }
 
   // Store the header:
   fread(&nhalos, sizeof(int64_t), 1, infile);
   fread(&nheader, sizeof(int64_t), 1, infile);
   header = malloc(nheader*sizeof(char *));
   for (i=0; i<nheader; i++){
      header[i] = malloc(LINE_MAX*sizeof(char));
      fread(header[i], sizeof(char), LINE_MAX, infile); 
   } 
   
   // Parse the halos:
   filectr = 0; 
   outfile = new_binary_file(inname, filectr, header, nheader);
   for (i=0, hif=0; i<nhalos; i++, hif++) {
      if (hif == maxhalos) {
         fseek(outfile, 0, SEEK_SET); 
         fwrite(&hif, sizeof(int64_t), 1, outfile);
         fclose(outfile);
         filectr++; 
         outfile = new_binary_file(inname, filectr, header, nheader);
         hif = 0;
      }
      fread(&halo, sizeof(struct Halo_Data), 1, infile);
      fwrite(&halo, sizeof(struct Halo_Data), 1, outfile);
   }
   
   fseek(outfile, 0, SEEK_SET); 
   fwrite(&hif, sizeof(int64_t), 1, outfile);
   fclose(outfile);
   
   fclose(infile);
   
   exit(EXIT_SUCCESS);
}


FILE * new_binary_file(char *origname, int part, char **header, int64_t nhl)
{
   char fname[LINE_MAX];
   FILE *f;
   int i;
   int64_t num=0;

   sprintf(fname, "%s.%d", origname, part);
   if (!(f = fopen(fname, "w"))) {
      fprintf(stderr, "Cannot create file: %s\n", fname);
      exit(EXIT_FAILURE);
   }

   fwrite(&num, sizeof(int64_t), 1, f);
   fwrite(&nhl, sizeof(int64_t), 1, f);
   for (i=0; i<nhl; i++) {
      fwrite(header[i], sizeof(char), LINE_MAX, f);
   }
   return f;
}


