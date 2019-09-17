#ifndef _HALOS_C
#define _HALOS_C

#include "halos.h"

struct Halo_Prop_Info Halo_Info[NUMFIELDS] = {
        { "ID", INT64, NULL},  
        { "DescID", INT64, NULL}, 
        { "Mvir", FLOAT, NULL},
        { "Vmax", FLOAT, NULL},
        { "Vrms", FLOAT, NULL},
        { "Rvir", FLOAT, NULL},
        { "Rs", FLOAT, NULL},
        { "Np", INT64, NULL},
        { "X", FLOAT, NULL},
        { "Y", FLOAT, NULL},
        { "Z", FLOAT, NULL},
        { "VX", FLOAT, NULL},
        { "VY", FLOAT, NULL},
        { "VZ", FLOAT, NULL},
        { "JX", FLOAT, NULL},
        { "JY", FLOAT, NULL},
        { "JZ", FLOAT, NULL},
        { "Spin", FLOAT, NULL},
        { "rs_klypin", FLOAT,NULL},
        { "Mvir_all", FLOAT, NULL},
        { "M200b", FLOAT, NULL},
        { "M200c", FLOAT, NULL},
        { "M500c", FLOAT, NULL},
        { "M2500c", FLOAT, NULL},
        { "Xoff", FLOAT, NULL}, 
        { "Voff", FLOAT, NULL},
        { "spin_bullock", FLOAT, NULL},
        { "b_to_a", FLOAT, NULL}, 
        { "c_to_a", FLOAT, NULL},
        { "Ax", FLOAT, NULL},
        { "Ay", FLOAT, NULL},
        { "Az", FLOAT, NULL},
        { "b_to_a_500c", FLOAT, NULL},
        { "c_to_a_500c", FLOAT, NULL},
        { "Ax_500c", FLOAT, NULL},
        { "Ay_500c", FLOAT, NULL},
        { "Az_500c", FLOAT, NULL},
        { "T_div_U", FLOAT, NULL},
        { "M_pe_Behroozi", FLOAT, NULL},
        { "M_pe_Diemer", FLOAT, NULL},
   // EXTRA properties:
        { "Halfmass_Radius", FLOAT, NULL},
        { "rvmax", FLOAT, NULL}, 
        { "PID", INT64, NULL}  
};

void parse_string_to_halo(struct Halo_Data *halo, char *line)
{
   sscanf(line, 
          "%"SCNd64" %"SCNd64" %e %f %f %f %f %"SCNd64 // (1-8) 
          "%f %f %f %f %f %f %e %e %e "            // (9-17)  
          "%f %f %e %e %e %e %e "                  // (18-24)
	  "%f %f %f %f %f %f %f %f "               // (25-32)
          "%f %f %f %f %f "                  // (33-37) 
          "%f %e %e "                        // (38-40) 
   // EXTRA properties:
          "%f %f %"SCNd64"\n",       // (41-43)

         &(halo->ID), &(halo->DescID),  &(halo->Mvir), &(halo->Vmax), 
         &(halo->Vrms), &(halo->Rvir), &(halo->Rs), &(halo->Np), 
         &(halo->X[0]), &(halo->X[1]), &(halo->X[2]), 
         &(halo->V[0]), &(halo->V[1]), &(halo->V[2]), 
         &(halo->J[0]), &(halo->J[1]), &(halo->J[2]), 
         &(halo->Spin), &(halo->rs_klypin), &(halo->Mvir_all), 
         &(halo->M200b), &(halo->M200c), &(halo->M500c), &(halo->M2500c),
         &(halo->Xoff), &(halo->Voff), &(halo->spin_bullock), 
         &(halo->b_to_a), &(halo->c_to_a),
         &(halo->A[0]), &(halo->A[1]), &(halo->A[2]), 
         &(halo->b_to_a_500c), &(halo->c_to_a_500c), 
         &(halo->A_500c[0]), &(halo->A_500c[1]), &(halo->A_500c[2]), 
         &(halo->T_div_U), &(halo->M_pe_Behroozi), &(halo->M_pe_Diemer),
   // EXTRA properties:
         &(halo->Halfmass_Radius), &(halo->rvmax), &(halo->PID) ); 
}

