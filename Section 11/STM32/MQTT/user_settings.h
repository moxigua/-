


/*操作系统*/
#define __CORTEX_M3__         //STM32F103ZGT6是Cortex-m3内核
#define WOLFSSL_CMSIS_RTOS    //操作系统使用的是CMSIS-RTOS
/*Tcp的发送和接受函数*/
#define WOLFSSL_USER_IO       //我们需要自己定义Tcp发送和接受函数
/*文件系统*/
#define NO_FILESYSTEM         //没有文件系统
/*产生随机数*/
#define NO_DEV_RANDOM
#define WOLFSSL_GENSEED_FORTEST
/*内存分配*/
#define USE_WOLFSSL_MEMORY    //分配内存空间函数
#define WOLFSSL_MALLOC_CHECK  //分配后检查

#define USE_FAST_MATH          //快速计算RSH,DH等算法
#define TFM_TIMING_RESISTANT   //在小堆栈系统中适用，会使用避免使用大量静态阵列

#define NO_WRITEV          //禁用writev()

/*调试*/
//#define DEBUG_WOLFSSL
//#define WOLFSSL_LOG_PRINTF

/*需要用到SHA512加密*/
#define WOLFSSL_SHA512
#define WOLFSSL_SHA384
/*不使用大概会减少3kB内存*/
#define NO_SESSION_CACHE
#define WOLFSSL_SHA256

#define WOLFSSL_STATIC_RSA
#define WOLFSSL_LOW_MEMORY
#define WOLFSSL_

/*自定义时间*/
#define USER_TIME
/*根据百度云给的证书做对应的修改*/
//#define USE_CERT_BUFFERS_2048   //公钥所占的字节数
//#define FP_MAX_BITS  8192       //快速计算时需要定义，默认是4096
#define FP_MAX_BITS  8192       //快速计算时需要定义，默认是4096
#define WOLFSSL_MIN_DHKEY_BITS 512  //DHKEY

