-- redirect system 'print' so we can capture it
_print = print
print = myprint

-- now setup some core functions
function help(topic)
	print("-- Help --")
	print("")
	if (topic == nil) then
		print("Topics available...")
		print("plap - help about plap")
	end
end
