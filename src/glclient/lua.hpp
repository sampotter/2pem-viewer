#ifndef __LUA_HPP__
#define __LUA_HPP__

#include <string>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace lua {
    struct state {
        state();
        ~state();
        bool do_file(std::string const & path) const;
#ifdef USING_LUA_5_3
        int get_field(int index, std::string const & key) const;
#else
        void get_field(int index, std::string const & key) const;
#endif
#ifdef USING_LUA_5_3
        int get_global(std::string const & name) const;
#else
        void get_global(std::string const & name) const;
#endif
        void open_libs() const;
        void pop(int n) const;
        int to_integer(int index) const;
        double to_number(int index) const;
        std::string to_string(int index) const;
    private:
        lua_State * lua_state_;
    };
}

#endif // __LUA_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
