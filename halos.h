#ifndef _HALOS_H
#define _HALOS_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define LINE_MAX 512
#define BAD_VALUE 2e38

/***************************************************
 TODO: Modify these files according to your outputs!
 ***************************************************/
/*
#define NUMPROP 40      // Original format
*/
#define NUMPROP 43      // num columns in file
#define NUMFIELDS 43    // num properties  

struct Halo_Data {
   int64_t ID;      // (1) 
   int64_t DescID;  // (2) 
   float   Mvir;    // (3) 
   float   Vmax;    // (4) 
   float   Vrms;    // (5)
   float   Rvir;    // (6)
   float   Rs;      // (7)
   int64_t Np;     // (8)
   float   X[3];   // (9,10,11)
   float   V[3];   // (12,13,14)
   float   J[3];   // (15,16,17)
   float   Spin;   // (18)
   float   rs_klypin;   // (19)
   float   Mvir_all;   // (20)
   float   M200b;   // (21)
   float   M200c;   // (22)
   float   M500c;   // (23)
   float   M2500c;  // (24)
   float   Xoff, Voff;   // (25,26)
   float   spin_bullock;  // (27)
   float   b_to_a, c_to_a;  // (28,29)
   float   A[3];    // (30,31,32)
   float   b_to_a_500c, c_to_a_500c;  // (33,34)
   float   A_500c[3];   // (35,36,37)
   float   T_div_U;       // (38)
   float   M_pe_Behroozi, M_pe_Diemer;  // (39,40)
   
   // EXTRA properties:

   float   Halfmass_Radius;     // (41)
   float   rvmax;       // (42)
   int64_t PID;   // (43)
};

//enum Halo_Prop_Type {FLOAT, INT64, DOUBLE, INT};
enum Halo_Prop_Type {FLOAT, INT64};

struct Halo_Prop_Info {
   char name[25];
   enum Halo_Prop_Type type;
   //short int dims;
   void *ptr;
};

// Defined and initialized in halos.c
extern struct Halo_Prop_Info Halo_Info[NUMFIELDS];


void parse_string_to_halo(struct Halo_Data *halo, char *line);

void parse_halo_to_string(char *line, struct Halo_Data *halo);

void *check_realloc(void *ptr, size_t size);

void fill_halo_info_ptrs(struct Halo_Data * halo);
#endif
