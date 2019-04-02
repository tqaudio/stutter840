if(SMTG_AAX_SDK_PATH AND SMTG_ADD_VSTGUI)
  include(AddAAXLibrary)
  set(
    base_sources
    ${SDK_ROOT}/public.sdk/source/vst/basewrapper/basewrapper.sdk.cpp
  )
  set(
    Stutter840_aax_sources
    source/aax.cpp
    ../Stutter840/include/buffer.h
    ../Stutter840/include/constants.h
    ../Stutter840/include/controller.h
    ../Stutter840/include/id.h
    ../Stutter840/include/parameter.h
    ../Stutter840/include/processor.h
    ../Stutter840/include/version.h
    ../Stutter840/source/buffer.cpp
    ../Stutter840/source/controller.cpp
    ../Stutter840/source/factory.cpp
    ../Stutter840/source/parameter.cpp
    ../Stutter840/source/processor.cpp
  )
  set(Stutter840_sources ${Stutter840_aax_sources} ${base_sources})
  set(target Stutter840aax)
  smtg_add_aaxplugin(${target} ${Stutter840_sources})
  target_include_directories(${target} PUBLIC ../Stutter840/include)
  target_include_directories(${target} PUBLIC ../Stutter840/source)
  target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
  target_link_libraries(${target} PRIVATE base sdk vstgui_support aaxwrapper)

  smtg_add_vst3_resource(${target} "../Stutter840/resource/main.uidesc")

  smtg_add_vst3_resource(${target} "../Stutter840/image/needle_1x.png")
  smtg_add_vst3_resource(${target} "../Stutter840/image/needle_2x.png")
  smtg_add_vst3_resource(${target} "../Stutter840/image/needle_3x.png")

  if(SMTG_MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/../Stutter840/resource/Info.plist" PREPROCESS)
  elseif(SMTG_WIN)
    target_sources(${target} PRIVATE ../Stutter840/resource/Plugin.rc)
  endif()
endif()
