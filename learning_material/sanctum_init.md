## init

### 清空 

​	初始化的过程，首先将`mscratch`、`x1`和`x2`中的数值清0。之后会调用`platform`下的`platform_clean_core`和`platform_purge_core`函数。

```assembly
//platform_clean_core
li x3, 0
li x4, 0
li x5, 0
li x6, 0
li x7, 0
li x8, 0
li x9, 0
li x10, 0
li x11, 0
li x12, 0
li x13, 0
...
li x31, 0
ret 
```

​	这样通用寄存器就会被全部清空。目前代码中的`platform_purge_core`为空，之后直接返回。

​	接着初始化过程需要设置堆栈，这个过程如下：

```assembly
la sp, stack_ptr
li t0, STACK_SIZE
csrr t1, mhartid
mul t0, t0, t1
li t1, MENTRY_FRAME_SIZE
sub sp, sp, t0
sub sp, sp, t1

call sm_init

//sm_init should not return
li a0, 0x987329872
call platform_panic 
```

​	其中`stack_ptr`在`stack.S`中是`data`段中的`stack`部分，`STACK_SIZE`为`0x1000`。之后由于是多核的程序，那么会根据`hart`的`id`进行栈地址分配，低`id`高地址。并且每个hart将栈顶的一部分区域用于作为`mentry_frame`，这个部分为`HLS`的64B和`INTEGER_CONTEXT_SIZE`总共`0x100`B组成。之后就会调用`sm`初始化函数进行初始化。

### SM初始化

#### SM状态

```c
typedef struct sm_state_t{
    sm_keys_t keys;
    sm_core_t cores[NUM_CORES];
    sm_region_t regions[NUM_REGIONS];
    hash_t signing_enclave_measurement;
    region_map_t untrusted_regions;
    mailbox_t untrusted_mailboxes[NUM_UNTRUSTED_MAILBOXES];
    platform_lock_t untrusted_state_lock;
} sm_state_t;
```

​	首先是第一个成员，`keys`的类型为`sm_keys_t`，它的定义如下：

```c
typedef struct sm_key_t {
    public_key_t manufacturer_public_key;
    
    uint32_t device_public_key_present;
    public_key_t device_public_key;
    signature_t device_signature;
    
    hash_t software_measurement;
    public_key_t software_public_key;
    secret_key_t software_secret_key;
    signature_t software_signature;
    
    size_t software_measured_bytes;
    uint8_t* software_measured_binary[];
} sm_key_t;
```

​	首先这里的`public_key_t`的是一个长度为32的`byte`型数组，签名`signature_t`是一个长度为64的`byte`型数组，`secret_key`是一个长度为64的`byte`数组。之后测量值相关的部分。直接来是`sm_core_t`:

```c
typedef struct sm_core_t {
    enclave_id_t owner;
    thread_id_t thread;
    uintptr_t hls_ptr;
    
    platform_lock_t lock;
} sm_core_t;
```

​	其中`enclave_id_t`和`thread_id_t`都是一个`long long`类型，同时`platform_lock_t`的格式如下：

```c
typedef struct{
    uint64_t lock_flag;
    uint64_t _pad[7];
} platform_lock_t;
```

```c
typedef struct sm_region_t{
    enclave_id_t owner;
    region_type_t type;
    region_state_t state;
    
    platform_lock_t lock;
} sm_region_t;

typedef enum{
    REGION_TYPE_UNTRUSTED = 0,
    REGION_TYPE_ENCLAVE = 1,
    REGION_TYPE_METADATA = 2,
} region_type_t;

typedef enum{
    REGION_STATE_INVALID = 0,
    REGION_STATE_FREE = 1,
    REGION_STATE_BLOCKED = 2,
    REGION_STATE_OWNED = 3,
} region_type_t;
```

​	下面是锁的获取和释放，可以原子的实现对锁的操作。

```c
#define platform_lock_acquire(lock)({unsigned __tmp;\
					asm volatile("amoswap.w.aq %[result], %[value], (%[address]) \n":[result] "=r"(__tmp):[value]:"r"(1), [address]"r"(&((lock)->lock_flag)))});
					~__tmp; })
#define platform_lock_release(lock)({\
					asm volatile ("amoswap.w.rl x0, x0, (%[address])\n"::[address]"r"(&((lock)->lock_flag)))})
```

​	下面是`DRAM Region id`和地址互相转化的过程。

```c
static inline region_id_t addr_to_region_id(uintptr_t addr){
    return ((addr-RAM_BASE)&REGION_MASK) >> REGION_SHIFT;
}

static inline void* region_id_to_addr(uint64_t region_id){
    return (void*)(RAM_BASE+(region_id << REGION_SHIFT));
}
```

```c
void sm_init(void){
    sm_state_t *sm = get_sm_state_ptr();
    uintptr_t core_id = platform_get_core_id();
    if(core_id == 0){
        for(int i = 0; i < NUM_CORES; i++){
            sm->cores[i].owner = OWNER_UNTRUSTED;
            sm->cores[i].thread = ENCLAVE_THREAD_NONE;
            sm->cores[i].hls_ptr = ((uintptr)&stack_ptr) - (i*STACK_SIZE) - HLS_SIZE;
            unlock_core(i);					//原子地获取锁
        }
        
        for(int i=0; i<NUM_REGIONS; i++){
            bool region_doesnt_include_sm = (uint64_t)region_id_to_addr(i) > (SM_ADDR+SM_LEN);
            if(region_doesnt_include_sm){
                sm->untrusted_regions.flags[i] = true;
            }
            sm->regions[i].owner = region_doesnt_include_sm ? OWNER_UNTRUSTED : OWNER_SM;
            sm->regions[i].type = REGION_TYPE_UNTRUSTED;
            sm->regions[i].state = REGION_STATE_OWNED;
            unlock_region(i);
        }
    }
}
```

​	`get_sm_state_ptr` 直接将`0x80000000`的地址返回作为`sm_state_t` 指针,  

