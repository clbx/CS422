for i in {1..254};
do
	for j in {1..254};
	do
		ping 10.4.$i.$j -c 1 -w 5  >/dev/null && echo "10.4.$i.$j" &
	done
done
