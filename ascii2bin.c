#include "halos.h"

int main(int argc, char **argv)
{
   char inname[LINE_MAX], outname[LINE_MAX];
   FILE *infile, *outfile;
   char stmp[LINE_MAX];
   char **header;
   int64_t i, nheader=0, nhalos=0;
   struct Halo_Data halo;
   
   if (argc != 3) {
      printf("Usage: ./ascii2bin <inFile> <outFile>\n");
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
 
   // Count header lines:
   do {
      fgets(stmp, LINE_MAX, infile); 
      if (stmp[0] == '#')
         nheader++;
   } while (stmp[0] == '#');
   fseek(infile, 0, SEEK_SET);

   // Copy the header:
   fwrite(&nhalos, sizeof(int64_t), 1, outfile);
   fwrite(&nheader, sizeof(int64_t), 1, outfile);
   for (i=0; i<nheader; i++){
      fgets(stmp, LINE_MAX, infile); 
      fwrite(stmp, sizeof(char), LINE_MAX, outfile);
   } 
   
   // Parse the halos:
   while(fgets(stmp, LINE_MAX, infile) != NULL) {
      parse_string_to_halo(&halo, stmp);
      fwrite(&halo, sizeof(struct Halo_Data), 1, outfile);
      nhalos++;
   }
   
   // Update the hnalos field:
   fseek(outfile, 0, SEEK_SET);
   fwrite(&nhalos, sizeof(int64_t), 1, outfile);

   fclose(infile);
   fclose(outfile);
   
   exit(EXIT_SUCCESS);
}
