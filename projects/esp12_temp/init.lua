function startup()
	print('Boot wait period done. Starting up...')
	dofile('script1.lua')
end
		
tmr.alarm(0, 10000, 0, startup)