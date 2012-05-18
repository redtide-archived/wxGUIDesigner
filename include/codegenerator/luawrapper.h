#ifndef WXGD_LUAWRAPPER_H
#define WXGD_LUAWRAPPER_H

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <lua.hpp>

/**
 * Lua variable or argument types
 */
enum LuaType
{
    BOOL,
    INTEGER,
    STRING,
    DOUBLE
};

/**
 * To store the value of a Lua argument or variable
 */
union LuaValue{
    bool BVAL;      /**< Boolean */
    size_t IVAL;    /**< Integer */
    char* SVAL;     /**< String */
    double DVAL;    /**< Double */
};

/**
 * Data structure that defines a Lua function argument
 */
struct LuaArgument
{
    LuaType type;
    LuaValue value;
};

/**
 * To generate exceptions specifically for the Lua wrapper
 */
class LuaException : public std::runtime_error
{
    public:
        LuaException(const std::string& error):runtime_error(error){}
};

/**
 * To store a list of arguments that can be passed to Lua::CallFunction() function.
 */
class LuaFunctionArguments
{
    private:
        std::vector<LuaArgument> m_arguments; /**< Stores the list of arguments */

    public:

        /**
        * Takes care of freeing string arguments from stack.
        */
        ~LuaFunctionArguments();

        /**
        * Adds a boolean argument value.
        */
        void Add(bool value);

        /**
        * Adds an integer argument value.
        */
        void Add(size_t value);

        /**
        * Adds a string argument value.
        */
        void Add(const std::string& value);

        /**
        * Adds a double argument value.
        */
        void Add(double value);

        /**
        * Amount of arguments the object holds
        */
        size_t Count();

        /**
        * Gets an argument data at a specific index position
        */
        LuaArgument operator[](size_t index);
};

/**
 * Wraps the Lua functionality needed by the code generator.
 */
class Lua
{
    private:

        lua_State* m_lua; /**< Points to the initialized Lua engine */
        bool m_isLoaded; /**< Holds the status of the Lua engine */

    public:

        /**
        * Default constructor that sets Lua state as not loaded
        */
        Lua(){m_isLoaded = false;}

        /**
        * Constructor that calls Load()
        */
        Lua(const std::string& file);

        /**
        * Constructor that automatically unloads Lua if initialized.
        */
        ~Lua();

        /**
        * Initializes the Lua engine and setups neccessary resources.
        */
        void Load(const std::string& file);

        /**
        * Uninitializes the Lua engine and free's it's resources
        */
        void Unload();

        /**
        * To call a function stored on the lua script loaded by LoadScript()
        *
        * @return Always return a string since we dont need more.
        */
        std::string CallFunction(std::string functionName, LuaFunctionArguments arguments);

        /**
        * Gets a global variable value
        */
        std::string GetVar(const std::string& name);
};

#endif // WXGD_LUAWRAPPER_H
