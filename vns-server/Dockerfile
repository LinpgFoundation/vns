# Use an official Ubuntu runtime as a parent image
FROM alpine:latest

# Set the working directory in the container
WORKDIR /app

# Install any needed packages specified in requirements.txt
RUN apk update && apk add --no-cache cmake g++ gcc make

# Copy the current directory contents into the container at /app
COPY ./extern ./extern
COPY ./CMakeLists.txt ./
COPY ./server.cpp ./

# Specify command to build your project
RUN mkdir build

# go to build dir
WORKDIR /app/build

# make file
RUN cmake ..
RUN cmake --build . --config Release

# Run server
CMD ["./vns_server"]

# cp -r ../vns-cpp/ extern/
# docker build -t vns-server .
# docker run --name vns-server -d -p 8181:8181 vns-server