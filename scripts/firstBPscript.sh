#Invoke as follows: simics -stall -x runbenchmark.simics
#Load checkpoint taken after the first breakpoint of the application
read-configuration /home/user/simics-workspace/checkpoints/after-first-break.conf
#We disable breakpoints
disable-magic-breakpoint
instruction-fetch-mode instruction-fetch-trace
istc-disable
dstc-disable
#We do not need to mount host and copy benchmarks to root folder. They are already
#copied to the checkpoint we saved.
#Here is the point right after the first breakpoint.
#Load cache hierarcy parameters
echo "Loading caches"
run-command-file cache-hierarchy.simics
#Continue simulation for 100,000,000 instructions to warm-up caches
echo "Warming up caches"
c 100_000_000
#Reset statistics
dc.reset-statistics
ic.reset-statistics
l2c.reset-statistics
ptime
#Continue simulation for 1,000,000,000 instructions
echo "Simulating..."
c 1000_000_000
#Collect statistics
dc.statistics
ic.statistics
l2c.statistics
ptime
exit
