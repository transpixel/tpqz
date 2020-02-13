# Start image as container

docker run --rm -t -i -d -P \
	--mount="type=bind,source=${HOME}/repos/tpqz,destination=/tpqz" \
	--name=tpqzBuild_gcc-latest \
	--hostname=tpqzBuild \
	transpixel/tpqz:gcc7.3

