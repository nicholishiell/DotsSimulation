clear
reset
set key off
set border 3

# Add a vertical dotted line at x=0 to show centre (mean) of distribution.
set yzeroaxis

# Each bar is half the (visual) width of its x-range.
bin_width = 25.;
bin_halfWidth = bin_width/2.;

set boxwidth bin_halfWidth absolute
set style fill solid 1.0 noborder

bin_number(x) = floor(x/bin_width)

rounded(x) = bin_width * ( bin_number(x) + 0.5 )

plot 'totalHisto.dat' using (rounded($1)):(1) smooth frequency with boxes

set term png             
set output "totalHisto.png" 
replot
set term xterm



