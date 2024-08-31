
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#define CL_TARGET_OPENCL_VERSION 100
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/opencl.h>

typedef struct {
    double x, y, z;
} Point;

// Load binary file.
// Return value must be freed with delete[].
unsigned char * loadBinaryFile(const char *file_name, size_t *size);
void printError(cl_int error);

int main() {

  struct timeval start, end;

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
  cl_int error;
  cl_context context =
      clCreateContext(NULL, 1, &cpu_device, NULL, NULL, &error);
 if(error!=CL_SUCCESS) printError(error);
  // Create a command queue.
  cl_command_queue queue =
      clCreateCommandQueue(context, cpu_device, 0, &error);
  if(error!=CL_SUCCESS) printError(error);

  
  printf("\nwork 1\n");
  // Read FPGA binary
  size_t length=0;
  const char *fileName="lorenz.aocx";
  unsigned char *binary = loadBinaryFile(fileName, &length);
  // Create program

   cl_program program  = clCreateProgramWithBinary( context, num_devices,
                                             &cpu_device,
                                           &length ,
                                            (const unsigned char **)&binary,
                                            NULL, &errorProgram); 
  if(error!=CL_SUCCESS) printError(error);
  printf("\nwork 2\n");
  // Build the program
 
  if (clBuildProgram(program, 0,  NULL, "", NULL, NULL) != CL_SUCCESS) {
    printf("Error in program\n");
  }

  // Create kernel.
  const char *kernel_name = "sieveOfEratosthenes";
  cl_kernel kernel = clCreateKernel(program,  kernel_name, &error);
  if(error!=CL_SUCCESS) printError(error);
  printf("work 3\n");
  // Host side data

  double dt = 0.01; // Time step
  int steps = 1000000;
  Point * listPoint;

  listPoint=(Point *)malloc(sizeof(Point)*steps);

  listPoint[0].x=1.0;
  listPoint[0].y=0.0;
  listPoint[0].z=20.0;

  for(int i=1;i<steps;i++){
    listPoint[i].x=0.0;
    listPoint[i].y=0.0;
    listPoint[i].z=0.0;
  }
  printf("work 4\n");
  
  // Create memory Object
  cl_mem listPoint_hps =
      clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(Point)*steps, listPoint, NULL);
      

  printf("work 5\n");
  // FPGA side data

  printf("work 6\n");

  // Execute kernel
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &listPoint_hps);
  clSetKernelArg(kernel, 1, sizeof(double), &dt);
  clSetKernelArg(kernel, 2, sizeof(int), &steps);


  cl_event kernel_event;

   gettimeofday(&start,NULL);
  if (clEnqueueTask(queue, kernel, 0, NULL, &kernel_event) != CL_SUCCESS) {
    printf("INVALID TASK\n");
  }

  clWaitForEvents(1, &kernel_event);
  clReleaseEvent(kernel_event);

  printf("work 7\n");
  // Read data from FPGA
  clEnqueueReadBuffer(queue, listPoint_hps, CL_TRUE, 0, sizeof(Point)*(steps), listPoint,
                      0, NULL, NULL);

  printf("work 8\n");

  // Print output
  
  gettimeofday(&end,NULL);
    long double time_taken;
     time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - 
                              start.tv_usec)) * 1e-6;
    
  printf("lorenz(%d) execution time= %Lf secs\n", steps, time_taken);
  printf("%d\t%lf\t%lf\t%lf\n",steps, listPoint[steps].x, listPoint[steps].y, listPoint[steps].z);



  clFlush(queue);

  clFinish(queue);

  // Free memory
  clReleaseMemObject(listPoint_hps);
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);

  

  // printf("works....");

  return 0;
}


unsigned char * loadBinaryFile(const char *file_name, size_t *size) {
  // Open the File
  char cwd[512];
  long ftell_size;
  size_t elements_read;
  char fname[50];
  strcpy(fname,file_name);
  getcwd(cwd,sizeof(cwd));
  strcat(cwd,"/");
  strcat( cwd,fname);
  printf("cdw: %s",cwd);
#ifdef _WIN32
  if(fopen_s(&fp, file_name, "rb") != 0) {
    return NULL;
  }
#else
  FILE* fp = fopen(cwd, "rb");
  if(fp == 0) {
    return NULL;
  }
#endif

  // Get the size of the file
  fseek(fp, 0, SEEK_END);
  ftell_size = ftell(fp);
  if (ftell_size < 0) {
    fclose(fp);
    return NULL;
  }
  *size = ftell_size;

  // Allocate space for the binary
  unsigned char *binary = (unsigned char *)malloc(ftell_size* (long) sizeof(char));

  // Go back to the file start
  rewind(fp);

  // Read the file into the binary
  elements_read = fread(binary, *size, 1, fp);
  if(elements_read == 0) {
    fclose(fp);
    return NULL;
  }

  fclose(fp);
  printf("program succesfully read\n");
  return binary;
}

