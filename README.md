# openCL-basics
Basic guide to OpenCL. With docker image of the OpenCL intel FPGA SDK  and some examples.

# Command in docker Step by Step

The docker is located on the ***docker-image*** branch in the ***docker-compiler-images*** folder.

To build and run the docker container using the OpenCL Intel FPGA SDK image after cloning the project Open the terminal and go to the folder with the docker image, then follow the next steps:

1.Build an image using a Dockerfile.

```
   docker build . -t <image-name>
  ```
2. Run a container based on the image, with a specified name and network configuration.

```
docker run -td --name <container-name> -net=host <image name> bash
```

3.List all containers to verify their status.
```
docker ps -a
```

4.Copy a file with the OpenCL code from the host to the container.
```
  docker cp <file-to-copy> <container-name>:<address-to-copy-to>
```
  
5. Access Container Shell. This will allow to execute commands directly inside the running container via a Bash shell.
```
  sudo docker exec -ti <container-name> bash
```

# Examples
There are a few OpenCL example applications that you can run:
1. Factorial

  This example calculates the factorial of a number using OpenCL. Implement the factorial logic in a kernel, then compile and run it in the Docker container.

2. Sieve of Eratosthenes

  The Sieve of Eratosthenes is an algorithm for finding all prime numbers up to a specified integer. You can implement this in OpenCL to leverage parallel processing.

3. Sieve of Eratosthenes (NDRANGE)

  This is an optimized version of the Sieve of Eratosthenes using NDRANGE for parallel execution.

4. Matrix Multiplication

  Implement a matrix multiplication algorithm in OpenCL, which is a common example for demonstrating parallel processing capabilities.

5. Lorenz

  This example involves solving the Lorenz system of differential equations using OpenCL.

## Compile OpenCL kernel
Use command 
```
make
``` 

This command will compile the host program according to the `Makefile` in the directory of the example.
## Compile OpenCL kernel
Use:

```
aoc <file-name> -o <output-name> -board=de10_nano_sharedonly -v -report 
```
   - ` <file-name>.cl`: Your OpenCL kernel file.
   - ` <output-name>.aocx`: The output file for the FPGA bitstream.

Note: Before running the `aoc` command, execute `source /etc/profile` to set the environment variables.

