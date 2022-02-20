-- replace some system functions
_print = print
print = myprint
_loadfile = loadfile
loadfile = myloadfile

loginfo("LuaVM initialised...")
