all: run

run: build
	-docker rm autogeoreference.mzk.cz
	docker run -i -t -p 8081:80 --name autogeoreference.mzk.cz -v /home/dudae/tmp:/data autogeoreference.mzk.cz

build:
	docker build -t autogeoreference.mzk.cz .
