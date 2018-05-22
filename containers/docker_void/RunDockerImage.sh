#!/bin/bash

export DirCode="$(pwd)/../../"
docker run \
	--rm -d -P \
	--mount="type=bind,source=${DirCode}/,destination=/tpqz" \
	--name=tpDev \
	--hostname=tpHost \
	transpixel/void:dev_v1.0.0

