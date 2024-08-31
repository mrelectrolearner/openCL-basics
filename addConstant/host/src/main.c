
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CL_TARGET_OPENCL_VERSION 100
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/opencl.h>

#define N (10)

int main() {
  // Search for an openCL platform
  cl_platform_id cpu_paltform = NULL;
  if (clGetPlatformIDs(1, &cpu_paltform, NULL) != CL_SUCCESS) {
    printf("Error getting platform\n");
  }

  // Search for an openCL device
  cl_device_id cpu_device = NULL;
  cl_int errorProgram;
  cl_uint num_devices;
  clGetDeviceIDs(cpu_paltform, CL_DEVICE_TYPE_ALL, 1, &cpu_device,
                 &num_devices);

  // Create a context.
  cl_int errorContext;
  cl_context context =
      clCreateContext(NULL, 1, &cpu_device, NULL, NULL, &errorContext);
  if (errorContext != CL_SUCCESS) {
    printf("error context creation");
  }
  // Create a command queue.
  cl_int errorqueueCreation;
  cl_command_queue queue =
      clCreateCommandQueue(context, cpu_device, 0, &errorqueueCreation);
  if (errorqueueCreation != CL_SUCCESS) {
    printf("error queue creation");
  }

  // Read FPGA binary
  size_t length = 0x100000000;
  unsigned char *binary = (unsigned char *)malloc(length);
  FILE *fp = fopen(
      "/home/luis/Documents/openCl/bioinformatics/addConstant/addConstant.cl",
      "rb");
  fread(binary, length, 1, fp);
  fclose(fp);
  printf("%s\n", binary);
  // Create program
  cl_program program = clCreateProgramWithSource(
      context, 1, (const char **)&binary, &length, &errorProgram);
  if (errorProgram == CL_INVALID_BINARY) {
    printf("errorProgram\n");
  }

  // Build the program
  cl_int ret = clBuildProgram(program, 1, &cpu_device, NULL, NULL, NULL);
  printf("ret at %d is %d\n", __LINE__, ret);
  // Create kernel.
  cl_int errorcode;
  cl_kernel kernel = clCreateKernel(program, "vector_add", &errorcode);
  if (CL_INVALID_PROGRAM == errorcode) {
    printf("errorcode\n");
  }

  // Host side data
  int *host_din, *host_dout;
  posix_memalign((void **)(&host_din), 64, sizeof(int) * N);
  posix_memalign((void **)(&host_dout), 64, sizeof(int) * N);

  for (int i = 0; i < N; i++) {
    host_din[i] = i ;
    host_dout[i] = 0;
    printf("host_din[%d]=%d\n", i, host_din[i]);
  }

  // Create memory Object
  cl_mem dev_din =
      clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * N, NULL, NULL);
  cl_mem dev_dout =
      clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * N, NULL, NULL);

  // FPGA side data
  clEnqueueWriteBuffer(queue, dev_din, CL_TRUE, 0, sizeof(int) * N, host_din, 0,
                       NULL, NULL);

  // Execute kernel
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &dev_din);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &dev_dout);

  cl_event kernel_event;

  if (clEnqueueTask(queue, kernel, 0, NULL, &kernel_event) ==
      CL_INVALID_KERNEL) {
    printf("CL_INVALID_KERNEL\n");
  }
  clWaitForEvents(1, &kernel_event);
	clReleaseEvent(kernel_event);

  // Read data from FPGA
  clEnqueueReadBuffer(queue, dev_dout, CL_TRUE, 0, sizeof(int) * N, host_dout,
                      0, NULL, NULL);

  // Print output
  for (int i = 0; i < N; i++)
    printf("din[%d] = %d; dout[%d] = %d\n", i, host_din[i], i, host_dout[i]);

  clFlush(queue);

  clFinish(queue);

  // Free memory
  clReleaseMemObject(dev_din);
  clReleaseMemObject(dev_dout);
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);

  free(host_din);
  free(host_dout);

  //printf("works....");

  return 0;
}