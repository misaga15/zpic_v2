/**
 * ZPIC - em1d
 *
 * Laser Wakefield Acceleration - MPI Parallel Version
 */

#include <stdlib.h>
#include <math.h>

#include "../simulation.h"

void sim_init( t_simulation* sim, int rank, int size ){

	// Time step
	float dt = 0.00095;
	float tmax = 40;

	// Global simulation box
	int   nx_global  = 20000;
	float box_global = 20.0;
	
	// Domain decomposition - split domain among MPI ranks
	int nx_local = nx_global / size;
	float box_local = box_global / size;
	
	if (rank == 0) {
		printf("Domain Decomposition:\n");
		printf("  Global domain: %d cells, %.2f box size\n", nx_global, box_global);
		printf("  Local domain per rank: %d cells, %.2f box size\n\n", nx_local, box_local);
	}

	// Diagnostic frequency
	int ndump = 1;

    // Initialize particles
	const int n_species = 1;

	// Use 10 particles per cell
	int ppc = 10;

	// Density profile - adjust start position for local domain
	// Global start is at 54. 0, need to check if this rank contains that region
	float global_start = 54.0;
	float local_box_start = rank * box_local;
	float local_box_end = (rank + 1) * box_local;
	
	t_density density;
	
	// Check if density step falls within this rank's domain
	if (global_start >= local_box_start && global_start < local_box_end) {
		// This rank contains the density step
		density. type = STEP;
		density.start = global_start - local_box_start;  // Convert to local coordinates
	} else if (global_start < local_box_start) {
		// Density step is before this rank - all plasma
		density.type = UNIFORM;
		density.n = 1.0;
	} else {
		// Density step is after this rank - all vacuum
		density.type = EMPTY;
	}

	t_species* species = (t_species *) malloc( n_species * sizeof( t_species ));
	spec_new( &species[0], "electrons", -1.0, ppc, NULL, NULL, nx_local, box_local, dt, &density );

	// Initialize Simulation data with LOCAL domain
	sim_new( sim, nx_local, box_local, dt, tmax, ndump, species, n_species );

	// Add laser pulse (this must come after sim_new)
	// Laser starts at global position 17.0
	float global_laser_start = 17.0;
	
	// Only add laser if it starts in this rank's domain
	if (global_laser_start >= local_box_start && global_laser_start < local_box_end) {
		t_emf_laser laser = {
			.start = global_laser_start - local_box_start,  // Convert to local coordinates
			. fwhm  = 2.0,
			.a0 = 2.0,
			.omega0 = 10.0,
			.polarization = M_PI_2
		};
		sim_add_laser( sim, &laser );
		
		if (rank == 0) {
			printf("Laser initialized in rank %d\n", rank);
		}
	}

	// Set moving window (this must come after sim_new)
	sim_set_moving_window( sim );

	// Set current smoothing (this must come after sim_new)
	t_smooth smooth = {
		.xtype = COMPENSATED,
		.xlevel = 4
	};

	sim_set_smooth( sim, &smooth );

}


void sim_report( t_simulation* sim ){

	// All electric field components
	//emf_report( &sim->emf, EFLD, 0 );
	//emf_report( &sim->emf, EFLD, 1 );
	//emf_report( &sim->emf, EFLD, 2 );

	//// Charge density
	//spec_report( &sim->species[0], CHARGE, NULL, NULL );

    //// x1u1 phasespace
	//const int pha_nx[] = {1024,512};
	//const float pha_range[][2] = {{0.0,20.0}, {-2.0,+2.0}};
	//spec_report(&sim->species[0], PHASESPACE(X1,U1), pha_nx, pha_range);

}