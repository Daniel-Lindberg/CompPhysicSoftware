make clean
make
./test
# Get the current date
date=$(date '+%Y-%m-%d')
# Copy over the current test
cp cppTestFd.xml "RegressionResults/cppTestFd_$(date '+%Y-%m-%d').xml"
second_recent=$(ls -Art RegressionResults/ | tail -n 2 | head -n 1)
most_recent=$(ls -Art RegressionResults/ | tail -n 2 | tail -n 1)
# Run regression against the two most recent files
python Regression_Test.py -f "RegressionResults/$second_recent" -c "RegressionResults/$most_recent"
#gnuplot tmp.gp
#gv tmp.ps
