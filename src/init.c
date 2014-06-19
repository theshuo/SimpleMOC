#include"SimpleMOC_header.h"

// Gets input from user and sets defaults
Input get_input( void )
{
	Input input;

	input.x_assemblies = 15;        // Number of assemblies in the x-axis of the reactor
	input.y_assemblies = 15;        // Number of assemblies in the y-axis of the reactor
	input.cai = 27;                 // This is the number of course axial intervals
	input.fai = 5;                  // This is the number of fine axial intervals per course axial interval
	input.axial_exp = 2;            // Axial source expansion order
	input.radial_ray_sep = 0.1;     // Radial ray separation
	input.axial_z_sep = 0.2;        // Axial stacked z-ray separation
	input.n_azimuthal = 32;         // Number of azimuthal angles
	input.n_polar_angles = 10;      // Number of polar angles
	input.n_egroups = 100;          // Number of energy groups
	input.decompose = 0;            // Turn decomposition on/off (1 on, 0 off)
	input.decomp_assemblies_ax = 1; // Number of assemblies per sub-domain (axially)

	// TODO: Add file/CLI user input

	return input;
}

// Sets the parameters for the reactor and calculates the radii
Reactor reactor_init( void )
{
	Reactor reactor;

	reactor.pin_cell_width = 1.26;
	reactor.pin_radius = 0.46;
	reactor.assembly_width = 1.26 * 17;
	reactor.n_radial_regions = 10;
	reactor.n_azimuthal_regions = 8;
	reactor.radii = determine_radii( reactor );

	return reactor;
}