VisualOdometryLib
=================

A stand-alone visual odometry and correspondence library, utilizing the census transform and sensor fusion.  (In work)

CURRENT STATE:

-The modified FAST feature tracking library is done.  Potential for SSE optimizations in the future.

-The CorrespondenceLib is largely done.  Sparse flow and stereo matching are feature complete.  Dense matching is lacking an implementation right now.

-OdometryLib [PLACEHOLDER]

-TrackerLib [PLACEHOLDER]

REQUIREMENTS/DEPENDENCIES:

-Currently, the SSE4 instruction set is required, as well as SSE2.  I have plans to provide non-SSE alternatives in the future, but only after primary functionality is established.

-The test harnesses use OpenCV for image import and visualization, but the Libs themselves are stand-alone C++11

-Tested functionality on VS11, and tested compilation on GCC
