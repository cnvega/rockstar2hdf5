#include <hdf5.h>

#include "halos.h"

#define _DEFP H5P_DEFAULT, H5P_DEFAULT

#define CHUNKSIZE 1000000   // ~30MB each dataset subblock 

#define READSIZE  1000   

// FIXME: It's not good to have global definitions, but this is the
// simplest way I found for now:
int64_t  TotNumHalos, ReadHalos;
hid_t    H5File;

// local prototypes: 
herr_t write_dataset(char*, hid_t, int, void*);
void read_halo_block(FILE *, struct Halo_Data*);
void write_halo_properties(struct Halo_Data*);


int main(int argc, char **argv)
{
   char inname[LINE_MAX], outname[LINE_MAX];
   FILE *infile, *outfile;
   char stmp[LINE_MAX];
   char **header, *header_all;
   int64_t i, nheader=0, nhalos=0;
   
   struct Halo_Data halos[READSIZE];

   hid_t    dataset, dataspace, attribute;
   hsize_t  dims, dims2[2];
   herr_t   status;
   hid_t    dtype;
   size_t   size;
   
   if (argc != 3) {
      printf("Usage: ./ascii2hdf5 <inFile> <outFile>\n");
      exit(EXIT_FAILURE);
   }
   sprintf(inname, "%s", argv[1]);
   sprintf(outname, "%s", argv[2]);
 
   // Open the files:
   if (!(infile = fopen(inname, "r"))) {
      fprintf(stderr, "Cannot read file: %s\n", inname);
      exit(EXIT_FAILURE);
   }
   
   H5File = H5Fcreate(outname, H5F_ACC_TRUNC, _DEFP); 
   
   // Count header lines:
   do {
      fgets(stmp, LINE_MAX, infile); 
      if (stmp[0] == '#')
         nheader++;
   } while (stmp[0] == '#');
   fseek(infile, 0, SEEK_SET);

   // Saving header properties as attributes: 
   dims = 1;
   dataspace = H5Screate_simple(1, &dims, NULL);

   attribute = H5Acreate2(H5File, "NLinesHeader", H5T_NATIVE_INT64, dataspace, _DEFP);
   status = H5Awrite(attribute, H5T_NATIVE_INT64, &nheader);
   status = H5Aclose(attribute);
   
   attribute = H5Acreate2(H5File, "LineSizeHeader", H5T_NATIVE_INT64, dataspace, _DEFP);
   i = LINE_MAX;
   status = H5Awrite(attribute, H5T_NATIVE_INT64, &i);
   status = H5Aclose(attribute);
   
   status = H5Sclose(dataspace);
  
   // Read the header:
   header_all = malloc(nheader*LINE_MAX*sizeof(char));
   header = malloc(nheader * sizeof(char*));
   for (i=0; i<nheader; i++){
      header[i] = &(header_all[i*LINE_MAX]);
      fgets(header[i], LINE_MAX, infile); 
   } 
  
   // Save the header as a string array:
   dtype = H5Tcopy(H5T_C_S1);
   size = LINE_MAX;
   status = H5Tset_size(dtype, size);
   dims2[0] = nheader; 
   dims2[1] = 1;
   dataspace = H5Screate_simple(2, dims2, NULL);
   dataset   = H5Dcreate2(H5File, "Header", dtype, dataspace, H5P_DEFAULT, _DEFP);
   status = H5Dwrite(dataset, dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, header_all);
   
   status = H5Dclose(dataset);
   status = H5Sclose(dataspace);
  

   // Parse the halos:
   TotNumHalos = 0;
   fill_halo_info_ptrs(&halos[0]);
   do {
      read_halo_block(infile, &halos[0]);
      write_halo_properties(&halos[0]);
   } while (ReadHalos == READSIZE);

   // Save the number of halos:
   dataspace = H5Screate_simple(1, &dims, NULL);
   attribute = H5Acreate2(H5File, "NumHalos", H5T_NATIVE_INT64, dataspace, _DEFP);
   status = H5Awrite(attribute, H5T_NATIVE_INT64, &TotNumHalos);
   status = H5Aclose(attribute);
   status = H5Sclose(dataspace);

   fclose(infile);
   status = H5Fclose(H5File);
   
   exit(EXIT_SUCCESS);
}


