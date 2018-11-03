#ifndef _HALOS_C
#define _HALOS_C

#include "halos.h"

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
          "%f %f\n",       // (41-42)

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
         &(halo->Halfmass_Radius), &(halo->rvmax)  ); 
}

void parse_halo_to_string(char *line, struct Halo_Data *halo)
{
   sprintf(line, 
          "%"PRId64" %"PRId64" %.4e %.2f %.2f %.3f %.3f %"PRId64" "   // (1-8) 
          "%.5f %.5f %.5f %.2f %.2f %.2f %.3e %.3e %.3e "   // (9-17)  
          "%.5f %.5f %.4e %.4e %.4e %.4e %.4e "             // (18-24)
	  "%.5f %.2f %.5f %.5f %.5f %.5f %.5f %.5f "        // (25-32) 
          "%.5f %.5f %.5f %.5f %.5f "                   // (33-37) 
          "%.4f %.3e %.3e "                             // (38-40) 
   // EXTRA properties:
          "%.3f %.3f\n",       // (41-42)

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
         halo->Halfmass_Radius, halo->rvmax  ); 
}

#endif
