#
# This make file is designed primarily to build the entire target,
# csmap.lib in this case, from scratch in an "installation" environment.
# The secondary objective of the design is to minimize the maintenance
# requirements of achieving the primary objective.  Thus, coding of a
# comprehensive map of interdependencies between files is deliberately
# avoided.
#
# Thus, one should _NOT_ expect this make file to compile only the
# specific modules you might have changed in a development/debug
# environment.  Such may happen, and you may be thankful; but do _NOT_
# rely on this.  Fortunately, build times on current Linux systems are
# so small I suspect you will probably never consider the build to
# be overly inefficient.
#
# Note that simply adding files with the appropriate extensions to
# the appropriate directories, automatically includes them in the
# make process.  Removing files from specific directories has the same
# affect.  To achieve this objective, certain files in particular are
# specifically noted and referenced in this make file and removed from the
# construction of the library.  Your maintenance/development activities may
# require you to add a module name to these lists.  At, or about, line 272
# below, is a convenient place to switch to hard coded module lists if
# such is desired.
#
# This make file was been written with the EXPRESS requirement that it
# exists in the "CsMapDev/Source" and that directory is the current
# working directory of the make executable which is processing it; with the
# original intent being that this makefile would be invoked by a higher
# level make file which executes something like the following:
#
#	$(MAKE) -C./Source -fLibrary.mak
#
# All the essential information about the desired build is expected to be
# passed in environmental variables.
#
PRJ_NAME = Library
TRG_BASE = CsMap
TRG_EXT = .a
TRG_NAME = $(TRG_BASE)$(TRG_EXT)
#
# Set the following default values using the ?= assignment operator.  This
# operator causes an assignment only if the variable is not already defined.
# My current gcc compiler is version 4.7.4, so I set the VERSION argument
# to 47.
#
VERSION ?= 47
CONFIGURATION ?= Linux
PROCESSOR ?= x64
#
# CSMAP_MAINS is set to a list of source modules known to contain "main"
# functions, and therefore are inappropriate for inclusion in the library.
# These names are filtered out before the make process starts.
#
CSMAP_MAINS = CsMapDll.c csTestCpp.cpp csConsoleUtilities.cpp CS_COMP.c
#
# CSMAP_CINCLUDE is set to a list of 'C' source modules which are
# "include'd" into other 'C' modules and are not to be compiled on their
# own.
#
CSMAP_CINCLUDE = CSsys34KMS.c
#
# CSMAP_STDHDRS is set to a list of source modules which exist primarily for
# generation of pre-compiled headers when using the Microsoft Visual Studio
# IDE.  We do not need to consider them for this Linux/Unix/Mac library.
#
CSMAP_STDHDRS = CS_stdhdrC.c CS_stdhdrCpp.cpp CStest0.cpp
#
# CSMAP_MFC is set to a list of source modules which are part of the MFC
# portion of the Windows library.  We exclude these from the Linux/Unix
# build of the library.  There is no MFC equivalent functionality in the
# Linux build.
#
CSMAP_MFC = csBrowser.cpp \
			csDataDir.cpp \
			csDualBrowse.cpp \
			csEdit.cpp \
			csKeyNm.cpp \
			CS_mfc.cpp \
			csTest.cpp \
			CSwinHlp.cpp \
			dtEdit.cpp \
			dtSelect.cpp \
			elEdit.cpp \
			elSelect.cpp \
			gdcEdit.cpp \
			mgTest.cpp

#
# The following definitions are instituted to facilitate building multiple
# versions of the Library. The variables referenced by these definitions
# are expected to be passed, in the environment, from a parent makefile.  
#
OUT_DIR ?= ../lib$(VERSION)/$(CONFIGURATION)
INT_DIR ?= ../obj$(VERSION)/$(PRJ_NAME)/$(CONFIGURATION)
SRC_DIR ?= $(MAKEDIR)

C_FLG ?= -c -Wall -O2 -I../Include
CXX_FLG ?= -c -Wall -O2 -I../Include

LCL_C_FLG = $(C_FLG)
LCL_CXX_FLG = $(CXX_FLG)
ifeq ($(PROCESSOR),x64)
	OUT_DIR := $(OUT_DIR)64
	INT_DIR := $(INT_DIR)64
	LCL_C_FLG += -m64 -fPIC
	LCL_CXX_FLG += -m64 -fPIC
