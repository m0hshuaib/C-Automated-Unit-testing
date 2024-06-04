#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TrackTests
#include <boost/test/unit_test.hpp>

#include "gpx-parser.h"
#include "track.h"
#include <vector>
#include <stdexcept>

#include "waypoint.h"

using namespace GPS;

// Function to compare two waypoints manually since the == operator is not available
bool areWaypointsEqual(const Waypoint& wp1, const Waypoint& wp2) {
    return (wp1.latitude() == wp2.latitude() &&
            wp1.longitude() == wp2.longitude() &&
            wp1.altitude() == wp2.altitude());
}

BOOST_AUTO_TEST_SUITE( MostEasterlyWaypoint )


/* NOTE ABOUT EXPECTED TEST OUTPUT: In all cases,
 * the most easterly waypoint is the waypoint with a longitude value
 * that is closest to +180o
 * */



/* Error Case: Zero Track points
 * Purpose:Ensures that the function throws a domain error when no waypoints are available
 * */
BOOST_AUTO_TEST_CASE( ZeroWaypoints )
{
    std::vector<Trackpoint> theTrackPoints = {};
    Track theTrack = Track(theTrackPoints);

    theTrack.mostEasterlyWaypoint();

    BOOST_CHECK_THROW( theTrack.mostEasterlyWaypoint(), std::domain_error);
}

/* Typical Case: Track with a clear single most eastern waypoint
 * Purpose: Ensures correct identification of the most easterly waypoint among multiple options.
 * */
BOOST_AUTO_TEST_CASE( OneTypicalMostEasterly )
{
    Waypoint wp1(10, 54.42204773426058, 100);
    Waypoint wp2(20, 87.42204773426058, 100);
    Waypoint wp3(30, -45.42204773426058, 100);
    Waypoint wp4(40, -54.42204773426058, 100);
    Trackpoint tp1 = {wp1, 1000};
    Trackpoint tp2 = {wp2, 2000};
    Trackpoint tp3 = {wp3, 3000};
    Trackpoint tp4 = {wp4, 4000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2, tp3, tp4};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp2;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}

/* Typical Case: Testing with Waypoints having small whole number difference in longitude
 * Purpose: Validates precision in calculations/comparisons of longitude values.
 * */
BOOST_AUTO_TEST_CASE(SmallWholeNumberDifference)
{
    Waypoint wp1(51.234, 142, 200);
    Waypoint wp2(52.234, 150, 200);
    Waypoint wp3(53.345, 148, 300);
    Waypoint wp4(54.345, 143, 300);
    Trackpoint tp1 = {wp1, 1000};
    Trackpoint tp2 = {wp2, 2000};
    Trackpoint tp3 = {wp3, 3000};
    Trackpoint tp4 = {wp4, 4000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2, tp3, tp4};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp2;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}


/* Typical Case: Track with the first waypoint as the most easterly
 * */
BOOST_AUTO_TEST_CASE(MostEasterlyIsFirst)
{
    Waypoint wp1(10, 1.118760108947754, 100);
    Waypoint wp2(20, 1.118561625480652, 100);
    Waypoint wp3(30, 1.118427515029907, 100);
    Waypoint wp4(40, 1.118068099021912, 100);
    Trackpoint tp1 = {wp1, 1000};
    Trackpoint tp2 = {wp2, 2000};
    Trackpoint tp3 = {wp3, 3000};
    Trackpoint tp4 = {wp4, 4000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2, tp3, tp4};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp1;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}

/* Typical Case: Track with last waypoint as most easterly
 * */
BOOST_AUTO_TEST_CASE( MostEasterlyIsLast )
{
    Waypoint wp1(10, 1.118060108947754, 100);
    Waypoint wp2(20, 1.118461625480652, 100);
    Waypoint wp3(30, 1.118527515029907, 100);
    Waypoint wp4(40, 1.118768099021912, 100);
    Trackpoint tp1 = {wp1, 1000};
    Trackpoint tp2 = {wp2, 2000};
    Trackpoint tp3 = {wp3, 3000};
    Trackpoint tp4 = {wp4, 4000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2, tp3, tp4};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp4;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}

