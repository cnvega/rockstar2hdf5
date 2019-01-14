#include "halos.h"

int main(int argc, char **argv)
{
   char inname[LINE_MAX], outname[LINE_MAX];
   FILE *infile, *outfile;
   char stmp[LINE_MAX];
   int64_t i, nheader=0, nhalos=0;
   struct Halo_Data halo;
   
   if (argc != 3) {
      printf("Usage: ./bin2ascii <inFile> <outFile>\n");
      exit(EXIT_FAILURE);
   }
   sprintf(inname, "%s", argv[1]);
   sprintf(outname, "%s", argv[2]);
 
   // Open the files:
   if (!(infile = fopen(inname, "r"))) {
      fprintf(stderr, "Cannot read file: %s\n", inname);
      exit(EXIT_FAILURE);
   }
   if (!(outfile = fopen(outname, "w"))) {
      fprintf(stderr, "Cannot create file: %s\n", outname);
      exit(EXIT_FAILURE);
   }
 
   // Copy the header:
   fread(&nhalos, sizeof(int64_t), 1, infile);
   fread(&nheader, sizeof(int64_t), 1, infile);
   for (i=0; i<nheader; i++){
      fread(stmp, sizeof(char), LINE_MAX, infile);
      fprintf(outfile, "%s", stmp); 
   } 
   
   // Parse the halos:
   for (i=0; i<nhalos; i++){
      fread(&halo, sizeof(struct Halo_Data), 1, infile);
      parse_halo_to_string(stmp, &halo);
      fprintf(outfile, "%s", stmp); 
   }
   
   fclose(infile);
   fclose(outfile);
   
   exit(EXIT_SUCCESS);
}