endif

ifeq ($(PROCESSOR),x86)
	OUT_DIR := $(OUT_DIR)32
	INT_DIR := $(INT_DIR)32
#	LCL_C_FLG += -m32
#	LCL_CXX_FLG += -m32
endif
#
# The -o option on the compiler is used to get the objects written to the
# directory specified by the INT_DIR variable. Doing so enables building
# libraries for different configurations without an implied "clean" of any
# configuration(s) already built.  I suspect that it also means that the
# compilations will not be batched and a new instance of the compiler is
# invoked for each module.  Slows us down a bit, but overall helps preserve
# sanity when trying to build multiple configurations of the library from a
# single set of source files, which is a major objective of this makefile.
#
$(INT_DIR)/%.o:$(SRC_DIR)%.c
	$(CC) $(LCL_C_FLG) -o$(INT_DIR)/$(<:.c=.o) $<

$(INT_DIR)/%.o:$(SRC_DIR)%.cpp
	$(CXX) $(LCL_CXX_FLG) -o$(INT_DIR)/$(<:.cpp=.o) $<

# Note that the following causes all .c and .cpp files in the Source
# directory to be included in the csmap.lib target. This is often helpful,
# sometimes painful.  In some cases, the features used may not be available
# in your make implementation.  So, in case this is undesirable or does not
# work, a variable which lists all current distribution sources is coded
# below, with a different name. OsGeo contributors must make sure this
# listing remains current even though it may not be used in your
# environment.  Note that modules not intended for the basic library
# are filtered out.
# 
CSMAP_CC_SRC := $(wildcard *.c)
CSMAP_CC_SRC := $(filter-out $(CSMAP_MAINS),$(CSMAP_CC_SRC))  
CSMAP_CC_SRC := $(filter-out $(CSMAP_STDHDRS),$(CSMAP_CC_SRC))  
CSMAP_CC_SRC := $(filter-out $(CSMAP_CINCLUDE),$(CSMAP_CC_SRC))  

CSMAP_CPP_SRC := $(wildcard *.cpp)
CSMAP_CPP_SRC := $(filter-out $(CSMAP_MAINS),$(CSMAP_CPP_SRC))  
CSMAP_CPP_SRC := $(filter-out $(CSMAP_STDHDRS),$(CSMAP_CPP_SRC))  
CSMAP_CPP_SRC := $(filter-out $(CSMAP_MFC),$(CSMAP_CPP_SRC))  

# A hard coded list of the 'C' modules required to exist in the library.
CSMAP_SRC_CC = 	CS_alber.c \
	CS_angle.c \
	CS_ansi.c \
	CS_ats77.c \
	CS_azmea.c \
	CS_azmed.c \
	CS_badekas.c \
	CS_bonne.c \
	CS_bpcnc.c \
	CS_bursa.c \
	CS_bynFile.c \
	CS_category.c \
	cs_ctio.c \
	CS_csini.c \
	CS_csio.c \
	CS_csprm.c \
	CS_csWktLoc.c \
	CS_ctio.c \
	CS_datum.c \
	CS_defaults.c \
	CS_defCmp.c \
	CS_defCmpEx.c \
	CS_dtcalc.c \
	CS_dtio.c \
	CS_dtmBridge.c \
	CS_edcnc.c \
	CS_edcyl.c \
	CS_egm96.c \
	CS_ekrt4.c \
	CS_ekrt6.c \
	CS_elCalc.c \
	CS_elio.c \
	CS_erpt.c \
	CS_error.c \
	CS_fips.c \
	CS_frame.c \
	CS_frnch.c \
	CS_gauss.c \
	CS_general.c \
	CS_geocn.c \
	CS_geoct.c \
	CS_geoid96.c \
	CS_geoid99.c \
	CS_geoidHeight.c \
	CS_gissupprt.c \
	CS_gnomc.c \
	CS_gpio.c \
	CS_gridi.c \
	CS_groups.c \
	CS_guiApi.c \
	CS_gxIndex.c \
	CS_gxio.c \
	CS_gxprm.c \
	CS_hlApi.c \
	CS_hmlsn.c \
	CS_hpApi.c \
	CS_japan.c \
	CS_krovk.c \
	CS_lmbrt.c \
	CS_lmtan.c \
	CS_mgrs.c \
	CS_millr.c \
	CS_modpc.c \
	CS_molod.c \
	CS_molwd.c \
	CS_mrcat.c \
	CS_mstro.c \
	CS_mulrg.c \
	CS_nacyl.c \
	CS_nadcn.c \
	CS_nerth.c \
	CS_ntv1.c \
	CS_ntv2.c \
	CS_nullx.c \
	CS_nzlnd.c \
	CS_oblqm.c \
	CS_optional.c \
	CS_ortho.c \
	CS_osgm91.c \
	CS_ost02.c \
	CS_ost97.c \
	CS_ostn02.c \
	CS_ostn97.c \
	CS_ostro.c \
	CS_parm3.c \
	CS_parm4.c \
	CS_parm6.c \
	CS_parm7.c \
	CS_plycn.c \
	CS_pstro.c \
	CS_rlsUpdt.c \
	CS_robin.c \
	CS_sinus.c \
	CS_sstro.c \
	CS_supprt.c \
	CS_swiss.c \
	CS_sys34.c \
	CS_system.c \
	CS_tacyl.c \
	CS_trmer.c \
	CS_trmrs.c \
	CS_units.c \
	CS_unity.c \
	CS_vdgrn.c \
	CS_vertconUS.c \
	CS_vrtcon.c \
	CS_wgs72.c \
	CS_winkelTripel.c \
	CS_zones.c \
	CScscomp.c \
	CSdata.c \
	CSdataDT.c \
	CSdataPJ.c \
	CSdataU.c \
	CSdatumCatalog.c \
	CSdictDiff.c \
	CSdtcomp.c \
	CSelcomp.c \
	CSgeodeticSupport.c \
	CSgpcomp.c \
	CSgxcomp.c \
	CSwktFlavors.c
	

