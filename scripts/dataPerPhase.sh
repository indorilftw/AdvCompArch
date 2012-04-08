#initialization actions as in previous examples
#Continue simulation for 100,000,000 instructions to warm-up caches
echo "Warming up caches"
c 100_000_000
$phase = 0
while $phase < 50 {
#write results for current phase at different file
$outputfile = ("output." + "phase"+ $phase)
output-file-start $outputfile
#We need to reset statistics at the beginning of each new phase
#(without of course resetting the cache contents)
dc.reset-statistics
ic.reset-statistics
l2c.reset-statistics
ptime
#Continue simulation for 100,000,000 instructions
echo "Simulating for phase " + $phase
c 100_000_000
#Collect statistics
dc.statistics
ic.statistics
l2c.statistics
ptime
output-file-stop $outputfile
$phase += 1
}
exit
