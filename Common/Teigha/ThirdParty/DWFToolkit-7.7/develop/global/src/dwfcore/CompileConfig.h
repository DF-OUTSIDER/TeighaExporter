// Start STL Optimizations
// NOTE: These definitions MUST be made before including any STL classes.  The reason they are not #undef'd
// is to serve as a warning that you aren't including this file before any other files.

// _SECURE_SCL set to 0 disables bounds checking and other "debug" style features for STL containers.
// Performance can be severely affected by not setting this to 0( defaults to 1/on ).  It's recommended
// to always set this to 0 for release builds, and only to set to 1 for debug builds you want to test.
#if( _SECURE_SCL != 0 )
#	pragma message( "** _SECURE_SCL is ACTIVE ** Set to 0, or #include CompileConfig.h before including STL ** " )
#else
#	define _SECURE_SCL 0
#endif

// _HAS_ITERATOR_DEBUGGING is not disabled, because it is supposedly automatically disabled for 
// release builds and is enabled for debug builds.  If you want to disable it for Debug builds, 
// uncomment the following :
#if( _HAS_ITERATOR_DEBUGGING != 0 )
#	pragma message( "** _HAS_ITERATOR_DEBUGGING is ACTIVE ** Set to 0, or #include CompileConfig.h before including STL ** " )
#else
#	define _HAS_ITERATOR_DEBUGGING 0
#endif
