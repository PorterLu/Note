## 用户线程

### 数据结构

```rust
use std::arch::asm;

const DEFAULT_STACK_SIZE: usize = 1024 * 1024 * 2;
const MAX_THREADS: usize = 4;
static mut RUNTIME: usize = 0;
```

​	声明一些全局变量。

```rust
pub struct Runtime {
	threads: Vec<Thread>,
    current: usize,
}

#[derive(PartialEq, Eq, Debug)]
enum State {
    Available,
    Running,
    Ready,
}

struct Thread {
	id: usize,
    stack: Vec<u8>,
    ctx: ThreadContext,
    state: State,
}

#[derive(Debug, Default)]
#[repr(C)]
struct ThreadContext {
    rsp: u64,
    r15: u64,
    r14: u64,
    r13: u64,
    r12: u64,
    rbx: u64,
    rbp: u64,
}
```

​	声明用户态线程所需要的数据结构，由于用户态线程对于内核是不可知的，我们需要一个`Runtime`进行调度，这里`Runtime`中维护了一个`Thread`队列。由于用户态线程除了单独的栈，调用和普通函数一样，我们在保存寄存器状态时，只需要保存`callee`保存寄存器。

### Runtime

​	在运行用户态线程之前，我们需要对运行时进行初始化。

```rust
impl Thread {
    fn new(id: usize) -> Self {
        Thread {
            id, 
            stack: vec![0_u8; DEFAULT_STACK_SIZE],
            ctx: ThreadContext::default(),
            state: State:;Available,
        }
    }
}
impl Runtime {
    pub fn new() -> Self {
        let base_therad = Thread {
            id: 0,
            stack: vec![0_u8; DEFAULT_STACK_SIZE],
            ctx: ThreadContext::default(),
            state: State::Running,
        }
        
        let mut threads = vec![base_thread];
        let mut available_threads: Vec<Thread> = (1..MAX_THREADS)
        								.map(|i| Thread::new(i))
        								.collect();
        threads.append(&mut available_threads);
        
        Runtime {
            threads,
            current: 0,
        }
    }
    
    pub fn init(&self) {
        unsafe {
            let r_ptr: *const Runtime = self;
            RUNTIME = r_ptr as usize;
        }
    }
}
```

​	这里初始化了主线程，这个线程用于调度其他的线程，是一开始默认调度的线程。同时将所有线程模板加入到队列中。运行时就是这个队列和当前线程号组成的。

```rust
impl Runtime {
    #[inline(never)]
    fn t_yield(&mut self) -> bool {
        let mut pos = self.current;
        while self.threads[pos].state != State::Ready {
            pos += 1;
            if pos == self.threads.len() {
                pos = 0;
            }
            if pos == self.current {
                return false;
            }
        }
        
        if self.threads[self.current].state != State::Available {
            self.threads[self.current].state = State::Ready;
        }
        
        self.threads[pos].state = State::Running;
        let old_pos = self.current;
        self.current = pos;
        
        unsafe {
            let old: *mut ThreadContext = &mut self.threads[old_pos].ctx;
            let new: *const ThreadContext = &self.threads[pos].ctx;
            asm!("call switch", in("rdi") old, in("rsi") new, clobber_abi("C"));
        }
        
        self.threads.len() > 0
    }
}
```

​	首先介绍`t_yield`，这个方法会遍历`thread`队列，找到一个状态是`Ready`的线程，我们会将自己的`current`的状态设置为`Ready`。同时将我们选择的线程状态设置为`running`, 之后交换新旧线程的上下文。

```rust
pub fn spawn(&mut self, f: fn()) {
    let available = self
    		.threads
    		.iter_mut()
    		.find(|t| t.state == State:;Available)
    		.expect("no available thread.")
    let size = available.stack.len();
    unsafe {
        let s_ptr = available.stack.as_mut_ptr().offset(size as isize);
        let s_ptr = (s_ptr as usize as !15) as *mut u8;
        std::ptr::write(s_ptr.offset(-16) as *mut u64, guard as u64);
        std::ptr::write(s_ptr.offset(-24) as *mut u64, skip as u64);
        std::ptr::write(s_ptr.offset(-32) as *mut u64, f as u64);
        available.ctx.rsp = s_ptr.offset(-32) as u64;
    }
    available.state = State::Ready;
}
#[naked]
unsafe extern "C" fn skip() {
    asm!("ret", options(noreturn))
}

fn guard() {
    unsafe {
        let rt_ptr = RUNTIME as *mut Runtime;
        (*rt_ptr).t_return();
    };
}
```

​	这里`spawn`会遍历队列，找到一个没有被占位的`Thread`结构体，往线程对应的栈中压入`f`、`skip`、`guard`函数，之后将上下文中的`sp`指针指向`f`的位置，那么这个`Thread`就被创建完成加入到了队列中。

​	当所有的线程创建完毕时，在主线程中会调用`run`方法，

```rust
impl runtime {
    pub fn run(&mut self) -> ! {
        while self.t_yield() {}
        std::process::exit(0);
    }
    fn t_return(&mut self) {
        if self.current != 0 {
            self.threads[self.current].state = State::Available;
            self.t_yield();
        }
    }
}
```

​	`run`中将会不断地调用我们如下分析过的`t_yield`方法。

```rust
#[naked]
#[no_mangle]
unsafe extern "C" fn switch() {
    asm!(
"mov [rdi + 0x00], rsp",
        "mov [rdi + 0x08], r15",
        "mov [rdi + 0x10], r14",
        "mov [rdi + 0x18], r13",
        "mov [rdi + 0x20], r12",
        "mov [rdi + 0x28], rbx",
        "mov [rdi + 0x30], rbp",
        "mov rsp, [rsi + 0x00]",
        "mov r15, [rsi + 0x08]",
        "mov r14, [rsi + 0x10]",
        "mov r13, [rsi + 0x18]",
        "mov r12, [rsi + 0x20]",
        "mov rbx, [rsi + 0x28]",
        "mov rbp, [rsi + 0x30]",
        "ret", options(noreturn)    
    );
}
```

​	如上是`switch`函数，我们需要交换所有的线程中的`callee`保存寄存器。