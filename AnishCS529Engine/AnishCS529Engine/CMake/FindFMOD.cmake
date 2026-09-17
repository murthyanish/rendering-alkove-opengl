# FindFMOD.cmake — locates a manually-downloaded FMOD Engine (Linux) install.
# Set FMOD_ROOT to the extracted SDK path, e.g. ~/fmod/api/core and ~/fmod/api/studio,
# or export it as an env var: export FMOD_ROOT=/opt/fmod

set(FMOD_ROOT "~/fmod/api")

find_path(FMOD_INCLUDE_DIR fmod.hpp
  HINTS "${FMOD_ROOT}/core/inc" "$ENV{FMOD_ROOT}/core/inc"
)
find_path(FMOD_STUDIO_INCLUDE_DIR fmod_studio.hpp
  HINTS "${FMOD_ROOT}/studio/inc" "$ENV{FMOD_ROOT}/studio/inc"
)
find_library(FMOD_LIBRARY NAMES fmod
  HINTS "${FMOD_ROOT}/core/lib/x86_64" "$ENV{FMOD_ROOT}/core/lib/x86_64"
)
find_library(FMOD_STUDIO_LIBRARY NAMES fmodstudio
  HINTS "${FMOD_ROOT}/studio/lib/x86_64" "$ENV{FMOD_ROOT}/studio/lib/x86_64"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FMOD DEFAULT_MSG
  FMOD_LIBRARY FMOD_INCLUDE_DIR FMOD_STUDIO_LIBRARY FMOD_STUDIO_INCLUDE_DIR
)

if(FMOD_FOUND AND NOT TARGET FMOD::FMOD)
  add_library(FMOD::FMOD SHARED IMPORTED)
  set_target_properties(FMOD::FMOD PROPERTIES
    IMPORTED_LOCATION "${FMOD_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FMOD_INCLUDE_DIR}"
  )
  add_library(FMOD::FMODStudio SHARED IMPORTED)
  set_target_properties(FMOD::FMODStudio PROPERTIES
    IMPORTED_LOCATION "${FMOD_STUDIO_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FMOD_STUDIO_INCLUDE_DIR}"
  )
endif()