void printError(cl_int error) {
  // Print error message
  switch(error)
  {
    case -1:
      printf("CL_DEVICE_NOT_FOUND ");
      break;
    case -2:
      printf("CL_DEVICE_NOT_AVAILABLE ");
      break;
    case -3:
      printf("CL_COMPILER_NOT_AVAILABLE ");
      break;
    case -4:
      printf("CL_MEM_OBJECT_ALLOCATION_FAILURE ");
      break;
    case -5:
      printf("CL_OUT_OF_RESOURCES ");
      break;
    case -6:
      printf("CL_OUT_OF_HOST_MEMORY ");
      break;
    case -7:
      printf("CL_PROFILING_INFO_NOT_AVAILABLE ");
      break;
    case -8:
      printf("CL_MEM_COPY_OVERLAP ");
      break;
    case -9:
      printf("CL_IMAGE_FORMAT_MISMATCH ");
      break;
    case -10:
      printf("CL_IMAGE_FORMAT_NOT_SUPPORTED ");
      break;
    case -11:
      printf("CL_BUILD_PROGRAM_FAILURE ");
      break;
    case -12:
      printf("CL_MAP_FAILURE ");
      break;
    case -13:
      printf("CL_MISALIGNED_SUB_BUFFER_OFFSET ");
      break;
    case -14:
      printf("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST ");
      break;

    case -30:
      printf("CL_INVALID_VALUE ");
      break;
    case -31:
      printf("CL_INVALID_DEVICE_TYPE ");
      break;
    case -32:
      printf("CL_INVALID_PLATFORM ");
      break;
    case -33:
      printf("CL_INVALID_DEVICE ");
      break;
    case -34:
      printf("CL_INVALID_CONTEXT ");
      break;
    case -35:
      printf("CL_INVALID_QUEUE_PROPERTIES ");
      break;
    case -36:
      printf("CL_INVALID_COMMAND_QUEUE ");
      break;
    case -37:
      printf("CL_INVALID_HOST_PTR ");
      break;
    case -38:
      printf("CL_INVALID_MEM_OBJECT ");
      break;
    case -39:
      printf("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR ");
      break;
    case -40:
      printf("CL_INVALID_IMAGE_SIZE ");
      break;
    case -41:
      printf("CL_INVALID_SAMPLER ");
      break;
    case -42:
      printf("CL_INVALID_BINARY ");
      break;
    case -43:
      printf("CL_INVALID_BUILD_OPTIONS ");
      break;
    case -44:
      printf("CL_INVALID_PROGRAM ");
      break;
    case -45:
      printf("CL_INVALID_PROGRAM_EXECUTABLE ");
      break;
    case -46:
      printf("CL_INVALID_KERNEL_NAME ");
      break;
    case -47:
      printf("CL_INVALID_KERNEL_DEFINITION ");
      break;
    case -48:
      printf("CL_INVALID_KERNEL ");
      break;
    case -49:
      printf("CL_INVALID_ARG_INDEX ");
      break;
    case -50:
      printf("CL_INVALID_ARG_VALUE ");
      break;
    case -51:
      printf("CL_INVALID_ARG_SIZE ");
      break;
    case -52:
      printf("CL_INVALID_KERNEL_ARGS ");
      break;
    case -53:
      printf("CL_INVALID_WORK_DIMENSION ");
      break;
    case -54:
      printf("CL_INVALID_WORK_GROUP_SIZE ");
      break;
    case -55:
      printf("CL_INVALID_WORK_ITEM_SIZE ");
      break;
    case -56:
      printf("CL_INVALID_GLOBAL_OFFSET ");
      break;
    case -57:
      printf("CL_INVALID_EVENT_WAIT_LIST ");
      break;
    case -58:
      printf("CL_INVALID_EVENT ");
      break;
    case -59:
      printf("CL_INVALID_OPERATION ");
      break;
    case -60:
      printf("CL_INVALID_GL_OBJECT ");
      break;
    case -61:
      printf("CL_INVALID_BUFFER_SIZE ");
      break;
    case -62:
      printf("CL_INVALID_MIP_LEVEL ");
      break;
    case -63:
      printf("CL_INVALID_GLOBAL_WORK_SIZE ");
      break;
    default:
      printf("UNRECOGNIZED ERROR CODE (%d)", error);
  }
}