# A hard coded list of the 'C++' modules required to exist in the library.
CSMAP_SRC_CPP = csIoUtil.cpp \
	CS_wellknowntext.cpp \
	CS_csvFileSupport.cpp \
	CScs2Wkt.cpp \
	CSdt2Wkt.cpp \
	CSel2Wkt.cpp \
	CS_epsgStuff.cpp \
	CS_epsgSupport.cpp \
	CS_nameMapper.cpp \
	CS_nameMapperSupport.cpp \
	CS_wktObject.cpp \
	CS_wktObjKonstants.cpp

# The following combines the two above lists and provides a convenient
# place in this file to switch between the wildcard generated list and
# the hard coded lists defined above.

# The following two assignments use the wildcard generated module lists.
CSMAP_LIB_SRC = $(CSMAP_CC_SRC)
CSMAP_LIB_SRC += $(CSMAP_CPP_SRC)

# The following two assignments use the hard coded module lists.
#CSMAP_LIB_SRC = $(CSMAP_SRC_CC)
#CSMAP_LIB_SRC += $(CSMAP_SRC_CPP)

#
# From the selected source list, generate an equivalent list of objects
# with the desired intermediate directory prefix.
#
CSMAP_LIB_OBJ := $(patsubst %.c,%.o,$(CSMAP_LIB_SRC))
CSMAP_LIB_OBJ := $(patsubst %.cpp,%.o,$(CSMAP_LIB_OBJ))
CSMAP_LIB_OBJ := $(addprefix $(INT_DIR)/,$(CSMAP_LIB_OBJ))

#
# OK, we should now be able to do some real work.  This is rather
# simple now.
#
$(OUT_DIR)/$(TRG_NAME) : $(CSMAP_LIB_OBJ)
	ar rv $(OUT_DIR)/$(TRG_NAME) $?

.PHONY : clean
clean :
	rm -f $(INT_DIR)/*.o
	rm -f $(OUT_DIR)/$(TRG_NAME)

rebuild: clean $(OUT_DIR)/$(TRG_NAME)

#
# The following create the directories in which the results are to be
# written if they don't already exist.  The -p option on the 'mkdir'
# command creates all intermediate directories as well; and also inhibits
# an error condition if they already exist.
#
$(CSMAP_LIB_OBJ) : | $(INT_DIR)

$(OUT_DIR)/$(TRG_NAME) : | $(OUT_DIR)

$(INT_DIR) :
	mkdir -p $(INT_DIR)

$(OUT_DIR) :
	mkdir -p $(OUT_DIR)

