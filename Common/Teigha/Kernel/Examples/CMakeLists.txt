add_subdirectory(ExAlloc)

if(NOT WINCE AND NOT WINRT AND NOT ANDROID)
add_subdirectory(SATBrepSample)
add_subdirectory(SATConverter)
add_subdirectory(SATGeomTracer)
if(NOT BORLAND AND MSVC)
add_subdirectory(XmlGLES2View)
endif(NOT BORLAND AND MSVC)
add_subdirectory(DwfxSignatureSample)
endif(NOT WINCE AND NOT WINRT AND NOT ANDROID)

