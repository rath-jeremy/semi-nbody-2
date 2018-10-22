#include "SemiNBody.h"
void print_particle(Particle * particle){
	double vals[4]= {particle->state.lmbda,particle->state.Y,particle->state.Lambda,particle->state.X};
	for(int i =0; i<4;i++){	
		printf("%.8f\t",vals[i]);
	}
}
void print_particle_variationals(Particle * particle){
	double dvals[4]= {particle->state.dlmbda,particle->state.dY,particle->state.dLambda,particle->state.dX};
	for(int i =0; i<4;i++){	
		printf("%.8f\t",dvals[i]);
	}
}
void print_derivs(double * derivs){
	for(int i =0; i<NDIM;i++){	
		printf("%.8f\t",derivs[i]);
	}
}
void print_jac(double * jac){
	for(int i =0; i<NDIM;i++){	
		for(int j=0;j<NDIM;j++){
			printf("%.8f\t",*(jac +INDX(i,j)) );
		}
		printf("\n");
	}
}
double get_delta_sq(Particle * particle){
	double delta[4]= {particle->state.dlmbda,particle->state.dY,particle->state.dLambda,particle->state.dX};
	double delta_sq = 0;
	for(int i =0; i<4;i++){	
		delta_sq += delta[i] * delta[i];
	}
	return delta_sq;
}
double get_LCE(Particle * particle,const double log_d0, const double time){
	const double dsq = get_delta_sq(particle);
	const double log_d = 0.5 * log(dsq);
	const double LCE = (log_d - log_d0) / time;
	return LCE;
}

double get_MEGNO(Particle * particle){
	return (particle->megno).megno;
}
void print_aux_vars(MEGNO_Auxilary_Variables * m){
	printf("%.8e\t",m->Y);
	printf("%.8e\t",m->W);
	printf("%.8e\t",m->megno);
}
int main(void){

	const double e0 = 0.1;
	const double a0 = 1;
	const double l0 = M_PI / 2;
	const double pmg0 = 2;
	
	
	// planet perturber
	Planet planet;
	const double a1 = pow(1.5 , 2./3.);
	planet.mu = 1e-5;
	planet.state.x  = a1;
	planet.state.y  = 0 ;
	planet.state.vx = 0 ;
	planet.state.vy = 1 / sqrt(a1);

	// resonances perturber
	ResonancePerturbation res;
	res.resonances = (Resonance *) malloc(3 * sizeof(Resonance));
	initialize_resonance_multiplet(res.resonances,true,7,2,pow(5./7.,2./3.));
	//for(int l=0;l<3;l++){printf("%.5f\t",(res.resonances+l)->fCoeff);}
	//printf("\n");
	res.mu =1e-5;
	res.e = 0.05;
	res.lmbda = M_PI / 4.;
	res.pomega =0;
	res.N_resonances=3;
	res.mean_motion  = 7./5.;
	
	//particle
	Particle particle;
	initialize_phase_state(&(particle.state),a0,l0,e0,pmg0);
	particle.state.dlmbda  = 1;
	particle.state.dLambda = 1;
	particle.state.dX = 0;
	particle.state.dY = 0;
	initialize_megno_vars(&(particle.megno));

	Simulation sim;
	sim.N_planets = 0;
	sim.N_resonance = 1;
	sim.N_particles = 1;
	sim.planets = &planet;
	sim.resonances= &res;
	sim.particles= &particle;
	
	sim.t = 0.;
	const double dt = 2 * M_PI / 60.0;
	sim.dt = dt;
	if(1){
		const int Nout = 100;
		const double tfin = 2 * M_PI * 50;
		int Nsteps = (int) tfin / dt;
		int Nsubstep = Nsteps / Nout;
		for(int i=0; i<Nout ;i++){
			printf("%.8f\t",sim.t);
			print_particle(sim.particles);
			print_particle_variationals(sim.particles);
			print_aux_vars(&(sim.particles->megno));
			printf("\n");
			for(int j=0; j<Nsubstep;j++){simulation_step(&sim);}
		}
	}
	if(0){
		ResonancePerturbation res;
		res.resonances = (Resonance *) malloc(3 * sizeof(Resonance));
		initialize_resonance_multiplet(res.resonances,true,7,2,pow(5./7.,2./3.));
		for(int l=0;l<3;l++){printf("%.5f\t",(res.resonances+l)->fCoeff);}
		printf("\n");
		res.mu =1e-5;
		res.e = 0.05;
		res.lmbda = M_PI / 4.;
		res.pomega =0;
		res.N_resonances=3;
		double derivs[NDIM]={0};
		double jac[NDIM*NDIM]={0};
		add_resonance_derivs(&(particle.state),&res, derivs,jac);
		print_derivs(derivs);
		printf("\n");
		printf("\n");
		print_jac(jac);
		free(res.resonances);
	}
	return 0;
}
