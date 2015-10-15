function startup()
	print('in startup')
	dofile('script1.lua')
end
		
tmr.alarm(0, 10000, 0, startup)