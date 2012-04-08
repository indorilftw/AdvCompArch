#Invoke as follows: simics -stall -x runbenchmark.simics
#Load checkpoint
read-configuration /home/user/simics-workspace/checkpoints/after-boot.conf
hfs0.root /home/user/
enable-magic-breakpoint
instruction-fetch-mode instruction-fetch-trace
istc-disable
dstc-disable
#Mount host, cp benchmark to root folder, umount host, and start simulation
con0.input "bash\n"
con0.input "mount /host\n"
con0.input "cp /host/simics-workspace/bencmarks/mcf /\n"
con0.input "cp /host/simics-workspace/bencmarks/inp.in.mcf.train /\n"
con0.input "umount /host\n"
con0.input "cd /\n"
con0.input "./mcf inp.in.mcf.train \n"
c
#First breakpoint encountered
#NOTE: you may take checkpoint here for each different application, in order
#to omit the execution of its initialization phase in subsequent simulations (see
#Parartima C). E.g.:
#write-configuration /home/user/simics-workspace/checkpoints/after-first-break.conf
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

