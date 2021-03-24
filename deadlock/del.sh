#!/bin/bash

ipcs | grep gabka | cut -d' ' -f2 > deldata 
#sed -i 1d deldata
cat deldata | xargs ipcrm -s
cat deldata | xargs ipcrm -m

rm deldata