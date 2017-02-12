#include "lua.hpp"

#include <stdexcept>

lua::state::state():
	lua_state_ {luaL_newstate()}
{
	if (lua_state_ == nullptr) {
		throw std::runtime_error("Memory allocation error");
	}
}

lua::state::~state()
{
	lua_close(lua_state_);
}

bool
lua::state::do_file(std::string const & path) const
{
	return luaL_dofile(lua_state_, path.c_str());
}

int
lua::state::get_field(int index, std::string const & key) const
{
	return lua_getfield(lua_state_, index, key.c_str());
}

int
lua::state::get_global(std::string const & name) const
{
	return lua_getglobal(lua_state_, name.c_str());
}

void
lua::state::open_libs() const
{
	luaL_openlibs(lua_state_);
}

void
lua::state::pop(int n) const
{
	lua_pop(lua_state_, n);
}

int
lua::state::to_integer(int index) const
{
	return lua_tointeger(lua_state_, index);
}

double
lua::state::to_number(int index) const
{
	return lua_tonumber(lua_state_, index);
}

std::string
lua::state::to_string(int index) const
{
	return lua_tostring(lua_state_, index);
}
