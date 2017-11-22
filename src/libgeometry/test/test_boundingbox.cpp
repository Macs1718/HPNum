# include <vector>
# include "core/logger.hpp"
# include "core/log_to_std_error.hpp"
# include "core/log_to_std_output.hpp"
# include "geometry/boundingbox.hpp"

int main()
{
	int exit_status = EXIT_SUCCESS;
	Core::Logger log;
	log.subscribe(new Core::LogToStdError( Core::Logger::error ));
	log.subscribe(new Core::LogToStdOutput( Core::Logger::information ));
    std::vector<double> crds = { 
		-1., -1., -1.,
		-0.5, -0.75, -0.5,
		-0.25, -0.25, -0.25,
		0., 0., 0.,
		+1., -1., -1.,
		+0.5, -0.75, -0.5,
		+0.25, -0.25, -0.25,
		-1., +1., -1.,
		-0.5, +0.75, -0.5,
		-0.25, +0.25, -0.25,
		-1., -1., +1.,
		-0.5, -0.75, +0.5,
		-0.25, -0.25, +0.25,
		+1., +1., -1.,
		+0.5, +0.75, -0.5,
		+0.25, +0.25, -0.25,
		+1., -1., +1.,
		+0.5, -0.75, +0.5,
		+0.25, -0.25, +0.25,
		-1., +1., +1.,
		-0.5, +0.75, +0.5,
		-0.25, +0.25, +0.25,
		+1.,  +1.  , +1.,
		+0.5, +0.75, +0.5,
		+0.25,+0.25, +0.25 };

    int nbPts = crds.size()/3;		
    auto bx = 
      Geometry::compute_boundingBox<double,PerNode>( nbPts,
	                     crds.data(), crds.data()+1, crds.data()+2 );
    // Check the bounding box :
	auto crdMin = bx.minCoords();
	auto crdMax = bx.maxCoords();
	double eps  = Geometry::Tolerance<double>::value();
	if (( !Geometry::is_in_tolerance( crdMin, std::array<double,3>{-1.-eps,-1.-eps,-1.-eps} )) ||
	    ( !Geometry::is_in_tolerance( crdMax, std::array<double,3>{+1.+eps,+1.+eps,+1.+eps} )) ) {
		log << LogError << "Computed box : [ " << crdMin[0] << ", " << crdMin[1] << ", " << crdMin[2] << " ] <-> [ "
		    << crdMax[0] << ", " << crdMax[1] << ", " << crdMax[2] << " ]" << std::endl;
		log << "Excepted box : [ -1., -1., -1. ] <-> [ 1., 1., 1. ]" << std::endl;
		exit_status = EXIT_FAILURE;
	}
	std::size_t nbPts1, nbPts2;
	Geometry::Box<double> b1, b2;	
	std::vector<std::size_t> indices(nbPts);
	for ( std::size_t i = 0; i < nbPts; ++i )
	    indices[i] = i;
    std::tie(nbPts1, b1, nbPts2, b2) = 
        Geometry::subdivideAdjustAndSort<double, PerNode>( bx, nbPts,
					       crds.data(), crds.data()+1, crds.data()+2,
					       indices.data() );
	// Check the split and the sort algorithms :
	if ( nbPts1+nbPts2 != nbPts) {
	    log << LogError << "Total number of nodes in splitted boxes not same than number of nodes" << std::endl;
	    log << "Total number nodes : " << nbPts1 << " + " << nbPts2 << " = " << nbPts1+nbPts2
	        << " and number of nodes : " << nbPts << std::endl;
	    exit_status = EXIT_FAILURE;
	}
	// Detect the orientation of the split :
	const int split_WE = 1;
	const int split_NS = 2;
	const int split_FB = 4;
	int split_orientation = split_WE | split_NS | split_FB;
	for ( std::size_t i = 0; i < nbPts1; ++i ) {
	    for ( std::size_t j = 0; j < nbPts2; ++j ) {
	        double x1,y1,z1,x2,y2,z2;
	        x1 = crds[3*indices[i]+0];
	        y1 = crds[3*indices[i]+1];
	        z1 = crds[3*indices[i]+2];
	        x2 = crds[3*indices[j+nbPts1]+0];
	        y2 = crds[3*indices[j+nbPts1]+1];
	        z2 = crds[3*indices[j+nbPts1]+2];
	        if ( x1 > x2 ) split_orientation &= split_NS | split_FB;
	        if ( y1 > y2 ) split_orientation &= split_WE | split_FB;
	        if ( z1 > z2 ) split_orientation &= split_WE | split_NS;
        }
	}
	if ( split_orientation == 0 ) {
	    log << LogError << "Bad split of the original box.Not split orientation found !" << std::endl;
	    exit_status = EXIT_FAILURE;
	}
    log << LogInformation << "Number of nodes in first subbox : " << nbPts1 << std::endl
        << "Number of nodes in second subbox : " << nbPts2 << std::endl;
    auto cMin1 = b1.minCoords();
    auto cMax1 = b1.maxCoords();
    auto cMin2 = b2.minCoords();
    auto cMax2 = b2.maxCoords();
    log << "First box : [ " << cMin1[0] << ", " << cMin1[1] << ", " << cMin1[2] << " ] <-> [ "
        << cMax1[0] << ", " << cMax1[1] << ", " << cMax1[2] << " ]" << std::endl;
    log << "Second box : [ " << cMin2[0] << ", " << cMin2[1] << ", " << cMin2[2] << " ] <-> [ "
        << cMax2[0] << ", " << cMax2[1] << ", " << cMax2[2] << " ]" << std::endl;
	return exit_status;
}
