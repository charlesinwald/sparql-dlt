nodes=$1
for (( i = 1; i <= $nodes ; i++ ))
do
  ./sparql_dlt & 2>&1 | tee $i
done;
