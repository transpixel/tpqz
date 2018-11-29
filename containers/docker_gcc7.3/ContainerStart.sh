# Start image as container

docker run --rm -t -i -d -P \
	--mount="type=bind,source=/home/stcdave/gitTPQZ/tpqz,destination=/tpqz" \
	--name=tpqzBuild_gcc-latest \
	--hostname=tpqzBuild \
	transpixel/tpqz:gcc7.3

