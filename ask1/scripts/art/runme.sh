#!/bin/sh
sed 's/config_line_number = 512/config_line_number = '$1'/g' cache-hierarchy_old.simics > cache-hierarchy_buf.simics
sed 's/config_assoc = 4/config_assoc = '$2'/g' cache-hierarchy_buf.simics > cache-hierarchy.simics
sed -i 's/outputfile = .*$/outputfile = ("art-'$1'.'$2'")/g' art.simics
../../simics -stall -x art.simics