// Edge Case: Track with a single track point
BOOST_AUTO_TEST_CASE( OneTrackPoint )
{
    Waypoint wp1(10, 54.42204773426058, 100);
    Trackpoint tp1 = {wp1, 1000};
    std::vector<Trackpoint> theTrackPoints = {tp1};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp1;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}

// Edge Case: Track with a two track points
BOOST_AUTO_TEST_CASE( TwoTrackPoints )
{
    Waypoint wp1(10, 54.42204773426058, 100);
    Waypoint wp2(20, 54.52204773426058, 200);
    Trackpoint tp1 = {wp1, 1000};
    Trackpoint tp2 = {wp2, 2000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp2;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}

// Edge Case: Track points with maximum and minimum longitudes
BOOST_AUTO_TEST_CASE( MaxAndMinLongitude )
{
    Waypoint wp1(10, 180, 100);
    Waypoint wp2(10, -180, 100);
    Trackpoint tp1 = {wp1, 1000};
    Trackpoint tp2 = {wp2, 1000};
    std::vector<Trackpoint> theTrackPoints = {tp1,tp2};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp1;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}



/* Boundary Case:
 * Purpose: Tests precision near critical geographic boundaries (0 degrees longitude).
 * */
BOOST_AUTO_TEST_CASE(WaypointsNearPrimeMeridian)
{
    Waypoint wpNearPrimeMeridian(50.123, 0.005, 100);
    Waypoint wpExactlyAtPrimeMeridian(53.456, 0.000, 400);
    Trackpoint tp1 = {wpNearPrimeMeridian, 1000};
    Trackpoint tp2 = {wpExactlyAtPrimeMeridian, 4000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wpNearPrimeMeridian;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}

// Boundary Case:
BOOST_AUTO_TEST_CASE(WaypointsNearInternationalDateLine)
{
    Waypoint wpNearDateLineEast(51.234, 179.995, 200);
    Waypoint wpNearDateLineWest(52.345, -179.995, 300);
    Trackpoint tp1 = {wpNearDateLineEast, 2000};
    Trackpoint tp2 = {wpNearDateLineWest, 3000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wpNearDateLineEast;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}


/* Atypical Case: Track with two sequential track points having longitudes
 * that are equally further east
 *
 * Purpose: Checks how the function deals with ties in longitude values.
 * */
BOOST_AUTO_TEST_CASE( TwoEquallyMostEastSequential )
{
    Waypoint wp1(10, 54.42204773426058, 100);
    Waypoint wp2(20, 87.42204773426058, 100);
    Waypoint wp3(30, 87.42204773426058, 100);
    Waypoint wp4(40, -54.42204773426058, 100);
    Trackpoint tp1 = {wp1, 1000};
    Trackpoint tp2 = {wp2, 2000};
    Trackpoint tp3 = {wp3, 3000};
    Trackpoint tp4 = {wp4, 4000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2, tp3, tp4};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp2;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}

/*
 * Atypical Case: Track with two track points having longitudes
 * that are equally further east with a track point between them.
 *
 * Purpose: Checks how the function deals with ties in longitude values.
 * */
BOOST_AUTO_TEST_CASE( TwoEquallyMostEastSeperate )
{
    Waypoint wp1(10, 54.42204773426058, 100);
    Waypoint wp2(20, 87.42204773426058, 100);
    Waypoint wp3(30, -45.42204773426058, 100);
    Waypoint wp4(30, 87.42204773426058, 100);
    Waypoint wp5(40, -54.42204773426058, 100);
    Trackpoint tp1 = {wp1, 1000};
    Trackpoint tp2 = {wp2, 2000};
    Trackpoint tp3 = {wp3, 3000};
    Trackpoint tp4 = {wp4, 4000};
    std::vector<Trackpoint> theTrackPoints = {tp1, tp2, tp3, tp4};
    Track theTrack(theTrackPoints);
    Waypoint expectedMostEasterlyWaypoint = wp2;

    Waypoint actualMostEasterlyWaypoint = theTrack.mostEasterlyWaypoint();

    BOOST_CHECK(areWaypointsEqual(expectedMostEasterlyWaypoint, actualMostEasterlyWaypoint));
}
BOOST_AUTO_TEST_SUITE_END()
