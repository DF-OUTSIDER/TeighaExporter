Building CS-MAP on Windows and Linux

The CS-MAP distribution will produce a series of nine directories:

Include: Contains all header files referenced the source code in the Source directory.

Source: Contains all the source code for the CS-MAP library itself.

Dictionaries: Contains the coordinate system dictionaries in source form, and the source code for a compiler which will convert the dictionary source to the operational binary form.

Documentation: A .pdf file which described how to use the features of the library is included in this directory.

TestCpp: Contains the source code for the C++ version of the console test program and the test data which it uses.  (The original 'C' version named simply Test has been deprecated.)

ConsoleUtilities: A repository for C++ source code which has been generated over the years to perform maintenance operations programatically, rather than manually.  Much of this is retained for the sole purpose of supporting future maintenance efforts. Of value for CS-MAP contributing developers only.

VC90: Contains solution and project files suitable for use with Microsoft Visual Studio 2008 (Version 9.0).

VC100: Contains solution and project files suitable for use with Microsoft Visual Studio 2010 (Version 10.0).

VC110: Contains solution and project files suitable for use with Microsoft Visual Studio 2012 (Version 11.0).

Building the entire product is a series of five steps:
	1> Build the CS-MAP library.
	2> Build the dictionary compiler.
	3> Run the dictionary compiler.
	4> Build the console test program.
	5> Execute the console test program.

After installation, and before building, it will be best to obtain a copy of the Canadian National Transformation file (NTV2_0.gsb) and copy it to the Dictionaries/Canada directory.  This data file is not in the public domain, thus it may not be distributed as a part of this Open Source distribution.  Geomatics Canada reserves the right to distribute this file and maintain a list of those using it.  This is part of an ISO 9000 consideration.  Therefore, since we do not distribute the file as part of this open source distribution, we recommend strongly that you simply obtain a copy, even if only for testing purposes.

Chances are very good you already have a copy of this file on your system already.  If not, you can obtain one (no fee, but registration is required) at:

	http://www.geod.nrcan.gc.ca

You will need to inform the CS-MAP library of the existence of this file and where it is located. In release 13 and thereafter, the existence and location of the NTV2_0.gsb file is conveyed to the CS-MAP library through one or more definitions in the Geodetic Transformation Dictionary.  Thus, you must modify the source file for this dictionary (GeodeticTransform.asc) and then recompile it using the dictionary compiler.  As of this writing, there is no UI or alternative means of modifying this information.

The TEST.DAT data file in the Test directory contains several hundred test points which are directly related to the above mentioned grid shift data file.  To prevent confusion and unnecessary technical support, these test points are commented out in the distribution.  After obtaining a copy of the above mentioned data file, these test should be uncommented back in, so that the test program will test this feature.

The situation described above concerning the Canadian National Transformation also applies to other sources of geodetic transformation data.  In these cases, it is not so much that we know we are not permitted to distribute the file, it is more that we are unable to determine that we are permitted to distribute the data file.  Thus, to be safe the files are not distributed.  At this writing, this situation applies to other CSRS related Canadian files and certain French municipal data files.  Check the appropriate locations in the distribution folder hierarchy for README files which describe the situation for these locations and provides suggestions on how to obtain a copy of the file.

A similar situation exists with regard to the Danish System 34 conversion modules.  We are unable to distribute the coefficients for this transformation, even though the code is freely available on the Internet.  Should you need to, or desire to, include the Danish System 34 conversion in your application, please carefully read the introductory comments of the provided "CSsys34KMS.c" (distributed in the 'Source" folder, but NOT included in any builds).

OK. Now for building on your system.

Make files are provided for both Linux and Windows environments.  Makefiles targeted for, and tested on, Linux should provide the basis for creating decent makefiles in other Unix type environments. The following information applies to both the Windows and Linux environments.

There are essentially four base components to the CS-MAP distribution which must be completed to have a usable library.  These compponents are:
	1> The Library itself, which exists as a static object C/C++ object library named csmap.a/csmap.lib.
	2> Six binary dictionary files which are produced by compiling the provided ASCII source files.
	3> The coordinate system compiler which converts the dictionary source files into a usable binary form.
	4> The Console Test program which is used to verify successful installation, and can be used to verify the integrity of the library after changes to the source code have been made.

The build process is rather straight forward:
	1> Build the library.					(Source/Library.mak (.nmk))
	2> Build the dictionary compiler.    	(Dictionaries/Compiler.mak (.nmk))
	3> Compile the dictionaries.    		(Dictionaries/Compiler.mak (.nmk))
	4> Build the console test module.		(TestCpp/TestCpp.mak (.nmk))
	5> Execute the test module to verify valid installation.

An overall make file, CsMap.mak (.nmk), is provided which will invoke each of the above mentioned makefiles in the proper order.

To complete all five steps on Linux systems, simple make the CsMapDev directory the current working directory and enter the following command:

	make -fCsMap.mak Linux64 QuickTest

On Windows based platforms, you will first need to import to the environment the Visual Studio variable settings.  These are typically set by executing the command script named 'vcvars32.bat' which is traditionally located in the ..\VC\bin directory of the Visual Studio installation.  Having accomplished that, building the library for your system can be accomplished by entering the following command:

	nmake -fCsMap.nmk Release QuickTest

This procedure has been tested with Visual Studio 2008 (Version 9.0), Visual Studio 2010 (Version 10.0), and Visual Studio 2012 (version 11.0). Examination of the makefiles will show that minor tweaks to the makefiles will enable Release/Debug builds of all three compiler releases on the same system concurrently.  Check the detailed comments in the appropriate makefile.

Finally, also notice that in the directories named CsMapDev\VC90, CsMapDev\VC100, and CsMapDev\VC110 you will find complete Visual Studio solution and project files which can be used to make the entire product; one set of files for each of the supported Visual Studio releases.
	
Defects and Enhancements

Please report any defects in the code, build process, and/or documentation using the Trac facility located at "trac.osgeo.org/csmap"  While CS-MAP shares a subversion repository with other coordinate system related products, it has its own defect tracking database at the indicated location.  You will need to create a login for yourself to record a bug, but anyone can create a login and there is no fee.  You can also use this means to request an enhancement to the product.

With regard to defects and/or enhancements, there are no guarantees that any action will be initiated in any given time frame.  As the product gains wider use, there will be more and more developers interested in a bug free product and the chances of a timely correction or implementation will increase.

If you are a developer with experience in the area of coordinate system conversion and/or transformation, please consider becoming a contributor to the project.  Visit

	http://trac.osgeo.org/metacrs/

for more information.  Of course, you can send a patch to any of the listed committers requesting that the change be officially implemented.  This is the fastest way to get a bug fixed; but still no guarantees.
