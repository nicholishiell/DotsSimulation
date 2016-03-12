set size square
 
set terminal jpeg 
#set xrange [0:10]
#set yrange [0:10]

file = '../rawData/Data_vs_Time.dat'

set title "AvgX  verse time"
set output sprintf('avgX_v_time.jpg', file)
plot file u 2:3:1 with points palette
#plot file using 2:3 

set title "AvgY  verse time"
set output sprintf('avgY_v_time.jpg', file)
plot file u 2:4:1 with points palette
#plot file using 2:4 

set title "X std dev verse time"
set output sprintf('stdX_v_time.jpg', file)
plot file u 2:5:1 with points palette
#plot file using 2:5 

set title "Y std dev verse time"
set output sprintf('stdY_v_time.jpg', file)
plot file u 2:6:1 with points palette
#plot file using 2:6 



