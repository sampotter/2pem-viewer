if (NOT ${WINDOWS})
  set (SHOULD_USE_OSC 1)
endif ()

configure_file(config.hpp.in config.hpp)

# Local Variables:
# indent-tabs-mode: nil
# End:
