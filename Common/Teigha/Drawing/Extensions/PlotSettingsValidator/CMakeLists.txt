#
#  PlotSettingsValidator library
#

tdrawing_sources(${TD_PSVALIDATOR_LIB}
    PlotSettingsValidatorModule.cpp
    PlotSettingsValidatorPEImpl.cpp
    PlotSettingsValidatorPEImpl.h
    PlotSettingsValidatorModule.h
	PlotSettingsValidatorCustomMediaPE.h
	PlotSettingsDefs.h
	)
if(ODA_SHARED AND MSVC)		
tdrawing_sources(${TD_PSVALIDATOR_LIB}
    PlotSettingsValidator.rc
	)
endif(ODA_SHARED AND MSVC)

add_definitions(-DPLOTSETTINGSEXPORT_EXPORTS)

include_directories()
				  
tdrawing_tx(${TD_PSVALIDATOR_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB})

tdrawing_project_group(${TD_PSVALIDATOR_LIB} "Extensions")