void parse_halo_to_string(char *line, struct Halo_Data *halo)
{
   sprintf(line, 
          "%"PRId64" %"PRId64" %.3e %.2f %.2f %.3f %.3f %"PRId64" "   // (1-8) 
          "%.5f %.5f %.5f %.2f %.2f %.2f %.3e %.3e %.3e "   // (9-17)  
          "%.5f %.5f %.4e %.4e %.4e %.4e %.4e "             // (18-24)
	  "%.5f %.2f %.5f %.5f %.5f %.5f %.5f %.5f "        // (25-32) 
          "%.5f %.5f %.5f %.5f %.5f "                   // (33-37) 
          "%.4f %.3e %.3e "                             // (38-40) 
   // EXTRA properties:
          "%.3f %.3f %"PRId64"\n",       // (41-42)

         halo->ID, halo->DescID,  halo->Mvir, halo->Vmax, 
         halo->Vrms, halo->Rvir, halo->Rs, halo->Np, 
         halo->X[0], halo->X[1], halo->X[2], 
         halo->V[0], halo->V[1], halo->V[2], 
         halo->J[0], halo->J[1], halo->J[2], 
         halo->Spin, halo->rs_klypin, halo->Mvir_all, 
         halo->M200b, halo->M200c, halo->M500c, halo->M2500c,
         halo->Xoff, halo->Voff, halo->spin_bullock, 
         halo->b_to_a, halo->c_to_a,
         halo->A[0], halo->A[1], halo->A[2], 
         halo->b_to_a_500c, halo->c_to_a_500c, 
         halo->A_500c[0], halo->A_500c[1], halo->A_500c[2], 
         halo->T_div_U, halo->M_pe_Behroozi, halo->M_pe_Diemer,
   // EXTRA properties:
         halo->Halfmass_Radius, halo->rvmax, halo->PID  ); 
}

void *check_realloc(void *ptr, size_t size) {
  void *res = realloc(ptr, size);
  if ((res == NULL) && (size > 0)) {
    fprintf(stderr, "[Error] Failed to allocate %"PRId64" bytes of memory!\n", (int64_t)size);
    exit(EXIT_FAILURE);
  }  
  return res;
}


void fill_halo_info_ptrs(struct Halo_Data * halo)
{
    Halo_Info[0].ptr = &halo->ID;
    Halo_Info[1].ptr = &halo->DescID;
    Halo_Info[2].ptr = &halo->Mvir;
    Halo_Info[3].ptr = &halo->Vmax;
    Halo_Info[4].ptr = &halo->Vrms;
    Halo_Info[5].ptr = &halo->Rvir;
    Halo_Info[6].ptr = &halo->Rs;
    Halo_Info[7].ptr = &halo->Np;
    Halo_Info[8].ptr = &halo->X[0];
    Halo_Info[9].ptr = &halo->X[1];
    Halo_Info[10].ptr = &halo->X[2];
    Halo_Info[11].ptr = &halo->V[0];
    Halo_Info[12].ptr = &halo->V[1];
    Halo_Info[13].ptr = &halo->V[2];
    Halo_Info[14].ptr = &halo->J[0];
    Halo_Info[15].ptr = &halo->J[1];
    Halo_Info[16].ptr = &halo->J[2];
    Halo_Info[17].ptr = &halo->Spin;
    Halo_Info[18].ptr = &halo->rs_klypin;
    Halo_Info[19].ptr = &halo->Mvir_all;
    Halo_Info[20].ptr = &halo->M200b;
    Halo_Info[21].ptr = &halo->M200c;
    Halo_Info[22].ptr = &halo->M500c;
    Halo_Info[23].ptr = &halo->M2500c;
    Halo_Info[24].ptr = &halo->Xoff; 
    Halo_Info[25].ptr = &halo->Voff;
    Halo_Info[26].ptr = &halo->spin_bullock;
    Halo_Info[27].ptr = &halo->b_to_a;
    Halo_Info[28].ptr = &halo->c_to_a;
    Halo_Info[29].ptr = &halo->A[0];
    Halo_Info[30].ptr = &halo->A[1];
    Halo_Info[31].ptr = &halo->A[2];
    Halo_Info[32].ptr = &halo->b_to_a_500c;
    Halo_Info[33].ptr = &halo->c_to_a_500c;
    Halo_Info[34].ptr = &halo->A_500c[0];
    Halo_Info[35].ptr = &halo->A_500c[1];
    Halo_Info[36].ptr = &halo->A_500c[2];
    Halo_Info[37].ptr = &halo->T_div_U;
    Halo_Info[38].ptr = &halo->M_pe_Behroozi;
    Halo_Info[39].ptr = &halo->M_pe_Diemer;
   // EXTRA properties:
    Halo_Info[40].ptr = &halo->Halfmass_Radius;
    Halo_Info[41].ptr = &halo->rvmax; 
    Halo_Info[42].ptr = &halo->PID; 
}

#endif
