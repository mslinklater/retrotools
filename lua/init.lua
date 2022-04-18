-- replace some system functions

-- redirect system 'print' so we can capture it
_print = print
print = myprint

loginfo("LuaVM initialised...")

