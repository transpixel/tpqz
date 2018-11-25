# Start image as container

docker run --rm -t -i -d -P \
	--mount="type=bind,source=/home/stcdave/gitTPQZ/tpqz,destination=/tpqz" \
	--name=tpqzBuild_void-latest \
	--hostname=tpqzBuild \
	transpixel/tpqz:void-latest