void read_halo_block(FILE *fin, struct Halo_Data *halos)
{
   char stmp[LINE_MAX];
  
   ReadHalos = 0;
   while(fgets(stmp, LINE_MAX, fin) != NULL) {
      parse_string_to_halo(&halos[ReadHalos], stmp);
      ReadHalos++;
      if (ReadHalos == READSIZE) break;
   }
}


herr_t write_dataset(char *dname, hid_t data_type, int ndims, void *data)
{
   hsize_t   dims[2] = {ReadHalos, ndims}; 
   hsize_t   maxdims[2] = {H5S_UNLIMITED, ndims};
   hsize_t   chunk_dims[2] = {CHUNKSIZE, ndims};
   hsize_t   offset[2], size[2];
   hid_t     dataset;  
   hid_t     filespace, memspace;
   hid_t     prop;                     

   herr_t    status, statusR;

   if (TotNumHalos == 0)  // it creates and writes the dataset
   {
      prop = H5Pcreate(H5P_DATASET_CREATE);

      // Modify dataset creation properties, i.e. enable chunking  
      status = H5Pset_chunk(prop, 2, chunk_dims);
      memspace =  H5Screate_simple(2, dims, maxdims);

      dataset = H5Dcreate2(H5File, dname, data_type, memspace,
                           H5P_DEFAULT, prop, H5P_DEFAULT);
     
      statusR = H5Dwrite(dataset, data_type, H5S_ALL, H5S_ALL,
                         H5P_DEFAULT, data);

      status = H5Dclose(dataset);
      status = H5Sclose(memspace);
      status = H5Pclose(prop);
   }
   else  // Extend an existing dataset and write the data in that portion 
   {
      size[0] = TotNumHalos + ReadHalos;
      size[1] = ndims;
  
      offset[0] = TotNumHalos;
      offset[1] = 0;

      memspace = H5Screate_simple(2, dims, NULL);

      dataset = H5Dopen2(H5File, dname, H5P_DEFAULT);
      status  = H5Dset_extent(dataset, size);

      // Select a hyperslab in extended portion of dataset  
      filespace = H5Dget_space(dataset);
      status = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset, NULL, dims, NULL);
    
      // Write the data to the extended portion of dataset
      statusR = H5Dwrite(dataset, data_type, memspace, filespace, H5P_DEFAULT, data);
   
      status = H5Dclose(dataset);
      status = H5Sclose(filespace);
      status = H5Sclose(memspace);
   }

   if (statusR < 0)
   {
      printf("ERROR in hdf5 lib: Writing of dataset '%s' failed \n\nExit.\n", dname);
      exit(EXIT_FAILURE);
   }
   return statusR;
}

void write_halo_properties(struct Halo_Data * halos )
{
   int p, i, index;
   int64_t h, *i64buf;
   float *buffer, *buffer3d;

   struct Halo_Data * loc;

   buffer = malloc(ReadHalos*sizeof(float));
   buffer3d = malloc(3*ReadHalos*sizeof(float));
   i64buf = malloc(ReadHalos*sizeof(int64_t));

   for (p=0; p<NUMFIELDS; p++) {
      
      loc = Halo_Info[p].ptr;
      
      switch (Halo_Info[p].type) {
         case FLOAT:
            if (Halo_Info[p].dims == 1) {
               for (h=0; h<ReadHalos; h++)
                  buffer[h] = *((float*)&loc[h]);
               write_dataset(Halo_Info[p].name, H5T_NATIVE_FLOAT, 
                             Halo_Info[p].dims, buffer);
            }
            if (Halo_Info[p].dims == 3) {
               for (h=0; h<ReadHalos; h++)
                  for (i=0; i<3; i++) 
                     buffer3d[i+3*h] = ((float*)&loc[h])[i];
               write_dataset(Halo_Info[p].name, H5T_NATIVE_FLOAT, 
                             Halo_Info[p].dims, buffer3d);
            }
            break;

         case INT64:
            for (h=0; h<ReadHalos; h++)
               i64buf[h] = *((int64_t*)&loc[h]);
            write_dataset(Halo_Info[p].name, H5T_NATIVE_INT64, Halo_Info[p].dims, i64buf);
            break;
       }
   }

   TotNumHalos += ReadHalos;

   free(buffer);
   free(buffer3d);
   free(i64buf);
}


