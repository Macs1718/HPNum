// Box class tests
# include <iostream>
# include <cmath>
# include "core/logger.hpp"
# include "core/log_to_std_error.hpp"
# include "geometry/box.hpp"
# include "geometry/tolerance.hpp"

int main()
{
    Core::Logger log;
    log.subscribe(new Core::LogToStdError( Core::Logger::error ));
    Geometry::Box<double> box1( {-1., 0., -2.}, { 1., 2., 2. } );
    Geometry::Box<double> box2( {-1.,-1.,-1. }, 2., 2., 3. );
    
    double cx, cy, cz;
    std::tie(cx,cy,cz) = box1.center();
    auto minCrds = box1.minCoords();
    auto maxCrds = box1.maxCoords();
    double ccx, ccy, ccz;
    ccx = 0.5*(minCrds[0]+maxCrds[0]); ccy = 0.5*(minCrds[1]+maxCrds[1]);
    ccz = 0.5*(minCrds[2]+maxCrds[2]);
    if ( !Geometry::is_in_tolerance({cx,cy,cz},{ccx,ccy,ccz}) ) {
		log << LogError << "Wrong value for the center of the first box !\n"
		    << "Computed  values : " << cx << ", " << cy << ", " << cz
		    << "and excepted values : 0, 1, 0" << std::endl;
		return EXIT_FAILURE;
	}
	double dx, dy, dz;
	std::tie(dx,dy,dz) = box2.dimensions();

    if ( !Geometry::is_in_tolerance({dx,dy,dz},{ 2., 2., 3. }) ) {
        log << LogError << "Wrong value for the dimension of the second box !\n"
		    << "Computed values : " << dx << ", " << dy << ", " << dz 
		    << " and excepted values : 2., 2., 3." << std::endl;
		return EXIT_FAILURE;
	}
	
	if ( !Geometry::is_in_tolerance( box2.diameterSqr(), 17. ) ) {
		log << LogError << "Wrong value for the squared diameter of the second box \n"
		    << "Computed value : " << box2.diameterSqr() << " and excepted value : 17." << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
